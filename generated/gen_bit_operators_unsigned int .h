link_internal u32
UnsetLeastSignificantSetBit(u32 *Input)
{
  u32 StartValue = *Input;
  u32 Cleared = StartValue & (StartValue - 1);
  *Input = Cleared;

  u32 Result = (StartValue & ~(Cleared));
  return Result;
}

link_internal u32
CountBitsSet_Kernighan(u32 Input)
{
  u32 Result = 0;
  while (Input != 0)
  {
    Input = Input & (Input - 1);
    Result++;
  }
  return Result;
}

link_internal u32
GetNthSetBit(u32 Target, u32 NBit)
{
  u32 Result = u32_MAX;
  for (u32 BitIndex = 0; BitIndex < NBit; ++BitIndex)
  {
    Result = UnsetLeastSignificantSetBit(&Target);
  }
  return Result;
}

