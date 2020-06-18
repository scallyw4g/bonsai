int ding = 42;

#include <src/tests/fixtures/preprocessor/include_test.cpp>

#define MacroKeyword    this_is_a_variable_name
int MacroKeyword = 42;

#define MacroFunction(a) a

#define MacroFunction2(a) int MacroKeyword = a

#define MacroFunction3(a, b, c, d) a b c d


MacroFunction(int this_is_a_variable_name = 42);

MacroFunction(int MacroKeyword = 42);



MacroFunction2(42);

MacroFunction(MacroFunction2(42));

MacroFunction(MacroFunction(MacroFunction2(42)));


MacroFunction3(int, this_is_a_variable_name, =, 42);
