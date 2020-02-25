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

#define TMP_BUFFER_SIZE (1024)
  char Temp[TMP_BUFFER_SIZE];

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
        s32 BytesWritten = snprintf(Temp, TMP_BUFFER_SIZE-1, "%d", Value);
        Assert(BytesWritten < TMP_BUFFER_SIZE);
        counted_string CSValue = CountedString(Temp, Memory);
        Append(&Builder, CSValue);
      }
      else if (*fmt == 'l')
      {
        ++fmt;
        if (*fmt == 'u')
        {
          u64 Value = va_arg(args, u64);
          s32 BytesWritten = snprintf(Temp, TMP_BUFFER_SIZE-1, "%lu", Value);
          Assert(BytesWritten < TMP_BUFFER_SIZE);
          counted_string CSValue = CountedString(Temp, Memory);
          Append(&Builder, CSValue);
        }
        else if (*fmt == 'd')
        {
          s64 Value = va_arg(args, s64);
          s32 BytesWritten = snprintf(Temp, TMP_BUFFER_SIZE-1, "%ld", Value);
          Assert(BytesWritten < TMP_BUFFER_SIZE);
          counted_string CSValue = CountedString(Temp, Memory);
          Append(&Builder, CSValue);
        }
      }
      else if (*fmt == 'x')
      {
        u64 Value = va_arg(args, u64);
        s32 BytesWritten = snprintf(Temp, TMP_BUFFER_SIZE-1, "%lu", Value);
        Assert(BytesWritten < TMP_BUFFER_SIZE);
        counted_string CSValue = CountedString(Temp, Memory);
        Append(&Builder, CSValue);
      }
      else if (*fmt == 'u')
      {
        u32 Value = va_arg(args, u32);
        s32 BytesWritten = snprintf(Temp, TMP_BUFFER_SIZE-1, "%u", Value);
        Assert(BytesWritten < TMP_BUFFER_SIZE);
        counted_string CSValue = CountedString(Temp, Memory);
        Append(&Builder, CSValue);
      }
      else if (*fmt == 'c')
      {
        s32 Value = va_arg(args, s32);
        s32 BytesWritten = snprintf(Temp, TMP_BUFFER_SIZE-1, "%c", Value);
        Assert(BytesWritten < TMP_BUFFER_SIZE);
        counted_string CSValue = CountedString(Temp, Memory);
        Append(&Builder, CSValue);
      }
      else if (*fmt == 's')
      {
        char* Value = va_arg(args, char*);
        s32 BytesWritten = snprintf(Temp, TMP_BUFFER_SIZE-1, "%s", Value);
        Assert(BytesWritten < TMP_BUFFER_SIZE);
        counted_string CSValue = CountedString(Temp, Memory);
        Append(&Builder, CSValue);
      }
      else if (*fmt == 'f')
      {
        r64 Value = va_arg(args, r64);
        s32 BytesWritten = snprintf(Temp, TMP_BUFFER_SIZE-1, "%f", Value);
        Assert(BytesWritten < TMP_BUFFER_SIZE);
        counted_string CSValue = CountedString(Temp, Memory);
        Append(&Builder, CSValue);
      }
      else if (*fmt == 'b')
      {
        b32 BoolVal = (b32)va_arg(args, int);
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
