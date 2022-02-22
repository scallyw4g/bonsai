global_variable const umm TempDebugOutputBufferSize = 64*1024;
global_variable char TempDebugOutputBuffer__[TempDebugOutputBufferSize];

bonsai_function void PrintToStdout(counted_string S);

enum log_level
{
  LogLevel_Undefined,

  LogLevel_Debug,
  LogLevel_Normal,
  LogLevel_Error,
  LogLevel_Shush,
};

struct terminal_colors
{
  counted_string Red          = CSz("\x1b[31m");
  counted_string Blue         = CSz("\x1b[34m");
  counted_string Purple       = CSz("\x1b[35m");
  counted_string Green        = CSz("\x1b[32m");
  counted_string Yellow       = CSz("\x1b[33m");
  counted_string BrightYellow = CSz("\x1b[93m");
  counted_string White        = CSz("\x1b[37m");
  counted_string Grey         = CSz("\x1b[38;5;242m");
};

global_variable terminal_colors TerminalColors = {};

#if 0
// TODO(Jesse): Metaprogram this?  ATM we only ever turn colors off
bonsai_function void
SetTerminalColorsOn()
{
  TerminalColors.Red          = CSz("\x1b[31m");
  TerminalColors.Blue         = CSz("\x1b[34m");
  TerminalColors.Purple       = CSz("\x1b[35m");
  TerminalColors.Green        = CSz("\x1b[32m");
  TerminalColors.Yellow       = CSz("\x1b[33m");
  TerminalColors.BrightYellow = CSz("\x1b[93m");
  TerminalColors.White        = CSz("\x1b[37m");
}
#endif

// TODO(Jesse): Metaprogram this
bonsai_function void
SetTerminalColorsOff()
{
#if 1
  TerminalColors.Red          = CSz("");
  TerminalColors.Blue         = CSz("");
  TerminalColors.Purple       = CSz("");
  TerminalColors.Green        = CSz("");
  TerminalColors.Yellow       = CSz("");
  TerminalColors.BrightYellow = CSz("");
  TerminalColors.White        = CSz("");
  TerminalColors.Grey         = CSz("");
#endif
}

bonsai_function void SetupStdout(u32 ArgCount, const char** ArgStrings);




#define LogDirect(...) PrintToStdout(FormatCountedString_(TempDebugOutputBuffer__, TempDebugOutputBufferSize, __VA_ARGS__))


#define DebugChars(...) do {                  \
                                              \
  if (Global_LogLevel <= LogLevel_Debug) {    \
    LogDirect(__VA_ARGS__);                   \
  }                                           \
                                              \
} while (false)


#define DebugLine(...) do {                                                    \
                                                                               \
  if (Global_LogLevel <= LogLevel_Debug) {                                     \
    LogDirect(__VA_ARGS__);                                                    \
    LogDirect(Newline);                                                        \
  }                                                                            \
                                                                               \
} while (false)


#define DebugMessage(...) do {                                                 \
                                                                               \
  if (Global_LogLevel <= LogLevel_Debug) {                                     \
    LogDirect("%S   Debug   %S- ", TerminalColors.Blue, TerminalColors.White); \
    LogDirect(__VA_ARGS__);                                                    \
    LogDirect(Newline);                                                        \
  }                                                                            \
                                                                               \
} while (false)



#define Info(...) do {                                                           \
                                                                                 \
  if (Global_LogLevel <= LogLevel_Normal) {                                      \
    LogDirect("%S   Info    %S- ", TerminalColors.Blue, TerminalColors.White);   \
    LogDirect(__VA_ARGS__);                                                      \
    LogDirect(Newline);                                                          \
  }                                                                              \
                                                                                 \
} while (false)



#define Error(...) do {                                                         \
                                                                                \
  if (Global_LogLevel <= LogLevel_Error) {                                      \
    LogDirect("%S ! Error   %S- ", TerminalColors.Red, TerminalColors.White);   \
    LogDirect(__VA_ARGS__);                                                     \
    LogDirect(Newline);                                                         \
    RuntimeBreak();                                                             \
  }                                                                             \
                                                                                \
} while (false)


#define Warn(...) do {                                                             \
                                                                                   \
  if (Global_LogLevel <= LogLevel_Normal) {                                        \
    LogDirect("%S * Warning %S- ", TerminalColors.Yellow, TerminalColors.White);   \
    LogDirect(__VA_ARGS__);                                                        \
    LogDirect(Newline);                                                            \
  }                                                                                \
                                                                                   \
} while (false)

#define LogSuccess(...) do {                                                      \
                                                                                  \
  if (Global_LogLevel <= LogLevel_Normal) {                                       \
    LogDirect("%S   Success %S- ", TerminalColors.Green, TerminalColors.White);   \
    LogDirect(__VA_ARGS__);                                                       \
    LogDirect(Newline);                                                           \
  }                                                                               \
                                                                                  \
} while (false)

#define OpenGlDebugMessage(...) do {                                                            \
                                                                                                \
  if (Global_LogLevel <= LogLevel_Debug) {                                                      \
    LogDirect("%S * OpenGl Debug Message %S- ", TerminalColors.Yellow, TerminalColors.White);   \
    LogDirect(__VA_ARGS__);                                                                     \
    LogDirect(Newline);                                                                         \
  }                                                                                             \
                                                                                                \
} while (false)
