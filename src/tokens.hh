#pragma once

#include <iostream>
#include <ostream>
#include <string>
#include <string_view>
#include <vector>

#define ENUM_TOKEN_TYPES(F)                                                    \
  F(Def, "Def")                                                                \
  F(Return, "Return")                                                          \
                                                                               \
  F(Identifier, "Identifier")                                                  \
  F(OpenParen, "OpenParen")                                                    \
  F(CloseParen, "CloseParen")                                                  \
  F(Colon, "Colon")                                                            \
  F(Semicolon, "Semicolon")                                                            \
  F(OpenCurly, "OpenCurly")                                                    \
  F(CloseCurly, "CloseCurly")                                                  \
  F(IntLiteral, "IntLiteral")                                                  \
                                                                               \
  F(Eof, "Eof")

enum class TokenType {
#define F(name, text) name,
  ENUM_TOKEN_TYPES(F)
#undef F
};

struct Location {
  std::string_view filename;
  int line;
  int column;
};

struct Token {
  TokenType type;
  Location location;
  std::string_view text;
  int int_lit;

  Token() {}

  static Token from_type(TokenType type, Location location);
  static Token from_identifier(std::string_view identifier, Location location);
  static Token from_int_literal(int value, Location location);
};

inline std::ostream &operator<<(std::ostream &os, const Location &loc) {
  os << loc.filename << ":" << loc.line << ":" << loc.column;
  return os;
}

inline std::ostream &operator<<(std::ostream &os, const TokenType &type) {
  switch (type) {
#define F(name, keyword)                                                       \
  case TokenType::name:                                                        \
    os << keyword;                                                             \
    break;
    ENUM_TOKEN_TYPES(F)
#undef F
  }
  return os;
}

inline std::ostream &operator<<(std::ostream &os, const Token &tok) {
  os << "Token: " << tok.type;
  os << ", " << tok.location;
  if (!tok.text.empty()) {
    os << ", (" << tok.text << ")";
  }
  return os;
}

