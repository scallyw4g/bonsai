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

function char*
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


  char *Buffer = AllocateProtection(char, TranArena, 32, False);
  snprintf(Buffer, 32, "%.1f%c", (r32)Display, Units);
  return Buffer;
}

template<typename number_type>counted_string
NumericValueToString(number_type Number, const char* Format)
{
  u32 BufferLength = 32;
  char *Buffer = AllocateProtection(char, TranArena, BufferLength, False);
  snprintf(Buffer, BufferLength, Format, Number);

  counted_string Result = CS(Buffer);
  return Result;
}

function counted_string
CS(s64 Number)
{
  counted_string Result = NumericValueToString(Number, "%ld");
  return Result;
}

function counted_string
CS(u64 Number)
{
  counted_string Result = NumericValueToString(Number, "%lu");
  return Result;
}

function counted_string
CS(s32 Number)
{
  counted_string Result = NumericValueToString(Number, "%i");
  return Result;
}

function counted_string
CS(u32 Number)
{
  counted_string Result = NumericValueToString(Number, "%u");
  return Result;
}

function counted_string
CS(r64 Number)
{
  counted_string Result = NumericValueToString(Number, "%.2f");
  return Result;
}

function counted_string
CS(r32 Number)
{
  counted_string Result = NumericValueToString(Number, "%.2f");
  return Result;
}

function counted_string
CS(v2 V)
{
  char *Buffer = AllocateProtection(char, TranArena, 32, False);
  snprintf(Buffer, 32, "(%.2f,%.2f)", V.x, V.y);
  counted_string Result = CS(Buffer);
  return Result;
}

function char*
ToString(u64 Number)
{
  char *Buffer = AllocateProtection(char, TranArena, 32, False);
  snprintf(Buffer, 32, "%lu", Number);
  return Buffer;
}

function char*
ToString(s32 Number)
{
  char *Buffer = AllocateProtection(char, TranArena, 32, False);
  snprintf(Buffer, 32, "%i", Number);
  return Buffer;
}

function char*
ToString(u32 Number)
{
  char *Buffer = AllocateProtection(char, TranArena, 32, False);
  snprintf(Buffer, 32, "%u", Number);
  return Buffer;
}

function char*
ToString(r32 Number)
{
  char *Buffer = AllocateProtection(char, TranArena, 32, False);
  snprintf(Buffer, 32, "%.2f", Number);
  return Buffer;
}

function char*
FormatMemorySize(u64 Number)
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

  char *Buffer = AllocateProtection(char, TranArena, 32, False);
  snprintf(Buffer, 32, "%.1f%c", (r32)Display, Units);

  return Buffer;
}

function char*
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

  char *Buffer = AllocateProtection(char, TranArena, 32, False);
  snprintf(Buffer, 32, "%.1f%c", Display, Units);

  return Buffer;
}

// Note(Jesse): Shamelessly copied, then modified, from the Handmade Hero codebase
global_variable char DecChars[] = "0123456789";
global_variable char LowerHexChars[] = "0123456789abcdef";
global_variable char UpperHexChars[] = "0123456789ABCDEF";

// TODO(Jesse): Figure out how to not require a statically allocated buffer size
global_variable u32 MaxResultLength = 32;
function void
CopyToDest(char* Dest, u32* Count, char C)
{
  Dest[*Count] = C;
  *Count = *Count+1;
  Assert(*Count < MaxResultLength);
}

function u32
u64ToChar(char* Dest, u64 Value, u32 Base = 10, char *Digits = DecChars)
{
  TIMED_FUNCTION();
  Assert(Base != 0);

  u32 Count = 0;
  do
  {
      u64 DigitIndex = (Value % Base);
      char Digit = Digits[DigitIndex];

      CopyToDest(Dest, &Count, Digit);

      Value /= Base;

  } while(Value != 0);

  char *Start = (char*)Dest;
  char *End = (char*)Dest+Count;

  while(Start < End)
  {
      --End;
      char Temp = *End;
      *End = *Start;
      *Start = Temp;
      ++Start;
  }

  return Count;
}

function u32
s64ToChar(char* Dest, s64 Value, u32 Base = 10, char *Digits = DecChars)
{
  TIMED_FUNCTION();
  u32 Count = 0;

  if (Value < 0)
  {
    *Dest++ = '-';
    Count++;
    Value = -Value;
  }

  Count += u64ToChar(Dest, (u64)Value, Base, Digits);
  return Count;
}

