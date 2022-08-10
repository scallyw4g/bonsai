
#if 1
#include <src/tests/bug_defines.h>
#include <src/tests/fixtures/preprocessor/other_file_that_should_parse.cpp>

void func(arg_type *ArgName);
void func(arg_type *);

#define VariadicMacroFunction3(...) __VA_ARGS__
VariadicMacroFunction3(int, i, =, 42);

int foo = 0;
long int foo = 0;
long foo = 0;
long long foo = 0;
long double foo = 0;

unsigned long foo = 0;
unsigned long long foo = 0;
unsigned long int foo = 0;

#define __QWORD unsigned long long
#define __STD_TYPE typedef
#define __DEV_T_TYPE __QWORD
#define __FSID_T_TYPE struct { int __val[2]; }

__STD_TYPE __FSID_T_TYPE __fsid_t;
__STD_TYPE __DEV_T_TYPE __dev_t;



#define __THING int
#define __INDIRECT_THING_TYPE_NAME __alias_for_int
#define __THING_TYPE_NAME __INDIRECT_THING_TYPE_NAME

__STD_TYPE __THING __THING_TYPE_NAME;




#if BUG_EXTERN_BLOCK_DEFINE
#define EXTERN_BLOCK extern "C" {
#define EXTERN_BLOCK_END }
EXTERN_BLOCK
#include <some_other_file_that_wants_c_linkage.h>
u32 ExternalFunc() { }
EXTERN_BLOCK_END
#endif



_Pragma("whatever")
_Pragma("whatever");

__pragma(msvc_style(thing));


int x, y;

#if BUG_COMMA_OPERATOR
int u = x < 8 ? x : y,
    v = h < 4 ? y : x == 12 || y == 14 ? x : y;
#endif

unsigned int Int0 = 42;
int          Int2 = 42;
signed int   Int3 = 42;

unsigned     Int1 = 42;
signed       Int4 = 42;


typedef unsigned short int u16;

typedef long int __int64;
typedef long int s64;

#if BUG_TYPEDEF_THING
// NOTE(Jesse): I'm actually not even sure this is valid code.  Clang rejects
// it last I checked, though msvc accepted it at one point.
typedef unsigned s64 u64;
#endif

typedef void func(int *ArgName);

typedef unsigned int u32;

u32 SizeOfInt = sizeof(int);

typedef struct foo bar;

typedef struct {
  int thingy;
} thing;

typedef struct thing {
  int thingy;
} thing;


const unsigned short const * dfsafdsa;
const unsigned short * dfsafdsa;

const unsigned long const * dfsafdsa;
const unsigned long * dfsafdsa;

const long long int thing;
const long double thing;

unsigned long int thing;
unsigned long long int thing;

typedef long long __m64 __attribute__((__vector_size__(8), __aligned__(8)));

int foo __attribute__((__aligned__(8)));

#if BUG_ATTRIBUTE_KEYWORD
enum foo
{
  val __attribute__((enum_attribute))
};

__attribute__((type_attribute)) int foo;

struct test_tfdsa
{
  struct
  {
    int foo __attribute__((__aligned__(8)));
  };
};
#endif

typedef int __v4si __attribute__((__vector_size__(16)));
typedef float __v4sf __attribute__((__vector_size__(16)));
typedef float __m128 __attribute__((__vector_size__(16), __aligned__(16)));

struct test_tfdsa
{
  struct
  {
    int foo;
  };
};

struct test_outer
{
  struct test_inner
  {
    int foo;
  };

  test_inner other;
  test_inner *otherptr;
};

struct test_tfdsa
{
  struct fdsafdsa
  {
    int foo, bar;
  };
};

struct test_tfdsa
{
  struct fdsafdsa foo;
};

struct test_tfdsa
{
  struct fdsafdsa *foo;
};



#define MacroFunction(P1, P2) P1 P2
MacroFunction(u32 AnUnsignedInt = 42;, )
MacroFunction(u32 AnotherUnsignedInt = sizeof(u32);, )

#define VariadicMacroFunction1(P1, P2, ...) P1 P2 __VA_ARGS__
#define VariadicMacroFunction2(P1, P2 ...) P1 P2 __VA_ARGS__



#define bonsai_function static


#if BUG_BODY_SCOPE_ACROSS_C_TOKEN_CURSOR_BOUNDARY
#define CLOSE_SKETCHY_FUNC }
bonsai_function u32
SketchyFunc()
{
  return 42;
CLOSE_SKETCHY_FUNC
#endif

bonsai_function u32
CheekyFunction()
{
  return 42;
}

bonsai_function u32
CallTheCheekyFunction()
{
  u32 CheekyResult = CheekyFunction();
  return CheekyResult;
}


bonsai_function u32
AnotherCheekyFunction()
{
  return 42;
}

bonsai_function u32
CallAnotherCheekyFunction()
{
  u32 AnotherCheekyResult = AnotherCheekyFunction();
  return AnotherCheekyResult;
}


bonsai_function void
CallBothCheeks()
{
  CheekyDisappearingFunction();
  AnotherCheekyFunction();
}




std::vector<int> IntVector;

struct thing
{
  std::vector<int> AnotherVector;
};


thing::thing() // Constructor bonsai_function
{
  std::vector<int> IntVectorInAFunction;
}

// Apparently constructor functions are allowed to have template arguments ..
// so this is legal if thing takes a template parameter
thing::thing<int>()
{
}

#endif

int
thing::DoTheThing(unsigned int Count) // Member bonsai_function
{
  int Result = (int)Count;
  return Result;
}

bonsai_function thing
TestFunc0()
{
  thing Result = {};
  return (Result);
}

void
TestFunc1()
{
  bool whatever = true;

  if (whatever)
  {
    /* This should

     * parse
     */
#define foo
  }

  if (whatever)
  {
    // This does parse
#define bar
  }

  int Six = 3 + 3;
}

static inline unsigned __attribute__((function_attribute))
TestFunc(int foo)
{
}


#define global_variable static
#define bonsai_function static
#define CSz(NullTerminatedCString) \
  CS(NullTerminatedCString, sizeof(NullTerminatedCString)-1)
#include <src/bonsai_stdlib/headers/console_macros.h>

#undef foo;
#undef bar;

struct fingy
{
  union {
    int foo;
  } fing;
};

