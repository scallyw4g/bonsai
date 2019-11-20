#define PI 3.1415926535897932384626433832795028841971693993751058209749445923078164062
#define TWOPI (2.0*PI)
#define PIf (float(PI))

#define local_persist static
#define global_variable static
#define debug_global static

#define True  1
#define False 0

#define ArrayCount(a) (sizeof(a)/sizeof(a[0]))
#define InvalidDefaultCase default: {Error("InvalidDefaultCase"); Assert(False);} break

// 8 Bit types
typedef uint8_t  u8;

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

#define s8_MIN  (INT8MAX_MIN)
#define s16_MIN (INT16MAX_MIN)
#define s32_MIN (INT32MAX_MIN)
#define s64_MIN (INT64MAX_MIN)

#define s8_MAX  (INT8MAX_MAX)
#define s16_MAX (INT16MAX_MAX)
#define s32_MAX (INT32MAX_MAX)
#define s64_MAX (INT64MAX_MAX)

#define u8_MAX  (UINT8_MAX)
#define u16_MAX (UINT16_MAX)
#define u32_MAX (UINT32_MAX)
#define u64_MAX (UINT64_MAX)

