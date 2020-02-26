counted_string
CountedString(umm Count, memory_arena* Memory)
{
  counted_string Result = {
    .Start = Allocate(const char, Memory, Count),
    .Count = Count
  };
  return Result;
}

counted_string
CountedString(const char* Start, memory_arena* Memory)
{
  umm StringLength = Length(Start);

  counted_string Result = CountedString(StringLength, Memory);
  MemCopy((u8*)Start, (u8*)Result.Start, StringLength);

  return Result;
}

counted_string
CountedString(const char* Start, umm Count, memory_arena* Memory)
{
  counted_string Result = {
    .Start = Allocate(const char, Memory, Count),
    .Count = Count
  };

  MemCopy((u8*)Start, (u8*)Result.Start, Count);

  return Result;
}

function void
Append(string_builder* Builder, counted_string String)
{
  Push(&Builder->Chunks, String, Builder->Memory);
}

function counted_string
Finalize(string_builder* Builder, memory_arena* PermMemory)
{
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

#define STRING_BUFFER_LENGTH 2048

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

#if 0
counted_string
FormatCountedString(memory_arena *Memory, const char* FormatString, ...)
{
  char *Buffer = AllocateProtection(char, Memory, STRING_BUFFER_LENGTH, False);

  va_list Arguments;
  va_start(Arguments, FormatString);
  vsnprintf(Buffer, STRING_BUFFER_LENGTH-1, FormatString, Arguments);
  va_end(Arguments);

  counted_string Result = CountedString(Buffer);
  return Result;
}
#endif

function counted_string
Concat(counted_string S1, counted_string S2, memory_arena* Memory)
{
  umm TotalLength = S1.Count + S2.Count;
  counted_string Result = {
    .Start = Allocate(char, Memory, TotalLength),
    .Count = TotalLength
  };

  MemCopy((u8*)S1.Start, (u8*)Result.Start, S1.Count);
  MemCopy((u8*)S2.Start, (u8*)Result.Start+S1.Count, S2.Count);

  return Result;
}

function const char*
GetNullTerminated(counted_string Str, memory_arena* Memory = TranArena)
{
  const char* Result = Allocate(const char, Memory, Str.Count+1);
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


  char *Buffer = Allocate(char, TranArena, 32);
  snprintf(Buffer, 32, "%.1f%c", (r32)Display, Units);
  return Buffer;
}

template<typename number_type>counted_string
NumericValueToString(number_type Number, const char* Format)
{
  u32 BufferLength = 32;
  char *Buffer = Allocate(char, TranArena, BufferLength);
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
  char *Buffer = Allocate(char, TranArena, 32);
  snprintf(Buffer, 32, "(%.2f,%.2f)", V.x, V.y);
  counted_string Result = CS(Buffer);
  return Result;
}

function char*
ToString(u64 Number)
{
  char *Buffer = Allocate(char, TranArena, 32);
  snprintf(Buffer, 32, "%lu", Number);
  return Buffer;
}

function char*
ToString(s32 Number)
{
  char *Buffer = Allocate(char, TranArena, 32);
  snprintf(Buffer, 32, "%i", Number);
  return Buffer;
}

function char*
ToString(u32 Number)
{
  char *Buffer = Allocate(char, TranArena, 32);
  snprintf(Buffer, 32, "%u", Number);
  return Buffer;
}

function char*
ToString(r32 Number)
{
  char *Buffer = Allocate(char, TranArena, 32);
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

  char *Buffer = Allocate(char, TranArena, 32);
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

  char *Buffer = Allocate(char, TranArena, 32);
  snprintf(Buffer, 32, "%.1f%c", Display, Units);

  return Buffer;
}


// This is to silence the warnings when passing counted_strings to this function
#define FormatCountedString(Memory, Fmt, ...)             \
  _Pragma("clang diagnostic push")                        \
  _Pragma("clang diagnostic ignored \"-Wclass-varargs\"") \
  FormatCountedString_(Memory, Fmt, __VA_ARGS__)          \
  _Pragma("clang diagnostic pop")


function counted_string
FormatCountedString_(memory_arena* Memory, const char* fmt...)
{
  string_builder Builder = {};

  va_list args;
  va_start(args, fmt);

  const char* StartPreceedingChars = fmt;
  const char* OnePastLast = fmt;
  while (*fmt != '\0')
  {
    if ( *fmt == '%' )
    {
      umm NumPreceedingChars = (umm)(OnePastLast - StartPreceedingChars);
      counted_string PreceedingNonFormatSpecifiers = CS(StartPreceedingChars, NumPreceedingChars);
      Append(&Builder, PreceedingNonFormatSpecifiers);
      ++fmt;

      if (*fmt == 'd')
      {
        s32 Value = va_arg(args, s32);
        Append(&Builder, CS(Value));
      }
      else if (*fmt == 'l')
      {
        ++fmt;
        if (*fmt == 'u')
        {
          u64 Value = va_arg(args, u64);
          Append(&Builder, CS(Value));
        }
        else if (*fmt == 'd')
        {
          s64 Value = va_arg(args, s64);
          Append(&Builder, CS(Value));
        }
      }
      else if (*fmt == 'x')
      {
        u64 Value = va_arg(args, u64);
        Append(&Builder, CS(Value));
      }
      else if (*fmt == 'u')
      {
        u32 Value = va_arg(args, u32);
        Append(&Builder, CS(Value));
      }
      else if (*fmt == 'c')
      {
        char Value = (char)va_arg(args, s32);
        Append(&Builder, CS(&Value, 1));
      }
      else if (*fmt == 's')
      {
        char* Value = va_arg(args, char*);
        counted_string CSValue = CountedString(Value, Memory);
        Append(&Builder, CSValue);
      }
      else if (*fmt == 'f')
      {
        r64 Value = va_arg(args, r64);
        Append(&Builder, CS(Value));
      }
      else if (*fmt == 'b')
      {
        b32 BoolVal = (b32)va_arg(args, u32);
        counted_string Output = {};

        if (BoolVal)
          Output = CS("True");
        else
          Output = CS("False");

        Append(&Builder, Output);
      }
      else if (*fmt == 'S')
      {
        u32 Count = va_arg(args, u32);
        char* Start = va_arg(args, char*);

        counted_string Output = {
          .Start = Start,
          .Count = Count
        };

        Append(&Builder, Output);
      }
      else if (*fmt == '.')
      {
        Assert(*(++fmt) == '*')
        Assert(*(++fmt) == 's')
        u32 Count = va_arg(args, u32);
        char* Start = va_arg(args, char*);

        counted_string Output = {
          .Start = Start,
          .Count = Count
        };

        Append(&Builder, Output);
      }
      else
      {
        va_arg(args, void*);
        Error("Invalid Format String");
      }

      ++fmt;
      StartPreceedingChars = fmt;
    }
    else
    {
      ++fmt;
    }

    OnePastLast = fmt;
  }

  umm NumPreceedingChars = (umm)(OnePastLast - StartPreceedingChars);
  counted_string PreceedingNonFormatSpecifiers = CS(StartPreceedingChars, NumPreceedingChars);
  Append(&Builder, PreceedingNonFormatSpecifiers);

  counted_string Result = Finalize(&Builder, Memory);

  va_end(args);

  return Result;
}
