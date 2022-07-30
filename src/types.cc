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
