// callsite
// external/bonsai_stdlib/src/c_parser.h:62:0

// def (generate_string_table)
// external/bonsai_stdlib/src/poof_functions.h:1821:0
link_internal b32
IsValid(parse_error_code Value)
{
  b32 Result = False;
  switch (Value)
  {
        case ParseErrorCode_None:
    case ParseErrorCode_ExpectedSemicolonOrEquals:
    case ParseErrorCode_StreamEndedUnexpectedly:
    case ParseErrorCode_RequireTokenFailed:
    case ParseErrorCode_InvalidTokenGenerated:
    case ParseErrorCode_MalformedType:
    case ParseErrorCode_PoofUserlandError:
    case ParseErrorCode_PoofTypeError:
    case ParseErrorCode_DUnionParse:
    case ParseErrorCode_UndefinedDatatype:
    case ParseErrorCode_InvalidKeyword:
    case ParseErrorCode_InvalidDirective:
    case ParseErrorCode_InvalidOperator:
    case ParseErrorCode_InvalidMetaTransformOp:
    case ParseErrorCode_InvalidArgument:
    case ParseErrorCode_InvalidArgumentType:
    case ParseErrorCode_InvalidArgumentCount:
    case ParseErrorCode_InvalidName:
    case ParseErrorCode_InvalidTag:
    case ParseErrorCode_InvalidFunction:
    case ParseErrorCode_SyntaxError:
    case ParseErrorCode_NotImplemented:
    case ParseErrorCode_InputStreamNull:
    case ParseErrorCode_InternalCompilerError:
    case ParseErrorCode_StackOverflow:
    case ParseErrorCode_Unknown:

    {
      Result = True;
    }
  }
  return Result;
}



link_internal counted_string
ToStringPrefixless(parse_error_code Type)
{
  cs Result = {};
  if (IsValid(Type))
  {
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
      case ParseErrorCode_InvalidKeyword: { Result = CSz("InvalidKeyword"); } break;
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
  }
  else
  {
    Result = CSz("(CORRUPT ENUM VALUE)");
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (enum_t.name)", Type); } */
  return Result;
}

link_internal counted_string
ToString(parse_error_code Type)
{
  Assert(IsValid(Type));

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
    case ParseErrorCode_InvalidKeyword: { Result = CSz("ParseErrorCode_InvalidKeyword"); } break;
    case ParseErrorCode_InvalidDirective: { Result = CSz("ParseErrorCode_InvalidDirective"); } break;
    case ParseErrorCode_InvalidOperator: { Result = CSz("ParseErrorCode_InvalidOperator"); } break;
    case ParseErrorCode_InvalidMetaTransformOp: { Result = CSz("ParseErrorCode_InvalidMetaTransformOp"); } break;
    case ParseErrorCode_InvalidArgument: { Result = CSz("ParseErrorCode_InvalidArgument"); } break;
    case ParseErrorCode_InvalidArgumentType: { Result = CSz("ParseErrorCode_InvalidArgumentType"); } break;
    case ParseErrorCode_InvalidArgumentCount: { Result = CSz("ParseErrorCode_InvalidArgumentCount"); } break;
    case ParseErrorCode_InvalidName: { Result = CSz("ParseErrorCode_InvalidName"); } break;
    case ParseErrorCode_InvalidTag: { Result = CSz("ParseErrorCode_InvalidTag"); } break;
    case ParseErrorCode_InvalidFunction: { Result = CSz("ParseErrorCode_InvalidFunction"); } break;
    case ParseErrorCode_SyntaxError: { Result = CSz("ParseErrorCode_SyntaxError"); } break;
    case ParseErrorCode_NotImplemented: { Result = CSz("ParseErrorCode_NotImplemented"); } break;
    case ParseErrorCode_InputStreamNull: { Result = CSz("ParseErrorCode_InputStreamNull"); } break;
    case ParseErrorCode_InternalCompilerError: { Result = CSz("ParseErrorCode_InternalCompilerError"); } break;
    case ParseErrorCode_StackOverflow: { Result = CSz("ParseErrorCode_StackOverflow"); } break;
    case ParseErrorCode_Unknown: { Result = CSz("ParseErrorCode_Unknown"); } break;


    
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (enum_t.name)", Type); } */
  return Result;
}

