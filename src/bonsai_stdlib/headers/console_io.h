// NOTE(Jesse): This is here such that we can see "StringsMatch()"
//
meta(string_and_value_tables(log_level));
#include <metaprogramming/output/string_and_value_tables_log_level.h>

global_variable native_file Stdout =
{
  .Handle = stdout,
  .Path = CSz("stdout")
};

bonsai_function void
SetupStdout(u32 ArgCount, const char** ArgStrings)
{
  setvbuf(stdout, 0, _IONBF, 0);
  setvbuf(stderr, 0, _IONBF, 0);

  for ( u32 ArgIndex = 1;
        ArgIndex < ArgCount;
        ++ArgIndex)
  {
    counted_string Arg = CS(ArgStrings[ArgIndex]);

    if (StringsMatch(CS("-c0"), Arg) ||
        StringsMatch(CS("--colors-off"), Arg) )
    {
      SetTerminalColorsOff();
    }
    else if ( StringsMatch(CS("--log-level"), Arg) )
    {
      if (ArgIndex+1 < ArgCount)
      {
        ArgIndex += 1;
        counted_string LevelString = CS(ArgStrings[ArgIndex]);
        log_level Level = LogLevel(LevelString);
        if (Level > LogLevel_Undefined)
        {
          Global_LogLevel = Level;
          Info("Setting Global_LogLevel to %S", LevelString);
        }
        else
        {
          Warn("Invalid --log-level switch value (%S)", LevelString);
          DebugChars("           - Valid values are [ ");
          for (u32 LevelIndex = LogLevel_Debug;
              LevelIndex <= LogLevel_Shush;
              ++LevelIndex)
          {
            DebugChars("%S", ToString((log_level)LevelIndex));
            if (LevelIndex < LogLevel_Shush) { DebugChars(", "); } else { DebugChars(" ]"); }
          }

        }

      }
      else
      {
        // TODO(Jesse): Helpfully specify what the accepted values are.
        Error("Log Level required when using the --log-level switch.");
      }
    }

  }

  return;
}
