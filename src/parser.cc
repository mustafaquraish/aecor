#include "parser.hh"

#include <filesystem>
#include <iostream>

#include "ast.hh"
#include "lexer.hh"
#include "tokens.hh"
#include "types.hh"

#define UNHANDLED_TYPE()                                                       \
  error_loc(token().location, format("Unexpected token: " << token().type))

Token &Parser::consume_impl(TokenType token_type, const char *sloc) {
  auto &ret = expect_impl(token_type, sloc);
  curr++;
  return ret;
};

Token &Parser::expect_impl(TokenType token_type, const char *sloc) {
  if (token_is(token_type)) return token();
  error_loc(token().location, format("Expected token: " << token_type));
};

bool Parser::consume_if(TokenType token_type) {
  if (token_is(token_type)) {
    curr++;
    return true;
  }
  return false;
}

void Parser::consume_line_end() {
  if (token().type == TokenType::Semicolon) {
    curr++;
    return;
  }
  if (token().newline_before) { return; }

  error_loc(token().location, "Expected newline / semicolon");
}

Type *Parser::parse_type() {
  Type *type = nullptr;

  // Type Prefixes here
  auto running = true;
  while (running) {
    switch (token().type) {
      case TokenType::Ampersand:
        type = new Type(BaseType::Pointer, type, token().location);
        ++curr;
        break;

      default: running = false; break;
    }
  }

  switch (token().type) {
    case TokenType::I32:
      type = new Type(BaseType::I32, type, token().location);
      break;
    case TokenType::Bool:
      type = new Type(BaseType::Bool, type, token().location);
      break;
    case TokenType::U8:
      type = new Type(BaseType::U8, type, token().location);
      break;
    case TokenType::F32:
      type = new Type(BaseType::F32, type, token().location);
      break;
    case TokenType::String:
      type = new Type(BaseType::U8,
                      new Type(BaseType::Pointer, type, token().location),
                      token().location);
      break;
    case TokenType::UnypedPtr:
      type = new Type(BaseType::Void,
                      new Type(BaseType::Pointer, type, token().location),
                      token().location);
      break;
    case TokenType::Void:
      type = new Type(BaseType::Void, type, token().location);
      break;
    case TokenType::Identifier: {
      type              = new Type(BaseType::Struct, type, token().location);
      type->struct_name = token().text;
      break;
    }

    default: UNHANDLED_TYPE();
  }
  ++curr;

  // We want to have Ptr1->Ptr2->Base, but we have Base->Ptr2->Ptr1
  // So, we'll just reverse the type linked-list.
  // This will be more useful once we have something like Base->Ptr->Arr
  type = Type::reverse_linked_list(type);

  // Type Postfixes here

  return type;
}

FunctionDef *Parser::parse_function() {
  consume(TokenType::Def);

  auto struct_name = string_view();
  auto is_method   = false;
  auto name        = consume(TokenType::Identifier);
  if (consume_if(TokenType::ColonColon)) {
    is_method   = true;
    struct_name = name.text;
    name        = consume(TokenType::Identifier);
  }

  auto func         = new FunctionDef(name.location);
  func->name        = name.text;
  func->struct_name = struct_name;
  func->is_method   = is_method;

  consume(TokenType::OpenParen);
  bool first = true;
  while (!token_is(TokenType::CloseParen)) {
    auto name  = consume(TokenType::Identifier);
    Type *type = nullptr;
    if (first) {
      first = false;
      if (is_method) {
        if (name.text == "this") {
          auto struct_type         = new Type(BaseType::Struct, name.location);
          struct_type->struct_name = struct_name;
          type = new Type(BaseType::Pointer, struct_type, name.location);
        } else {
          error_loc(name.location,
                    "Expected 'this', static methods not supported yet");
        }
      }
    }
    if (!type) {
      consume(TokenType::Colon);
      type = parse_type();
    }
    func->params.push_back(new Variable{name.text, type, name.location});
    if (!consume_if(TokenType::Comma)) break;
  }

  consume(TokenType::CloseParen);
  if (func->is_method && func->params.size() == 0) {
    error_loc(
        name.location,
        "Expected 'this' as first argument, static methods not supported yet");
  }

  if (consume_if(TokenType::Colon)) {
    func->return_type = parse_type();
  } else {
    if (name.text == "main") {
      func->return_type = new Type(BaseType::I32, name.location);
    } else {
      func->return_type = new Type(BaseType::Void, name.location);
    }
  }

  if (consume_if(TokenType::Extern)) {
    if (is_method) { error_loc(name.location, "Extern methods not supported"); }
    func->is_extern = true;
    if (consume_if(TokenType::OpenParen)) {
      auto name         = consume(TokenType::StringLiteral);
      func->extern_name = name.text;
      consume(TokenType::CloseParen);
    } else {
      func->extern_name = func->name;
    }
    func->body = nullptr;
  } else {
    func->is_extern = false;
    func->body      = parse_block();
  }
  return func;
};

