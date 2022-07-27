#pragma once

#include <tokens.hh>
#include <types.hh>

using namespace std;

#define ENUM_AST_TYPES(F)                                                      \
  F(FunctionDef, "FunctionDef")                                                \
  F(Return, "Return")                                                          \
  F(IntLiteral, "IntLiteral")                                                  \
  F(StringLiteral, "StringLiteral")                                            \
  F(BoolLiteral, "BoolLiteral")                                                \
  F(If, "If")                                                                  \
  F(VarDeclaration, "VarDeclaration")                                          \
  F(Assignment, "Assignment")                                          \
  F(Plus, "Plus")                                                              \
  F(Minus, "Minus")                                                            \
  F(Multiply, "Multiply")                                                      \
  F(Divide, "Divide")                                                          \
  F(Var, "Var")                                                                \
  F(Call, "Call")                                                              \
  F(Block, "Block")

enum class ASTType {
#define F(name, text) name,
  ENUM_AST_TYPES(F)
#undef F
};

struct Variable {
  string_view name;
  Type *type;
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
