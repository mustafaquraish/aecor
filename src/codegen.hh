#pragma once

#include <ast.hh>
#include <fstream>

struct CodeGenerator {
  CodeGenerator(const char *filename);
  ~CodeGenerator();

  void generate(AST *node);

 private:
  void gen_op(ASTType type);
  void gen_indent(int indent);
  void gen(AST *node, int indent);

  std::ofstream out;
};