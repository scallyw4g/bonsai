#define PLATFORM_LIBRARY_AND_WINDOW_IMPLEMENTATIONS 1
#define PLATFORM_GL_IMPLEMENTATIONS 1

#include <bonsai_types.h>

#define InvalidDefaultWhileParsing(Parser, ErrorMessage) \
    default: { OutputParsingError(Parser, Parser->Tokens.At, ErrorMessage); Assert(False); } break;

function b32
IsWhitespace(c_token_type Type)
{
  b32 Result = Type == CTokenType_Newline       ||
               Type == CTokenType_CarrigeReturn ||
               Type == CTokenType_Space;
  return Result;
}

function void
Advance(c_token_cursor* Tokens, u32 Lookahead = 0)
{
  if (Remaining(Tokens, Lookahead))
  {
    Tokens->At += Lookahead+1;
  }
  else
  {
    Warn("Attempted to advance token stream past its end.");
  }

  return;
}

function void
Advance(c_parse_result* Parser, u32 Lookahead = 0)
{
  if (Remaining(&Parser->Tokens, Lookahead))
  {
    for (u32 LookaheadIndex = 0;
        LookaheadIndex < Lookahead+1;
        ++LookaheadIndex)
    {
      if (Parser->Tokens.At[LookaheadIndex].Type == CTokenType_Newline)
      {
        ++Parser->LineNumber;
      }
    }

    Parser->Tokens.At += Lookahead+1;
  }
  else
  {
    Warn("Attempted to advance parser past end of stream on file : %.*s", (u32)Parser->Filename.Count, Parser->Filename.Start);
  }

  return;
}

function c_token*
PeekTokenRawPointer(c_parse_result* Parser, u32 Lookahead = 0)
{
  c_token* Result = {};
  if (Remaining(&Parser->Tokens, Lookahead))
  {
    Result = Parser->Tokens.At+Lookahead;
  }
  else
  {
    Warn("Tried to peek a token on an empty stream");
  }

  return Result;
}

function c_token
PeekTokenRaw(c_parse_result* Parser, u32 Lookahead = 0)
{
  c_token* Pointer = PeekTokenRawPointer(Parser, Lookahead);
  c_token Result = {};
  if (Pointer) Result = *Pointer;
  return Result;
}

function u32
OffsetOfNext(c_parse_result* Parser, u32 Offset,  c_token_type Close)
{
  c_token* Next = PeekTokenRawPointer(Parser, Offset);

  while (Next && Next->Type != Close)
  {
    ++Offset;
    Next = PeekTokenRawPointer(Parser, Offset);
  }

  return Offset;
}

function c_token*
PeekTokenPointer(c_parse_result* Parser, u32 Lookahead = 0)
{
  c_token* Result = {};
  u32 TokenHits = 0;
  u32 LocalLookahead = 0;
  while (Remaining(&Parser->Tokens, LocalLookahead))
  {
    Result = PeekTokenRawPointer(Parser, LocalLookahead);
    if ( Result->Type == CTokenType_CommentSingleLine)
    {
      LocalLookahead = OffsetOfNext(Parser, LocalLookahead, CTokenType_Newline);
    }
    else if ( Result->Type == CTokenType_CommentMultiLineStart)
    {
      LocalLookahead = OffsetOfNext(Parser, LocalLookahead, CTokenType_CommentMultiLineEnd);
    }
    else if (IsWhitespace(Result->Type))
    {
    }
    else
    {
      if (TokenHits++ == Lookahead)
      {
        break;
      }
    }

    ++LocalLookahead;
  }

  return Result;
}

function c_token
PeekToken(c_parse_result* Parser, u32 Lookahead = 0)
{
  c_token* Pointer = PeekTokenPointer(Parser, Lookahead);
  c_token Result = {};
  if (Pointer) Result = *Pointer;
  return Result;
}

function c_token
PopTokenRaw(c_parse_result* Parser)
{
  c_token Result = PeekTokenRaw(Parser);
  if (Remaining(&Parser->OutputTokens))
  {
    Push(Result, &Parser->OutputTokens);
  }

  if (Remaining(&Parser->Tokens))
  {
    Advance(Parser);
  }

  if (Result.Type == CTokenType_Identifier && StringsMatch(CS("break_here"), Result.Value))
  {
    RuntimeBreak();
    if (Remaining(&Parser->Tokens)) { Advance(Parser); }
    Result = PeekTokenRaw(Parser);
  }

  return Result;
}

function void
EatUntil(c_parse_result* Parser, c_token_type Close)
{
  while (Remaining(&Parser->Tokens))
  {
    if(PopTokenRaw(Parser).Type == Close)
    {
      break;
    }
  }
}

function c_token
PopToken(c_parse_result* Parser)
{
  c_token Result = {};
  while (Remaining(&Parser->Tokens))
  {
    Result = PopTokenRaw(Parser);
    if ( Result.Type == CTokenType_CommentSingleLine)
    {
      EatUntil(Parser, CTokenType_Newline);
    }
    else if ( Result.Type == CTokenType_CommentMultiLineStart)
    {
      EatUntil(Parser, CTokenType_CommentMultiLineEnd);
    }
    else if ( IsWhitespace(Result.Type) )
    {
    }
    else
    {
      break;
    }
  }

  return Result;
}

function b32
OptionalTokenRaw(c_parse_result* Parser, c_token_type Type)
{
  b32 Result = (PeekTokenRaw(Parser).Type == Type);
  if (Result) { PopTokenRaw(Parser); }
  return Result;
}

function b32
OptionalToken(c_parse_result* Parser, c_token T)
{
  b32 Result = (PeekToken(Parser) == T);
  if (Result) { PopToken(Parser); }
  return Result;
}

function b32
OptionalToken(c_parse_result* Parser, c_token_type Type)
{
  b32 Result = (PeekToken(Parser).Type == Type);
  if (Result) { PopToken(Parser); }
  return Result;
}

function counted_string
PopIdentifier(ansi_stream* SourceFileStream)
{
  counted_string Result = {
    .Start = SourceFileStream->At
  };

  while (Remaining(SourceFileStream))
  {
    c_token T = GetToken(SourceFileStream);
    if (T.Type == CTokenType_Unknown)
    {
      ++SourceFileStream->At;
    }
    else
    {
      break;
    }
  }

  Result.Count = (umm)SourceFileStream->At - (umm)Result.Start;
  return Result;
}

function c_parse_result
TokenizeAnsiStream(ansi_stream Code, memory_arena* Memory, b32 IgnoreQuotes = False)
{
  c_parse_result Result = {
    .Filename = Code.Filename
  };

  if (!Code.Start)
  {
    Error("Input AnsiStream for %.*s is null.", (s32)Code.Filename.Count, Code.Filename.Start);
    return Result;
  }

  Result.Tokens = AllocateTokenBuffer(Memory, (u32)Megabytes(1));
  if (!Result.Tokens.Start)
  {
    Error("Allocating Token Buffer");
    return Result;
  }


  Result.OutputTokens = AllocateTokenBuffer(Memory, (u32)Megabytes(1));
  if (!Result.OutputTokens.Start)
  {
    Error("Allocating OutputTokens Buffer");
    return Result;
  }

  b32 ParsingSingleLineComment = False;
  b32 ParsingMultiLineComment = False;
  while(Remaining(&Code))
  {
    const c_token FirstT = GetToken(&Code);
    c_token PushT = { .Type = FirstT.Type, .Value = CS(Code.At, 1) };

    switch (FirstT.Type)
    {
      case CTokenType_FSlash:
      {
        const c_token SecondT = GetToken(&Code, 1);
        switch (SecondT.Type)
        {
          case CTokenType_FSlash:
          {
            ParsingSingleLineComment = True;
            PushT.Type = CTokenType_CommentSingleLine;
            PushT.Value = CS(Code.At, 2);
            Advance(&Code);
            Advance(&Code);
          } break;

          case CTokenType_Star:
          {
            ParsingMultiLineComment = True;
            PushT.Type = CTokenType_CommentMultiLineStart;
            PushT.Value = CS(Code.At, 2);
            Advance(&Code);
            Advance(&Code);
          } break;

          default:
          {
            Advance(&Code);
          } break;
        }
      } break;

      case CTokenType_Star:
      {
        if (GetToken(&Code, 1).Type == CTokenType_FSlash)
        {
            ParsingMultiLineComment = False;
            PushT.Type = CTokenType_CommentMultiLineEnd;
            PushT.Value = CS(Code.At, 2);
            Advance(&Code);
            Advance(&Code);
        }
        else
        {
          PushT.Type = CTokenType_Star;
          PushT.Value = CS(Code.At, 1);
          Advance(&Code);
        }
      } break;

      case CTokenType_SingleQuote:
      {
        if (IgnoreQuotes || ParsingSingleLineComment || ParsingMultiLineComment)
        {
          Advance(&Code);
        }
        else
        {
          PushT.Type = CTokenType_Char;
          PushT.Value = PopQuotedCharLiteral(&Code, True);
        }
      } break;

      case CTokenType_DoubleQuote:
      {
        if (IgnoreQuotes || ParsingSingleLineComment || ParsingMultiLineComment)
        {
          Advance(&Code);
        }
        else
        {
          PushT.Type = CTokenType_String;
          PushT.Value = PopQuotedString(&Code, True);
        }
      } break;

      case CTokenType_Newline:
      {
        ParsingSingleLineComment = False;
        Advance(&Code);
      } break;

      case CTokenType_Unknown:
      {
        PushT.Type = CTokenType_Identifier;
        PushT.Value = PopIdentifier(&Code);
      } break;

      default:
      {
        Advance(&Code);
      } break;
    }

    Assert(PushT.Type);
    Push(PushT, &Result.Tokens);

    continue;
  }


  TruncateToCurrentSize(&Result.Tokens);
  Rewind(&Result.Tokens);
  Result.Valid = True;

#if 0
  while (Remaining(&Result.Tokens))
  {
    PrintToken(PopTokenRaw(&Result));
  }
  Result.Valid = False;
#endif

  return Result;
}

function c_parse_result
TokenizeString(counted_string Code, counted_string Filename, memory_arena* Memory, b32 IgnoreQuotes = False)
{
  c_parse_result Result = TokenizeAnsiStream(AnsiStream(Code, Filename), Memory, IgnoreQuotes);
  return Result;
}

function c_parse_result
TokenizeFile(counted_string Filename, memory_arena* Memory, b32 IgnoreQuotes = False)
{
  ansi_stream SourceFileStream = AnsiStreamFromFile(Filename, Memory);
  c_parse_result Result = TokenizeAnsiStream(SourceFileStream, Memory, IgnoreQuotes);
  return Result;
}

function void
TruncateAtNextLineEnd(c_parse_result* Parser, u32 Count)
{
  while (Remaining(&Parser->Tokens))
  {
    if(PopTokenRaw(Parser).Type == CTokenType_Newline)
    {
      if (Count == 0)
      {
        break;
      }
      --Count;
    }
  }
  Parser->Tokens.End = Parser->Tokens.At;
}

