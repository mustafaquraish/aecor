#include "tokens.hh"

std::vector<Token> lex(std::string source) {
  std::vector<Token> tokens;
  int line = 1, column = 1;

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
      tokens.push_back(Token::from_type(TokenType::OpenParen, {line, column}));
      ++column;
      break;
    }

    case ')': {
      tokens.push_back(Token::from_type(TokenType::CloseParen, {line, column}));
      ++column;
      break;
    }

    case '{': {
      tokens.push_back(Token::from_type(TokenType::OpenCurly, {line, column}));
      ++column;
      break;
    }

    case '}': {
      tokens.push_back(Token::from_type(TokenType::CloseCurly, {line, column}));
      ++column;
      break;
    }

    case ':': {
      tokens.push_back(Token::from_type(TokenType::Colon, {line, column}));
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
          ++column;
          ++i;
        }
        ++column;

        tokens.push_back(Token::from_int_literal(value, {line, start}));

      } else if (isalpha(c)) {
        int start = i;
        while (isalpha(source[i]) || isdigit(source[i])) {
          ++column;
          ++i;
        }
        ++column;


        auto view = std::string_view(source).substr(start, i - start);
        tokens.push_back(Token::from_identifier(view, {line, start}));

      } else {
        printf("Unknown character: %c at test.ae:%d:%d\n", c, line, column);
        exit(1);
      }
    }
  }
  };
  return tokens;
};
