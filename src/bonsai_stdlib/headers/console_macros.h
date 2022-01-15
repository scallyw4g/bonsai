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

struct terminal_colors
{
  counted_string Red    = CSz("\x1b[31m");
  counted_string Blue   = CSz("\x1b[34m");
  counted_string Purple = CSz("\x1b[35m");
  counted_string Green  = CSz("\x1b[32m");
  counted_string Yellow = CSz("\x1b[33m");
  counted_string White  = CSz("\x1b[37m");
};


global_variable terminal_colors TerminalColors = {};

bonsai_function void
SetTerminalColorsOn()
{
  TerminalColors.Red    = CSz("\x1b[31m");
  TerminalColors.Blue   = CSz("\x1b[34m");
  TerminalColors.Purple = CSz("\x1b[35m");
  TerminalColors.Green  = CSz("\x1b[32m");
  TerminalColors.Yellow = CSz("\x1b[33m");
  TerminalColors.White  = CSz("\x1b[37m");
}

bonsai_function void
SetTerminalColorsOff()
{
  TerminalColors.Red    = CSz("");
  TerminalColors.Blue   = CSz("");
  TerminalColors.Purple = CSz("");
  TerminalColors.Green  = CSz("");
  TerminalColors.Yellow = CSz("");
  TerminalColors.White  = CSz("");
}

bonsai_function void SetupStdout(u32 ArgCount, const char** ArgStrings);

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
    Log("%S   Info    %S- ", TerminalColors.Blue, TerminalColors.White);  \
    Debug(__VA_ARGS__);                                    \
  }                                                        \
                                                           \
} while (false)



#define Error(...) do {                                   \
                                                          \
  if (Global_LogLevel <= LogLevel_Error) {                \
    Log("%S ! Error   %S- ", TerminalColors.Red, TerminalColors.White);  \
    Debug(__VA_ARGS__);                                   \
  }                                                       \
                                                          \
} while (false)


#define Warn(...) do {                                      \
                                                            \
  if (Global_LogLevel <= LogLevel_Normal) {                 \
    Log("%S * Warning %S- ", TerminalColors.Yellow, TerminalColors.White); \
    Debug(__VA_ARGS__);                                     \
  }                                                         \
                                                            \
} while (false)

#define LogSuccess(...) do {                                  \
                                                           \
  if (Global_LogLevel <= LogLevel_Normal) {                \
    Log("%S   Success %S- ", TerminalColors.Green, TerminalColors.White); \
    Debug(__VA_ARGS__);                                    \
  }                                                        \
                                                           \
} while (false)

#define OpenGlDebugMessage(...) do {                                     \
                                                                         \
  if (Global_LogLevel <= LogLevel_Debug) {                               \
    Log("%S * OpenGl Debug Message %S- ", TerminalColors.Yellow, TerminalColors.White); \
    Debug(__VA_ARGS__);                                                  \
  }                                                                      \
                                                                         \
} while (false)
