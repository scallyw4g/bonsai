#include <metaprogramming/preprocessor.h>

c_token
GetToken(ansi_stream* Stream, u32 Lookahead = 0)
{
  c_token Result = {};

  if (Stream->At+Lookahead < Stream->End)
  {
    char At = *(Stream->At+Lookahead);
    switch (At)
    {
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
      case CTokenType_Space:
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
      case CTokenType_Newline:
      case CTokenType_CarrigeReturn:
      case CTokenType_EOF:
      {
        Result = { .Type = (c_token_type)At };
      } break;
    }
  }
  else
  {
    Warn("Attempted to get token past end of stream on file : %.*s", (u32)Stream->Filename.Count, Stream->Filename.Start);
  }

  return Result;
}

function b32
IsWhitespace(c_token_type Type)
{
  b32 Result = Type == CTokenType_Newline       ||
               Type == CTokenType_CarrigeReturn ||
               Type == CTokenType_Space;
  return Result;
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
    Warn("Attempted to advance parser past end of stream on file : %.*s", (u32)Parser->FileName.Count, Parser->FileName.Start);
  }

  return;
}

function c_token
PeekTokenRaw(c_parse_result* Parser, u32 Lookahead = 0)
{
  c_token Result = {};
  if (Remaining(&Parser->Tokens, Lookahead))
  {
    Result = *(Parser->Tokens.At+Lookahead);
  }
  else
  {
    Warn("Tried to peek a token on an empty stream");
  }

  return Result;
}

