#pragma once

#include <ast.hh>
#include <locale>
#include <string>
#include <string_view>
#include <tokens.hh>
#include <types.hh>
#include <utils.hh>
#include <vector>

using namespace std;

struct Parser {
  vector<Token> tokens;
  int curr = 0;

  Type *parse_type();

  AST *parse_format_string_literal();

  AST *parse_factor(bool in_parens);
  AST *parse_term(bool in_parens);
  AST *parse_additive(bool in_parens);
  AST *parse_bw_and(bool in_parens);
  AST *parse_bw_xor(bool in_parens);
  AST *parse_bw_or(bool in_parens);
  AST *parse_relational(bool in_parens);
  AST *parse_logical_and(bool in_parens);
  AST *parse_logical_or(bool in_parens);
  AST *parse_expression(bool in_parens = false);

  AST *parse_global_var();
  FunctionDef *parse_function();
  StructDef *parse_enum();
  StructDef *parse_struct();
  AST *parse_block();
  AST *parse_statement();

  void include_file(Program *program, string_view filename);
  void parse_use(Program *program);
  void parse_into_program(Program *program);

  void parse_compiler_option(Program *program);

  Program *parse_program();

  Token &consume_impl(TokenType token_type, const char *sloc);
  Token &expect_impl(TokenType token_type, const char *sloc);

  bool consume_if(TokenType token_type);

  bool token_is(TokenType token_type) { return token().type == token_type; }
  void consume_newline_or(TokenType type = TokenType::Semicolon);

  Token &token() { return tokens[curr]; };
  Parser(vector<Token> tokens) : tokens(tokens) {}
};

#define consume(token_type) consume_impl(token_type, HERE)
#define expect(token_type) expect_impl(token_type, HERE)
