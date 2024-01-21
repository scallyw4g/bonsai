link_internal counted_string
ToString(parse_error_code Type)
{
  counted_string Result = {};
  switch (Type)
  {
    case ParseErrorCode_None: { Result = CSz("None"); } break;
    case ParseErrorCode_ExpectedSemicolonOrEquals: { Result = CSz("ExpectedSemicolonOrEquals"); } break;
    case ParseErrorCode_StreamEndedUnexpectedly: { Result = CSz("StreamEndedUnexpectedly"); } break;
    case ParseErrorCode_RequireTokenFailed: { Result = CSz("RequireTokenFailed"); } break;
    case ParseErrorCode_InvalidTokenGenerated: { Result = CSz("InvalidTokenGenerated"); } break;
    case ParseErrorCode_MalformedType: { Result = CSz("MalformedType"); } break;
    case ParseErrorCode_PoofUserlandError: { Result = CSz("PoofUserlandError"); } break;
    case ParseErrorCode_PoofTypeError: { Result = CSz("PoofTypeError"); } break;
    case ParseErrorCode_DUnionParse: { Result = CSz("DUnionParse"); } break;
    case ParseErrorCode_UndefinedDatatype: { Result = CSz("UndefinedDatatype"); } break;
    case ParseErrorCode_InvalidDirective: { Result = CSz("InvalidDirective"); } break;
    case ParseErrorCode_InvalidOperator: { Result = CSz("InvalidOperator"); } break;
    case ParseErrorCode_InvalidMetaTransformOp: { Result = CSz("InvalidMetaTransformOp"); } break;
    case ParseErrorCode_InvalidArgument: { Result = CSz("InvalidArgument"); } break;
    case ParseErrorCode_InvalidArgumentType: { Result = CSz("InvalidArgumentType"); } break;
    case ParseErrorCode_InvalidArgumentCount: { Result = CSz("InvalidArgumentCount"); } break;
    case ParseErrorCode_InvalidName: { Result = CSz("InvalidName"); } break;
    case ParseErrorCode_InvalidTag: { Result = CSz("InvalidTag"); } break;
    case ParseErrorCode_InvalidFunction: { Result = CSz("InvalidFunction"); } break;
    case ParseErrorCode_SyntaxError: { Result = CSz("SyntaxError"); } break;
    case ParseErrorCode_NotImplemented: { Result = CSz("NotImplemented"); } break;
    case ParseErrorCode_InputStreamNull: { Result = CSz("InputStreamNull"); } break;
    case ParseErrorCode_InternalCompilerError: { Result = CSz("InternalCompilerError"); } break;
    case ParseErrorCode_StackOverflow: { Result = CSz("StackOverflow"); } break;
    case ParseErrorCode_Unknown: { Result = CSz("Unknown"); } break;

    
  }
  return Result;
}

