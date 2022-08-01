#pragma once

#include <iostream>
#include <string_view>
#include <tokens.hh>

using namespace std;

#define ENUM_BASE_TYPES(F)                                                     \
  F(I32, "int32_t")                                                            \
  F(Bool, "bool")                                                              \
  F(U8, "char")                                                                \
  F(Void, "void")

enum class BaseType {
#define F(name, text) name,
  ENUM_BASE_TYPES(F)
#undef F
      Pointer,
  Struct,  // clangfmt pls ;cc
};

struct Type {
  BaseType base;
  Type *ptr_to;
  string_view struct_name;
  Location location;

  Type(BaseType base, Location loc)
      : base(base), ptr_to(nullptr), location(loc) {}
  Type(BaseType base, Type *ptr_to, Location loc)
      : base(base), ptr_to(ptr_to), location(loc) {}
  Type(BaseType base, BaseType ptr_to_typ, Location loc)
      : base(base), ptr_to(new Type(ptr_to_typ, loc)), location(loc) {}

  bool is_struct_or_ptr() const {
    if (base == BaseType::Struct) return true;
    if (base != BaseType::Pointer) return false;
    return ptr_to && ptr_to->base == BaseType::Struct;
  }

  bool operator==(const Type &other) const;

  static Type *reverse_linked_list(Type *);
};

inline std::ostream &operator<<(std::ostream &os, const Type &type) {
  switch (type.base) {
#define F(name, text)                                                          \
  case BaseType::name: os << text; break;
    ENUM_BASE_TYPES(F)
#undef F
    case BaseType::Pointer:
      os << *type.ptr_to;
      os << "*";
      break;
    case BaseType::Struct: os << type.struct_name; break;
  };
  return os;
}
