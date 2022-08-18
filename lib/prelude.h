#include "stdarg.h"
char* format_string(const char* format, ...) {
  va_list args;
  va_start(args, format);
  int size = vsnprintf(NULL, 0, format, args);
  va_end(args);
  va_start(args, format);
  char* s = calloc(1, size + 1);
  vsprintf(s, format, args);
  s[size] = '\0';
  va_end(args);
  return s;
}
