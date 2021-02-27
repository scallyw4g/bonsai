global_variable const umm TempDebugOutputBufferSize = 4096;
global_variable char TempDebugOutputBuffer__[TempDebugOutputBufferSize];

bonsai_function void PrintToStdout(counted_string S);

#define Log(...) PrintToStdout(FormatCountedString_(TempDebugOutputBuffer__, TempDebugOutputBufferSize, __VA_ARGS__))

#define Debug(...) \
  Log(__VA_ARGS__); \
  Log(Newline)

#define Info(...)                                                          \
  Log(BLUE_TERMINAL "   Info   " WHITE_TERMINAL " - "); \
  Debug(__VA_ARGS__)

#define Error(...)                                                        \
  Log(RED_TERMINAL " ! Error  " WHITE_TERMINAL " - "); \
  Debug(__VA_ARGS__)

#define Warn(...)                                                            \
  Log(YELLOW_TERMINAL " * Warning" WHITE_TERMINAL " - "); \
  Debug(__VA_ARGS__)

#define Success(...)                                                          \
  Log(GREEN_TERMINAL " ✓ Success" WHITE_TERMINAL " - "); \
  Debug(__VA_ARGS__)

#define OpenGlDebugMessage(...)                                                           \
  Log(YELLOW_TERMINAL " * OpenGl Debug Message" WHITE_TERMINAL " - "); \
  Debug(__VA_ARGS__)

