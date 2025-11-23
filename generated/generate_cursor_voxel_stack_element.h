// callsite
// src/engine/world_update.h:21:0

// def (generate_cursor)
// external/bonsai_stdlib/src/poof_functions.h:1569:0

struct voxel_stack_element_cursor
{
  voxel_stack_element *Start;
  // TODO(Jesse)(immediate): For the love of fucksakes change these to indices
  voxel_stack_element *At;
  voxel_stack_element *End;
};




link_internal voxel_stack_element_cursor
VoxelStackElementCursor(umm ElementCount, memory_arena* Memory)
{
  voxel_stack_element *Start = (voxel_stack_element*)PushStruct(Memory, sizeof(voxel_stack_element)*ElementCount, 1, 0);
  voxel_stack_element_cursor Result = {};

  Result.Start = Start;
  Result.End = Start+ElementCount;
  Result.At = Start;

  return Result;
}

link_internal voxel_stack_element*
GetPtr( voxel_stack_element_cursor *Cursor, umm ElementIndex)
{
  /* ENSURE_OWNED_BY_THREAD(Cursor); */

  voxel_stack_element *Result = {};
  if (ElementIndex < AtElements(Cursor)) { Result = Cursor->Start+ElementIndex; }
  return Result;
}

link_internal voxel_stack_element*
TryGetPtr( voxel_stack_element_cursor *Cursor, umm ElementIndex)
{
  return GetPtr(Cursor, ElementIndex);
}

link_internal voxel_stack_element*
GetPtrUnsafe( voxel_stack_element_cursor *Cursor, umm ElementIndex)
{
  /* ENSURE_OWNED_BY_THREAD(Cursor); */

  voxel_stack_element *Result = {};
  if (ElementIndex < TotalElements(Cursor)) { Result = Cursor->Start+ElementIndex; }
  return Result;
}

link_internal voxel_stack_element
Get( voxel_stack_element_cursor *Cursor, umm ElementIndex)
{
  /* ENSURE_OWNED_BY_THREAD(Cursor); */

  Assert(ElementIndex < CurrentCount(Cursor));
  voxel_stack_element Result = Cursor->Start[ElementIndex];
  return Result;
}

link_internal void
Set( voxel_stack_element_cursor *Cursor, umm ElementIndex, voxel_stack_element Element)
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

link_internal voxel_stack_element*
Advance( voxel_stack_element_cursor *Cursor)
{
  /* ENSURE_OWNED_BY_THREAD(Cursor); */

  voxel_stack_element * Result = {};
  if ( Cursor->At < Cursor->End ) { Result = Cursor->At++; }
  return Result;
}

link_internal voxel_stack_element *
Push( voxel_stack_element_cursor *Cursor, voxel_stack_element Element)
{
  /* ENSURE_OWNED_BY_THREAD(Cursor); */

  Assert( Cursor->At < Cursor->End );
  voxel_stack_element *Result = Cursor->At;
  *Cursor->At++ = Element;
  return Result;
}

link_internal voxel_stack_element
Pop( voxel_stack_element_cursor *Cursor)
{
  /* ENSURE_OWNED_BY_THREAD(Cursor); */

  Assert( Cursor->At > Cursor->Start );
  voxel_stack_element Result = Cursor->At[-1];
  Cursor->At--;
  return Result;
}

link_internal s32
LastIndex( voxel_stack_element_cursor *Cursor)
{
  /* ENSURE_OWNED_BY_THREAD(Cursor); */

  s32 Result = s32(CurrentCount(Cursor))-1;
  return Result;
}

link_internal voxel_stack_element*
LastElement( voxel_stack_element_cursor *Cursor)
{
  voxel_stack_element *Result = {};
  s32 I = LastIndex(Cursor);
  if (I > -1) { Result = Cursor->Start + I; }
  return Result;
}

link_internal b32
Remove( voxel_stack_element_cursor *Cursor, voxel_stack_element Query)
{
  /* ENSURE_OWNED_BY_THREAD(Cursor); */

  b32 Result = False;
  CursorIterator(ElementIndex, Cursor)
  {
    voxel_stack_element Element = Get(Cursor, ElementIndex);
    if (AreEqual(Element, Query))
    {
      b32 IsLastIndex = LastIndex(Cursor) == s32(ElementIndex);
      voxel_stack_element Tmp = Pop(Cursor);

      if (IsLastIndex) { Assert(AreEqual(Tmp, Query)); }
      else { Set(Cursor, ElementIndex, Tmp); }
      Result = True;
    }
  }
  return Result;
}


link_internal b32
ResizeCursor( voxel_stack_element_cursor *Cursor, umm Count, memory_arena *Memory)
{
  /* ENSURE_OWNED_BY_THREAD(Cursor); */

  umm CurrentSize = TotalSize(Cursor);

  TruncateToElementCount(Cursor, Count);
  umm NewSize = TotalSize(Cursor);

  Assert(NewSize/sizeof(voxel_stack_element) == Count);

  /* Info("Attempting to reallocate CurrentSize(%u), NewSize(%u)", CurrentSize, NewSize); */
  Ensure(Reallocate((u8*)Cursor->Start, Memory, CurrentSize, NewSize));
  return 0;
}

link_internal void
Unshift( voxel_stack_element_cursor *Cursor )
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



