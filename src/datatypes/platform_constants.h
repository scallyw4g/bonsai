#ifndef BONSAI_PLAT_CONSTANTS_H
#define BONSAI_PLAT_CONSTANTS_H

#include <stdio.h>

/*
 *  Common
 */

#define True  1
#define False 0

// Screen Resolution

#define SCREEN_RATIO 2

#define SCR_WIDTH (3840/SCREEN_RATIO)
#define SCR_HEIGHT (2160/SCREEN_RATIO)


#define TargetFrameTime 0.0166666666f

/*
 *  Paths
 */
#define MODELS_PATH "models"
#define SHADER_PATH "shaders"
#define SHADER_HEADER "/header.glsl"


// Wrapper so assertions give us file/line numbers
#define AssertNoGlErrors {            \
  int glErrorNo = glGetError();       \
  DumpGlErrorEnum(glErrorNo);         \
  Assert(glErrorNo == GL_NO_ERROR); }

#define ArrayCount(a) (sizeof(a)/sizeof(a[0]))
#define InvalidDefaultCase default: {Assert(!"Invalid Default Case");} break

#define LOCAL_PERSIST static


/*
 * Memory
 */

#define MEMPROTECT (MEMPROTECT_OVERFLOW || MEMPROTECT_UNDERFLOW)
#define MEMPROTECT_UNDERFLOW (0)
#define MEMPROTECT_OVERFLOW (0)

#if MEMPROTECT_UNDERFLOW && MEMPROTECT_OVERFLOW
#error "Unfortunately, Underflow and Overflow protection at the same time is impossible"
#endif

#define BONSAI_ALLOCATOR_VIRTUAL 1
#define BONSAI_ALLOCATOR_CALLOC 0

#if BONSAI_ALLOCATOR_CALLOC && BONSAI_ALLOCATOR_VIRTUAL
#error "Multiple Allocators Defined"
#endif



/*
 *  MSVC
 */
#ifdef _MSC_VER

#define __FUNCTION_NAME__ __FUNCTION__

#define global_variable static
#define debug_global static

#define VariadicOutputDebugString(FormatString, ...) {    \
    char Buffer[1024] = {};                               \
    _snprintf_s(Buffer, 1023, FormatString, __VA_ARGS__); \
    PrintConsole(Buffer); }


#define Info(...)  PrintConsole("   Info - ");             \
                   VariadicOutputDebugString(__VA_ARGS__); \
                   PrintConsole("\n")

#define Debug(...) VariadicOutputDebugString(__VA_ARGS__); \
                   PrintConsole("\n")

#define Error(...) PrintConsole(" ! Error - ");            \
                   VariadicOutputDebugString(__VA_ARGS__);   \
                   PrintConsole("\n")

#define Warn(...) PrintConsole(" * Warn - ");            \
                  VariadicOutputDebugString(__VA_ARGS__); \
                  PrintConsole("\n")

#define RuntimeBreak() __debugbreak()

// Disable warnings about insecure CRT functions
#pragma warning(disable : 4996)


/*
 *  GCC
 */
#else

#define __FUNCTION_NAME__ __func__

// Hush up gcc about unreferenced globals
#define global_variable static __attribute__((unused))
#define debug_global static __attribute__((unused))

#define Info(...)  printf("   Info - ");  printf(__VA_ARGS__); printf("\n")
#define Debug(...) printf(__VA_ARGS__);   printf("\n")
#define Error(...) printf(" ! Error - "); printf(__VA_ARGS__); printf("\n")
#define Warn(...)  printf(" * Warn - "); printf(__VA_ARGS__); printf("\n")


#define RuntimeBreak() raise(SIGTRAP)


#endif // _MSC_VER

#endif // BONSAI_PLAT_CONTANTS_H
