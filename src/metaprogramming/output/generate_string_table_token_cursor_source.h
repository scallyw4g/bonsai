    bonsai_function counted_string
    ToString( token_cursor_source Type)
    {
      counted_string Result = {};
      switch (Type)
      {
                    case TokenCursorSource_Unknown: { Result = CSz("TokenCursorSource_Unknown"); } break;
            case TokenCursorSource_RootFile: { Result = CSz("TokenCursorSource_RootFile"); } break;
            case TokenCursorSource_Include: { Result = CSz("TokenCursorSource_Include"); } break;
            case TokenCursorSource_MacroExpansion: { Result = CSz("TokenCursorSource_MacroExpansion"); } break;
            case TokenCursorSource_MetaprogrammingExpansion: { Result = CSz("TokenCursorSource_MetaprogrammingExpansion"); } break;
            case TokenCursorSource_PasteOperator: { Result = CSz("TokenCursorSource_PasteOperator"); } break;
            case TokenCursorSource_CommandLineOption: { Result = CSz("TokenCursorSource_CommandLineOption"); } break;
            case TokenCursorSource_BodyText: { Result = CSz("TokenCursorSource_BodyText"); } break;
            case TokenCursorSource_IntermediateRepresentaton: { Result = CSz("TokenCursorSource_IntermediateRepresentaton"); } break;
            case TokenCursorSource_Count: { Result = CSz("TokenCursorSource_Count"); } break;

      }
      return Result;
    }

