
#define ONE 1
#define ZERO 0
#include <src/tests/fixtures/preprocessor/logical_operators_inl.cpp>
#undef ONE
#undef ZERO

#define ONE (1)
#define ZERO (0)
#include <src/tests/fixtures/preprocessor/logical_operators_inl.cpp>
#undef ONE
#undef ZERO

#define ONE !(0)
#define ZERO !(1)
#include <src/tests/fixtures/preprocessor/logical_operators_inl.cpp>
#undef ONE
#undef ZERO

#define ONE (2-1)
#define ZERO (1-1)
#include <src/tests/fixtures/preprocessor/logical_operators_inl.cpp>
#undef ONE
#undef ZERO

#define ONE 1
#define ZERO (!ONE)
#include <src/tests/fixtures/preprocessor/logical_operators_inl.cpp>
#undef ONE
#undef ZERO

#define ONE 1
#include <src/tests/fixtures/preprocessor/logical_operators_inl.cpp>
#undef ONE
#undef ZERO

#define ONE !UNDEFINED
#define ZERO !ONE
#include <src/tests/fixtures/preprocessor/logical_operators_inl.cpp>
#undef ONE
#undef ZERO


