#define BONSAI_NO_MUTEX_TRACKING
#define BONSAI_NO_PUSH_METADATA
#define BONSAI_NO_DEBUG_MEMORY_ALLOCATOR

#include <bonsai_types.h>
#include <unix_platform.cpp>

enum c_token_type
{
  CTokenType_Unknown,

  CTokenType_Comment,
  CTokenType_Identifier,
  CTokenType_String,

  CTokenType_OpenBrace     = '{',
  CTokenType_CloseBrace    = '}',
  CTokenType_OpenParen     = '(',
  CTokenType_CloseParen    = ')',
  CTokenType_Dot           = '.',
  CTokenType_Comma         = ',',
  CTokenType_Semicolon     = ';',
  CTokenType_Colon         = ':',
  CTokenType_Hash          = '#',
  CTokenType_Space         = ' ',
  CTokenType_Star          = '*',
  CTokenType_Ampersand     = '&',
  CTokenType_SingleQuote   = '\'',
  CTokenType_DoubleQuote   = '"',
  CTokenType_Equals        = '=',
  CTokenType_LT            = '<',
  CTokenType_GT            = '>',
  CTokenType_Plus          = '+',
  CTokenType_Minus         = '-',
  CTokenType_Percent       = '%',
  CTokenType_Bang          = '!',
  CTokenType_Hat           = '^',
  CTokenType_Question      = '?',
  CTokenType_FSlash        = '/',
  CTokenType_BSlash        = '\\',
  CTokenType_Tilde         = '~',
  CTokenType_Backtick      = '`',
  CTokenType_Newline       = '\n',
  CTokenType_CarrigeReturn = '\r',
  CTokenType_EOF           = EOF,
};

struct c_token
{
  c_token_type Type;
  counted_string Value;
};

struct c_token_buffer
{
  c_token* Start;
  c_token* At;
  c_token* End;
};

inline void
PrintToken(c_token Token)
{
  switch (Token.Type)
  {
    case CTokenType_Identifier:
    {
      Log("%.*s", Token.Value.Count, Token.Value.Start);
    } break;

    case CTokenType_String:
    {
      Log("\"%.*s\"", Token.Value.Count, Token.Value.Start);
    } break;

    default:
    {
      Log("'%c'", Token.Type);
    }
  }
}

b32
operator==(c_token T1, c_token T2)
{
  b32 Result = (T1.Type == T2.Type);
  if (Result && T1.Value.Count > 0)
  {
    Result &= StringsMatch(T1.Value, T2.Value);
  }
  return Result;
}

b32
operator!=(c_token T1, c_token T2)
{
  b32 Result = !(T1==T2);
  return Result;
}

inline c_token
CToken(counted_string Value)
{
  c_token Result = {
    .Type = CTokenType_Identifier,
    .Value = Value
  };
  return Result;
}

inline c_token
CToken(c_token_type Type, counted_string Value = CountedString(""))
{
  c_token Result = {
    .Type = Type,
    .Value = Value
  };
  return Result;
}

c_token_buffer
AllocateTokenBuffer(memory_arena* Memory, u32 Count)
{
  c_token_buffer Result = {};
  Result.Start = Allocate(c_token, Memory, Count);
  Result.At = Result.Start;
  Result.End = Result.Start + Count;

  return Result;
}

template <typename BufferType, typename PushType> void
Push(BufferType *Buffer, PushType Token)
{
  if (Buffer->At < Buffer->End)
  {
    *Buffer->At = Token;
    Buffer->At++;
  }
  else
  {
    Error("Ran out of space pushing onto buffer.");
  }
}

