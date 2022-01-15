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


global_variable log_level Global_LogLevel = LogLevel_Debug;


#define Log(...) PrintToStdout(FormatCountedString_(TempDebugOutputBuffer__, TempDebugOutputBufferSize, __VA_ARGS__))


#define TestOut(...)  \
    Log(__VA_ARGS__); \
    Log(Newline)


#define DebugChars(...) do {                 \
                                             \
  if (Global_LogLevel <= LogLevel_Debug) {   \
    Log(__VA_ARGS__);                        \
  }                                          \
                                             \
} while (false)


#define Debug(...) do {                      \
                                             \
  if (Global_LogLevel <= LogLevel_Debug) {   \
    Log(__VA_ARGS__);                        \
    Log(Newline);                            \
  }                                          \
                                             \
} while (false)



#define Info(...) do {                                     \
                                                           \
  if (Global_LogLevel <= LogLevel_Normal) {                \
    Log(BLUE_TERMINAL "   Info   " WHITE_TERMINAL " - ");  \
    Debug(__VA_ARGS__);                                    \
  }                                                        \
                                                           \
} while (false)



#define Error(...) do {                                   \
                                                          \
  if (Global_LogLevel <= LogLevel_Error) {                \
    Log(RED_TERMINAL " ! Error  " WHITE_TERMINAL " - ");  \
    Debug(__VA_ARGS__);                                   \
  }                                                       \
                                                          \
} while (false)


#define Warn(...) do {                                      \
                                                            \
  if (Global_LogLevel <= LogLevel_Normal) {                 \
    Log(YELLOW_TERMINAL " * Warning" WHITE_TERMINAL " - "); \
    Debug(__VA_ARGS__);                                     \
  }                                                         \
                                                            \
} while (false)

#define LogSuccess(...) do {                                  \
                                                           \
  if (Global_LogLevel <= LogLevel_Normal) {                \
    Log(GREEN_TERMINAL " ✓ " WHITE_TERMINAL " - "); \
    Debug(__VA_ARGS__);                                    \
  }                                                        \
                                                           \
} while (false)

#define OpenGlDebugMessage(...) do {                                     \
                                                                         \
  if (Global_LogLevel <= LogLevel_Debug) {                               \
    Log(YELLOW_TERMINAL " * OpenGl Debug Message" WHITE_TERMINAL " - "); \
    Debug(__VA_ARGS__);                                                  \
  }                                                                      \
                                                                         \
} while (false)
