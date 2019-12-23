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

  CTokenType_OpenBrace     = '(',
  CTokenType_CloseBrace    = ')',
  CTokenType_OpenParen     = '{',
  CTokenType_CloseParen    = '}',
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
  if (Token.Type == CTokenType_Identifier)
  {
    Log("%.*s", Token.Value.Count, Token.Value.Start);
  }
  else
  {
    Log("%c", Token.Type);
  }
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

s32
main(s32 ArgCount, const char** Args)
{
  if (ArgCount > 1)
  {
    memory_arena Memory_ = {};
    memory_arena* Memory = &Memory_;


    for (s32 FileIndex = 1;
        FileIndex < ArgCount;
        ++FileIndex)
    {
      const char* FileName = Args[FileIndex];
      ansi_stream SourceFileStream = AnsiStreamFromFile(FileName, Memory);
      if (!SourceFileStream.Start)
      {
        Error("Allocating stream for %s", FileName);
      }

      c_token_buffer Tokens_ = AllocateTokenBuffer(Memory, (u32)Kilobytes(100));
      c_token_buffer* Tokens = &Tokens_;

      while(Remaining(&SourceFileStream))
      {
        c_token T = GetToken(&SourceFileStream);

        if (T.Type == CTokenType_Unknown)
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
        }
        else
        {
          SourceFileStream.At++;
        }

        Push(Tokens, T);

        continue;
      }

      Tokens->End = Tokens->At;
      Tokens->At = Tokens->Start;
      while (Remaining(Tokens))
      {
        PrintToken(*Tokens->At);
        Tokens->At++;
      }
    }
  }
  else
  {
    Warn("No files passed in, exiting.");
  }

  return 0;
}
