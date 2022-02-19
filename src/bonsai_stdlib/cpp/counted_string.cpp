
meta(buffer(counted_string))
#include <metaprogramming/output/buffer_counted_string.h>

meta(index_of(counted_string))
#include <metaprogramming/output/index_of_counted_string.h>


counted_string
CountedString(umm Count, memory_arena* Memory)
{
  TIMED_FUNCTION();
  counted_string Result = {
    .Count = Count,
    .Start = AllocateProtection(const char, Memory, Count, False),
  };
  return Result;
}

counted_string
CountedString(const char* Start, memory_arena* Memory)
{
  TIMED_FUNCTION();
  umm StringLength = Length(Start);

  counted_string Result = CountedString(StringLength, Memory);
  MemCopy((u8*)Start, (u8*)Result.Start, StringLength);

  return Result;
}

counted_string
CountedString(const char* Start, umm Count, memory_arena* Memory)
{
  TIMED_FUNCTION();
  counted_string Result = {
    .Count = Count,
    .Start = AllocateProtection(const char, Memory, Count, False),
  };

  MemCopy((u8*)Start, (u8*)Result.Start, Count);

  return Result;
}

bonsai_function counted_string
Concat(counted_string S1, counted_string S2, memory_arena* Memory)
{
  umm TotalLength = S1.Count + S2.Count;
  counted_string Result = {
    .Count = TotalLength,
    .Start = AllocateProtection(char, Memory, TotalLength, False),
  };

  MemCopy((u8*)S1.Start, (u8*)Result.Start, S1.Count);
  MemCopy((u8*)S2.Start, (u8*)Result.Start+S1.Count, S2.Count);

  return Result;
}

bonsai_function const char*
GetNullTerminated(counted_string Str, memory_arena* Memory = TranArena)
{
  const char* Result = AllocateProtection(const char, Memory, Str.Count+1, False);
  MemCopy((u8*)Str.Start, (u8*)Result, Str.Count);
  return Result;
}

bonsai_function void
ToCapitalCaseInplace(counted_string* Source)
{
  b32 NextCharToUpper = True;
  for (u32 CharIndex = 0;
      CharIndex < Source->Count;
      )
  {
    char* At = (char*)Source->Start + CharIndex;

    if (*At == '_')
    {
      NextCharToUpper = True;
      ++CharIndex;
      continue;
    }

    if (NextCharToUpper)
    {
      *At = ToUpper(*At);
      NextCharToUpper = False;
    }

    ++CharIndex;
  }
}

bonsai_function counted_string
ToCapitalCase(counted_string Source, memory_arena* Memory)
{
  u32 ResultLength = 0;
  for (u32 CharIndex = 0;
      CharIndex < Source.Count;
      ++CharIndex)
  {
    if (Source.Start[CharIndex] != '_')
    {
      ++ResultLength;
    }
  }

  counted_string Result = CountedString(ResultLength, Memory);

  b32 NextCharToUpper = True;
  u32 SourceIndex = 0;
  for (u32 ResultIndex = 0;
      ResultIndex < Result.Count;
      )
  {
    char At = Source.Start[SourceIndex];

    if (At == '_')
    {
      NextCharToUpper = True;
      ++SourceIndex;
      continue;
    }

    if (NextCharToUpper)
    {
      At = ToUpper(At);
      NextCharToUpper = False;
    }

    // TODO(Jesse, id: 52, tags: gross): Gross..
    *(char*)(Result.Start + ResultIndex) = At;
    ++SourceIndex;
    ++ResultIndex;
  }

  return Result;
}

bonsai_function void
PadForFormatWidth(char_cursor *Dest, u32 PadCount)
{
  {
    while (PadCount) { CopyToDest(Dest, ' '); --PadCount; }
  }
}

