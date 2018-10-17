u8_stream
U8_StreamFromFile(const char* SourceFile, memory_arena *Memory)
{
  FILE *File = fopen(SourceFile, "r");
  u8* FileContents = 0;
  umm FileSize = 0;

  if (File)
  {
    fseek(File, 0L, SEEK_END);
    FileSize = (umm)ftell(File);
    rewind(File);
    FileContents = Allocate(u8, Memory, FileSize);
    ReadBytes((u8*)FileContents, FileSize, File);
  }
  else
  {
    Error("Opening %s", SourceFile);
  }

  u8_stream Result = {
    FileContents,
    FileContents,
    FileContents + FileSize
  };

  return Result;
}

ansi_stream
AnsiStreamFromFile(const char* SourceFile, memory_arena *Memory)
{
  u8_stream Binary = U8_StreamFromFile(SourceFile, Memory);
  ansi_stream Result = AnsiStream(&Binary);
  return Result;
}

ansi_stream
ReadEntireFileIntoString(const char *SourceFile, memory_arena *Memory)
{
  ansi_stream Stream = AnsiStreamFromFile(SourceFile, Memory);
  return Stream;
}

r32_stream
R32_Stream(u32 Count, memory_arena *Memory)
{
  r32 *Elements = Allocate(r32, Memory, Count);
  r32_stream Result = {};

  Result.Start = Elements;
  Result.At = Elements;
  Result.End = Elements + Count;

  return Result;
}

u32_stream
U32_Stream(u32 Count, memory_arena *Memory)
{
  u32 *Elements = Allocate(u32, Memory, Count);
  u32_stream Result = {};

  Result.Start = Elements;
  Result.At = Elements;
  Result.End = Elements + Count;

  return Result;
}

v3_stream
V3_Stream(u32 Count, memory_arena *Memory)
{
  v3 *Elements = Allocate(v3, Memory, Count);
  v3_stream Result = {};

  Result.Start = Elements;
  Result.At = Elements;
  Result.End = Elements + Count;

  return Result;
}

char *
ReadUntilTerminatorList(ansi_stream *Cursor, const char *TerminatorList, memory_arena *Arena)
{
  // TODO(Jesse): Make this return a counted string and everything that depends on
  // it _NOT_ rely on the fact it's currently null terminated

  counted_string String = ReadUntilTerminatorList(Cursor, TerminatorList);

  char *Result = Allocate(char, Arena, String.Count + 1);
  MemCopy((u8*)String.Start, (u8*)Result, String.Count);

  return Result;
}

char *
PopWord(ansi_stream *Cursor, memory_arena *Arena, const char *Delimeters = 0)
{
  if (!Delimeters)
    Delimeters = " \n";

  EatWhitespace(Cursor);
  char *Result = ReadUntilTerminatorList(Cursor, Delimeters, Arena);
  EatAllCharacters(Cursor, Delimeters);
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

counted_string
PopQuotedString(ansi_stream* Cursor, memory_arena* Arena)
{
  if (*Cursor->At == '"' || *Cursor->At == '\'' )
  {
    ++Cursor->At;
  }
  else
  {
    Assert( *(Cursor->At-1) == '"' || *(Cursor->At-1) == '\'' );
  }

  char Terminator[2] = {};
  Terminator[0] = *(Cursor->At-1);

  counted_string Result = CountedString(ReadUntilTerminatorList(Cursor, Terminator, Arena));
  return Result;
}

char *
PopLine(ansi_stream *Cursor, memory_arena *Arena)
{
  char *Result = ReadUntilTerminatorList(Cursor, "\n", Arena);
  EatWhitespace(Cursor);
  return Result;
}
