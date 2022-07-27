#include <lexer.hh>
#include <utils.hh>

char Lexer::peek(int offset) {
  if (i + offset >= source.size()) {
    return 0;
  }
  return source[i + offset];
}


std::vector<Token> Lexer::lex() {
  for (i = 0; i < source.length(); ++i) {
    switch (source[i]) {
      case '\t':
      case '\v':
      case '\r':
      case ' ': {
        ++column;
        break;
      }

      case '\n':
        ++line;
        column = 1;
        break;

      case '(': push(TokenType::OpenParen, 1); break;
      case ')': push(TokenType::CloseParen, 1); break;
      case '{': push(TokenType::OpenCurly, 1); break;
      case '}': push(TokenType::CloseCurly, 1); break;
      case ':': push(TokenType::Colon, 1); break;
      case ';': push(TokenType::Semicolon, 1); break;
      case '+': push(TokenType::Plus, 1); break;
      case '-': push(TokenType::Minus, 1); break;

      case '/': {
        // Ignoring comments
        if (peek() == '/') {
          ++i;
          while (i < source.length() && source[i] != '\n') {
            ++i;
          }
          ++line;
          column = 1;
        } else {
          push(TokenType::Slash, 1);
        }
        break;
      }
      case '*': push(TokenType::Star, 1); break;
      case '&': push(TokenType::Ampersand, 1); break;
      case '|': push(TokenType::Line, 1); break;

      default: {
        // kwywords / idents / literals
        if (isdigit(source[i])) {
          int start = i;
          int value = 0;
          while (isdigit(source[i])) {
            value = value * 10 + (source[i] - '0');
            ++i;
          }

          push(Token::from_int_literal(value, location()));
          column += i - start;
          --i;

        } else if (isalpha(source[i]) || source[i] == '_') {
          int start = i;
          while (isalpha(source[i]) || isdigit(source[i]) || source[i] == '_') {
            ++i;
          }

          auto view = source.substr(start, i - start);
          push_name(view);

          column += i - start;
          --i;

        } else {
          std::cerr << location() << ": unexpected character '" << source[i] << "' at index " << i
                    << std::endl;
          std::cerr << HERE << " Location in source" << std::endl;
          exit(1);
        }
      }
    }
  };

  push(TokenType::Eof);
  return tokens;
};
