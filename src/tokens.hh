#pragma once

#include <iostream>
#include <ostream>
#include <string>
#include <string_view>
#include <vector>

#define ENUM_KEYWORDS(F)                                                       \
  F(And, "and")                                                                \
  F(Bool, "bool")                                                              \
  F(Def, "def")                                                                \
  F(Defer, "defer")                                                            \
  F(Else, "else")                                                              \
  F(False, "false")                                                            \
  F(For, "for")                                                                \
  F(I32, "i32")                                                                \
  F(If, "if")                                                                  \
  F(Let, "let")                                                                \
  F(Not, "not")                                                                \
  F(Or, "or")                                                                  \
  F(Return, "return")                                                          \
  F(Struct, "struct")                                                          \
  F(True, "true")                                                              \
  F(Void, "void")                                                              \
  F(While, "while")

#define ENUM_TOKEN_TYPES(F)                                                    \
  F(Ampersand, "Ampersand")                                                    \
  F(CloseCurly, "CloseCurly")                                                  \
  F(CloseParen, "CloseParen")                                                  \
  F(Colon, "Colon")                                                            \
  F(ColonColon, "ColonColon")                                                  \
  F(Comma, "Comma")                                                            \
  F(Dot, "Dot")                                                                \
  F(Eof, "Eof")                                                                \
  F(Equals, "Equals")                                                          \
  F(GreaterThan, "GreaterThan")                                                \
  F(Identifier, "Identifier")                                                  \
  F(IntLiteral, "IntLiteral")                                                  \
  F(LessThan, "LessThan")                                                      \
  F(Line, "Line")                                                              \
  F(Minus, "Minus")                                                            \
  F(OpenCurly, "OpenCurly")                                                    \
  F(OpenParen, "OpenParen")                                                    \
  F(Plus, "Plus")                                                              \
  F(Semicolon, "Semicolon")                                                    \
  F(Slash, "Slash")                                                            \
  F(Star, "Star")                                                              \
  F(StringLiteral, "StringLiteral")

#define ENUM_ALL_TOKENS(F)                                                     \
  ENUM_KEYWORDS(F)                                                             \
  ENUM_TOKEN_TYPES(F)

enum class TokenType {
#define F(name, text) name,
  ENUM_ALL_TOKENS(F)
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
  bool newline_before = false;

  Token() {}

  static Token from_type(TokenType type, Location location,
                         std::string_view text = {});
  static Token from_name(std::string_view name, Location location);
  static Token from_int_literal(int value, Location location);
};

inline std::ostream &operator<<(std::ostream &os, const Location &loc) {
  os << loc.filename << ":" << loc.line << ":" << loc.column;
  return os;
}

inline std::ostream &operator<<(std::ostream &os, const TokenType &type) {
  switch (type) {
#define F(name, keyword)                                                       \
  case TokenType::name: os << keyword; break;
    ENUM_ALL_TOKENS(F)
#undef F
  }
  return os;
}

inline std::ostream &operator<<(std::ostream &os, const Token &tok) {
  os << "Token: " << tok.type;
  os << ", " << tok.location;
  if (!tok.text.empty()) { os << ", (" << tok.text << ")"; }
  return os;
}
