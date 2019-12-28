// TODO(Jesse): Profile and check collision rate of this!
// @optimize
inline umm
StringHash(const char* S1)
{
  umm Result = 0x4fa691fd6a2c2f69;
  while (*S1++)
  {
    umm Char = (umm)*S1;
    umm Mix = Char | (Char<<8) | (Char<<16) | (Char<<24) | (Char<<32) | (Char<<40) | (Char<<48) | (Char<<56);
    Result ^= ((Result<<32) | (Result>>32)) & Mix;
  }

  return Result;
}

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
