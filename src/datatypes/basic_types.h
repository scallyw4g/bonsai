#define PI 3.1415926535897932384626433832795028841971693993751058209749445923078164062
#define TWOPI (2.0*PI)
#define PIf (float(PI))

#define U64_MAX ((u64)-1)

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
