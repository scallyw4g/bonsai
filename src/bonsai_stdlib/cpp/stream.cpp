template <typename stream_t, typename element_t>inline stream_t
AllocateBuffer(u32 Count, memory_arena* Memory)
{
  element_t* Start = Allocate(element_t, Memory, Count);
  stream_t Result = {
    .Start = Start,
    .At = Start,
    .End = Start + Count,
  };

  return Result;
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

v3_cursor
V3Cursor(u32 Count, memory_arena *Memory)
{
  v3 *Elements = Allocate(v3, Memory, Count);
  v3_cursor Result = {};

  Result.Start = Elements;
  Result.At = Elements;
  Result.End = Elements + Count;

  return Result;
}

//
// TODO(Jesse, id: 145, tags: platform): Make this use platform agnostic functions!
u8_stream
U8_StreamFromFile(const char* SourceFile, memory_arena *Memory)
{
  u8* FileContents = 0;
  umm FileSize = 0;

  native_file File = OpenFile(SourceFile, "rb");
  if (File.Handle)
  {
    fseek(File.Handle, 0L, SEEK_END);
    FileSize = (umm)ftell(File.Handle);
    if (FileSize)
    {
      rewind(File.Handle);
      FileContents = (u8*)Allocate(u8, Memory, FileSize);
      ReadBytesIntoBuffer(File.Handle, FileSize, FileContents);
    }
    else
    {
      Warn("File %s is empty!", SourceFile);
    }

    CloseFile(&File);
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

u8_stream
U8_StreamFromFile(counted_string SourceFile, memory_arena *Memory)
{
  u8_stream Result = U8_StreamFromFile( GetNullTerminated(SourceFile), Memory );
  return Result;
}

ansi_stream
AnsiStreamFromFile(const char* SourceFile, memory_arena *Memory)
{
  u8_stream Binary = U8_StreamFromFile(SourceFile, Memory);
  ansi_stream Result = AnsiStream(&Binary);
  Result.Filename = CS(SourceFile);

  return Result;
}

ansi_stream
AnsiStreamFromFile(counted_string SourceFile, memory_arena *Memory)
{
  const char* NullTerminated = GetNullTerminated(SourceFile, Memory);
  ansi_stream Result = AnsiStreamFromFile(NullTerminated, Memory);
  return Result;
}

ansi_stream
ReadEntireFileIntoAnsiStream(counted_string SourceFile, memory_arena *Memory)
{
  ansi_stream Stream = AnsiStreamFromFile(SourceFile, Memory);
  return Stream;
}

counted_string
CS(u8_stream Stream)
{
  counted_string Result = {
    .Count = TotalSize(&Stream),
    .Start = (const char*)Stream.Start,
  };
  return Result;
}

counted_string
ReadEntireFileIntoString(counted_string SourceFile, memory_arena *Memory)
{
  counted_string Stream = CS(U8_StreamFromFile(SourceFile, Memory));
  return Stream;
}

char *
ReadUntilTerminatorList(ansi_stream *Cursor, const char *TerminatorList, memory_arena *Arena)
{
  /* TODO(Jesse, id: 146, tags: robustness, cleanup): Make this return a
   * counted string and everything that depends on it _NOT_ rely on the fact
   * it's currently null terminated
   */

  counted_string String = ReadUntilTerminatorList(Cursor, TerminatorList);

  char *Result = AllocateProtection(char, Arena, String.Count + 1, False);
  MemCopy((u8*)String.Start, (u8*)Result, String.Count);

  return Result;
}

char *
PopWord(ansi_stream *Cursor, memory_arena *Arena, const char *Delimeters = 0)
{
  if (!Delimeters)
    Delimeters = " \n\r";

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
PopQuotedCharLiteral(ansi_stream* Cursor, b32 IncludeQuotes = False)
{
  if (*Cursor->At == '\'')
  {
    Advance(Cursor);
  }
  else
  {
    if (Cursor->At-1 >= Cursor->Start)
    {
      Assert(*(Cursor->At-1) == '\'' );
    }
    else
    {
      Warn("Tried to pop a singularly quoted string off a stream that doesn't start with one!");
    }
  }

  char Terminator[2] = {};
  Terminator[0] = '\'';

  counted_string Result = ReadUntilTerminatorList(Cursor, Terminator, True);
  if (IncludeQuotes)
  {
    --Result.Start;
    Result.Count += 2;
    Assert(Cursor->Start <= Result.Start);
    Assert(Result.Start+Result.Count <= Cursor->End);
  }

  return Result;
}

counted_string
PopQuotedString(ansi_stream* Cursor, b32 IncludeQuotes = False)
{
  if (*Cursor->At == '"' || *Cursor->At == '\'' )
  {
    Advance(Cursor);
  }
  else
  {
    if (Cursor->At-1 >= Cursor->Start)
    {
      Assert( *(Cursor->At-1) == '"' || *(Cursor->At-1) == '\'' );
    }
    else
    {
      Warn("Tried to pop a quoted string off a stream that doesn't start with one!");
    }
  }

  char Terminator[2] = {};
  Terminator[0] = *(Cursor->At-1);

  counted_string Result = ReadUntilTerminatorList(Cursor, Terminator, true);
  if (IncludeQuotes)
  {
    --Result.Start;
    Result.Count += 2;
    Assert(Cursor->Start <= Result.Start);
    Assert(Result.Start+Result.Count <= Cursor->End);
  }
  return Result;
}

char *
PopLine(ansi_stream *Cursor, memory_arena *Arena)
{
  char *Result = ReadUntilTerminatorList(Cursor, "\n", Arena);
  EatWhitespace(Cursor);
  return Result;
}

v3_cursor
ParseV3Array(u32 ElementCount, ansi_stream FloatStream, memory_arena* Memory)
{
  v3_cursor Result = V3Cursor(ElementCount, Memory);

  for (umm DestIndex = 0;
      DestIndex < ElementCount;
      ++DestIndex)
  {
    for (u32 Inner = 0;
         Inner < 3;
        ++Inner)
    {
      counted_string Float = PopWordCounted(&FloatStream);
      Result.Start[DestIndex].E[Inner] = StringToFloat(&Float);
    }
    ++Result.At;
  }

  return Result;
}

r32_stream
ParseFloatArray(u32 TotalFloatCount, ansi_stream FloatStream, memory_arena* Memory)
{
  r32_stream Result = R32_Stream(TotalFloatCount, Memory);

  for (umm DestIndex = 0;
      DestIndex < TotalFloatCount;
      ++DestIndex)
  {
    counted_string Float = PopWordCounted(&FloatStream);
    Result.Start[DestIndex] = StringToFloat(&Float);
  }

  return Result;
}

void
Dump(v3_cursor* Array)
{
  umm ElementCount = AtElements(Array);

  for (umm ElementIndex = 0;
      ElementIndex < ElementCount;
      ++ElementIndex)
  {
    Print(Array->Start[ElementIndex]);
  }

  return;
}

void
Dump(xml_hashtable * Hash)
{
  for (u32 ElementIndex = 0;
      ElementIndex < Hash->Size;
      ++ElementIndex)
  {
    xml_tag* Element = Hash->Table[ElementIndex];
    if (Element)
    {
      Print(ElementIndex);
      Print(Element);
      while ( (Element = Element->NextInHash) )
      {
        Print(Element);
      }
    }
  }

  return;
}

void
Dump(xml_token_stream *Stream, umm TokenCount)
{
  for (u32 TokenIndex = 0;
      TokenIndex < TokenCount;
      ++TokenIndex)
  {
    Print(Stream->Start + TokenIndex);
  }
}
