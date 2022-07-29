#include <typecheck.hh>
#include <utils.hh>

void TypeChecker::check(AST *node) {
  for (auto child : *node->block.statements) {
    switch (child->type) {
      case ASTType::FunctionDef: check_function(child); break;
      default: {
        cerr << "\n" << HERE << " UNHANDLED TYPE IN check: " << child->type << std::endl;
        exit(1);
      }
    }
  }
}

Variable *TypeChecker::find_var(std::string_view name) {
  for (int i = scopes.size() - 1; i >= 0; i--) {
    auto &vars = scopes[i].variables;
    if (auto var = vars.find(name); var != vars.end()) {
      return var->second;
    }
  }
  return nullptr;
}


void TypeChecker::push_var(Variable *var, Location loc) {
  auto &scope = scopes.back();
  if (scope.variables.count(var->name) > 0) {
    cerr << loc << ": Variable " << var->name << " already defined in scope" << std::endl;
    exit(1);
  }
  scope.variables[var->name] = var;
}

bool TypeChecker::check_valid_type(Type *type) {
  // TODO: Keep track of defined structs and look them up later.
  switch (type->base) {
    case BaseType::I32:
    case BaseType::Bool:
    case BaseType::Void:
      return true;
    default: {
      cerr << "\n" << HERE << " UNHANDLED TYPE IN check_valid_type: " << type->base << std::endl;
      exit(1);
    }
  }
}

void TypeChecker::error(AST *node, const char *msg) {
  cerr << node->location << ": " << msg << endl;
  exit(1);
}

// Stubs
void TypeChecker::check_function(AST *node) {
  auto prev_func = curr_func;
  curr_func = node;
  functions[node->func_def.name] = node;
  push_scope();

  for (auto param : *node->func_def.params) {
    check_valid_type(param->type);
    push_var(param, node->location);
  }
  check_valid_type(node->func_def.return_type);

  check_block(node->func_def.body);

  pop_scope();
  curr_func = prev_func;
}

void TypeChecker::check_block(AST *node) {
  push_scope();
  for (auto child : *node->block.statements)
    check_statement(child);
  pop_scope();
}

void TypeChecker::check_statement(AST *node) {
  switch (node->type) {
    case ASTType::Block: check_block(node); return;
    case ASTType::Return: {
      if (!curr_func) {
        error(node, "Return statement outside of function");
      }
      if (curr_func->func_def.return_type->base == BaseType::Void) {
        error(node, "Cannot return from void function");
      }
      auto ret_type = check_expression(node->unary.expr);
      if (*ret_type != *curr_func->func_def.return_type) {
        error(node, "Return type does not match function return type");
      }
      return;
    }
    case ASTType::VarDeclaration: {
      // TODO: Infer type?
      if (!node->var_decl.var->type) {
        error(node, "Need to specify variable type");
      }
      check_valid_type(node->var_decl.var->type);
      if (node->var_decl.init) {
        auto init_type = check_expression(node->var_decl.init);
        if (*init_type != *node->var_decl.var->type) {
          error(node, "Variable type does not match initializer type");
        }
      }
      push_var(node->var_decl.var, node->location);
      return;
    }
    default: check_expression(node); return;
  }
}

Type *TypeChecker::check_call(AST *node) {
  assert(false);
  return nullptr;
}

Type *TypeChecker::check_expression(AST *node) {
  switch (node->type) {
    case ASTType::Call: return check_call(node);
    case ASTType::IntLiteral: return new Type(BaseType::I32);
    case ASTType::BoolLiteral: return new Type(BaseType::Bool);
    case ASTType::Var: {
      auto var = find_var(node->var.name);
      if (var == nullptr) {
        error(node, "Variable not found");
      }
      return var->type;
    }
    default: break;
  }
  cerr << "\n" << HERE << " UNHANDLED TYPE IN check_expression: " << node->type << std::endl;
  exit(1);
}