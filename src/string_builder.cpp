counted_string
CountedString(umm Count, memory_arena* Memory)
{
  TIMED_FUNCTION();
  counted_string Result = {
    .Start = AllocateProtection(const char, Memory, Count, False),
    .Count = Count
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
    .Start = AllocateProtection(const char, Memory, Count, False),
    .Count = Count
  };

  MemCopy((u8*)Start, (u8*)Result.Start, Count);

  return Result;
}

function void
Append(string_builder* Builder, counted_string String)
{
  TIMED_FUNCTION();
  Push(&Builder->Chunks, String, Builder->Memory);
}

function counted_string
Finalize(string_builder* Builder, memory_arena* PermMemory)
{
  TIMED_FUNCTION();
  u32 TotalMemRequired = 0;

  ITERATE_OVER(counted_string, &Builder->Chunks)
  {
    counted_string* At = GET_ELEMENT(Iter);
    TotalMemRequired += At->Count;
  }

  counted_string Result = CountedString(TotalMemRequired, PermMemory);

  u32 AtIndex = 0;

  ITERATE_OVER(counted_string, &Builder->Chunks)
  {
    counted_string* At = GET_ELEMENT(Iter);
    MemCopy((u8*)At->Start, (u8*)(Result.Start+AtIndex), At->Count);
    AtIndex += At->Count;
    Assert(AtIndex <= Result.Count);
  }
  Assert(AtIndex == Result.Count);

  VaporizeArena(Builder->Memory);
  return Result;
}

function counted_string
Concat(counted_string S1, counted_string S2, memory_arena* Memory)
{
  umm TotalLength = S1.Count + S2.Count;
  counted_string Result = {
    .Start = AllocateProtection(char, Memory, TotalLength, False),
    .Count = TotalLength
  };

  MemCopy((u8*)S1.Start, (u8*)Result.Start, S1.Count);
  MemCopy((u8*)S2.Start, (u8*)Result.Start+S1.Count, S2.Count);

  return Result;
}

function const char*
GetNullTerminated(counted_string Str, memory_arena* Memory = TranArena)
{
  const char* Result = AllocateProtection(const char, Memory, Str.Count+1, False);
  MemCopy((u8*)Str.Start, (u8*)Result, Str.Count);
  return Result;
}

function char
ToUpper(char C)
{
  u32 MapToUpper = 'a' - 'A';
  if (C >= 'a' && C <= 'z')
  {
    C -= MapToUpper;
  }
  return C;
}

function counted_string
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

    // TODO(Jesse): Gross..
    *(char*)(Result.Start + ResultIndex) = At;
    ++SourceIndex;
    ++ResultIndex;
  }

  return Result;
}

// TODO(Jesse): Generate this?  Need a compiler feature to generate stuff from primitive types.
// @compiler-feature
struct char_cursor
{
  char* Start;
  char* End;
  char* At;
  memory_arena* Memory; // TODO(Jesse): Do we actually want this in here?
};

function char
Peek(char_cursor* BufferCursor)
{
  char Result = *BufferCursor->At;
  return Result;
}

function char
Advance(char_cursor* BufferCursor)
{
  char Result = 0;
  if (Remaining(BufferCursor))
  {
    Result = *BufferCursor->At++;
  }
  else
  {
    Error("Attempted to advance an empty char_cursor!");
  }
  return Result;
}

function u8*
Reallocate(u8* Allocation, memory_arena* Arena, umm CurrentSize, umm RequestedSize)
{
  u8* Result = 0;
  if (Allocation + CurrentSize == Arena->At)
  {
    if (RequestedSize > CurrentSize)
    {
      umm Diff = RequestedSize - CurrentSize;
      if (Remaining(Arena) >= Diff)
      {
        Arena->At += Diff;
        Result = Allocation;
      }
      else
      {
        Error("Unable to reallocate : Arena didn't have enough space left to accommodate %lu bytes.", Diff);
      }
    }
    else
    {
      Error("Unable to reallocate : RequestedSize < CurrentSize");
    }
  }
  else
  {
    Error("Unable to reallocate : Allocation is not the final one on the arena, or the allocation was memprotected.");
  }

  return Result;
}

// Note(Jesse): Shamelessly copied, then modified, from the Handmade Hero codebase
global_variable char DecChars[] = "0123456789";
global_variable char LowerHexChars[] = "0123456789abcdef";
global_variable char UpperHexChars[] = "0123456789ABCDEF";

