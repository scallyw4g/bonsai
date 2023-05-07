struct v3_cursor
{
  v3 *Start;
  // TODO(Jesse)(immediate): For the love of fucksakes change these to indices
  v3 *At;
  v3 *End;
};

link_internal v3_cursor
V3Cursor(umm ElementCount, memory_arena* Memory)
{
  v3 *Start = (v3*)PushStruct(Memory, sizeof(v3)*ElementCount, 1, 0);
  v3_cursor Result = {
    .Start = Start,
    .End = Start+ElementCount,
    .At = Start,
  };
  return Result;
}

link_internal v3
Get(v3_cursor *Cursor, umm ElementIndex)
{
  Assert(ElementIndex < CurrentCount(Cursor));
  v3 Result = Cursor->Start[ElementIndex];
  return Result;
}

