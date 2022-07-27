#include "tokens.hh"

Token Token::from_type(TokenType type, Location location) {
  Token tok;
  tok.type     = type;
  tok.location = location;
  return tok;
}

Token Token::from_name(std::string_view identifier, Location location) {
  Token tok;
  tok.text     = identifier;
  tok.location = location;

  if (false) {}
#define F(name, text)            \
  else if (identifier == text) { \
    tok.type = TokenType::name;  \
  }
  ENUM_KEYWORDS(F)
#undef F
  else {
    tok.type = TokenType::Identifier;
  }

  return tok;
}

Token Token::from_int_literal(int value, Location location) {
  Token tok;
  tok.type     = TokenType::IntLiteral;
  tok.location = location;
  tok.int_lit  = value;
  return tok;
}
