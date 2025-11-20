// callsite
// external/bonsai_stdlib/src/counted_string.h:885:0

// def (parse_integer)
// external/bonsai_stdlib/src/counted_string.h:856:0
link_internal b32
ParseInteger(cs String, u32 *Out)
{
  b32 Result = True;

  RangeIterator_t(umm, Index, String.Count)
  {
    if (IsNumeric(String.Start[Index]) == False)
    {
      Result = False;
      break;
    }
  }

  if (Result)
  {
    *Out = ToU32(String);
  }

  return Result;
}
link_internal b32
ParseInteger(cs String, u64 *Out)
{
  b32 Result = True;

  RangeIterator_t(umm, Index, String.Count)
  {
    if (IsNumeric(String.Start[Index]) == False)
    {
      Result = False;
      break;
    }
  }

  if (Result)
  {
    *Out = ToU64(String);
  }

  return Result;
}
link_internal b32
ParseInteger(cs String, s32 *Out)
{
  b32 Result = True;

  RangeIterator_t(umm, Index, String.Count)
  {
    if (IsNumeric(String.Start[Index]) == False)
    {
      Result = False;
      break;
    }
  }

  if (Result)
  {
    *Out = ToS32(String);
  }

  return Result;
}
link_internal b32
ParseInteger(cs String, s64 *Out)
{
  b32 Result = True;

  RangeIterator_t(umm, Index, String.Count)
  {
    if (IsNumeric(String.Start[Index]) == False)
    {
      Result = False;
      break;
    }
  }

  if (Result)
  {
    *Out = ToS64(String);
  }

  return Result;
}


