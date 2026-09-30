// callsite
// external/bonsai_stdlib/src/counted_string.cpp:197:0

// def (string_concat)
// external/bonsai_stdlib/src/counted_string.cpp:158:0
link_internal counted_string
Concat( cs S0, cs S1, cs S2, cs S3, cs S4, cs S5, cs S6, cs S7, cs S8, cs S9,  memory_arena* Memory, umm ExtraChars = 0)
{
  umm TotalLength =   S0.Count +  S1.Count +  S2.Count +  S3.Count +  S4.Count +  S5.Count +  S6.Count +  S7.Count +  S8.Count +  S9.Count +  ExtraChars ;
  counted_string Result = {
    .Count = TotalLength,
    .Start = AllocateProtection(char, Memory, TotalLength, False),
  };

  u64 At = 0;
    MemCopy((u8*)S0.Start, (u8*)Result.Start+At, S0.Count);
  At += S0.Count;
  MemCopy((u8*)S1.Start, (u8*)Result.Start+At, S1.Count);
  At += S1.Count;
  MemCopy((u8*)S2.Start, (u8*)Result.Start+At, S2.Count);
  At += S2.Count;
  MemCopy((u8*)S3.Start, (u8*)Result.Start+At, S3.Count);
  At += S3.Count;
  MemCopy((u8*)S4.Start, (u8*)Result.Start+At, S4.Count);
  At += S4.Count;
  MemCopy((u8*)S5.Start, (u8*)Result.Start+At, S5.Count);
  At += S5.Count;
  MemCopy((u8*)S6.Start, (u8*)Result.Start+At, S6.Count);
  At += S6.Count;
  MemCopy((u8*)S7.Start, (u8*)Result.Start+At, S7.Count);
  At += S7.Count;
  MemCopy((u8*)S8.Start, (u8*)Result.Start+At, S8.Count);
  At += S8.Count;
  MemCopy((u8*)S9.Start, (u8*)Result.Start+At, S9.Count);
  At += S9.Count;


  return Result;
}

