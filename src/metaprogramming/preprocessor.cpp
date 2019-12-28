#define BONSAI_NO_MUTEX_TRACKING
#define BONSAI_NO_PUSH_METADATA
#define BONSAI_NO_DEBUG_MEMORY_ALLOCATOR

#include <bonsai_types.h>
#include <unix_platform.cpp>

global_variable memory_arena* TranArena = {};
#include <counted_string.cpp>
#include <stream.cpp>

enum c_token_type
{
  CTokenType_Unknown,

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

string_stream
StringStream()
{
  string_stream Stream = {};
  Stream.Sentinel.Next = &Stream.Sentinel;
  Stream.Sentinel.Prev = &Stream.Sentinel;
  return Stream;
}

void
LogStringStream(string_stream* Stream)
{
  string_chunk* At = Stream->Sentinel.Prev;
  while (At != &Stream->Sentinel)
  {
    Log("%.*s", At->String.Count, At->String.Start);
    At = At->Prev;
  }
  Log("\n\n");
}

void
ParseDiscriminatedUnion(c_parse_result* Parser, memory_arena* Memory)
{
  string_stream StructStream = StringStream();
  string_stream EnumStream = StringStream();

  RequireToken(Parser, CTokenType_OpenParen);
  c_token UnionName = RequireToken(Parser, CTokenType_Identifier);

  PushString(&StructStream,
             FormatCountedString(Memory, "struct %.*s\n{\n  %.*s_type Type;\n\n  union\n  {", UnionName.Value.Count, UnionName.Value.Start, UnionName.Value.Count, UnionName.Value.Start),
             Memory);

  PushString(&EnumStream,
             FormatCountedString(Memory, "enum %.*s_type\n{\n  type_%.*s_noop,\n", UnionName.Value.Count, UnionName.Value.Start, UnionName.Value.Count, UnionName.Value.Start),
             Memory);

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
        if ( ! (OptionalToken(Parser, CTokenType_Identifier) == CToken(CS("enum_only"))) )
        {
          PushString(&StructStream,
                     FormatCountedString(Memory, "    %.*s %.*s;", Interior.Value.Count, Interior.Value.Start, Interior.Value.Count, Interior.Value.Start),
                     Memory);
        }

        PushString(&EnumStream,
                   FormatCountedString(Memory, "  type_%.*s,", Interior.Value.Count, Interior.Value.Start),
                   Memory);

        RequireToken(Parser, CTokenType_Semicolon);
      } break;

      case CTokenType_CloseBrace:
      {
        PushString(&StructStream, FormatCountedString(Memory, "  };\n};\n"), Memory);
        PushString(&EnumStream, FormatCountedString(Memory, "};"), Memory);
        RequireToken(Parser, CTokenType_CloseParen);
        Complete = True;
      } break;

      case CTokenType_Newline:
      {
        PushString(&StructStream, FormatCountedString(Memory, "\n"), Memory);
        PushString(&EnumStream, FormatCountedString(Memory, "\n"), Memory);
      } break;

      case CTokenType_Space: { } break;

      default:
      {
        Parser->Valid = False;
      } break;
    }
  }

  if (Parser->Valid)
  {
    LogStringStream(&EnumStream);
    LogStringStream(&StructStream);
  }
  else
  {
    Error("Parsing d_union declaration");
  }

  return;
}

struct arguments
{
  counted_string OutPath;
  string_stream Files;
};

arguments
ParseArgs(const char** ArgStrings, s32 ArgCount, memory_arena* Memory)
{
  arguments Result = {
    .Files = StringStream(),
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
      PushString(&Result.Files, Arg, Memory);
    }
  }

  return Result;
}

s32
main(s32 ArgCount, const char** ArgStrings)
{
  b32 Result = True;

  if (ArgCount > 1)
  {
    memory_arena Memory_ = {};
    memory_arena* Memory = &Memory_;

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
            case CTokenType_Identifier:
            {
              if (StringsMatch(Token.Value, CS("d_union")))
              {
                ParseDiscriminatedUnion(&Parser, Memory);
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

      Result = Result && Parser.Valid;

      NextFile = PopString(&Args.Files);
    }
  }
  else
  {
    Warn("No files passed, exiting.");
  }

  return (s32)Result;
}
