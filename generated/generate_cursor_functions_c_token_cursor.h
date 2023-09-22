link_internal c_token_cursor
CTokenCursor(umm ElementCount, memory_arena* Memory)
{
  c_token *Start = (c_token*)PushStruct(Memory, sizeof(c_token)*ElementCount, 1, 0);
  c_token_cursor Result = {
    .Start = Start,
    .End = Start+ElementCount,
    .At = Start,
  };
  return Result;
}

link_internal c_token*
GetPtr(c_token_cursor *Cursor, umm ElementIndex)
{
  c_token *Result = {};
  if (ElementIndex < AtElements(Cursor)) {
    Result = Cursor->Start+ElementIndex;
  }
  return Result;
}

link_internal c_token*
GetPtrUnsafe(c_token_cursor *Cursor, umm ElementIndex)
{
  c_token *Result = {};
  if (ElementIndex < TotalElements(Cursor)) {
    Result = Cursor->Start+ElementIndex;
  }
  return Result;
}

link_internal c_token
Get(c_token_cursor *Cursor, umm ElementIndex)
{
  Assert(ElementIndex < CurrentCount(Cursor));
  c_token Result = Cursor->Start[ElementIndex];
  return Result;
}

link_internal void
Set(c_token_cursor *Cursor, umm ElementIndex, c_token Element)
{
  umm CurrentElementCount = CurrentCount(Cursor);
  Assert (ElementIndex <= CurrentElementCount);

  Cursor->Start[ElementIndex] = Element;
  if (ElementIndex == CurrentElementCount)
  {
    Cursor->At++;
  }
}

link_internal c_token*
Advance(c_token_cursor *Cursor)
{
  c_token * Result = {};
  if ( Cursor->At < Cursor->End ) { Result = Cursor->At++; }
  return Result;
}

link_internal c_token *
Push(c_token_cursor *Cursor, c_token Element)
{
  Assert( Cursor->At < Cursor->End );
  c_token *Result = Cursor->At;
  *Cursor->At++ = Element;
  return Result;
}

link_internal c_token
Pop(c_token_cursor *Cursor)
{
  Assert( Cursor->At > Cursor->Start );
  c_token Result = Cursor->At[-1];
  Cursor->At--;
  return Result;
}

link_internal s32
LastIndex(c_token_cursor *Cursor)
{
  s32 Result = s32(CurrentCount(Cursor))-1;
  return Result;
}

link_internal b32
Remove(c_token_cursor *Cursor, c_token Query)
{
  b32 Result = False;
  CursorIterator(ElementIndex, Cursor)
  {
    c_token Element = Get(Cursor, ElementIndex);
    if (AreEqual(Element, Query))
    {
      b32 IsLastIndex = LastIndex(Cursor) == s32(ElementIndex);
      c_token Tmp = Pop(Cursor);

      if (IsLastIndex) { Assert(AreEqual(Tmp, Query)); }
      else { Set(Cursor, ElementIndex, Tmp); }
      Result = True;
    }
  }
  return Result;
}


link_internal b32
ResizeCursor(c_token_cursor *Cursor, umm Count, memory_arena *Memory)
{
  umm CurrentSize = TotalSize(Cursor);

  TruncateToElementCount(Cursor, Count);
  umm NewSize = TotalSize(Cursor);

  Assert(NewSize/sizeof(c_token) == Count);

  /* Info("Attempting to reallocate CurrentSize(%u), NewSize(%u)", CurrentSize, NewSize); */
  Ensure(Reallocate((u8*)Cursor->Start, Memory, CurrentSize, NewSize));
  return 0;
}


