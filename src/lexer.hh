#pragma once

#include <string_view>
#include <tokens.hh>
#include <vector>

struct Lexer {
  std::string_view source;
  std::string_view filename;
  int i;
  int line;
  int column;
  std::vector<Token> tokens;

  Lexer(std::string_view source, std::string_view filename = "<unknown>")
      : source(source), filename(filename), i(0), line(1), column(1) {}

  Location location() { return Location{filename, line, column}; }

  void push(TokenType type, int length = 0) {
    tokens.push_back(Token::from_type(type, location()));
    column += length;
  }

  void push_name(std::string_view text) {
    tokens.push_back(Token::from_name(text, location()));
  }

  void push(Token token) { tokens.push_back(token); }

  char peek(int offset = 1);
  std::vector<Token> lex();
};
