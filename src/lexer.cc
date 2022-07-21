#include "tokens.hh"
#include <string_view>

std::vector<Token> lex(std::string_view source) {
  std::vector<Token> tokens;
  int line = 1;
  int column = 1;

  auto push_type = [&](TokenType const &type) {
    tokens.push_back(Token::from_type(type, Location(line, column)));
  };

  for (int i = 0; i < source.length(); ++i) {
    char c = source[i];
    switch (c) {
    case '\n': {
      push_type(TokenType::Newline);
      ++line;
      column = 1;
      break;
    }
    case '\t':
    case '\v':
    case '\r':
    case ' ': {
      ++column;
      break;
    }

    case '(': {
      push_type(TokenType::OpenParen);
      ++column;
      break;
    }

    case ')': {
      push_type(TokenType::CloseParen);
      ++column;
      break;
    }

    case '{': {
      push_type(TokenType::OpenCurly);
      ++column;
      break;
    }

    case '}': {
      push_type(TokenType::CloseCurly);
      ++column;
      break;
    }

    case ':': {
      push_type(TokenType::Colon);
      ++column;
      break;
    }

    default: {
      // kwywords / idents / literals
      if (isdigit(c)) {
        int start = i;
        int value = 0;
        while (isdigit(source[i])) {
          value = value * 10 + (source[i] - '0');
          ++i;
        }

        tokens.push_back(
            Token::from_int_literal(value, Location(line, column)));
        column += i - start;
        --i;

      } else if (isalpha(c)) {
        int start = i;
        while (isalpha(source[i]) || isdigit(source[i])) {
          ++i;
        }

        auto view = std::string_view(source).substr(start, i - start);

        if (view == "def") {

          push_type(TokenType::Def);
        } else if (view == "return") {
          push_type(TokenType::Return);
        } else {
          tokens.push_back(
              Token::from_identifier(view, Location(line, column)));
        };

        column += i - start;
        --i;

      } else {
        printf("Unknown character: %c at test.ae:%d:%d\n", c, line, column);
        exit(1);
      }
    }
    }
  };

  push_type(TokenType::Eof);
  return tokens;
};
