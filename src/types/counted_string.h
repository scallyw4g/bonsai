struct counted_string
{
  umm Count;
  const char* Start; // TODO(Jesse): Make this non-const?
};
meta(generate_stream(counted_string))
#include <metaprogramming/output/generate_stream_counted_string>
meta(generate_cursor(counted_string))
#include <metaprogramming/output/generate_cursor_counted_string>

umm
Length(const char *Str)
{
  const char *Start = Str;
  const char *End = Str;

  while (*End++);

  umm Result = (umm)(End - Start) - 1;
  return Result;
}

counted_string
CountedString(const char *S, umm Count)
{
  counted_string Result = {
    .Start = S, .Count = Count
  };
  return Result;
}

counted_string
CountedString(const char *S)
{
  counted_string Result = {};
  Result.Start = S;
  Result.Count = Length(S);
  return Result;
}

counted_string
CS(const char *S, umm Count)
{
  counted_string Result = CountedString(S, Count);
  return Result;
}

counted_string
CS(const char *S)
{
  counted_string Result = CountedString(S);
  return Result;
}

b32
Contains(counted_string Haystack, char Needle)
{
  b32 Result = False;
  for (u32 Index = 0;
      Index < Haystack.Count;
      ++Index)
  {
    Result |= (Needle == Haystack.Start[Index]);
  }
  return Result;
}

b32
Contains(const char* Haystack, char Needle)
{
  b32 Result = Contains(CS(Haystack), Needle);
  return Result;
}

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

function b32
IsPathSeparator(char C)
{
  b32 Result = C == '/';
  return Result;
}

function counted_string
StripExtension(counted_string FilePath)
{
  counted_string Result = FilePath;

  for (umm CharIndex = 0;
      CharIndex < Result.Count;
      ++CharIndex)
  {
    if (Result.Start[CharIndex] == '.')
    {
      Result.Count = CharIndex;
      break;
    }
  }

  return Result;
}

function counted_string
Basename(counted_string FilePath)
{
  umm LastPathSeparator = 0;
  for (umm CharIndex = 0;
      CharIndex < FilePath.Count;
      ++CharIndex)
  {
    if (IsPathSeparator(FilePath.Start[CharIndex]))
    {
      LastPathSeparator = CharIndex;
    }
  }
  counted_string Result = {
    .Start = FilePath.Start + LastPathSeparator,
    .Count = FilePath.Count - LastPathSeparator
  };

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

function counted_string
Trim(counted_string String)
{
  counted_string Result = String;
  while (Result.Count)
  {
    if (Result.Start[0] == ' ' || Result.Start[0] == '\n')
    {
      --Result.Count;
      ++Result.Start;
    }
    else
    {
      break;
    }
  }

  while (Result.Count)
  {
    if (Result.Start[Result.Count-1] == ' ' || Result.Start[Result.Count-1] == '\n')
    {
      --Result.Count;
    }
    else
    {
      break;
    }
  }

  return Result;
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
