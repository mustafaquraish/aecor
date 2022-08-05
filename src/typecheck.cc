#include <typecheck.hh>
#include <unordered_set>
#include <utils.hh>

void TypeChecker::check_program(Program *program) {
  check_all_structs(program);

  push_scope();
  for (auto &global_var : program->global_vars) check_statement(global_var);
  check_all_functions(program);
  pop_scope();
}

void TypeChecker::dfs_structs(StructDef *_struct, vector<StructDef *> &results,
                              unordered_set<StructDef *> &generated) {
  generated.insert(_struct);

  for (auto field : _struct->fields) {
    if (!type_is_valid(field->type)) {
      error_loc(field->type->location, "Type of field is undefined");
    }

    // Don't need to ensure dependency order for externs
    if (_struct->is_extern) continue;

    if (field->type->base == BaseType::Struct) {
      auto neighbor_name = field->type->struct_name;
      auto neighbor      = structs[neighbor_name];
      if (generated.count(neighbor) == 0) {
        dfs_structs(neighbor, results, generated);
      }
    }
  }
  results.push_back(_struct);
}

void TypeChecker::check_all_structs(Program *program) {
  for (auto _struct : program->structs) {
    auto name = _struct->type->struct_name;

    if (structs.count(name) > 0) {
      error_loc(_struct->location, "Struct has already been defined");
    }

    structs[name] = _struct;
    methods[name] = {};
  }

  // TODO: Check for loops in the dependency graph, and error
  unordered_set<StructDef *> generated;
  vector<StructDef *> results;
  for (auto node : program->structs) {
    if (generated.count(node) == 0) { dfs_structs(node, results, generated); }
  }

  program->structs = results;
};

