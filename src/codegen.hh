#pragma once

#include <ast.hh>
#include <sstream>
#include <vector>

struct CodeGenerator {
  struct Scope {
    std::vector<AST *> defers;
  };

  CodeGenerator() {}

  std::string gen_program(Program *program);

 private:
  void gen_op(ASTType type);
  void gen_indent(int indent);

  void gen_block(AST *node, int indent);
  void gen_expression(AST *node, int indent);
  void gen_statement(AST *node, int indent);
  void gen_function(AST *node, int indent);
  void gen_struct(AST *node, int indent);

  std::stringstream out;
  std::vector<Scope> scopes;
};
