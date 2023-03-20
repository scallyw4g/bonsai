struct v3i_cursor
{
  v3i *Start;
  // TODO(Jesse)(immediate): For the love of fucksakes change these to indices
  v3i *At;
  v3i *End;
};

link_internal v3i_cursor
V3iCursor(umm ElementCount, memory_arena* Memory)
{
  v3i *Start = (v3i*)PushStruct(Memory, sizeof(v3i)*ElementCount, 1, 0);
  v3i_cursor Result = {
    .Start = Start,
    .End = Start+ElementCount,
    .At = Start,
  };
  return Result;
}

