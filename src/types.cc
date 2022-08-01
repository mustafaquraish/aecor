#include <ast.hh>
#include <types.hh>

bool Type::operator==(const Type &other) const {
  if (base != other.base) return false;
  if (base == BaseType::Pointer) {
    if (ptr_to == nullptr && other.ptr_to == nullptr) return true;
    if (ptr_to == nullptr || other.ptr_to == nullptr) return false;
    return *ptr_to == *other.ptr_to;
  }
  if (base == BaseType::Struct) { return struct_name == other.struct_name; }
  return true;
}

Type *Type::reverse_linked_list(Type *list) {
  Type *rev = nullptr;

  for (Type *curr = list; curr;) {
    auto tmp     = curr->ptr_to;
    curr->ptr_to = rev;
    rev          = curr;
    curr         = tmp;
  }

  return rev;
}

std::ostream &operator<<(std::ostream &os, const Type &type) {
  switch (type.base) {
#define F(name, text)                                                          \
  case BaseType::name: os << text; break;
    ENUM_BASE_TYPES(F)
#undef F
    case BaseType::Pointer:
      os << *type.ptr_to;
      os << "*";
      break;
    case BaseType::Struct:
      if (type.struct_def->is_extern) {
        os << type.struct_def->extern_name;
      } else {
        os << type.struct_def->name;
      }
      break;
  };
  return os;
}