#include "stdarg.h"
#include "stddef.h"
#include "stdio.h"
#include "stdlib.h"

char* __format_string(const char* format, ...) {
  va_list args;
  va_start(args, format);

  int size = vsnprintf(NULL, 0, format, args);

  va_end(args);
  va_start(args, format);

  char* s = malloc(size + 1);

  vsprintf(s, format, args);
  va_end(args);
  return s;
}