        bonsai_function counted_string
    ToString( log_level Type)
    {
      counted_string Result = {};
      switch (Type)
      {
                  case LogLevel_Undefined: { Result = CSz("LogLevel_Undefined"); } break;
          case LogLevel_Debug: { Result = CSz("LogLevel_Debug"); } break;
          case LogLevel_Normal: { Result = CSz("LogLevel_Normal"); } break;
          case LogLevel_Error: { Result = CSz("LogLevel_Error"); } break;
          case LogLevel_Shush: { Result = CSz("LogLevel_Shush"); } break;

      }
      return Result;
    }

        bonsai_function log_level
    LogLevel(counted_string S)
    {
      log_level Result = {};

              if (StringsMatch(S, CSz("LogLevel_Undefined"))) { return LogLevel_Undefined; }
        if (StringsMatch(S, CSz("LogLevel_Debug"))) { return LogLevel_Debug; }
        if (StringsMatch(S, CSz("LogLevel_Normal"))) { return LogLevel_Normal; }
        if (StringsMatch(S, CSz("LogLevel_Error"))) { return LogLevel_Error; }
        if (StringsMatch(S, CSz("LogLevel_Shush"))) { return LogLevel_Shush; }


      return Result;
    }


