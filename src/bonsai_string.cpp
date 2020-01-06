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
GetNullTerminated(counted_string Str)
{
  const char* Result = Allocate(const char, TranArena, Str.Count+1);
  MemCopy((u8*)Str.Start, (u8*)Result, Str.Count);
  return Result;
}
