#include "parser.hh"
#include "tokens.hh"
#include <iostream>

#define UNHANDLED_TYPE()                                                       \
  default: {                                                                   \
    cerr << "Unexpected token: " << token() << " in " << __FUNCTION__          \
         << std::endl;                                                         \
    exit(1);                                                                   \
  }

void Parser::consume(TokenType token_type) {
  if (token().type != token_type) {
    std::cerr << token().location << ": expected " << token_type << " but got "
              << token().type << std::endl;
    exit(1);
  }

  curr++;
};

void Parser::expect(TokenType token_type) {
  if (token().type != token_type) {
    std::cerr << token().location << ": expected " << token_type << " but got "
              << token().type << std::endl;
    exit(1);
  }
};

AST *Parser::parse_function() {
  consume(TokenType::Def);
  expect(TokenType::Identifier);
  auto node = new AST(ASTType::FunctionDef, token().location);
  node->func_def.name = token().text;

  consume(TokenType::Identifier);
  consume(TokenType::OpenParen);
  // TODO: Add arguments
  consume(TokenType::CloseParen);
  consume(TokenType::Colon);
  expect(TokenType::Identifier);
  node->func_def.return_type = token().text;
  node->func_def.body = parse_block();
  return node;
};

AST *Parser::parse_program() {
  auto node = new AST(ASTType::Block, Location(1, 1));

  while (token().type != TokenType::Eof) {
    switch (token().type) {
    case TokenType::Def: {
      auto func = parse_function();
      node->block.statements.push_back(func);
    }
      UNHANDLED_TYPE();
    }
  };
  return node;
};

AST *Parser::parse_block() {
  auto node = new AST(ASTType::Block, token().location);

  consume(TokenType::OpenCurly);

  while (token().type != TokenType::CloseCurly) {
    auto statement = parse_statement();
    node->block.statements.push_back(statement);
  }

  consume(TokenType::CloseCurly);
  return node;
}

AST *Parser::parse_statement() {
  AST *node = nullptr;

  switch (token().type) {
  case TokenType::Return: {
    consume(TokenType::Return);

    node = parse_expr();
  }
    UNHANDLED_TYPE();
  }

  return node;
}

AST *Parser::parse_expr() {
  AST *node = nullptr;

  switch (token().type) {
  case TokenType::IntLiteral: {
    node = new AST(ASTType::IntLiteral, token().location);
    node->int_literal.value = token().int_lit;
    consume(TokenType::IntLiteral);
  }
    UNHANDLED_TYPE();
  }

  return node;
}
