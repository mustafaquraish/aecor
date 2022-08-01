#include <ast.hh>
#include <filesystem>
#include <utils.hh>

#include "tokens.hh"

AST::AST(ASTType type, Location location) : type(type), location(location) {
  if (type == ASTType::Block) { block.statements = new vector<AST *>(); }
}

void Program::add_included_file(string_view filename) {
  auto absolute_path = std::filesystem::absolute(filename);
  auto canonical     = std::filesystem::canonical(absolute_path);
  included_files.insert(canonical.string());
}

bool Program::is_file_included(string_view filename) {
  auto absolute_path = std::filesystem::absolute(filename);
  auto canonical     = std::filesystem::canonical(absolute_path);
  return included_files.find(canonical.string()) != included_files.end();
}
