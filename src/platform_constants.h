#ifndef BONSAI_PLAT_CONTANTS_H
#define BONSAI_PLAT_CONTANTS_H

#include <stdio.h>

/*
 *  Common
 */

// TODO(Jesse): Finish Axing GLM!
#define USE_GLM 1

#define True  1
#define False 0

// Screen Resolution

#define SCREEN_RATIO 4

#define SCR_WIDTH (3840/SCREEN_RATIO)
#define SCR_HEIGHT (2160/SCREEN_RATIO)


/*
 *  Paths
 */
#define MODELS_PATH "models"
#define SHADER_PATH "shaders"


// Wrapper so assertions give us file/line numbers
#define AssertNoGlErrors {            \
  int glErrorNo = glGetError();       \
  DumpGlErrorEnum(glErrorNo);         \
  Assert(glErrorNo == GL_NO_ERROR); }



/*
 *  MSVC specific
 */
#ifdef _MSC_VER

#define GLOBAL_VARIABLE static
#define DEBUG_GLOBAL static

#define Assert(condition) if (!(condition)) __debugbreak();

#define VariadicOutputDebugString(FormatString, ...) {   \
    char Buffer[1024] = {};                              \
    _snprintf_s(Buffer, 1023, FormatString, __VA_ARGS__);  \
    OutputDebugString(Buffer); }


#define Info(...)  OutputDebugString(" - Info - "); VariadicOutputDebugString(__VA_ARGS__); OutputDebugString("\n")
#define Debug(...) OutputDebugString(__VA_ARGS__); VariadicOutputDebugString("\n")
#define Error(...) OutputDebugString(" ! Error - "); VariadicOutputDebugString(__VA_ARGS__); OutputDebugString("\n")

/*
 *  GCC on Linux && Cygwin
 */
#else

#define Assert(condition) if (!(condition)) raise(SIGTRAP);

// Hush up gcc about unreferenced globals
#define GLOBAL_VARIABLE static __attribute__((unused))
#define DEBUG_GLOBAL static __attribute__((unused))

#define Info(...)  printf(" - Info - "); printf(__VA_ARGS__); printf("\n")
#define Debug(...) printf(__VA_ARGS__); printf("\n")
#define Error(...) printf(" ! Error - "); printf(__VA_ARGS__); printf("\n")

#endif // _MSC_VER

#endif // BONSAI_PLAT_CONTANTS_H
