#include <ast.hh>
#include <types.hh>

static bool types_eq(const Type *a, const Type *b) {
  if (a == nullptr && b == nullptr) return true;
  if (a == nullptr || b == nullptr) return false;
  return *a == *b;
}

bool Type::operator==(const Type &other) const {
  if (base != other.base) return false;

  // Should NOT be comparing methods with each other.
  if (base == BaseType::Method) return false;

  if (base == BaseType::Function) {
    if (!types_eq(return_type, other.return_type)) return false;
    if (arg_types.size() != other.arg_types.size()) return false;
    for (size_t i = 0; i < arg_types.size(); i++) {
      if (!types_eq(arg_types[i], other.arg_types[i])) return false;
    }
    return true;
  }
  if (base == BaseType::Pointer) {
    return types_eq(ptr_to, other.ptr_to);
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
    case BaseType::Function:
    case BaseType::Method:
      cerr << "Should not be using operator << on Function/Method" << endl;
      exit(1);
  };
  return os;
}