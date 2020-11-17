global_variable const umm TempDebugOutputBufferSize = 4096;
global_variable char TempDebugOutputBuffer__[TempDebugOutputBufferSize];

#define Debug(...)                                                                                     \
  LogToConsole(FormatCountedString_(TempDebugOutputBuffer__, TempDebugOutputBufferSize, __VA_ARGS__)); \
  LogToConsole(CSz(Newline))

#define Info(...)                                                     \
  LogToConsole(CSz(BLUE_TERMINAL "   Info   " WHITE_TERMINAL " - ")); \
  Debug(__VA_ARGS__);                                                 \

#define Error(...)                                                   \
  LogToConsole(CSz(RED_TERMINAL " ! Error  " WHITE_TERMINAL " - ")); \
  Debug(__VA_ARGS__);                                                \

#define Warn(...)                                                       \
  LogToConsole(CSz(YELLOW_TERMINAL " * Warning" WHITE_TERMINAL " - ")); \
  Debug(__VA_ARGS__);                                                   \

#define Success(...)                                                   \
  LogToConsole(CSz(GREEN_TERMINAL " ✓ Success" WHITE_TERMINAL " - ")); \
  Debug(__VA_ARGS__);                                                  \

#define OpenGlDebugMessage(...)                                                      \
  LogToConsole(CSz(YELLOW_TERMINAL " * OpenGl Debug Message" WHITE_TERMINAL " - ")); \
  Debug(__VA_ARGS__);                                                                \

