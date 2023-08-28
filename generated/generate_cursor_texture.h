struct texture_cursor
{
  texture *Start;
  // TODO(Jesse)(immediate): For the love of fucksakes change these to indices
  texture *At;
  texture *End;
};

link_internal texture_cursor
TextureCursor(umm ElementCount, memory_arena* Memory)
{
  texture *Start = (texture*)PushStruct(Memory, sizeof(texture)*ElementCount, 1, 0);
  texture_cursor Result = {
    .Start = Start,
    .End = Start+ElementCount,
    .At = Start,
  };
  return Result;
}

link_internal texture*
GetPtr(texture_cursor *Cursor, umm ElementIndex)
{
  texture *Result = {};
  if (ElementIndex < AtElements(Cursor)) {
    Result = Cursor->Start+ElementIndex;
  }
  return Result;
}

link_internal texture
Get(texture_cursor *Cursor, umm ElementIndex)
{
  Assert(ElementIndex < CurrentCount(Cursor));
  texture Result = Cursor->Start[ElementIndex];
  return Result;
}

link_internal void
Set(texture_cursor *Cursor, umm ElementIndex, texture Element)
{
  umm CurrentElementCount = CurrentCount(Cursor);
  Assert (ElementIndex <= CurrentElementCount);

  Cursor->Start[ElementIndex] = Element;
  if (ElementIndex == CurrentElementCount)
  {
    Cursor->At++;
  }
}

link_internal texture *
Push(texture_cursor *Cursor, texture Element)
{
  Assert( Cursor->At < Cursor->End );
  texture *Result = Cursor->At;
  *Cursor->At++ = Element;
  return Result;
}

link_internal texture
Pop(texture_cursor *Cursor)
{
  Assert( Cursor->At > Cursor->Start );
  texture Result = Cursor->At[-1];
  Cursor->At--;
  return Result;
}

link_internal s32
LastIndex(texture_cursor *Cursor)
{
  s32 Result = s32(CurrentCount(Cursor))-1;
  return Result;
}

link_internal b32
Remove(texture_cursor *Cursor, texture Query)
{
  b32 Result = False;
  CursorIterator(ElementIndex, Cursor)
  {
    texture Element = Get(Cursor, ElementIndex);
    if (AreEqual(Element, Query))
    {
      b32 IsLastIndex = LastIndex(Cursor) == s32(ElementIndex);
      texture Tmp = Pop(Cursor);

      if (IsLastIndex) { Assert(AreEqual(Tmp, Query)); }
      else { Set(Cursor, ElementIndex, Tmp); }
      Result = True;
    }
  }
  return Result;
}

link_internal void
DeepCopy(texture_cursor *Src, texture_cursor *Dest)
{
  umm SrcAt = AtElements(Src);
  Assert(SrcAt <= TotalElements(Dest));

  IterateOver(Src, Element, ElementIndex)
  {
    DeepCopy(Element, Dest->Start+ElementIndex);
  }

  Dest->At = Dest->Start+SrcAt;
  Assert(Dest->At <= Dest->End);
}

