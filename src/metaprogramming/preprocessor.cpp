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

  CTokenType_OpenBrace   = '(',
  CTokenType_CloseBrace  = ')',
  CTokenType_OpenParen   = '{',
  CTokenType_CloseParen  = '}',
  CTokenType_Dot         = '.',
  CTokenType_Comma       = ',',
  CTokenType_Semicolon   = ';',
  CTokenType_Colon       = ':',
  CTokenType_Hash        = '#',
  CTokenType_Space       = ' ',
  CTokenType_Star        = '*',
  CTokenType_Ampersand   = '&',
  CTokenType_SingleQuote = '\'',
  CTokenType_DoubleQuote = '"',
  CTokenType_Equals      = '=',
  CTokenType_LT          = '<',
  CTokenType_GT          = '>',
  CTokenType_Plus        = '+',
  CTokenType_Minus       = '-',
  CTokenType_Percent     = '%',
  CTokenType_Bang        = '!',
  CTokenType_Hat         = '^',
  CTokenType_Question    = '?',
  CTokenType_FSlash      = '/',
  CTokenType_BSlash      = '\\',
  CTokenType_Tilde       = '~',
  CTokenType_Backtick    = '`',

  CTokenType_DUnion,
};


struct c_token
{
  c_token_type Type;
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
    case CTokenType_DUnion:
    {
      Log("-- CTokenType_DUnion");
    } break;

    default:
    {
      Log("%c", Token.Type);
    } break;
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
        char At = *SourceFileStream.At++;

        switch (At)
        {
          case CTokenType_OpenBrace:
          {
            c_token PushToken = { .Type = CTokenType_OpenBrace, };
            Push(Tokens, PushToken);
          } break;
          case CTokenType_CloseBrace:
          {
            c_token PushToken = { .Type= CTokenType_CloseBrace };
            Push(Tokens, PushToken);
          } break;
          case CTokenType_OpenParen:
          {
            c_token PushToken = { .Type= CTokenType_OpenParen };
            Push(Tokens, PushToken);
          } break;
          case CTokenType_CloseParen:
          {
            c_token PushToken = { .Type= CTokenType_CloseParen };
            Push(Tokens, PushToken);
          } break;
          case CTokenType_Dot:
          {
            c_token PushToken = { .Type= CTokenType_Dot };
            Push(Tokens, PushToken);
          } break;
          case CTokenType_Comma:
          {
            c_token PushToken = { .Type= CTokenType_Comma };
            Push(Tokens, PushToken);
          } break;
          case CTokenType_Semicolon:
          {
            c_token PushToken = { .Type= CTokenType_Semicolon };
            Push(Tokens, PushToken);
          } break;
          case CTokenType_Colon:
          {
            c_token PushToken = { .Type= CTokenType_Colon };
            Push(Tokens, PushToken);
          } break;
          case CTokenType_Hash:
          {
            c_token PushToken = { .Type= CTokenType_Hash };
            Push(Tokens, PushToken);
          } break;
          case CTokenType_Space:
          {
            c_token PushToken = { .Type= CTokenType_Space };
            Push(Tokens, PushToken);
          } break;
          case CTokenType_Star:
          {
            c_token PushToken = { .Type= CTokenType_Star };
            Push(Tokens, PushToken);
          } break;
          case CTokenType_Ampersand:
          {
            c_token PushToken = { .Type= CTokenType_Ampersand };
            Push(Tokens, PushToken);
          } break;
          case CTokenType_SingleQuote:
          {
            c_token PushToken = { .Type= CTokenType_SingleQuote };
            Push(Tokens, PushToken);
          } break;
          case CTokenType_DoubleQuote:
          {
            c_token PushToken = { .Type= CTokenType_DoubleQuote };
            Push(Tokens, PushToken);
          } break;
          case CTokenType_Equals:
          {
            c_token PushToken = { .Type= CTokenType_Equals };
            Push(Tokens, PushToken);
          } break;
          case CTokenType_LT:
          {
            c_token PushToken = { .Type= CTokenType_LT };
            Push(Tokens, PushToken);
          } break;
          case CTokenType_GT:
          {
            c_token PushToken = { .Type= CTokenType_GT };
            Push(Tokens, PushToken);
          } break;
          case CTokenType_Plus:
          {
            c_token PushToken = { .Type= CTokenType_Plus };
            Push(Tokens, PushToken);
          } break;
          case CTokenType_Minus:
          {
            c_token PushToken = { .Type= CTokenType_Minus };
            Push(Tokens, PushToken);
          } break;
          case CTokenType_Percent:
          {
            c_token PushToken = { .Type= CTokenType_Percent };
            Push(Tokens, PushToken);
          } break;
          case CTokenType_Bang:
          {
            c_token PushToken = { .Type= CTokenType_Bang };
            Push(Tokens, PushToken);
          } break;
          case CTokenType_Hat:
          {
            c_token PushToken = { .Type= CTokenType_Hat };
            Push(Tokens, PushToken);
          } break;
          case CTokenType_Question:
          {
            c_token PushToken = { .Type= CTokenType_Question };
            Push(Tokens, PushToken);
          } break;
          case CTokenType_FSlash:
          {
            c_token PushToken = { .Type= CTokenType_FSlash };
            Push(Tokens, PushToken);
          } break;
          case CTokenType_BSlash:
          {
            c_token PushToken = { .Type= CTokenType_BSlash };
            Push(Tokens, PushToken);
          } break;
          case CTokenType_Tilde:
          {
            c_token PushToken = { .Type= CTokenType_Tilde };
            Push(Tokens, PushToken);
          } break;
          case CTokenType_Backtick:
          {
            c_token PushToken = { .Type= CTokenType_Backtick };
            Push(Tokens, PushToken);
          } break;
          case CTokenType_Unknown:
          {
            c_token PushToken = { .Type= CTokenType_Unknown };
            Push(Tokens, PushToken);
          } break;

          default:
          {
            Warn("Hit Default");
          } break;
        }

        /* counted_string Token = PopWordCounted(&SourceFileStream, Delimeters); */
        /* if (StringsMatch(Token, CS("d_union"))) */
        /* { */
        /*   c_token PushToken = {.Type=CTokenType_DUnion}; */
        /*   Push(Tokens, PushToken); */
        /* } */
        /* else */
        /* { */
        /*   c_token PushToken = {}; */
        /*   Push(Tokens, PushToken); */
        /* } */

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
