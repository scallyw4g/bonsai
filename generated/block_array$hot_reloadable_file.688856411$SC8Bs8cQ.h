// callsite
// external/bonsai_stdlib/src/file.cpp:14:0

// def (block_array)
// external/bonsai_stdlib/src/poof_functions.h:2824:0




struct hot_reloadable_file_block
{
  hot_reloadable_file Elements[8];
};


struct hot_reloadable_file_block_array_index
{
  umm Index; 
};

struct hot_reloadable_file_block_array
poof(
  @collection
  
  
)
{
  hot_reloadable_file_block **BlockPtrs; poof(@array_length(Element->BlockCount))
  u32   BlockCount;
  u32   ElementCount;
  memory_arena *Memory; poof(@no_serialize)
  
};

link_internal hot_reloadable_file_block_array
HotReloadableFileBlockArray(memory_arena *Memory)
{
  hot_reloadable_file_block_array Result = {};
  Result.Memory = Memory;
  return Result;
}

link_internal b32
AreEqual(hot_reloadable_file_block_array_index *Thing1, hot_reloadable_file_block_array_index *Thing2)
{
  if (Thing1 && Thing2)
  {
        b32 Result = MemoryIsEqual((u8*)Thing1, (u8*)Thing2, sizeof( hot_reloadable_file_block_array_index ) );

    return Result;
  }
  else
  {
    return (Thing1 == Thing2);
  }
}

link_internal b32
AreEqual(hot_reloadable_file_block_array_index Thing1, hot_reloadable_file_block_array_index Thing2)
{
    b32 Result = MemoryIsEqual((u8*)&Thing1, (u8*)&Thing2, sizeof( hot_reloadable_file_block_array_index ) );

  return Result;
}


typedef hot_reloadable_file_block_array hot_reloadable_file_paged_list;

link_internal hot_reloadable_file_block_array_index
operator++( hot_reloadable_file_block_array_index &I0 )
{
  I0.Index++;
  return I0;
}

link_internal b32
operator<( hot_reloadable_file_block_array_index I0, hot_reloadable_file_block_array_index I1 )
{
  b32 Result = I0.Index < I1.Index;
  return Result;
}

link_internal b32
operator==( hot_reloadable_file_block_array_index I0, hot_reloadable_file_block_array_index I1 )
{
  b32 Result = I0.Index == I1.Index;
  return Result;
}

link_inline umm
GetIndex( hot_reloadable_file_block_array_index *Index)
{
  umm Result = Index->Index;
  return Result;
}


link_internal hot_reloadable_file_block_array_index
ZerothIndex( hot_reloadable_file_block_array *Arr )
{
  return {};
}

link_internal hot_reloadable_file_block_array_index
Capacity( hot_reloadable_file_block_array *Arr )
{
  hot_reloadable_file_block_array_index Result = {Arr->BlockCount * 8};
  return Result;
}

link_internal hot_reloadable_file_block_array_index
AtElements( hot_reloadable_file_block_array *Arr )
{
  hot_reloadable_file_block_array_index Result = {Arr->ElementCount};
  return Result;
}


link_internal umm
TotalElements( hot_reloadable_file_block_array *Arr )
{
  umm Result = AtElements(Arr).Index;
  return Result;
}


link_internal hot_reloadable_file_block_array_index
LastIndex( hot_reloadable_file_block_array *Arr )
{
  hot_reloadable_file_block_array_index Result = {};
  umm Count = AtElements(Arr).Index;
  if (Count) Result.Index = Count-1;
  return Result;
}

link_internal umm
Count( hot_reloadable_file_block_array *Arr )
{
  auto Result = AtElements(Arr).Index;
  return Result;
}

link_internal hot_reloadable_file_block *
GetBlock( hot_reloadable_file_block_array *Arr, hot_reloadable_file_block_array_index Index )
{
  umm BlockIndex   = Index.Index / 8;
  Assert(BlockIndex < Arr->BlockCount);
  hot_reloadable_file_block *Block = Arr->BlockPtrs[BlockIndex];
  return Block;
}

link_internal hot_reloadable_file *
GetPtr( hot_reloadable_file_block_array *Arr, hot_reloadable_file_block_array_index Index )
{
  Assert(Arr->BlockPtrs);
  Assert(Index.Index < AtElements(Arr).Index);
  Assert(Index.Index < Capacity(Arr).Index);

  hot_reloadable_file_block *Block = GetBlock(Arr, Index);

  umm ElementIndex = Index.Index % 8;
  hot_reloadable_file *Result = (Block->Elements + ElementIndex);
  return Result;
}


link_internal hot_reloadable_file *
GetPtr( hot_reloadable_file_block_array *Arr, umm Index )
{
  hot_reloadable_file_block_array_index I = {Index};
  return GetPtr(Arr, I);
}


link_internal hot_reloadable_file *
TryGetPtr( hot_reloadable_file_block_array *Arr, hot_reloadable_file_block_array_index Index)
{
  hot_reloadable_file * Result = {};
  if (Arr->BlockPtrs && Index < AtElements(Arr))
  {
    Result = GetPtr(Arr, Index);
  }
  return Result;
}

