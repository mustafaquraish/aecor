#include <types.hh>

bool Type::operator==(const Type &other) const {
  if (base != other.base) return false;
  if (ptr_to == nullptr && other.ptr_to == nullptr) return true;
  if (ptr_to == nullptr || other.ptr_to == nullptr) return false;
  return *ptr_to == *other.ptr_to;
}