function void
RewindUntil(c_parse_result* Parser, c_token_type Type)
{
  while (Parser->Tokens.At > Parser->Tokens.Start)
  {
    if (Parser->Tokens.At->Type == Type)
    {
      ++Parser->Tokens.At;
      break;
    }
    --Parser->Tokens.At;
  }
}

function void
TruncateAtPreviousLineStart(c_parse_result* Parser, u32 Count )
{
  while (Parser->Tokens.At > Parser->Tokens.Start)
  {
    if (Parser->Tokens.At->Type == CTokenType_Newline)
    {
      if (Count == 0)
      {
        ++Parser->Tokens.At;
        break;
      }
      --Count;
    }
    --Parser->Tokens.At;
  }

  Parser->Tokens.Start = Parser->Tokens.At;
}

function void
Dump(c_parse_result* Parser, u32 LinesToDump = u32_MAX)
{
  c_parse_result LocalParser = *Parser;
  LocalParser.OutputTokens = {};

  Rewind(&LocalParser.Tokens);
  while(Remaining(&LocalParser.Tokens) && LinesToDump > 0)
  {
    c_token T = PopTokenRaw(&LocalParser);
    PrintToken(T);

    if (T.Type == CTokenType_Newline)
    {
      --LinesToDump;
    }
  }
}

function void
OutputErrorHelperLine(c_parse_result* Parser, c_token* ErrorToken, c_token Expected, counted_string ErrorString, u32 LineNumber)
{
  Rewind(&Parser->Tokens);

  u32 ColumnCount = 1;
  while (Remaining(&Parser->Tokens))
  {

    if (Parser->Tokens.At == ErrorToken)
    {
      counted_string TokenTypeName = ToString(ErrorToken->Type);
      Log("^----> Unexpected token type: %.*s", TokenTypeName.Count, TokenTypeName.Start );

      if (ErrorToken->Value.Count)
      {
        Log("(%.*s)" , ErrorToken->Value.Count, ErrorToken->Value.Start);
      }

      counted_string ExpectedTypeName = ToString(Expected.Type);
      Log(". Expected: %.*s", ExpectedTypeName.Count, ExpectedTypeName.Start);

      if (Expected.Value.Count)
      {
        Log("(%.*s)" , Expected.Value.Count, Expected.Value.Start);
      }

      Log("\n");

      for (u32 ColumnIndex = 0;
          ColumnIndex < ColumnCount;
          ++ColumnIndex)
      {
        Log(" ");
      }

      Log("      %.*s:%u:%u: %.*s\n", Parser->Filename.Count, Parser->Filename.Start, LineNumber, ColumnCount, ErrorString.Count, ErrorString.Start);

      break;
    }

    if (Parser->Tokens.At->Type == CTokenType_Newline)
    {
    }
    else if (Parser->Tokens.At->Value.Count)
    {
      for (u32 ValueIndex = 0;
          ValueIndex < Parser->Tokens.At->Value.Count;
          ++ValueIndex)
      {
        Log(" ");
        ++ColumnCount;
      }
    }
    else
    {
      Log(" ");
      ++ColumnCount;
    }

    ++Parser->Tokens.At;
  }

  return;
}

function void
OutputParsingError(c_parse_result* Parser, c_token* ErrorToken, c_token ExpectedToken, counted_string ErrorString)
{
  u32 LinesOfContext = 4;

  Log("----\n");

  /* u32 ColumnNumber = 0; */
  /* Log("%.*s:%u:%u: Error\n", Parser->Filename.Count, Parser->Filename.Start, Parser->LineNumber, ColumnNumber); */

  c_parse_result LocalParser = *Parser;
  LocalParser.OutputTokens = {};
  LocalParser.Tokens.At = ErrorToken;

  c_parse_result LeadingLines  = LocalParser;
  c_parse_result ErrorLine     = LocalParser;
  c_parse_result TrailingLines = LocalParser;

  {
    RewindUntil(&LeadingLines, CTokenType_Newline);
    LeadingLines.Tokens.End = LeadingLines.Tokens.At;
    TruncateAtPreviousLineStart(&LeadingLines, LinesOfContext);
    Dump(&LeadingLines);
  }

  {
    TruncateAtPreviousLineStart(&ErrorLine, 0);
    TruncateAtNextLineEnd(&ErrorLine, 0);
    Dump(&ErrorLine);

    if (ErrorToken >= ErrorLine.Tokens.Start &&
        ErrorToken < ErrorLine.Tokens.End)
    {
      OutputErrorHelperLine(&ErrorLine, ErrorToken, ExpectedToken, ErrorString, LocalParser.LineNumber);
    }
    else
    {
      Error("Determining where the error occured");
      Log("%s:%u:0\n", __FILE__, __LINE__);
      Log("Error was : %.*s\n", ErrorString.Count, ErrorString.Start);
    }

  }

  {
    EatUntil(&TrailingLines, CTokenType_Newline);
    TrailingLines.Tokens.Start = TrailingLines.Tokens.At;
    TruncateAtNextLineEnd(&TrailingLines, LinesOfContext);
    Dump(&TrailingLines);
  }

  Log("----\n");

  return;
}

function void
OutputParsingError(c_parse_result* Parser, c_token* ErrorToken, counted_string ErrorString)
{
  OutputParsingError(Parser, ErrorToken, CToken(CTokenType_Unknown), ErrorString);
  return;
}

function c_token
RequireToken(c_parse_result* Parser, c_token ExpectedToken)
{
  c_token* ErrorToken = PeekTokenPointer(Parser);
  c_token Result = *ErrorToken;

  if (Result.Type != ExpectedToken.Type || (ExpectedToken.Value.Count > 0 && !StringsMatch(ExpectedToken.Value, Result.Value) ))
  {
    OutputParsingError(Parser, ErrorToken, ExpectedToken, CS("Require Token Failed"));
    Parser->Valid = False;
    RuntimeBreak();
  }
  else
  {
    PopToken(Parser);
  }

  return Result;
}

function c_token
RequireToken(c_parse_result* Parser, c_token_type ExpectedType)
{
  c_token Result = RequireToken(Parser, CToken(ExpectedType));
  return Result;
}

d_union_member
DUnionMember(counted_string Name, counted_string Type, d_union_flags Flags)
{
  d_union_member Result = {};
  Result.Name = Name;
  Result.Type = Type;
  Result.Flags = Flags;
  return Result;
}

void
PushMember(d_union_decl* dUnion, c_token MemberIdentifierToken, d_union_flags Flags, memory_arena* Memory)
{
  Assert(MemberIdentifierToken.Type == CTokenType_Identifier);
  d_union_member Member = DUnionMember(MemberIdentifierToken.Value, MemberIdentifierToken.Value, Flags);
  Push(&dUnion->Members, Member, Memory);
}

function counted_string
GenerateEnumDef(d_union_decl* dUnion, memory_arena* Memory)
{
  TIMED_FUNCTION();

  counted_string Result = FormatCountedString(Memory, CSz("enum %.*s_type\n{\n  type_%.*s_noop,\n"), dUnion->Name.Count, dUnion->Name.Start, dUnion->Name.Count, dUnion->Name.Start);

  for (d_union_member_iterator Iter = Iterator(&dUnion->Members);
      IsValid(&Iter);
      Advance(&Iter))
  {
    d_union_member* Member = &Iter.At->Element;
    Result = Concat(Result, FormatCountedString(Memory, CSz("  type_%S,\n"), Member->Type), Memory);
  }

  Result = Concat(Result, CS("};\n\n"), Memory);
  return Result;
}

function counted_string
GenerateStructDef(d_union_decl* dUnion, memory_arena* Memory)
{
  TIMED_FUNCTION();

  counted_string UnionName = dUnion->Name;
  counted_string TagType = dUnion->CustomEnumType.Count ?
    dUnion->CustomEnumType :
    FormatCountedString(Memory, CSz("%S_type"), UnionName);

  counted_string Result = FormatCountedString(Memory, CSz("struct %S\n{\n  %S Type;\n"),
      UnionName, TagType);

  ITERATE_OVER(&dUnion->CommonMembers)
  {
    struct_member* Member = GET_ELEMENT(Iter);
    Assert(Member->Type == type_struct_member_variable);
    Result =
      Concat(Result,
        FormatCountedString(Memory, CSz("  %S %S;\n"),
          Member->struct_member_variable.Type,
          Member->struct_member_variable.Name),
      Memory);
  }
  Result = Concat(Result, CS("\n  union\n  {\n"), Memory);

  for (d_union_member_iterator Iter = Iterator(&dUnion->Members);
      IsValid(&Iter);
      Advance(&Iter))
  {
    d_union_member* Member = &Iter.At->Element;
    if (Member->Flags != d_union_flag_enum_only)
    {
      Result = Concat(Result, FormatCountedString(Memory, CSz("    %.*s %.*s;\n"), Member->Type.Count, Member->Type.Start, Member->Name.Count, Member->Name.Start), Memory);
    }
  }

  Result = Concat(Result, CS("  };\n};\n\n"), Memory);

  return Result;
}

function enum_def*
GetEnumByType(enum_def_stream* ProgramEnums, counted_string EnumType)
{
  TIMED_FUNCTION();

  enum_def* Result = 0;
  for (enum_def_iterator Iter = Iterator(ProgramEnums);
      IsValid(&Iter);
      Advance(&Iter))
  {
    enum_def* Enum = &Iter.At->Element;
    if (StringsMatch(Enum->Name, EnumType))
    {
      Result = Enum;
      break;
    }
  }

  return Result;
}

function struct_def*
GetStructByType(struct_def_stream* ProgramStructs, counted_string StructType)
{
  TIMED_FUNCTION();

  struct_def* Result = 0;
  for (struct_def_iterator Iter = Iterator(ProgramStructs);
      IsValid(&Iter);
      Advance(&Iter))
  {
    struct_def* Struct = &Iter.At->Element;
    if (StringsMatch(Struct->Name, StructType))
    {
      Result = Struct;
      break;
    }
  }

  return Result;
}

function datatype
GetDatatypeByName(program_datatypes* Datatypes, counted_string Name)
{
  TIMED_FUNCTION();

  struct_def* S = GetStructByType(&Datatypes->Structs, Name);
  enum_def* E = GetEnumByType(&Datatypes->Enums, Name);

  datatype Result = {};

  if (S)
  {
    Assert(!E);
    Result.Type = type_struct_def;
    Result.struct_def = S;
  }
  else if (E)
  {
    Result.Type = type_enum_def;
    Result.enum_def = E;
  }
  else
  {
    InvalidCodePath();
  }

  return Result;
}

