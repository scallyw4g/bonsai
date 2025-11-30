// callsite
// src/engine/graphics.h:182:0

// def (block_array)
// external/bonsai_stdlib/src/poof_functions.h:2724:0




struct dummy_work_queue_entry_build_chunk_mesh_block
{
  /* u32 Index; */
  umm At;
  dummy_work_queue_entry_build_chunk_mesh Elements[8];
};


struct dummy_work_queue_entry_build_chunk_mesh_block_array_index
{
  umm Index; 
};

struct dummy_work_queue_entry_build_chunk_mesh_block_array
poof(
  @collection
  
  
)
{
  dummy_work_queue_entry_build_chunk_mesh_block **BlockPtrs; poof(@array_length(Element->BlockCount))
  u32   BlockCount;
  u32   ElementCount;
  memory_arena *Memory; poof(@no_serialize)
  
};

link_internal dummy_work_queue_entry_build_chunk_mesh_block_array
DummyWorkQueueEntryBuildChunkMeshBlockArray(memory_arena *Memory)
{
  dummy_work_queue_entry_build_chunk_mesh_block_array Result = {};
  Result.Memory = Memory;
  return Result;
}

link_internal b32
AreEqual(dummy_work_queue_entry_build_chunk_mesh_block_array_index *Thing1, dummy_work_queue_entry_build_chunk_mesh_block_array_index *Thing2)
{
  if (Thing1 && Thing2)
  {
        b32 Result = MemoryIsEqual((u8*)Thing1, (u8*)Thing2, sizeof( dummy_work_queue_entry_build_chunk_mesh_block_array_index ) );

    return Result;
  }
  else
  {
    return (Thing1 == Thing2);
  }
}

link_internal b32
AreEqual(dummy_work_queue_entry_build_chunk_mesh_block_array_index Thing1, dummy_work_queue_entry_build_chunk_mesh_block_array_index Thing2)
{
    b32 Result = MemoryIsEqual((u8*)&Thing1, (u8*)&Thing2, sizeof( dummy_work_queue_entry_build_chunk_mesh_block_array_index ) );

  return Result;
}


typedef dummy_work_queue_entry_build_chunk_mesh_block_array dummy_work_queue_entry_build_chunk_mesh_paged_list;

link_internal dummy_work_queue_entry_build_chunk_mesh_block_array_index
operator++( dummy_work_queue_entry_build_chunk_mesh_block_array_index &I0 )
{
  I0.Index++;
  return I0;
}

link_internal b32
operator<( dummy_work_queue_entry_build_chunk_mesh_block_array_index I0, dummy_work_queue_entry_build_chunk_mesh_block_array_index I1 )
{
  b32 Result = I0.Index < I1.Index;
  return Result;
}

link_internal b32
operator==( dummy_work_queue_entry_build_chunk_mesh_block_array_index I0, dummy_work_queue_entry_build_chunk_mesh_block_array_index I1 )
{
  b32 Result = I0.Index == I1.Index;
  return Result;
}

link_inline umm
GetIndex( dummy_work_queue_entry_build_chunk_mesh_block_array_index *Index)
{
  umm Result = Index->Index;
  return Result;
}


link_internal dummy_work_queue_entry_build_chunk_mesh_block_array_index
ZerothIndex( dummy_work_queue_entry_build_chunk_mesh_block_array *Arr )
{
  return {};
}

link_internal dummy_work_queue_entry_build_chunk_mesh_block_array_index
Capacity( dummy_work_queue_entry_build_chunk_mesh_block_array *Arr )
{
  dummy_work_queue_entry_build_chunk_mesh_block_array_index Result = {Arr->BlockCount * 8};
  return Result;
}

link_internal dummy_work_queue_entry_build_chunk_mesh_block_array_index
AtElements( dummy_work_queue_entry_build_chunk_mesh_block_array *Arr )
{
  dummy_work_queue_entry_build_chunk_mesh_block_array_index Result = {Arr->ElementCount};
  return Result;
}


link_internal umm
TotalElements( dummy_work_queue_entry_build_chunk_mesh_block_array *Arr )
{
  umm Result = AtElements(Arr).Index;
  return Result;
}


link_internal dummy_work_queue_entry_build_chunk_mesh_block_array_index
LastIndex( dummy_work_queue_entry_build_chunk_mesh_block_array *Arr )
{
  dummy_work_queue_entry_build_chunk_mesh_block_array_index Result = {};
  umm Count = AtElements(Arr).Index;
  if (Count) Result.Index = Count-1;
  return Result;
}

link_internal umm
Count( dummy_work_queue_entry_build_chunk_mesh_block_array *Arr )
{
  auto Result = AtElements(Arr).Index;
  return Result;
}