bonsai_function counted_string
FormatCountedString_(char_cursor* DestCursor, counted_string FS, va_list Args)
{
  TIMED_FUNCTION();

  char_cursor FormatCursor_ = {
    .Start = (char*)FS.Start,
    .At    = (char*)FS.Start,
    .End   = (char*)FS.Start + FS.Count
  };

  char_cursor* FormatCursor = &FormatCursor_;

  while (Remaining(FormatCursor))
  {
    char CursorAt = Advance(FormatCursor);

    if (CursorAt == '%')
    {
      u32 FormatWidth = 0;
      char FormatWidthPeek = Peek(FormatCursor);
      if (FormatWidthPeek == '*')
      {
        CursorAt = Advance(FormatCursor); // *
        CursorAt = Advance(FormatCursor); // Next
        FormatWidth = va_arg(Args, u32);
      }
      else if (IsNumeric(FormatWidthPeek))
      {
        FormatWidth = EmbeddedU32(FormatCursor);
        CursorAt = Advance(FormatCursor);
      }
      else
      {
        CursorAt = Advance(FormatCursor);
      }


      u32 FormatPrecision = 0;
      if (CursorAt == '.')
      {
        char FormatPrecisionPeek = Peek(FormatCursor);

        if (FormatPrecisionPeek == '*')
        {
          CursorAt = Advance(FormatCursor); // *
          CursorAt = Advance(FormatCursor); // Next
          FormatPrecision = va_arg(Args, u32);
        }
        else if (IsNumeric(FormatPrecisionPeek))
        {
          FormatPrecision = EmbeddedU32(FormatCursor);
          CursorAt = Advance(FormatCursor);
        }
        else
        {
          Error("Invalid dot specifier in format string");
        }
      }

      switch (CursorAt)
      {
        case 'd':
        {
          s32 Value = va_arg(Args, s32);
          s64ToChar(DestCursor, (s64)Value, FormatWidth);
        } break;

        case 'l':
        {
          CursorAt = Advance(FormatCursor);
          if (CursorAt == 'u')
          {
            u64 Value = va_arg(Args, u64);
            u64ToChar(DestCursor, Value, FormatWidth);

          }
          else if (CursorAt == 'd')
          {
            s64 Value = va_arg(Args, s64);
            s64ToChar(DestCursor, Value, FormatWidth);
          }
        } break;

        case 'x':
        {
          u64 Value = va_arg(Args, u64);
          // TODO(Jesse): Implement u64ToHex()
          Warn("Hex Values unsupported at current");
          u64ToChar(DestCursor, Value, FormatWidth);
        } break;

        case 'u':
        {
          u32 Value = va_arg(Args, u32);
          u64ToChar(DestCursor, (u64)Value, FormatWidth);
        } break;

        case 'c':
        {
          char Value = (char)va_arg(Args, s32);

          if (FormatWidth)
          {
            u32 PadCount = FormatWidth - 1;
            PadForFormatWidth(DestCursor, PadCount);
          }

          CopyToDest(DestCursor, Value);
        } break;

        case 's':
        {
          char* Value = va_arg(Args, char*);
          u32 ValueLen = FormatPrecision ? FormatPrecision : (u32)Length(Value);

          if (FormatWidth)
          {
            u32 PadCount = FormatWidth - ValueLen;
            PadForFormatWidth(DestCursor, PadCount);
          }

          u32 Count = 0;
          while (*Value)
          {
            CopyToDest(DestCursor, *Value++);
            if (FormatPrecision && ++Count == FormatPrecision)
            {
              break;
            }
          }
        } break;

        case 'f':
        {
          r64 Value = va_arg(Args, r64);
          f64ToChar(DestCursor, Value, FormatPrecision ? FormatPrecision : DEFAULT_FORMAT_PRECISION, FormatWidth );
        } break;

        case 'b':
        {
          b32 BoolVal = (b32)va_arg(Args, u32);
          BoolVal ?
            CopyToDest(DestCursor, 'T') :
            CopyToDest(DestCursor, 'F');
        } break;

        case 'S':
        {
          counted_string String = va_arg(Args, counted_string);

          if (FormatWidth)
          {
            PadForFormatWidth(DestCursor, FormatWidth - (u32)String.Count);
          }

          CopyToDest(DestCursor, String);

        } break;

        default:
        {
          va_arg(Args, void*);
          Error("Invalid Format String");
        } break;

      }
    }
    else
    {
      CopyToDest(DestCursor, CursorAt);
    }
  }

  umm DestElementsWritten = AtElements(DestCursor);
  counted_string Result = CS((const char*)DestCursor->Start, DestElementsWritten);
  return Result;
}

bonsai_function counted_string
FormatCountedString_(char_cursor* DestCursor, counted_string FS, ...)
{
  va_list Args;
  va_start(Args, FS);
  counted_string Result = FormatCountedString_(DestCursor, FS, Args);
  va_end(Args);

  return Result;
}

// This is to silence the warnings when passing counted_strings to this bonsai_function
#define FormatCountedString(Memory, Fmt, ...)             \
  _Pragma("clang diagnostic push")                        \
  _Pragma("clang diagnostic ignored \"-Wclass-varargs\"") \
  FormatCountedString_(Memory, Fmt, __VA_ARGS__)          \
  _Pragma("clang diagnostic pop")

bonsai_function counted_string
FormatCountedString_(memory_arena* Memory, counted_string FS, ...)
{
  TIMED_FUNCTION();

  umm FinalBufferStartingSize = FS.Count + 64;
  char* FinalBuffer = AllocateProtection(char, Memory, FinalBufferStartingSize, False);

  char_cursor DestCursor = {
    .Start = FinalBuffer,
    .At    = FinalBuffer,
    .End   = FinalBuffer + FinalBufferStartingSize,
    .Memory = Memory
  };

  va_list Args;
  va_start(Args, FS);
  counted_string Result = FormatCountedString_(&DestCursor, FS, Args);
  va_end(Args);

  return Result;
}