c_token
GetToken(ansi_stream* Stream)
{
  c_token Result = {};

  char At = *Stream->At;
  switch (At)
  {
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

  return Result;
}

struct string_chunk
{
  counted_string String;
  string_chunk* Next;
  string_chunk* Prev;
};

struct string_stream
{
  string_chunk Sentinel;
};

struct c_parse_result
{
  b32 Valid;
  c_token_buffer Tokens;
};

function b32
IsWhitespace(c_token_type Type)
{
  b32 Result = Type == CTokenType_Newline       ||
               Type == CTokenType_CarrigeReturn ||
               Type == CTokenType_Space;
  return Result;
}

function c_token
PeekTokenRaw(c_parse_result* Parser)
{
  c_token Result = {};
  if (Remaining(&Parser->Tokens))
  {
    Result = *Parser->Tokens.At;
  }
  else
  {
    Warn("Tried to peek a token on an empty stream");
  }

  return Result;
}

function c_token
PeekToken(c_parse_result* Parser)
{
  c_token Result = {};
  while (Remaining(&Parser->Tokens))
  {
    Result = *Parser->Tokens.At;
    if (IsWhitespace(Result.Type))
    {
      Parser->Tokens.At++;
    }
    else
    {
      break;
    }
  }

  return Result;
}

function c_token
PopTokenRaw(c_parse_result* Parser)
{
  c_token Result = {};
  if (Remaining(&Parser->Tokens))
  {
    Result = *Parser->Tokens.At++;
  }
  else
  {
    Warn("Tried to pop a token on an empty stream");
  }

  return Result;
}

function c_token
PopToken(c_parse_result* Parser)
{
  c_token Result = PopTokenRaw(Parser);
  while ( IsWhitespace(Result.Type) && Remaining(&Parser->Tokens) )
  {
    Result = *Parser->Tokens.At++;
  }

  return Result;
}

function c_token
OptionalToken(c_parse_result* Parser, c_token_type Type)
{
  c_token Result = PeekToken(Parser);

  if (Result.Type == Type)
  {
    PopToken(Parser);
  }
  else
  {
    Result = CToken(CTokenType_Unknown);
  }

  return Result;
}

function c_parse_result
TokenizeFile(const char* FileName, memory_arena* Memory)
{
  c_parse_result Result = {};

  // TODO(Jesse): Since we store pointers directly into this buffer, we need to
  // keep the memory around.  Should we tokenize such that we allocate new
  // memory for things that need it?  (only Strings/Identifiers at the moment)
  ansi_stream SourceFileStream = AnsiStreamFromFile(FileName, Memory);
  if (!SourceFileStream.Start)
  {
    Error("Allocating stream for %s", FileName);
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
        ++SourceFileStream.At;
        if (Remaining(&SourceFileStream))
        {
          T = GetToken(&SourceFileStream);
          switch (T.Type)
          {
            case CTokenType_FSlash:
            {
              T.Type = CTokenType_Comment;
              T.Value = CS(PopLine(&SourceFileStream, Memory));
            } break;

            case CTokenType_Star:
            {
              T.Type = CTokenType_Comment;
              T.Value = ReadUntilTerminatorString(&SourceFileStream, CS("*/"));
            } break;

            default: {} break;
          }
        }
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

        while (1)
        {
          T = GetToken(&SourceFileStream);
          if (T.Type == CTokenType_Unknown && Remaining(&SourceFileStream))
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
      } break;

      default:
      {
        SourceFileStream.At++;
      } break;
    }

    Push(&Result.Tokens, T);

    continue;
  }


  Result.Tokens.End = Result.Tokens.At;
  Result.Tokens.At = Result.Tokens.Start;
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

function c_token
RequireToken(c_parse_result* Parser, c_token_type Type)
{
  c_token Result = PopToken(Parser);

  if (Result.Type != Type)
  {
    Error("Encountered Unexpected token type");
    PrintToken(Result);

    Log("\nExpected: ");
    PrintToken(CToken(Type));
    Log("\n");

    Parser->Valid = False;
  }

  return Result;
}

#define DList_Push(Stream, Push)      \
{                                     \
  Push->Prev = &Stream->Sentinel;     \
  Push->Next = Stream->Sentinel.Next; \
  Stream->Sentinel.Next->Prev = Push; \
  Stream->Sentinel.Next = Push;       \
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

#define InitSentinel(Sentinel) \
{ \
  Sentinel.Next = &Sentinel; \
  Sentinel.Prev = &Sentinel; \
}

enum d_union_flags
{
  d_union_flag_none,
  d_union_flag_enum_only
};

struct d_union_member
{
  counted_string Type;
  counted_string Name;
  d_union_flags Flags;
  d_union_member* Next;
  d_union_member* Prev;
};

struct d_union_decl
{
  counted_string Name;
  d_union_member Sentinel;
};

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

function b32
WriteEnumTo(d_union_decl* dUnion, native_file* OutFile, memory_arena* Memory)
{
  counted_string Decl = FormatCountedString(Memory, "enum %.*s_type\n{\n  type_%.*s_noop,\n", dUnion->Name.Count, dUnion->Name.Start, dUnion->Name.Count, dUnion->Name.Start);
  b32 Result = WriteToFile(OutFile, Decl);

  d_union_member* Member = dUnion->Sentinel.Next;
  while (Member != &dUnion->Sentinel)
  {
    counted_string MemberDef = FormatCountedString(Memory, "  type_%.*s,\n", Member->Type.Count, Member->Type.Start);
    Result &= WriteToFile(OutFile, MemberDef);
    Member = Member->Next;
  }

  Result &= WriteToFile(OutFile, CS("};\n\n"));
  return Result;
}

function b32
WriteStructTo(d_union_decl* dUnion, native_file* OutFile, memory_arena* Memory)
{
  counted_string UnionName = dUnion->Name;
  counted_string Decl = FormatCountedString(Memory, "struct %.*s\n{\n  %.*s_type Type;\n\n  union\n  {\n", UnionName.Count, UnionName.Start, UnionName.Count, UnionName.Start);
  b32 Result = WriteToFile(OutFile, Decl);

  d_union_member* Member = dUnion->Sentinel.Next;
  while (Member != &dUnion->Sentinel)
  {
    if (Member->Flags != d_union_flag_enum_only)
    {
      counted_string MemberDef = FormatCountedString(Memory, "    %.*s %.*s;\n", Member->Type.Count, Member->Type.Start, Member->Name.Count, Member->Name.Start);
      Result &= WriteToFile(OutFile, MemberDef);
    }
    Member = Member->Next;
  }

  Result &= WriteToFile(OutFile, CS("  };\n};\n"));
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
  while (!Complete)
  {
    c_token Interior = PopTokenRaw(Parser);

    switch (Interior.Type)
    {
      case CTokenType_Identifier:
      {
        d_union_flags Flags = {};
        // TODO(Jesse): How should we talk about string constants that refer to identifiers in the code?
        if ( OptionalToken(Parser, CTokenType_Identifier) == CToken(CS("enum_only")) )
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

      case CTokenType_Newline:
      case CTokenType_Comment:
      case CTokenType_Space: { } break;

      default:
      {
        Parser->Valid = False;
      } break;
    }
  }

  return dUnion;
}

/* function void */
/* ParseForMembers(c_parse_result* Parser, memory_arena* Memory) */
/* { */
/*   NotImplemented; */

/*   RequireToken(Parser, CTokenType_OpenParen); */
/*   counted_string Type = RequireToken(Parser, CTokenType_Identifier).Value; */

/*   RequireToken(Parser, CTokenType_Comma); */
/*   RequireToken(Parser, CTokenType_OpenBrace); */

/*   return; */
/* } */

struct arguments
{
  counted_string OutPath;
  string_stream Files;
};

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
  arguments Result = {};
  InitSentinel(Result.Files.Sentinel);


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
      PushString(&Result.Files, Arg, Memory);
    }
  }

  return Result;
}