StructDef *Parser::parse_struct() {
  consume(TokenType::Struct);

  auto name = consume(TokenType::Identifier);

  auto _struct  = new StructDef(name.location);
  _struct->name = name.text;

  if (consume_if(TokenType::Extern)) {
    _struct->is_extern = true;
    if (consume_if(TokenType::OpenParen)) {
      auto name            = consume(TokenType::StringLiteral);
      _struct->extern_name = name.text;
      consume(TokenType::CloseParen);
    } else {
      _struct->extern_name = _struct->name;
    }
  } else {
    _struct->is_extern = false;
    consume(TokenType::OpenCurly);
    while (!token_is(TokenType::CloseCurly)) {
      auto name = consume(TokenType::Identifier);
      consume(TokenType::Colon);
      auto type = parse_type();
      _struct->fields.push_back(new Variable{name.text, type, name.location});
      consume_line_end();
    }
    consume(TokenType::CloseCurly);
  }

  auto type         = new Type(BaseType::Struct, name.location);
  type->struct_name = name.text;
  _struct->type     = type;

  return _struct;
}

void Parser::include_file(Program *program, string_view filename) {
  if (program->is_file_included(filename)) {
    // Already included
    return;
  }
  program->add_included_file(filename);

  auto source = slurp_file(string(filename).c_str());
  auto lexer  = Lexer(*source, filename);
  auto tokens = lexer.lex();

  auto parser = Parser(tokens);
  parser.parse_into_program(program);
}

void Parser::parse_use(Program *program) {
  consume(TokenType::Use);
  auto name = consume(TokenType::StringLiteral);
  consume_line_end();
  include_file(program, name.text);
}

void Parser::parse_into_program(Program *program) {
  while (!token_is(TokenType::Eof)) {
    switch (token().type) {
      case TokenType::Def: {
        auto func = parse_function();
        program->functions.push_back(func);
        break;
      }
      case TokenType::Use: {
        parse_use(program);
        break;
      }
      case TokenType::Struct: {
        auto structure = parse_struct();
        program->structs.push_back(structure);
        break;
      }
      default: UNHANDLED_TYPE();
    }
  };
};

Program *Parser::parse_program() {
  auto program = new Program();
  include_file(program, "./lib/prelude.ae");
  program->add_included_file(token().location.filename);
  parse_into_program(program);
  return program;
}

AST *Parser::parse_block() {
  auto node = new AST(ASTType::Block, token().location);

  consume(TokenType::OpenCurly);

  while (token().type != TokenType::CloseCurly) {
    auto statement = parse_statement();
    node->block.statements->push_back(statement);
  }

  consume(TokenType::CloseCurly);
  return node;
}

AST *Parser::parse_statement() {
  AST *node = nullptr;

  switch (token().type) {
    case TokenType::If: {
      node = new AST(ASTType::If, token().location);
      consume(TokenType::If);
      node->if_stmt.cond = parse_expression();
      node->if_stmt.body = parse_block();
      if (consume_if(TokenType::Else)) {
        node->if_stmt.els = parse_block();
      } else {
        node->if_stmt.els = nullptr;
      }
      break;
    }
    case TokenType::While: {
      node = new AST(ASTType::While, token().location);
      consume(TokenType::While);
      node->while_loop.cond = parse_expression();
      node->while_loop.body = parse_statement();
      break;
    }
    case TokenType::For: {
      node = new AST(ASTType::For, token().location);
      consume(TokenType::For);

      node->for_loop.init = nullptr;
      node->for_loop.cond = nullptr;
      node->for_loop.incr = nullptr;

      // FIXME: Allow variable declarations in here
      if (!token_is(TokenType::Semicolon))
        node->for_loop.init = parse_expression();
      consume(TokenType::Semicolon);
      if (!token_is(TokenType::Semicolon))
        node->for_loop.cond = parse_expression();
      consume(TokenType::Semicolon);

      // FIXME: Should we always require a curly?
      if (!token_is(TokenType::CloseCurly))
        node->for_loop.incr = parse_expression();

      node->for_loop.body = parse_statement();
      break;
    }
    case TokenType::Defer: {
      node = new AST(ASTType::Defer, token().location);
      consume(TokenType::Defer);
      node->unary.expr = parse_statement();
      consume_line_end();
      break;
    }
    case TokenType::Return: {
      node = new AST(ASTType::Return, token().location);
      consume(TokenType::Return);
      node->unary.expr = parse_expression();
      consume_line_end();
      break;
    }
    case TokenType::Let: {
      node = new AST(ASTType::VarDeclaration, token().location);
      consume(TokenType::Let);

      auto name = consume(TokenType::Identifier);

      Type *type = nullptr;
      if (consume_if(TokenType::Colon)) { type = parse_type(); }

      node->var_decl.var = new Variable{name.text, type, name.location};

      if (token_is(TokenType::Equals)) {
        consume(TokenType::Equals);
        node->var_decl.init = parse_expression();
      } else {
        node->var_decl.init = nullptr;
      }
      consume_line_end();
      break;
    }
    case TokenType::OpenCurly: {
      node = parse_block();
      break;
    }
    default: {
      node = parse_expression();
      consume_line_end();
      break;
    }
  }

  return node;
}