d_union_decl
ParseDiscriminatedUnion(c_parse_result* Parser, program_datatypes* Datatypes, counted_string Name, memory_arena* Memory)
{
  TIMED_FUNCTION();

  d_union_decl dUnion = {};

  dUnion.Name = Name;

  if (OptionalToken(Parser, CTokenType_OpenBrace))
  {
    b32 Complete = False;
    while (!Complete && Remaining(&Parser->Tokens))
    {
      c_token Interior = PeekToken(Parser);

      switch (Interior.Type)
      {
        case CTokenType_Identifier:
        {
          RequireToken(Parser, Interior);

          d_union_flags Flags = {};
          if ( OptionalToken(Parser, CToken(ToString(enum_only))) )
          {
            Flags = d_union_flag_enum_only;
          }

          PushMember(&dUnion, Interior, Flags, Memory);
        } break;

        case CTokenType_CloseBrace:
        {
          RequireToken(Parser, CTokenType_CloseBrace);
          Complete = True;
        } break;

        default:
        {
          Parser->Valid = False;
          Complete = True;
        } break;
      }
    }
  }
  else
  {
    dUnion.CustomEnumType = RequireToken(Parser, CTokenType_Identifier).Value;;

    enum_def* EnumDef = GetEnumByType(&Datatypes->Enums, dUnion.CustomEnumType);
    if (EnumDef)
    {
      ITERATE_OVER(&EnumDef->Fields)
      {
        enum_field* Field = GET_ELEMENT(Iter);
        counted_string MemberName = Concat(Concat(dUnion.Name, CS("_"), Memory), Field->Name, Memory);
        PushMember(&dUnion, CToken(MemberName), d_union_flag_none, Memory);
      }
    }
    else
    {
      Error("Couldn't find enum %.*s", (s32)dUnion.CustomEnumType.Count, dUnion.CustomEnumType.Start);
    }
  }

  if (OptionalToken(Parser, CTokenType_Comma))
  {
    RequireToken(Parser, CTokenType_OpenBrace);
    while (!OptionalToken(Parser, CTokenType_CloseBrace))
    {
      struct_member Decl = {
        .Type = type_struct_member_variable,
        .struct_member_variable = {
          .Type = RequireToken(Parser, CTokenType_Identifier).Value,
          .Name = RequireToken(Parser, CTokenType_Identifier).Value
        }
      };
      Push(&dUnion.CommonMembers, Decl, Memory);
    }
  }

  return dUnion;
}

function b32
DoDebugWindow(const char** ArgStrings, s32 ArgCount)
{
  b32 Result = False;
  for (s32 ArgIndex = 1;
      ArgIndex < ArgCount;
      ++ArgIndex)
  {
    counted_string Arg = CS(ArgStrings[ArgIndex]);
    if (StringsMatch(CS("-d"), Arg) ||
        StringsMatch(CS("--do-debug-window"), Arg) )
    {
      Result = True;
    }
  }

  return Result;
}

function arguments
ParseArgs(const char** ArgStrings, s32 ArgCount, memory_arena* Memory)
{
  arguments Result = {
    .Outpath = CS("src/metaprogramming/output"),
    .Files = AllocateBuffer<counted_string_cursor, counted_string>((u32)ArgCount, Memory),
  };

  for (s32 ArgIndex = 1;
      ArgIndex < ArgCount;
      ++ArgIndex)
  {
    counted_string Arg = CS(ArgStrings[ArgIndex]);
    if (StringsMatch(CS("-d"), Arg) ||
        StringsMatch(CS("--do-debug-window"), Arg) )
    {
      Result.DoDebugWindow = True;
    }
    else if (StringsMatch(CS("-o"), Arg) ||
        StringsMatch(CS("--output-path"), Arg) )
    {
      if (++ArgIndex < ArgCount)
      {
        Result.Outpath = CS(ArgStrings[ArgIndex]);
      }
      else
      {
        Error("Please pass an argument to -o | --output-path");
      }

    }
    else
    {
      Push(Arg, &Result.Files);
    }
  }

  TruncateToCurrentSize(&Result.Files);
  Rewind(&Result.Files);

  return Result;
}

global_variable random_series TempFileEntropy = {3215432};

function b32
Output(c_token_cursor Code, counted_string Filename, memory_arena* Memory)
{
  TIMED_FUNCTION();
  b32 Result = False;

  native_file TempFile = GetTempFile(&TempFileEntropy, Memory);
  if (TempFile.Handle)
  {
    Rewind(&Code);
    b32 FileWritesSucceeded = True;
    while(Remaining(&Code))
    {
      if (Code.At->Value.Count)
      {
        FileWritesSucceeded &= WriteToFile(&TempFile, Code.At->Value);
      }
      else
      {
        FileWritesSucceeded &= WriteToFile(&TempFile, CS((const char*)&Code.At->Type, 1));
      }

      Advance(&Code);
    }
    FileWritesSucceeded &= CloseFile(&TempFile);

    if (FileWritesSucceeded)
    {
      if (Rename(TempFile, Filename))
      {
        Result = True;
      }
      else
      {
        Error("Renaming tempfile: %.*s -> %.*s", (s32)TempFile.Path.Count, TempFile.Path.Start, (s32)Filename.Count, Filename.Start);
      }
    }
    else
    {
      Error("Writing to tempfile: %.*s", (s32)TempFile.Path.Count, TempFile.Path.Start);
    }
  }
  else
  {
    Error("Opening tempfile: %.*s", (s32)TempFile.Path.Count, TempFile.Path.Start);
  }

  return Result;
}

enum output_mode
{
  Output_NoOverwrite,
  Output_Unsafe,
};

function b32
Output(counted_string Code, counted_string OutputFilename, memory_arena* Memory, output_mode Mode = Output_NoOverwrite)
{
  TIMED_FUNCTION();
  Mode = Output_Unsafe;
  b32 Result = False;

  native_file TempFile = GetTempFile(&TempFileEntropy, Memory);
  if (TempFile.Handle)
  {
    b32 FileWritesSucceeded = WriteToFile(&TempFile, Code);
    FileWritesSucceeded &= WriteToFile(&TempFile, CS("\n"));
    FileWritesSucceeded &= CloseFile(&TempFile);

    if (FileWritesSucceeded)
    {
      if (Mode == Output_NoOverwrite)
      {
        if (FileExists(OutputFilename))
        {
          counted_string FileContents = ReadEntireFileIntoString(OutputFilename, Memory);
          if (StringsMatch(Trim(Code), Trim(FileContents)))
          {
            Info("File contents matched output for %.*s", (u32)OutputFilename.Count, OutputFilename.Start);
          }
          else
          {
            Error("File contents didn't match for %.*s", (u32)OutputFilename.Count, OutputFilename.Start);
            Error("TODO(Jesse): Should probably emit to a similar filname with an incremented extension or something..");
          }
        }
        else if (Rename(TempFile, OutputFilename))
        {
          Info("Generated and output %.*s", (u32)OutputFilename.Count, OutputFilename.Start);
          Result = True;
        }
        else
        {
          Error("Renaming tempfile: %.*s -> %.*s", (s32)TempFile.Path.Count, TempFile.Path.Start, (s32)OutputFilename.Count, OutputFilename.Start);
        }
      }
      else
      {
        if (Rename(TempFile, OutputFilename))
        {
          Info("Generated and output %.*s", (u32)OutputFilename.Count, OutputFilename.Start);
          Result = True;
        }
        else
        {
          Error("Renaming tempfile: %.*s -> %.*s", (s32)TempFile.Path.Count, TempFile.Path.Start, (s32)OutputFilename.Count, OutputFilename.Start);
        }
      }
    }
    else
    {
      Error("Writing to tempfile: %.*s", (s32)TempFile.Path.Count, TempFile.Path.Start);
    }
  }
  else
  {
    Error("Opening tempfile: %.*s", (s32)TempFile.Path.Count, TempFile.Path.Start);
  }

  return Result;
}

function void
DumpStringStreamToConsole(counted_string_stream* Stream)
{
  for (counted_string_iterator Iter = Iterator(Stream);
      IsValid(&Iter);
      Advance(&Iter))
  {
    counted_string Message = Iter.At->Element;
    Log("%.*s\n", Message.Count, Message.Start);
  }
}

function void
EatBetween(c_parse_result* Parser, c_token_type Open, c_token_type Close)
{
  u32 Depth = 0;
  RequireToken(Parser, Open);

  while (Parser->Valid && Remaining(&Parser->Tokens))
  {
    c_token T = PopToken(Parser);

    if (T.Type == Open)
    {
      ++Depth;
    }

    if (T.Type == Close)
    {
      if (Depth == 0) break;
      --Depth;
    }
  }

  return;
}

function void
EatNextScope(c_parse_result* Parser)
{
  EatBetween(Parser, CTokenType_OpenBrace, CTokenType_CloseBrace);
  return;
}

function void
EatUnionDef(c_parse_result* Parser)
{
  EatNextScope(Parser);
  OptionalToken(Parser, CTokenType_Semicolon);
  return;
}

function struct_def
StructDef(counted_string Name, counted_string Sourcefile)
{
  struct_def Result = {
    .Name = Name,
    .DefinedInFile = Sourcefile
  };
 
  return Result;
}

function b32
EatStar(c_parse_result* Parser)
{
  b32 Result = OptionalToken(Parser, CTokenType_Star);
  return Result;
}

function void
EatFunctionDecl(c_parse_result* Parser)
{
  EatBetween(Parser, CTokenType_OpenParen, CTokenType_CloseParen);

  if (PeekToken(Parser).Type == CTokenType_Semicolon)
  {
    RequireToken(Parser, CTokenType_Semicolon);
  }
  else
  {
    EatNextScope(Parser);
  }

  return;
}

function b32
IsCxxDefinitionKeyword(counted_string Value)
{
  b32 Result = StringsMatch(Value, CS("volatile")) ||
               StringsMatch(Value, CS("struct")) ||
               StringsMatch(Value, CS("const"));
  return Result;
}

function struct_def
ParseStructBody(c_parse_result* Parser, counted_string StructName, memory_arena* Memory);