function void
CopyToDest(char_cursor* Dest, char C)
{
  b32 DoCopy = True;
  if (!Remaining(Dest))
  {
    if (Dest->Memory)
    {
      umm CurrentSize = TotalSize(Dest);
      umm Increment = 32;
      if (Reallocate((u8*)Dest->Start, Dest->Memory, CurrentSize, CurrentSize+Increment))
      {
        Dest->End += Increment;
      }
      else
      {
        Error("Reallocation Failed");
        DoCopy = False;
      }
    }
    else
    {
      Error("No memory pointer!");
      DoCopy = False;
    }
  }

  if (DoCopy)
  {
    *Dest->At++ = C;
  }
}

function void
u64ToChar(char_cursor* Dest, u64 Value, u32 Base = 10, char *Digits = DecChars)
{
  TIMED_FUNCTION();
  Assert(Base != 0);

  char *Start = Dest->At;
  do
  {
      u64 DigitIndex = (Value % Base);
      char Digit = Digits[DigitIndex];
      CopyToDest(Dest, Digit);
      Value /= Base;
  } while(Value != 0);

  char *End = Dest->At;

  while(Start < End)
  {
      --End;
      char Temp = *End;
      *End = *Start;
      *Start = Temp;
      ++Start;
  }

  return;
}

function void
s64ToChar(char_cursor* Dest, s64 Value, u32 Base = 10, char *Digits = DecChars)
{
  TIMED_FUNCTION();
  if (Value < 0)
  {
    CopyToDest(Dest, '-');
    Value = -Value;
  }

  u64ToChar(Dest, (u64)Value, Base, Digits);
  return;
}

// Note(Jesse): Shamelessly copied, then modified, from the Handmade Hero codebase
#define DEFAULT_FORMAT_PRECISION (16)
function void
f64ToChar(char_cursor* Dest, r64 Value, u32 Precision = DEFAULT_FORMAT_PRECISION)
{
  TIMED_FUNCTION();
  if(Value < 0)
  {
      CopyToDest(Dest, '-');
      Value = -Value;
  }

  u64 IntegerPart = (u64)Value;
  Value -= (r64)IntegerPart;
  u64ToChar(Dest, IntegerPart);

  CopyToDest(Dest, '.');

  // TODO(casey): Note that this is NOT an accurate way to do this!
  for(u32 PrecisionIndex = 0;
      PrecisionIndex < Precision;
      ++PrecisionIndex)
  {
      Value *= 10.0f;
      u32 Int = (u32)Value;
      Value -= (r32)Int;
      CopyToDest(Dest, DecChars[Int]);
  }

  return;
}

// This is to silence the warnings when passing counted_strings to this function
#define FormatCountedString(Memory, Fmt, ...)             \
  _Pragma("clang diagnostic push")                        \
  _Pragma("clang diagnostic ignored \"-Wclass-varargs\"") \
  FormatCountedString_(Memory, Fmt, __VA_ARGS__)          \
  _Pragma("clang diagnostic pop")

function b32
IsNumeric(char C)
{
  b32 Result = (C >= '0' && C <= '9');
  return Result;
}

function u32
ToU32(char C)
{
  Assert(IsNumeric(C));
  u32 Result = (u32)C - (u32)'0';
  return Result;
}

function u32
Exp(u32 Base, u32 Exponent)
{
  u32 Result = 1;
  for (u32 Ignored = 0;
      Ignored < Exponent;
      ++Ignored)
  {
    Result *= Base;
  }

  return Result;
}

function u32
ToU32(counted_string S)
{
  u32 Result = 0;
  for (u32 CharIndex = 0;
      CharIndex < S.Count;
      ++CharIndex)
  {
    u32 Digit = ToU32(S.Start[CharIndex]);
    Result += (Digit * Exp(10, ((u32)S.Count - CharIndex - 1)));
  }

  return Result;
}

function u32
EmbeddedU32(char_cursor* FormatCursor)
{
  char* NumStart = FormatCursor->At;
  Assert(IsNumeric(*NumStart));
  while(IsNumeric(Peek(FormatCursor))) { Advance(FormatCursor); }

  u32 CharCount = (u32)(FormatCursor->At - NumStart);
  counted_string NumberString = CS((const char*)NumStart, CharCount);
  u32 Result = ToU32(NumberString);
  return Result;
}

