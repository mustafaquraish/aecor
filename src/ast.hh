#pragma once

#include <tokens.hh>
#include <types.hh>

using namespace std;

#define ENUM_AST_TYPES(F)                                                      \
  F(Assignment, "Assignment")                                                  \
  F(Address, "Address")                                                        \
  F(And, "And")                                                                \
  F(Block, "Block")                                                            \
  F(BoolLiteral, "BoolLiteral")                                                \
  F(Call, "Call")                                                              \
  F(Dereference, "Dereference")                                                \
  F(Divide, "Divide")                                                          \
  F(FunctionDef, "FunctionDef")                                                \
  F(GreaterThan, "GreaterThan")                                                \
  F(If, "If")                                                                  \
  F(IntLiteral, "IntLiteral")                                                  \
  F(LessThan, "LessThan")                                                      \
  F(Minus, "Minus")                                                            \
  F(Multiply, "Multiply")                                                      \
  F(Not, "Not")                                                                \
  F(Or, "Or")                                                                  \
  F(Plus, "Plus")                                                              \
  F(Return, "Return")                                                          \
  F(StringLiteral, "StringLiteral")                                            \
  F(Var, "Var")                                                                \
  F(VarDeclaration, "VarDeclaration")                                          \
  F(While, "While")

enum class ASTType {
#define F(name, text) name,
  ENUM_AST_TYPES(F)
#undef F
};

struct Variable {
  string_view name;
  Type *type;
  Location location;
};

struct AST {
  ASTType type;
  Location location;

  union {
    struct {
      string_view name;
      vector<Variable *> *params;
      Type *return_type;
      AST *body;
    } func_def;

    struct {
      vector<AST *> *statements;
    } block;

    struct {
      AST *expr;
    } unary;

    struct {
      AST *lhs;
      AST *rhs;
    } binary;

    struct {
      string_view name;
    } var;

    struct {
      AST *callee;
      vector<AST *> *args;
    } call;

    struct {
      Variable *var;
      AST *init;
    } var_decl;

    struct {
      AST *cond;
      AST *body;
      AST *els;
    } if_stmt;

    struct {
      AST *cond;
      AST *body;
    } while_loop;

    int int_literal;
    string_view string_literal;
    bool bool_literal;
  };

  AST(ASTType type, Location location);
};

inline std::ostream &operator<<(std::ostream &os, const ASTType &type) {
  switch (type) {
#define F(name, keyword)                                                       \
  case ASTType::name: os << keyword; break;
    ENUM_AST_TYPES(F)
#undef F
  }
  return os;
}