function struct_member
ParseDeclaration(c_parse_result* Parser, counted_string StructName, memory_arena* Memory)
{
  TIMED_FUNCTION();
  struct_member Result = {
    .Type = type_struct_member_variable
  };

  c_token FirstToken = PeekToken(Parser);
  b32 Unnamed = False;
  switch(FirstToken.Type)
  {
    case CTokenType_Tilde:
    {
      RequireToken(Parser, CTokenType_Tilde);
      counted_string Name = RequireToken(Parser, CTokenType_Identifier).Value;
      Assert(StringsMatch(StructName, Name));
      EatFunctionDecl(Parser);
      Result.Type = type_struct_member_function;
      Result.struct_member_function.Type = type_struct_member_function_destructor;
    } break;

    case CTokenType_Identifier:
    {
      b32 Done = False;
      u32 DefKeywordsEncountered = 0;
      while (!Done)
      {
        c_token NextToken = PeekToken(Parser);
        switch (NextToken.Type)
        {
          case CTokenType_LT:
          {
            EatUntil(Parser, CTokenType_GT);
          } break;

          case CTokenType_OpenParen:
          {
            Assert(StringsMatch(StructName, FirstToken.Value));
            EatFunctionDecl(Parser);
            Result.Type = type_struct_member_function;
            Result.struct_member_function.Type = type_struct_member_function_constructor;
            Done = True;
          } break;

          case CTokenType_Ampersand:
          {
            RequireToken(Parser, CTokenType_Ampersand);
            Result.struct_member_variable.Type = Concat(Result.struct_member_variable.Type, CS("&"), Memory);
          } break;

          case CTokenType_Star:
          {
            RequireToken(Parser, CTokenType_Star);
            Result.struct_member_variable.Type = Concat(Result.struct_member_variable.Type, CS("*"), Memory);
          } break;

          case CTokenType_Identifier:
          {
            if (StringsMatch(NextToken.Value, CS("unsigned")))
            {
              Result.struct_member_variable.Type = Concat( Result.struct_member_variable.Type, RequireToken(Parser, CTokenType_Identifier).Value, Memory);
            }
            else if (StringsMatch(NextToken.Value, CS("union")))
            {
              RequireToken(Parser, CToken(CS("union")));

              Result.Type = type_struct_member_union;
              Result.struct_member_union.Body = ParseStructBody(Parser, CS("anonymous union"), Memory);
              Done = True;
              Unnamed = True;
            }
            else if (StringsMatch(NextToken.Value, CS("operator")))
            {
              Result.Type = type_struct_member_function;
              Result.struct_member_function.Type = type_struct_member_function_operator;
              EatUntil(Parser, CTokenType_CloseBrace);
              Done = True;
              Unnamed = True;
            }

            if (!IsCxxDefinitionKeyword(NextToken.Value))
            {
              if (DefKeywordsEncountered == 1 && PeekToken(Parser, 1).Type == CTokenType_OpenParen)
              {
                /* counted_string FunctionName = */ RequireToken(Parser, CTokenType_Identifier);
                EatFunctionDecl(Parser);
                Result.Type = type_struct_member_function;
                Result.struct_member_function.Type = type_struct_member_function_normal;
              }

              if (DefKeywordsEncountered++ == 1)
              {
                Done = True;
              }
            }

            if (!Done)
            {
              Result.struct_member_variable.Type = Concat( Result.struct_member_variable.Type, RequireToken(Parser, CTokenType_Identifier).Value, Memory);
            }
          } break;

          InvalidDefaultWhileParsing(Parser, CS("While parsing decl type 0."));
        }

        continue;
      }

    } break;

    InvalidDefaultWhileParsing(Parser, CS("While parsing decl type 1."));
  }

  if (!Unnamed && Result.Type == type_struct_member_variable)
  {
    Result.struct_member_variable.Name = RequireToken(Parser, CTokenType_Identifier).Value;
    switch (PeekToken(Parser).Type)
    {
      case CTokenType_OpenParen:
      case CTokenType_OpenBracket:
      case CTokenType_Equals:
      {
        EatUntil(Parser, CTokenType_Semicolon);
      } break;

      case CTokenType_Semicolon:
      {
        RequireToken(Parser, CTokenType_Semicolon);
      } break;

      InvalidDefaultWhileParsing(Parser, CS("While parsing decl name."));
    }
  }

  return Result;
}

#if 0
function void
DumpCDeclStreamToConsole(struct_member_stream* Stream)
{
  for (struct_member_iterator Iter = Iterator(Stream);
      IsValid(&Iter);
      Advance(&Iter))
  {
    switch(Iter.At->Element.Type)
    {
      case type_struct_member_function:
      {
        switch(Iter.At->Element.struct_member_function.Type)
        {
          case type_struct_member_function_normal:
          {
            Log("  Function\n");
          } break;
          case type_struct_member_function_constructor:
          {
            Log("  Constructor\n");
          } break;
          case type_struct_member_function_destructor:
          {
            Log("  Destructor\n");
          } break;
          InvalidDefaultCase;
        }
      } break;

      case type_struct_member_variable:
      {
        counted_string Type = Iter.At->Element.struct_member_variable.Type;
        counted_string Name = Iter.At->Element.struct_member_variable.Name;
        Log("  %.*s %.*s\n", Type.Count, Type.Start, Name.Count, Name.Start);
      } break;

      InvalidDefaultCase;
    }
  }
}

function void
DumpStruct(struct_def* Struct)
{
  Log("%.*s\n", Struct->Name.Count, Struct->Name.Start);
  DumpCDeclStreamToConsole(&Struct->Fields);
}

function void
PrintCDecl(struct_member* Decl, struct_def_stream* ProgramStructs)
{
  switch (Decl->Type)
  {
    case type_struct_member_variable:
    {
      Log("%.*s %.*s",
          Decl->struct_member_variable.Type.Count, Decl->struct_member_variable.Type.Start,
          Decl->struct_member_variable.Name.Count, Decl->struct_member_variable.Name.Start);

      Log("\n");
    } break;

    case type_struct_member_union:
    {
      for (struct_member_iterator Iter = Iterator(&Decl->struct_member_union.Body.Fields);
          IsValid(&Iter);
          Advance(&Iter))
      {
        if (Iter.At->Element.Type == type_struct_member_variable)
        {
          struct_def* Struct = GetStructByType(ProgramStructs, Iter.At->Element.struct_member_variable.Type);
          if (Struct)
          {
            PrintCDecl(&Iter.At->Element, ProgramStructs);
            DumpStruct(Struct);
          }
          else
          {
            Error("Couldn't find struct type %*.s", (u32)Iter.At->Element.struct_member_variable.Name.Count, Iter.At->Element.struct_member_variable.Name.Start);
          }
        }
        else
        {
          Error("Nested structs/unions and function pointers unsupported.");
        }
      }
    } break;

    InvalidDefaultCase;
  }

}
#endif

function void
TrimFirstToken(c_parse_result* Parser, c_token_type TokenType)
{
  Assert(Parser->Tokens.At == Parser->Tokens.Start);
  RequireToken(Parser, TokenType);
  Parser->Tokens.Start = Parser->Tokens.At;
}

function void
TrimLastToken(c_parse_result* Parser, c_token_type TokenType)
{
  c_token* CurrentToken = Parser->Tokens.End-1;

  while (CurrentToken > Parser->Tokens.Start)
  {
    if (CurrentToken->Type == TokenType)
    {
      Parser->Tokens.End = CurrentToken-1;
      Parser->Tokens.At = Parser->Tokens.End;
      break;
    }

    --CurrentToken;
  }
}

function void
TrimTrailingWhitespace(c_parse_result* Parser)
{
  c_token* CurrentToken = Parser->Tokens.End-1;

  while (CurrentToken > Parser->Tokens.Start)
  {
    if (CurrentToken->Type == CTokenType_Space)
    {
      Parser->Tokens.End = CurrentToken;
    }
    else
    {
      break;
    }

    --CurrentToken;
  }
}

function b32
HasMemberOfType(struct_def* Struct, counted_string MemberType)
{
  b32 Result = False;
  if (MemberType.Start)
  {
    for (struct_member_iterator Iter = Iterator(&Struct->Fields);
        IsValid(&Iter) && !Result;
        Advance(&Iter))
    {
      switch (Iter.At->Element.Type)
      {
        case type_struct_member_variable:
        {
          if (StringsMatch(Iter.At->Element.struct_member_variable.Type, MemberType))
          {
            Result = True;
          }
        } break;

        InvalidDefaultCase;
      }
    }
  }

  return Result;
}

function c_parse_result
GetBodyTextForNextScope(c_parse_result* Parser)
{
  c_parse_result BodyText = *Parser;
  BodyText.OutputTokens = {};

  BodyText.Tokens.Start = BodyText.Tokens.At;
  EatNextScope(Parser);
  BodyText.Tokens.End = Parser->Tokens.At;

  TrimFirstToken(&BodyText, CTokenType_OpenBrace);
  TrimLastToken(&BodyText, CTokenType_CloseBrace);
  TrimTrailingWhitespace(&BodyText);

  return BodyText;
}

function struct_def
ParseStructBody(c_parse_result* Parser, counted_string StructName, memory_arena* Memory)
{
  TIMED_FUNCTION();
  struct_def Result = StructDef(StructName, Parser->Filename);

  RequireToken(Parser, CTokenType_OpenBrace);

  c_token NextToken = PeekToken(Parser);

  while (Remaining(&Parser->Tokens) && NextToken.Type != CTokenType_CloseBrace)
  {
    if (NextToken.Type == CTokenType_Hash)
    {
      EatUntil(Parser, CTokenType_Newline);
    }
    else
    {
      struct_member Declaration = ParseDeclaration(Parser, Result.Name, Memory);
      Push(&Result.Fields, Declaration, Memory);
    }

    NextToken = PeekToken(Parser);
  }

  return Result;
}

function enum_def
ParseEnum(c_parse_result* Parser, memory_arena* Memory)
{
  TIMED_FUNCTION();
  counted_string EnumName = RequireToken(Parser, CTokenType_Identifier).Value;

  enum_def Enum = {
    .Name = EnumName
  };

  RequireToken(Parser, CTokenType_OpenBrace);

  c_token NextToken = {};
  while (Remaining(&Parser->Tokens) && NextToken.Type != CTokenType_CloseBrace)
  {
    enum_field Field = {};
    Field.Name = RequireToken(Parser, CTokenType_Identifier).Value;

    if (OptionalToken(Parser, CTokenType_Equals))
    {
      // Can be an int literal, or a char literal : (42 or '4' or '42' or even up to '4242')
      // TODO(Jesse, id: 111, tags: back_burner, metaprogramming): Proper expression parsing.  ie: enum_value_name = (1 << 4) or enum_value_name = SOME_MACRO(thing, ding)
      Field.Value = PopToken(Parser).Value;
    }

    Push(&Enum.Fields, Field, Memory);

    if(OptionalToken(Parser, CTokenType_Comma))
    {
      NextToken = PeekToken(Parser);
    }
    else
    {
      break;
    }
  }

  return Enum;
}

function void
ParseDatatypes(c_parse_result* Parser, program_datatypes* Datatypes, memory_arena* Memory)
{
  while (Parser->Valid && Remaining(&Parser->Tokens))
  {
    c_token Token = PopToken(Parser);
    switch (Token.Type)
    {
      case CTokenType_Identifier:
      {
        if (StringsMatch(Token.Value, CS("union")))
        {
          c_token UnionName = RequireToken(Parser, CTokenType_Identifier);
          Info("unions are unsupported at the moment: %.*s", (s32)UnionName.Value.Count, UnionName.Value.Start);
          EatUnionDef(Parser);
        }
        else if (StringsMatch(Token.Value, CS("typedef")))
        {
          // typedef struct { .... } the_struct_name;
          if (OptionalToken(Parser, CToken(CS("struct"))))
          {
            if (PeekToken(Parser) == CToken(CTokenType_OpenBrace))
            {
              struct_def S = ParseStructBody(Parser, CS(""), Memory);

              RequireToken(Parser, CTokenType_CloseBrace);
              S.Name = RequireToken(Parser, CTokenType_Identifier).Value;
              RequireToken(Parser, CTokenType_Semicolon);

              Push(&Datatypes->Structs, S, Memory);
            }
            else
            {
              // C-style typedef struct THING OTHERTHING;
              EatUntil(Parser, CTokenType_Semicolon);
            }
          }
        }
        else if (StringsMatch(Token.Value, CS("enum")))
        {
          enum_def Enum = ParseEnum(Parser, Memory);
          Push(&Datatypes->Enums, Enum, Memory);
        }
        else if (StringsMatch(Token.Value, CS("struct")))
        {
          c_token T = PopToken(Parser);
          switch (T.Type)
          {
            case CTokenType_Identifier:
            {
              if ( PeekToken(Parser) == CToken(CTokenType_OpenBrace) )
              {
                struct_def S = ParseStructBody(Parser, T.Value, Memory);
                Push(&Datatypes->Structs, S, Memory);
              }
            } break;

            default: {} break;
          }
        }
      } break;

      default: {} break;
    }
  }

  return;
}

