#include <emscripten/emscripten.h>
#include <emscripten/html5.h>

#define RED_TERMINAL ""
#define BLUE_TERMINAL ""
#define GREEN_TERMINAL ""
#define YELLOW_TERMINAL ""
#define WHITE_TERMINAL ""

#define FullBarrier  asm volatile("" ::: "memory"); _mm_sfence();

inline u64
GetCycleCount()
{
  u64 Result = 0;
  return Result;
}

void PlatformDebugStacktrace();

typedef umm gl_context;
struct os
{
  gl_context GlContext;
  b32 ContinueRunning = True;
};
