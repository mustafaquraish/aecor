#pragma once

#define STRINGIZING(x) #x
#define STR(x) STRINGIZING(x)

#define HERE (__FILE__ ":" STR(__LINE__) ":")