ASTType token_to_op(TokenType type) {
  switch (type) {
    case TokenType::And: return ASTType::And;
    case TokenType::Or: return ASTType::Or;

    case TokenType::GreaterThan: return ASTType::GreaterThan;
    case TokenType::GreaterThanEquals: return ASTType::GreaterThanEquals;
    case TokenType::LessThan: return ASTType::LessThan;
    case TokenType::LessThanEquals: return ASTType::LessThanEquals;
    case TokenType::Equals: return ASTType::Assignment;
    case TokenType::EqualEquals: return ASTType::Equals;
    case TokenType::NotEquals: return ASTType::NotEquals;

    case TokenType::Minus: return ASTType::Minus;
    case TokenType::Plus: return ASTType::Plus;
    case TokenType::Slash: return ASTType::Divide;
    case TokenType::Star: return ASTType::Multiply;
    case TokenType::MinusEquals: return ASTType::MinusEquals;
    case TokenType::PlusEquals: return ASTType::PlusEquals;
    case TokenType::SlashEquals: return ASTType::DivideEquals;
    case TokenType::StarEquals: return ASTType::MultiplyEquals;
    default: break;
  }
  cerr << HERE << " Unhandled token in " << __FUNCTION__ << ": " << type
       << endl;
  exit(1);
}

AST *Parser::parse_factor(bool in_parens) {
  AST *node = nullptr;

  switch (token().type) {
    case TokenType::True: {
      node               = new AST(ASTType::BoolLiteral, token().location);
      node->bool_literal = true;
      consume(TokenType::True);
      break;
    }
    case TokenType::False: {
      node               = new AST(ASTType::BoolLiteral, token().location);
      node->bool_literal = false;
      consume(TokenType::False);
      break;
    }
    case TokenType::Dot: {
      node = new AST(ASTType::Member, token().location);

      auto lhs      = new AST(ASTType::Var, token().location);
      lhs->var.name = "this";
      consume(TokenType::Dot);

      auto name = consume(TokenType::Identifier);

      node->member.lhs  = lhs;
      node->member.name = name.text;
      break;
    }
    case TokenType::Not: {
      node = new AST(ASTType::Not, token().location);
      consume(TokenType::Not);
      node->unary.expr = parse_factor(true);
      break;
    }
    case TokenType::Minus: {
      node = new AST(ASTType::UnaryMinus, token().location);
      consume(TokenType::Minus);
      node->unary.expr = parse_factor(true);
      break;
    }
    case TokenType::Ampersand: {
      node = new AST(ASTType::Address, token().location);
      consume(TokenType::Ampersand);
      node->unary.expr = parse_factor(true);
      break;
    }
    case TokenType::Star: {
      node = new AST(ASTType::Dereference, token().location);
      consume(TokenType::Star);
      node->unary.expr = parse_factor(true);
      break;
    }
    case TokenType::IntLiteral: {
      node              = new AST(ASTType::IntLiteral, token().location);
      node->num_literal = token().text;
      consume(TokenType::IntLiteral);
      break;
    }
    case TokenType::FloatLiteral: {
      node              = new AST(ASTType::FloatLiteral, token().location);
      node->num_literal = token().text;
      consume(TokenType::FloatLiteral);
      break;
    }
    case TokenType::StringLiteral: {
      node                 = new AST(ASTType::StringLiteral, token().location);
      node->string_literal = token().text;
      consume(TokenType::StringLiteral);
      break;
    }
    case TokenType::Identifier: {
      node           = new AST(ASTType::Var, token().location);
      node->var.name = consume(TokenType::Identifier).text;
      break;
    }
    case TokenType::OpenParen: {
      consume(TokenType::OpenParen);
      node = parse_expression(true);
      consume(TokenType::CloseParen);
      break;
    }
    default: UNHANDLED_TYPE();
  }

  auto done = false;
  while (!done) {
    if (!in_parens && token().newline_before) break;
    switch (token().type) {
      case TokenType::OpenParen: {
        auto paren_loc = consume(TokenType::OpenParen).location;
        auto args      = new vector<AST *>();
        while (!token_is(TokenType::CloseParen)) {
          args->push_back(parse_expression());
          if (!consume_if(TokenType::Comma)) break;
        }
        consume(TokenType::CloseParen);
        auto call_type = ASTType::Call;
        if (node->type == ASTType::Member) { call_type = ASTType::MethodCall; }
        auto call         = new AST(call_type, paren_loc);
        call->call.callee = node;
        call->call.args   = args;
        node              = call;
        break;
      }
      case TokenType::Dot: {
        consume(TokenType::Dot);
        auto name           = consume(TokenType::Identifier);
        auto member         = new AST(ASTType::Member, name.location);
        member->member.lhs  = node;
        member->member.name = name.text;
        node                = member;
        break;
      }
      case TokenType::As: {
        consume(TokenType::As);
        auto cast          = new AST(ASTType::Cast, token().location);
        cast->cast.lhs     = node;
        cast->cast.to_type = parse_type();
        node               = cast;
        break;
      }

      default: done = true;
    }
  }

  return node;
}

