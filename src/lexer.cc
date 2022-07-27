#include <lexer.hh>
#include <utils.hh>

std::vector<Token> Lexer::lex() {
  for (int i = 0; i < source.length(); ++i) {
    char c = source[i];
    switch (c) {
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
      case '/': push(TokenType::Slash, 1); break;
      case '*': push(TokenType::Star, 1); break;
      case '&': push(TokenType::Ampersand, 1); break;
      case '|': push(TokenType::Line, 1); break;

      default: {
        // kwywords / idents / literals
        if (isdigit(c)) {
          int start = i;
          int value = 0;
          while (isdigit(source[i])) {
            value = value * 10 + (source[i] - '0');
            ++i;
          }

          push(Token::from_int_literal(value, location()));
          column += i - start;
          --i;

        } else if (isalpha(c) || source[i] == '_') {
          int start = i;
          while (isalpha(source[i]) || isdigit(source[i]) || source[i] == '_') {
            ++i;
          }

          auto view = source.substr(start, i - start);
          push_name(view);

          column += i - start;
          --i;

        } else {
          std::cerr << location() << ": unexpected character '" << c << "'"
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
