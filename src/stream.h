#include <string>

struct counted_string
{
  const char* Start;
  umm Count;
};

struct u32_stream
{
  u32* Start;
  u32* At;
  u32* End;
};

struct u8_stream
{
  u8* Start;
  u8* At;
  u8* End;
};

struct ansi_stream
{
  const char* Start;
  const char* At;
  const char* End;
};

struct r32_stream
{
  r32* Start;
  r32* At;
  r32* End;
};

struct v3_stream
{
  v3* Start;
  v3* At;
  v3* End;
};

template <typename element_t, typename stream_t>inline void
Push(element_t Element, stream_t *Array)
{
  Assert( Array->At < Array->End );
  *Array->At = Element;
  return;
}

template <typename element_t, typename stream_t> inline element_t
Pop(stream_t *Stream)
{
  Assert(Remaining(Stream));
  element_t Element = *Stream->At++;
  return Element;
}

struct mesh_metadata
{
  u32 VertCount;
  u32 NormalCount;
  u32 UVCount;
  u32 FaceCount;
};

umm
Length(const char *Str)
{
  const char *Start = Str;
  const char *End = Str;

  while (*End++);

  umm Result = (umm)(End - Start) - 1;
  return Result;
}

counted_string
CountedString(const char *S)
{
  counted_string Result = {};
  Result.Start = S;
  Result.Count = Length(S);
  return Result;
}

counted_string
CS(const char *S)
{
  counted_string Result = CountedString(S);
  return Result;
}

ansi_stream
AnsiStream(counted_string String)
{
  ansi_stream Result = {};

  Result.Start = String.Start;
  Result.At    = String.Start;
  Result.End   = String.Start + String.Count;

  return Result;
}

ansi_stream
AnsiStream(counted_string* String)
{
  ansi_stream Result = {};

  Result.Start = String->Start;
  Result.At    = String->Start;
  Result.End   = String->Start + String->Count;

  return Result;
}

ansi_stream
AnsiStream(const char *Input)
{
  ansi_stream Result = {};

  Result.Start = Input;
  Result.At    = Input;
  Result.End   = Input + Length(Input) + 1;

  return Result;
}

ansi_stream
AnsiStream(u8_stream *Input)
{
  ansi_stream Result = {};

  Result.Start = (const char*)Input->Start;
  Result.At    = (const char*)Input->At;
  Result.End   = (const char*)Input->End;

  return Result;
}

u8_stream
U8_Stream(u8* Start, u8* End)
{
  u8_stream Result = {
    Start,
    Start,
    End
  };
  return Result;
}

u32_stream
U32_Stream(u32* Start, u32* End)
{
  u32_stream Result = {
    Start,
    Start,
    End
  };
  return Result;
}

b32
Contains(counted_string Haystack, char Needle)
{
  b32 Result = False;
  for (u32 Index = 0;
      Index < Haystack.Count;
      ++Index)
  {
    Result |= (Needle == Haystack.Start[Index]);
  }
  return Result;
}

b32
Contains(const char* Haystack, char Needle)
{
  b32 Result = False;
  while (*Haystack)
  {
    Result |= (Needle == *Haystack++);
  }
  return Result;
}

void
EatAllCharacters(ansi_stream *Cursor, const char *Characters)
{
  while ( Remaining(Cursor) && Contains(Characters, *Cursor->At) )
  {
    ++Cursor->At;
  }
}

counted_string
ReadUntilTerminatorList(ansi_stream *Cursor, const char *TerminatorList)
{
  const char *Start = Cursor->At;

  while (Remaining(Cursor) && *Cursor->At)
  {
    const char *TerminatorAt = TerminatorList;
    while (*TerminatorAt)
    {
      if(*Cursor->At == *TerminatorAt)
      {
        goto finished;
      }
      ++TerminatorAt;
    }

    ++Cursor->At;
  }

finished:

  counted_string Result = {};
  Result.Start = Start;
  Result.Count = (umm)(Cursor->At - Start);

  if (Remaining(Cursor))
  {
    ++Cursor->At;
  }

  return Result;
}

counted_string
ReadUntilTerminatorList(counted_string* String, const char* TerminatorList)
{
  ansi_stream Cursor = AnsiStream(String);
  counted_string Result = ReadUntilTerminatorList(&Cursor, TerminatorList);
  return Result;
}

void
EatWhitespace(ansi_stream *Cursor)
{
  EatAllCharacters(Cursor, "\n ");
  return;
}

counted_string
PopWordCounted(ansi_stream *Cursor, const char *Delimeters = 0)
{
  if (!Delimeters)
    Delimeters = " \n";

  EatWhitespace(Cursor);

  counted_string Result = ReadUntilTerminatorList(Cursor, Delimeters);
  return Result;
}
