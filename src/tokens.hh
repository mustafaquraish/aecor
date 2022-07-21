#pragma once

#include <iostream>
#include <ostream>
#include <string>
#include <string_view>
#include <vector>

#define ENUM_TOKEN_TYPES(F) \
  F(Keyword, "Keyword") \
  F(Identifier, "Identifier") \
  F(OpenParen, "OpenParen") \
  F(CloseParen, "CloseParen") \
  F(Colon, "Colon") \
  F(OpenCurly, "OpenCurly") \
  F(CloseCurly, "CloseCurly") \
  F(IntLiteral, "IntLiteral") \



enum class TokenType {
#define F(name, text) name,
ENUM_TOKEN_TYPES(F)
#undef F
};

struct Location {
  int line;
  int column;

  Location() : line(1), column(1) {}
  Location(int line, int column) : line(line), column(column) {}
};

struct Token {
  TokenType type;
  Location location;
  std::string_view text;
  int int_lit;

  Token() {}

  static Token from_type(TokenType type, Location location);
  static Token from_identifier(std::string_view identifier, Location location);
  static Token from_keyword(std::string_view keyword, Location location);
  static Token from_int_literal(int value, Location location);
};

inline std::ostream &operator<<(std::ostream &os, const Location &loc) {
  os << "test.ae:" << loc.line << ":" << loc.column;
  return os;
}

inline std::ostream &operator<<(std::ostream &os, const Token &tok) {
  os << "Token: ";
  switch (tok.type) {
  #define F(name, keyword) case TokenType::name: os << keyword; break;
  ENUM_TOKEN_TYPES(F)
  #undef F
  }
  os << ", " << tok.location;
  if (!tok.text.empty()) {
    os << ", (" << tok.text << ")";
  }
  return os;
}

std::vector<Token> lex(std::string source);
