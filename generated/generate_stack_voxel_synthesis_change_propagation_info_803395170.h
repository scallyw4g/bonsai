// callsite
// src/engine/voxel_synthesis.h:315:0

// def (generate_stack)
// external/bonsai_stdlib/src/poof_functions.h:1472:0
struct voxel_synthesis_change_propagation_info_stack
{
  voxel_synthesis_change_propagation_info *Start;
  umm At;
  umm End;
  v3i Min, Max;
};

link_internal voxel_synthesis_change_propagation_info_stack
VoxelSynthesisChangePropagationInfoStack(umm ElementCount, memory_arena* Memory)
{
  voxel_synthesis_change_propagation_info *Start = (voxel_synthesis_change_propagation_info*)PushStruct(Memory, sizeof(voxel_synthesis_change_propagation_info)*ElementCount, 1, 0);
  voxel_synthesis_change_propagation_info_stack Result = {};
  Result.Start = Start;
  Result.End = ElementCount;
  Result.At = 0;
  return Result;
}

link_internal umm
CurrentCount(voxel_synthesis_change_propagation_info_stack *Cursor)
{
  umm Result = Cursor->At;
  return Result;
}

link_internal s32
LastIndex(voxel_synthesis_change_propagation_info_stack *Cursor)
{
  s32 Result = s32(Cursor->At)-1;
  return Result;
}

link_internal voxel_synthesis_change_propagation_info
Get(voxel_synthesis_change_propagation_info_stack *Cursor, umm ElementIndex)
{
  Assert(ElementIndex < Cursor->At);
  voxel_synthesis_change_propagation_info Result = Cursor->Start[ElementIndex];
  return Result;
}

link_internal void
Set(voxel_synthesis_change_propagation_info_stack *Cursor, umm ElementIndex, voxel_synthesis_change_propagation_info Element)
{
  umm CurrentElementCount = Cursor->At;
  Assert (ElementIndex <= CurrentElementCount);

  Cursor->Start[ElementIndex] = Element;
  if (ElementIndex == CurrentElementCount)
  {
    Cursor->At++;
  }
}

link_internal voxel_synthesis_change_propagation_info *
Push(voxel_synthesis_change_propagation_info_stack *Cursor, voxel_synthesis_change_propagation_info Element)
{
  Assert( Cursor->At < Cursor->End );
  voxel_synthesis_change_propagation_info *Result = Cursor->Start+Cursor->At;
  Cursor->Start[Cursor->At++] = Element;
  return Result;
}

link_internal voxel_synthesis_change_propagation_info
Pop(voxel_synthesis_change_propagation_info_stack *Cursor)
{
  Assert( Cursor->At > 0 );
  voxel_synthesis_change_propagation_info Result = Cursor->Start[LastIndex(Cursor)];
  Cursor->At--;
  return Result;
}

link_internal b32
RemoveUnordered(voxel_synthesis_change_propagation_info_stack *Cursor, voxel_synthesis_change_propagation_info Query)
{
  b32 Result = False;
  StackIterator(ElementIndex, Cursor)
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

