#include "tokens.hh"
#include <ast.hh>
#include <utils.hh>

void print_indent(int indent) {
  for (int i = 0; i < indent; i++) {
    cout << "  ";
  }
}

static void print_ast(AST *node, int indent) {
  switch (node->type) {
  case ASTType::FunctionDef: {
    print_indent(indent);
    cout << *node->func_def.return_type << " " << node->func_def.name << "() ";
    print_ast(node->func_def.body, indent);
    break;
  }

  case ASTType::Block: {
    print_indent(indent);
    cout << "{\n";
    for (auto statement : *node->block.statements) {
      print_ast(statement, indent + 1);
    }
    cout << "}\n";
    break;
  }

  case ASTType::Return: {
    print_indent(indent);
    cout << "return ";
    print_ast(node->unary.expr, indent + 1);
    cout << ";\n";
    break;
  }

  case ASTType::IntLiteral: {
    cout << node->int_literal.value;
    break;
  }

  default: {
    cerr << "\n"
         << HERE << "UNHANDLED TYPE IN print_ast: " << node->type << std::endl;
    exit(1);
  }
  }
};

void print_ast(AST *node) {
  for (auto child : *node->block.statements) {
    print_ast(child, 0);
  }
}
