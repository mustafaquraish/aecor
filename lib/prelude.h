#include "stdarg.h"
#include "stddef.h"
#include "stdio.h"
#include "stdlib.h"

char* format_string(const char* format, ...) {
  va_list args;
  va_start(args, format);
  char *formatted;
  vasprintf(&formatted, format, args);
  va_end(args);
  return formatted;
}
