// callsite
// src/engine/serdes.cpp:374:0

// def (block_array)
// external/bonsai_stdlib/src/poof_functions.h:2724:0




struct entity_block
{
  /* u32 Index; */
  umm At;
  entity Elements[8];
};


struct entity_block_array_index
{
  umm Index; 
};

struct entity_block_array
poof(
  @collection
  
  
)
{
  entity_block **BlockPtrs; poof(@array_length(Element->BlockCount))
  u32   BlockCount;
  u32   ElementCount;
  memory_arena *Memory; poof(@no_serialize)
  
};

link_internal entity_block_array
EntityBlockArray(memory_arena *Memory)
{
  entity_block_array Result = {};
  Result.Memory = Memory;
  return Result;
}

link_internal b32
AreEqual(entity_block_array_index *Thing1, entity_block_array_index *Thing2)
{
  if (Thing1 && Thing2)
  {
        b32 Result = MemoryIsEqual((u8*)Thing1, (u8*)Thing2, sizeof( entity_block_array_index ) );

    return Result;
  }
  else
  {
    return (Thing1 == Thing2);
  }
}

link_internal b32
AreEqual(entity_block_array_index Thing1, entity_block_array_index Thing2)
{
    b32 Result = MemoryIsEqual((u8*)&Thing1, (u8*)&Thing2, sizeof( entity_block_array_index ) );

  return Result;
}


typedef entity_block_array entity_paged_list;

link_internal entity_block_array_index
operator++( entity_block_array_index &I0 )
{
  I0.Index++;
  return I0;
}

link_internal b32
operator<( entity_block_array_index I0, entity_block_array_index I1 )
{
  b32 Result = I0.Index < I1.Index;
  return Result;
}

link_internal b32
operator==( entity_block_array_index I0, entity_block_array_index I1 )
{
  b32 Result = I0.Index == I1.Index;
  return Result;
}

link_inline umm
GetIndex( entity_block_array_index *Index)
{
  umm Result = Index->Index;
  return Result;
}


link_internal entity_block_array_index
ZerothIndex( entity_block_array *Arr )
{
  return {};
}

link_internal entity_block_array_index
Capacity( entity_block_array *Arr )
{
  entity_block_array_index Result = {Arr->BlockCount * 8};
  return Result;
}

link_internal entity_block_array_index
AtElements( entity_block_array *Arr )
{
  entity_block_array_index Result = {Arr->ElementCount};
  return Result;
}


link_internal umm
TotalElements( entity_block_array *Arr )
{
  umm Result = AtElements(Arr).Index;
  return Result;
}


link_internal entity_block_array_index
LastIndex( entity_block_array *Arr )
{
  entity_block_array_index Result = {};
  umm Count = AtElements(Arr).Index;
  if (Count) Result.Index = Count-1;
  return Result;
}

link_internal umm
Count( entity_block_array *Arr )
{
  auto Result = AtElements(Arr).Index;
  return Result;
}

link_internal entity_block *
GetBlock( entity_block_array *Arr, entity_block_array_index Index )
{
  umm BlockIndex   = Index.Index / 8;
  Assert(BlockIndex < Arr->BlockCount);
  entity_block *Block = Arr->BlockPtrs[BlockIndex];
  return Block;
}

link_internal entity *
GetPtr( entity_block_array *Arr, entity_block_array_index Index )
{
  Assert(Arr->BlockPtrs);
  Assert(Index.Index < Capacity(Arr).Index);

  entity_block *Block = GetBlock(Arr, Index);

  umm ElementIndex = Index.Index % 8;
  entity *Result = (Block->Elements + ElementIndex);
  return Result;
}


link_internal entity *
GetPtr( entity_block_array *Arr, umm Index )
{
  entity_block_array_index I = {Index};
  return GetPtr(Arr, I);
}


link_internal entity *
TryGetPtr( entity_block_array *Arr, entity_block_array_index Index)
{
  entity * Result = {};
  if (Arr->BlockPtrs && Index < AtElements(Arr))
  {
    Result = GetPtr(Arr, Index);
  }
  return Result;
}