// TODO(Jesse, id: 364, tags: speed): This should probably go away and make sure we always just use counted strings
bonsai_function counted_string
FormatCountedString_(char* Buffer, umm BufferSize, const char *FS, va_list Args)
{
  TIMED_FUNCTION();

  char_cursor DestCursor = {
    .Start = Buffer,
    .At    = Buffer,
    .End   = Buffer + BufferSize,
  };
  counted_string Result = FormatCountedString_(&DestCursor, CS(FS), Args);
  return Result;
}

// TODO(Jesse, id: 365, tags: speed): This should probably go away and make sure we always just use counted strings
bonsai_function counted_string
FormatCountedString_(char* Buffer, umm BufferSize, const char *FS, ...)
{
  TIMED_FUNCTION();

  char_cursor DestCursor = {
    .Start = Buffer,
    .At    = Buffer,
    .End   = Buffer + BufferSize,
  };

  va_list Args;
  va_start(Args, FS);
  counted_string Result = FormatCountedString_(&DestCursor, CS(FS), Args);
  va_end(Args);

  return Result;
}

bonsai_function counted_string
FormatCountedString_(char* Buffer, umm BufferSize, counted_string FS, va_list Args)
{
  TIMED_FUNCTION();

  char_cursor DestCursor = {
    .Start = Buffer,
    .At    = Buffer,
    .End   = Buffer + BufferSize,
  };
  counted_string Result = FormatCountedString_(&DestCursor, FS, Args);
  return Result;
}

bonsai_function counted_string
FormatCountedString_(char* Buffer, umm BufferSize, counted_string FS, ...)
{
  TIMED_FUNCTION();

  char_cursor DestCursor = {
    .Start = Buffer,
    .At    = Buffer,
    .End   = Buffer + BufferSize,
  };

  va_list Args;
  va_start(Args, FS);
  counted_string Result = FormatCountedString_(&DestCursor, FS, Args);
  va_end(Args);

  return Result;
}

bonsai_function counted_string
MemorySize(r64 Number)
{
  r64 KB = (r64)Kilobytes(1);
  r64 MB = (r64)Megabytes(1);
  r64 GB = (r64)Gigabytes(1);

  r64 Display = Number;
  char Units = ' ';

  if (Number >= KB && Number < MB)
  {
    Display = Number / KB;
    Units = 'K';
  }
  else if (Number >= MB && Number < GB)
  {
    Display = Number / MB;
    Units = 'M';
  }
  else if (Number >= GB)
  {
    Display = Number / GB;
    Units = 'G';
  }


  counted_string Result = FormatCountedString(TranArena, CSz("%.1f%c"), (r32)Display, Units);
  return Result;
}

bonsai_function counted_string
MemorySize(u64 Number)
{
  counted_string Result = MemorySize((r64)Number);
  return Result;
}

bonsai_function counted_string
CS(s64 Number)
{
  counted_string Result = FormatCountedString(TranArena, CSz("%ld"), Number);
  return Result;
}

bonsai_function counted_string
CS(u64 Number)
{
  counted_string Result = FormatCountedString(TranArena, CSz("%lu"), Number);
  return Result;
}

bonsai_function counted_string
CS(s32 Number)
{
  counted_string Result = FormatCountedString(TranArena, CSz("%d"), Number);
  return Result;
}

bonsai_function counted_string
CS(u32 Number)
{
  counted_string Result = FormatCountedString(TranArena, CSz("%u"), Number);
  return Result;
}

bonsai_function counted_string
CS(r64 Number)
{
  counted_string Result = FormatCountedString(TranArena, CSz("%.2f"), Number);
  return Result;
}

bonsai_function counted_string
CS(r32 Number)
{
  counted_string Result = FormatCountedString(TranArena, CSz("%.2f"), Number);
  return Result;
}

bonsai_function counted_string
CS(v2 V)
{
  counted_string Result = FormatCountedString(TranArena, CSz("(%.2f,%.2f)"), V.x, V.y);
  return Result;
}

bonsai_function counted_string
FormatThousands(u64 Number)
{
  u64 OneThousand = 1000;
  r32 Display = (r32)Number;
  char Units = ' ';

  if (Number >= OneThousand)
  {
    Display = (r32)Number / (r32)OneThousand;
    Units = 'K';
  }

  counted_string Result = FormatCountedString(TranArena, CSz("%.1f%c"), Display, Units);
  return Result;
}

bonsai_function r32
ToF64(counted_string* String)
{
  const char* Temp = GetNullTerminated(*String, TranArena);
  r32 Result = (r32)atof(Temp);
  return Result;
}

