#define PI64 (3.1415926535897932384626433832795028841971693993)
#define PI32 (float(PI64))

#define local_persist static
#define global_variable static
#define debug_global static

/* #define True  ((bool)1) */
/* #define False ((bool)0) */

#define ArrayCount(a) (sizeof(a)/sizeof(a[0]))
#define InvalidDefaultCase default: {Error("InvalidDefaultCase"); Assert(False);} break

// 8 Bit types
typedef uint8_t  u8;
typedef int8_t   s8;

// 16 Bit types
typedef uint16_t u16;
typedef int16_t  s16;

// 32 Bit types
typedef int32_t  s32;
typedef uint32_t u32;
typedef u32      b32;
typedef float    real32;
typedef real32   r32;
typedef r32      radians;
typedef r32      degrees;

// 64 Bit types
typedef int64_t  s64;
typedef uint64_t u64;
typedef u64      umm;
typedef double   r64;

#if 1

#define s8_MAX  ((s8) 0x7f)
#define s16_MAX ((s16)0x7fff)
#define s32_MAX ((s32)0x7fffffff)
#define s64_MAX ((s64)0x7fffffffffffffff)

#define s8_MIN  ((s8) 0x80)
#define s16_MIN ((s16)0x8000)
#define s32_MIN ((s32)0x80000000)
#define s64_MIN ((s64)0x8000000000000000)

#define u8_MAX  (0xff)
#define u16_MAX (0xffff)
#define u32_MAX (0xffffffff)
#define u64_MAX (0xffffffffffffffff)

#define f32_MAX (1E+37f)
#define f32_MIN (1E-37f)

#else

#define s8_MAX  (INT8_MAX)
#define s16_MAX (INT16_MAX)
#define s32_MAX (INT32_MAX)
#define s64_MAX (INT64_MAX)

#define s8_MIN  (INT8_MIN)
#define s16_MIN (INT16_MIN)
#define s32_MIN (INT32_MIN)
#define s64_MIN (INT64_MIN)

#define u8_MAX  (UINT8_MAX)
#define u16_MAX (UINT16_MAX)
#define u32_MAX (UINT32_MAX)
#define u64_MAX (UINT64_MAX)

#endif

CAssert(s8_MAX  == INT8_MAX);
CAssert(s16_MAX == INT16_MAX);
CAssert(s32_MAX == INT32_MAX);
CAssert(s64_MAX == INT64_MAX);

CAssert(s8_MIN  == INT8_MIN);
CAssert(s16_MIN == INT16_MIN);
CAssert(s32_MIN == INT32_MIN);
CAssert(s64_MIN == INT64_MIN);

CAssert(u8_MAX  == UINT8_MAX);
CAssert(u16_MAX == UINT16_MAX);
CAssert(u32_MAX == UINT32_MAX);
CAssert(u64_MAX == UINT64_MAX);

#define CSz(NullTerminatedCString) \
  CS(NullTerminatedCString, sizeof(NullTerminatedCString)-1)

struct counted_string
{
  umm Count;
  const char* Start; // TODO(Jesse, tags: cleanup, open_question): Make this non-const?
};
CAssert(sizeof(counted_string) == 16);

counted_string
CS(const char *S, umm Count)
{
  counted_string Result = {
    .Start = S, .Count = Count
  };
  return Result;
}