link_internal hot_reloadable_file *
TryGetPtr( hot_reloadable_file_block_array *Arr, umm Index)
{
  auto Result = TryGetPtr(Arr, hot_reloadable_file_block_array_index{Index});
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
CS( hot_reloadable_file_block_array_index Index )
{
  return FSz("(%u)", Index.Index);
}

link_internal b32
ValidateBlocksMatchCapacity( hot_reloadable_file_block_array *Array )
{
  auto Cap = Capacity(Array).Index;
  auto Elements = AtElements(Array).Index;
  b32 Result = Elements <= Cap;
  Assert(Result);
  return Result;
}

link_internal hot_reloadable_file *
Set( hot_reloadable_file_block_array *Arr,
  hot_reloadable_file *Element,
  hot_reloadable_file_block_array_index Index )
{
  Assert(Arr->BlockPtrs);
  Assert(Index.Index < Capacity(Arr).Index);
  hot_reloadable_file_block *Block = GetBlock(Arr, Index);
  umm ElementIndex = Index.Index % 8;
  auto Slot = Block->Elements+ElementIndex;
  *Slot = *Element;
  return Slot;
}

link_internal void
NewBlock( hot_reloadable_file_block_array *Arr )
{
  hot_reloadable_file_block  *NewBlock     = Allocate( hot_reloadable_file_block , Arr->Memory,                 1);
  hot_reloadable_file_block **NewBlockPtrs = Allocate( hot_reloadable_file_block*, Arr->Memory, Arr->BlockCount+1);

  RangeIterator_t(u32, BlockI, Arr->BlockCount)
  {
    NewBlockPtrs[BlockI] = Arr->BlockPtrs[BlockI];
  }

  NewBlockPtrs[Arr->BlockCount] = NewBlock;

  
  
  Arr->BlockPtrs = NewBlockPtrs;
  Arr->BlockCount += 1;
}

link_internal void
RemoveUnordered( hot_reloadable_file_block_array *Array, hot_reloadable_file_block_array_index Index)
{
  auto LastI = LastIndex(Array);
  Assert(Index.Index <= LastI.Index);

  auto LastElement = GetPtr(Array, LastI);
  Set(Array, LastElement, Index);
  Array->ElementCount -= 1;
}

link_internal void
RemoveOrdered( hot_reloadable_file_block_array *Array, hot_reloadable_file_block_array_index IndexToRemove)
{
  Assert(IndexToRemove.Index < Array->ElementCount);

  hot_reloadable_file *Prev = {};

  hot_reloadable_file_block_array_index Max = AtElements(Array);
  RangeIteratorRange_t(umm, Index, Max.Index, IndexToRemove.Index)
  {
    hot_reloadable_file *E = GetPtr(Array, Index);

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
RemoveOrdered( hot_reloadable_file_block_array *Array, hot_reloadable_file *Element )
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

link_internal hot_reloadable_file_block_array_index
Find( hot_reloadable_file_block_array *Array, hot_reloadable_file *Query)
{
  hot_reloadable_file_block_array_index Result = {INVALID_BLOCK_ARRAY_INDEX};
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
IsValid(hot_reloadable_file_block_array_index *Index)
{
  hot_reloadable_file_block_array_index Test = {INVALID_BLOCK_ARRAY_INDEX};
  b32 Result = (AreEqual(Index, &Test) == False);
  return Result;
}

link_internal hot_reloadable_file *
Push( hot_reloadable_file_block_array *Array, hot_reloadable_file *Element)
{
  Assert(Array->Memory);

  if (AtElements(Array) == Capacity(Array))
  {
    NewBlock(Array);
  }

  hot_reloadable_file *Result = Set(Array, Element, AtElements(Array));

  Array->ElementCount += 1;

  Assert(ValidateBlocksMatchCapacity(Array));
  return Result;
}

link_internal hot_reloadable_file *
Push( hot_reloadable_file_block_array *Array )
{
  hot_reloadable_file Element = {};
  auto Result = Push(Array, &Element);
  return Result;
}

link_internal void
Insert( hot_reloadable_file_block_array *Array, hot_reloadable_file_block_array_index Index, hot_reloadable_file *Element )
{
  Assert(Index.Index <= LastIndex(Array).Index);
  Assert(Array->Memory);

  // Alocate a new thingy
  hot_reloadable_file *Prev = Push(Array);

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
Insert( hot_reloadable_file_block_array *Array, u32 Index, hot_reloadable_file *Element )
{
  Insert(Array, { .Index = Index }, Element);
}

link_internal void
Shift( hot_reloadable_file_block_array *Array, hot_reloadable_file *Element )
{
  Insert(Array, { .Index = 0 }, Element);
}

/* element_t.has_tag(do_editor_ui)? */
/* { */
/*   do_editor_ui_for_container( block_array_t ) */
/* } */





link_internal hot_reloadable_file *
Pop( hot_reloadable_file_block_array *Array )
{
  hot_reloadable_file * Result = {};

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
ClearList( hot_reloadable_file_block_array *Array )
{
  IterateOver(Array, Element, ElementIndex)
  {
    *Element = {};
  }

  Array->ElementCount = 0;
}



