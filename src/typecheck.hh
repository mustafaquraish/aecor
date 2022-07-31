#pragma once

#include <ast.hh>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

struct TypeChecker {
  struct Scope {
    std::unordered_map<string_view, Variable *> variables;
  };

  TypeChecker() {}
  void dfs_structs(StructDef *_struct, vector<StructDef *> &results,
                   unordered_set<StructDef *> &generated);

  void check_all_functions(Program *program);
  void check_all_structs(Program *program);

  void check_program(Program *program);

  void check_block(AST *node);
  void check_statement(AST *node);

  void check_struct(StructDef *node);
  void check_function(FunctionDef *node);

  Type *check_method_call(AST *node);
  Type *check_call(AST *node);
  Type *check_expression(AST *node);

  bool type_is_valid(Type *type);

  Variable *find_var(std::string_view name);

  void push_scope() { scopes.push_back({}); }
  void pop_scope() { scopes.pop_back(); }

  void push_var(Variable *var, Location loc);
  Variable *get_struct_member(std::string_view struct_name,
                              std::string_view member);

  vector<Scope> scopes;
  unordered_map<string_view, FunctionDef *> functions;
  unordered_map<string_view, StructDef *> structs;
  unordered_map<string_view, unordered_map<string_view, FunctionDef *>> methods;
  FunctionDef *curr_func = nullptr;
};
