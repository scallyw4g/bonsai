meta(stream_and_cursor(counted_string))
#include <metaprogramming/output/stream_and_cursor_counted_string.h>

struct tagged_counted_string_stream
{
  counted_string Tag;
  counted_string_stream Stream;
};
meta(generate_stream(tagged_counted_string_stream))
#include <metaprogramming/output/generate_stream_tagged_counted_string_stream.h>

// TODO(Jesse, id: 99, tags: compiler_feature, metaprogramming): Generate this?  Need a compiler feature to generate stuff from primitive types.
struct char_cursor
{
  char* Start;
  char* End;
  char* At;
  memory_arena* Memory; // TODO(Jesse, id: 100, tags: open_question, metaprogramming): Do we actually want this in here?
};

// Note(Jesse): Shamelessly copied from the Handmade Hero codebase
global_variable char DecChars[] = "0123456789";
global_variable char LowerHexChars[] = "0123456789abcdef";
global_variable char UpperHexChars[] = "0123456789ABCDEF";

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
CountedString(const char *S)
{
  counted_string Result = {};
  Result.Start = S;
  Result.Count = Length(S);
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

// TODO(Jesse, id: 101, tags: string_hash, profile, speed, hash, already_done_elsewhere, high_priority): Profile and check collision rate of this!
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
AreEqual(counted_string S1, counted_string S2)
{
  b32 Result = StringsMatch(&S1, &S2);
  return Result;
}

inline b32
Contains(counted_string S1, counted_string S2)
{
  b32 Result = False;

  if (S1.Count >= S2.Count)
  {
    u32 Diff = (u32)(S1.Count - S2.Count);
    for (u32 S1Index = 0;
        S1Index <= Diff;
        ++S1Index)
    {
      counted_string Temp1 = { .Start = S1.Start+S1Index, .Count = S2.Count };
      Assert(Temp1.Start+Temp1.Count <= S1.Start+S1.Count);
      Result = StringsMatch(&Temp1, &S2);
      if (Result) { return Result; }
    }
  }

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

function char
Advance(char_cursor* BufferCursor)
{
  char Result = 0;
  if (Remaining(BufferCursor))
  {
    Result = *BufferCursor->At++;
  }
  else
  {
    Error("Attempted to advance an empty char_cursor!");
  }
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

function b32
IsUpper(char C)
{
  b32 Result = (C >= 'A' && C <= 'Z');
  return Result;
}

function b32
IsLower(char C)
{
  b32 Result = (C >= 'a' && C <= 'z');
  return Result;
}

function b32
IsAlpha(char C)
{
  b32 Result = IsUpper(C) || IsLower(C);
  return Result;
}

function b32
IsHex(char C)
{
  b32 Result = ( (C >= '0' && C <= '9') ||
                 (C >= 'a' && C <= 'f') ||
                 (C >= 'A' && C <= 'F') );
  return Result;
}

function b32
IsNumeric(char C)
{
  b32 Result = (C >= '0' && C <= '9');
  return Result;
}

function b32
IsAlphaNumeric(char C)
{
  b32 Result = IsAlpha(C) || IsNumeric(C);
  return Result;
}

function u64
ToU64(char C)
{
  Assert(IsNumeric(C));
  u64 Result = (u64)C - (u64)'0';
  return Result;
}

function u32
ToU32(char C)
{
  u32 Result = SafeTruncateToU32(ToU64(C));
  return Result;
}

function r64
Exp(r64 Base, s32 Exponent)
{
  s32 IterationDirection = Exponent > 0 ? 1 : -1;

  r64 Result = 1;

  for (s32 Iterator = 0;
      Iterator != Exponent;
      Iterator += IterationDirection)
  {
    IterationDirection == 1 ?
      Result *= Base :
      Result /= Base;
  }

  return Result;
}

function u64
Exp(u64 Base, s32 Exponent)
{
  s32 IterationDirection = Exponent > 0 ? 1 : -1;

  u64 Result = 1;

  for (s32 Iterator = 0;
      Iterator != Exponent;
      Iterator += IterationDirection)
  {
    IterationDirection == 1 ?
      Result *= Base :
      Result /= Base;
  }

  return Result;
}

function u32
IsNumeric(counted_string S)
{
  u32 Result = True;
  for (u32 CharIndex = 0;
      CharIndex < S.Count;
      ++CharIndex)
  {
    Result &= IsNumeric(S.Start[CharIndex]);
  }

  return Result;
}

function u64
ToU64(counted_string S)
{
  u64 Result = 0;
  for (u64 CharIndex = 0;
      CharIndex < S.Count;
      ++CharIndex)
  {
    u64 Digit = ToU64(S.Start[CharIndex]);
    Result += (Digit * Exp(10UL, SafeTruncateToS32(S.Count - CharIndex - 1L) ));
  }

  return Result;
}

function s32
ToS32(counted_string S)
{
  s32 Result = SafeTruncateToS32(ToU64(S));
  return Result;
}

function u32
ToU32(counted_string S)
{
  u32 Result = SafeTruncateToU32(ToU64(S));
  return Result;
}

function char
Peek(char_cursor* BufferCursor)
{
  char Result = *BufferCursor->At;
  return Result;
}

function u32
EmbeddedU32(char_cursor* FormatCursor)
{
  char* NumStart = FormatCursor->At;
  Assert(IsNumeric(*NumStart));
  while(IsNumeric(Peek(FormatCursor))) { Advance(FormatCursor); }

  u32 CharCount = (u32)(FormatCursor->At - NumStart);
  counted_string NumberString = CS((const char*)NumStart, CharCount);
  u32 Result = ToU32(NumberString);
  return Result;
}

function b32
Reallocate(char_cursor *Dest, umm Increment)
{
  b32 Result = False;
  umm CurrentSize = TotalSize(Dest);
  if (Reallocate((u8*)Dest->Start, Dest->Memory, CurrentSize, CurrentSize+Increment))
  {
    Dest->End += Increment;
    Result = True;
  }

  return Result;
}

function void
CopyToDest(char_cursor *Dest, char C)
{
  b32 DoCopy = True;
  if (!Remaining(Dest))
  {
    if (Dest->Memory)
    {
      umm Increment = 32;
      if (Reallocate(Dest, Increment))
      {
        // Everyone's happy
      }
      else
      {
        // Reallocate already prints out an error for this case, no need to bother the user twice.
        DoCopy = False;
      }
    }
    else
    {
      Error("Unable to reallocate char_cursor; no memory pointer!");
      DoCopy = False;
    }
  }

  if (DoCopy)
  {
    *Dest->At++ = C;
  }
}

function void
u64ToChar(char_cursor* Dest, u64 Value, u32 Base = 10, char *Digits = DecChars)
{
  Assert(Base != 0);

  char *Start = Dest->At;
  do
  {
      u64 DigitIndex = (Value % Base);
      char Digit = Digits[DigitIndex];
      CopyToDest(Dest, Digit);
      Value /= Base;
  } while(Value != 0);

  char *End = Dest->At;

  while(Start < End)
  {
      --End;
      char Temp = *End;
      *End = *Start;
      *Start = Temp;
      ++Start;
  }

  return;
}

function void
s64ToChar(char_cursor* Dest, s64 Value, u32 Base = 10, char *Digits = DecChars)
{
  if (Value < 0)
  {
    CopyToDest(Dest, '-');
    Value = -Value;
  }

  u64ToChar(Dest, (u64)Value, Base, Digits);
  return;
}

// Note(Jesse): Shamelessly copied, then modified, from the Handmade Hero codebase
#define DEFAULT_FORMAT_PRECISION (16)
function void
f64ToChar(char_cursor* Dest, r64 Value, u32 Precision = DEFAULT_FORMAT_PRECISION)
{
  if(Value < 0)
  {
      CopyToDest(Dest, '-');
      Value = -Value;
  }

  u64 IntegerPart = (u64)Value;
  Value -= (r64)IntegerPart;
  u64ToChar(Dest, IntegerPart);

  CopyToDest(Dest, '.');

  // Note(casey): Note that this is NOT an accurate way to do this!
  for(u32 PrecisionIndex = 0;
      PrecisionIndex < Precision;
      ++PrecisionIndex)
  {
      Value *= 10.0f;
      u32 Int = (u32)Value;
      Value -= (r32)Int;
      CopyToDest(Dest, DecChars[Int]);
  }

  return;
}

