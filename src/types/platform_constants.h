/*
 *  Common
 */

#define RED_TERMINAL "\x1b[31m"
#define BLUE_TERMINAL "\x1b[34m"
#define GREEN_TERMINAL "\x1b[32m"
#define YELLOW_TERMINAL "\x1b[33m"
#define WHITE_TERMINAL "\x1b[37m"

#define Newline "\n"

// Screen Resolution

#define SCREEN_RATIO 1

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

#define GlDebugMessage(...)  PrintConsole(" * Gl Debug Message - ");             \
                             VariadicOutputDebugString(__VA_ARGS__); \
                             PrintConsole("\n")

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
 *  Clang
 */
#else

#define BONSAI_FUNCTION_NAME __func__


#define Info(...)                                                  \
  LogToConsole(CSz(BLUE_TERMINAL "   Info" WHITE_TERMINAL " - ")); \
  printf(__VA_ARGS__);                                             \
  LogToConsole(CSz("\n"))

#define Debug(...)     \
  printf(__VA_ARGS__); \
  LogToConsole(CSz("\n"))

#define Error(...)                                                 \
  LogToConsole(CSz(RED_TERMINAL " ! Error" WHITE_TERMINAL " - ")); \
  printf(__VA_ARGS__);                                             \
  LogToConsole(CSz("\n"))

#define Warn(...)                                                    \
  LogToConsole(CSz(YELLOW_TERMINAL " * Warn" WHITE_TERMINAL " - ")); \
  printf(__VA_ARGS__);                                               \
  LogToConsole(CSz("\n"))

#define OpenGlDebugMessage(...)                                                      \
  LogToConsole(CSz(YELLOW_TERMINAL " * OpenGl Debug Message" WHITE_TERMINAL " - ")); \
  printf(__VA_ARGS__);                                                               \
  LogToConsole(CSz("\n"))

#define RuntimeBreak() raise(SIGTRAP)
#define TriggeredRuntimeBreak() if (GetDebugState) { GetDebugState()->TriggerRuntimeBreak ? RuntimeBreak() : 0 ; }


#endif // _MSC_VER
