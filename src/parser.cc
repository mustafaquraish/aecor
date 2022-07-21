#include "parser.hh"
#include "tokens.hh"
#include <iostream>

#define UNHANDLED_TYPE()                                                       \
    cerr << "Unexpected token: " << token() << " in " << __FUNCTION__          \
         << std::endl;                                                         \
    exit(1);

Token &Parser::consume_impl(TokenType token_type, const char *sloc) {
  auto &ret = expect_impl(token_type, sloc);
  curr++;
  return ret;
};

Token &Parser::expect_impl(TokenType token_type, const char *sloc) {
  if (token().type != token_type) {
    std::cerr << token().location << ": expected " << token_type << " but got "
              << token().type << std::endl;
    std::cerr << sloc << "Location in compiler" << std::endl;
    exit(1);
  }
  return token();
};

AST *Parser::parse_function() {
  auto node = new AST(ASTType::FunctionDef, token().location);
  consume(TokenType::Def);

  auto name = consume(TokenType::Identifier);
  node->func_def.name = name.text;

  consume(TokenType::OpenParen);
  // TODO: Add arguments
  consume(TokenType::CloseParen);
  consume(TokenType::Colon);
  auto ret_type = consume(TokenType::Identifier);
  node->func_def.return_type = ret_type.text;

  node->func_def.body = parse_block();
  return node;
};

AST *Parser::parse_program() {
  auto node = new AST(ASTType::Block, {{}, {}, {}});
  node->block.statements = new vector<AST *>();

  while (token().type != TokenType::Eof) {
    switch (token().type) {
    case TokenType::Def: {
      auto func = parse_function();
      node->block.statements->push_back(func);
      break;
    }

    default: UNHANDLED_TYPE();
    }
  };
  return node;
};

AST *Parser::parse_block() {
  auto node = new AST(ASTType::Block, token().location);
  node->block.statements = new vector<AST *>();

  consume(TokenType::OpenCurly);

  while (token().type != TokenType::CloseCurly) {
    auto statement = parse_statement();
    node->block.statements->push_back(statement);
  }

  consume(TokenType::CloseCurly);
  return node;
}

AST *Parser::parse_statement() {
  AST *node = nullptr;

  switch (token().type) {
  case TokenType::Return: {
    node = new AST(ASTType::Return, token().location);
    consume(TokenType::Return);
    node->unary.expr = parse_expr();
    consume(TokenType::Semicolon);
    break;
  }
  default: UNHANDLED_TYPE();
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
    break;
  }
  default: UNHANDLED_TYPE();
  }

  return node;
}
