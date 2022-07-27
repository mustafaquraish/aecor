#include <codegen.hh>
#include <utils.hh>

void CodeGenerator::gen_indent(int indent) {
  for (int i = 0; i < indent; i++) out << "  ";
}

void CodeGenerator::gen_op(ASTType type) {
  switch (type) {
    case ASTType::Plus: out << " + "; return;
    case ASTType::Minus: out << " - "; return;
    case ASTType::Multiply: out << " * "; return;
    case ASTType::Divide: out << " / "; return;
    default: break;
  }
  cerr << "\n" << HERE << "UNHANDLED TYPE IN gen_op: " << type << std::endl;
  exit(1);
}

void CodeGenerator::gen(AST *node, int indent) {
  switch (node->type) {
    case ASTType::FunctionDef: {
      gen_indent(indent);
      out << *node->func_def.return_type << " " << node->func_def.name << "() ";
      gen(node->func_def.body, indent);
      break;
    }

    case ASTType::Block: {
      gen_indent(indent);
      out << "{\n";
      for (auto statement : *node->block.statements) {
        gen(statement, indent + 1);
      }
      out << "}\n";
      break;
    }

    case ASTType::Return: {
      gen_indent(indent);
      out << "return ";
      gen(node->unary.expr, indent + 1);
      out << ";\n";
      break;
    }

    case ASTType::Plus:
    case ASTType::Minus:
    case ASTType::Multiply:
    case ASTType::Divide: {
      gen(node->binary.lhs, indent + 1);
      gen_op(node->type);
      gen(node->binary.rhs, indent + 1);
      break;
    }

    case ASTType::IntLiteral: {
      out << node->int_literal.value;
      break;
    }

    default: {
      cerr << HERE << "UNHANDLED TYPE IN gen: " << node->type << std::endl;
      exit(1);
    }
  }
}

std::string CodeGenerator::generate(AST *node) {
  out.clear();
  for (auto child : *node->block.statements) { gen(child, 0); }
  return out.str();
}