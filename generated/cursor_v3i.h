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

link_internal v3i
Get(v3i_cursor *Cursor, umm ElementIndex)
{
  Assert(ElementIndex < CurrentCount(Cursor));
  v3i Result = Cursor->Start[ElementIndex];
  return Result;
}

link_internal void
Set(v3i_cursor *Cursor, umm ElementIndex, v3i Element)
{
  umm CurrentElementCount = CurrentCount(Cursor);
  Assert (ElementIndex <= CurrentElementCount);

  Cursor->Start[ElementIndex] = Element;
  if (ElementIndex == CurrentElementCount)
  {
    Cursor->At++;
  }
}

link_internal v3i *
Push(v3i_cursor *Cursor, v3i Element)
{
  Assert( Cursor->At < Cursor->End );
  v3i *Result = Cursor->At;
  *Cursor->At++ = Element;
  return Result;
}

link_internal v3i
Pop(v3i_cursor *Cursor)
{
  Assert( Cursor->At > Cursor->Start );
  v3i Result = Cursor->At[-1];
  Cursor->At--;
  return Result;
}

link_internal s32
LastIndex(v3i_cursor *Cursor)
{
  s32 Result = s32(CurrentCount(Cursor))-1;
  return Result;
}

link_internal b32
Remove(v3i_cursor *Cursor, v3i Query)
{
  b32 Result = False;
  CursorIterator(ElementIndex, Cursor)
  {
    v3i Element = Get(Cursor, ElementIndex);
    if (AreEqual(Element, Query))
    {
      b32 IsLastIndex = LastIndex(Cursor) == s32(ElementIndex);
      v3i Tmp = Pop(Cursor);

      if (IsLastIndex) { Assert(AreEqual(Tmp, Query)); }
      else { Set(Cursor, ElementIndex, Tmp); }
      Result = True;
    }
  }
  return Result;
}

