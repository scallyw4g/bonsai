#define PI64 (3.1415926535897932384626433832795028841971693993)
#define PI32 ((float)PI64)

#define local_persist     static
#define global_variable   static
#define debug_global      static

#define link_internal     static
#define link_external     extern "C"

#if BONSAI_WIN32
#define dynamic_link_lib_export __declspec(dllexport) link_external

#else
#define dynamic_link_lib_export link_external

#endif

#define bonsai_function link_internal


#define True  (1)
#define False (0)

#define ArrayCount(a) (sizeof(a)/sizeof(a[0]))
#define InvalidDefaultCase default: {Error("InvalidDefaultCase"); Assert(False);} break

// 8 Bit types
typedef char           s8;
typedef unsigned char  u8;

// 16 Bit types
typedef short          s16;
typedef unsigned short u16;

// 32 Bit types
typedef int          s32;
typedef unsigned int u32;
typedef float        r32;
typedef u32          b32;

typedef r32      radians;
typedef r32      degrees;

// 64 Bit types
typedef long long int           s64;
typedef unsigned long long int  u64;
typedef double                  r64;
typedef u64                     umm;

CAssert(sizeof(s64) == 8);
CAssert(sizeof(u64) == 8);
CAssert(sizeof(r64) == 8);
CAssert(sizeof(umm) == 8);

CAssert(sizeof(s32) == 4);
CAssert(sizeof(u32) == 4);
CAssert(sizeof(r32) == 4);

CAssert(sizeof(s16) == 2);
CAssert(sizeof(u16) == 2);

CAssert(sizeof(s8) == 1);
CAssert(sizeof(u8) == 1);

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


#if 0
// #include <stdint.h>
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
#endif

#define CSz(NullTerminatedCString) \
  CS(NullTerminatedCString, sizeof(NullTerminatedCString)-1)

struct counted_string
{
  umm Count;
  const char* Start; // TODO(Jesse, id: 94, tags: cleanup, open_question): Make this non-const?
};
CAssert(sizeof(counted_string) == 16);

bonsai_function counted_string FormatCountedString_(char* Dest, umm DestSize, const char *FS, ...);

counted_string
CS(const char *S, umm Count)
{
  counted_string Result = {
    .Count = Count, .Start = S
  };
  return Result;
}



meta(
  func index_of(Type)
  {
    bonsai_function umm
    IndexOf((Type.name)_buffer *Buf, (Type.name) Element)
    {
      umm Result = Buf->Count;

      for (u32 ElementIndex = 0;
          ElementIndex < Buf->Count;
          ++ElementIndex)
      {
        if (AreEqual(Buf->Start[ElementIndex], Element))
        {
          Result = ElementIndex;
          break;
        }
      }

      return Result;
    }
  }
)

meta(
  func are_equal(Type)
  {
    bonsai_function b32
    AreEqual((Type.name) *Thing1, (Type.name) *Thing2)
    {
      b32 Result = MemoryIsEqual((u8*)Thing1, (u8*)Thing2, sizeof( (Type.name) ) );
      return Result;
    }
  }
)

