#include <lexer.hh>
#include <utils.hh>

char Lexer::peek(int offset) {
  if (i + offset >= source.size()) { return 0; }
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
        seen_newline = true;
        column       = 1;
        break;

      case '(': push(TokenType::OpenParen, 1); break;
      case ')': push(TokenType::CloseParen, 1); break;
      case '{': push(TokenType::OpenCurly, 1); break;
      case '}': push(TokenType::CloseCurly, 1); break;
      case '<': push(TokenType::LessThan, 1); break;
      case '>': push(TokenType::GreaterThan, 1); break;
      case ';': push(TokenType::Semicolon, 1); break;
      case '+': push(TokenType::Plus, 1); break;
      case '-': push(TokenType::Minus, 1); break;
      case '=': push(TokenType::Equals, 1); break;
      case ',': push(TokenType::Comma, 1); break;
      case '.': push(TokenType::Dot, 1); break;

      case ':': {
        if (peek() == ':')
          push(TokenType::ColonColon, 2);
        else
          push(TokenType::Colon, 1);
        break;
      }
      case '/': {
        // Ignoring comments
        if (peek() == '/') {
          ++i;
          while (i < source.length() && source[i] != '\n') { ++i; }
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
        auto loc = location();
        // keywords / idents / literals
        if (isdigit(source[i])) {
          int start = i;
          while (isdigit(source[i])) { ++i; }

          if (source[i] == '.') {
            ++i;
            while (isdigit(source[i])) { ++i; }
            push(Token::from_type(TokenType::FloatLiteral, loc,
                                  source.substr(start, i - start)));
          } else {
            push(Token::from_type(TokenType::IntLiteral, loc,
                                  source.substr(start, i - start)));
          }

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

        } else if (source[i] == '"') {
          int start = i + 1;

          // help, boolean expressions are hard
          while (source[i] == '\\' || peek() != '"') { ++i; }
          ++i;

          auto view = source.substr(start, i - start);
          push(Token::from_type(TokenType::StringLiteral, loc, view));
          column += i - start + 2;

        } else {
          error_loc(loc,
                    format("Unrecognized character '" << source[i] << "'"));
        }
      }
    }
  };

  push(TokenType::Eof);
  return tokens;
};
