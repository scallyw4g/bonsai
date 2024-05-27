// external/bonsai_stdlib/src/counted_string.cpp:170:0

link_internal counted_string
Concat(cs S0, cs S1,  memory_arena* Memory, umm ExtraChars = 0)
{
  umm TotalLength = S0.Count +  S1.Count +  0;
  counted_string Result = {
    .Count = TotalLength + ExtraChars,
    .Start = AllocateProtection(char, Memory, TotalLength, False),
  };

  u64 At = 0;
  MemCopy((u8*)S0.Start, (u8*)Result.Start+At, S0.Count);
  At += S0.Count;
  MemCopy((u8*)S1.Start, (u8*)Result.Start+At, S1.Count);
  At += S1.Count;


  return Result;
}