link_internal entity *
TryGetPtr( entity_block_array *Arr, umm Index)
{
  auto Result = TryGetPtr(Arr, entity_block_array_index{Index});
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
CS( entity_block_array_index Index )
{
  return FSz("(%u)", Index.Index);
}

link_internal entity *
Set( entity_block_array *Arr,
  entity *Element,
  entity_block_array_index Index )
{
  Assert(Arr->BlockPtrs);
  Assert(Index.Index < Capacity(Arr).Index);
  entity_block *Block = GetBlock(Arr, Index);
  umm ElementIndex = Index.Index % 8;
  auto Slot = Block->Elements+ElementIndex;
  *Slot = *Element;
  return Slot;
}

link_internal void
NewBlock( entity_block_array *Arr )
{
  entity_block  *NewBlock     = Allocate( entity_block , Arr->Memory,                 1);
  entity_block **NewBlockPtrs = Allocate( entity_block*, Arr->Memory, Arr->BlockCount+1);

  RangeIterator_t(u32, BlockI, Arr->BlockCount)
  {
    NewBlockPtrs[BlockI] = Arr->BlockPtrs[BlockI];
  }

  NewBlockPtrs[Arr->BlockCount] = NewBlock;

  
  
  Arr->BlockPtrs = NewBlockPtrs;
  Arr->BlockCount += 1;
}

link_internal void
RemoveUnordered( entity_block_array *Array, entity_block_array_index Index)
{
  auto LastI = LastIndex(Array);
  Assert(Index.Index <= LastI.Index);

  auto LastElement = GetPtr(Array, LastI);
  Set(Array, LastElement, Index);
  Array->ElementCount -= 1;
}

link_internal void
RemoveOrdered( entity_block_array *Array, entity_block_array_index IndexToRemove)
{
  Assert(IndexToRemove.Index < Array->ElementCount);

  entity *Prev = {};

  entity_block_array_index Max = AtElements(Array);
  RangeIteratorRange_t(umm, Index, Max.Index, IndexToRemove.Index)
  {
    entity *E = GetPtr(Array, Index);

    if (Prev)
    {
      *Prev = *E;
    }

    Prev = E;
  }

  Array->ElementCount -= 1;
}

link_internal void
RemoveOrdered( entity_block_array *Array, entity *Element )
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

link_internal entity_block_array_index
Find( entity_block_array *Array, entity *Query)
{
  entity_block_array_index Result = {INVALID_BLOCK_ARRAY_INDEX};
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
IsValid(entity_block_array_index *Index)
{
  entity_block_array_index Test = {INVALID_BLOCK_ARRAY_INDEX};
  b32 Result = (AreEqual(Index, &Test) == False);
  return Result;
}

link_internal entity *
Push( entity_block_array *Array, entity *Element)
{
  Assert(Array->Memory);

  if (AtElements(Array) == Capacity(Array))
  {
    NewBlock(Array);
  }

  entity *Result = Set(Array, Element, AtElements(Array));

  Array->ElementCount += 1;

  return Result;
}

link_internal entity *
Push( entity_block_array *Array )
{
  entity Element = {};
  auto Result = Push(Array, &Element);
  return Result;
}

link_internal void
Insert( entity_block_array *Array, entity_block_array_index Index, entity *Element )
{
  Assert(Index.Index <= LastIndex(Array).Index);
  Assert(Array->Memory);

  // Alocate a new thingy
  entity *Prev = Push(Array);

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
Insert( entity_block_array *Array, u32 Index, entity *Element )
{
  Insert(Array, { .Index = Index }, Element);
}

link_internal void
Shift( entity_block_array *Array, entity *Element )
{
  Insert(Array, { .Index = 0 }, Element);
}

/* element_t.has_tag(do_editor_ui)? */
/* { */
/*   do_editor_ui_for_container( block_array_t ) */
/* } */


link_internal entity *
Pop( entity_block_array *Array )
{
  if (auto Result = TryGetPtr(Array, LastIndex(Array)))
  {
    Assert(Array->ElementCount > 0);
    Array->ElementCount -= 1;
    return Result;
  }
  return 0;
}



