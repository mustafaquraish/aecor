#include <errno.h>
#include <string.h>

#include <fstream>
#include <iostream>
#include <ranges>
#include <sstream>
#include <tokens.hh>
#include <utils.hh>

std::string *slurp_file(const char *filename) {
  std::ifstream file(filename);
  if (file.fail()) {
    std::cerr << "Could not open file " << filename << ": " << strerror(errno)
              << std::endl;
    exit(1);
  }
  std::stringstream buffer;
  buffer << file.rdbuf();
  return new std::string(buffer.str());
}

static std::vector<std::string_view> split_lines(std::string_view text) {
  std::vector<std::string_view> lines;
  int start = 0;
  for (int i = 0; i < text.size(); i++) {
    if (text[i] == '\n') {
      lines.push_back(text.substr(start, i - start));
      start = i + 1;
    }
  }
  lines.push_back(text.substr(start, text.size() - start));
  return lines;
}

void error_loc(const Location &loc, std::string_view message,
               std::string_view hint) {
  auto filename = std::string(loc.filename);
  auto file     = slurp_file(filename.c_str());
  auto lines    = split_lines(*file);

  auto min_line = std::max(loc.line - 2, 1);
  auto max_line = std::min(loc.line + 2, (int)lines.size());

  std::cerr << "---------------------------------------------------------------"
               "-------\n";
  std::cerr << loc << ": Error: " << message << std::endl;
  if (!hint.empty()) { std::cerr << "Hint: " << hint << std::endl; }
  std::cerr << "---------------------------------------------------------------"
               "-------\n";
  for (int i = min_line; i <= max_line; i++) {
    fprintf(stderr, "%4d | ", i);
    std::cerr << lines[i - 1] << std::endl;
    if (i == loc.line) {
      std::cerr << "\u001b[31m";
      for (int j = 0; j < loc.column + 6; j++) { std::cerr << " "; }
      std::cerr << "^ " << message << "\u001b[0m" << std::endl;
    }
  }
  std::cerr << "---------------------------------------------------------------"
               "-------\n";
  delete file;
  exit(1);
}