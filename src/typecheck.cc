#include <typecheck.hh>
#include <unordered_set>
#include <utils.hh>

void TypeChecker::check_program(Program *program) {
  check_all_structs(program);
  check_all_functions(program);
}

void TypeChecker::dfs_structs(AST *node, vector<AST *> &results,
                              unordered_set<AST *> &generated) {
  generated.insert(node);

  for (auto field : *node->struct_def.fields) {
    if (!type_is_valid(field->type)) {
      error_loc(field->location, "Type of field is undefined");
    }

    if (field->type->base == BaseType::Struct) {
      auto neighbor_name = field->type->struct_name;
      auto neighbor      = structs[neighbor_name];
      if (generated.count(neighbor) == 0) {
        dfs_structs(neighbor, results, generated);
      }
    }
  }
  results.push_back(node);
}

void TypeChecker::check_all_structs(Program *program) {
  for (auto node : program->structs) {
    auto name = node->struct_def.struct_type->struct_name;

    if (structs.count(name) > 0) {
      error_loc(node->location, "Struct has already been defined");
    }

    structs[name] = node;
  }

  // TODO: Check for loops in the dependency graph, and error
  unordered_set<AST *> generated;
  vector<AST *> results;
  for (auto node : program->structs) {
    if (generated.count(node) == 0) { dfs_structs(node, results, generated); }
  }

  program->structs = results;
};

void TypeChecker::check_all_functions(Program *program) {
  for (auto func : program->functions) {
    auto name = func->func_def.name;
    for (auto param : *func->func_def.params) {
      if (!type_is_valid(param->type))
        error_loc(param->location, "Invalid parameter type");
    }
    if (!type_is_valid(func->func_def.return_type))
      error_loc(func->location, "Invalid return type");

    if (functions.count(name) > 0) {
      error_loc(func->location, "Function is already defined");
    }

    functions[name] = func;
  }

  for (auto func : program->functions) { check_function(func); }
}

Variable *TypeChecker::find_var(std::string_view name) {
  for (int i = scopes.size() - 1; i >= 0; i--) {
    auto &vars = scopes[i].variables;
    if (auto var = vars.find(name); var != vars.end()) { return var->second; }
  }
  return nullptr;
}

Variable *TypeChecker::get_struct_member(std::string_view struct_name,
                                         std::string_view member) {
  auto _struct = structs[struct_name];
  for (auto field : *_struct->struct_def.fields) {
    if (field->name == member) { return field; }
  }
  return nullptr;
}

void TypeChecker::push_var(Variable *var, Location loc) {
  auto &scope = scopes.back();
  if (scope.variables.count(var->name) > 0) {
    error_loc(loc, "Variable is already defined in scope");
  }
  scope.variables[var->name] = var;
}

bool TypeChecker::type_is_valid(Type *type) {
  // TODO: Keep track of defined structs and look them up later.
  switch (type->base) {
    case BaseType::I32:
    case BaseType::Bool:
    case BaseType::Void: return true;
    case BaseType::Pointer: return type_is_valid(type->ptr_to);
    case BaseType::Struct: return structs.count(type->struct_name) > 0;
    default: break;
  }
  cerr << HERE << " UNHANDLED TYPE IN check_valid_type: " << type->base
       << std::endl;
  exit(1);
}

// Stubs
void TypeChecker::check_function(AST *node) {
  auto prev_func = curr_func;
  curr_func      = node;
  push_scope();

  // The types of parameters and return are checked in decl-pass
  for (auto param : *node->func_def.params) { push_var(param, node->location); }

  check_block(node->func_def.body);

  pop_scope();
  curr_func = prev_func;
}

void TypeChecker::check_block(AST *node) {
  push_scope();
  for (auto child : *node->block.statements) check_statement(child);
  pop_scope();
}

void TypeChecker::check_statement(AST *node) {
  switch (node->type) {
    case ASTType::Block: check_block(node); return;
    case ASTType::Return: {
      if (!curr_func) {
        error_loc(node->location, "Return statement outside of function");
      }
      if (curr_func->func_def.return_type->base == BaseType::Void) {
        error_loc(node->location, "Cannot return from void function");
      }
      auto ret_type = check_expression(node->unary.expr);
      if (*ret_type != *curr_func->func_def.return_type) {
        error_loc(node->unary.expr->location,
                  "Return type does not match function return type");
      }
      return;
    }
    case ASTType::VarDeclaration: {
      // TODO: Infer type?
      if (node->var_decl.init) {
        auto init_type = check_expression(node->var_decl.init);
        if (!node->var_decl.var->type) {
          node->var_decl.var->type = init_type;
        } else if (*node->var_decl.var->type != *init_type) {
          error_loc(node->var_decl.init->location,
                    "Variable type does not match initializer type");
        }
      } else {
        if (!node->var_decl.var->type) {
          error_loc(node->var_decl.var->location,
                    "Variable type cannot be inferred, specify explicitly");
        }
        if (!type_is_valid(node->var_decl.var->type)) {
          error_loc(node->var_decl.var->location, "Invalid variable type");
        }
      }
      push_var(node->var_decl.var, node->location);
      return;
    }
    case ASTType::Defer: {
      check_expression(node->unary.expr);
      return;
    }
    case ASTType::While: {
      auto cond_type = check_expression(node->while_loop.cond);
      if (cond_type->base != BaseType::Bool) {
        auto &loc = node->while_loop.cond->location;
        error_loc(loc, "Condition must be boolean");
      }
      check_statement(node->while_loop.body);
      return;
    }
    case ASTType::For: {
      if (node->for_loop.init) check_expression(node->for_loop.init);
      if (node->for_loop.cond) {
        auto cond_type = check_expression(node->for_loop.cond);
        if (cond_type->base != BaseType::Bool) {
          error_loc(node->for_loop.cond->location, "Condition must be boolean");
        }
      }
      if (node->for_loop.incr) check_expression(node->for_loop.incr);
      return;
    }
    case ASTType::If: {
      auto cond_type = check_expression(node->if_stmt.cond);
      if (cond_type->base != BaseType::Bool) {
        error_loc(node->if_stmt.cond->location, "Condition must be boolean");
      }
      check_statement(node->if_stmt.body);
      if (node->if_stmt.els) { check_statement(node->if_stmt.els); }
      return;
    }
    default: check_expression(node); return;
  }
}