function program_datatypes
ParseAllDatatypes(c_parse_result_cursor Files_in, memory_arena* Memory)
{
  TIMED_FUNCTION();
  c_parse_result_cursor* Files = &Files_in;

  program_datatypes Result = {};

  for (u32 ParserIndex = 0;
      ParserIndex < (u32)Count(Files);
      ++ParserIndex)
  {
    c_parse_result* Parser = Files->Start+ParserIndex;
    ParseDatatypes(Parser, &Result, Memory);

    Rewind(&Parser->Tokens);
  }

  return Result;
}

function c_parse_result_cursor
AllocateTokenizedFiles(u32 Count, memory_arena* Memory)
{
  c_parse_result* Start = Allocate(c_parse_result, Memory, Count);
  c_parse_result_cursor Result = {
    .Start = Start,
    .At = Start,
    .End = Start + Count,
  };

  return Result;
}

function c_parse_result_cursor
TokenizeAllFiles(counted_string_cursor* Filenames, memory_arena* Memory)
{
  TIMED_FUNCTION();
  Assert(Filenames->At == Filenames->Start);

  c_parse_result_cursor Result = AllocateTokenizedFiles((u32)Count(Filenames), Memory);
  while ( Filenames->At < Filenames->End )
  {
    counted_string CurrentFile = *Filenames->At;

    c_parse_result Parser = TokenizeFile(CurrentFile, Memory);
    if (Parser.Valid)
    {
      Rewind(&Parser.Tokens);
      Push(Parser, &Result);
    }
    else
    {
      Error("Tokenizing File: %.*s", (s32)CurrentFile.Count, CurrentFile.Start);
    }

    ++Filenames->At;
  }

  TruncateToCurrentSize(&Result);
  Rewind(&Result);

  return Result;
}

function counted_string
GenerateCursorFor_DEPRECATED(counted_string DatatypeName, memory_arena* Memory)
{
  TIMED_FUNCTION();

  counted_string DatatypeDef = FormatCountedString(Memory,
CSz(R"INLINE_CODE(
struct %.*s_cursor
{
  %.*s* Start;
  %.*s* End;
  %.*s* At;
};
)INLINE_CODE"),
    DatatypeName.Count, DatatypeName.Start,
    DatatypeName.Count, DatatypeName.Start,
    DatatypeName.Count, DatatypeName.Start,
    DatatypeName.Count, DatatypeName.Start
    );

  counted_string ConstructorName = ToCapitalCase(DatatypeName, Memory);
  counted_string ConstructorDef = FormatCountedString(Memory,
  // TODO(Jesse, id: 180, tags: metaprogramming): Can we use Allocate() here instead?
CSz(R"INLINE_CODE(
function %.*s_cursor
%.*sCursor(umm ElementCount, memory_arena* Memory)
{
  %.*s* Start = (%.*s*)PushStruct(Memory, sizeof(%.*s), 1, 1);
  %.*s_cursor Result = {
    .Start = Start,
    .End = Start+ElementCount,
    .At = Start,
  };
  return Result;
};
)INLINE_CODE"),
    DatatypeName.Count, DatatypeName.Start,
    ConstructorName.Count, ConstructorName.Start,
    DatatypeName.Count, DatatypeName.Start,
    DatatypeName.Count, DatatypeName.Start,
    DatatypeName.Count, DatatypeName.Start,
    DatatypeName.Count, DatatypeName.Start
    );


  counted_string Result = Concat(DatatypeDef, ConstructorDef, Memory);
  return Result;
}

function void
DoWorkToOutputThisStuff(c_parse_result* Parser, counted_string OutputForThisParser, counted_string NewFilename, memory_arena* Memory)
{
  TIMED_FUNCTION();

  if (!Parser->Valid)
  {
    Error("Bad parser state");
    return;
  }

  // TODO(Jesse id: 182, tags: high_priority) This should respect Args.Outpath passed in!
  if (PeekTokenRaw(Parser).Type == CTokenType_Newline &&
      PeekTokenRaw(Parser, 1).Type == CTokenType_Hash &&
      PeekTokenRaw(Parser, 2) == CToken(CS("include")))
  {
    RequireToken(Parser, CToken(CTokenType_Hash));
    RequireToken(Parser, CToken(CS("include")));
    RequireToken(Parser, CTokenType_LT);
    RequireToken(Parser, CToken(CS("metaprogramming")));
    RequireToken(Parser, CTokenType_FSlash);
    RequireToken(Parser, CToken(CS("output")));
    RequireToken(Parser, CTokenType_FSlash);
    counted_string IncludeFilename = RequireToken(Parser, CTokenType_Identifier).Value;

    string_builder IncludePathBuilder = {};
    Append(&IncludePathBuilder, CS("src/metaprogramming/output/"));
    Append(&IncludePathBuilder, IncludeFilename);

    if (OptionalToken(Parser, CTokenType_Dot))
    {
      Append(&IncludePathBuilder, CS("."));
      counted_string Extension = RequireToken(Parser, CTokenType_Identifier).Value;
      Append(&IncludePathBuilder, Extension);
    }

    RequireToken(Parser, CTokenType_GT);

    counted_string IncludePath = Finalize(&IncludePathBuilder, Memory);
    Output(OutputForThisParser, IncludePath, Memory);
  }
  else
  {
    // TODO(Jesse id: 183, tags: high_priority) This should respect Args.OutPath passed in!
    counted_string IncludePath = Concat(CS("src/metaprogramming/output/"), NewFilename, Memory);
    Output(OutputForThisParser, IncludePath, Memory);

    Push(CToken(CTokenType_Newline), &Parser->OutputTokens);
    Push(CToken(CTokenType_Hash), &Parser->OutputTokens);
    Push(CToken(CS("include")), &Parser->OutputTokens);
    Push(CToken(CTokenType_Space), &Parser->OutputTokens);

    Push(CToken(CTokenType_LT), &Parser->OutputTokens);
    Push(CToken(CS("metaprogramming")), &Parser->OutputTokens);
    Push(CToken(CTokenType_FSlash), &Parser->OutputTokens);
    Push(CToken(CS("output")), &Parser->OutputTokens);
    Push(CToken(CTokenType_FSlash), &Parser->OutputTokens);
    Push(CToken(NewFilename), &Parser->OutputTokens);
    Push(CToken(CTokenType_GT), &Parser->OutputTokens);
  }
}

// TODO(Jesse, id: 113, tags: cleanup): Remove these?
debug_global platform Plat = {};
debug_global os Os = {};

// TODO(Jesse, id: 112, tags: bootstrap_debug_system, copy_paste): This is copy-pasted from the callgraph tests .. should we be
// able to call this from anywhere?  It's also in the platform layer
function b32
BootstrapDebugSystem(b32 OpenDebugWindow)
{
  shared_lib DebugLib = OpenLibrary(DEFAULT_DEBUG_LIB);
  if (!DebugLib) { Error("Loading DebugLib :( "); return False; }

  GetDebugState = (get_debug_state_proc)GetProcFromLib(DebugLib, "GetDebugState_Internal");
  if (!GetDebugState) { Error("Retreiving GetDebugState from Debug Lib :( "); return False; }

  if (OpenDebugWindow)
  {
    s32 DebugFlags = GLX_CONTEXT_DEBUG_BIT_ARB;
    b32 WindowSuccess = OpenAndInitializeWindow(&Os, &Plat, DebugFlags);
    if (!WindowSuccess) { Error("Initializing Window :( "); return False; }
    Assert(Os.Window);
    AssertNoGlErrors;

    InitializeOpenGlExtensions(&Os);

    b32 ShadingLanguageIsRecentEnough = CheckShadingLanguageVersion();
    if (!ShadingLanguageIsRecentEnough) {  return False; }
  }

  debug_init_debug_system_proc InitDebugSystem = (debug_init_debug_system_proc)GetProcFromLib(DebugLib, "InitDebugSystem");
  if (!InitDebugSystem) { Error("Retreiving InitDebugSystem from Debug Lib :( "); return False; }
  InitDebugSystem(OpenDebugWindow);

  debug_state* DebugState = GetDebugState();
  DebugState->DebugDoScopeProfiling = True;
  DebugState->Plat = &Plat;

  glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
  glClearDepth(1.0f);

  glBindFramebuffer(GL_FRAMEBUFFER, DebugState->GameGeoFBO.ID);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  return True;
}

function meta_func*
StreamContains(meta_func_stream* Stream, counted_string Name)
{
  meta_func* Result = {};
  ITERATE_OVER(Stream)
  {
    meta_func* Current = GET_ELEMENT(Iter);
    if (StringsMatch(Current->Name, Name))
    {
      Result = Current;
      break;
    }
  }

  return Result;
}

function counted_string*
StreamContains(counted_string_stream* Stream, counted_string Name)
{
  counted_string* Result = {};
  ITERATE_OVER(Stream)
  {
    counted_string* Current = GET_ELEMENT(Iter);
    if (StringsMatch(Current, Name))
    {
      Result = Current;
      break;
    }
  }

  return Result;
}

function person*
StreamContains(person_stream* People, counted_string Name)
{
  person* Result = {};
  ITERATE_OVER(People)
  {
    person* Current = GET_ELEMENT(Iter);
    if (StringsMatch(Current->Name, Name))
    {
      Result = Current;
      break;
    }
  }

  return Result;
}

function tagged_counted_string_stream*
StreamContains(tagged_counted_string_stream_stream* Stream, counted_string Tag)
{
  tagged_counted_string_stream* Result = {};
  ITERATE_OVER(Stream)
  {
    tagged_counted_string_stream* Current = GET_ELEMENT(Iter);
    if (StringsMatch(Current->Tag, Tag))
    {
      Result = Current;
      break;
    }
  }
  return Result;
}

function todo*
StreamContains(todo_stream* Todos, counted_string TodoId)
{
  todo* Result = {};
  ITERATE_OVER(Todos)
  {
    todo* Current = GET_ELEMENT(Iter);
    if (StringsMatch(Current->Id, TodoId))
    {
      Result = Current;
      break;
    }
  }
  return Result;
}

function tag*
StreamContains(tag_stream* TodoLists, counted_string Tag)
{
  tag* Result = {};
  ITERATE_OVER(TodoLists)
  {
    tag* Current = GET_ELEMENT(Iter);
    if (StringsMatch(Current->Tag, Tag))
    {
      Result = Current;
      break;
    }
  }
  return Result;
}

function counted_string
ConcatTokensUntilNewline(c_parse_result* Parser, memory_arena* Memory)
{
  string_builder CommentValueBuilder = {};
  while (PeekTokenRaw(Parser).Type != CTokenType_Newline)
  {
    Append(&CommentValueBuilder, PopTokenRaw(Parser).Value);
  }
  counted_string Result = Finalize(&CommentValueBuilder, Memory);
  return Result;
}