#define SUCCESS_EXIT_CODE 0
#define FAILURE_EXIT_CODE 1

s32
main(s32 ArgCount, const char** ArgStrings)
{
  b32 Success = True;

  if (ArgCount > 1)
  {
    memory_arena Memory_ = {};
    memory_arena* Memory = &Memory_;
    random_series FileNameEntropy = {3215432};

    arguments Args = ParseArgs(ArgStrings, ArgCount, Memory);

    counted_string NextFile = PopString(&Args.Files);
    while (NextFile.Count)
    {
      // FIXME(Jesse): This is poor form - I know this is null-terminated but the functions consuming this should operate on counted_strings
      const char* FileName = NextFile.Start;
      c_parse_result Parser = TokenizeFile(FileName, Memory);
      if (Parser.Valid)
      {
        c_token_buffer* Tokens = &Parser.Tokens;
        while (Parser.Valid && Remaining(Tokens))
        {
          c_token Token = PopToken(&Parser);
          switch( Token.Type )
          {
            case CTokenType_Comment:
            {
            } break;

            case CTokenType_Identifier:
            {
              if (StringsMatch(Token.Value, CS("d_union")))
              {
                d_union_decl dUnion = ParseDiscriminatedUnion(&Parser, Memory);

                if (Parser.Valid)
                {
                  native_file TempFile = GetTempFile(&FileNameEntropy, Memory);
                  if (TempFile.Handle)
                  {
                    b32 FileWritesSucceeded = WriteEnumTo(&dUnion, &TempFile, Memory);
                    FileWritesSucceeded &= WriteStructTo(&dUnion, &TempFile, Memory);
                    FileWritesSucceeded &= CloseFile(&TempFile);

                    if (FileWritesSucceeded)
                    {
                      counted_string FinalFilePath = Concat(CS("src/metaprogramming/output"), Basename(NextFile), Memory);
                      if (!Rename(TempFile, FinalFilePath))
                      {
                        Error("Renaming tempfile: %.*s", (s32)TempFile.Path.Count, TempFile.Path.Start);
                        Success = False;
                      }
                    }
                    else
                    {
                      Error("Writing to tempfile: %.*s", (s32)TempFile.Path.Count, TempFile.Path.Start);
                      Success = False;
                    }
                  }
                  else
                  {
                    Error("Opening tempfile: %.*s", (s32)TempFile.Path.Count, TempFile.Path.Start);
                    Success = False;
                  }
                }
                else
                {
                  Error("Parsing d_union declaration");
                  Success = False;
                }
              }

              if (StringsMatch(Token.Value, CS("for_members_in")))
              {
                /* ParseForMembers(&Parser, Memory); */
              }

            } break;

            default: { } break;
          }

          continue;
        }
      }
      else
      {
        Error("Tokenizing File: %s", FileName);
      }

      Success = Success && Parser.Valid;

      NextFile = PopString(&Args.Files);
    }
  }
  else
  {
    Warn("No files passed, exiting.");
  }

  s32 Result = Success ? SUCCESS_EXIT_CODE : FAILURE_EXIT_CODE ;
  return Result;
}
