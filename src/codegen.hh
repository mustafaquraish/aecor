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

  // For for/if/while
  void gen_control_body(AST *ast, int indent);

  void gen_block(AST *node, int indent);
  void gen_expression(AST *node, int indent);
  void gen_var_decl(AST *node, int indent);
  void gen_statement(AST *node, int indent);

  void gen_type_and_name(Type *type, string_view name, int indent);
  void gen_global_vars(Program *program);

  void gen_function_name(FunctionDef *func);
  void gen_function_decls(Program *program);
  void gen_function(FunctionDef *node, int indent);

  void gen_enum_value(string_view enum_name, string_view value_name);
  void gen_enum(StructDef *node, int indent);
  void gen_struct_decls(Program *program);
  void gen_struct(StructDef *node, int indent);

  std::stringstream out;
  std::vector<Scope> scopes;
};
