global_variable const umm TempDebugOutputBufferSize = 4096;
global_variable char TempDebugOutputBuffer__[TempDebugOutputBufferSize];

bonsai_function void PrintToStdout(counted_string S);

enum log_level
{
  LogLevel_Debug,
  LogLevel_Normal,
  LogLevel_Error,
  LogLevel_Shush,
};


global_variable log_level Global_LogLevel = LogLevel_Shush;


#define Log_Internal(...) PrintToStdout(FormatCountedString_(TempDebugOutputBuffer__, TempDebugOutputBufferSize, __VA_ARGS__))


#define TestOut(...)  \
    Log_Internal(__VA_ARGS__); \
    Log_Internal(Newline)


#define Debug(...) do {                      \
                                             \
  if (Global_LogLevel <= LogLevel_Debug) {   \
    Log_Internal(__VA_ARGS__);                        \
    Log_Internal(Newline);                            \
  }                                          \
                                             \
} while (false)



#define Info(...) do {                                     \
                                                           \
  if (Global_LogLevel <= LogLevel_Normal) {                \
    Log_Internal(BLUE_TERMINAL "   Info   " WHITE_TERMINAL " - ");  \
    Debug(__VA_ARGS__);                                    \
  }                                                        \
                                                           \
} while (false)



#define Error(...) do {                                   \
                                                          \
  if (Global_LogLevel <= LogLevel_Error) {                \
    Log_Internal(RED_TERMINAL " ! Error  " WHITE_TERMINAL " - ");  \
    Debug(__VA_ARGS__);                                   \
  }                                                       \
                                                          \
} while (false)


#define Warn(...) do {                                      \
                                                            \
  if (Global_LogLevel <= LogLevel_Normal) {                 \
    Log_Internal(YELLOW_TERMINAL " * Warning" WHITE_TERMINAL " - "); \
    Debug(__VA_ARGS__);                                     \
  }                                                         \
                                                            \
} while (false)

#define LogSuccess(...) do {                                  \
                                                           \
  if (Global_LogLevel <= LogLevel_Normal) {                \
    Log_Internal(GREEN_TERMINAL " ✓ foo" WHITE_TERMINAL " - "); \
    Debug(__VA_ARGS__);                                    \
  }                                                        \
                                                           \
} while (false)

#define OpenGlDebugMessage(...) do {                                     \
                                                                         \
  if (Global_LogLevel <= LogLevel_Debug) {                               \
    Log_Internal(YELLOW_TERMINAL " * OpenGl Debug Message" WHITE_TERMINAL " - "); \
    Debug(__VA_ARGS__);                                                  \
  }                                                                      \
                                                                         \
} while (false)
