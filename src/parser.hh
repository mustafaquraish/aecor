#pragma once

#include <ast.hh>
#include <locale>
#include <string>
#include <string_view>
#include <tokens.hh>
#include <vector>

using namespace std;

struct Parser {
  vector<Token> tokens;
  int curr = 0;

  AST *parse_program();
  AST *parse_function();
  AST *parse_block();
  AST *parse_statement();
  AST *parse_expr();

  void consume(TokenType token_type);
  void expect(TokenType token_type);

  Parser(vector<Token> tokens) : tokens(tokens) {}
  Token &token() { return tokens[curr]; };
};
