    bonsai_function counted_string
    ToString( log_level Type)
    {
      counted_string Result = {};
      switch (Type)
      {
                    case LogLevel_Debug: { Result = CSz("LogLevel_Debug"); } break;
            case LogLevel_Normal: { Result = CSz("LogLevel_Normal"); } break;
            case LogLevel_Error: { Result = CSz("LogLevel_Error"); } break;
            case LogLevel_Shush: { Result = CSz("LogLevel_Shush"); } break;

      }
      return Result;
    }

