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
  bool seen_newline;
  std::vector<Token> tokens;

  Lexer(std::string_view source, std::string_view filename = "<unknown>")
      : source(source), filename(filename), i(0), line(1), column(1) {}

  Location location(int line_off = -1) {
    if (line_off < 0) line_off = column;
    return Location{filename, line, line_off};
  }

  void push(TokenType type, int length = 0) {
    push(Token::from_type(type, location()));
    column += length;
  }

  void push_name(std::string_view text) {
    push(Token::from_name(text, location()));
  }

  void push(Token token) {
    token.newline_before = seen_newline;
    tokens.push_back(token);
    seen_newline = false;
  }

  char peek(int offset = 1);
  std::vector<Token> lex();
};
