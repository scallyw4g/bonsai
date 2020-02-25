function void
Concat(string_builder* Builder, counted_string String)
{
  Push(&Builder->Chunks, String, &Builder->Memory);
}

function counted_string
Finalize(string_builder* Builder, memory_arena* PermMemory)
{
  u32 TotalMemRequired = 0;

  ITERATE_OVER(counted_string, &Builder->Chunks)
  {
    counted_string* At = GET_ELEMENT(Iter);
    TotalMemRequired += At->Count;
  }

  counted_string Result = CountedString(TotalMemRequired, PermMemory);

  u32 AtIndex = 0;

  ITERATE_OVER(counted_string, &Builder->Chunks)
  {
    counted_string* At = GET_ELEMENT(Iter);
    MemCopy((u8*)At->Start, (u8*)(Result.Start+AtIndex), At->Count);
    AtIndex += At->Count;
    Assert(AtIndex <= Result.Count);
  }

  // @temp-string-builder-memory
  // TODO(Jesse): Jettison the temp memory used by this thing!

  Assert(AtIndex == Result.Count);
  return Result;
}
