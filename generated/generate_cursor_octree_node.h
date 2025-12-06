// callsite
// src/engine/world.h:93:0

// def (generate_cursor)
// external/bonsai_stdlib/src/poof_functions.h:1569:0

struct octree_node_ptr_cursor
{
  octree_node_ptr *Start;
  // TODO(Jesse)(immediate): For the love of fucksakes change these to indices
  octree_node_ptr *At;
  octree_node_ptr *End;
};




link_internal octree_node_ptr_cursor
OctreeNodePtrCursor(umm ElementCount, memory_arena* Memory)
{
  octree_node_ptr *Start = (octree_node_ptr*)PushStruct(Memory, sizeof(octree_node_ptr)*ElementCount, 1, 0);
  octree_node_ptr_cursor Result = {};

  Result.Start = Start;
  Result.End = Start+ElementCount;
  Result.At = Start;

  return Result;
}

link_internal octree_node_ptr*
GetPtr( octree_node_ptr_cursor *Cursor, umm ElementIndex)
{
  /* ENSURE_OWNED_BY_THREAD(Cursor); */

  octree_node_ptr *Result = {};
  if (ElementIndex < AtElements(Cursor)) { Result = Cursor->Start+ElementIndex; }
  return Result;
}

link_internal octree_node_ptr*
TryGetPtr( octree_node_ptr_cursor *Cursor, umm ElementIndex)
{
  return GetPtr(Cursor, ElementIndex);
}

link_internal octree_node_ptr*
GetPtrUnsafe( octree_node_ptr_cursor *Cursor, umm ElementIndex)
{
  /* ENSURE_OWNED_BY_THREAD(Cursor); */

  octree_node_ptr *Result = {};
  if (ElementIndex < TotalElements(Cursor)) { Result = Cursor->Start+ElementIndex; }
  return Result;
}

link_internal octree_node_ptr
Get( octree_node_ptr_cursor *Cursor, umm ElementIndex)
{
  /* ENSURE_OWNED_BY_THREAD(Cursor); */

  Assert(ElementIndex < CurrentCount(Cursor));
  octree_node_ptr Result = Cursor->Start[ElementIndex];
  return Result;
}

link_internal void
Set( octree_node_ptr_cursor *Cursor, umm ElementIndex, octree_node_ptr Element)
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

link_internal octree_node_ptr*
Advance( octree_node_ptr_cursor *Cursor)
{
  /* ENSURE_OWNED_BY_THREAD(Cursor); */

  octree_node_ptr * Result = {};
  if ( Cursor->At < Cursor->End ) { Result = Cursor->At++; }
  return Result;
}

link_internal octree_node_ptr *
Push( octree_node_ptr_cursor *Cursor, octree_node_ptr Element)
{
  /* ENSURE_OWNED_BY_THREAD(Cursor); */

  Assert( Cursor->At < Cursor->End );
  octree_node_ptr *Result = Cursor->At;
  *Cursor->At++ = Element;
  return Result;
}

link_internal octree_node_ptr
Pop( octree_node_ptr_cursor *Cursor)
{
  /* ENSURE_OWNED_BY_THREAD(Cursor); */

  Assert( Cursor->At > Cursor->Start );
  octree_node_ptr Result = Cursor->At[-1];
  Cursor->At--;
  return Result;
}

link_internal s32
LastIndex( octree_node_ptr_cursor *Cursor)
{
  /* ENSURE_OWNED_BY_THREAD(Cursor); */

  s32 Result = s32(CurrentCount(Cursor))-1;
  return Result;
}

link_internal octree_node_ptr*
LastElement( octree_node_ptr_cursor *Cursor)
{
  octree_node_ptr *Result = {};
  s32 I = LastIndex(Cursor);
  if (I > -1) { Result = Cursor->Start + I; }
  return Result;
}

link_internal b32
Remove( octree_node_ptr_cursor *Cursor, octree_node_ptr Query)
{
  /* ENSURE_OWNED_BY_THREAD(Cursor); */

  b32 Result = False;
  CursorIterator(ElementIndex, Cursor)
  {
    octree_node_ptr Element = Get(Cursor, ElementIndex);
    if (AreEqual(Element, Query))
    {
      b32 IsLastIndex = LastIndex(Cursor) == s32(ElementIndex);
      octree_node_ptr Tmp = Pop(Cursor);

      if (IsLastIndex) { Assert(AreEqual(Tmp, Query)); }
      else { Set(Cursor, ElementIndex, Tmp); }
      Result = True;
    }
  }
  return Result;
}


link_internal b32
ResizeCursor( octree_node_ptr_cursor *Cursor, umm Count, memory_arena *Memory)
{
  /* ENSURE_OWNED_BY_THREAD(Cursor); */

  umm CurrentSize = TotalSize(Cursor);

  TruncateToElementCount(Cursor, Count);
  umm NewSize = TotalSize(Cursor);

  Assert(NewSize/sizeof(octree_node_ptr) == Count);

  /* Info("Attempting to reallocate CurrentSize(%u), NewSize(%u)", CurrentSize, NewSize); */
  Ensure(Reallocate((u8*)Cursor->Start, Memory, CurrentSize, NewSize));
  return 0;
}

link_internal void
Unshift( octree_node_ptr_cursor *Cursor )
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



