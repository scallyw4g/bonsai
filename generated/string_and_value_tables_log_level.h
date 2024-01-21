link_internal counted_string
ToString(log_level Type)
{
  counted_string Result = {};
  switch (Type)
  {
    case LogLevel_Undefined: { Result = CSz("Undefined"); } break;
    case LogLevel_Verbose: { Result = CSz("Verbose"); } break;
    case LogLevel_Debug: { Result = CSz("Debug"); } break;
    case LogLevel_Info: { Result = CSz("Info"); } break;
    case LogLevel_Error: { Result = CSz("Error"); } break;
    case LogLevel_Shush: { Result = CSz("Shush"); } break;

    
  }
  return Result;
}

link_internal log_level
LogLevel(counted_string S)
{
  log_level Result = {};

  if (StringsMatch(S, CSz("LogLevel_Undefined"))) { return LogLevel_Undefined; }
  if (StringsMatch(S, CSz("LogLevel_Verbose"))) { return LogLevel_Verbose; }
  if (StringsMatch(S, CSz("LogLevel_Debug"))) { return LogLevel_Debug; }
  if (StringsMatch(S, CSz("LogLevel_Info"))) { return LogLevel_Info; }
  if (StringsMatch(S, CSz("LogLevel_Error"))) { return LogLevel_Error; }
  if (StringsMatch(S, CSz("LogLevel_Shush"))) { return LogLevel_Shush; }

  return Result;
}