Type *TypeChecker::check_call(AST *node) {
  // TODO: Allow expressions evaluating to functions?
  if (node->call.callee->type != ASTType::Var) {
    error_loc(node->call.callee->location,
              "Functions need to explicitly be specified by name.");
  }
  auto &name = node->call.callee->var.name;

  // This is a hack, don't check the types of parameters
  if (name == "print" || name == "println") {
    for (auto arg : *node->call.args) check_expression(arg);
    return new Type(BaseType::Void);
  }

  if (functions.count(name) == 0) {
    error_loc(node->location, "Function not found");
  }
  auto func = functions[name];
  if (func->func_def.params->size() != node->call.args->size()) {
    error_loc(node->location,
              "Number of arguments does not match function signature");
  }
  auto &params = *func->func_def.params;
  for (int i = 0; i < params.size(); i++) {
    auto param    = params[i];
    auto arg      = node->call.args->at(i);
    auto arg_type = check_expression(arg);
    if (*param->type != *arg_type) {
      error_loc(arg->location,
                "Argument type does not match function parameter type");
    }
  }

  return func->func_def.return_type;
}

Type *TypeChecker::check_expression(AST *node) {
  switch (node->type) {
    case ASTType::Call: return check_call(node);
    case ASTType::IntLiteral: return new Type(BaseType::I32);
    case ASTType::BoolLiteral: return new Type(BaseType::Bool);
    // THIS IS AN UGLY HACK, FIX STRINGS PLS
    case ASTType::StringLiteral: return new Type(BaseType::Void);
    case ASTType::Var: {
      auto var = find_var(node->var.name);
      if (var == nullptr) { error_loc(node->location, "Variable not found"); }
      return var->type;
    }
    // TODO: Allow more comlex binary expressions, will eventually need support
    // for pointers
    case ASTType::Plus:
    case ASTType::Minus:
    case ASTType::Multiply:
    case ASTType::Divide: {
      auto lhs_type = check_expression(node->binary.lhs);
      auto rhs_type = check_expression(node->binary.rhs);
      if (lhs_type->base != BaseType::I32 || rhs_type->base != BaseType::I32) {
        error_loc(node->location, "Operands must be integers");
      }
      return new Type(BaseType::I32);
    }

    case ASTType::LessThan:
    case ASTType::GreaterThan: {
      auto lhs_type = check_expression(node->binary.lhs);
      auto rhs_type = check_expression(node->binary.rhs);
      if (lhs_type->base != BaseType::I32 || rhs_type->base != BaseType::I32) {
        error_loc(node->location, "Operands must be integers");
      }
      return new Type(BaseType::Bool);
    }

    case ASTType::And:
    case ASTType::Or: {
      auto lhs_type = check_expression(node->binary.lhs);
      auto rhs_type = check_expression(node->binary.rhs);
      if (lhs_type->base != BaseType::Bool ||
          rhs_type->base != BaseType::Bool) {
        error_loc(node->location, "Operands must be boolean");
      }
      return new Type(BaseType::Bool);
    }

    case ASTType::Not: {
      auto expr_type = check_expression(node->unary.expr);
      if (expr_type->base != BaseType::Bool) {
        error_loc(node->unary.expr->location, "Expression must be boolean");
      }
      return new Type(BaseType::Bool);
    }

    case ASTType::UnaryMinus: {
      auto expr_type = check_expression(node->unary.expr);
      if (expr_type->base != BaseType::I32) {
        error_loc(node->unary.expr->location, "Expression must be a number");
      }
      return new Type(BaseType::I32);
    }

    case ASTType::Address: {
      // TODO: Only allow pointers to l-values, reject literals;
      auto expr_type = check_expression(node->unary.expr);
      return new Type(BaseType::Pointer, expr_type);
    }

    case ASTType::Dereference: {
      auto expr_type = check_expression(node->unary.expr);
      if (expr_type->base != BaseType::Pointer) {
        error_loc(node->unary.expr->location,
                  "Expression must be a pointer-type");
      }
      return expr_type->ptr_to;
    }

    case ASTType::Assignment: {
      auto lhs = check_expression(node->binary.lhs);
      auto rhs = check_expression(node->binary.rhs);
      if (*lhs != *rhs) {
        error_loc(node->location,
                  "Variable type does not match assignment type");
      }
      return lhs;
    }

    case ASTType::Member: {
      auto lhs_type = check_expression(node->member.lhs);
      if (!lhs_type->is_struct_or_ptr()) {
        error_loc(node->member.lhs->location,
                  "LHS of member access must be a (pointer to) struct");
      }
      node->member.is_pointer = (lhs_type->base == BaseType::Pointer);
      auto struct_type        = lhs_type;
      if (lhs_type->base == BaseType::Pointer) {
        struct_type = lhs_type->ptr_to;
      }
      auto field =
          get_struct_member(struct_type->struct_name, node->member.name);
      if (!field) { error_loc(node->location, "Field not found in struct"); }
      return field->type;
    }

    default: break;
  }
  cerr << HERE << " UNHANDLED TYPE IN check_expression: " << node->type
       << std::endl;
  exit(1);
}
