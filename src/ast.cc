#include <ast.hh>
#include <utils.hh>

#include "tokens.hh"

AST::AST(ASTType type, Location location)
    : type(type), location(location) {
  if (type == ASTType::Block) {
    block.statements = new vector<AST *>();
  }
}