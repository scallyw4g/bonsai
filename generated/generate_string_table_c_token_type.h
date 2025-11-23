// callsite
// external/bonsai_stdlib/src/c_token.h:225:0

// def (generate_string_table)
// external/bonsai_stdlib/src/poof_functions.h:1821:0
link_internal b32
IsValid(c_token_type Value)
{
  b32 Result = False;
  switch (Value)
  {
        case CTokenType_Unknown:
    case CT_ControlChar_Start_of_Heading:
    case CT_ControlChar_Start_of_Text:
    case CT_ControlChar_End_of_Text:
    case CT_ControlChar_End_of_Transmission:
    case CT_ControlChar_Enquiry:
    case CT_ControlChar_Acknowledgement:
    case CT_ControlChar_Bell:
    case CT_ControlChar_Backspace:
    case CT_ControlChar_Vertical_Tab:
    case CT_ControlChar_Form_Feed:
    case CT_ControlChar_Shift_Out:
    case CT_ControlChar_Shift_In:
    case CT_ControlChar_Data_Link_Escape:
    case CT_ControlChar_Device_Control_1:
    case CT_ControlChar_Device_Control_2:
    case CT_ControlChar_Device_Control_3:
    case CT_ControlChar_Device_Control_4:
    case CT_ControlChar_Negative_Acknowledgement:
    case CT_ControlChar_Synchronous_Idle:
    case CT_ControlChar_End_of_Transmission_Block:
    case CT_ControlChar_Cancel:
    case CT_ControlChar_End_of_Medium:
    case CT_ControlChar_Substitute:
    case CT_ControlChar_Escape:
    case CT_ControlChar_File_Separator:
    case CT_ControlChar_Group_Separator:
    case CT_ControlChar_Record_Separator:
    case CT_ControlChar_Unit_Separator:
    case CT_ControlChar_Delete:
    case CTokenType_OpenBracket:
    case CTokenType_CloseBracket:
    case CTokenType_OpenBrace:
    case CTokenType_CloseBrace:
    case CTokenType_OpenParen:
    case CTokenType_CloseParen:
    case CTokenType_Dot:
    case CTokenType_Comma:
    case CTokenType_Semicolon:
    case CTokenType_Colon:
    case CTokenType_Hash:
    case CTokenType_At:
    case CTokenType_Dollar:
    case CTokenType_Space:
    case CTokenType_Tab:
    case CTokenType_Star:
    case CTokenType_Ampersand:
    case CTokenType_SingleQuote:
    case CTokenType_DoubleQuote:
    case CTokenType_Equals:
    case CTokenType_LT:
    case CTokenType_GT:
    case CTokenType_Plus:
    case CTokenType_Minus:
    case CTokenType_Percent:
    case CTokenType_Bang:
    case CTokenType_Hat:
    case CTokenType_Question:
    case CTokenType_FSlash:
    case CTokenType_BSlash:
    case CTokenType_Tilde:
    case CTokenType_Backtick:
    case CTokenType_Pipe:
    case CTokenType_Newline:
    case CTokenType_CarrigeReturn:
    case CTokenType_EOF:
    case CTokenType_CommentSingleLine:
    case CTokenType_CommentMultiLine:
    case CTokenType_EscapedNewline:
    case CTokenType_Identifier:
    case CTokenType_StringLiteral:
    case CTokenType_CharLiteral:
    case CTokenType_IntLiteral:
    case CTokenType_DoubleLiteral:
    case CTokenType_FloatLiteral:
    case CTokenType_Poof:
    case CT_Keyword_Noreturn:
    case CT_Keyword_Override:
    case CT_Keyword_Virtual:
    case CT_Keyword_Noexcept:
    case CT_Keyword_Explicit:
    case CT_Keyword_Constexpr:
    case CT_Keyword_Namespace:
    case CT_Keyword_Class:
    case CTokenType_Struct:
    case CTokenType_Enum:
    case CTokenType_Union:
    case CTokenType_Typedef:
    case CTokenType_Using:
    case CTokenType_ThreadLocal:
    case CTokenType_Const:
    case CTokenType_Static:
    case CTokenType_Volatile:
    case CTokenType_Long:
    case CTokenType_Unsigned:
    case CTokenType_Signed:
    case CTokenType_Bool:
    case CTokenType_Auto:
    case CTokenType_Void:
    case CTokenType_Double:
    case CTokenType_Float:
    case CTokenType_Char:
    case CTokenType_Int:
    case CTokenType_Short:
    case CTokenType_Inline:
    case CTokenType_TemplateKeyword:
    case CTokenType_OperatorKeyword:
    case CT_Keyword_Public:
    case CT_Keyword_Private:
    case CT_Keyword_Protected:
    case CT_KeywordPragma:
    case CT_KeywordAttribute:
    case CT_StaticAssert:
    case CTokenType_Extern:
    case CTokenType_Asm:
    case CTokenType_Goto:
    case CTokenType_Ellipsis:
    case CTokenType_If:
    case CTokenType_Else:
    case CTokenType_Break:
    case CTokenType_Switch:
    case CTokenType_Case:
    case CT_Keyword_Delete:
    case CTokenType_Default:
    case CTokenType_For:
    case CTokenType_While:
    case CTokenType_Continue:
    case CTokenType_Return:
    case CTokenType_LeftShift:
    case CTokenType_RightShift:
    case CTokenType_LessEqual:
    case CTokenType_GreaterEqual:
    case CTokenType_AreEqual:
    case CTokenType_NotEqual:
    case CTokenType_PlusEquals:
    case CTokenType_MinusEquals:
    case CTokenType_TimesEquals:
    case CTokenType_DivEquals:
    case CTokenType_ModEquals:
    case CTokenType_AndEquals:
    case CTokenType_OrEquals:
    case CTokenType_XorEquals:
    case CTokenType_Increment:
    case CTokenType_Decrement:
    case CTokenType_LogicalAnd:
    case CTokenType_LogicalOr:
    case CTokenType_Arrow:
    case CT_ScopeResolutionOperator:
    case CT_NameQualifier:
    case CT_MacroLiteral:
    case CT_MacroLiteral_SelfRefExpansion:
    case CT_PreprocessorHasInclude:
    case CT_PreprocessorHasIncludeNext:
    case CT_PreprocessorPaste:
    case CT_PreprocessorPaste_InvalidToken:
    case CT_PreprocessorInclude:
    case CT_PreprocessorIncludeNext:
    case CT_PreprocessorIf:
    case CT_PreprocessorElse:
    case CT_PreprocessorElif:
    case CT_PreprocessorEndif:
    case CT_PreprocessorIfDefined:
    case CT_PreprocessorIfNotDefined:
    case CT_PreprocessorDefine:
    case CT_PreprocessorUndef:
    case CT_PreprocessorPragma:
    case CT_PreprocessorError:
    case CT_PreprocessorWarning:
    case CT_Preprocessor_VA_ARGS_:
    case CT_Preprocessor_Nuked:
    case CT_InsertedCode:
    case CT_PoofInsertedCode:
    case CT_PoofModifiedToken:

    {
      Result = True;
    }
  }
  return Result;
}



