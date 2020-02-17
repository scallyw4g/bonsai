counted_string
CountedString(umm Count, memory_arena* Memory)
{
  counted_string Result = {
    .Start = Allocate(const char, Memory, Count),
    .Count = Count
  };
  return Result;
}

char *
FormatString(memory_arena *Memory, const char* FormatString, ...)
{
  char *Buffer = AllocateProtection(char, Memory, 1024, False);

  va_list Arguments;
  va_start(Arguments, FormatString);
  vsnprintf(Buffer, 1023, FormatString, Arguments);
  va_end(Arguments);

  return Buffer;
}

counted_string
FormatCountedString(memory_arena *Memory, const char* FormatString, ...)
{
  char *Buffer = AllocateProtection(char, Memory, 1024, False);

  va_list Arguments;
  va_start(Arguments, FormatString);
  vsnprintf(Buffer, 1023, FormatString, Arguments);
  va_end(Arguments);

  counted_string Result = CountedString(Buffer);
  return Result;
}

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

