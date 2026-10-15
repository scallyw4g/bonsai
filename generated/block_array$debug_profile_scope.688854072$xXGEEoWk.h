// callsite
// external/bonsai_stdlib/src/work_queue.h:52:0

// def (block_array)
// external/bonsai_stdlib/src/poof_functions.h:2824:0




struct debug_profile_scope_block
{
  debug_profile_scope Elements[8];
};


struct debug_profile_scope_block_array_index
{
  umm Index; 
};

struct debug_profile_scope_block_array
poof(
  @collection
  
  
)
{
  debug_profile_scope_block **BlockPtrs; poof(@array_length(Element->BlockCount))
  u32   BlockCount;
  u32   ElementCount;
  memory_arena *Memory; poof(@no_serialize)
  
};

link_internal debug_profile_scope_block_array
DebugProfileScopeBlockArray(memory_arena *Memory)
{
  debug_profile_scope_block_array Result = {};
  Result.Memory = Memory;
  return Result;
}

link_internal b32
AreEqual(debug_profile_scope_block_array_index *Thing1, debug_profile_scope_block_array_index *Thing2)
{
  if (Thing1 && Thing2)
  {
        b32 Result = MemoryIsEqual((u8*)Thing1, (u8*)Thing2, sizeof( debug_profile_scope_block_array_index ) );

    return Result;
  }
  else
  {
    return (Thing1 == Thing2);
  }
}

link_internal b32
AreEqual(debug_profile_scope_block_array_index Thing1, debug_profile_scope_block_array_index Thing2)
{
    b32 Result = MemoryIsEqual((u8*)&Thing1, (u8*)&Thing2, sizeof( debug_profile_scope_block_array_index ) );

  return Result;
}


typedef debug_profile_scope_block_array debug_profile_scope_paged_list;

link_internal debug_profile_scope_block_array_index
operator++( debug_profile_scope_block_array_index &I0 )
{
  I0.Index++;
  return I0;
}

link_internal b32
operator<( debug_profile_scope_block_array_index I0, debug_profile_scope_block_array_index I1 )
{
  b32 Result = I0.Index < I1.Index;
  return Result;
}

link_internal b32
operator==( debug_profile_scope_block_array_index I0, debug_profile_scope_block_array_index I1 )
{
  b32 Result = I0.Index == I1.Index;
  return Result;
}

link_inline umm
GetIndex( debug_profile_scope_block_array_index *Index)
{
  umm Result = Index->Index;
  return Result;
}


link_internal debug_profile_scope_block_array_index
ZerothIndex( debug_profile_scope_block_array *Arr )
{
  return {};
}

link_internal debug_profile_scope_block_array_index
Capacity( debug_profile_scope_block_array *Arr )
{
  debug_profile_scope_block_array_index Result = {Arr->BlockCount * 8};
  return Result;
}

link_internal debug_profile_scope_block_array_index
AtElements( debug_profile_scope_block_array *Arr )
{
  debug_profile_scope_block_array_index Result = {Arr->ElementCount};
  return Result;
}


link_internal umm
TotalElements( debug_profile_scope_block_array *Arr )
{
  umm Result = AtElements(Arr).Index;
  return Result;
}


link_internal debug_profile_scope_block_array_index
LastIndex( debug_profile_scope_block_array *Arr )
{
  debug_profile_scope_block_array_index Result = {};
  umm Count = AtElements(Arr).Index;
  if (Count) Result.Index = Count-1;
  return Result;
}

link_internal umm
Count( debug_profile_scope_block_array *Arr )
{
  auto Result = AtElements(Arr).Index;
  return Result;
}

link_internal debug_profile_scope_block *
GetBlock( debug_profile_scope_block_array *Arr, debug_profile_scope_block_array_index Index )
{
  umm BlockIndex   = Index.Index / 8;
  Assert(BlockIndex < Arr->BlockCount);
  debug_profile_scope_block *Block = Arr->BlockPtrs[BlockIndex];
  return Block;
}

link_internal debug_profile_scope *
GetPtr( debug_profile_scope_block_array *Arr, debug_profile_scope_block_array_index Index )
{
  Assert(Arr->BlockPtrs);
  Assert(Index.Index < AtElements(Arr).Index);
  Assert(Index.Index < Capacity(Arr).Index);

  debug_profile_scope_block *Block = GetBlock(Arr, Index);

  umm ElementIndex = Index.Index % 8;
  debug_profile_scope *Result = (Block->Elements + ElementIndex);
  return Result;
}


link_internal debug_profile_scope *
GetPtr( debug_profile_scope_block_array *Arr, umm Index )
{
  debug_profile_scope_block_array_index I = {Index};
  return GetPtr(Arr, I);
}


link_internal debug_profile_scope *
TryGetPtr( debug_profile_scope_block_array *Arr, debug_profile_scope_block_array_index Index)
{
  debug_profile_scope * Result = {};
  if (Arr->BlockPtrs && Index < AtElements(Arr))
  {
    Result = GetPtr(Arr, Index);
  }
  return Result;
}

