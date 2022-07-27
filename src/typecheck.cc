#include <typecheck.hh>

void TypeChecker::check(AST *node) {
  // TODO: Actually check some stuff...
}

// Stubs
void TypeChecker::check_function(AST *node) {
}
void TypeChecker::check_block(AST *node) {
}
void TypeChecker::check_statement(AST *node) {
}

Type *TypeChecker::check_call(AST *node) {
  return nullptr;
}
Type *TypeChecker::check_expression(AST *node) {
  return nullptr;
}