#include <string>

struct counted_string
{
  const char* Start;
  umm Count;
};

struct binary_stream_u32
{
  u32* Start;
  u32* At;
  u32* End;
};

struct binary_stream_u8
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

umm
Length(const char *Str)
{
  const char *Start = Str;
  const char *End = Str;

  while (*End++);

  umm Result = (umm)(End - Start) - 1;
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
AnsiStream(binary_stream_u8 *Input)
{
  ansi_stream Result = {};

  Result.Start = (const char*)Input->Start;
  Result.At    = (const char*)Input->At;
  Result.End   = (const char*)Input->End;

  return Result;
}

binary_stream_u8
BinaryStream(u8* Start, u8* End)
{
  binary_stream_u8 Result = {
    Start,
    Start,
    End
  };
  return Result;
}

binary_stream_u32
BinaryStream(u32* Start, u32* End)
{
  binary_stream_u32 Result = {
    Start,
    Start,
    End
  };
  return Result;
}

binary_stream_u8
BinaryStreamU8(u8* Start, u8* End)
{
  binary_stream_u8 Result = {
    Start,
    Start,
    End
  };
  return Result;
}

binary_stream_u8
BinaryStreamFromFile(const char* SourceFile, memory_arena *Memory)
{
  FILE *File = fopen(SourceFile, "r");
  u8* FileContents = 0;
  umm FileSize = 0;

  if (File)
  {
    fseek(File, 0L, SEEK_END);
    FileSize = (umm)ftell(File);
    rewind(File);
    FileContents = (u8*)PushStruct(Memory, (FileSize));
    ReadBytes((u8*)FileContents, FileSize, File);
  }
  else
  {
    Error("Opening %s", SourceFile);
  }

  binary_stream_u8 Result = {
    FileContents,
    FileContents,
    FileContents + FileSize
  };

  return Result;
}

ansi_stream
AnsiStreamFromFile(const char* SourceFile, memory_arena *Memory)
{
  binary_stream_u8 Binary = BinaryStreamFromFile(SourceFile, Memory);
  ansi_stream Result = AnsiStream(&Binary);
  return Result;
}

ansi_stream
ReadEntireFileIntoString(const char *SourceFile, memory_arena *Memory)
{
  ansi_stream Stream = AnsiStreamFromFile(SourceFile, Memory);
  return Stream;
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

  umm ResultLength = 0;
  b32 FoundTerminator = False;

  while (Remaining(Cursor) && *Cursor->At++)
  {
    ResultLength = (umm)(Cursor->At - Start);
    const char *TerminatorAt = TerminatorList;
    while (*TerminatorAt)
    {
      if(*Cursor->At == *TerminatorAt)
      {
        FoundTerminator = True;
        ResultLength = (umm)(Cursor->At - Start);
        EatAllCharacters(Cursor, TerminatorList);
        goto done;
      }
      ++TerminatorAt;
    }
  }
done:

  counted_string Result = {};
  Result.Start = Start;
  Result.Count = ResultLength;
  return Result;
}

char *
ReadUntilTerminatorList(ansi_stream *Cursor, const char *TerminatorList, memory_arena *Arena)
{
  const char *Start = Cursor->At;

  umm ResultLength = 0;
  b32 FoundTerminator = False;

  while (Remaining(Cursor) && *Cursor->At++)
  {
    ResultLength = (umm)(Cursor->At - Start);
    const char *TerminatorAt = TerminatorList;
    while (*TerminatorAt)
    {
      if(*Cursor->At == *TerminatorAt)
      {
        FoundTerminator = True;
        ResultLength = (umm)(Cursor->At - Start);
        EatAllCharacters(Cursor, TerminatorList);
        goto done;
      }
      ++TerminatorAt;
    }
  }
done:

  char *Result = Allocate(char, Arena, ResultLength + 1);
  MemCopy((u8*)Start, (u8*)Result, ResultLength);

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

char *
PopWord(ansi_stream *Cursor, memory_arena *Arena, const char *Delimeters = 0)
{
  if (!Delimeters)
    Delimeters = " \n";

  EatWhitespace(Cursor);
  char *Result = ReadUntilTerminatorList(Cursor, Delimeters, Arena);
  return Result;
}

counted_string
PopXmlTag(ansi_stream *Cursor)
{
  if (*Cursor->At == '<')
    Cursor->At++;

  EatWhitespace(Cursor);

  const char* Delimeters = "> \n";

  counted_string Result = ReadUntilTerminatorList(Cursor, Delimeters);
  return Result;
}

char *
PopLine(ansi_stream *Cursor, memory_arena *Arena)
{
  char *Result = ReadUntilTerminatorList(Cursor, "\n", Arena);
  return Result;
}

r32
PopFloat(ansi_stream *Cursor, memory_arena *Arena)
{
  char *Float = PopWord(Cursor, Arena);
  r32 Result = (r32)atof(Float);
  return Result;
}

u32
PopU32(ansi_stream *Cursor, memory_arena *Arena, const char* Delim = 0)
{
  char *Str = PopWord(Cursor, Arena, Delim);
  u32 Result = (u32)atoi(Str);
  return Result;
}

