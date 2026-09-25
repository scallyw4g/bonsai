// callsite
// external/bonsai_debug/debug.h:133:0

// def (string_and_value_tables)
// external/bonsai_stdlib/src/poof_functions.h:2181:0
link_internal b32
IsValid(callgraph_window_view_mode Value)
{
  b32 Result = False;
  switch (Value)
  {
            case CallgraphWindowViewMode_Frame:

        case CallgraphWindowViewMode_Jobs:


    {
      Result = True;
    }
  }
  return Result;
}



link_internal counted_string
ToStringPrefixless(callgraph_window_view_mode Type)
{
  cs Result = {};
  if (IsValid(Type))
  {
    switch (Type)
    {
                  case CallgraphWindowViewMode_Frame: { Result = CSz("Frame"); } break;

            case CallgraphWindowViewMode_Jobs: { Result = CSz("Jobs"); } break;



      
    }
  }
  else
  {
    Result = CSz("(CORRUPT ENUM VALUE)");
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (enum_t.name)", Type); } */
  return Result;
}

link_internal counted_string
ToString(callgraph_window_view_mode Type)
{
  Assert(IsValid(Type));

  counted_string Result = {};
  switch (Type)
  {
            case CallgraphWindowViewMode_Frame: { Result = CSz("CallgraphWindowViewMode_Frame"); } break;

        case CallgraphWindowViewMode_Jobs: { Result = CSz("CallgraphWindowViewMode_Jobs"); } break;



    
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (enum_t.name)", Type); } */
  return Result;
}

link_internal callgraph_window_view_mode
CallgraphWindowViewMode(counted_string S)
{
  callgraph_window_view_mode Result = {};

    if (StringsMatch(S, CSz("CallgraphWindowViewMode_Frame"))) { return CallgraphWindowViewMode_Frame; }
  if (StringsMatch(S, CSz("CallgraphWindowViewMode_Jobs"))) { return CallgraphWindowViewMode_Jobs; }


  return Result;
}