link_internal dummy_work_queue_entry_build_chunk_mesh_block *
GetBlock( dummy_work_queue_entry_build_chunk_mesh_block_array *Arr, dummy_work_queue_entry_build_chunk_mesh_block_array_index Index )
{
  umm BlockIndex   = Index.Index / 8;
  Assert(BlockIndex < Arr->BlockCount);
  dummy_work_queue_entry_build_chunk_mesh_block *Block = Arr->BlockPtrs[BlockIndex];
  return Block;
}

link_internal dummy_work_queue_entry_build_chunk_mesh *
GetPtr( dummy_work_queue_entry_build_chunk_mesh_block_array *Arr, dummy_work_queue_entry_build_chunk_mesh_block_array_index Index )
{
  Assert(Arr->BlockPtrs);
  Assert(Index.Index < Capacity(Arr).Index);

  dummy_work_queue_entry_build_chunk_mesh_block *Block = GetBlock(Arr, Index);

  umm ElementIndex = Index.Index % 8;
  dummy_work_queue_entry_build_chunk_mesh *Result = (Block->Elements + ElementIndex);
  return Result;
}


link_internal dummy_work_queue_entry_build_chunk_mesh *
GetPtr( dummy_work_queue_entry_build_chunk_mesh_block_array *Arr, umm Index )
{
  dummy_work_queue_entry_build_chunk_mesh_block_array_index I = {Index};
  return GetPtr(Arr, I);
}


link_internal dummy_work_queue_entry_build_chunk_mesh *
TryGetPtr( dummy_work_queue_entry_build_chunk_mesh_block_array *Arr, dummy_work_queue_entry_build_chunk_mesh_block_array_index Index)
{
  dummy_work_queue_entry_build_chunk_mesh * Result = {};
  if (Arr->BlockPtrs && Index < AtElements(Arr))
  {
    Result = GetPtr(Arr, Index);
  }
  return Result;
}

link_internal dummy_work_queue_entry_build_chunk_mesh *
TryGetPtr( dummy_work_queue_entry_build_chunk_mesh_block_array *Arr, umm Index)
{
  auto Result = TryGetPtr(Arr, dummy_work_queue_entry_build_chunk_mesh_block_array_index{Index});
  return Result;
}

/* link_internal void */
/* Swap( (element_t.name)_block_array *Arr, umm I0, umm I1) */
/* { */
/*   Assert(I0 < AtElements(Arr).Index); */
/*   Assert(I1 < AtElements(Arr).Index); */

/*   auto P0 = GetPtr(Arr, I0); */
/*   auto P1 = GetPtr(Arr, I1); */

/*   auto Tmp = *P0; */
/*   *P0 = *P1; */

/*   *P1 = Tmp; */
/* } */





link_internal cs
CS( dummy_work_queue_entry_build_chunk_mesh_block_array_index Index )
{
  return FSz("(%u)", Index.Index);
}

link_internal dummy_work_queue_entry_build_chunk_mesh *
Set( dummy_work_queue_entry_build_chunk_mesh_block_array *Arr,
  dummy_work_queue_entry_build_chunk_mesh *Element,
  dummy_work_queue_entry_build_chunk_mesh_block_array_index Index )
{
  Assert(Arr->BlockPtrs);
  Assert(Index.Index < Capacity(Arr).Index);
  dummy_work_queue_entry_build_chunk_mesh_block *Block = GetBlock(Arr, Index);
  umm ElementIndex = Index.Index % 8;
  auto Slot = Block->Elements+ElementIndex;
  *Slot = *Element;
  return Slot;
}

link_internal void
NewBlock( dummy_work_queue_entry_build_chunk_mesh_block_array *Arr )
{
  dummy_work_queue_entry_build_chunk_mesh_block  *NewBlock     = Allocate( dummy_work_queue_entry_build_chunk_mesh_block , Arr->Memory,                 1);
  dummy_work_queue_entry_build_chunk_mesh_block **NewBlockPtrs = Allocate( dummy_work_queue_entry_build_chunk_mesh_block*, Arr->Memory, Arr->BlockCount+1);

  RangeIterator_t(u32, BlockI, Arr->BlockCount)
  {
    NewBlockPtrs[BlockI] = Arr->BlockPtrs[BlockI];
  }

  NewBlockPtrs[Arr->BlockCount] = NewBlock;

  
  
  Arr->BlockPtrs = NewBlockPtrs;
  Arr->BlockCount += 1;
}

