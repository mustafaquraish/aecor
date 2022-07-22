#include <fstream>
#include <iostream>
#include <lexer.hh>
#include <parser.hh>
#include <sstream>
#include <stdio.h>

using namespace std;

std::string slurp_file(const char *filename) {
  std::ifstream file(filename);
  std::stringstream buffer;
  buffer << file.rdbuf();
  return buffer.str();
}

int main(int argc, char *argv[]) {
  auto source = slurp_file("test.ae");

  auto lexer = Lexer(source, "test.ae");
  auto tokens = lexer.lex();

  // std::cout << "----- Tokens: ---" << std::endl;
  // for (auto token : tokens) {
  //   cout << token << endl;
  // }
  // std::cout << "-----------------" << std::endl;

  auto parser = Parser(tokens);
  auto program = parser.parse_program();
  print_ast(program);

  return 0;
}
