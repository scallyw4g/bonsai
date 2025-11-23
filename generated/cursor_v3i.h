// callsite
// external/bonsai_stdlib/src/vector.cpp:5:0

// def (generate_cursor)
// external/bonsai_stdlib/src/poof_functions.h:1569:0

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
  v3i_cursor Result = {};

  Result.Start = Start;
  Result.End = Start+ElementCount;
  Result.At = Start;

  return Result;
}

link_internal v3i*
GetPtr( v3i_cursor *Cursor, umm ElementIndex)
{
  /* ENSURE_OWNED_BY_THREAD(Cursor); */

  v3i *Result = {};
  if (ElementIndex < AtElements(Cursor)) { Result = Cursor->Start+ElementIndex; }
  return Result;
}

link_internal v3i*
TryGetPtr( v3i_cursor *Cursor, umm ElementIndex)
{
  return GetPtr(Cursor, ElementIndex);
}

link_internal v3i*
GetPtrUnsafe( v3i_cursor *Cursor, umm ElementIndex)
{
  /* ENSURE_OWNED_BY_THREAD(Cursor); */

  v3i *Result = {};
  if (ElementIndex < TotalElements(Cursor)) { Result = Cursor->Start+ElementIndex; }
  return Result;
}

link_internal v3i
Get( v3i_cursor *Cursor, umm ElementIndex)
{
  /* ENSURE_OWNED_BY_THREAD(Cursor); */

  Assert(ElementIndex < CurrentCount(Cursor));
  v3i Result = Cursor->Start[ElementIndex];
  return Result;
}

link_internal void
Set( v3i_cursor *Cursor, umm ElementIndex, v3i Element)
{
  /* ENSURE_OWNED_BY_THREAD(Cursor); */

  umm CurrentElementCount = CurrentCount(Cursor);
  Assert (ElementIndex <= CurrentElementCount);

  Cursor->Start[ElementIndex] = Element;
  if (ElementIndex == CurrentElementCount)
  {
    Cursor->At++;
  }
}

link_internal v3i*
Advance( v3i_cursor *Cursor)
{
  /* ENSURE_OWNED_BY_THREAD(Cursor); */

  v3i * Result = {};
  if ( Cursor->At < Cursor->End ) { Result = Cursor->At++; }
  return Result;
}

link_internal v3i *
Push( v3i_cursor *Cursor, v3i Element)
{
  /* ENSURE_OWNED_BY_THREAD(Cursor); */

  Assert( Cursor->At < Cursor->End );
  v3i *Result = Cursor->At;
  *Cursor->At++ = Element;
  return Result;
}

link_internal v3i
Pop( v3i_cursor *Cursor)
{
  /* ENSURE_OWNED_BY_THREAD(Cursor); */

  Assert( Cursor->At > Cursor->Start );
  v3i Result = Cursor->At[-1];
  Cursor->At--;
  return Result;
}

link_internal s32
LastIndex( v3i_cursor *Cursor)
{
  /* ENSURE_OWNED_BY_THREAD(Cursor); */

  s32 Result = s32(CurrentCount(Cursor))-1;
  return Result;
}

link_internal v3i*
LastElement( v3i_cursor *Cursor)
{
  v3i *Result = {};
  s32 I = LastIndex(Cursor);
  if (I > -1) { Result = Cursor->Start + I; }
  return Result;
}

link_internal b32
Remove( v3i_cursor *Cursor, v3i Query)
{
  /* ENSURE_OWNED_BY_THREAD(Cursor); */

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


link_internal b32
ResizeCursor( v3i_cursor *Cursor, umm Count, memory_arena *Memory)
{
  /* ENSURE_OWNED_BY_THREAD(Cursor); */

  umm CurrentSize = TotalSize(Cursor);

  TruncateToElementCount(Cursor, Count);
  umm NewSize = TotalSize(Cursor);

  Assert(NewSize/sizeof(v3i) == Count);

  /* Info("Attempting to reallocate CurrentSize(%u), NewSize(%u)", CurrentSize, NewSize); */
  Ensure(Reallocate((u8*)Cursor->Start, Memory, CurrentSize, NewSize));
  return 0;
}

link_internal void
Unshift( v3i_cursor *Cursor )
{
  /* ENSURE_OWNED_BY_THREAD(Cursor); */
  umm Count = AtElements(Cursor);
  if (Count)
  {
    for (umm Index = 1; Index < Count; ++Index)
    {
      Cursor->Start[Index-1] = Cursor->Start[Index];
    }

    // NOTE(Jesse): This is actually correct, even though it doesn't look
    // like it at first glance.  At is OnePastLast, so decrementing and
    // then clearing overwrites the last value that was set.
    Cursor->At--;
    *Cursor->At = {};
  }
}



