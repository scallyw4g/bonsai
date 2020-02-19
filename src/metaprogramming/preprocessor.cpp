#include <metaprogramming/preprocessor.h>

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

function c_token*
PeekTokenPointer(c_parse_result* Parser, u32 Lookahead = 0)
{
  c_token* Result = {};
  u32 TokenHits = 0;
  u32 LocalLookahead = 0;
  while (Remaining(&Parser->Tokens, LocalLookahead))
  {
    Result = PeekTokenRawPointer(Parser, LocalLookahead);
    if ( Result->Type == CTokenType_Comment ||
         IsWhitespace(Result->Type) )
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

function c_token
PopToken(c_parse_result* Parser)
{
  c_token Result = PopTokenRaw(Parser);
  while (Remaining(&Parser->Tokens))
  {
    if ( Result.Type == CTokenType_Comment ||
        IsWhitespace(Result.Type) )
    {
      Result = PopTokenRaw(Parser);
    }
    else
    {
      break;
    }
  }

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

function b32
IsMetaprogrammingDirective(counted_string Identifier)
{
  b32 Result = False;

  meta(
    for_enum_values( metaprogramming_directive,
      (EnumName, EnumValue) {
        Result |= StringsMatch(ToString(EnumName), Identifier);
      }
    )
  )
#include <metaprogramming/output/for_enum_values_metaprogramming_directive>

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
TokenizeAnsiStream(ansi_stream Code, memory_arena* Memory)
{
  c_parse_result Result = {
    .Filename = Code.Filename
  };

  if (!Code.Start)
  {
    Error("Allocating stream for %.*s", (s32)Code.Filename.Count, Code.Filename.Start);
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

  while(Remaining(&Code))
  {
    c_token T = GetToken(&Code);

    switch (T.Type)
    {
      case CTokenType_FSlash:
      {
        T = GetToken(&Code, 1);
        switch (T.Type)
        {
          case CTokenType_FSlash:
          {
            T.Type = CTokenType_Comment;
            T.Value = CS(ReadUntilTerminatorList(&Code, "\n", Memory));
            --Code.At;
          } break;

          case CTokenType_Star:
          {
            T.Type = CTokenType_Comment;
            T.Value = ReadUntilTerminatorString(&Code, CS("*/"));
            ++T.Value.Count;
            T.Value = Concat(T.Value, CS("*/"), Memory);
            Advance(&Code);
          } break;

          default:
          {
            T.Type = CTokenType_FSlash;
            Advance(&Code);
          } break;
        }
      } break;

      case CTokenType_SingleQuote:
      {
        T.Type = CTokenType_Char;
        T.Value = PopQuotedCharLiteral(&Code);
      } break;

      case CTokenType_DoubleQuote:
      {
        T.Type = CTokenType_String;
        T.Value = PopQuotedString(&Code);
      } break;

      case CTokenType_Unknown:
      {
        T.Type = CTokenType_Identifier;
        T.Value = PopIdentifier(&Code);
      } break;

      default:
      {
        Advance(&Code);
      } break;
    }

    Push(T, &Result.Tokens);

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
TokenizeString(counted_string Code, counted_string Filename, memory_arena* Memory)
{
  c_parse_result Result = TokenizeAnsiStream(AnsiStream(Code, Filename), Memory);
  return Result;
}

function c_parse_result
TokenizeFile(counted_string Filename, memory_arena* Memory)
{
  ansi_stream SourceFileStream = AnsiStreamFromFile(Filename, Memory);
  c_parse_result Result = TokenizeAnsiStream(SourceFileStream, Memory);
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
      Log("Error was : %.*s", ErrorString.Count, ErrorString.Start);
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
  counted_string Result = FormatCountedString(Memory, "enum %.*s_type\n{\n  type_%.*s_noop,\n", dUnion->Name.Count, dUnion->Name.Start, dUnion->Name.Count, dUnion->Name.Start);

  for (d_union_member_iterator Iter = Iterator(&dUnion->Members);
      IsValid(&Iter);
      Advance(&Iter))
  {
    d_union_member* Member = &Iter.At->Element;
    Result = Concat(Result, FormatCountedString(Memory, "  type_%.*s,\n", Member->Type.Count, Member->Type.Start), Memory);
  }

  Result = Concat(Result, CS("};\n\n"), Memory);
  return Result;
}

function counted_string
GenerateStructDef(d_union_decl* dUnion, memory_arena* Memory)
{
  counted_string UnionName = dUnion->Name;
  counted_string Result = FormatCountedString(Memory, "struct %.*s\n{\n  %.*s_type Type;\n\n  union\n  {\n", UnionName.Count, UnionName.Start, UnionName.Count, UnionName.Start);

  for (d_union_member_iterator Iter = Iterator(&dUnion->Members);
      IsValid(&Iter);
      Advance(&Iter))
  {
    d_union_member* Member = &Iter.At->Element;
    if (Member->Flags != d_union_flag_enum_only)
    {
      Result = Concat(Result, FormatCountedString(Memory, "    %.*s %.*s;\n", Member->Type.Count, Member->Type.Start, Member->Name.Count, Member->Name.Start), Memory);
    }
  }

  Result = Concat(Result, CS("  };\n};\n\n"), Memory);

  return Result;
}

d_union_decl
ParseDiscriminatedUnion(c_parse_result* Parser, counted_string Name, memory_arena* Memory)
{
  d_union_decl dUnion = {};

  dUnion.Name = Name;

  RequireToken(Parser, CTokenType_Comma);
  RequireToken(Parser, CTokenType_OpenBrace);

  b32 Complete = False;
  while (!Complete && Remaining(&Parser->Tokens))
  {
    c_token Interior = PopToken(Parser);

    switch (Interior.Type)
    {
      case CTokenType_Identifier:
      {
        d_union_flags Flags = {};
        // TODO(Jesse): How should we talk about this type of identifier?
        if ( OptionalToken(Parser, CToken(CS("enum_only"))) )
        {
          Flags = d_union_flag_enum_only;
        }

        PushMember(&dUnion, Interior, Flags, Memory);

        RequireToken(Parser, CTokenType_Comma);
      } break;

      case CTokenType_CloseBrace:
      {
        Complete = True;
      } break;

      default:
      {
        // TODO(Jesse): This sucks..
        --Parser->Tokens.At;
        Parser->Valid = False;
      } break;
    }
  }

  return dUnion;
}

function enum_def*
GetEnumByType(enum_def_stream* ProgramEnums, counted_string EnumType)
{
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

arguments
ParseArgs(const char** ArgStrings, s32 ArgCount, memory_arena* Memory)
{
  arguments Result = {
    .OutPath = CS("src/metaprogramming/output"),
    .Files = AllocateBuffer<counted_string_cursor, counted_string>((u32)ArgCount, Memory),
  };

  for (s32 ArgIndex = 1;
      ArgIndex < ArgCount;
      ++ArgIndex)
  {
    counted_string Arg = CS(ArgStrings[ArgIndex]);
    if (StringsMatch(CS("-o"), Arg) ||
        StringsMatch(CS("--output-path"), Arg) )
    {
      if (++ArgIndex < ArgCount)
      {
        Result.OutPath = CS(ArgStrings[ArgIndex]);
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
  b32 Result = False;

  native_file TempFile = GetTempFile(&TempFileEntropy, Memory);
  if (TempFile.Handle)
  {
    Rewind(&Code);
    u32 NeededSize = 0;
    while(Remaining(&Code))
    {
      counted_string TS = ToString(Code.At[0], Memory);
      NeededSize += TS.Count;
      Advance(&Code);
    }

    ansi_stream Output = AllocateBuffer<ansi_stream, const char>(NeededSize, Memory);

    Rewind(&Code);
    while(Remaining(&Code))
    {
      c_token T = Code.At[0];
      counted_string TS = ToString(T, Memory);

      u8* OutputAt = (u8*)Output.At;
      // TODO(Jesse): Gross.
      Assert(TS.Count);
      if (Advance(&Output, (u32)TS.Count-1))
      {
        MemCopy((u8*)TS.Start, OutputAt, TS.Count);
      }
      else
      {
        Error("Ran out of memory copying token (%.*s) onto output stream for file (%.*s).", (u32)TS.Count, TS.Start, (u32)Filename.Count, Filename.Start);
        return Result;
      }

      Assert((umm)Output.At <= (umm)Output.End);
      Advance(&Code);
    }
    Assert((umm)Output.At == (umm)Output.End);

    b32 FileWritesSucceeded = WriteToFile(&TempFile, Output);
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

void
Push(c_decl_stream* Stream, c_decl Element, memory_arena* Memory)
{
  c_decl_stream_chunk* NextChunk = Allocate(c_decl_stream_chunk, Memory, 1);
  NextChunk->Element = Element;

  if (!Stream->FirstChunk)
  {
    Stream->FirstChunk = NextChunk;
    Stream->LastChunk = NextChunk;
  }
  else
  {
    Stream->LastChunk->Next = NextChunk;
    Stream->LastChunk = NextChunk;
  }

  Assert(NextChunk->Next == 0);
  Assert(Stream->LastChunk->Next == 0);

  return;
}

function c_decl_iterator
CDIterator(c_decl_stream* Stream)
{
  c_decl_iterator Iterator = {
    .At = Stream->FirstChunk
  };
  return Iterator;
}

function b32
IsValid(c_decl_iterator* Iter)
{
  b32 Result = (Iter->At != 0);
  return Result;
}

function void
Advance(c_decl_iterator* Iter)
{
  Iter->At = Iter->At->Next;
}

function void
DumpCDeclStreamToConsole(c_decl_stream* Stream)
{
  for (c_decl_iterator Iter = CDIterator(Stream);
      IsValid(&Iter);
      Advance(&Iter))
  {
    switch(Iter.At->Element.Type)
    {
      case type_c_decl_function:
      {
        switch(Iter.At->Element.c_decl_function.Type)
        {
          case CFunctionType_Normal:
          {
            Log("  Function\n");
          } break;
          case CFunctionType_Constructor:
          {
            Log("  Constructor\n");
          } break;
          case CFunctionType_Destructor:
          {
            Log("  Destructor\n");
          } break;
        }
      } break;

      case type_c_decl_variable:
      {
        counted_string Type = Iter.At->Element.c_decl_variable.Type;
        counted_string Name = Iter.At->Element.c_decl_variable.Name;
        Log("  %.*s %.*s\n", Type.Count, Type.Start, Name.Count, Name.Start);
      } break;

      InvalidDefaultCase;
    }
  }
}

function struct_def
StructDef(counted_string Name)
{
  struct_def Result = {};
  Result.Name = Name;
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

#define InvalidDefaultWhileParsing(Parser, ErrorMessage) \
    default: { OutputParsingError(Parser, Parser->Tokens.At, ErrorMessage); Assert(False); } break;

function b32
IsCxxDefinitionKeyword(counted_string Value)
{
  b32 Result = StringsMatch(Value, CS("volatile")) ||
               StringsMatch(Value, CS("struct")) ||
               StringsMatch(Value, CS("const"));
  return Result;
}

function struct_def ParseStructBody(c_parse_result* Parser, counted_string StructName, memory_arena* Memory);

function c_decl
ParseDeclaration(c_parse_result* Parser, counted_string StructName, memory_arena* Memory)
{
  c_decl Result = {
    .Type = type_c_decl_variable
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
      Result.Type = type_c_decl_function;
      Result.c_decl_function.Type = CFunctionType_Destructor;
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
            Result.Type = type_c_decl_function;
            Result.c_decl_function.Type = CFunctionType_Constructor;
            Done = True;
          } break;

          case CTokenType_Ampersand:
          {
            RequireToken(Parser, CTokenType_Ampersand);
            Result.c_decl_variable.Type = Concat(Result.c_decl_variable.Type, CS("&"), Memory);
          } break;

          case CTokenType_Star:
          {
            RequireToken(Parser, CTokenType_Star);
            Result.c_decl_variable.Type = Concat(Result.c_decl_variable.Type, CS("*"), Memory);
          } break;

          case CTokenType_Identifier:
          {
            if (StringsMatch(NextToken.Value, CS("unsigned")))
            {
              Result.c_decl_variable.Type = Concat( Result.c_decl_variable.Type, RequireToken(Parser, CTokenType_Identifier).Value, Memory);
            }
            else if (StringsMatch(NextToken.Value, CS("union")))
            {
              RequireToken(Parser, CToken(CS("union")));

              Result.Type = type_c_decl_union;
              Result.c_decl_union.Body = ParseStructBody(Parser, CS("anonymous union"), Memory);
              Done = True;
              Unnamed = True;
            }
            else if (StringsMatch(NextToken.Value, CS("operator")))
            {
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
                Result.Type = type_c_decl_function;
                Result.c_decl_function.Type = CFunctionType_Normal;
              }

              if (DefKeywordsEncountered++ == 1)
              {
                Done = True;
              }
            }

            if (!Done)
            {
              Result.c_decl_variable.Type = Concat( Result.c_decl_variable.Type, RequireToken(Parser, CTokenType_Identifier).Value, Memory);
            }
          } break;

          InvalidDefaultWhileParsing(Parser, CS("While parsing decl type 0."));
        }

        continue;
      }

    } break;

    InvalidDefaultWhileParsing(Parser, CS("While parsing decl type 1."));
  }

  if (!Unnamed && Result.Type == type_c_decl_variable)
  {
    Result.c_decl_variable.Name = RequireToken(Parser, CTokenType_Identifier).Value;
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

function void
DumpStruct(struct_def* Struct)
{
  Log("%.*s\n", Struct->Name.Count, Struct->Name.Start);
  DumpCDeclStreamToConsole(&Struct->Fields);
}

function void
PrintCDecl(c_decl* Decl, struct_def_stream* ProgramStructs)
{
  switch (Decl->Type)
  {
    case type_c_decl_variable:
    {
      Log("%.*s %.*s",
          Decl->c_decl_variable.Type.Count, Decl->c_decl_variable.Type.Start,
          Decl->c_decl_variable.Name.Count, Decl->c_decl_variable.Name.Start);

      Log("\n");
    } break;

    case type_c_decl_union:
    {
      for (c_decl_iterator Iter = CDIterator(&Decl->c_decl_union.Body.Fields);
          IsValid(&Iter);
          Advance(&Iter))
      {
        if (Iter.At->Element.Type == type_c_decl_variable)
        {
          struct_def* Struct = GetStructByType(ProgramStructs, Iter.At->Element.c_decl_variable.Type);
          if (Struct)
          {
            PrintCDecl(&Iter.At->Element, ProgramStructs);
            DumpStruct(Struct);
          }
          else
          {
            Error("Couldn't find struct type %*.s", (u32)Iter.At->Element.c_decl_variable.Name.Count, Iter.At->Element.c_decl_variable.Name.Start);
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

struct for_enum_constraints
{
  // Replacement Patterns
  counted_string TypeName;
  counted_string ValueName;
};

struct for_member_constraints
{
  counted_string MemberContains;

  // Replacement Patterns
  counted_string TypeTag;
  counted_string TypeName;
  counted_string ValueName;
};

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
    for (c_decl_iterator Iter = CDIterator(&Struct->Fields);
        IsValid(&Iter) && !Result;
        Advance(&Iter))
    {
      switch (Iter.At->Element.Type)
      {
        case type_c_decl_variable:
        {
          if (StringsMatch(Iter.At->Element.c_decl_variable.Type, MemberType))
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

function counted_string
ParseForEnumValues(c_parse_result* Parser, counted_string TypeName, enum_def_stream* ProgramEnums, memory_arena* Memory)
{
  counted_string Result = {};

  for_enum_constraints Constraints = {};

  enum_def* Target = GetEnumByType(ProgramEnums, TypeName);

  if (Target)
  {
    RequireToken(Parser, CTokenType_Comma);

    if (OptionalToken(Parser, CToken(CTokenType_OpenParen)))
    {
      Constraints.TypeName = RequireToken(Parser, CTokenType_Identifier).Value;

      RequireToken(Parser, CTokenType_Comma);
      Constraints.ValueName = RequireToken(Parser, CTokenType_Identifier).Value;

      RequireToken(Parser, CTokenType_CloseParen);
    }

    Assert(Constraints.TypeName.Count);
    Assert(Constraints.ValueName.Count);

    c_parse_result BodyText = GetBodyTextForNextScope(Parser);

    for (enum_field_iterator Iter = Iterator(&Target->Fields);
        IsValid(&Iter);
        Advance(&Iter))
    {
      enum_field Enum = Iter.At->Element;
      Rewind(&BodyText.Tokens);
      while (Remaining(&BodyText.Tokens))
      {
        c_token T = PopTokenRaw(&BodyText);
        if (StringsMatch(T.Value, Constraints.TypeName))
        {
          Result = Concat(Result, CS(FormatString(Memory, "%.*s", Enum.Name.Count, Enum.Name.Start)), Memory);
        }
        else if (StringsMatch(T.Value, Constraints.ValueName))
        {
          Result = Concat(Result, CS(FormatString(Memory, "%.*s", Enum.Value.Count, Enum.Value.Start)), Memory);
        }
        else
        {
          Result = Concat(Result, ToString(T, Memory), Memory);
        }
      }
    }

  }

  return Result;
}

function counted_string
ParseForMembers(c_parse_result* Parser, for_member_constraints* Constraints, struct_def* Target, struct_def_stream* ProgramStructs, memory_arena* Memory)
{
  counted_string Result = {};

  RequireToken(Parser, CTokenType_Comma);

  if (OptionalToken(Parser, CToken(CS("where_member_contains"))))
  {
    Constraints->MemberContains = RequireToken(Parser, CTokenType_Identifier).Value;
  }

  if (OptionalToken(Parser, CToken(CTokenType_Comma)))
  {
    RequireToken(Parser, CTokenType_OpenParen);
    Constraints->TypeTag = RequireToken(Parser, CTokenType_Identifier).Value;

    RequireToken(Parser, CTokenType_Comma);
    Constraints->TypeName = RequireToken(Parser, CTokenType_Identifier).Value;

    RequireToken(Parser, CTokenType_Comma);
    Constraints->ValueName = RequireToken(Parser, CTokenType_Identifier).Value;

    RequireToken(Parser, CTokenType_CloseParen);
  }

  Assert(Constraints->TypeTag.Count);
  Assert(Constraints->TypeName.Count);
  Assert(Constraints->ValueName.Count);

  c_parse_result BodyText = GetBodyTextForNextScope(Parser);

  c_decl_stream_chunk* AtChunk = Target->Fields.FirstChunk;
  while (AtChunk)
  {
    switch (AtChunk->Element.Type)
    {
      case type_c_decl_variable:
      {
        // TODO(Jesse): Do we actually do anything here?
      } break;

      case type_c_decl_union:
      {
        for (c_decl_iterator Iter = CDIterator(&AtChunk->Element.c_decl_union.Body.Fields);
            IsValid(&Iter);
            Advance(&Iter))
        {
          if (Iter.At->Element.Type == type_c_decl_variable)
          {
            struct_def* Struct = GetStructByType(ProgramStructs, Iter.At->Element.c_decl_variable.Type);
            if (Struct)
            {
              if (HasMemberOfType(Struct, Constraints->MemberContains))
              {
                Rewind(&BodyText.Tokens);
                while (Remaining(&BodyText.Tokens))
                {
                  c_token T = PopTokenRaw(&BodyText);
                  if (StringsMatch(T.Value, Constraints->TypeTag))
                  {
                    Result = Concat(Result, CS(FormatString(Memory, "type_%.*s", Iter.At->Element.c_decl_variable.Type.Count, Iter.At->Element.c_decl_variable.Type.Start)), Memory);
                  }
                  else if (StringsMatch(T.Value, Constraints->TypeName))
                  {
                    Result = Concat(Result, CS(FormatString(Memory, "%.*s", Iter.At->Element.c_decl_variable.Type.Count, Iter.At->Element.c_decl_variable.Type.Start)), Memory);
                  }
                  else if (StringsMatch(T.Value, Constraints->ValueName))
                  {
                    Result = Concat(Result, CS(FormatString(Memory, "%.*s", Iter.At->Element.c_decl_variable.Name.Count, Iter.At->Element.c_decl_variable.Name.Start)), Memory);
                  }
                  else
                  {
                    Result = Concat(Result, ToString(T, Memory), Memory);
                  }
                }
              }
            }
            else
            {
              Error("Couldn't find struct type %*.s", (u32)Iter.At->Element.c_decl_variable.Name.Count, Iter.At->Element.c_decl_variable.Name.Start);
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

    AtChunk = AtChunk->Next;
  }


  return Result;
}

function struct_def
ParseStructBody(c_parse_result* Parser, counted_string StructName, memory_arena* Memory)
{
  struct_def Result = StructDef(StructName);

  RequireToken(Parser, CTokenType_OpenBrace);

  c_token NextToken = PeekToken(Parser);

  while (Remaining(&Parser->Tokens) && NextToken.Type != CTokenType_CloseBrace)
  {
    if (NextToken.Type == CTokenType_Hash)
    {
      EatUntil(Parser, CTokenType_Newline);
      continue;
    }

    c_decl Declaration = ParseDeclaration(Parser, Result.Name, Memory);
    Push(&Result.Fields, Declaration, Memory);
    NextToken = PeekToken(Parser);
  }

  return Result;
}

function enum_def
ParseEnum(c_parse_result* Parser, memory_arena* Memory)
{
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
      Field.Value = PopToken(Parser).Value;
    }

    if(OptionalToken(Parser, CTokenType_Comma))
    {
      NextToken = PeekToken(Parser);
    }
    else
    {
      break;
    }

    Push(&Enum.Fields, Field, Memory);
  }

  return Enum;
}

function program_datatypes
ParseAllDatatypes(c_parse_result_cursor Files_in, memory_arena* Memory)
{
  c_parse_result_cursor* Files = &Files_in;

  enum_def_stream EnumStream = {};
  struct_def_stream StructStream = {};

  for (u32 ParserIndex = 0;
      ParserIndex < (u32)Count(Files);
      ++ParserIndex)
  {
    c_parse_result* Parser = Files->Start+ParserIndex;
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
            if (PeekToken(Parser) == CToken(CS("struct")))
            {
              RequireToken(Parser, CToken((CS("struct"))));
              if (PeekToken(Parser) == CToken(CTokenType_OpenBrace))
              {
                struct_def S = ParseStructBody(Parser, CS(""), Memory);
                Push(&StructStream, S, Memory);

                RequireToken(Parser, CTokenType_CloseBrace);
                S.Name = RequireToken(Parser, CTokenType_Identifier).Value;
                RequireToken(Parser, CTokenType_Semicolon);
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
            Push(&EnumStream, Enum, Memory);
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
                  Push(&StructStream, S, Memory);
                }
              } break;

              default: {} break;
            }
          }
        } break;

        default: {} break;
      }

    }

    Rewind(&Parser->Tokens);
  }

  program_datatypes Result = {
    .Enums = EnumStream,
    .Structs = StructStream
  };

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
GenerateValueTableFor(enum_def* Enum, memory_arena* Memory)
{
  counted_string FunctionName = ToCapitalCase(Enum->Name, Memory);
  counted_string Result = FormatCountedString(Memory,
R"INLINE_CODE(
function %.*s
%.*s(counted_string S)
{
  %.*s Result = {};
)INLINE_CODE",
              Enum->Name.Count, Enum->Name.Start,
              FunctionName.Count, FunctionName.Start,
              Enum->Name.Count, Enum->Name.Start);

  for (enum_field_iterator Iter = Iterator(&Enum->Fields);
      IsValid(&Iter);
      Advance(&Iter))
  {
    Result = Concat(Result,
        CS(FormatString(Memory,
            "  if (StringsMatch(CS(\"%.*s\"), S)) { Result = %.*s; }\n",
            Iter.At->Element.Name.Count, Iter.At->Element.Name.Start,
            Iter.At->Element.Name.Count, Iter.At->Element.Name.Start
            )), Memory);
  }

  Result = Concat(Result, CS("  return Result;"), Memory);
  Result = Concat(Result, CS("\n}\n\n"), Memory);

  return Result;
}

function counted_string
GenerateCursorFor(counted_string StructName, memory_arena* Memory)
{
  counted_string Result = FormatCountedString(Memory,
R"INLINE_CODE(
struct %.*s_cursor
{
  %.*s* Start;
  %.*s* End;
  %.*s* At;
};
)INLINE_CODE",
    StructName.Count, StructName.Start,
    StructName.Count, StructName.Start,
    StructName.Count, StructName.Start,
    StructName.Count, StructName.Start
    );

  return Result;
}

function counted_string
GenerateStringTableFor(enum_def* Enum, memory_arena* Memory)
{
  counted_string Result = FormatCountedString(Memory,
R"INLINE_CODE(
function counted_string
ToString(%.*s Type)
{
  counted_string Result = {};
  switch (Type)
  {
)INLINE_CODE", Enum->Name.Count, Enum->Name.Start);

  for (enum_field_iterator Iter = Iterator(&Enum->Fields);
      IsValid(&Iter);
      Advance(&Iter))
  {
    Result = Concat(Result,
        CS(FormatString(Memory,
            "    case %.*s: { Result = CS(\"%.*s\"); } break;\n",
            Iter.At->Element.Name.Count, Iter.At->Element.Name.Start,
            Iter.At->Element.Name.Count, Iter.At->Element.Name.Start
            )), Memory);
  }

  Result = Concat(Result, CS("  }\n  return Result;\n}\n\n"), Memory);

  return Result;
}

function counted_string
GenerateStreamFor(counted_string StructName, memory_arena* Memory)
{
  counted_string StreamCode = FormatCountedString(Memory,
R"INLINE_CODE(
struct %.*s_stream_chunk
{
  %.*s Element;
  %.*s_stream_chunk* Next;
};

struct %.*s_stream
{
  %.*s_stream_chunk* FirstChunk;
  %.*s_stream_chunk* LastChunk;
};

)INLINE_CODE", StructName.Count, StructName.Start,
   StructName.Count, StructName.Start,
   StructName.Count, StructName.Start,
   StructName.Count, StructName.Start,
   StructName.Count, StructName.Start,
   StructName.Count, StructName.Start,
   StructName.Count, StructName.Start);

  counted_string PushCode = FormatCountedString(Memory,
R"INLINE_CODE(
function void
Push(%.*s_stream* Stream, %.*s Element, memory_arena* Memory)
{
  // TODO(Jesse): Can we use Allocate() here instead?
  %.*s_stream_chunk* NextChunk = (%.*s_stream_chunk*)PushStruct(Memory, sizeof(%.*s_stream_chunk), 1, 1);
  NextChunk->Element = Element;

  if (!Stream->FirstChunk)
  {
    Assert(!Stream->LastChunk);
    Stream->FirstChunk = NextChunk;
    Stream->LastChunk = NextChunk;
  }
  else
  {
    Stream->LastChunk->Next = NextChunk;
    Stream->LastChunk = NextChunk;
  }

  Assert(NextChunk->Next == 0);
  Assert(Stream->LastChunk->Next == 0);

  return;
}

)INLINE_CODE",
      StructName.Count, StructName.Start,
      StructName.Count, StructName.Start,
      StructName.Count, StructName.Start,
      StructName.Count, StructName.Start,
      StructName.Count, StructName.Start);

  counted_string IteratorCode = FormatCountedString(Memory,
R"INLINE_CODE(
struct %.*s_iterator
{
  %.*s_stream* Stream;
  %.*s_stream_chunk* At;
};

function %.*s_iterator
Iterator(%.*s_stream* Stream)
{
  %.*s_iterator Iterator = {
    .Stream = Stream,
    .At = Stream->FirstChunk
  };
  return Iterator;
}

function b32
IsValid(%.*s_iterator* Iter)
{
  b32 Result = Iter->At != 0;
  return Result;
}

function void
Advance(%.*s_iterator* Iter)
{
  Iter->At = Iter->At->Next;
}

)INLINE_CODE",
      StructName.Count, StructName.Start,
      StructName.Count, StructName.Start,
      StructName.Count, StructName.Start,
      StructName.Count, StructName.Start,
      StructName.Count, StructName.Start,
      StructName.Count, StructName.Start,
      StructName.Count, StructName.Start,
      StructName.Count, StructName.Start);

  counted_string Result = StreamCode;
  Result = Concat(Result, PushCode, Memory);
  Result = Concat(Result, IteratorCode, Memory);

  return Result;
}

function metaprogramming_directive
GetMetaprogrammingDirective(c_parse_result* Parser)
{
  metaprogramming_directive Result = MetaprogrammingDirective(RequireToken(Parser, CTokenType_Identifier).Value);
  if (!Result)
    { OutputParsingError(Parser, Parser->Tokens.At, CS("Expected metaprogramming directive.")); }
  return Result;
}

function void
DoWorkToOutputThisStuff(c_parse_result* Parser, counted_string OutputForThisParser, counted_string NewFilename, memory_arena* Memory)
{
  RequireToken(Parser, CTokenType_CloseParen);
  RequireToken(Parser, CTokenType_CloseParen);

  if (!Parser->Valid)
  {
    Error("Bad parser state");
    return;
  }

  if (PeekToken(Parser).Type == CTokenType_Hash &&
      PeekToken(Parser, 1) == CToken(CS("include")))
  {
    RequireToken(Parser, CToken(CTokenType_Hash));
    RequireToken(Parser, CToken(CS("include")));
    RequireToken(Parser, CTokenType_LT);
    RequireToken(Parser, CToken(CS("metaprogramming")));
    RequireToken(Parser, CTokenType_FSlash);
    RequireToken(Parser, CToken(CS("output")));
    RequireToken(Parser, CTokenType_FSlash);
    counted_string IncludePath = RequireToken(Parser, CTokenType_Identifier).Value;

    if (OptionalToken(Parser, CTokenType_Dot))
    {
      IncludePath = Concat(IncludePath, CS("."), Memory);
      counted_string Extension = RequireToken(Parser, CTokenType_Identifier).Value;
      IncludePath = Concat(IncludePath, Extension,  Memory);
    }

    RequireToken(Parser, CTokenType_GT);

    IncludePath = Concat(CS("src/metaprogramming/output/"), IncludePath, Memory);
    Output(OutputForThisParser, IncludePath, Memory);
  }
  else
  {
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

#ifndef EXCLUDE_PREPROCESSOR_MAIN
s32
main(s32 ArgCount, const char** ArgStrings)
{
  setbuf(stdout, NULL);
  setbuf(stderr, NULL);

  b32 Success = True;

  if (ArgCount > 1)
  {
    memory_arena Memory_ = {};
    memory_arena* Memory = &Memory_;

    counted_string_stream SuccessFiles = {};
    counted_string_stream FailFiles = {};

    arguments Args = ParseArgs(ArgStrings, ArgCount, Memory);
    Assert(Args.Files.Start == Args.Files.At);

    c_parse_result_cursor ParsedFiles = TokenizeAllFiles(&Args.Files, Memory);
    Assert(ParsedFiles.Start == ParsedFiles.At);

    program_datatypes Datatypes = ParseAllDatatypes(ParsedFiles, Memory);
    Assert(ParsedFiles.Start == ParsedFiles.At);

    for (u32 ParserIndex = 0;
        ParserIndex < Count(&ParsedFiles);
        ++ParserIndex)
    {
      c_parse_result* Parser = ParsedFiles.Start+ParserIndex;
      Assert(Parser->Valid);
      Assert(Remaining(&Parser->Tokens));

      Rewind(&Parser->OutputTokens);
      Parser->LineNumber = 1;
      while (Parser->Valid && Remaining(&Parser->Tokens))
      {
        c_token NextToken = PeekToken(Parser);

        switch( NextToken.Type )
        {
          case CTokenType_Identifier:
          {
            if (OptionalToken(Parser, CToken(CS("meta"))))
            {
              RequireToken(Parser, CTokenType_OpenParen);
              metaprogramming_directive Directive = GetMetaprogrammingDirective(Parser);
              RequireToken(Parser, CTokenType_OpenParen);
              counted_string DatatypeName = RequireToken(Parser, CTokenType_Identifier).Value;

              counted_string OutfileName = Concat(
                  Concat( ToString(Directive),
                          CS("_"), Memory),
                          DatatypeName, Memory);
              switch (Directive)
              {
                case generate_stream:
                {
                  counted_string Code = GenerateStreamFor(DatatypeName, Memory);
                  DoWorkToOutputThisStuff(Parser, Code, OutfileName, Memory);
                } break;

                case generate_cursor:
                {
                  counted_string Code = GenerateCursorFor(DatatypeName, Memory);
                  DoWorkToOutputThisStuff(Parser, Code, OutfileName, Memory);
                } break;

                case generate_string_table:
                {
                  enum_def* Enum = GetEnumByType(&Datatypes.Enums, DatatypeName);
                  counted_string Code = GenerateStringTableFor(Enum, Memory);
                  DoWorkToOutputThisStuff(Parser, Code, OutfileName, Memory);
                } break;

                case generate_value_table:
                {
                  enum_def* Enum = GetEnumByType(&Datatypes.Enums, DatatypeName);
                  counted_string Code = GenerateValueTableFor(Enum, Memory);
                  DoWorkToOutputThisStuff(Parser, Code, OutfileName, Memory);
                } break;


                case for_enum_values:
                {
                  counted_string Code = ParseForEnumValues(Parser, DatatypeName, &Datatypes.Enums, Memory);
                  DoWorkToOutputThisStuff(Parser, Code, OutfileName, Memory);
                } break;

                case for_members_in:
                {
                  for_member_constraints Constraints = {};
                  struct_def* Target = GetStructByType(&Datatypes.Structs, DatatypeName);

                  if (Target)
                  {
                    counted_string Code = ParseForMembers(Parser, &Constraints, Target, &Datatypes.Structs, Memory);
                    DoWorkToOutputThisStuff(Parser, Code, OutfileName, Memory);
                  }
                  else
                  {
                    Parser->Valid = False;
                    Error("Couldn't find matching struct %.*s", (s32)DatatypeName.Count, DatatypeName.Start);
                  }
                } break;

                case d_union:
                {
                  d_union_decl dUnion = ParseDiscriminatedUnion(Parser, DatatypeName, Memory);
                  if (Parser->Valid)
                  {
                    counted_string EnumString = GenerateEnumDef(&dUnion, Memory);
                    counted_string StructString = GenerateStructDef(&dUnion, Memory);
                    counted_string Code = Concat(EnumString, StructString, Memory);
                    DoWorkToOutputThisStuff(Parser, Code, OutfileName, Memory);

                    c_parse_result StructParse = TokenizeString(StructString, OutfileName, Memory);
                    RequireToken(&StructParse, CToken(CS("struct")));
                    counted_string StructName = RequireToken(&StructParse, CTokenType_Identifier).Value;
                    struct_def S = ParseStructBody(&StructParse, StructName, Memory);
                    Push(&Datatypes.Structs, S, Memory);

                    c_parse_result EnumParse = TokenizeString(EnumString, OutfileName, Memory);
                    RequireToken(&EnumParse, CToken(CS("enum")));
                    enum_def E = ParseEnum(&EnumParse, Memory);
                    Push(&Datatypes.Enums, E, Memory);

                    Assert( GetStructByType(&Datatypes.Structs, StructName) );
                    Assert( GetEnumByType(&Datatypes.Enums, E.Name) );
                  }
                  else
                  {
                    Error("Parsing d_union declaration");
                  }
                } break;

                InvalidDefaultWhileParsing(Parser, CS("Invalid directive."));
              }
            }
            else
            {
              PopToken(Parser);
            }
          } break;

          default:
          {
            PopToken(Parser);
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

    Log("\n");

    DumpStringStreamToConsole(&SuccessFiles);
    DumpStringStreamToConsole(&FailFiles);

  }
  else
  {
    Warn("No files passed, exiting.");
  }

#define SUCCESS_EXIT_CODE 0
#define FAILURE_EXIT_CODE 1
  s32 Result = Success ? SUCCESS_EXIT_CODE : FAILURE_EXIT_CODE ;
  return Result;
}
#endif
