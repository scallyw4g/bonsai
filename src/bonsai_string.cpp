inline b32
StringsMatch(const char *S1, const char *S2)
{
  b32 Result = S1 && S2;
  while (Result && (*S1 || *S2))
  {
    Result &= (*S1++ == *S2++);
  }

  return Result;
}

inline b32
StringsMatch(counted_string* S1, counted_string* S2)
{
  b32 Result = True;

  if (S1 && S2 && S1->Count == S2->Count)
  {
    for (u32 CharIndex = 0;
        CharIndex < S1->Count;
        ++CharIndex)
    {
      Result = (Result && (S1->Start[CharIndex] == S2->Start[CharIndex]));
    }
  }
  else
  {
    Result = False;
  }

  return Result;
}

inline b32
StringsMatch(counted_string S1, counted_string* S2)
{
  b32 Result = StringsMatch(&S1, S2);
  return Result;
}

inline b32
StringsMatch(counted_string* S1, counted_string S2)
{
  b32 Result = StringsMatch(S1, &S2);
  return Result;
}

inline b32
StringsMatch(counted_string S1, counted_string S2)
{
  b32 Result = StringsMatch(&S1, &S2);
  return Result;
}

inline b32
Contains(const char *S1, const char *S2)
{
  const char *S1At = S1;
  while (*S1At)
  {
    const char *S2At = S2;

    b32 Result = (*S1At == *S2At);
    while (Result && *S1At && *S2At)
    {
      Result &= (*S1At++ == *S2At++);
    }

    if (Result && *S2At == 0)
    {
      return True;
    }
    else
    {
      ++S1At;
    }
  }

  return False;
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

void
Trim(counted_string* String)
{
  umm Count = String->Count;
  const char* Start = String->Start;

  for (umm CharIndex = 0;
      CharIndex < String->Count;
      ++CharIndex)
  {
    if (Start[CharIndex] == ' ' || Start[CharIndex] == '\n')
    {
      --Count;
      Start = Start + CharIndex + 1;
    }
    else
    {
      break;
    }
  }

  Assert(Count >= 0);

  for (s64 CharIndex = (s64)String->Count-1;
      CharIndex >= 0;
      --CharIndex)
  {
    if (String->Start[CharIndex] == ' ' || String->Start[CharIndex] == '\n')
    {
      --Count;
    }
    else
    {
      break;
    }
  }

  Assert(Count >= 0);
  String->Count = Count;
  String->Start = Start;

  return;
}

counted_string
Split(counted_string* String, char SplitTarget)
{
  counted_string Result = {};

  for (u32 CharIndex = 0;
      CharIndex < String->Count;
      ++CharIndex)
  {
    if (String->Start[CharIndex] == SplitTarget)
    {
      Result.Start = String->Start + CharIndex + 1;
      Result.Count = String->Count - CharIndex - 1;

      String->Count = CharIndex;
    }
  }

  return Result;
}
