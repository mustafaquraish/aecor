#include <stdio.h>

#include <codegen.hh>
#include <fstream>
#include <iostream>
#include <lexer.hh>
#include <parser.hh>
#include <sstream>

using namespace std;

std::string slurp_file(const char *filename) {
  std::ifstream file(filename);
  std::stringstream buffer;
  buffer << file.rdbuf();
  return buffer.str();
}

int main(int argc, char *argv[]) {
  if (argc == 1) {
    cerr << "Usage: " << argv[0] << " <filename.ae>" << endl;
    return 1;
  }

  auto filename = argv[1];
  auto source = slurp_file(filename);

  auto lexer  = Lexer(source, filename);
  auto tokens = lexer.lex();

  auto parser  = Parser(tokens);
  auto program = parser.parse_program();

  auto generator = CodeGenerator();
  auto ccode = generator.generate(program);

  std::ofstream out("test.c");
  out << ccode;
  out.close();

  auto exit_code = system("gcc test.c -o test");
  if (exit_code != 0) {
    std::cerr << "[-] Compilation failed" << std::endl;
    return exit_code;
  }

  return 0;
}
