
meta(generate_stream)
struct counted_string
{
  umm Count;
  const char* Start; // TODO(Jesse): Make this non-const?
};

#include <metaprogramming/output/counted_string.h>

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