link_internal debug_profile_scope *
TryGetPtr( debug_profile_scope_block_array *Arr, umm Index)
{
  auto Result = TryGetPtr(Arr, debug_profile_scope_block_array_index{Index});
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
CS( debug_profile_scope_block_array_index Index )
{
  return FSz("(%u)", Index.Index);
}

link_internal b32
ValidateBlocksMatchCapacity( debug_profile_scope_block_array *Array )
{
  auto Cap = Capacity(Array).Index;
  auto Elements = AtElements(Array).Index;
  b32 Result = Elements <= Cap;
  Assert(Result);
  return Result;
}

link_internal debug_profile_scope *
Set( debug_profile_scope_block_array *Arr,
  debug_profile_scope *Element,
  debug_profile_scope_block_array_index Index )
{
  Assert(Arr->BlockPtrs);
  Assert(Index.Index < Capacity(Arr).Index);
  debug_profile_scope_block *Block = GetBlock(Arr, Index);
  umm ElementIndex = Index.Index % 8;
  auto Slot = Block->Elements+ElementIndex;
  *Slot = *Element;
  return Slot;
}

link_internal void
NewBlock( debug_profile_scope_block_array *Arr )
{
  debug_profile_scope_block  *NewBlock     = Allocate( debug_profile_scope_block , Arr->Memory,                 1);
  debug_profile_scope_block **NewBlockPtrs = Allocate( debug_profile_scope_block*, Arr->Memory, Arr->BlockCount+1);

  RangeIterator_t(u32, BlockI, Arr->BlockCount)
  {
    NewBlockPtrs[BlockI] = Arr->BlockPtrs[BlockI];
  }

  NewBlockPtrs[Arr->BlockCount] = NewBlock;

  
  
  Arr->BlockPtrs = NewBlockPtrs;
  Arr->BlockCount += 1;
}

link_internal void
RemoveUnordered( debug_profile_scope_block_array *Array, debug_profile_scope_block_array_index Index)
{
  auto LastI = LastIndex(Array);
  Assert(Index.Index <= LastI.Index);

  auto LastElement = GetPtr(Array, LastI);
  Set(Array, LastElement, Index);
  Array->ElementCount -= 1;
}

link_internal void
RemoveOrdered( debug_profile_scope_block_array *Array, debug_profile_scope_block_array_index IndexToRemove)
{
  Assert(IndexToRemove.Index < Array->ElementCount);

  debug_profile_scope *Prev = {};

  debug_profile_scope_block_array_index Max = AtElements(Array);
  RangeIteratorRange_t(umm, Index, Max.Index, IndexToRemove.Index)
  {
    debug_profile_scope *E = GetPtr(Array, Index);

    if (Prev)
    {
      *Prev = *E;
    }

    Prev = E;
  }

  Array->ElementCount -= 1;
  Assert(ValidateBlocksMatchCapacity(Array));
}

link_internal void
RemoveOrdered( debug_profile_scope_block_array *Array, debug_profile_scope *Element )
{
  IterateOver(Array, E, I)
  {
    if (E == Element)
    {
      RemoveOrdered(Array, I);
      break;
    }
  }
  Assert(ValidateBlocksMatchCapacity(Array));
}

link_internal debug_profile_scope_block_array_index
Find( debug_profile_scope_block_array *Array, debug_profile_scope *Query)
{
  debug_profile_scope_block_array_index Result = {INVALID_BLOCK_ARRAY_INDEX};
  IterateOver(Array, E, Index)
  {
    if ( AreEqual(E, Query) )
    {
      Result = Index;
      break;
    }
  }
  return Result;
}



link_internal b32
IsValid(debug_profile_scope_block_array_index *Index)
{
  debug_profile_scope_block_array_index Test = {INVALID_BLOCK_ARRAY_INDEX};
  b32 Result = (AreEqual(Index, &Test) == False);
  return Result;
}

link_internal debug_profile_scope *
Push( debug_profile_scope_block_array *Array, debug_profile_scope *Element)
{
  Assert(Array->Memory);

  if (AtElements(Array) == Capacity(Array))
  {
    NewBlock(Array);
  }

  debug_profile_scope *Result = Set(Array, Element, AtElements(Array));

  Array->ElementCount += 1;

  Assert(ValidateBlocksMatchCapacity(Array));
  return Result;
}

link_internal debug_profile_scope *
Push( debug_profile_scope_block_array *Array )
{
  debug_profile_scope Element = {};
  auto Result = Push(Array, &Element);
  return Result;
}

link_internal void
Insert( debug_profile_scope_block_array *Array, debug_profile_scope_block_array_index Index, debug_profile_scope *Element )
{
  Assert(Index.Index <= LastIndex(Array).Index);
  Assert(Array->Memory);

  // Alocate a new thingy
  debug_profile_scope *Prev = Push(Array);

  auto Last = LastIndex(Array);

  RangeIteratorReverseRange(I, s32(Last.Index), s32(Index.Index))
  {
    auto E = GetPtr(Array, umm(I));
    *Prev = *E;
    Prev = E;
  }

  *Prev = *Element;
  Assert(ValidateBlocksMatchCapacity(Array));
}

link_internal void
Insert( debug_profile_scope_block_array *Array, u32 Index, debug_profile_scope *Element )
{
  Insert(Array, { .Index = Index }, Element);
}

link_internal void
Shift( debug_profile_scope_block_array *Array, debug_profile_scope *Element )
{
  Insert(Array, { .Index = 0 }, Element);
}

/* element_t.has_tag(do_editor_ui)? */
/* { */
/*   do_editor_ui_for_container( block_array_t ) */
/* } */





link_internal debug_profile_scope *
Pop( debug_profile_scope_block_array *Array )
{
  debug_profile_scope * Result = {};

  if (auto P = TryGetPtr(Array, LastIndex(Array)))
  {
    Assert(Array->ElementCount > 0);
    Array->ElementCount -= 1;
    Result = P;
  }
  Assert(ValidateBlocksMatchCapacity(Array));
  return Result;
}

link_internal void
ClearList( debug_profile_scope_block_array *Array )
{
  IterateOver(Array, Element, ElementIndex)
  {
    *Element = {};
  }

  Array->ElementCount = 0;
}



