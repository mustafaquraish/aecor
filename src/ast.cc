#include "tokens.hh"
#include <ast.hh>

void print_indent(int indent) {
  for (int i = 0; i < indent; i++) {
    cout << "  ";
  }
}

void print_ast(AST *node, int indent) {
  print_indent(indent);
  switch (node->type) {
  case ASTType::FunctionDef: {
    cout << node->func_def.return_type << " " << node->func_def.name << "()";
    print_ast(node->func_def.body);
  }

  case ASTType::Block: {
    cout << "{\n";
    for (auto statement : node->block.statements) {
      print_ast(statement, indent + 1);
    }
    cout << "}\n";
  }

  case ASTType::Return: {
    print_indent(indent);
    cout << "return ";
    print_ast(node->unary.expr);
    cout << ";\n";
  }

  case ASTType::IntLiteral: {
    cout << node->int_literal.value;
  }

  default: {
    cout << "UNHANDLED TYPE IN print_ast: " << node->type;
  }
  }
};
