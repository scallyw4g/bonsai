int ding = 42;

#include <src/tests/fixtures/preprocessor/include_test.cpp>

#define MacroKeyword    this_is_a_variable_name
#define IndirectMacroKeyword MacroKeyword
int MacroKeyword = 42;
int IndirectMacroKeyword = 42;

#define MacroFunction(a) a

#define MacroFunction2(a) int MacroKeyword = a

#define MacroFunction3(a, b, c, d) a b c d

#define MacroFunction4(a, ...) a

#define MacroFunction5(a, b, ...) a b __VA_ARGS__

#define MacroFunction6(...) __VA_ARGS__

#define MacroFunction7(a, b) a b

#define MacroFunction8(a, b) \
  a                          \
  b


// MacroFunction


MacroFunction(int this_is_a_variable_name = 42);

MacroFunction(int MacroKeyword = 42);

MacroFunction(SomeRegularFunctionCall(42));


// MacroFunction2


MacroFunction2(42);

MacroFunction(MacroFunction2(42));

MacroFunction(MacroFunction(MacroFunction2(42)));


// MacroFunction3


MacroFunction3(int, this_is_a_variable_name, =, 42);

MacroFunction3(int, MacroKeyword, =, 42);

MacroFunction3(MacroFunction2(42), , , );


// MacroFunction4


MacroFunction4(int this_is_a_variable_name = 42, , , );

MacroFunction4(int this_is_a_variable_name = 42, other, crap here, should just disappear, including MacroKeyword, MacroFunction2(666));


// MacroFunction5


MacroFunction5(int, this_is_a_variable_name, =, 42);


// MacroFunction6


MacroFunction6(int, this_is_a_variable_name, =, 42);

MacroFunction6(int, MacroKeyword, =, 42);

MacroFunction6(MacroFunction(int MacroKeyword = 42));

MacroFunction6(MacroFunction2(42));


// MacroFunction7


MacroFunction7(SomeRegularFunctionCall(4, 2);, SomeRegularFunctionCall(42));


// MacroFunction8


MacroFunction8(4, 2)
MacroFunction8 (4, 2)
MacroFunction8
MacroFunction8(MacroFunction8, 0)


#define self_including_macro_keyword self_including_macro_keyword 42
self_including_macro_keyword // should expand to "self_including_macro_keyword 42"
#undef self_including_macro_keyword

// TODO(Jesse tags: immediate): This shouldn't result in infinite-recursion .. I think?
//
/* #define self_including_macro_keyword MacroFunction(self_including_macro_keyword 42) */
/* self_including_macro_keyword // should expand to "self_including_macro_keyword 42" */
/* #undef self_including_macro_keyword */
