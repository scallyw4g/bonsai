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
  CTokenType_DUnion,
  CTokenType_Count,
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
    case CTokenType_Unknown:
    {
      Log("-- CTokenType_Unknown");
    } break;
    case CTokenType_DUnion:
    {
      Log("-- CTokenType_DUnion");
    } break;
    InvalidDefaultCase;
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
        const char* Delimeters = " #(){},:;";
        counted_string Token = PopWordCounted(&SourceFileStream, Delimeters);
        if (StringsMatch(Token, CS("d_union")))
        {
          c_token PushToken = {.Type=CTokenType_DUnion};
          Push(Tokens, PushToken);
        }
        else
        {
          c_token PushToken = {};
          Push(Tokens, PushToken);
        }
      }

      Tokens->At = Tokens->Start;
      while (Remaining(Tokens))
      {
        if (Tokens->At->Type == CTokenType_DUnion)
        {
          PrintToken(*Tokens->At);
        }
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
