#pragma once
#include <sstream>
#include <string>
#include <string_view>

#define STRINGIZING(x) #x
#define STR(x) STRINGIZING(x)

#define HERE (__FILE__ ":" STR(__LINE__) ":")
#define format(...)                                                            \
  [&]() {                                                                      \
    std::stringstream ss;                                                      \
    ss << __VA_ARGS__;                                                         \
    return ss.str();                                                           \
  }()

std::string *slurp_file(const char *filename);

struct Location;
[[noreturn]] void error_loc(const Location &loc, std::string_view message,
                            std::string_view hint = "");