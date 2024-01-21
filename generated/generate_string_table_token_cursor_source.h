link_internal counted_string
ToString(token_cursor_source Type)
{
  counted_string Result = {};
  switch (Type)
  {
    case TokenCursorSource_Unknown: { Result = CSz("Unknown"); } break;
    case TokenCursorSource_RootFile: { Result = CSz("RootFile"); } break;
    case TokenCursorSource_Include: { Result = CSz("Include"); } break;
    case TokenCursorSource_MacroExpansion: { Result = CSz("MacroExpansion"); } break;
    case TokenCursorSource_MetaprogrammingExpansion: { Result = CSz("MetaprogrammingExpansion"); } break;
    case TokenCursorSource_PoofSymbolIteration: { Result = CSz("PoofSymbolIteration"); } break;
    case TokenCursorSource_PasteOperator: { Result = CSz("PasteOperator"); } break;
    case TokenCursorSource_CommandLineOption: { Result = CSz("CommandLineOption"); } break;
    case TokenCursorSource_BodyText: { Result = CSz("BodyText"); } break;
    case TokenCursorSource_IntermediateRepresentaton: { Result = CSz("IntermediateRepresentaton"); } break;
    case TokenCursorSource_Count: { Result = CSz("Count"); } break;

    
  }
  return Result;
}

