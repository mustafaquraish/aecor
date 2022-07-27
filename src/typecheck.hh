#pragma once

#include <ast.hh>
#include <vector>

// Just a stub for now.

struct TypeChecker {
  TypeChecker() {}

  void check(AST *node);

 private:
  void check_function(AST *node);
  void check_block(AST *node);
  void check_statement(AST *node);

  Type *check_call(AST *node);
  Type *check_expression(AST *node);
};