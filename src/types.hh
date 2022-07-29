#pragma once

#include <iostream>
#include <string_view>

using namespace std;

#define ENUM_BASE_TYPES(F)                                                     \
  F(I32, "int32_t")                                                            \
  F(Bool, "bool")                                                              \
  F(Void, "void")

enum class BaseType {
#define F(name, text) name,
  ENUM_BASE_TYPES(F)
#undef F
      Pointer,
};

struct Type {
  BaseType base;
  Type *ptr_to;

  Type(BaseType base) : base(base), ptr_to(nullptr) {}
  Type(BaseType base, Type *ptr_to) : base(base), ptr_to(ptr_to) {}
  Type(BaseType base, BaseType ptr_to_typ)
      : base(base), ptr_to(new Type(ptr_to_typ)) {}

  bool operator==(const Type &other) const;
};

inline std::ostream &operator<<(std::ostream &os, const Type &tok) {
  switch (tok.base) {
#define F(name, text)                                                          \
  case BaseType::name: os << text; break;
    ENUM_BASE_TYPES(F)
#undef F
    case BaseType::Pointer:
      os << *tok.ptr_to;
      os << "*";
      break;
  };
  return os;
}
