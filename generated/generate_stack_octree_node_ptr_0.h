// callsite
// src/engine/world.h:81:0

// def (generate_stack)
// external/bonsai_stdlib/src/poof_functions.h:1472:0
struct octree_node_ptr_stack
{
  octree_node_ptr *Start;
  umm At;
  umm End;
  
};

link_internal octree_node_ptr_stack
OctreeNodePtrStack(umm ElementCount, memory_arena* Memory)
{
  octree_node_ptr *Start = (octree_node_ptr*)PushStruct(Memory, sizeof(octree_node_ptr)*ElementCount, 1, 0);
  octree_node_ptr_stack Result = {};
  Result.Start = Start;
  Result.End = ElementCount;
  Result.At = 0;
  return Result;
}

link_internal umm
CurrentCount(octree_node_ptr_stack *Cursor)
{
  umm Result = Cursor->At;
  return Result;
}

link_internal s32
LastIndex(octree_node_ptr_stack *Cursor)
{
  s32 Result = s32(Cursor->At)-1;
  return Result;
}

link_internal octree_node_ptr
Get(octree_node_ptr_stack *Cursor, umm ElementIndex)
{
  Assert(ElementIndex < Cursor->At);
  octree_node_ptr Result = Cursor->Start[ElementIndex];
  return Result;
}

link_internal void
Set(octree_node_ptr_stack *Cursor, umm ElementIndex, octree_node_ptr Element)
{
  umm CurrentElementCount = Cursor->At;
  Assert (ElementIndex <= CurrentElementCount);

  Cursor->Start[ElementIndex] = Element;
  if (ElementIndex == CurrentElementCount)
  {
    Cursor->At++;
  }
}

link_internal octree_node_ptr *
Push(octree_node_ptr_stack *Cursor, octree_node_ptr Element)
{
  Assert( Cursor->At < Cursor->End );
  octree_node_ptr *Result = Cursor->Start+Cursor->At;
  Cursor->Start[Cursor->At++] = Element;
  return Result;
}

link_internal octree_node_ptr
Pop(octree_node_ptr_stack *Cursor)
{
  Assert( Cursor->At > 0 );
  octree_node_ptr Result = Cursor->Start[LastIndex(Cursor)];
  Cursor->At--;
  return Result;
}

link_internal b32
RemoveUnordered(octree_node_ptr_stack *Cursor, octree_node_ptr Query)
{
  b32 Result = False;
  StackIterator(ElementIndex, Cursor)
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

