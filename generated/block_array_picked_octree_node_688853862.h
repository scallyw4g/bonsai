// callsite
// src/engine/world.h:99:0

// def (block_array)
// external/bonsai_stdlib/src/poof_functions.h:2724:0




struct picked_octree_node_block
{
  /* u32 Index; */
  umm At;
  picked_octree_node Elements[8];
};


struct picked_octree_node_block_array_index
{
  umm Index; 
};

struct picked_octree_node_block_array
poof(
  @collection
  
  
)
{
  picked_octree_node_block **BlockPtrs; poof(@array_length(Element->BlockCount))
  u32   BlockCount;
  u32   ElementCount;
  memory_arena *Memory; poof(@no_serialize)
  
};

link_internal picked_octree_node_block_array
PickedOctreeNodeBlockArray(memory_arena *Memory)
{
  picked_octree_node_block_array Result = {};
  Result.Memory = Memory;
  return Result;
}

link_internal b32
AreEqual(picked_octree_node_block_array_index *Thing1, picked_octree_node_block_array_index *Thing2)
{
  if (Thing1 && Thing2)
  {
        b32 Result = MemoryIsEqual((u8*)Thing1, (u8*)Thing2, sizeof( picked_octree_node_block_array_index ) );

    return Result;
  }
  else
  {
    return (Thing1 == Thing2);
  }
}

link_internal b32
AreEqual(picked_octree_node_block_array_index Thing1, picked_octree_node_block_array_index Thing2)
{
    b32 Result = MemoryIsEqual((u8*)&Thing1, (u8*)&Thing2, sizeof( picked_octree_node_block_array_index ) );

  return Result;
}


typedef picked_octree_node_block_array picked_octree_node_paged_list;

link_internal picked_octree_node_block_array_index
operator++( picked_octree_node_block_array_index &I0 )
{
  I0.Index++;
  return I0;
}

link_internal b32
operator<( picked_octree_node_block_array_index I0, picked_octree_node_block_array_index I1 )
{
  b32 Result = I0.Index < I1.Index;
  return Result;
}

link_internal b32
operator==( picked_octree_node_block_array_index I0, picked_octree_node_block_array_index I1 )
{
  b32 Result = I0.Index == I1.Index;
  return Result;
}

link_inline umm
GetIndex( picked_octree_node_block_array_index *Index)
{
  umm Result = Index->Index;
  return Result;
}


link_internal picked_octree_node_block_array_index
ZerothIndex( picked_octree_node_block_array *Arr )
{
  return {};
}

link_internal picked_octree_node_block_array_index
Capacity( picked_octree_node_block_array *Arr )
{
  picked_octree_node_block_array_index Result = {Arr->BlockCount * 8};
  return Result;
}

link_internal picked_octree_node_block_array_index
AtElements( picked_octree_node_block_array *Arr )
{
  picked_octree_node_block_array_index Result = {Arr->ElementCount};
  return Result;
}


link_internal umm
TotalElements( picked_octree_node_block_array *Arr )
{
  umm Result = AtElements(Arr).Index;
  return Result;
}


link_internal picked_octree_node_block_array_index
LastIndex( picked_octree_node_block_array *Arr )
{
  picked_octree_node_block_array_index Result = {};
  umm Count = AtElements(Arr).Index;
  if (Count) Result.Index = Count-1;
  return Result;
}

link_internal umm
Count( picked_octree_node_block_array *Arr )
{
  auto Result = AtElements(Arr).Index;
  return Result;
}

link_internal picked_octree_node_block *
GetBlock( picked_octree_node_block_array *Arr, picked_octree_node_block_array_index Index )
{
  umm BlockIndex   = Index.Index / 8;
  Assert(BlockIndex < Arr->BlockCount);
  picked_octree_node_block *Block = Arr->BlockPtrs[BlockIndex];
  return Block;
}

link_internal picked_octree_node *
GetPtr( picked_octree_node_block_array *Arr, picked_octree_node_block_array_index Index )
{
  Assert(Arr->BlockPtrs);
  Assert(Index.Index < Capacity(Arr).Index);

  picked_octree_node_block *Block = GetBlock(Arr, Index);

  umm ElementIndex = Index.Index % 8;
  picked_octree_node *Result = (Block->Elements + ElementIndex);
  return Result;
}


link_internal picked_octree_node *
GetPtr( picked_octree_node_block_array *Arr, umm Index )
{
  picked_octree_node_block_array_index I = {Index};
  return GetPtr(Arr, I);
}


link_internal picked_octree_node *
TryGetPtr( picked_octree_node_block_array *Arr, picked_octree_node_block_array_index Index)
{
  picked_octree_node * Result = {};
  if (Arr->BlockPtrs && Index < AtElements(Arr))
  {
    Result = GetPtr(Arr, Index);
  }
  return Result;
}

