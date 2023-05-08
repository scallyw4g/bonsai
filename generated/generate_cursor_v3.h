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

link_internal void
Set(v3_cursor *Cursor, umm ElementIndex, v3 Element)
{
  umm CurrentElementCount = CurrentCount(Cursor);
  Assert (ElementIndex <= CurrentElementCount);

  Cursor->Start[ElementIndex] = Element;
  if (ElementIndex == CurrentElementCount)
  {
    Cursor->At++;
  }
}

link_internal v3 *
Push(v3_cursor *Cursor, v3 Element)
{
  Assert( Cursor->At < Cursor->End );
  v3 *Result = Cursor->At;
  *Cursor->At++ = Element;
  return Result;
}

link_internal v3
Pop(v3_cursor *Cursor)
{
  Assert( Cursor->At > Cursor->Start );
  v3 Result = Cursor->At[-1];
  Cursor->At--;
  return Result;
}

link_internal s32
LastIndex(v3_cursor *Cursor)
{
  s32 Result = s32(CurrentCount(Cursor))-1;
  return Result;
}

link_internal b32
Remove(v3_cursor *Cursor, v3 Query)
{
  b32 Result = False;
  CursorIterator(ElementIndex, Cursor)
  {
    v3 Element = Get(Cursor, ElementIndex);
    if (AreEqual(Element, Query))
    {
      b32 IsLastIndex = LastIndex(Cursor) == s32(ElementIndex);
      v3 Tmp = Pop(Cursor);

      if (IsLastIndex) { Assert(AreEqual(Tmp, Query)); }
      else { Set(Cursor, ElementIndex, Tmp); }
      Result = True;
    }
  }
  return Result;
}

