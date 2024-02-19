// external/bonsai_debug/src/api.h:114:0

struct debug_profile_scope_ptr_cursor
{
  debug_profile_scope_ptr *Start;
  // TODO(Jesse)(immediate): For the love of fucksakes change these to indices
  debug_profile_scope_ptr *At;
  debug_profile_scope_ptr *End;
  /* OWNED_BY_THREAD_MEMBER(); */
};



link_internal debug_profile_scope_ptr_cursor
DebugProfileScopePtrCursor(umm ElementCount, memory_arena* Memory)
{
  debug_profile_scope_ptr *Start = (debug_profile_scope_ptr*)PushStruct(Memory, sizeof(debug_profile_scope_ptr)*ElementCount, 1, 0);
  debug_profile_scope_ptr_cursor Result = {
    .Start = Start,
    .End = Start+ElementCount,
    .At = Start,
    /* OWNED_BY_THREAD_MEMBER_INIT() */
  };
  return Result;
}

link_internal debug_profile_scope_ptr*
GetPtr(debug_profile_scope_ptr_cursor *Cursor, umm ElementIndex)
{
  /* ENSURE_OWNED_BY_THREAD(Cursor); */

  debug_profile_scope_ptr *Result = {};
  if (ElementIndex < AtElements(Cursor)) { Result = Cursor->Start+ElementIndex; }
  return Result;
}

link_internal debug_profile_scope_ptr*
GetPtrUnsafe(debug_profile_scope_ptr_cursor *Cursor, umm ElementIndex)
{
  /* ENSURE_OWNED_BY_THREAD(Cursor); */

  debug_profile_scope_ptr *Result = {};
  if (ElementIndex < TotalElements(Cursor)) { Result = Cursor->Start+ElementIndex; }
  return Result;
}

link_internal debug_profile_scope_ptr
Get(debug_profile_scope_ptr_cursor *Cursor, umm ElementIndex)
{
  /* ENSURE_OWNED_BY_THREAD(Cursor); */

  Assert(ElementIndex < CurrentCount(Cursor));
  debug_profile_scope_ptr Result = Cursor->Start[ElementIndex];
  return Result;
}

link_internal void
Set(debug_profile_scope_ptr_cursor *Cursor, umm ElementIndex, debug_profile_scope_ptr Element)
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

link_internal debug_profile_scope_ptr*
Advance(debug_profile_scope_ptr_cursor *Cursor)
{
  /* ENSURE_OWNED_BY_THREAD(Cursor); */

  debug_profile_scope_ptr * Result = {};
  if ( Cursor->At < Cursor->End ) { Result = Cursor->At++; }
  return Result;
}

link_internal debug_profile_scope_ptr *
Push(debug_profile_scope_ptr_cursor *Cursor, debug_profile_scope_ptr Element)
{
  /* ENSURE_OWNED_BY_THREAD(Cursor); */

  Assert( Cursor->At < Cursor->End );
  debug_profile_scope_ptr *Result = Cursor->At;
  *Cursor->At++ = Element;
  return Result;
}

link_internal debug_profile_scope_ptr
Pop(debug_profile_scope_ptr_cursor *Cursor)
{
  /* ENSURE_OWNED_BY_THREAD(Cursor); */

  Assert( Cursor->At > Cursor->Start );
  debug_profile_scope_ptr Result = Cursor->At[-1];
  Cursor->At--;
  return Result;
}

link_internal s32
LastIndex(debug_profile_scope_ptr_cursor *Cursor)
{
  /* ENSURE_OWNED_BY_THREAD(Cursor); */

  s32 Result = s32(CurrentCount(Cursor))-1;
  return Result;
}

link_internal debug_profile_scope_ptr*
LastElement(debug_profile_scope_ptr_cursor *Cursor)
{
  debug_profile_scope_ptr *Result = {};
  s32 I = LastIndex(Cursor);
  if (I > -1) { Result = Cursor->Start + I; }
  return Result;
}

link_internal b32
Remove(debug_profile_scope_ptr_cursor *Cursor, debug_profile_scope_ptr Query)
{
  /* ENSURE_OWNED_BY_THREAD(Cursor); */

  b32 Result = False;
  CursorIterator(ElementIndex, Cursor)
  {
    debug_profile_scope_ptr Element = Get(Cursor, ElementIndex);
    if (AreEqual(Element, Query))
    {
      b32 IsLastIndex = LastIndex(Cursor) == s32(ElementIndex);
      debug_profile_scope_ptr Tmp = Pop(Cursor);

      if (IsLastIndex) { Assert(AreEqual(Tmp, Query)); }
      else { Set(Cursor, ElementIndex, Tmp); }
      Result = True;
    }
  }
  return Result;
}


link_internal b32
ResizeCursor(debug_profile_scope_ptr_cursor *Cursor, umm Count, memory_arena *Memory)
{
  /* ENSURE_OWNED_BY_THREAD(Cursor); */

  umm CurrentSize = TotalSize(Cursor);

  TruncateToElementCount(Cursor, Count);
  umm NewSize = TotalSize(Cursor);

  Assert(NewSize/sizeof(debug_profile_scope_ptr) == Count);

  /* Info("Attempting to reallocate CurrentSize(%u), NewSize(%u)", CurrentSize, NewSize); */
  Ensure(Reallocate((u8*)Cursor->Start, Memory, CurrentSize, NewSize));
  return 0;
}

link_internal void
Unshift( debug_profile_scope_ptr_cursor *Cursor )
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



