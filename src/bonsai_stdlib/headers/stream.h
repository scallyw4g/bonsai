
template <typename element_t, typename stream_t>inline element_t *
Push(element_t Element, stream_t *Array)
{
  element_t *Result = Array->At;
  Assert( Array->At < Array->End );
  *Array->At++ = Element;
  return Result;
}

template <typename element_t, typename stream_t> inline element_t
Pop(stream_t *Stream)
{
  Assert(Remaining(Stream));
  element_t Element = *Stream->At++;
  return Element;
}

template <typename stream_t> inline umm
CurrentCount(stream_t* Stream)
{
  umm Result = (umm)(Stream->At - Stream->Start);
  return Result;
}

template <typename stream_t> inline umm
Count(stream_t* Stream)
{
  umm Result = (umm)(Stream->End - Stream->Start);
  return Result;
}

template <typename stream_t> inline void
TruncateToCurrentSize(stream_t* Stream)
{
  Assert(Stream->At <= Stream->End);
  Stream->End = Stream->At;
}

template <typename stream_t> inline void
Rewind(stream_t* Stream)
{
  Stream->At = Stream->Start;
}

ansi_stream
AnsiStream(counted_string* String, counted_string Filename = {})
{
  ansi_stream Result = {};

  Result.Start = String->Start;
  Result.At    = String->Start;
  Result.End   = String->Start + String->Count;

  Result.Filename = Filename;

  return Result;
}

ansi_stream
AnsiStream(counted_string String, counted_string Filename = {})
{
  ansi_stream Result = AnsiStream(&String, Filename);
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

void
EatAllCharacters(ansi_stream *Cursor, const char *Characters)
{
  while ( Remaining(Cursor) && Contains(Characters, *Cursor->At) )
  {
    ++Cursor->At;
  }
}

counted_string
ReadUntilTerminatorString(ansi_stream *Cursor, counted_string Terminator)
{
  const char *Start = Cursor->At;

  b32 Found = False;
  while ( (Remaining(Cursor) >= Terminator.Count) &&
          !Found &&
          *Cursor->At)
  {
    if (StringsMatch(CS(Cursor->At, Terminator.Count), Terminator ))
    {
      Found = True;
    }

    ++Cursor->At;
  }

  counted_string Result = {};
  if (Found)
  {
    Cursor->At -= Terminator.Count;
    Result.Start = Start;
    Result.Count = (umm)(Cursor->At - Start);
  }

  if (Remaining(Cursor) >= Terminator.Count)
  {
    Cursor->At += Terminator.Count;
  }
  else
  {
    Cursor->At = Cursor->End;
  }

  return Result;
}

function b32
Advance(ansi_stream* Cursor, u32 Lookahead = 0)
{
  b32 Result = False;
  if (Remaining(Cursor, Lookahead))
  {
    Cursor->At += Lookahead+1;
    Result = True;
  }
  else
  {
    Error("Tried advancing a cursor past its end!");
  }

  return Result;
}

counted_string
ReadUntilTerminatorList(ansi_stream *Cursor, const char *TerminatorList, b32 SkipEscapedChars = False)
{
  const char *Start = Cursor->At;

  b32 Done = False;
  while (!Done && Remaining(Cursor) && *Cursor->At)
  {
    const char *TerminatorAt = TerminatorList;
    while (!Done && *TerminatorAt)
    {
      if(*Cursor->At == *TerminatorAt)
      {
        Done = True;
      }
      ++TerminatorAt;
    }

    if (SkipEscapedChars && *Cursor->At == '\\' && Remaining(Cursor) )
    {
      Done = False;
      Advance(Cursor);
    }

    if (!Done)
    {
      Advance(Cursor);
    }
  }

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

function counted_string
CountedString(ansi_stream Stream)
{
  counted_string Result = {
    .Start = Stream.Start,
    .Count = (umm)(Stream.End-Stream.Start)
  };
  return Result;
}
