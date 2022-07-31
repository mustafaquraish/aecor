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
  void dfs_structs(AST *node, vector<AST *> &results,
                   unordered_set<AST *> &generated);
  void check_all_structs(Program *program);

  void check_program(Program *program);

  void check_function(AST *node);
  void check_block(AST *node);
  void check_statement(AST *node);

  void check_struct(AST *node);

  bool check_valid_type(Type *type);
  Type *check_call(AST *node);
  Type *check_expression(AST *node);

  Variable *find_var(std::string_view name);

  void push_scope() { scopes.push_back({}); }
  void pop_scope() { scopes.pop_back(); }

  void push_var(Variable *var, Location loc);
  Variable *get_struct_member(std::string_view struct_name,
                              std::string_view member);

  std::vector<Scope> scopes;
  std::unordered_map<string_view, AST *> functions;
  std::unordered_map<string_view, AST *> structs;
  AST *curr_func = nullptr;
};