// Note(Jesse): Shamelessly copied, then modified, from the Handmade Hero codebase
function u32
f64ToChar(char* Dest, r64 Value, u32 Precision = 16)
{
  TIMED_FUNCTION();

  u32 Count = 0;
  if(Value < 0)
  {
      CopyToDest(Dest, &Count, '-');
      Value = -Value;
  }

  u64 IntegerPart = (u64)Value;
  Value -= (r64)IntegerPart;
  Count += u64ToChar(Dest+Count, IntegerPart);

  CopyToDest(Dest, &Count, '.');

  // TODO(casey): Note that this is NOT an accurate way to do this!
  for(u32 PrecisionIndex = 0;
      PrecisionIndex < Precision;
      ++PrecisionIndex)
  {
      Value *= 10.0f;
      u32 Int = (u32)Value;
      Value -= (r32)Int;
      CopyToDest(Dest, &Count, DecChars[Int]);
  }

  return Count;
}

#define STRING_BUFFER_LENGTH 1024

char *
FormatString(memory_arena *Memory, const char* FormatString, ...)
{
  char *Buffer = AllocateProtection(char, Memory, STRING_BUFFER_LENGTH, False);

  va_list Arguments;
  va_start(Arguments, FormatString);
  vsnprintf(Buffer, STRING_BUFFER_LENGTH-1, FormatString, Arguments);
  va_end(Arguments);

  return Buffer;
}

// This is to silence the warnings when passing counted_strings to this function
#define FormatCountedString(Memory, Fmt, ...)             \
  _Pragma("clang diagnostic push")                        \
  _Pragma("clang diagnostic ignored \"-Wclass-varargs\"") \
  FormatCountedString_(Memory, Fmt, __VA_ARGS__)          \
  _Pragma("clang diagnostic pop")

#define CSz(NullTerminatedCString) \
  CS(NullTerminatedCString, sizeof(NullTerminatedCString)-1)

function counted_string
FormatCountedString_(memory_arena* Memory, counted_string FS, ...)
{
  TIMED_FUNCTION();

  va_list Args;
  va_start(Args, FS);

  u32 At = 0;

#define FINAL_BUFFER_SIZE (1024)
  char* FinalBuffer = AllocateProtection(char, Memory, FINAL_BUFFER_SIZE, False);

  for (u32 FormatIndex = 0;
      FormatIndex < FS.Count;
      ++FormatIndex)
  {
    if (FS.Start[FormatIndex] == '%')
    {
      ++FormatIndex;
      Assert(FormatIndex < FS.Count);

      switch (FS.Start[FormatIndex])
      {
        case 'd':
        {
          s32 Value = va_arg(Args, s32);
          At += s64ToChar(FinalBuffer+At, (s64)Value);
        } break;

        case 'l':
        {
          ++FormatIndex;
          Assert(FormatIndex < FS.Count);
          if (FS.Start[FormatIndex] == 'u')
          {
            u64 Value = va_arg(Args, u64);
            At += u64ToChar(FinalBuffer+At, Value);

          }
          else if (FS.Start[FormatIndex] == 'd')
          {
            s64 Value = va_arg(Args, s64);
            At += s64ToChar(FinalBuffer+At, Value);
          }
        } break;

        case 'x':
        {
          u64 Value = va_arg(Args, u64);
          At += u64ToChar(FinalBuffer+At, Value);
        } break;

        case 'u':
        {
          u32 Value = va_arg(Args, u32);
          At += u64ToChar(FinalBuffer+At, (u64)Value);
        } break;

        case 'c':
        {
          char Value = (char)va_arg(Args, s32);
          FinalBuffer[At++] = Value;
        } break;

        case 's':
        {
          char* Value = va_arg(Args, char*);
          while (*Value)
          {
            FinalBuffer[At++] = *Value;
          }
        } break;

        case 'f':
        {
          r64 Value = va_arg(Args, r64);
          At += f64ToChar(FinalBuffer+At, Value);
        } break;

        case 'b':
        {
          b32 BoolVal = (b32)va_arg(Args, u32);
          BoolVal ?
            FinalBuffer[At++] = 'T' :
            FinalBuffer[At++] = 'F';
        } break;

        case 'S':
        {
          counted_string String = va_arg(Args, counted_string);
          for (u32 CharIndex = 0;
              CharIndex < String.Count;
              ++CharIndex)
          {
            FinalBuffer[At++] = String.Start[CharIndex];
          }

        } break;

        case '.':
        {
          Assert(FS.Start[FormatIndex+1] == '*');
          Assert(FS.Start[FormatIndex+2] == 's');
          FormatIndex += 2;
          Assert(FormatIndex < FS.Count);

          umm Count = va_arg(Args, umm);
          char* Start = va_arg(Args, char*);
          for (u32 CharIndex = 0;
              CharIndex < Count;
              ++CharIndex)
          {
            FinalBuffer[At++] = Start[CharIndex];
          }

        } break;

        default:
        {
          va_arg(Args, void*);
          Error("Invalid Format String");
        } break;

      }

      Assert(FormatIndex < FS.Count);
    }
    else
    {
      FinalBuffer[At++] = FS.Start[FormatIndex];
    }

    Assert(At < FINAL_BUFFER_SIZE);
  }

  va_end(Args);

  counted_string Result = CountedString((const char*)FinalBuffer, At);
  return Result;
}