link_internal void
RemoveUnordered( dummy_work_queue_entry_build_chunk_mesh_block_array *Array, dummy_work_queue_entry_build_chunk_mesh_block_array_index Index)
{
  auto LastI = LastIndex(Array);
  Assert(Index.Index <= LastI.Index);

  auto LastElement = GetPtr(Array, LastI);
  Set(Array, LastElement, Index);
  Array->ElementCount -= 1;
}

link_internal void
RemoveOrdered( dummy_work_queue_entry_build_chunk_mesh_block_array *Array, dummy_work_queue_entry_build_chunk_mesh_block_array_index IndexToRemove)
{
  Assert(IndexToRemove.Index < Array->ElementCount);

  dummy_work_queue_entry_build_chunk_mesh *Prev = {};

  dummy_work_queue_entry_build_chunk_mesh_block_array_index Max = AtElements(Array);
  RangeIteratorRange_t(umm, Index, Max.Index, IndexToRemove.Index)
  {
    dummy_work_queue_entry_build_chunk_mesh *E = GetPtr(Array, Index);

    if (Prev)
    {
      *Prev = *E;
    }

    Prev = E;
  }

  Array->ElementCount -= 1;
}

link_internal void
RemoveOrdered( dummy_work_queue_entry_build_chunk_mesh_block_array *Array, dummy_work_queue_entry_build_chunk_mesh *Element )
{
  IterateOver(Array, E, I)
  {
    if (E == Element)
    {
      RemoveOrdered(Array, I);
      break;
    }
  }
}

link_internal dummy_work_queue_entry_build_chunk_mesh_block_array_index
Find( dummy_work_queue_entry_build_chunk_mesh_block_array *Array, dummy_work_queue_entry_build_chunk_mesh *Query)
{
  dummy_work_queue_entry_build_chunk_mesh_block_array_index Result = {INVALID_BLOCK_ARRAY_INDEX};
  IterateOver(Array, E, Index)
  {
    if ( E == Query )
    {
      Result = Index;
      break;
    }
  }
  return Result;
}



link_internal b32
IsValid(dummy_work_queue_entry_build_chunk_mesh_block_array_index *Index)
{
  dummy_work_queue_entry_build_chunk_mesh_block_array_index Test = {INVALID_BLOCK_ARRAY_INDEX};
  b32 Result = (AreEqual(Index, &Test) == False);
  return Result;
}

link_internal dummy_work_queue_entry_build_chunk_mesh *
Push( dummy_work_queue_entry_build_chunk_mesh_block_array *Array, dummy_work_queue_entry_build_chunk_mesh *Element)
{
  Assert(Array->Memory);

  if (AtElements(Array) == Capacity(Array))
  {
    NewBlock(Array);
  }

  dummy_work_queue_entry_build_chunk_mesh *Result = Set(Array, Element, AtElements(Array));

  Array->ElementCount += 1;

  return Result;
}

link_internal dummy_work_queue_entry_build_chunk_mesh *
Push( dummy_work_queue_entry_build_chunk_mesh_block_array *Array )
{
  dummy_work_queue_entry_build_chunk_mesh Element = {};
  auto Result = Push(Array, &Element);
  return Result;
}

link_internal void
Insert( dummy_work_queue_entry_build_chunk_mesh_block_array *Array, dummy_work_queue_entry_build_chunk_mesh_block_array_index Index, dummy_work_queue_entry_build_chunk_mesh *Element )
{
  Assert(Index.Index <= LastIndex(Array).Index);
  Assert(Array->Memory);

  // Alocate a new thingy
  dummy_work_queue_entry_build_chunk_mesh *Prev = Push(Array);

  auto Last = LastIndex(Array);

  RangeIteratorReverseRange(I, s32(Last.Index), s32(Index.Index))
  {
    auto E = GetPtr(Array, umm(I));
    *Prev = *E;
    Prev = E;
  }

  *Prev = *Element;
}

link_internal void
Insert( dummy_work_queue_entry_build_chunk_mesh_block_array *Array, u32 Index, dummy_work_queue_entry_build_chunk_mesh *Element )
{
  Insert(Array, { .Index = Index }, Element);
}

link_internal void
Shift( dummy_work_queue_entry_build_chunk_mesh_block_array *Array, dummy_work_queue_entry_build_chunk_mesh *Element )
{
  Insert(Array, { .Index = 0 }, Element);
}

/* element_t.has_tag(do_editor_ui)? */
/* { */
/*   do_editor_ui_for_container( block_array_t ) */
/* } */


link_internal dummy_work_queue_entry_build_chunk_mesh *
Pop( dummy_work_queue_entry_build_chunk_mesh_block_array *Array )
{
  if (auto Result = TryGetPtr(Array, LastIndex(Array)))
  {
    Assert(Array->ElementCount > 0);
    Array->ElementCount -= 1;
    return Result;
  }
  return 0;
}