function todo*
UpdateTodo(todo_stream* Stream, todo Todo, memory_arena* Memory)
{
  todo* Result = StreamContains(Stream, Todo.Id);
  if (Result)
  {
    Result->Value = Todo.Value;
    Result->FoundInCodebase = Todo.FoundInCodebase;
  }
  else
  {
    Push(Stream, Todo, Memory);
    Result = StreamContains(Stream, Todo.Id);
  }
  return Result;
}

function tag*
GetExistingOrCreate(tag_stream* Stream, counted_string Tag, memory_arena* Memory)
{
  tag* Result = StreamContains(Stream, Tag);
  if (!Result)
  {
    tag NewTag = { .Tag = Tag };
    Push(Stream, NewTag, Memory);
    Result = StreamContains(Stream, Tag);
  }
  return Result;
}

function person*
GetExistingOrCreate(person_stream* People, counted_string PersonName, memory_arena* Memory)
{
  person* Person = StreamContains(People, PersonName);
  if (!Person)
  {
    person NewPerson = { .Name = PersonName };
    Push(People, NewPerson, Memory);
    Person = StreamContains(People, PersonName);
  }
  return Person;
}

function void
EatWhitespace(c_parse_result* Parser)
{
  while (Remaining(&Parser->Tokens) && IsWhitespace(PeekTokenRaw(Parser).Type))
  {
    PopTokenRaw(Parser);
  }

  return;
}

static u32 LargestIdFoundInFile = 0;

function todo
Todo(counted_string Id, counted_string Value, b32 FoundInCodebase)
{
  todo Result = { .Id = Id, .Value = Value, .FoundInCodebase = FoundInCodebase };
  return Result;
}

function person_stream
ParseAllTodosFromFile(counted_string Filename, memory_arena* Memory)
{
  person_stream People = {};

  c_parse_result Parser_ = TokenizeFile(Filename, Memory, True);
  c_parse_result* Parser = &Parser_;

  while (Remaining(&Parser->Tokens))
  {
    RequireToken(Parser, CTokenType_Hash);
    counted_string PersonName = RequireToken(Parser, CTokenType_Identifier).Value;

    person* Person = GetExistingOrCreate(&People, PersonName, Memory);
    while (OptionalToken(Parser, CTokenType_Hash))
    {
      RequireToken(Parser, CTokenType_Hash);
      counted_string TagName = RequireToken(Parser, CTokenType_Identifier).Value;

      tag* Tag = GetExistingOrCreate(&Person->Tags, TagName, Memory);
      while (OptionalToken(Parser, CTokenType_Minus))
      {
        RequireToken(Parser, CTokenType_Hash);
        counted_string TodoId = RequireToken(Parser, CTokenType_Identifier).Value;

        LargestIdFoundInFile = Max(LargestIdFoundInFile, ToU32(TodoId));
        counted_string TodoValue = Trim(ConcatTokensUntilNewline(Parser, Memory));
        todo NewTodo = Todo(TodoId, TodoValue, False);
        Push(&Tag->Todos, NewTodo, Memory);
        EatWhitespace(Parser);
      }

      EatWhitespace(Parser);
    }
  }

  return People;
}

function counted_string
GenerateOutfileNameFor(counted_string Name, counted_string DatatypeName, memory_arena* Memory, counted_string Modifier = {})
{
  string_builder OutfileBuilder = {};
  Append(&OutfileBuilder, Name);
  Append(&OutfileBuilder, CSz("_"));
  Append(&OutfileBuilder, DatatypeName);
  if (Modifier.Count)
  {
    Append(&OutfileBuilder, CSz("_"));
    Append(&OutfileBuilder, Modifier);
  }
  Append(&OutfileBuilder, CSz(".h"));
  counted_string Result = Finalize(&OutfileBuilder, Memory);

  return Result;
}

function void
ReplaceValues(string_builder* OutputBuilder, c_parse_result* BodyText, counted_string MemberMatch, counted_string MemberType, counted_string MemberName, memory_arena* Memory)
{
  Assert(MemberName.Count);

  Rewind(&BodyText->Tokens);
  while (Remaining(&BodyText->Tokens))
  {
    c_token BodyToken = PopTokenRaw(BodyText);

    if (BodyToken.Type == CTokenType_String)
    {
      c_parse_result StringParse = TokenizeString(BodyToken.Value, BodyText->Filename, Memory, True);
      ReplaceValues(OutputBuilder, &StringParse, MemberMatch, MemberType, MemberName, Memory);
    }
    else if ( BodyToken.Type == CTokenType_OpenParen &&
              OptionalToken(BodyText, CToken(MemberMatch)) )
    {
      RequireToken(BodyText, CTokenType_Dot);
      meta_arg_operator Op = MetaArgOperator(RequireToken(BodyText, CTokenType_Identifier).Value);
      switch (Op)
      {
        case meta_arg_operator_noop:
        {
          Error("Invalid operator encountered.");
        } break;

        case type:
        {
          if (MemberType.Count)
          {
            Append(OutputBuilder, MemberType);
          }
          else
          {
            Error("Tried substituting a type for a member without one!  Did you try and call .type on an enum?");
          }
        } break;

        case name:
        {
          Append(OutputBuilder, MemberName);
        } break;

        InvalidDefaultCase;
      }

      RequireToken(BodyText, CTokenType_CloseParen);
    }
    else
    {
      Append(OutputBuilder, BodyToken.Value);
    }
  }
}

function counted_string
GetNameForCDecl(struct_member* Decl)
{
  counted_string Result = {};

  switch (Decl->Type)
  {
    case type_struct_member_noop:
    {
      Result = CSz("(unnamed)");
    } break;

    case type_struct_member_function:
    {
      // TODO(Jesse id: 189): Parse out function names?
      Result = CSz("(unnamed function)");
    } break;

    case type_struct_member_variable:
    {
      Result = Decl->struct_member_variable.Name;
    } break;

    case type_struct_member_union:
    {
      Result = CSz("(unnamed union)");
    } break;
  }

  return Result;
}

function counted_string
Transform(meta_transform_op Transformations, counted_string Input, memory_arena* Memory)
{
  counted_string Result = Input;
  while ( Transformations )
  {
    if ( Transformations & to_capital_case )
    {
      UnsetBitfield(meta_transform_op, Transformations, to_capital_case );
      Result = ToCapitalCase(Result, Memory);
    }
  }

  return Result;
}

function counted_string
Execute(meta_func* Func, counted_string ArgType, program_datatypes* Datatypes, meta_func_stream* FunctionDefs, memory_arena* Memory);

function counted_string
Execute(counted_string FuncName, c_parse_result Scope, counted_string ArgName, counted_string ArgType, program_datatypes* Datatypes, meta_func_stream* FunctionDefs, memory_arena* Memory)
{
  datatype Data = GetDatatypeByName(Datatypes, ArgType);
  datatype* Datatype = &Data;

  Rewind(&Scope.Tokens);

  string_builder OutputBuilder = {};
  while (Remaining(&Scope.Tokens))
  {
    c_token BodyToken = PopTokenRaw(&Scope);

    if ( BodyToken.Type == CTokenType_String )
    {
      c_parse_result StringParse = TokenizeString(BodyToken.Value, Scope.Filename, Memory, True);
      counted_string Code = Execute(FuncName, StringParse, ArgName, ArgType, Datatypes, FunctionDefs, Memory);
      Append(&OutputBuilder, Code);
    }
    else if ( BodyToken.Type == CTokenType_OpenParen )
    {
      if ( OptionalToken(&Scope, CToken(ArgName)) )
      {
        RequireToken(&Scope, CTokenType_Dot);
        meta_arg_operator Operator = MetaArgOperator( RequireToken(&Scope, CTokenType_Identifier).Value);

        meta_transform_op Transformations = meta_transform_op_noop;

        while (OptionalToken(&Scope, CTokenType_Dot))
        {
          meta_transform_op NextOp = MetaTransformOp(RequireToken(&Scope, CTokenType_Identifier).Value);
          if (NextOp != meta_transform_op_noop)
          {
            Transformations = (meta_transform_op)(Transformations | NextOp);
          }
          else
          {
            Error("Parsing transform ops.");
          }
        }

        switch (Operator)
        {
          case meta_arg_operator_noop:
          {
            Error("Invalid operator encountered.");
          } break;

          case type:
          {
            if (Datatype->Type == type_struct_def)
            {
              counted_string Name = Transform(Transformations, Datatype->struct_def->Name, Memory);
              Append(&OutputBuilder, Name);
            }
            else
            {
              Error("Calling .type on an enum datatype is forbidden.");
            }
          } break;

          case name:
          {
            if (Datatype->Type == type_enum_def)
            {
              counted_string Name = Transform(Transformations, Datatype->enum_def->Name, Memory);
              Append(&OutputBuilder, Name);
            }
            else
            {
              Assert(Datatype->Type == type_struct_def);
              counted_string Name = Transform(Transformations, Datatype->struct_def->Name, Memory);
              Append(&OutputBuilder, Name);
            }
          } break;

          case map_members:
          {
            RequireToken(&Scope, CTokenType_OpenParen);
            counted_string EnumValueMatch  = RequireToken(&Scope, CTokenType_Identifier).Value;
            RequireToken(&Scope, CTokenType_CloseParen);

            counted_string ContainingConstraint = {};
            if ( OptionalToken(&Scope, CTokenType_Dot) )
            {
              RequireToken(&Scope, CToken(CSz("containing")));
              RequireToken(&Scope, CTokenType_OpenParen);
              ContainingConstraint = RequireToken(&Scope, CTokenType_Identifier).Value;
              RequireToken(&Scope, CTokenType_CloseParen);
            }

            c_parse_result NextScope = GetBodyTextForNextScope(&Scope);

            if (Datatype->Type == type_struct_def)
            {
              ITERATE_OVER(&Datatype->struct_def->Fields)
              {
                struct_member* Member = GET_ELEMENT(Iter);

                counted_string MemberName = GetNameForCDecl(Member);
                counted_string MemberType = CSz("TODO(Jesse): How do we get the type?");

                switch (Member->Type)
                {
                  case type_struct_member_noop:
                  {
                    InvalidCodePath();
                  } break;

                  case type_struct_member_function:
                  {
                  } break;

                  case type_struct_member_variable:

                  {
                    if ( ContainingConstraint.Count &&
                         !StringsMatch(Member->struct_member_variable.Type, ContainingConstraint) )
                    {
                      // Containing constraint failed
                    }
                    else
                    {
                      ReplaceValues(&OutputBuilder, &NextScope, EnumValueMatch, MemberType, MemberName, Memory);
                    }

                  } break;

                  case type_struct_member_union:
                  {
                    for (struct_member_iterator UnionMemberIter = Iterator(&Member->struct_member_union.Body.Fields);
                        IsValid(&UnionMemberIter);
                        Advance(&UnionMemberIter))
                    {
                      struct_member* UnionMember = GET_ELEMENT(UnionMemberIter);
                      if (UnionMember->Type == type_struct_member_variable)
                      {
                        struct_def* Struct = GetStructByType(&Datatypes->Structs, UnionMember->struct_member_variable.Type);
                        if (Struct)
                        {
                          if (ContainingConstraint.Count && HasMemberOfType(Struct, ContainingConstraint))
                          {
                            ReplaceValues(&OutputBuilder, &NextScope, EnumValueMatch, Struct->Name, Struct->Name, Memory);
                          }
                        }
                        else
                        {
                          counted_string Name = UnionMember->struct_member_variable.Type;
                          counted_string ParentStructName = Datatype->struct_def->Name;
                          Warn("Couldn't find struct type '%.*s' in union parent '%.*s'.", (u32)Name.Count, Name.Start, (u32)ParentStructName.Count, ParentStructName.Start);
                        }

                      }
                      else
                      {
                        Error("Nested structs/unions and function pointers unsupported.");
                      }
                    }
                  } break;
                }

                continue;
              }
            }
            else
            {
              Error("Called map_members on a datatype that wasn't a struct - %.*s", (u32)Datatype->enum_def->Name.Count, Datatype->enum_def->Name.Start);
            }

          } break;

          case map_values:
          {
            RequireToken(&Scope, CTokenType_OpenParen);
            counted_string EnumValueMatch  = RequireToken(&Scope, CTokenType_Identifier).Value;
            RequireToken(&Scope, CTokenType_CloseParen);
            c_parse_result NextScope = GetBodyTextForNextScope(&Scope);

            if (Datatype->Type == type_enum_def)
            {
              ITERATE_OVER(&Datatype->enum_def->Fields)
              {
                enum_field* Member = GET_ELEMENT(Iter);
                ReplaceValues(&OutputBuilder, &NextScope, EnumValueMatch, CSz(""), Member->Name, Memory);
                continue;
              }

            }
            else
            {
              Error("Called map_values on a datatype that wasn't an enum - %.*s", (u32)Datatype->struct_def->Name.Count, Datatype->struct_def->Name.Start);
            }

          } break;
        }

        RequireToken(&Scope, CTokenType_CloseParen);
      }
      else
      {
        meta_func* NestedFunc = StreamContains( FunctionDefs, PeekToken(&Scope).Value );
        if (NestedFunc)
        {
          RequireToken(&Scope, CToken(NestedFunc->Name));

          RequireToken(&Scope, CTokenType_OpenParen);
          RequireToken(&Scope, CToken(ArgName));
          RequireToken(&Scope, CTokenType_CloseParen);
          RequireToken(&Scope, CTokenType_CloseParen);

          if (!StringsMatch(NestedFunc->Name, FuncName))
          {
            counted_string NestedCode = Execute(NestedFunc, ArgType, Datatypes, FunctionDefs, Memory);
            Append(&OutputBuilder, NestedCode);
          }
          else
          {
            Error("Recursive function calls not supported.");
          }
        }
        else
        {
          Append(&OutputBuilder, BodyToken.Value);
        }
      }
    }
    else
    {
      Append(&OutputBuilder, BodyToken.Value);
    }
  }

  counted_string Result = Finalize(&OutputBuilder, Memory);
  return Result;
}

