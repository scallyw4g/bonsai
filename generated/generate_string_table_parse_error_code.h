link_internal counted_string
ToString(parse_error_code Type)
{
  counted_string Result = {};
  switch (Type)
  {
    case ParseErrorCode_None: { Result = CSz("ParseErrorCode_None"); } break;
    case ParseErrorCode_ExpectedSemicolonOrEquals: { Result = CSz("ParseErrorCode_ExpectedSemicolonOrEquals"); } break;
    case ParseErrorCode_StreamEndedUnexpectedly: { Result = CSz("ParseErrorCode_StreamEndedUnexpectedly"); } break;
    case ParseErrorCode_RequireTokenFailed: { Result = CSz("ParseErrorCode_RequireTokenFailed"); } break;
    case ParseErrorCode_InvalidTokenGenerated: { Result = CSz("ParseErrorCode_InvalidTokenGenerated"); } break;
    case ParseErrorCode_MalformedType: { Result = CSz("ParseErrorCode_MalformedType"); } break;
    case ParseErrorCode_PoofUserlandError: { Result = CSz("ParseErrorCode_PoofUserlandError"); } break;
    case ParseErrorCode_PoofTypeError: { Result = CSz("ParseErrorCode_PoofTypeError"); } break;
    case ParseErrorCode_DUnionParse: { Result = CSz("ParseErrorCode_DUnionParse"); } break;
    case ParseErrorCode_UndefinedDatatype: { Result = CSz("ParseErrorCode_UndefinedDatatype"); } break;
    case ParseErrorCode_InvalidOperator: { Result = CSz("ParseErrorCode_InvalidOperator"); } break;
    case ParseErrorCode_InvalidMetaTransformOp: { Result = CSz("ParseErrorCode_InvalidMetaTransformOp"); } break;
    case ParseErrorCode_InvalidArgument: { Result = CSz("ParseErrorCode_InvalidArgument"); } break;
    case ParseErrorCode_InvalidArgumentType: { Result = CSz("ParseErrorCode_InvalidArgumentType"); } break;
    case ParseErrorCode_InvalidArgumentCount: { Result = CSz("ParseErrorCode_InvalidArgumentCount"); } break;
    case ParseErrorCode_InvalidName: { Result = CSz("ParseErrorCode_InvalidName"); } break;
    case ParseErrorCode_InvalidFunction: { Result = CSz("ParseErrorCode_InvalidFunction"); } break;
    case ParseErrorCode_SyntaxError: { Result = CSz("ParseErrorCode_SyntaxError"); } break;
    case ParseErrorCode_NotImplemented: { Result = CSz("ParseErrorCode_NotImplemented"); } break;
    case ParseErrorCode_InputStreamNull: { Result = CSz("ParseErrorCode_InputStreamNull"); } break;
    case ParseErrorCode_InternalCompilerError: { Result = CSz("ParseErrorCode_InternalCompilerError"); } break;
    case ParseErrorCode_StackOverflow: { Result = CSz("ParseErrorCode_StackOverflow"); } break;
    case ParseErrorCode_Unknown: { Result = CSz("ParseErrorCode_Unknown"); } break;
  }
  return Result;
}

