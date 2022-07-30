#include <codegen.hh>
#include <utils.hh>

void CodeGenerator::gen_indent(int indent) {
  for (int i = 0; i < indent; i++) out << "  ";
}

void CodeGenerator::gen_op(ASTType type) {
  switch (type) {
    case ASTType::And: out << " && "; return;
    case ASTType::Plus: out << " + "; return;
    case ASTType::Minus: out << " - "; return;
    case ASTType::Multiply: out << " * "; return;
    case ASTType::Or: out << " || "; return;
    case ASTType::Divide: out << " / "; return;
    case ASTType::LessThan: out << " < "; return;
    case ASTType::GreaterThan: out << " > "; return;
    default: break;
  }
  cerr << "\n" << HERE << "UNHANDLED TYPE IN gen_op: " << type << std::endl;
  exit(1);
}

bool callee_is(AST *node, std::string_view name) {
  if (node->type != ASTType::Call) return false;
  if (node->call.callee->type != ASTType::Var) return false;
  if (node->call.callee->var.name != name) return false;
  return true;
}

void CodeGenerator::gen(AST *node, int indent) {
  switch (node->type) {
    case ASTType::FunctionDef: {
      gen_indent(indent);
      out << *node->func_def.return_type << " " << node->func_def.name << "(";

      bool first = true;
      for (auto arg : *node->func_def.params) {
        if (!first) out << ", ";
        first = false;
        out << *arg->type << " " << arg->name;
      }
      out << ") ";
      gen(node->func_def.body, indent + 1);
      break;
    }

    case ASTType::Block: {
      out << "{\n";
      for (auto statement : *node->block.statements) {
        gen_indent(indent);
        gen(statement, indent + 1);
        out << ";\n";
      }
      out << "}\n";
      break;
    }

    case ASTType::Return: {
      out << "return ";
      gen(node->unary.expr, indent + 1);
      out << "";
      break;
    }

    case ASTType::And:
    case ASTType::Or:
    case ASTType::LessThan:
    case ASTType::GreaterThan:
    case ASTType::Plus:
    case ASTType::Minus:
    case ASTType::Multiply:
    case ASTType::Divide: {
      out << "(";
      gen(node->binary.lhs, indent + 1);
      gen_op(node->type);
      gen(node->binary.rhs, indent + 1);
      out << ")";
      break;
    }

    case ASTType::IntLiteral: {
      out << node->int_literal;
      break;
    }

    case ASTType::BoolLiteral: {
      out << (node->bool_literal ? "true" : "false");
      break;
    }

    case ASTType::Var: {
      out << node->var.name;
      break;
    }

    case ASTType::If: {
      out << "if (";
      gen(node->if_stmt.cond, indent + 1);
      out << ") ";
      gen(node->if_stmt.body, indent + 1);
      if (node->if_stmt.els) {
        out << " else ";
        gen(node->if_stmt.els, indent + 1);
      }
      break;
    }

    case ASTType::Call: {
      bool newline_after_first = false;
      if (callee_is(node, "print")) {
        out << "printf";
      } else if (callee_is(node, "println")) {
        out << "printf";
        newline_after_first = true;
      } else {
        gen(node->call.callee, indent + 1);
      }
      out << "(";
      bool first = true;
      for (auto arg : *node->call.args) {
        if (!first) out << ", ";
        gen(arg, indent + 1);
        if (first && newline_after_first) out << " \"\\n\"";
        first = false;
      }
      out << ")";
      break;
    }

    case ASTType::StringLiteral: {
      out << '"' << node->string_literal << '"';
      break;
    }

    case ASTType::VarDeclaration: {
      out << *node->var_decl.var->type << " " << node->var_decl.var->name;
      if (node->var_decl.init) {
        out << " = ";
        gen(node->var_decl.init, indent + 1);
      }
      break;
    }

    case ASTType::Assignment: {
      gen(node->binary.lhs, indent + 1);
      out << " = ";
      gen(node->binary.rhs, indent + 1);
      break;
    }

    case ASTType::While: {
      out << "while (";
      gen(node->while_loop.cond, indent + 1);
      out << ") ";
      gen(node->while_loop.body, indent + 1);
      break;
    }

    case ASTType::Not: {
      out << "(!";
      gen(node->unary.expr, indent + 1);
      out << ")";
      break;
    }

    default: {
      cerr << HERE << " UNHANDLED TYPE IN gen: " << node->type << std::endl;
      exit(1);
    }
  }
}

std::string CodeGenerator::generate(AST *node) {
  out.clear();
  out << "#include <stdio.h>\n";
  out << "#include <stdbool.h>\n";
  out << "#include <stdint.h>\n";
  out << "#include <stdlib.h>\n\n";
  for (auto child : *node->block.statements) {
    gen(child, 0);
    out << "\n";
  }
  return out.str();
}