link_internal counted_string
ToStringPrefixless(c_token_type Type)
{
  cs Result = {};
  if (IsValid(Type))
  {
    switch (Type)
    {
            case CTokenType_Unknown: { Result = CSz("Unknown"); } break;
      case CT_ControlChar_Start_of_Heading: { Result = CSz("Heading"); } break;
      case CT_ControlChar_Start_of_Text: { Result = CSz("Text"); } break;
      case CT_ControlChar_End_of_Text: { Result = CSz("Text"); } break;
      case CT_ControlChar_End_of_Transmission: { Result = CSz("Transmission"); } break;
      case CT_ControlChar_Enquiry: { Result = CSz("Enquiry"); } break;
      case CT_ControlChar_Acknowledgement: { Result = CSz("Acknowledgement"); } break;
      case CT_ControlChar_Bell: { Result = CSz("Bell"); } break;
      case CT_ControlChar_Backspace: { Result = CSz("Backspace"); } break;
      case CT_ControlChar_Vertical_Tab: { Result = CSz("Tab"); } break;
      case CT_ControlChar_Form_Feed: { Result = CSz("Feed"); } break;
      case CT_ControlChar_Shift_Out: { Result = CSz("Out"); } break;
      case CT_ControlChar_Shift_In: { Result = CSz("In"); } break;
      case CT_ControlChar_Data_Link_Escape: { Result = CSz("Escape"); } break;
      case CT_ControlChar_Device_Control_1: { Result = CSz("1"); } break;
      case CT_ControlChar_Device_Control_2: { Result = CSz("2"); } break;
      case CT_ControlChar_Device_Control_3: { Result = CSz("3"); } break;
      case CT_ControlChar_Device_Control_4: { Result = CSz("4"); } break;
      case CT_ControlChar_Negative_Acknowledgement: { Result = CSz("Acknowledgement"); } break;
      case CT_ControlChar_Synchronous_Idle: { Result = CSz("Idle"); } break;
      case CT_ControlChar_End_of_Transmission_Block: { Result = CSz("Block"); } break;
      case CT_ControlChar_Cancel: { Result = CSz("Cancel"); } break;
      case CT_ControlChar_End_of_Medium: { Result = CSz("Medium"); } break;
      case CT_ControlChar_Substitute: { Result = CSz("Substitute"); } break;
      case CT_ControlChar_Escape: { Result = CSz("Escape"); } break;
      case CT_ControlChar_File_Separator: { Result = CSz("Separator"); } break;
      case CT_ControlChar_Group_Separator: { Result = CSz("Separator"); } break;
      case CT_ControlChar_Record_Separator: { Result = CSz("Separator"); } break;
      case CT_ControlChar_Unit_Separator: { Result = CSz("Separator"); } break;
      case CT_ControlChar_Delete: { Result = CSz("Delete"); } break;
      case CTokenType_OpenBracket: { Result = CSz("OpenBracket"); } break;
      case CTokenType_CloseBracket: { Result = CSz("CloseBracket"); } break;
      case CTokenType_OpenBrace: { Result = CSz("OpenBrace"); } break;
      case CTokenType_CloseBrace: { Result = CSz("CloseBrace"); } break;
      case CTokenType_OpenParen: { Result = CSz("OpenParen"); } break;
      case CTokenType_CloseParen: { Result = CSz("CloseParen"); } break;
      case CTokenType_Dot: { Result = CSz("Dot"); } break;
      case CTokenType_Comma: { Result = CSz("Comma"); } break;
      case CTokenType_Semicolon: { Result = CSz("Semicolon"); } break;
      case CTokenType_Colon: { Result = CSz("Colon"); } break;
      case CTokenType_Hash: { Result = CSz("Hash"); } break;
      case CTokenType_At: { Result = CSz("At"); } break;
      case CTokenType_Dollar: { Result = CSz("Dollar"); } break;
      case CTokenType_Space: { Result = CSz("Space"); } break;
      case CTokenType_Tab: { Result = CSz("Tab"); } break;
      case CTokenType_Star: { Result = CSz("Star"); } break;
      case CTokenType_Ampersand: { Result = CSz("Ampersand"); } break;
      case CTokenType_SingleQuote: { Result = CSz("SingleQuote"); } break;
      case CTokenType_DoubleQuote: { Result = CSz("DoubleQuote"); } break;
      case CTokenType_Equals: { Result = CSz("Equals"); } break;
      case CTokenType_LT: { Result = CSz("LT"); } break;
      case CTokenType_GT: { Result = CSz("GT"); } break;
      case CTokenType_Plus: { Result = CSz("Plus"); } break;
      case CTokenType_Minus: { Result = CSz("Minus"); } break;
      case CTokenType_Percent: { Result = CSz("Percent"); } break;
      case CTokenType_Bang: { Result = CSz("Bang"); } break;
      case CTokenType_Hat: { Result = CSz("Hat"); } break;
      case CTokenType_Question: { Result = CSz("Question"); } break;
      case CTokenType_FSlash: { Result = CSz("FSlash"); } break;
      case CTokenType_BSlash: { Result = CSz("BSlash"); } break;
      case CTokenType_Tilde: { Result = CSz("Tilde"); } break;
      case CTokenType_Backtick: { Result = CSz("Backtick"); } break;
      case CTokenType_Pipe: { Result = CSz("Pipe"); } break;
      case CTokenType_Newline: { Result = CSz("Newline"); } break;
      case CTokenType_CarrigeReturn: { Result = CSz("CarrigeReturn"); } break;
      case CTokenType_EOF: { Result = CSz("EOF"); } break;
      case CTokenType_CommentSingleLine: { Result = CSz("CommentSingleLine"); } break;
      case CTokenType_CommentMultiLine: { Result = CSz("CommentMultiLine"); } break;
      case CTokenType_EscapedNewline: { Result = CSz("EscapedNewline"); } break;
      case CTokenType_Identifier: { Result = CSz("Identifier"); } break;
      case CTokenType_StringLiteral: { Result = CSz("StringLiteral"); } break;
      case CTokenType_CharLiteral: { Result = CSz("CharLiteral"); } break;
      case CTokenType_IntLiteral: { Result = CSz("IntLiteral"); } break;
      case CTokenType_DoubleLiteral: { Result = CSz("DoubleLiteral"); } break;
      case CTokenType_FloatLiteral: { Result = CSz("FloatLiteral"); } break;
      case CTokenType_Poof: { Result = CSz("Poof"); } break;
      case CT_Keyword_Noreturn: { Result = CSz("Noreturn"); } break;
      case CT_Keyword_Override: { Result = CSz("Override"); } break;
      case CT_Keyword_Virtual: { Result = CSz("Virtual"); } break;
      case CT_Keyword_Noexcept: { Result = CSz("Noexcept"); } break;
      case CT_Keyword_Explicit: { Result = CSz("Explicit"); } break;
      case CT_Keyword_Constexpr: { Result = CSz("Constexpr"); } break;
      case CT_Keyword_Namespace: { Result = CSz("Namespace"); } break;
      case CT_Keyword_Class: { Result = CSz("Class"); } break;
      case CTokenType_Struct: { Result = CSz("Struct"); } break;
      case CTokenType_Enum: { Result = CSz("Enum"); } break;
      case CTokenType_Union: { Result = CSz("Union"); } break;
      case CTokenType_Typedef: { Result = CSz("Typedef"); } break;
      case CTokenType_Using: { Result = CSz("Using"); } break;
      case CTokenType_ThreadLocal: { Result = CSz("ThreadLocal"); } break;
      case CTokenType_Const: { Result = CSz("Const"); } break;
      case CTokenType_Static: { Result = CSz("Static"); } break;
      case CTokenType_Volatile: { Result = CSz("Volatile"); } break;
      case CTokenType_Long: { Result = CSz("Long"); } break;
      case CTokenType_Unsigned: { Result = CSz("Unsigned"); } break;
      case CTokenType_Signed: { Result = CSz("Signed"); } break;
      case CTokenType_Bool: { Result = CSz("Bool"); } break;
      case CTokenType_Auto: { Result = CSz("Auto"); } break;
      case CTokenType_Void: { Result = CSz("Void"); } break;
      case CTokenType_Double: { Result = CSz("Double"); } break;
      case CTokenType_Float: { Result = CSz("Float"); } break;
      case CTokenType_Char: { Result = CSz("Char"); } break;
      case CTokenType_Int: { Result = CSz("Int"); } break;
      case CTokenType_Short: { Result = CSz("Short"); } break;
      case CTokenType_Inline: { Result = CSz("Inline"); } break;
      case CTokenType_TemplateKeyword: { Result = CSz("TemplateKeyword"); } break;
      case CTokenType_OperatorKeyword: { Result = CSz("OperatorKeyword"); } break;
      case CT_Keyword_Public: { Result = CSz("Public"); } break;
      case CT_Keyword_Private: { Result = CSz("Private"); } break;
      case CT_Keyword_Protected: { Result = CSz("Protected"); } break;
      case CT_KeywordPragma: { Result = CSz("KeywordPragma"); } break;
      case CT_KeywordAttribute: { Result = CSz("KeywordAttribute"); } break;
      case CT_StaticAssert: { Result = CSz("StaticAssert"); } break;
      case CTokenType_Extern: { Result = CSz("Extern"); } break;
      case CTokenType_Asm: { Result = CSz("Asm"); } break;
      case CTokenType_Goto: { Result = CSz("Goto"); } break;
      case CTokenType_Ellipsis: { Result = CSz("Ellipsis"); } break;
      case CTokenType_If: { Result = CSz("If"); } break;
      case CTokenType_Else: { Result = CSz("Else"); } break;
      case CTokenType_Break: { Result = CSz("Break"); } break;
      case CTokenType_Switch: { Result = CSz("Switch"); } break;
      case CTokenType_Case: { Result = CSz("Case"); } break;
      case CT_Keyword_Delete: { Result = CSz("Delete"); } break;
      case CTokenType_Default: { Result = CSz("Default"); } break;
      case CTokenType_For: { Result = CSz("For"); } break;
      case CTokenType_While: { Result = CSz("While"); } break;
      case CTokenType_Continue: { Result = CSz("Continue"); } break;
      case CTokenType_Return: { Result = CSz("Return"); } break;
      case CTokenType_LeftShift: { Result = CSz("LeftShift"); } break;
      case CTokenType_RightShift: { Result = CSz("RightShift"); } break;
      case CTokenType_LessEqual: { Result = CSz("LessEqual"); } break;
      case CTokenType_GreaterEqual: { Result = CSz("GreaterEqual"); } break;
      case CTokenType_AreEqual: { Result = CSz("AreEqual"); } break;
      case CTokenType_NotEqual: { Result = CSz("NotEqual"); } break;
      case CTokenType_PlusEquals: { Result = CSz("PlusEquals"); } break;
      case CTokenType_MinusEquals: { Result = CSz("MinusEquals"); } break;
      case CTokenType_TimesEquals: { Result = CSz("TimesEquals"); } break;
      case CTokenType_DivEquals: { Result = CSz("DivEquals"); } break;
      case CTokenType_ModEquals: { Result = CSz("ModEquals"); } break;
      case CTokenType_AndEquals: { Result = CSz("AndEquals"); } break;
      case CTokenType_OrEquals: { Result = CSz("OrEquals"); } break;
      case CTokenType_XorEquals: { Result = CSz("XorEquals"); } break;
      case CTokenType_Increment: { Result = CSz("Increment"); } break;
      case CTokenType_Decrement: { Result = CSz("Decrement"); } break;
      case CTokenType_LogicalAnd: { Result = CSz("LogicalAnd"); } break;
      case CTokenType_LogicalOr: { Result = CSz("LogicalOr"); } break;
      case CTokenType_Arrow: { Result = CSz("Arrow"); } break;
      case CT_ScopeResolutionOperator: { Result = CSz("ScopeResolutionOperator"); } break;
      case CT_NameQualifier: { Result = CSz("NameQualifier"); } break;
      case CT_MacroLiteral: { Result = CSz("MacroLiteral"); } break;
      case CT_MacroLiteral_SelfRefExpansion: { Result = CSz("SelfRefExpansion"); } break;
      case CT_PreprocessorHasInclude: { Result = CSz("PreprocessorHasInclude"); } break;
      case CT_PreprocessorHasIncludeNext: { Result = CSz("PreprocessorHasIncludeNext"); } break;
      case CT_PreprocessorPaste: { Result = CSz("PreprocessorPaste"); } break;
      case CT_PreprocessorPaste_InvalidToken: { Result = CSz("InvalidToken"); } break;
      case CT_PreprocessorInclude: { Result = CSz("PreprocessorInclude"); } break;
      case CT_PreprocessorIncludeNext: { Result = CSz("PreprocessorIncludeNext"); } break;
      case CT_PreprocessorIf: { Result = CSz("PreprocessorIf"); } break;
      case CT_PreprocessorElse: { Result = CSz("PreprocessorElse"); } break;
      case CT_PreprocessorElif: { Result = CSz("PreprocessorElif"); } break;
      case CT_PreprocessorEndif: { Result = CSz("PreprocessorEndif"); } break;
      case CT_PreprocessorIfDefined: { Result = CSz("PreprocessorIfDefined"); } break;
      case CT_PreprocessorIfNotDefined: { Result = CSz("PreprocessorIfNotDefined"); } break;
      case CT_PreprocessorDefine: { Result = CSz("PreprocessorDefine"); } break;
      case CT_PreprocessorUndef: { Result = CSz("PreprocessorUndef"); } break;
      case CT_PreprocessorPragma: { Result = CSz("PreprocessorPragma"); } break;
      case CT_PreprocessorError: { Result = CSz("PreprocessorError"); } break;
      case CT_PreprocessorWarning: { Result = CSz("PreprocessorWarning"); } break;
      case CT_Preprocessor_VA_ARGS_: { Result = CSz("ARGS_"); } break;
      case CT_Preprocessor_Nuked: { Result = CSz("Nuked"); } break;
      case CT_InsertedCode: { Result = CSz("InsertedCode"); } break;
      case CT_PoofInsertedCode: { Result = CSz("PoofInsertedCode"); } break;
      case CT_PoofModifiedToken: { Result = CSz("PoofModifiedToken"); } break;


      
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
ToString(c_token_type Type)
{
  Assert(IsValid(Type));

  counted_string Result = {};
  switch (Type)
  {
        case CTokenType_Unknown: { Result = CSz("CTokenType_Unknown"); } break;
    case CT_ControlChar_Start_of_Heading: { Result = CSz("CT_ControlChar_Start_of_Heading"); } break;
    case CT_ControlChar_Start_of_Text: { Result = CSz("CT_ControlChar_Start_of_Text"); } break;
    case CT_ControlChar_End_of_Text: { Result = CSz("CT_ControlChar_End_of_Text"); } break;
    case CT_ControlChar_End_of_Transmission: { Result = CSz("CT_ControlChar_End_of_Transmission"); } break;
    case CT_ControlChar_Enquiry: { Result = CSz("CT_ControlChar_Enquiry"); } break;
    case CT_ControlChar_Acknowledgement: { Result = CSz("CT_ControlChar_Acknowledgement"); } break;
    case CT_ControlChar_Bell: { Result = CSz("CT_ControlChar_Bell"); } break;
    case CT_ControlChar_Backspace: { Result = CSz("CT_ControlChar_Backspace"); } break;
    case CT_ControlChar_Vertical_Tab: { Result = CSz("CT_ControlChar_Vertical_Tab"); } break;
    case CT_ControlChar_Form_Feed: { Result = CSz("CT_ControlChar_Form_Feed"); } break;
    case CT_ControlChar_Shift_Out: { Result = CSz("CT_ControlChar_Shift_Out"); } break;
    case CT_ControlChar_Shift_In: { Result = CSz("CT_ControlChar_Shift_In"); } break;
    case CT_ControlChar_Data_Link_Escape: { Result = CSz("CT_ControlChar_Data_Link_Escape"); } break;
    case CT_ControlChar_Device_Control_1: { Result = CSz("CT_ControlChar_Device_Control_1"); } break;
    case CT_ControlChar_Device_Control_2: { Result = CSz("CT_ControlChar_Device_Control_2"); } break;
    case CT_ControlChar_Device_Control_3: { Result = CSz("CT_ControlChar_Device_Control_3"); } break;
    case CT_ControlChar_Device_Control_4: { Result = CSz("CT_ControlChar_Device_Control_4"); } break;
    case CT_ControlChar_Negative_Acknowledgement: { Result = CSz("CT_ControlChar_Negative_Acknowledgement"); } break;
    case CT_ControlChar_Synchronous_Idle: { Result = CSz("CT_ControlChar_Synchronous_Idle"); } break;
    case CT_ControlChar_End_of_Transmission_Block: { Result = CSz("CT_ControlChar_End_of_Transmission_Block"); } break;
    case CT_ControlChar_Cancel: { Result = CSz("CT_ControlChar_Cancel"); } break;
    case CT_ControlChar_End_of_Medium: { Result = CSz("CT_ControlChar_End_of_Medium"); } break;
    case CT_ControlChar_Substitute: { Result = CSz("CT_ControlChar_Substitute"); } break;
    case CT_ControlChar_Escape: { Result = CSz("CT_ControlChar_Escape"); } break;
    case CT_ControlChar_File_Separator: { Result = CSz("CT_ControlChar_File_Separator"); } break;
    case CT_ControlChar_Group_Separator: { Result = CSz("CT_ControlChar_Group_Separator"); } break;
    case CT_ControlChar_Record_Separator: { Result = CSz("CT_ControlChar_Record_Separator"); } break;
    case CT_ControlChar_Unit_Separator: { Result = CSz("CT_ControlChar_Unit_Separator"); } break;
    case CT_ControlChar_Delete: { Result = CSz("CT_ControlChar_Delete"); } break;
    case CTokenType_OpenBracket: { Result = CSz("CTokenType_OpenBracket"); } break;
    case CTokenType_CloseBracket: { Result = CSz("CTokenType_CloseBracket"); } break;
    case CTokenType_OpenBrace: { Result = CSz("CTokenType_OpenBrace"); } break;
    case CTokenType_CloseBrace: { Result = CSz("CTokenType_CloseBrace"); } break;
    case CTokenType_OpenParen: { Result = CSz("CTokenType_OpenParen"); } break;
    case CTokenType_CloseParen: { Result = CSz("CTokenType_CloseParen"); } break;
    case CTokenType_Dot: { Result = CSz("CTokenType_Dot"); } break;
    case CTokenType_Comma: { Result = CSz("CTokenType_Comma"); } break;
    case CTokenType_Semicolon: { Result = CSz("CTokenType_Semicolon"); } break;
    case CTokenType_Colon: { Result = CSz("CTokenType_Colon"); } break;
    case CTokenType_Hash: { Result = CSz("CTokenType_Hash"); } break;
    case CTokenType_At: { Result = CSz("CTokenType_At"); } break;
    case CTokenType_Dollar: { Result = CSz("CTokenType_Dollar"); } break;
    case CTokenType_Space: { Result = CSz("CTokenType_Space"); } break;
    case CTokenType_Tab: { Result = CSz("CTokenType_Tab"); } break;
    case CTokenType_Star: { Result = CSz("CTokenType_Star"); } break;
    case CTokenType_Ampersand: { Result = CSz("CTokenType_Ampersand"); } break;
    case CTokenType_SingleQuote: { Result = CSz("CTokenType_SingleQuote"); } break;
    case CTokenType_DoubleQuote: { Result = CSz("CTokenType_DoubleQuote"); } break;
    case CTokenType_Equals: { Result = CSz("CTokenType_Equals"); } break;
    case CTokenType_LT: { Result = CSz("CTokenType_LT"); } break;
    case CTokenType_GT: { Result = CSz("CTokenType_GT"); } break;
    case CTokenType_Plus: { Result = CSz("CTokenType_Plus"); } break;
    case CTokenType_Minus: { Result = CSz("CTokenType_Minus"); } break;
    case CTokenType_Percent: { Result = CSz("CTokenType_Percent"); } break;
    case CTokenType_Bang: { Result = CSz("CTokenType_Bang"); } break;
    case CTokenType_Hat: { Result = CSz("CTokenType_Hat"); } break;
    case CTokenType_Question: { Result = CSz("CTokenType_Question"); } break;
    case CTokenType_FSlash: { Result = CSz("CTokenType_FSlash"); } break;
    case CTokenType_BSlash: { Result = CSz("CTokenType_BSlash"); } break;
    case CTokenType_Tilde: { Result = CSz("CTokenType_Tilde"); } break;
    case CTokenType_Backtick: { Result = CSz("CTokenType_Backtick"); } break;
    case CTokenType_Pipe: { Result = CSz("CTokenType_Pipe"); } break;
    case CTokenType_Newline: { Result = CSz("CTokenType_Newline"); } break;
    case CTokenType_CarrigeReturn: { Result = CSz("CTokenType_CarrigeReturn"); } break;
    case CTokenType_EOF: { Result = CSz("CTokenType_EOF"); } break;
    case CTokenType_CommentSingleLine: { Result = CSz("CTokenType_CommentSingleLine"); } break;
    case CTokenType_CommentMultiLine: { Result = CSz("CTokenType_CommentMultiLine"); } break;
    case CTokenType_EscapedNewline: { Result = CSz("CTokenType_EscapedNewline"); } break;
    case CTokenType_Identifier: { Result = CSz("CTokenType_Identifier"); } break;
    case CTokenType_StringLiteral: { Result = CSz("CTokenType_StringLiteral"); } break;
    case CTokenType_CharLiteral: { Result = CSz("CTokenType_CharLiteral"); } break;
    case CTokenType_IntLiteral: { Result = CSz("CTokenType_IntLiteral"); } break;
    case CTokenType_DoubleLiteral: { Result = CSz("CTokenType_DoubleLiteral"); } break;
    case CTokenType_FloatLiteral: { Result = CSz("CTokenType_FloatLiteral"); } break;
    case CTokenType_Poof: { Result = CSz("CTokenType_Poof"); } break;
    case CT_Keyword_Noreturn: { Result = CSz("CT_Keyword_Noreturn"); } break;
    case CT_Keyword_Override: { Result = CSz("CT_Keyword_Override"); } break;
    case CT_Keyword_Virtual: { Result = CSz("CT_Keyword_Virtual"); } break;
    case CT_Keyword_Noexcept: { Result = CSz("CT_Keyword_Noexcept"); } break;
    case CT_Keyword_Explicit: { Result = CSz("CT_Keyword_Explicit"); } break;
    case CT_Keyword_Constexpr: { Result = CSz("CT_Keyword_Constexpr"); } break;
    case CT_Keyword_Namespace: { Result = CSz("CT_Keyword_Namespace"); } break;
    case CT_Keyword_Class: { Result = CSz("CT_Keyword_Class"); } break;
    case CTokenType_Struct: { Result = CSz("CTokenType_Struct"); } break;
    case CTokenType_Enum: { Result = CSz("CTokenType_Enum"); } break;
    case CTokenType_Union: { Result = CSz("CTokenType_Union"); } break;
    case CTokenType_Typedef: { Result = CSz("CTokenType_Typedef"); } break;
    case CTokenType_Using: { Result = CSz("CTokenType_Using"); } break;
    case CTokenType_ThreadLocal: { Result = CSz("CTokenType_ThreadLocal"); } break;
    case CTokenType_Const: { Result = CSz("CTokenType_Const"); } break;
    case CTokenType_Static: { Result = CSz("CTokenType_Static"); } break;
    case CTokenType_Volatile: { Result = CSz("CTokenType_Volatile"); } break;
    case CTokenType_Long: { Result = CSz("CTokenType_Long"); } break;
    case CTokenType_Unsigned: { Result = CSz("CTokenType_Unsigned"); } break;
    case CTokenType_Signed: { Result = CSz("CTokenType_Signed"); } break;
    case CTokenType_Bool: { Result = CSz("CTokenType_Bool"); } break;
    case CTokenType_Auto: { Result = CSz("CTokenType_Auto"); } break;
    case CTokenType_Void: { Result = CSz("CTokenType_Void"); } break;
    case CTokenType_Double: { Result = CSz("CTokenType_Double"); } break;
    case CTokenType_Float: { Result = CSz("CTokenType_Float"); } break;
    case CTokenType_Char: { Result = CSz("CTokenType_Char"); } break;
    case CTokenType_Int: { Result = CSz("CTokenType_Int"); } break;
    case CTokenType_Short: { Result = CSz("CTokenType_Short"); } break;
    case CTokenType_Inline: { Result = CSz("CTokenType_Inline"); } break;
    case CTokenType_TemplateKeyword: { Result = CSz("CTokenType_TemplateKeyword"); } break;
    case CTokenType_OperatorKeyword: { Result = CSz("CTokenType_OperatorKeyword"); } break;
    case CT_Keyword_Public: { Result = CSz("CT_Keyword_Public"); } break;
    case CT_Keyword_Private: { Result = CSz("CT_Keyword_Private"); } break;
    case CT_Keyword_Protected: { Result = CSz("CT_Keyword_Protected"); } break;
    case CT_KeywordPragma: { Result = CSz("CT_KeywordPragma"); } break;
    case CT_KeywordAttribute: { Result = CSz("CT_KeywordAttribute"); } break;
    case CT_StaticAssert: { Result = CSz("CT_StaticAssert"); } break;
    case CTokenType_Extern: { Result = CSz("CTokenType_Extern"); } break;
    case CTokenType_Asm: { Result = CSz("CTokenType_Asm"); } break;
    case CTokenType_Goto: { Result = CSz("CTokenType_Goto"); } break;
    case CTokenType_Ellipsis: { Result = CSz("CTokenType_Ellipsis"); } break;
    case CTokenType_If: { Result = CSz("CTokenType_If"); } break;
    case CTokenType_Else: { Result = CSz("CTokenType_Else"); } break;
    case CTokenType_Break: { Result = CSz("CTokenType_Break"); } break;
    case CTokenType_Switch: { Result = CSz("CTokenType_Switch"); } break;
    case CTokenType_Case: { Result = CSz("CTokenType_Case"); } break;
    case CT_Keyword_Delete: { Result = CSz("CT_Keyword_Delete"); } break;
    case CTokenType_Default: { Result = CSz("CTokenType_Default"); } break;
    case CTokenType_For: { Result = CSz("CTokenType_For"); } break;
    case CTokenType_While: { Result = CSz("CTokenType_While"); } break;
    case CTokenType_Continue: { Result = CSz("CTokenType_Continue"); } break;
    case CTokenType_Return: { Result = CSz("CTokenType_Return"); } break;
    case CTokenType_LeftShift: { Result = CSz("CTokenType_LeftShift"); } break;
    case CTokenType_RightShift: { Result = CSz("CTokenType_RightShift"); } break;
    case CTokenType_LessEqual: { Result = CSz("CTokenType_LessEqual"); } break;
    case CTokenType_GreaterEqual: { Result = CSz("CTokenType_GreaterEqual"); } break;
    case CTokenType_AreEqual: { Result = CSz("CTokenType_AreEqual"); } break;
    case CTokenType_NotEqual: { Result = CSz("CTokenType_NotEqual"); } break;
    case CTokenType_PlusEquals: { Result = CSz("CTokenType_PlusEquals"); } break;
    case CTokenType_MinusEquals: { Result = CSz("CTokenType_MinusEquals"); } break;
    case CTokenType_TimesEquals: { Result = CSz("CTokenType_TimesEquals"); } break;
    case CTokenType_DivEquals: { Result = CSz("CTokenType_DivEquals"); } break;
    case CTokenType_ModEquals: { Result = CSz("CTokenType_ModEquals"); } break;
    case CTokenType_AndEquals: { Result = CSz("CTokenType_AndEquals"); } break;
    case CTokenType_OrEquals: { Result = CSz("CTokenType_OrEquals"); } break;
    case CTokenType_XorEquals: { Result = CSz("CTokenType_XorEquals"); } break;
    case CTokenType_Increment: { Result = CSz("CTokenType_Increment"); } break;
    case CTokenType_Decrement: { Result = CSz("CTokenType_Decrement"); } break;
    case CTokenType_LogicalAnd: { Result = CSz("CTokenType_LogicalAnd"); } break;
    case CTokenType_LogicalOr: { Result = CSz("CTokenType_LogicalOr"); } break;
    case CTokenType_Arrow: { Result = CSz("CTokenType_Arrow"); } break;
    case CT_ScopeResolutionOperator: { Result = CSz("CT_ScopeResolutionOperator"); } break;
    case CT_NameQualifier: { Result = CSz("CT_NameQualifier"); } break;
    case CT_MacroLiteral: { Result = CSz("CT_MacroLiteral"); } break;
    case CT_MacroLiteral_SelfRefExpansion: { Result = CSz("CT_MacroLiteral_SelfRefExpansion"); } break;
    case CT_PreprocessorHasInclude: { Result = CSz("CT_PreprocessorHasInclude"); } break;
    case CT_PreprocessorHasIncludeNext: { Result = CSz("CT_PreprocessorHasIncludeNext"); } break;
    case CT_PreprocessorPaste: { Result = CSz("CT_PreprocessorPaste"); } break;
    case CT_PreprocessorPaste_InvalidToken: { Result = CSz("CT_PreprocessorPaste_InvalidToken"); } break;
    case CT_PreprocessorInclude: { Result = CSz("CT_PreprocessorInclude"); } break;
    case CT_PreprocessorIncludeNext: { Result = CSz("CT_PreprocessorIncludeNext"); } break;
    case CT_PreprocessorIf: { Result = CSz("CT_PreprocessorIf"); } break;
    case CT_PreprocessorElse: { Result = CSz("CT_PreprocessorElse"); } break;
    case CT_PreprocessorElif: { Result = CSz("CT_PreprocessorElif"); } break;
    case CT_PreprocessorEndif: { Result = CSz("CT_PreprocessorEndif"); } break;
    case CT_PreprocessorIfDefined: { Result = CSz("CT_PreprocessorIfDefined"); } break;
    case CT_PreprocessorIfNotDefined: { Result = CSz("CT_PreprocessorIfNotDefined"); } break;
    case CT_PreprocessorDefine: { Result = CSz("CT_PreprocessorDefine"); } break;
    case CT_PreprocessorUndef: { Result = CSz("CT_PreprocessorUndef"); } break;
    case CT_PreprocessorPragma: { Result = CSz("CT_PreprocessorPragma"); } break;
    case CT_PreprocessorError: { Result = CSz("CT_PreprocessorError"); } break;
    case CT_PreprocessorWarning: { Result = CSz("CT_PreprocessorWarning"); } break;
    case CT_Preprocessor_VA_ARGS_: { Result = CSz("CT_Preprocessor_VA_ARGS_"); } break;
    case CT_Preprocessor_Nuked: { Result = CSz("CT_Preprocessor_Nuked"); } break;
    case CT_InsertedCode: { Result = CSz("CT_InsertedCode"); } break;
    case CT_PoofInsertedCode: { Result = CSz("CT_PoofInsertedCode"); } break;
    case CT_PoofModifiedToken: { Result = CSz("CT_PoofModifiedToken"); } break;


    
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (enum_t.name)", Type); } */
  return Result;
}

