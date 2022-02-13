
#define foo bar
foo

#if 1
  foo
#else
  fdsafdsa
#endif

foo

#undef foo


#include <src/tests/fixtures/preprocessor/include_test.cpp> // \
this is actually commented
#include <src/tests/fixtures/preprocessor/include_test_2.cpp>

#include <src/tests/fixtures/preprocessor/empty.cpp>
#include <src/tests/fixtures/preprocessor/empty.cpp>

#include <src/tests/fixtures/preprocessor/random_define.cpp> /* this should be fine */

#include <src/tests/fixtures/preprocessor/random_define.cpp> // this should be fine \
This line is actually commented
#include <src/tests/fixtures/preprocessor/random_define.cpp> /* this should be fine */
#include <src/tests/fixtures/preprocessor/random_define.cpp> /* this should be fine */  \

#include <src/tests/fixtures/preprocessor/empty.cpp> /*
                                                       */   
#include <src/tests/fixtures/preprocessor/random_define.cpp> // this should be fine \\\\
This line is actually commented


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

#define MacroFunction9(a, b) a ## b


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


#if 0
// MacroFunction9


MacroFunction9(this_is_a_, variable_name);
MacroFunction9(Macro, Keyword);
MacroFunction9(IndirectMacro, Keyword);


#define some_thing this_is_a_variable_name
#define some foo

MacroFunction9(some, _thing); // this_is_a_variable_name

#undef some
MacroFunction9(some, _thing); // this_is_a_variable_name

#undef some_thing
MacroFunction9(some, _thing); // some_thing

#endif


#define self_including_macro_keyword self_including_macro_keyword 42
self_including_macro_keyword // should expand to "self_including_macro_keyword 42"
#undef self_including_macro_keyword

#define self_including_macro_keyword MacroFunction(self_including_macro_keyword 42)
self_including_macro_keyword // should expand to "self_including_macro_keyword 42"
#undef self_including_macro_keyword

// NOTE(Jesse): I have no idea what this is specified to expand to.  I observed
// Clang not expanding it at all, so that's what I've programmed.
//
// UPDATE(Jesse): This now ends up in an infinite expansion loop.  Fixing the
// following cases should be done at some point, but since it's currently not
// a problem I don't have any reason to want to do it.
//
  /* #define m1() m2() */
  /* #define m2() m1() */
  /* m2() */

#define temp__() temp__()
temp__()
