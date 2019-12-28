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