function counted_string
Execute(meta_func* Func, counted_string ArgType, program_datatypes* Datatypes, meta_func_stream* FunctionDefs, memory_arena* Memory)
{
  counted_string Result = Execute(Func->Name, Func->Body, Func->ArgName, ArgType, Datatypes, FunctionDefs, Memory);
  return Result;
}

function void
ConcatStreams(counted_string_stream* S1, counted_string_stream* S2, memory_arena* Memory)
{
  ITERATE_OVER(S2)
  {
    counted_string* Element = GET_ELEMENT(Iter);
    Push(S1, *Element, Memory);
  }
  return;
}

function b32
IsMetaprogrammingOutput(counted_string Filename, counted_string OutputDirectory)
{
  b32 Result = Contains(Filename, OutputDirectory);
  return Result;
}

/* #include <bonsai_stdlib/headers/debug_print.h> */

function counted_string_stream
ParseDatatypeList(c_parse_result* Parser, program_datatypes* Datatypes, tagged_counted_string_stream_stream* NameLists, memory_arena* Memory)
{
  counted_string_stream Result = {};
  while (PeekToken(Parser).Type == CTokenType_Identifier)
  {
    counted_string DatatypeName    = RequireToken(Parser, CTokenType_Identifier).Value;

    struct_def* Struct                 = GetStructByType(&Datatypes->Structs, DatatypeName);
    enum_def* Enum                     = GetEnumByType(&Datatypes->Enums, DatatypeName);
    tagged_counted_string_stream* List = StreamContains(NameLists, DatatypeName);

    if (Struct || Enum)
    {
      Push(&Result, DatatypeName, Memory);
    }
    else if (List)
    {
      ConcatStreams(&Result, &List->Stream, Memory);
    }
    else
    {
      Warn("Type (%.*s) could not be resolved to a struct or named_list, ignoring it.", (u32)DatatypeName.Count, DatatypeName.Start);
    }

    OptionalToken(Parser, CTokenType_Comma);
  }

  return Result;
}

function meta_func
ParseFunctionDef(c_parse_result* Parser, counted_string FuncName)
{
  RequireToken(Parser, CTokenType_OpenParen);
  counted_string ArgName = RequireToken(Parser, CTokenType_Identifier).Value;
  RequireToken(Parser, CTokenType_CloseParen);
  c_parse_result Body = GetBodyTextForNextScope(Parser);

  meta_func Func = {
    .Name = FuncName,
    .ArgName = ArgName,
    .Body = Body,
  };

  return Func;
}

