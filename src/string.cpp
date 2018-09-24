
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

char *
FormatString(const char* FormatString, ...)
{
  char *Buffer = AllocateProtection(char, TranArena, 1024, False);

  va_list Arguments;
  va_start(Arguments, FormatString);
  vsnprintf(Buffer, 1023, FormatString, Arguments);
  va_end(Arguments);

  return Buffer;
}

