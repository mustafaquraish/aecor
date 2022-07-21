#include "tokens.hh"

std::vector<Token> lex(std::string source) {
  std::vector<Token> tokens;
  int line = 1;
  int column = 1;

  for (int i = 0; i < source.length(); ++i) {
    char c = source[i];
    switch (c) {
    case '\n': {
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
      tokens.push_back(Token::from_type(TokenType::OpenParen, Location(line, column)));
      ++column;
      break;
    }

    case ')': {
      tokens.push_back(Token::from_type(TokenType::CloseParen, Location(line, column)));
      ++column;
      break;
    }

    case '{': {
      tokens.push_back(Token::from_type(TokenType::OpenCurly, Location(line, column)));
      ++column;
      break;
    }

    case '}': {
      tokens.push_back(Token::from_type(TokenType::CloseCurly, Location(line, column)));
      ++column;
      break;
    }

    case ':': {
      tokens.push_back(Token::from_type(TokenType::Colon, Location(line, column)));
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

        tokens.push_back(Token::from_int_literal(value, Location(line, column)));
        column += i - start;
        --i;

      } else if (isalpha(c)) {
        int start = i;
        while (isalpha(source[i]) || isdigit(source[i])) {
          ++i;
        }

        auto view = std::string_view(source).substr(start, i - start);
        tokens.push_back(Token::from_identifier(view, Location(line, column)));
        column += i - start;
        --i;


      } else {
        printf("Unknown character: %c at test.ae:%d:%d\n", c, line, column);
        exit(1);
      }
    }
  }
  };
  return tokens;
};
