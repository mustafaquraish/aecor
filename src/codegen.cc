#include <codegen.hh>
#include <utils.hh>

#include "ast.hh"

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

    case ASTType::Address: out << "&"; return;
    case ASTType::Dereference: out << "*"; return;
    case ASTType::Not: out << "!"; return;
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

void CodeGenerator::gen_block(AST *node, int indent) {
  out << "{\n";
  scopes.push_back({});
  for (auto statement : *node->block.statements) {
    gen_statement(statement, indent + 1);
  }

  auto &defers = scopes.back().defers;
  if (defers.size() > 0) {
    out << "\n";
    gen_indent(indent + 1);
    out << "/* defers for block */\n";
    for (int i = defers.size() - 1; i >= 0; i--) {
      gen_statement(defers[i], indent + 1);
    }
  }
  scopes.pop_back();

  gen_indent(indent);
  out << "}";
}

void CodeGenerator::gen_function(AST *node, int indent) {
  out << *node->func_def.return_type << " " << node->func_def.name << "(";

  bool first = true;
  for (auto arg : *node->func_def.params) {
    if (!first) out << ", ";
    first = false;
    out << *arg->type << " " << arg->name;
  }
  out << ") ";
  gen_block(node->func_def.body, indent);
}

void CodeGenerator::gen_struct(AST *node, int indent) {
  auto name = node->struct_def.struct_type->struct_name;

  out << "typedef struct " << name << " " << name << ";\n";
  out << "struct " << name << " {\n";
  for (auto field : *node->struct_def.fields) {
    gen_indent(indent + 1);
    out << *field->type << " " << field->name << ";\n";
  }
  out << "};\n\n";
}

void CodeGenerator::gen_expression(AST *node, int indent) {
  switch (node->type) {
    case ASTType::Not:
    case ASTType::Address:
    case ASTType::Dereference: {
      out << "(";
      gen_op(node->type);
      gen_expression(node->unary.expr, indent);
      out << ")";
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
      gen_expression(node->binary.lhs, indent);
      gen_op(node->type);
      gen_expression(node->binary.rhs, indent);
      out << ")";
      break;
    }
    case ASTType::IntLiteral: out << node->int_literal; break;
    case ASTType::BoolLiteral: out << node->bool_literal; break;
    case ASTType::Var: out << node->var.name; break;
    case ASTType::StringLiteral:
      out << '"' << node->string_literal << '"';
      break;

    case ASTType::Assignment: {
      gen_expression(node->binary.lhs, indent);
      out << " = ";
      gen_expression(node->binary.rhs, indent);
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
        gen_expression(node->call.callee, indent);
      }
      out << "(";
      bool first = true;
      for (auto arg : *node->call.args) {
        if (!first) out << ", ";
        gen_expression(arg, indent);
        if (first && newline_after_first) out << " \"\\n\"";
        first = false;
      }
      out << ")";
      break;
    }

    case ASTType::Member: {
      auto lhs = node->member.lhs;
      gen_expression(lhs, indent);
      out << (node->member.is_pointer ? "->" : ".") << node->member.name;
      break;
    }

    default: {
      cerr << "\n"
           << HERE << " UNHANDLED TYPE IN gen_expression: " << node->type
           << std::endl;
      exit(1);
    }
  }
}

void CodeGenerator::gen_statement(AST *node, int indent) {
  switch (node->type) {
    case ASTType::Block: {
      gen_indent(indent);
      gen_block(node, indent);
      out << "\n";
      break;
    }
    case ASTType::Return: {
      gen_indent(indent);
      out << "return ";
      gen_expression(node->unary.expr, indent);
      out << ";\n";
      break;
    }
    case ASTType::If: {
      gen_indent(indent);
      out << "if (";
      gen_expression(node->if_stmt.cond, indent);
      out << ") ";
      gen_statement(node->if_stmt.body, indent);
      if (node->if_stmt.els) {
        out << " else ";
        gen_statement(node->if_stmt.els, indent);
      }
      break;
    }

    case ASTType::VarDeclaration: {
      gen_indent(indent);
      out << *node->var_decl.var->type << " " << node->var_decl.var->name;
      if (node->var_decl.init) {
        out << " = ";
        gen_expression(node->var_decl.init, indent);
      }
      out << ";\n";
      break;
    }

    case ASTType::While: {
      gen_indent(indent);
      out << "while (";
      gen_expression(node->while_loop.cond, indent);
      out << ") ";
      gen_statement(node->while_loop.body, indent);
      break;
    }

    case ASTType::For: {
      gen_indent(indent);
      out << "for (";
      if (node->for_loop.init) gen_expression(node->for_loop.init, indent);
      out << "; ";
      if (node->for_loop.cond) gen_expression(node->for_loop.cond, indent);
      out << "; ";
      if (node->for_loop.incr) gen_expression(node->for_loop.incr, indent);
      out << ")";
      gen_statement(node->for_loop.body, indent);
      break;
    }

    case ASTType::Defer: {
      scopes.back().defers.push_back(node->unary.expr);
      break;
    }

    default: {
      gen_indent(indent);
      gen_expression(node, indent);
      out << ";\n";
      break;
    }
  }
}

std::string CodeGenerator::gen_program(Program *program) {
  out.clear();
  out << "#include <stdio.h>\n";
  out << "#include <stdbool.h>\n";
  out << "#include <stdint.h>\n";
  out << "#include <stdlib.h>\n\n";
  for (auto structure : program->structs) { gen_struct(structure, 0); }
  for (auto func : program->functions) { gen_function(func, 0); }
  return out.str();
}
