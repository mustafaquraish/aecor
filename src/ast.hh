#pragma once

#include <tokens.hh>
#include <types.hh>
#include <unordered_set>

using namespace std;

#define ENUM_AST_TYPES(F)                                                      \
  F(Assignment, "Assignment")                                                  \
  F(Address, "Address")                                                        \
  F(And, "And")                                                                \
  F(Block, "Block")                                                            \
  F(BoolLiteral, "BoolLiteral")                                                \
  F(BitwiseAnd, "BitwiseAnd")                                                  \
  F(BitwiseOr, "BitwiseOr")                                                    \
  F(Call, "Call")                                                              \
  F(Cast, "Cast")                                                              \
  F(Defer, "Defer")                                                            \
  F(Dereference, "Dereference")                                                \
  F(Divide, "Divide")                                                          \
  F(DivideEquals, "DivideEquals")                                              \
  F(Equals, "Equals")                                                          \
  F(FloatLiteral, "FloatLiteral")                                              \
  F(FormatStringLiteral, "FormatStringLiteral")                                \
  F(For, "For")                                                                \
  F(GreaterThan, "GreaterThan")                                                \
  F(GreaterThanEquals, "GreaterThanEquals")                                    \
  F(If, "If")                                                                  \
  F(IntLiteral, "IntLiteral")                                                  \
  F(LessThan, "LessThan")                                                      \
  F(LessThanEquals, "LessThanEquals")                                          \
  F(Member, "Member")                                                          \
  F(Minus, "Minus")                                                            \
  F(MinusEquals, "MinusEquals")                                                \
  F(Multiply, "Multiply")                                                      \
  F(MultiplyEquals, "MultiplyEquals")                                          \
  F(Not, "Not")                                                                \
  F(NotEquals, "NotEquals")                                                    \
  F(Or, "Or")                                                                  \
  F(Plus, "Plus")                                                              \
  F(PlusEquals, "PlusEquals")                                                  \
  F(Return, "Return")                                                          \
  F(StringLiteral, "StringLiteral")                                            \
  F(UnaryMinus, "UnaryMinus")                                                  \
  F(Var, "Var")                                                                \
  F(VarDeclaration, "VarDeclaration")                                          \
  F(While, "While")

enum class ASTType {
#define F(name, text) name,
  ENUM_AST_TYPES(F)
#undef F
};

struct FunctionDef;

struct Variable {
  string_view name;
  Type *type;
  Location location;

  bool is_extern = false;
  string_view extern_name;

  Variable(string_view name, Type *type, Location location)
      : name(name), type(type), location(location) {}
};

struct AST {
  ASTType type;
  Location location;

  union {
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

      // Filled in during typechecking
      Variable *var = nullptr;

      bool is_function      = false;
      FunctionDef *function = nullptr;
    } var;

    struct {
      string_view format_str;  // Filled in by typechecker
      vector<string_view> *format_parts;
      vector<AST *> *expr_args;
    } format_str;

    struct {
      AST *callee;
      vector<AST *> *args;
    } call;

    struct {
      AST *lhs;
      string_view name;
      bool is_pointer;
      bool is_method;
    } member;

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

    struct {
      AST *init;
      AST *cond;
      AST *incr;

      AST *body;
    } for_loop;

    struct {
      AST *lhs;
      Type *to_type;
    } cast;

    string_view num_literal;
    string_view string_literal;
    bool bool_literal;
  };

  AST(ASTType type, Location location);
};

struct FunctionDef {
  string_view name;
  vector<Variable *> params;
  Type *return_type;
  AST *body;

  bool is_method;
  string_view struct_name;
  Location location;

  bool is_extern;
  string_view extern_name;
  Type *type;

  FunctionDef(Location loc) : location(loc) {}
};

struct StructDef {
  Type *type;
  string_view name;
  vector<Variable *> fields;
  Location location;
  vector<FunctionDef *> methods;

  bool is_extern = false;
  string_view extern_name;

  StructDef(Location loc) : location(loc) {}
};

struct Program {
  vector<FunctionDef *> functions;
  vector<StructDef *> structs;
  vector<AST *> global_vars;

  vector<string_view> c_flags;
  vector<string_view> c_includes;

  void add_included_file(string_view filename);
  bool is_file_included(string_view filename);

  unordered_set<string> included_files;
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
