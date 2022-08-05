#include <codegen.hh>
#include <utils.hh>

#include "ast.hh"

void CodeGenerator::gen_indent(int indent) {
  for (int i = 0; i < indent; i++) out << "  ";
}

void CodeGenerator::gen_op(ASTType type) {
  switch (type) {
    case ASTType::And: out << " && "; return;
    case ASTType::Or: out << " || "; return;
    case ASTType::BitwiseOr: out << " | "; return;
    case ASTType::BitwiseAnd: out << " & "; return;

    case ASTType::Plus: out << " + "; return;
    case ASTType::Minus: out << " - "; return;
    case ASTType::Multiply: out << " * "; return;
    case ASTType::Divide: out << " / "; return;
    case ASTType::LessThan: out << " < "; return;
    case ASTType::GreaterThan: out << " > "; return;

    case ASTType::Assignment: out << " = "; return;
    case ASTType::PlusEquals: out << " += "; return;
    case ASTType::MinusEquals: out << " -= "; return;
    case ASTType::MultiplyEquals: out << " *= "; return;
    case ASTType::DivideEquals: out << " /= "; return;
    case ASTType::LessThanEquals: out << " <= "; return;
    case ASTType::GreaterThanEquals: out << " >= "; return;

    case ASTType::Equals: out << " == "; return;
    case ASTType::NotEquals: out << " != "; return;

    case ASTType::Address: out << "&"; return;
    case ASTType::Dereference: out << "*"; return;
    case ASTType::Not: out << "!"; return;
    case ASTType::UnaryMinus: out << "-"; return;
    default: break;
  }
  cerr << "\n" << HERE << " UNHANDLED TYPE IN gen_op: " << type << std::endl;
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

void CodeGenerator::gen_function(FunctionDef *func, int indent) {
  if (func->is_extern) return;

  out << *func->return_type << " ";
  gen_function_name(func);
  out << "(";

  bool first = true;
  for (auto arg : func->params) {
    if (!first) out << ", ";
    first = false;
    gen_type_and_name(arg->type, arg->name, indent);
  }
  out << ") ";
  gen_block(func->body, indent);
  out << "\n\n";
}

void CodeGenerator::gen_struct_decls(Program *program) {
  if (program->structs.size() == 0) return;

  out << "/* struct declarations */\n";
  for (auto _struct : program->structs) {
    if (_struct->is_extern) continue;
    auto name = _struct->is_extern ? _struct->extern_name : _struct->name;
    out << "typedef struct " << name << " " << name << ";\n";
  }
  out << "\n";
}

void CodeGenerator::gen_struct(StructDef *_struct, int indent) {
  if (_struct->is_extern) return;

  auto name = _struct->type->struct_name;
  out << "struct " << name << " {\n";
  for (auto field : _struct->fields) {
    gen_indent(indent + 1);
    gen_type_and_name(field->type, field->name, indent);
    out << ";\n";
  }
  out << "};\n\n";
}

void CodeGenerator::gen_function_name(FunctionDef *func) {
  if (func->is_extern) {
    out << func->extern_name;
  } else if (func->is_method) {
    out << func->struct_name << "__" << func->name;
  } else {
    out << func->name;
  }
}

void CodeGenerator::gen_function_decls(Program *program) {
  if (program->functions.size() == 0) return;

  out << "/* function declarations */\n";
  for (auto func : program->functions) {
    if (func->is_extern) continue;
    out << *func->return_type << " ";
    gen_function_name(func);
    out << "(";

    bool first = true;
    for (auto arg : func->params) {
      if (!first) out << ", ";
      first = false;
      gen_type_and_name(arg->type, arg->name, 0);
    }
    out << ");\n";
  }
  out << "\n";
}

void CodeGenerator::gen_type_and_name(Type *type, string_view name,
                                      int indent) {
  if (type->base == BaseType::Function) {
    if (type->return_type->base == BaseType::Function) {
      error_loc(type->location,
                "Cannot handle function return type from a function");
    }
    out << *type->return_type << "(*" << name << ")(";
    bool first = true;
    for (auto arg : type->arg_types) {
      if (!first) out << ", ";
      first = false;
      out << *arg;
    }
    out << ")";
  } else {
    out << *type << " " << name;
  }
}

void CodeGenerator::gen_expression(AST *node, int indent) {
  switch (node->type) {
    case ASTType::Not:
    case ASTType::UnaryMinus:
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
    case ASTType::BitwiseOr:
    case ASTType::BitwiseAnd:
    case ASTType::NotEquals:
    case ASTType::LessThan:
    case ASTType::GreaterThan:
    case ASTType::LessThanEquals:
    case ASTType::GreaterThanEquals:
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
    case ASTType::Equals: {
      gen_expression(node->binary.lhs, indent);
      gen_op(node->type);
      gen_expression(node->binary.rhs, indent);
      break;
    }
    case ASTType::IntLiteral: out << node->num_literal; break;
    case ASTType::FloatLiteral: out << node->num_literal; break;
    case ASTType::BoolLiteral: out << node->bool_literal; break;
    case ASTType::Var: {
      if (node->var.is_function) {
        // Function
        auto func = node->var.function;
        gen_function_name(func);
      } else {
        // Regular variable
        auto var = node->var.var;
        if (var->is_extern) {
          out << var->extern_name;
        } else {
          out << var->name;
        }
      }
      break;
    }
    case ASTType::StringLiteral:
      out << '"' << node->string_literal << '"';
      break;

    case ASTType::PlusEquals:
    case ASTType::MinusEquals:
    case ASTType::DivideEquals:
    case ASTType::MultiplyEquals:
    case ASTType::Assignment: {
      gen_expression(node->binary.lhs, indent);
      gen_op(node->type);
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

    case ASTType::Cast: {
      out << "((" << *node->cast.to_type << ")";
      gen_expression(node->cast.lhs, indent);
      out << ")";
      break;
    }

    case ASTType::SizeOf: {
      out << "sizeof(" << *node->sizeof_type << ")";
      break;
    }

    case ASTType::FormatStringLiteral: {
      out << "__format_string(\"" << node->string_literal << "\"";
      for (auto arg : *node->format_str.expr_args) {
        out << ", ";
        gen_expression(arg, indent);
      }
      out << ")";
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
      auto var = node->var_decl.var;
      if (var->is_extern) break;

      gen_indent(indent);
      gen_type_and_name(var->type, var->name, indent);
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

void CodeGenerator::gen_global_vars(Program *program) {
  if (program->global_vars.empty()) return;
  out << "/* global variables */\n";
  for (auto var_decl : program->global_vars) { gen_statement(var_decl, 0); }
  out << "\n";
}

std::string CodeGenerator::gen_program(Program *program) {
  out.clear();

  for (auto incl : program->c_includes) out << "#include \"" << incl << "\"\n";
  out << "\n";

  gen_struct_decls(program);
  for (auto structure : program->structs) { gen_struct(structure, 0); }
  gen_function_decls(program);
  gen_global_vars(program);
  for (auto func : program->functions) { gen_function(func, 0); }
  return out.str();
}
