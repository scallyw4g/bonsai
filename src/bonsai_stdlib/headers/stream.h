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

enum text_encoding
{
  TextEncoding_ASCII,

  TextEncoding_UTF8,
  TextEncoding_UTF16LE,
  TextEncoding_UTF16BE,

  TextEncoding_UTF32LE,
  TextEncoding_UTF32BE,

  TextEncoding_UTF7,
  TextEncoding_UTF1,

  TextEncoding_EBCDIC,
  TextEncoding_CSCU,
  TextEncoding_BOCU,
  TextEncoding_GB18030
};

meta(generate_string_table(text_encoding))
#include <metaprogramming/output/generate_string_table_text_encoding.h>

struct ansi_stream
{
  const char* Start;
  const char* At;
  const char* End;

  counted_string Filename;
  text_encoding Encoding;
};

struct r32_stream
{
  r32* Start;
  r32* At;
  r32* End;
};


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
TruncateToCurrentElements(stream_t* Stream)
{
  Assert(Stream->At <= Stream->End);
  Stream->End = Stream->At;
}

template <typename stream_t> inline void
Rewind(stream_t* Stream)
{
  Stream->At = Stream->Start;
}

template <typename stream_t> inline void
FastForward(stream_t* Stream)
{
  if (Stream->Start < Stream->End)
  {
    Stream->At = Stream->End -1;
  }
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

  if (Input->Start)
  {
    switch(*Input->Start)
    {
      case 239:
      {
        if (Count(Input) >= 3 &&
            Input->Start[1] == 187 &&
            Input->Start[2] == 191)
        {
          Assert(Count(&Result) == Count(Input));
          Result.At = Result.Start + 3;
          Result.Encoding = TextEncoding_UTF8;
        }
      } break;

      case 254:
      {
        if (Count(Input) >= 2 &&
            Input->Start[1] == 255)
        {
          Assert(Count(&Result) == Count(Input));
          Result.At = Result.Start + 2;
          Result.Encoding = TextEncoding_UTF16BE;
        }
      } break;

      case 255:
      {
        if (Count(Input) >= 4 &&
            Input->Start[1] == 254 &&
            Input->Start[2] == 0 &&
            Input->Start[3] == 0)
        {
          Assert(Count(&Result) == Count(Input));
          Result.At = Result.Start + 4;
          Result.Encoding = TextEncoding_UTF32LE;
        }
        else if (Count(Input) >= 2 &&
                 Input->Start[1] == 254)
        {
          Assert(Count(&Result) == Count(Input));
          Result.At = Result.Start + 2;
          Result.Encoding = TextEncoding_UTF16LE;
        }

      } break;

      case 0:
      {
        if (Count(Input) >= 4 &&
            Input->Start[1] == 0 &&
            Input->Start[2] == 254 &&
            Input->Start[3] == 255)
        {
          Assert(Count(&Result) == Count(Input));
          Result.At = Result.Start + 4;
          Result.Encoding = TextEncoding_UTF32BE;
        }
      } break;

      case 43:
      {
        if (Count(Input) >= 3 &&
            Input->Start[1] == 47 &&
            Input->Start[2] == 118)
            // TODO(Jesse, tags: robustness, text_encoding): Apparently the
            // next character (Input->Start[3]) is either ascii '8', '9', ':' or
            // ';' .. according to Wikipedia.  I'm putting an InvalidCodePath
            // in here so if we ever run into one of these in the wild we get a
            // chance to investigate fo-realz
            //
            // https://en.wikipedia.org/wiki/Byte_order_mark
        {
          InvalidCodePath();

          Assert(Count(&Result) == Count(Input));
          Result.At = Result.Start + 4;
          Result.Encoding = TextEncoding_UTF7;
        }
      } break;

      case 247:
      {
        if (Count(Input) >= 3 &&
            Input->Start[1] == 47 &&
            Input->Start[2] == 118)
        {
          Assert(Count(&Result) == Count(Input));
          Result.At = Result.Start + 3;
          Result.Encoding = TextEncoding_UTF1;
        }
      } break;

      case 221:
      {
        if (Count(Input) >= 4 &&
            Input->Start[1] == 115 &&
            Input->Start[2] == 102 &&
            Input->Start[3] == 115)
        {
          Assert(Count(&Result) == Count(Input));
          Result.At = Result.Start + 4;
          Result.Encoding = TextEncoding_EBCDIC;
        }
      } break;

      case 14:
      {
        if (Count(Input) >= 3 &&
            Input->Start[1] == 254 &&
            Input->Start[2] == 255)
        {
          Assert(Count(&Result) == Count(Input));
          Result.At = Result.Start + 3;
          Result.Encoding = TextEncoding_CSCU;
        }
      } break;

      case 251:
      {
        if (Count(Input) >= 3 &&
            Input->Start[1] == 238 &&
            Input->Start[2] == 40)
        {
          Assert(Count(&Result) == Count(Input));
          Result.At = Result.Start + 3;
          Result.Encoding = TextEncoding_BOCU;
        }
      } break;

      case 123:
      {
        if (Count(Input) >= 3 &&
            Input->Start[1] == 49 &&
            Input->Start[2] == 149 &&
            Input->Start[3] == 51)
        {
          Assert(Count(&Result) == Count(Input));
          Result.At = Result.Start + 3;
          Result.Encoding = TextEncoding_GB18030;
        }
      } break;

      default:
      {
        // Didn't get a BOM .. guess it's ascii, which is the default encoding
      } break;
    }
  }


  if (Result.Encoding == TextEncoding_UTF8 ||
      Result.Encoding == TextEncoding_ASCII)
  {
    // Fine, we expect these
  }
  else
  {
    /* Warn("Got a weird encoding (%S) for file (%S)", ToString(Result.Encoding), Result.Filename); */
  }


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

bonsai_function char
Advance(ansi_stream* Cursor, u32 Lookahead = 0)
{
  char Result = False;
  if (Remaining(Cursor, Lookahead))
  {
    Result = *Cursor->At;
    Cursor->At += Lookahead+1;
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
  EatAllCharacters(Cursor, "\r\n ");
  return;
}

counted_string
PopWordCounted(ansi_stream *Cursor, const char *Delimeters = 0)
{
  if (!Delimeters)
    Delimeters = " \r\n";

  EatWhitespace(Cursor);

  counted_string Result = ReadUntilTerminatorList(Cursor, Delimeters);
  return Result;
}

bonsai_function counted_string
CountedString(ansi_stream *Stream)
{
  counted_string Result = {
    .Count = (umm)(Stream->End-Stream->Start),
    .Start = Stream->Start,
  };
  return Result;
}

bonsai_function counted_string
CountedString(ansi_stream Stream)
{
  counted_string Result = {
    .Count = (umm)(Stream.End-Stream.Start),
    .Start = Stream.Start,
  };
  return Result;
}
