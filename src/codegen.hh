#pragma once

#include <ast.hh>
#include <sstream>

struct CodeGenerator {
  CodeGenerator() {}

  std::string generate(AST *node);

 private:
  void gen_type(Type *type);
  void gen_op(ASTType type);
  void gen_indent(int indent);
  void gen(AST *node, int indent);

  std::stringstream out;
};