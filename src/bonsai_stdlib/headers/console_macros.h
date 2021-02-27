global_variable const umm TempDebugOutputBufferSize = 4096;
global_variable char TempDebugOutputBuffer__[TempDebugOutputBufferSize];

bonsai_function void PrintToStdout(counted_string S);

#define PrintDebugLine(...)     \
  PrintToStdout(FormatCountedString_(TempDebugOutputBuffer__, TempDebugOutputBufferSize, __VA_ARGS__)); \
  PrintToStdout(CSz(Newline))

#define Debug(...) \
  PrintDebugLine(__VA_ARGS__)

#define Info(...)                                                          \
  PrintToStdout(CSz(BLUE_TERMINAL "   Info   " WHITE_TERMINAL " - ")); \
  Debug(__VA_ARGS__)

#define Error(...)                                                        \
  PrintToStdout(CSz(RED_TERMINAL " ! Error  " WHITE_TERMINAL " - ")); \
  Debug(__VA_ARGS__)

#define Warn(...)                                                            \
  PrintToStdout(CSz(YELLOW_TERMINAL " * Warning" WHITE_TERMINAL " - ")); \
  Debug(__VA_ARGS__)

#define Success(...)                                                          \
  PrintToStdout(CSz(GREEN_TERMINAL " ✓ Success" WHITE_TERMINAL " - ")); \
  Debug(__VA_ARGS__)

#define OpenGlDebugMessage(...)                                                           \
  PrintToStdout(CSz(YELLOW_TERMINAL " * OpenGl Debug Message" WHITE_TERMINAL " - ")); \
  Debug(__VA_ARGS__)

