    bonsai_function counted_string
    ToString( parse_error_code Type)
    {
      counted_string Result = {};
      switch (Type)
      {
                    case ParseErrorCode_None: { Result = CSz("ParseErrorCode_None"); } break;
            case ParseErrorCode_ExpectedSemicolonOrEquals: { Result = CSz("ParseErrorCode_ExpectedSemicolonOrEquals"); } break;
            case ParseErrorCode_StreamEndedUnexpectedly: { Result = CSz("ParseErrorCode_StreamEndedUnexpectedly"); } break;
            case ParseErrorCode_RequireTokenFailed: { Result = CSz("ParseErrorCode_RequireTokenFailed"); } break;
            case ParseErrorCode_InvalidTokenGenerated: { Result = CSz("ParseErrorCode_InvalidTokenGenerated"); } break;
            case ParseErrorCode_DUnionParse: { Result = CSz("ParseErrorCode_DUnionParse"); } break;
            case ParseErrorCode_Unknown: { Result = CSz("ParseErrorCode_Unknown"); } break;

      }
      return Result;
    }