void TypeChecker::check_all_functions(Program *program) {
  for (auto func : program->functions) {
    auto name        = func->name;
    auto struct_name = func->struct_name;
    Type *func_type  = nullptr;

    if (func->is_method) {
      if (structs.count(struct_name) == 0) {
        error_loc(func->location, "Struct for method does not exist");
      }
      if (methods[struct_name].count(name) > 0) {
        error_loc(func->location, "Method is already defined in struct");
      }
      if (auto var = get_struct_member(struct_name, name); var != nullptr) {
        error_loc(func->location, "Struct already has a field with this name");
      }

      func_type              = new Type(BaseType::Method, func->location);
      func_type->struct_name = struct_name;
    } else {
      func_type = new Type(BaseType::Function, func->location);

      if (functions.count(name) > 0) {
        error_loc(func->location, "Function is already defined");
      }
    }
    func_type->func_def = func;

    for (auto param : func->params) {
      if (!type_is_valid(param->type))
        error_loc(param->type->location, "Invalid parameter type");
      func_type->arg_types.push_back(param->type);
    }
    if (!type_is_valid(func->return_type))
      error_loc(func->return_type->location, "Invalid return type");
    func_type->return_type = func->return_type;
    func->type             = func_type;

    if (func->is_method) {
      methods[struct_name][name] = func;
    } else {
      functions[name] = func;
    }
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
  for (auto field : _struct->fields) {
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
    case BaseType::Char:
    case BaseType::I32:
    case BaseType::F32:
    case BaseType::Bool:
    case BaseType::Void:
    case BaseType::U8: return true;
    case BaseType::Pointer: return type_is_valid(type->ptr_to);
    case BaseType::Function: {
      for (auto arg_type : type->arg_types) {
        if (!type_is_valid(arg_type)) return false;
      }
      return type_is_valid(type->return_type);
    }
    case BaseType::Struct: {
      if (auto s = structs.find(type->struct_name); s != structs.end()) {
        type->struct_def = s->second;
        return true;
      }
      return false;
    }
    default: break;
  }
  cerr << HERE << " UNHANDLED TYPE IN type_is_valid: " << *type << std::endl;
  exit(1);
}

// Stubs
void TypeChecker::check_function(FunctionDef *func) {
  auto prev_func = curr_func;
  curr_func      = func;
  push_scope();

  // The types of parameters and return are checked in decl-pass
  for (auto param : func->params) { push_var(param, func->location); }

  if (func->body) { check_block(func->body); }

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
      if (curr_func->return_type->base == BaseType::Void) {
        error_loc(node->location, "Cannot return from void function");
      }
      auto ret_type = check_expression(node->unary.expr);
      if (*ret_type != *curr_func->return_type) {
        error_loc(node->unary.expr->location,
                  "Return type does not match function return type");
      }
      return;
    }
    case ASTType::VarDeclaration: {
      // TODO: Infer type?
      if (node->var_decl.init) {
        auto init_type = check_expression(node->var_decl.init);
        if (init_type->base == BaseType::Method) {
          error_loc(node->var_decl.init->location,
                    "Cannot assign methods to variables");
        }
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
          error_loc(node->var_decl.var->type->location,
                    "Invalid variable type");
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
      push_scope();
      if (node->for_loop.init) check_statement(node->for_loop.init);
      if (node->for_loop.cond) {
        auto cond_type = check_expression(node->for_loop.cond);
        if (cond_type->base != BaseType::Bool) {
          error_loc(node->for_loop.cond->location, "Condition must be boolean");
        }
      }
      if (node->for_loop.incr) check_expression(node->for_loop.incr);
      if (node->for_loop.body) check_statement(node->for_loop.body);
      pop_scope();
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

void TypeChecker::check_method_call(Type *method_type, AST *node) {
  auto callee = node->call.callee;
  if (callee->type != ASTType::Member &&
      callee->type != ASTType::StaticMember) {
    error_loc(callee->location,
              "Method call is not to a member, internal compiler error");
  }

  auto method         = methods[method_type->struct_name][callee->member.name];
  node->call.func_def = method;

  // Due to the way we handle typechecking, we might run this function twice
  // on the same node. This is fine, but we need to make sure we don't double
  // add the method argument twice implicitly.
  if (node->call.added_method_arg) return;
  node->call.added_method_arg = true;

  if (callee->type == ASTType::Member) {
    if (method->params.size() == 0) {
      // This should ideally never happen.
      error_loc(callee->location,
                "Instance method should have `this` argument, internal error");
    }
    auto method_param = method->params[0]->type;
    auto first_arg    = callee->member.lhs;
    if (callee->member.is_pointer && method_param->base != BaseType::Pointer) {
      auto ptr        = new AST(ASTType::Dereference, first_arg->location);
      ptr->unary.expr = first_arg;
      first_arg       = ptr;
    } else if (!callee->member.is_pointer &&
               method_param->base == BaseType::Pointer) {
      auto ptr        = new AST(ASTType::Address, first_arg->location);
      ptr->unary.expr = first_arg;
      first_arg       = ptr;
    }

    node->call.args->insert(node->call.args->begin(), first_arg);
  }
}

Type *TypeChecker::check_call(AST *node) {
  // This is a hack to avoid typechecking of `print` and `println`
  auto callee = node->call.callee;
  if (callee->type == ASTType::Var) {
    callee->var.is_function = false;
    auto &name              = node->call.callee->var.name;
    if (name == "print" || name == "println") {
      for (auto arg : *node->call.args) check_expression(arg);
      return new Type(BaseType::Void, node->location);
    }
  }

  auto func_type      = check_expression(callee);
  node->call.func_def = func_type->func_def;
  if (func_type->base != BaseType::Function &&
      func_type->base != BaseType::Method) {
    error_loc(node->call.callee->location, "Cannot call a non-function type");
  }

  if (func_type->base == BaseType::Method) {
    check_method_call(func_type, node);
  }

  auto &params = func_type->arg_types;
  if (params.size() != node->call.args->size()) {
    error_loc(node->location,
              "Number of arguments does not match function signature");
  }
  for (int i = 0; i < params.size(); i++) {
    auto param    = params[i];
    auto arg      = node->call.args->at(i);
    auto arg_type = check_expression(arg);
    if (*param != *arg_type) {
      error_loc(arg->location,
                "Argument type does not match function parameter type");
    }
  }

  return func_type->return_type;
}

Type *TypeChecker::check_pointer_arithmetic(AST *node, Type *left,
                                            Type *right) {
  switch (node->type) {
    case ASTType::Plus:
    case ASTType::Minus: {
      if (left->base == BaseType::Pointer && right->base == BaseType::I32) {
        return left;
      }
      if (left->base == BaseType::I32 && right->base == BaseType::Pointer) {
        return right;
      }
      if (left->base == BaseType::Pointer && right->base == BaseType::Pointer) {
        if (node->type == ASTType::Minus) {
          return new Type(BaseType::I32, node->location);
        }
      }
    }
    default: break;
  }
  error_loc(node->location, "Invalid pointer arithmetic");
}

Type *TypeChecker::check_format_string(AST *node) {
  stringstream ss;
  auto format_parts = node->format_str.format_parts;
  auto expr_args    = node->format_str.expr_args;
  if (format_parts->size() != expr_args->size() + 1) {
    error_loc(node->location,
              "Number of format parts does not match number of "
              "expressions");
  }

  // This is a hack, this should be in codegen
  for (int i = 0; i < expr_args->size(); i++) {
    auto format_part = format_parts->at(i);
    ss << format_part;

    auto expr_arg  = expr_args->at(i);
    auto expr_type = check_expression(expr_arg);
    if (expr_type->base == BaseType::I32 || expr_type->base == BaseType::Bool ||
        expr_type->base == BaseType::U8) {
      ss << "%d";
    } else if (expr_type->base == BaseType::F32) {
      ss << "%f";
    } else if (expr_type->base == BaseType::Pointer &&
               expr_type->ptr_to->base == BaseType::Char) {
      ss << "%s";
    } else if (expr_type->base == BaseType::Pointer) {
      ss << "%p";
    } else {
      error_loc(expr_arg->location, "Format expression is of unsupported type");
    }
  }
  ss << format_parts->back();

  node->format_str.format_str = *new string(ss.str());
  return new Type(BaseType::Pointer, BaseType::Char, node->location);
}

Type *TypeChecker::check_expression(AST *node) {
  switch (node->type) {
    case ASTType::Call: return check_call(node);
    case ASTType::IntLiteral: return new Type(BaseType::I32, node->location);
    case ASTType::FloatLiteral: return new Type(BaseType::F32, node->location);
    case ASTType::BoolLiteral: return new Type(BaseType::Bool, node->location);
    case ASTType::StringLiteral:
      return new Type(BaseType::Pointer, BaseType::Char, node->location);

    case ASTType::FormatStringLiteral: return check_format_string(node);
    case ASTType::SizeOf: {
      if (!type_is_valid(node->sizeof_type)) {
        error_loc(node->location, "Invalid type");
      }
      return new Type(BaseType::I32, node->location);
    }

    case ASTType::Var: {
      if (node->var.is_function) { return node->var.function->type; }

      auto var = find_var(node->var.name);
      if (var != nullptr) {
        node->var.is_function = false;
        node->var.var         = var;
        return var->type;
      }

      if (auto func = functions.find(node->var.name); func != functions.end()) {
        node->var.is_function = true;
        node->var.function    = func->second;
        auto func_type        = func->second->type;
        return func->second->type;
      }

      error_loc(node->location, "Unknown Identifier");
    }

    // TODO: Allow more complex binary expressions, will eventually need support
    // for pointers
    case ASTType::Plus:
    case ASTType::Minus:
    case ASTType::Multiply:
    case ASTType::Divide: {
      auto lhs_type = check_expression(node->binary.lhs);
      auto rhs_type = check_expression(node->binary.rhs);
      if (lhs_type->base == BaseType::Pointer ||
          rhs_type->base == BaseType::Pointer) {
        return check_pointer_arithmetic(node, lhs_type, rhs_type);
      }
      if (!lhs_type->is_numeric() || !rhs_type->is_numeric()) {
        error_loc(node->location, "Operator requires numeric types");
      }
      if (*lhs_type != *rhs_type) {
        error_loc(node->location, "Operands must be be of the same type");
      }
      return lhs_type;
    }

    case ASTType::LessThan:
    case ASTType::LessThanEquals:
    case ASTType::GreaterThan:
    case ASTType::GreaterThanEquals: {
      auto lhs_type = check_expression(node->binary.lhs);
      auto rhs_type = check_expression(node->binary.rhs);
      if (!lhs_type->is_numeric() || !rhs_type->is_numeric()) {
        error_loc(node->location, "Operator requires numeric types");
      }
      if (*lhs_type != *rhs_type) {
        error_loc(node->location, "Operands must be be of the same type");
      }
      return new Type(BaseType::Bool, node->location);
    }

    case ASTType::Equals:
    case ASTType::NotEquals: {
      auto lhs_type = check_expression(node->binary.lhs);
      auto rhs_type = check_expression(node->binary.rhs);
      if (*lhs_type != *rhs_type) {
        error_loc(node->location, "Operands must be be of the same type");
      }
      if (lhs_type->base == BaseType::Struct) {
        error_loc(node->location, "Cannot compare structs directly");
      }
      return new Type(BaseType::Bool, node->location);
    }

    case ASTType::And:
    case ASTType::Or: {
      auto lhs_type = check_expression(node->binary.lhs);
      auto rhs_type = check_expression(node->binary.rhs);
      if (lhs_type->base != BaseType::Bool ||
          rhs_type->base != BaseType::Bool) {
        error_loc(node->location, "Operands must be boolean");
      }
      return new Type(BaseType::Bool, node->location);
    }

    case ASTType::BitwiseOr:
    case ASTType::BitwiseAnd: {
      auto lhs_type = check_expression(node->binary.lhs);
      auto rhs_type = check_expression(node->binary.rhs);
      if (lhs_type->base != BaseType::I32 || rhs_type->base != BaseType::I32) {
        error_loc(node->location, "Operator requires integer types");
      }
      return lhs_type;
    }

    case ASTType::Not: {
      auto expr_type = check_expression(node->unary.expr);
      if (expr_type->base != BaseType::Bool) {
        error_loc(node->unary.expr->location, "Expression must be boolean");
      }
      return new Type(BaseType::Bool, expr_type->location);
    }

    case ASTType::UnaryMinus: {
      auto expr_type = check_expression(node->unary.expr);
      if (!expr_type->is_numeric()) {
        error_loc(node->unary.expr->location, "Expression must be a number");
      }
      return expr_type;
    }

    case ASTType::Address: {
      // TODO: Only allow pointers to l-values, reject literals;
      auto expr_type = check_expression(node->unary.expr);
      return new Type(BaseType::Pointer, expr_type, expr_type->location);
    }

    case ASTType::Dereference: {
      auto expr_type = check_expression(node->unary.expr);
      if (expr_type->base != BaseType::Pointer) {
        error_loc(node->unary.expr->location,
                  "Expression must be a pointer-type");
      }
      return expr_type->ptr_to;
    }

    case ASTType::PlusEquals:
    case ASTType::MinusEquals:
    case ASTType::DivideEquals:
    case ASTType::MultiplyEquals: {
      // TODO: Only allow assignments to l-values, reject literals;
      auto lhs_type = check_expression(node->binary.lhs);
      auto rhs_type = check_expression(node->binary.rhs);
      if (!lhs_type->is_numeric() || !rhs_type->is_numeric()) {
        error_loc(node->location, "Operator requires numeric types");
      }
      if (*lhs_type != *rhs_type) {
        error_loc(node->location, "Operands must be be of the same type");
      }
      return lhs_type;
    }

    case ASTType::Assignment: {
      // TODO: Only allow assignments to l-values, reject literals;
      auto lhs = check_expression(node->binary.lhs);
      auto rhs = check_expression(node->binary.rhs);
      if (*lhs != *rhs) {
        error_loc(node->location,
                  "Variable type does not match assignment type");
      }
      return lhs;
    }

    case ASTType::StaticMember: {
      if (node->member.lhs->type != ASTType::Var) {
        error_loc(node->location,
                  "Left hand side of `::` must be a struct name");
      }

      auto struct_name = node->member.lhs->var.name;
      if (structs.count(struct_name) == 0) {
        error_loc(node->member.lhs->location, "Unknown struct with this name");
      }

      auto field_name = node->member.name;

      if (auto _method = methods[struct_name].find(field_name);
          _method != methods[struct_name].end()) {
        auto method = _method->second;
        // Note: This _can_ be an instance method, the user just needs to
        //       call the method with the `this` parameter manually.
        return method->type;
      }

      error_loc(node->location, "Struct has no static method with this name");
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

      auto struct_name = struct_type->struct_name;
      auto field_name  = node->member.name;

      if (auto field = get_struct_member(struct_name, field_name))
        return field->type;

      if (auto _method = methods[struct_type->struct_name].find(field_name);
          _method != methods[struct_type->struct_name].end()) {
        auto method = _method->second;
        if (method->is_static) {
          error_loc(node->location,
                    "Member access requires a non-static method");
        }
        return method->type;
      }

      error_loc(node->location, "Struct has no member with this name");
    }

    case ASTType::Cast: {
      auto lhs_type = check_expression(node->cast.lhs);
      if (!type_is_valid(node->cast.to_type)) {
        error_loc(node->cast.to_type->location, "Type does not exist");
      }
      // TODO: Check if cast is valid
      return node->cast.to_type;
    }

    default: break;
  }
  cerr << HERE << " UNHANDLED TYPE IN check_expression: " << node->type
       << std::endl;
  exit(1);
}