#ifndef EXCLUDE_PREPROCESSOR_MAIN
#define SUCCESS_EXIT_CODE 0
#define FAILURE_EXIT_CODE 1
s32
main(s32 ArgCount, const char** ArgStrings)
{
  setbuf(stdout, NULL);
  setbuf(stderr, NULL);

  b32 ShouldOpenDebugWindow = DoDebugWindow(ArgStrings, ArgCount);
  if (ShouldOpenDebugWindow)
  {
    if (!BootstrapDebugSystem(ShouldOpenDebugWindow))
    {
      Error("Booting debug system");
      return FAILURE_EXIT_CODE;
    }
  }

  b32 Success = True;
  if (ArgCount > 1)
  {
    memory_arena Memory_ = {};
    memory_arena* Memory = &Memory_;

    counted_string_stream SuccessFiles = {};
    counted_string_stream FailFiles = {};

    arguments Args = ParseArgs(ArgStrings, ArgCount, Memory);
    Assert(Args.Files.Start == Args.Files.At);
    Assert(Args.DoDebugWindow == ShouldOpenDebugWindow);

    c_parse_result_cursor ParsedFiles = TokenizeAllFiles(&Args.Files, Memory);
    Assert(ParsedFiles.Start == ParsedFiles.At);

    program_datatypes Datatypes = ParseAllDatatypes(ParsedFiles, Memory);
    Assert(ParsedFiles.Start == ParsedFiles.At);

    person_stream People = ParseAllTodosFromFile(CSz("todos.md"), Memory);

    tagged_counted_string_stream_stream NameLists = {};

    meta_func_stream FunctionDefs = {};

    for (u32 ParserIndex = 0;
        ParserIndex < Count(&ParsedFiles);
        ++ParserIndex)
    {
      c_parse_result* Parser = ParsedFiles.Start+ParserIndex;
      Assert(Parser->Valid);

      if (IsMetaprogrammingOutput(Parser->Filename, Args.Outpath))
      {
        Info("Skipping %.*s", (u32)Parser->Filename.Count, Parser->Filename.Start);
        continue;
      }

      Rewind(&Parser->OutputTokens);
      Rewind(&Parser->Tokens);
      Parser->LineNumber = 1;
      while (Parser->Valid && Remaining(&Parser->Tokens))
      {
        c_token NextToken = PeekTokenRaw(Parser);

        switch( NextToken.Type )
        {
          // TODO(Jesse id: 157, tags: immediate) Should support multi-line comments as well
          case CTokenType_CommentSingleLine:
          {
            Ensure( PopTokenRaw(Parser).Type == CTokenType_CommentSingleLine);
            if (OptionalToken(Parser, CToken(CSz("TODO"))))
            {
              if (OptionalToken(Parser, CTokenType_OpenParen))
              {
                counted_string PersonName = RequireToken(Parser, CTokenType_Identifier).Value;
                counted_string_stream TodoTags = {};
                b32 GotAnyTags = False;

                counted_string IdValue = {};
                OptionalToken(Parser, CTokenType_Comma);

                b32 GeneratedNewId = False;
                if (OptionalToken(Parser, CToken(CSz("id"))))
                {
                  RequireToken(Parser, CTokenType_Colon);
                  IdValue = RequireToken(Parser, CTokenType_Identifier).Value;
                }
                else
                {
                  if (!IdValue.Count)
                  {
                    GeneratedNewId = True;
                    IdValue = CS(++LargestIdFoundInFile);
                    Push(CToken(CS(" id: ")), &Parser->OutputTokens);
                    Push(CToken(IdValue), &Parser->OutputTokens);
                  }

                }

                OptionalToken(Parser, CTokenType_Comma);

                if (StringsMatch(PeekToken(Parser).Value, CSz("tags")))
                {
                  if (GeneratedNewId)
                  {
                    Push(CToken(CTokenType_Comma), &Parser->OutputTokens);
                  }

                  RequireToken(Parser, CToken(CSz("tags")));
                  GotAnyTags = True;
                  RequireToken(Parser, CTokenType_Colon);
                  Push(&TodoTags, RequireToken(Parser, CTokenType_Identifier).Value, Memory);

                  while (OptionalToken(Parser, CTokenType_Comma))
                  {
                    Push(&TodoTags, RequireToken(Parser, CTokenType_Identifier).Value, Memory);
                  }
                }

                if (!GotAnyTags)
                {
                  counted_string Tag = CSz("untagged");
                  Push(&TodoTags, Tag, Memory);
                }

                RequireToken(Parser, CTokenType_CloseParen);
                OptionalToken(Parser, CTokenType_Colon);

                // TODO(Jesse id: 156, tags: immediate) This should actually concat all comments such that
                // multi-line todos get parsed correctly.
                counted_string TodoValue = Trim(ConcatTokensUntilNewline(Parser, Memory));
                person* Person = GetExistingOrCreate(&People, PersonName, Memory);

                ITERATE_OVER(&TodoTags)
                {
                  counted_string* TodoTag = GET_ELEMENT(Iter);
                  tag* Tag = GetExistingOrCreate(&Person->Tags, *TodoTag, Memory);
                  UpdateTodo(&Tag->Todos, Todo(IdValue, TodoValue, True), Memory);
                }

              }
            }
            else
            {
              // TODO(Jesse id: 155, tags: immediate): This should eat the comment if
              // it's not a todo so that commenting a preprocessor directive works
            }

          } break;

          case CTokenType_Identifier:
          {
            c_token IdentifierToken = PopToken(Parser);
            if (StringsMatch( IdentifierToken.Value, CS("meta") ))
            {
              RequireToken(Parser, CTokenType_OpenParen);

              counted_string DirectiveString = RequireToken(Parser, CTokenType_Identifier).Value;
              metaprogramming_directive Directive = MetaprogrammingDirective(DirectiveString);

              if (Directive == meta_directive_noop)
              {
                meta_func* Func = StreamContains(&FunctionDefs, DirectiveString);

                if (Func)
                {
                  Info("Calling function : %.*s", (u32)Func->Name.Count, Func->Name.Start);

                  RequireToken(Parser, CTokenType_OpenParen);
                  counted_string DatatypeName = RequireToken(Parser, CTokenType_Identifier).Value;
                  counted_string Code = Execute(Func, DatatypeName, &Datatypes, &FunctionDefs, Memory);

                  RequireToken(Parser, CTokenType_CloseParen);
                  RequireToken(Parser, CTokenType_CloseParen);

                  if (Code.Count)
                  {
                    counted_string OutfileName = GenerateOutfileNameFor(Func->Name, DatatypeName, Memory);
                    DoWorkToOutputThisStuff(Parser, Code, OutfileName, Memory);
                  }
                  else
                  {
                    Warn("Unable to generate code for meta_func %.*s", (u32)Func->Name.Count, Func->Name.Start);
                  }
                }
                else
                {
                  Error("Couldn't resolve %.*s to a metaprogramming directive or function name", (u32)DirectiveString.Count, DirectiveString.Start);
                }
              }
              else
              {
                switch (Directive)
                {

                  case func:
                  {
                    if (OptionalToken(Parser, CTokenType_OpenParen))
                    {
                      counted_string ArgType = RequireToken(Parser, CTokenType_Identifier).Value;
                      counted_string ArgName = RequireToken(Parser, CTokenType_Identifier).Value;
                      RequireToken(Parser, CTokenType_CloseParen);

                      c_parse_result Body = GetBodyTextForNextScope(Parser);

                      meta_func Func = {
                        .Name = CSz("anonymous_function"),
                        .ArgName = ArgName,
                        .Body = Body,
                      };

                      counted_string Code = Execute(&Func, ArgType, &Datatypes, &FunctionDefs, Memory);

                      RequireToken(Parser, CTokenType_CloseParen);
                      if (Code.Count)
                      {
                        counted_string OutfileName = GenerateOutfileNameFor(Func.Name, ArgType, Memory, GetRandomString(8, Hash(&Code), Memory));
                        DoWorkToOutputThisStuff(Parser, Code, OutfileName, Memory);
                      }
                      else
                      {
                        Warn("Unable to generate code for meta_func %.*s", (u32)Func.Name.Count, Func.Name.Start);
                      }
                    }
                    else
                    {
                      counted_string FuncName = RequireToken(Parser, CTokenType_Identifier).Value;
                      meta_func Func = ParseFunctionDef(Parser, FuncName);
                      Push(&FunctionDefs, Func, Memory);
                    }

                  } break;

                  case named_list:
                  {
                    RequireToken(Parser, CTokenType_OpenParen);

                    tagged_counted_string_stream NameList = {
                      .Tag = RequireToken(Parser, CTokenType_Identifier).Value
                    };

                    RequireToken(Parser, CTokenType_CloseParen);

                    RequireToken(Parser, CTokenType_OpenBrace);
                    while (PeekToken(Parser).Type == CTokenType_Identifier)
                    {
                      counted_string Name = RequireToken(Parser, CTokenType_Identifier).Value;
                      Push(&NameList.Stream, Name, Memory);
                      OptionalToken(Parser, CTokenType_Comma);
                    }

                    RequireToken(Parser, CTokenType_CloseBrace);

                    Push(&NameLists, NameList, Memory);

                  } break;

                  case generate_cursor_deprecated:
                  {
                    RequireToken(Parser, CTokenType_OpenParen);

                    counted_string DatatypeName = RequireToken(Parser, CTokenType_Identifier).Value;
                    counted_string Code = GenerateCursorFor_DEPRECATED(DatatypeName, Memory);
                    counted_string OutfileName = GenerateOutfileNameFor(ToString(Directive), DatatypeName, Memory);
                    RequireToken(Parser, CTokenType_CloseParen);
                    RequireToken(Parser, CTokenType_CloseParen);
                    DoWorkToOutputThisStuff(Parser, Code, OutfileName, Memory);
                  } break;

                  case for_datatypes:
                  {
                    RequireToken(Parser, CTokenType_OpenParen);
                    RequireToken(Parser, CToken(CSz("all")));
                    RequireToken(Parser, CTokenType_CloseParen);

                    counted_string_stream Excludes = {};
                    if (OptionalToken(Parser, CTokenType_Dot))
                    {
                      RequireToken(Parser, CToken(CSz("exclude")));
                      RequireToken(Parser, CTokenType_OpenParen);
                      Excludes = ParseDatatypeList(Parser, &Datatypes, &NameLists, Memory);
                      RequireToken(Parser, CTokenType_CloseParen);
                    }

                    RequireToken(Parser, CToken(ToString(func)));
                    meta_func StructFunc = ParseFunctionDef(Parser, CSz("for_datatypes_struct_callback"));

                    RequireToken(Parser, CToken(ToString(func)));
                    meta_func EnumFunc = ParseFunctionDef(Parser, CSz("for_datatypes_enum_callback"));

                    RequireToken(Parser, CTokenType_CloseParen);

                    string_builder OutputBuilder = {};

                    for (struct_def_iterator Iter = Iterator(&Datatypes.Structs);
                        IsValid(&Iter);
                        Advance(&Iter))
                    {
                      struct_def* Struct = &Iter.At->Element;
                      if (!StreamContains(&Excludes, Struct->Name))
                      {
                        counted_string Code = Execute(&StructFunc, Struct->Name, &Datatypes, &FunctionDefs, Memory);
                        Append(&OutputBuilder, Code);
                      }
                    }

                    for (enum_def_iterator Iter = Iterator(&Datatypes.Enums);
                        IsValid(&Iter);
                        Advance(&Iter))
                    {
                      enum_def* Enum = &Iter.At->Element;
                      if (!StreamContains(&Excludes, Enum->Name))
                      {
                        counted_string Code = Execute(&EnumFunc, Enum->Name, &Datatypes, &FunctionDefs, Memory);
                        Append(&OutputBuilder, Code);
                      }
                    }


                    counted_string Code = Finalize(&OutputBuilder, Memory);
                    counted_string OutfileName = GenerateOutfileNameFor(ToString(Directive), CSz("debug_print"), Memory);

                    DoWorkToOutputThisStuff(Parser, Code, OutfileName, Memory);

                  } break;

                  case d_union:
                  {
                    counted_string DatatypeName = RequireToken(Parser, CTokenType_Identifier).Value;
                    d_union_decl dUnion = ParseDiscriminatedUnion(Parser, &Datatypes, DatatypeName, Memory);
                    if (Parser->Valid)
                    {
                      counted_string OutfileName = GenerateOutfileNameFor(ToString(Directive), DatatypeName, Memory);

                      string_builder CodeBuilder = {};
                      if (!dUnion.CustomEnumType.Count)
                      {
                        counted_string EnumString = GenerateEnumDef(&dUnion, Memory);
                        Append(&CodeBuilder, EnumString);
                      }

                      {
                        counted_string StructString = GenerateStructDef(&dUnion, Memory);
                        Append(&CodeBuilder, StructString);
                      }

                      counted_string Code = Finalize(&CodeBuilder, Memory);

                      RequireToken(Parser, CTokenType_CloseParen);
                      DoWorkToOutputThisStuff(Parser, Code, OutfileName, Memory);

                    }
                    else
                    {
                      Error("Parsing d_union declaration");
                    }
                  } break;

                  InvalidDefaultWhileParsing(Parser, CS("Invalid directive."));
                }
              }
            }
          } break;

          default:
          {
            PopTokenRaw(Parser);
          } break;
        }

        continue;
      }

      if (Parser->Valid)
      {
        TruncateToCurrentSize(&Parser->OutputTokens);
        Output(Parser->OutputTokens, Parser->Filename, Memory);
      }

      continue;
    }

    /* DebugPrint(NameLists); */

    random_series Rng = {.Seed = 123125};
    native_file TempFile = GetTempFile(&Rng, Memory);
    b32 AllWritesSucceeded = TempFile.Handle ? True : False;
    if (AllWritesSucceeded)
    {
      ITERATE_OVER_AS(person, &People)
      {
        person* Person = GET_ELEMENT(personIter);
        AllWritesSucceeded &= WriteToFile(&TempFile, CSz("# "));
        AllWritesSucceeded &= WriteToFile(&TempFile, Person->Name);
        AllWritesSucceeded &= WriteToFile(&TempFile, CSz("\n"));
        ITERATE_OVER(&Person->Tags)
        {
          tag* Tag = GET_ELEMENT(Iter);

          todo_iterator InnerIter = Iterator(&Tag->Todos);
          if (IsValid(&InnerIter))
          {
            AllWritesSucceeded &= WriteToFile(&TempFile, CSz("  ## "));
            AllWritesSucceeded &= WriteToFile(&TempFile, Tag->Tag);
            AllWritesSucceeded &= WriteToFile(&TempFile, CSz("\n"));

            for (;
                IsValid(&InnerIter);
                Advance(&InnerIter))
            {
              todo* Todo = GET_ELEMENT(InnerIter);
              if (Todo->FoundInCodebase)
              {
                AllWritesSucceeded &= WriteToFile(&TempFile, CSz("    - #"));
                AllWritesSucceeded &= WriteToFile(&TempFile, Todo->Id);
                AllWritesSucceeded &= WriteToFile(&TempFile, CSz(" "));
                AllWritesSucceeded &= WriteToFile(&TempFile, Todo->Value);
                AllWritesSucceeded &= WriteToFile(&TempFile, CSz("\n"));
              }
            }

            AllWritesSucceeded &= WriteToFile(&TempFile, CSz("\n"));
          }
        }
      }
    }
    else
    {
      Error("Opening Tempfile");
    }

    if (AllWritesSucceeded)
    {
      Rename(TempFile, CSz("todos.md"));
      CloseFile(&TempFile);
    }

    Log("\n");

    DumpStringStreamToConsole(&SuccessFiles);
    DumpStringStreamToConsole(&FailFiles);

    /* quaternion Thing = {}; */
    /* Print(Thing); */
  }
  else
  {
    Warn("No files passed, exiting.");
  }

  if (ShouldOpenDebugWindow)
  {
    debug_state* DebugState = GetDebugState();

    DebugState->UIType = DebugUIType_CallGraph;
    DebugState->DisplayDebugMenu = True;

    DebugState->MainThreadAdvanceDebugSystem();

    while (Os.ContinueRunning)
    {
      v2 LastMouseP = Plat.MouseP;
      while ( ProcessOsMessages(&Os, &Plat) );
      Plat.MouseDP = LastMouseP - Plat.MouseP;

      DebugState->OpenDebugWindowAndLetUsDoStuff();
      BonsaiSwapBuffers(&Os);

      /* glBindFramebuffer(GL_FRAMEBUFFER, DebugState->GameGeoFBO.ID); */
      /* glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); */

      glBindFramebuffer(GL_FRAMEBUFFER, 0);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
  }

  s32 Result = Success ? SUCCESS_EXIT_CODE : FAILURE_EXIT_CODE ;
  return Result;
}
#endif
