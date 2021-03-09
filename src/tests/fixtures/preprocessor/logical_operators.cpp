/* #define ZEEROW (1-1) */

/* #if 0 */
/* invalid_path */
/* #endif */

/* #if ZEEROW */
/* invalid_path */
/* #endif */

/* #if 0 > 1 */
/* invalid_path */
/* #endif */

/* #if 100 == 101 */
/* invalid_path */
/* #endif */

/* #if (1 * 0) */
/* invalid_path */
/* #endif */

/* #if (2 < 1) */
/* invalid_path */
/* #endif */

/* #if (2-2) */
/* invalid_path */
/* #endif */

/* #if (1*1-1) */
/* invalid_path */
/* #endif */

/* #if ((2*1)-2) */
/* invalid_path */
/* #endif */

#if (2*1-2)
invalid_path
#endif


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


