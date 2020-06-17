int ding = 42;

#include <src/tests/fixtures/preprocessor/include_test.cpp>

#define MacroKeyword    this_is_a_variable_name
int MacroKeyword = 42;

#define MacroFuncion(a) (a)

MacroFunction(int defined_in_macro = 42);
