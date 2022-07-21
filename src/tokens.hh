#pragma once

#include <iostream>
#include <ostream>
#include <string>
#include <string_view>
#include <vector>

enum class TokenType {
  Keyword,
  Identifier,
  OpenParen,
  CloseParen,
  Colon,
  OpenCurly,
  CloseCurly,
  IntLiteral,
};

struct Location {
  int line;
  int column;
};

struct Token {
  TokenType type;
  Location location;
  std::string_view text;
  int int_lit;

  static Token from_type(TokenType type, Location location);
  static Token from_identifier(std::string_view identifier, Location location);
  static Token from_keyword(std::string_view keyword, Location location);
  static Token from_int_literal(std::string_view keyword, Location location);
};

inline std::ostream &operator<<(std::ostream &os, const Token &tok) {
  os << "[";
  os << "  " << tok.type;
  os << "]";
}

std::vector<Token> lex(std::string source);