AST *Parser::parse_term(bool in_parens) {
  auto lhs = parse_factor(in_parens);
  while (token_is(TokenType::Star) || token_is(TokenType::Slash)) {
    if (!in_parens && token().newline_before) break;
    auto node = new AST(token_to_op(token().type), token().location);
    ++curr;
    node->binary.lhs = lhs;
    node->binary.rhs = parse_term(in_parens);
    lhs              = node;
  }
  return lhs;
}

AST *Parser::parse_additive(bool in_parens) {
  auto lhs = parse_term(in_parens);
  while (token_is(TokenType::Plus) || token_is(TokenType::Minus)) {
    if (!in_parens && token().newline_before) break;
    auto node = new AST(token_to_op(token().type), token().location);
    ++curr;
    node->binary.lhs = lhs;
    node->binary.rhs = parse_additive(in_parens);
    lhs              = node;
  }
  return lhs;
}

AST *Parser::parse_relational(bool in_parens) {
  auto lhs = parse_additive(in_parens);
  while (token_is(TokenType::LessThan) || token_is(TokenType::GreaterThan) ||
         token_is(TokenType::LessThanEquals) ||
         token_is(TokenType::GreaterThanEquals) ||
         token_is(TokenType::EqualEquals) || token_is(TokenType::NotEquals)) {
    if (!in_parens && token().newline_before) break;
    auto node = new AST(token_to_op(token().type), token().location);
    ++curr;
    node->binary.lhs = lhs;
    node->binary.rhs = parse_relational(in_parens);
    lhs              = node;
  }
  return lhs;
}

AST *Parser::parse_logical_and(bool in_parens) {
  auto lhs = parse_relational(in_parens);
  while (token_is(TokenType::And)) {
    if (!in_parens && token().newline_before) break;
    auto node = new AST(token_to_op(token().type), token().location);
    ++curr;
    node->binary.lhs = lhs;
    node->binary.rhs = parse_logical_and(in_parens);
    lhs              = node;
  }
  return lhs;
}

AST *Parser::parse_logical_or(bool in_parens) {
  auto lhs = parse_logical_and(in_parens);
  while (token_is(TokenType::Or)) {
    if (!in_parens && token().newline_before) break;
    auto node = new AST(token_to_op(token().type), token().location);
    ++curr;
    node->binary.lhs = lhs;
    node->binary.rhs = parse_logical_or(in_parens);
    lhs              = node;
  }
  return lhs;
}

AST *Parser::parse_expression(bool in_parens) {
  auto lhs = parse_logical_or(in_parens);

  while (token_is(TokenType::Equals) || token_is(TokenType::PlusEquals) ||
         token_is(TokenType::MinusEquals) || token_is(TokenType::StarEquals) ||
         token_is(TokenType::SlashEquals)) {
    if (!in_parens && token().newline_before) break;
    auto node = new AST(token_to_op(token().type), token().location);
    ++curr;
    node->binary.lhs = lhs;
    node->binary.rhs = parse_expression(in_parens);
    lhs              = node;
  }

  return lhs;
}