function c_token
PeekToken(c_parse_result* Parser, u32 Lookahead = 0)
{
  c_token Result = {};
  u32 TokenHits = 0;
  u32 LocalLookahead = 0;
  while (Remaining(&Parser->Tokens, LocalLookahead))
  {
    Result = PeekTokenRaw(Parser, LocalLookahead);
    if ( Result.Type == CTokenType_Comment || IsWhitespace(Result.Type) )
    {
      // TODO(Jesse): Does this make any sense at all?
      Result.Type = CTokenType_Unknown;
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
PopTokenRaw(c_parse_result* Parser)
{
  c_token Result = PeekTokenRaw(Parser);
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
    if ( Result.Type == CTokenType_Comment || IsWhitespace(Result.Type) )
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

function c_parse_result
TokenizeFile(counted_string FileName, memory_arena* Memory)
{
  c_parse_result Result = {
    .FileName = FileName
  };

  // TODO(Jesse): Since we store pointers directly into this buffer, we need to
  // keep the memory around.  Should we tokenize such that we allocate new
  // memory for things that need it?  (Strings/Identifiers/Comments atm..)
  ansi_stream SourceFileStream = AnsiStreamFromFile(FileName, Memory);
  if (!SourceFileStream.Start)
  {
    Error("Allocating stream for %.*s", (s32)FileName.Count, FileName.Start);
    return Result;
  }

  Result.Tokens = AllocateTokenBuffer(Memory, (u32)Megabytes(100));
  if (!Result.Tokens.Start)
  {
    Error("Allocating Token Buffer");
    return Result;
  }

  while(Remaining(&SourceFileStream))
  {
    c_token T = GetToken(&SourceFileStream);

    switch (T.Type)
    {
      case CTokenType_FSlash:
      {
        T = GetToken(&SourceFileStream, 1);
        switch (T.Type)
        {
          case CTokenType_FSlash:
          {
            T.Type = CTokenType_Comment;
            T.Value = CS(ReadUntilTerminatorList(&SourceFileStream, "\n", Memory));
            --SourceFileStream.At;
          } break;

          case CTokenType_Star:
          {
            T.Type = CTokenType_Comment;
            T.Value = ReadUntilTerminatorString(&SourceFileStream, CS("*/"));
          } break;

          default:
          {
            ++SourceFileStream.At;
          } break;
        }
      } break;

      case CTokenType_SingleQuote:
      {
        T.Type = CTokenType_Char;
        T.Value = PopQuotedCharLiteral(&SourceFileStream);
      } break;

      case CTokenType_DoubleQuote:
      {
        T.Type = CTokenType_String;
        T.Value = PopQuotedString(&SourceFileStream);
      } break;

      case CTokenType_Unknown:
      {
        counted_string Value = {
          .Start = SourceFileStream.At,
        };

        while (Remaining(&SourceFileStream))
        {
          T = GetToken(&SourceFileStream);
          if (T.Type == CTokenType_Unknown)
          {
            SourceFileStream.At++;
          }
          else
          {
            break;
          }
        }

        Value.Count = (umm)SourceFileStream.At - (umm)Value.Start;

        T.Type = CTokenType_Identifier;
        T.Value = Value;

        if (StringsMatch(Value, CS("struct")))
        {
          ++Result.StructCount;
        }

      } break;

      default:
      {
        SourceFileStream.At++;
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
  Rewind(&Parser->Tokens);
  while(Remaining(&Parser->Tokens) && LinesToDump > 0)
  {
    c_token T = PopTokenRaw(Parser);
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
      Log("^----> Unexpected token type: %s", ToString(ErrorToken->Type));

      if (ErrorToken->Value.Count)
      {
        Log("(%.*s)" , ErrorToken->Value.Count, ErrorToken->Value.Start);
      }

      Log(". Expected: %s", ToString(Expected.Type));

      Log("\n");

      for (u32 ColumnIndex = 0;
          ColumnIndex < ColumnCount;
          ++ColumnIndex)
      {
        Log(" ");
      }

      Log("      %.*s:%u:%u: %.*s\n", Parser->FileName.Count, Parser->FileName.Start, LineNumber, ColumnCount, ErrorString.Count, ErrorString.Start);

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
OutputParsingError(c_parse_result* Parser, c_token* ErrorToken, c_token_type ExpectedType, counted_string ErrorString)
{
  u32 LinesOfContext = 4;

  Log("----\n");

  /* u32 ColumnNumber = 0; */
  /* Log("%.*s:%u:%u: Error\n", Parser->FileName.Count, Parser->FileName.Start, Parser->LineNumber, ColumnNumber); */

  c_parse_result LeadingLines  = *Parser;
  c_parse_result ErrorLine     = *Parser;
  c_parse_result TrailingLines = *Parser;

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
    Assert(ErrorToken >= ErrorLine.Tokens.Start);
    Assert(ErrorToken < ErrorLine.Tokens.End);
    OutputErrorHelperLine(&ErrorLine, ErrorToken, CToken(ExpectedType), ErrorString, Parser->LineNumber);
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
  OutputParsingError(Parser, ErrorToken, CTokenType_Unknown, ErrorString);
  return;
}

function c_token
RequireToken(c_parse_result* Parser, c_token ExpectedToken)
{
  c_token Result = PeekToken(Parser);
  c_token* ErrorToken = Parser->Tokens.At;

  if (Result.Type != ExpectedToken.Type || (ExpectedToken.Value.Count > 0 && !StringsMatch(ExpectedToken.Value, Result.Value) ))
  {
    OutputParsingError(Parser, ErrorToken, ExpectedToken.Type, CS("Require Token Failed"));
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

counted_string
PopString(string_stream* Stream)
{
  counted_string Result = Stream->Sentinel.Next->String;

  // FIXME(Jesse): We're leaking string_chunks here.. freelist anyone?
  // @memory-leak
  Stream->Sentinel.Next->Next->Prev = &Stream->Sentinel;
  Stream->Sentinel.Next = Stream->Sentinel.Next->Next;

  return Result;
}

d_union_member*
DUnionMember(counted_string Name, counted_string Type, d_union_flags Flags, memory_arena* Memory)
{
  d_union_member* Result = Allocate(d_union_member, Memory, 1);
  Result->Name = Name;
  Result->Type = Type;
  Result->Flags = Flags;
  return Result;
}

void
PushMember(d_union_decl* dUnion, c_token MemberIdentifierToken, d_union_flags Flags, memory_arena* Memory)
{
  Assert(MemberIdentifierToken.Type == CTokenType_Identifier);
  d_union_member* Member = DUnionMember(MemberIdentifierToken.Value, MemberIdentifierToken.Value, Flags, Memory);
  DList_Push(dUnion, Member);
}

template <typename cursor_t, typename element_t> inline cursor_t
Cursor(u32 Count, memory_arena* Memory)
{
  element_t* Start = Allocate(element_t, Memory, Count);
  cursor_t Cursor = {
    .Start = Start,
    .At = Start,
    .End = Start + Count
  };
  return Cursor;
}

function counted_string
GenerateEnumDef(d_union_decl* dUnion, memory_arena* Memory)
{
  counted_string Result = FormatCountedString(Memory, "enum %.*s_type\n{\n  type_%.*s_noop,\n", dUnion->Name.Count, dUnion->Name.Start, dUnion->Name.Count, dUnion->Name.Start);

  d_union_member* Member = dUnion->Sentinel.Next;
  while (Member != &dUnion->Sentinel)
  {
    Result = Concat(Result, FormatCountedString(Memory, "  type_%.*s,\n", Member->Type.Count, Member->Type.Start), Memory);
    Member = Member->Next;
  }

  Result = Concat(Result, CS("};\n\n"), Memory);
  return Result;
}

function counted_string
GenerateStructDef(d_union_decl* dUnion, memory_arena* Memory)
{
  counted_string UnionName = dUnion->Name;
  counted_string Result = FormatCountedString(Memory, "struct %.*s\n{\n  %.*s_type Type;\n\n  union\n  {\n", UnionName.Count, UnionName.Start, UnionName.Count, UnionName.Start);

  d_union_member* Member = dUnion->Sentinel.Next;
  while (Member != &dUnion->Sentinel)
  {
    if (Member->Flags != d_union_flag_enum_only)
    {
      Result = Concat(Result, FormatCountedString(Memory, "    %.*s %.*s;\n", Member->Type.Count, Member->Type.Start, Member->Name.Count, Member->Name.Start), Memory);
    }
    Member = Member->Next;
  }

  Result = Concat(Result, CS("  };\n};\n"), Memory);

  return Result;
}

d_union_decl
ParseDiscriminatedUnion(c_parse_result* Parser, memory_arena* Memory)
{
  d_union_decl dUnion = {};

  InitSentinel(dUnion.Sentinel);

  RequireToken(Parser, CTokenType_OpenParen);
  dUnion.Name = RequireToken(Parser, CTokenType_Identifier).Value;

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
        // TODO(Jesse): How should we talk about string constants that refer to identifiers in the code?
        if ( OptionalToken(Parser, CToken(CS("enum_only"))) )
        {
          Flags = d_union_flag_enum_only;
        }

        PushMember(&dUnion, Interior, Flags, Memory);

        RequireToken(Parser, CTokenType_Semicolon);
      } break;

      case CTokenType_CloseBrace:
      {
        RequireToken(Parser, CTokenType_CloseParen);
        Complete = True;
      } break;

      default:
      {
        Parser->Valid = False;
      } break;
    }
  }

  return dUnion;
}

function struct_def*
GetStructByType(struct_defs* ProgramStructs, counted_string StructType)
{
  struct_def* Result = 0;
  for (u32 StructIndex = 0;
      StructIndex < ProgramStructs->Count;
      ++StructIndex)
  {
    struct_def* Struct = ProgramStructs->Defs[StructIndex];
    /* Print(Struct->Name); */
    /* Log("\n"); */
    if (StringsMatch(Struct->Name, StructType))
    {
      Result = Struct;
      break;
    }
  }

  return Result;
}

void
PushString(string_stream* Stream, counted_string String, memory_arena* Memory)
{
  string_chunk* Push = Allocate(string_chunk, Memory, 1);
  Push->String = String;

  Push->Prev = &Stream->Sentinel;
  Push->Next = Stream->Sentinel.Next;
  Stream->Sentinel.Next->Prev = Push;
  Stream->Sentinel.Next = Push;

  return;
}

arguments
ParseArgs(const char** ArgStrings, s32 ArgCount, memory_arena* Memory)
{
  arguments Result = {
    .OutPath = CS("src/metaprogramming/output"),
    .Files = AllocateBuffer<static_string_buffer, counted_string>((u32)ArgCount, Memory),
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
Output(counted_string Code, counted_string FileName, memory_arena* Memory)
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
      if (Rename(TempFile, FileName))
      {
        Result = True;
      }
      else
      {
        Error("Renaming tempfile: %.*s -> %.*s", (s32)TempFile.Path.Count, TempFile.Path.Start, (s32)FileName.Count, FileName.Start);
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

function b32
Output(d_union_decl* dUnion, counted_string FileName, memory_arena* Memory)
{
  counted_string EnumString = GenerateEnumDef(dUnion, Memory);
  counted_string StructString = GenerateStructDef(dUnion, Memory);

  counted_string OutputString = Concat(EnumString, StructString, Memory);
  b32 Result = Output(OutputString, FileName, Memory);
  return Result;
}

function void
DumpStringStreamToConsole(string_stream* Stream)
{
  for (string_iterator Iter = SSIterator(Stream);
      IsValid(&Iter);
      Advance(&Iter))
  {
    counted_string Message = Iter.At->String;
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

function struct_defs
AllocateStructDefs(u32 Count, memory_arena* Memory)
{
  struct_defs Result = {
    .Count = Count,
    .Defs = Allocate(struct_def*, Memory, Count),
  };

  return Result;
};

function struct_def*
StructDef(counted_string Name, memory_arena* Memory)
{
  struct_def* Result = Allocate(struct_def, Memory, 1);
  Result->Name = Name;
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

function struct_def* ParseStructBody(c_parse_result* Parser, counted_string StructName, memory_arena* Memory);

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

          InvalidDefaultWhileParsing(Parser, CS("While parsing decl type."));
        }

        continue;
      }

    } break;

    InvalidDefaultWhileParsing(Parser, CS("While parsing decl type."));
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
PrintCDecl(c_decl* Decl, struct_defs* ProgramStructs)
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
      for (c_decl_iterator Iter = CDIterator(&Decl->c_decl_union.Body->Fields);
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

struct for_member_constraints
{
  counted_string ForMemberName;
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

function counted_string
ParseForMembers(c_parse_result* Parser, for_member_constraints* Constraints, struct_defs* ProgramStructs, memory_arena* Memory)
{
  counted_string Result = {};

  RequireToken(Parser, CTokenType_OpenParen);

  counted_string StructType = RequireToken(Parser, CTokenType_Identifier).Value;
  struct_def* TargetStruct = GetStructByType(ProgramStructs, StructType);
  if (TargetStruct)
  {
    Constraints->ForMemberName = StructType;
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

    c_parse_result BodyText = *Parser;
    BodyText.Tokens.Start = BodyText.Tokens.At;
    EatNextScope(Parser);
    BodyText.Tokens.End = Parser->Tokens.At;

    TrimFirstToken(&BodyText, CTokenType_OpenBrace);
    TrimLastToken(&BodyText, CTokenType_CloseBrace);

    c_decl_stream_chunk* AtChunk = TargetStruct->Fields.FirstChunk;
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
          for (c_decl_iterator Iter = CDIterator(&AtChunk->Element.c_decl_union.Body->Fields);
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

  }
  else
  {
    Error("Couldn't find matching struct %.*s", (s32)StructType.Count, StructType.Start);
  }

  return Result;
}

function struct_def*
ParseStructBody(c_parse_result* Parser, counted_string StructName, memory_arena* Memory)
{
  struct_def* Result = StructDef(StructName, Memory);

  RequireToken(Parser, CTokenType_OpenBrace);

  c_token NextToken = PeekToken(Parser);

  while (Remaining(&Parser->Tokens) && NextToken.Type != CTokenType_CloseBrace)
  {
    if (NextToken.Type == CTokenType_Hash)
    {
      EatUntil(Parser, CTokenType_Newline);
      continue;
    }

    c_decl Declaration = ParseDeclaration(Parser, Result->Name, Memory);
    Push(&Result->Fields, Declaration, Memory);
    NextToken = PeekToken(Parser);
  }

  return Result;
}

function struct_defs
ParseAllStructDefs(tokenized_files Files_in, u32 MaxStructCount, memory_arena* Memory)
{
  tokenized_files* Files = &Files_in;

  // TODO(Jesse): This leaks a bit of memory because MaxStructCount is over-eager
  // @memory-leak
  struct_cursor StructCursor = Cursor<struct_cursor, struct_def*>(MaxStructCount, Memory);

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
            if (PeekToken(Parser) == CToken(CS("struct")))
            {
              RequireToken(Parser, CToken((CS("struct"))));
              if (PeekToken(Parser) == CToken(CTokenType_OpenBrace))
              {
                struct_def* S = ParseStructBody(Parser, CS(""), Memory);
                Push(S, &StructCursor);

                RequireToken(Parser, CTokenType_CloseBrace);
                S->Name = RequireToken(Parser, CTokenType_Identifier).Value;
                RequireToken(Parser, CTokenType_Semicolon);
              }
              else
              {
                // C-style typedef struct THING OTHERTHING;
                EatUntil(Parser, CTokenType_Semicolon);
              }
            }
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
                  struct_def* S = ParseStructBody(Parser, T.Value, Memory);
                  Push(S, &StructCursor);
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


  u32 Count = (u32)CurrentCount(&StructCursor);
  struct_defs Result = AllocateStructDefs(Count, Memory);
  Result.Defs = StructCursor.Start;

  return Result;
}

function tokenized_files
AllocateTokenizedFiles(u32 Count, memory_arena* Memory)
{
  c_parse_result* Start = Allocate(c_parse_result, Memory, Count);
  tokenized_files Result = {
    .Start = Start,
    .At = Start,
    .End = Start + Count,
  };

  return Result;
}

function tokenized_files
TokenizeAllFiles(static_string_buffer* FileNames, memory_arena* Memory)
{
  Assert(FileNames->At == FileNames->Start);

  tokenized_files Result = AllocateTokenizedFiles((u32)Count(FileNames), Memory);
  while ( FileNames->At < FileNames->End )
  {
    counted_string CurrentFile = *FileNames->At;

    c_parse_result Parser = TokenizeFile(CurrentFile, Memory);
    if (Parser.Valid)
    {
      Rewind(&Parser.Tokens);
      Push(Parser, &Result);
      Result.StructCount += Parser.StructCount;
    }
    else
    {
      Error("Tokenizing File: %.*s", (s32)CurrentFile.Count, CurrentFile.Start);
    }

    ++FileNames->At;
  }

  TruncateToCurrentSize(&Result);
  Rewind(&Result);

  return Result;
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

    string_stream SuccessFiles = StringStream();
    string_stream FailFiles = StringStream();

    arguments Args = ParseArgs(ArgStrings, ArgCount, Memory);
    Assert(Args.Files.Start == Args.Files.At);

    tokenized_files ParsedFiles = TokenizeAllFiles(&Args.Files, Memory);
    Assert(ParsedFiles.Start == ParsedFiles.At);

    struct_defs Structs = ParseAllStructDefs(ParsedFiles, ParsedFiles.StructCount, Memory);
    Assert(ParsedFiles.Start == ParsedFiles.At);

#if 0
    for (u32 StructDefIndex = 0;
        StructDefIndex < Structs.Count;
        ++StructDefIndex)
    {
      struct_def* Struct = Structs.Defs[StructDefIndex];
      DumpStruct(Struct);
    }
#endif

    for (u32 ParserIndex = 0;
        ParserIndex < Count(&ParsedFiles);
        ++ParserIndex)
    {
      b32 SuccessfullyOutput = True;
      c_parse_result* Parser = ParsedFiles.Start+ParserIndex;
      Assert(Parser->Valid);
      Assert(Remaining(&Parser->Tokens));
      while (Parser->Valid && Remaining(&Parser->Tokens))
      {
        c_token Token = PopToken(Parser);
        switch( Token.Type )
        {
          case CTokenType_Identifier:
          {
            if (StringsMatch(Token.Value, CS("d_union")))
            {
              d_union_decl dUnion = ParseDiscriminatedUnion(Parser, Memory);

              if (Parser->Valid)
              {
                counted_string OutFilePath = Concat(Args.OutPath, Basename(Parser->FileName), Memory);
                if (Output(&dUnion, OutFilePath, Memory))
                {
                  counted_string Message = Concat(CS(GREEN_TERMINAL "  ✔  " WHITE_TERMINAL), Parser->FileName, Memory);
                  PushString(&SuccessFiles, Message, Memory);
                }
                else
                {
                  SuccessfullyOutput = False;
                }
              }
              else
              {
                Error("Parsing d_union declaration");
              }
            }

            if (StringsMatch(Token.Value, CS("for_members_in")))
            {
              for_member_constraints Constraints = {};
              counted_string ForMembersCode = ParseForMembers(Parser, &Constraints, &Structs, Memory);
              counted_string FileBasename = StripExtension(Basename(Parser->FileName));
              counted_string ForMembersCodeFilename = CS(FormatString(Memory, "src/metaprogramming/output/%.*s_for_members_in_%.*s.h",
                    FileBasename.Count, FileBasename.Start,
                    Constraints.ForMemberName.Count, Constraints.ForMemberName.Start ));
              Output(ForMembersCode, ForMembersCodeFilename, Memory);
            }

          } break;

          default: { } break;
        }

        continue;
      }

      if (!Parser->Valid || !SuccessfullyOutput)
      {
        counted_string Message = Concat(CS(RED_TERMINAL "  ✗  " WHITE_TERMINAL), Parser->FileName, Memory);
        PushString(&FailFiles, Message, Memory);
      }

      Rewind(&Parser->Tokens);

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
