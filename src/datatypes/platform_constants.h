/*
 *  Common
 */

// Screen Resolution

#define SCREEN_RATIO 2

#define SCR_WIDTH (3840/SCREEN_RATIO)
#define SCR_HEIGHT (2160/SCREEN_RATIO)

/*
 *  Paths
 */
#define MODELS_PATH "models"
#define SHADER_PATH "shaders"
#define SHADER_HEADER "/header.glsl"


// Wrapper so assertions give us file/line numbers
#define AssertNoGlErrors {            \
  u32 glErrorNo = glGetError();       \
  DumpGlErrorEnum(glErrorNo);         \
  Assert(glErrorNo == GL_NO_ERROR); }



/*
 * Memory
 */

#define MEMPROTECT (MEMPROTECT_OVERFLOW || MEMPROTECT_UNDERFLOW)
#define MEMPROTECT_UNDERFLOW (0)
#define MEMPROTECT_OVERFLOW (1)

#if MEMPROTECT_UNDERFLOW && MEMPROTECT_OVERFLOW
#error "Unfortunately, Underflow and Overflow protection at the same time is impossible"
#endif



/*
 *  MSVC
 */
#ifdef _MSC_VER

#define BONSAI_FUNCTION_NAME __FUNCTION__

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

#define BONSAI_FUNCTION_NAME __func__

#define Info(...)  printf("   Info - ");  printf(__VA_ARGS__); printf("\n")
#define Debug(...) printf(__VA_ARGS__);   printf("\n")
#define Error(...) printf(" ! Error - "); printf(__VA_ARGS__); printf("\n")
#define Warn(...)  printf(" * Warn - "); printf(__VA_ARGS__); printf("\n")

#define RuntimeBreak() raise(SIGTRAP)
#define TriggeredRuntimeBreak() Global_TriggerRuntimeBreak ? RuntimeBreak() : 0 ;


#endif // _MSC_VER
