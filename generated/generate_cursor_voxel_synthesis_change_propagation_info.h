struct voxel_synthesis_change_propagation_info_cursor
{
  voxel_synthesis_change_propagation_info *Start;
  // TODO(Jesse)(immediate): For the love of fucksakes change these to indices
  voxel_synthesis_change_propagation_info *At;
  voxel_synthesis_change_propagation_info *End;
};

link_internal voxel_synthesis_change_propagation_info_cursor
VoxelSynthesisChangePropagationInfoCursor(umm ElementCount, memory_arena* Memory)
{
  voxel_synthesis_change_propagation_info *Start = (voxel_synthesis_change_propagation_info*)PushStruct(Memory, sizeof(voxel_synthesis_change_propagation_info)*ElementCount, 1, 0);
  voxel_synthesis_change_propagation_info_cursor Result = {
    .Start = Start,
    .End = Start+ElementCount,
    .At = Start,
  };
  return Result;
}

link_internal voxel_synthesis_change_propagation_info
Get(voxel_synthesis_change_propagation_info_cursor *Cursor, umm ElementIndex)
{
  Assert(ElementIndex < CurrentCount(Cursor));
  voxel_synthesis_change_propagation_info Result = Cursor->Start[ElementIndex];
  return Result;
}

link_internal void
Set(voxel_synthesis_change_propagation_info_cursor *Cursor, umm ElementIndex, voxel_synthesis_change_propagation_info Element)
{
  umm CurrentElementCount = CurrentCount(Cursor);
  Assert (ElementIndex <= CurrentElementCount);

  Cursor->Start[ElementIndex] = Element;
  if (ElementIndex == CurrentElementCount)
  {
    Cursor->At++;
  }
}

link_internal voxel_synthesis_change_propagation_info *
Push(voxel_synthesis_change_propagation_info_cursor *Cursor, voxel_synthesis_change_propagation_info Element)
{
  Assert( Cursor->At < Cursor->End );
  voxel_synthesis_change_propagation_info *Result = Cursor->At;
  *Cursor->At++ = Element;
  return Result;
}

link_internal voxel_synthesis_change_propagation_info
Pop(voxel_synthesis_change_propagation_info_cursor *Cursor)
{
  Assert( Cursor->At > Cursor->Start );
  voxel_synthesis_change_propagation_info Result = Cursor->At[-1];
  Cursor->At--;
  return Result;
}

link_internal s32
LastIndex(voxel_synthesis_change_propagation_info_cursor *Cursor)
{
  s32 Result = s32(CurrentCount(Cursor))-1;
  return Result;
}

link_internal b32
Remove(voxel_synthesis_change_propagation_info_cursor *Cursor, voxel_synthesis_change_propagation_info Query)
{
  b32 Result = False;
  CursorIterator(ElementIndex, Cursor)
  {
    voxel_synthesis_change_propagation_info Element = Get(Cursor, ElementIndex);
    if (AreEqual(Element, Query))
    {
      b32 IsLastIndex = LastIndex(Cursor) == s32(ElementIndex);
      voxel_synthesis_change_propagation_info Tmp = Pop(Cursor);

      if (IsLastIndex) { Assert(AreEqual(Tmp, Query)); }
      else { Set(Cursor, ElementIndex, Tmp); }
      Result = True;
    }
  }
  return Result;
}

