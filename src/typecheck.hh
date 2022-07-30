#pragma once

#include <ast.hh>
#include <unordered_map>
#include <vector>

using namespace std;

struct Scope {
  std::unordered_map<string_view, Variable *> variables;
};

struct TypeChecker {
  TypeChecker() {}

  void check(AST *node);

 private:
  void check_function(AST *node);
  void check_block(AST *node);
  void check_statement(AST *node);

  bool check_valid_type(Type *type);
  Type *check_call(AST *node);
  Type *check_expression(AST *node);

  Variable *find_var(std::string_view name);

  void push_scope() { scopes.push_back({}); }
  void pop_scope() { scopes.pop_back(); }

  void push_var(Variable *var, Location loc);

  std::vector<Scope> scopes;
  std::unordered_map<string_view, AST *> functions;
  AST *curr_func = nullptr;
};