function counted_string
FormatCountedString_(memory_arena* Memory, counted_string FS, ...)
{
  TIMED_FUNCTION();

  va_list Args;
  va_start(Args, FS);

  umm FinalBufferStartingSize = FS.Count;
  char* FinalBuffer = AllocateProtection(char, Memory, FinalBufferStartingSize, False);

  char_cursor DestCursor_ = {
    .Start = FinalBuffer,
    .At    = FinalBuffer,
    .End   = FinalBuffer + FinalBufferStartingSize,
    .Memory = Memory
  };

  char_cursor FormatCursor_ = {
    .Start = (char*)FS.Start,
    .At    = (char*)FS.Start,
    .End   = (char*)FS.Start + FS.Count
  };


  char_cursor* FormatCursor = &FormatCursor_;
  char_cursor* DestCursor = &DestCursor_;

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
          s64ToChar(DestCursor, (s64)Value);
        } break;

        case 'l':
        {
          CursorAt = Advance(FormatCursor);
          if (CursorAt == 'u')
          {
            u64 Value = va_arg(Args, u64);
            u64ToChar(DestCursor, Value);

          }
          else if (CursorAt == 'd')
          {
            s64 Value = va_arg(Args, s64);
            s64ToChar(DestCursor, Value);
          }
        } break;

        case 'x':
        {
          u64 Value = va_arg(Args, u64);
          u64ToChar(DestCursor, Value);
        } break;

        case 'u':
        {
          u32 Value = va_arg(Args, u32);
          u64ToChar(DestCursor, (u64)Value);
        } break;

        case 'c':
        {
          char Value = (char)va_arg(Args, s32);

          if (FormatWidth)
          {
            u32 PadCount = FormatWidth - 1;
            while (PadCount) { CopyToDest(DestCursor, ' '); --PadCount; }
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
            while (PadCount) { CopyToDest(DestCursor, ' '); --PadCount; }
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
          f64ToChar(DestCursor, Value, FormatPrecision ? FormatPrecision : DEFAULT_FORMAT_PRECISION);
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
            u32 PadCount = FormatWidth - (u32)String.Count;
            while (PadCount) { CopyToDest(DestCursor, ' '); --PadCount; }
          }

          for (u32 CharIndex = 0;
              CharIndex < String.Count;
              ++CharIndex)
          {
            CopyToDest(DestCursor, String.Start[CharIndex]);
          }

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

  va_end(Args);

  umm DestElementsWritten = AtElements(DestCursor);
  counted_string Result = CS((const char*)DestCursor->Start, DestElementsWritten);
  return Result;
}

function counted_string
MemorySize(u64 Number)
{
  r64 KB = (r64)Kilobytes(1);
  r64 MB = (r64)Megabytes(1);
  r64 GB = (r64)Gigabytes(1);

  r64 Display = (r64)Number;
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

function counted_string
CS(s64 Number)
{
  counted_string Result = FormatCountedString(TranArena, CSz("%ld"), Number);
  return Result;
}

function counted_string
CS(u64 Number)
{
  counted_string Result = FormatCountedString(TranArena, CSz("%lu"), Number);
  return Result;
}

function counted_string
CS(s32 Number)
{
  counted_string Result = FormatCountedString(TranArena, CSz("%d"), Number);
  return Result;
}

function counted_string
CS(u32 Number)
{
  counted_string Result = FormatCountedString(TranArena, CSz("%u"), Number);
  return Result;
}

function counted_string
CS(r64 Number)
{
  counted_string Result = FormatCountedString(TranArena, CSz("%.2f"), Number);
  return Result;
}

function counted_string
CS(r32 Number)
{
  counted_string Result = FormatCountedString(TranArena, CSz("%.2f"), Number);
  return Result;
}

function counted_string
CS(v2 V)
{
  counted_string Result = FormatCountedString(TranArena, CSz("(%.2f,%.2f)"), V.x, V.y);
  return Result;
}

function counted_string
FormatThousands(u64 Number)
{
  u64 OneThousand = 1000;
  r32 Display = (r32)Number;
  char Units = ' ';

  if (Number >= OneThousand)
  {
    Display = Number / (r32)OneThousand;
    Units = 'K';
  }

  counted_string Result = FormatCountedString(TranArena, CSz("%.1f%c"), Display, Units);
  return Result;
}

function r32
StringToFloat(counted_string* String)
{
  const char* Temp = GetNullTerminated(*String, TranArena);
  r32 Result = (r32)atof(Temp);
  return Result;
}

function s32
StringToInt(counted_string String)
{
  const char* Temp = GetNullTerminated(String, TranArena);
  s32 Result = atoi(Temp);
  return Result;
}

function s32
StringToInt(counted_string* String)
{
  s32 Result = StringToInt(*String);
  return Result;
}

function u32
StringToUInt(counted_string *String)
{
  u32 Result = (u32)StringToInt(String);
  return Result;
}

