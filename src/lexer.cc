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
    }
  }
};