link_internal picked_octree_node *
TryGetPtr( picked_octree_node_block_array *Arr, umm Index)
{
  auto Result = TryGetPtr(Arr, picked_octree_node_block_array_index{Index});
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
CS( picked_octree_node_block_array_index Index )
{
  return FSz("(%u)", Index.Index);
}

link_internal picked_octree_node *
Set( picked_octree_node_block_array *Arr,
  picked_octree_node *Element,
  picked_octree_node_block_array_index Index )
{
  Assert(Arr->BlockPtrs);
  Assert(Index.Index < Capacity(Arr).Index);
  picked_octree_node_block *Block = GetBlock(Arr, Index);
  umm ElementIndex = Index.Index % 8;
  auto Slot = Block->Elements+ElementIndex;
  *Slot = *Element;
  return Slot;
}

link_internal void
NewBlock( picked_octree_node_block_array *Arr )
{
  picked_octree_node_block  *NewBlock     = Allocate( picked_octree_node_block , Arr->Memory,                 1);
  picked_octree_node_block **NewBlockPtrs = Allocate( picked_octree_node_block*, Arr->Memory, Arr->BlockCount+1);

  RangeIterator_t(u32, BlockI, Arr->BlockCount)
  {
    NewBlockPtrs[BlockI] = Arr->BlockPtrs[BlockI];
  }

  NewBlockPtrs[Arr->BlockCount] = NewBlock;

  
  
  Arr->BlockPtrs = NewBlockPtrs;
  Arr->BlockCount += 1;
}

link_internal void
RemoveUnordered( picked_octree_node_block_array *Array, picked_octree_node_block_array_index Index)
{
  auto LastI = LastIndex(Array);
  Assert(Index.Index <= LastI.Index);

  auto LastElement = GetPtr(Array, LastI);
  Set(Array, LastElement, Index);
  Array->ElementCount -= 1;
}

link_internal void
RemoveOrdered( picked_octree_node_block_array *Array, picked_octree_node_block_array_index IndexToRemove)
{
  Assert(IndexToRemove.Index < Array->ElementCount);

  picked_octree_node *Prev = {};

  picked_octree_node_block_array_index Max = AtElements(Array);
  RangeIteratorRange_t(umm, Index, Max.Index, IndexToRemove.Index)
  {
    picked_octree_node *E = GetPtr(Array, Index);

    if (Prev)
    {
      *Prev = *E;
    }

    Prev = E;
  }

  Array->ElementCount -= 1;
}

link_internal void
RemoveOrdered( picked_octree_node_block_array *Array, picked_octree_node *Element )
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

link_internal picked_octree_node_block_array_index
Find( picked_octree_node_block_array *Array, picked_octree_node *Query)
{
  picked_octree_node_block_array_index Result = {INVALID_BLOCK_ARRAY_INDEX};
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
IsValid(picked_octree_node_block_array_index *Index)
{
  picked_octree_node_block_array_index Test = {INVALID_BLOCK_ARRAY_INDEX};
  b32 Result = (AreEqual(Index, &Test) == False);
  return Result;
}

link_internal picked_octree_node *
Push( picked_octree_node_block_array *Array, picked_octree_node *Element)
{
  Assert(Array->Memory);

  if (AtElements(Array) == Capacity(Array))
  {
    NewBlock(Array);
  }

  picked_octree_node *Result = Set(Array, Element, AtElements(Array));

  Array->ElementCount += 1;

  return Result;
}

link_internal picked_octree_node *
Push( picked_octree_node_block_array *Array )
{
  picked_octree_node Element = {};
  auto Result = Push(Array, &Element);
  return Result;
}

link_internal void
Insert( picked_octree_node_block_array *Array, picked_octree_node_block_array_index Index, picked_octree_node *Element )
{
  Assert(Index.Index <= LastIndex(Array).Index);
  Assert(Array->Memory);

  // Alocate a new thingy
  picked_octree_node *Prev = Push(Array);

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
Insert( picked_octree_node_block_array *Array, u32 Index, picked_octree_node *Element )
{
  Insert(Array, { .Index = Index }, Element);
}

link_internal void
Shift( picked_octree_node_block_array *Array, picked_octree_node *Element )
{
  Insert(Array, { .Index = 0 }, Element);
}

/* element_t.has_tag(do_editor_ui)? */
/* { */
/*   do_editor_ui_for_container( block_array_t ) */
/* } */


link_internal picked_octree_node *
Pop( picked_octree_node_block_array *Array )
{
  if (auto Result = TryGetPtr(Array, LastIndex(Array)))
  {
    Assert(Array->ElementCount > 0);
    Array->ElementCount -= 1;
    return Result;
  }
  return 0;
}



