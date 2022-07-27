#pragma once

#include <tokens.hh>
#include <types.hh>

using namespace std;

#define ENUM_AST_TYPES(F)       \
  F(FunctionDef, "FunctionDef") \
  F(Return, "Return")           \
  F(IntLiteral, "IntLiteral")   \
  F(Plus, "Plus")               \
  F(Minus, "Minus")             \
  F(Multiply, "Multiply")       \
  F(Divide, "Divide")           \
  F(Block, "Block")

enum class ASTType {
#define F(name, text) name,
  ENUM_AST_TYPES(F)
#undef F
};

struct AST {
  ASTType type;
  Location location;

  union {
    struct {
      string_view name;
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
      int value;
    } int_literal;
  };

  AST(ASTType type, Location location) : type(type), location(location) {
    if (type == ASTType::Block) { block.statements = new vector<AST *>(); }
  }
};

inline std::ostream &operator<<(std::ostream &os, const ASTType &type) {
  switch (type) {
#define F(name, keyword) \
  case ASTType::name: os << keyword; break;
    ENUM_AST_TYPES(F)
#undef F
  }
  return os;
}
