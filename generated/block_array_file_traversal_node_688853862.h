// callsite
// external/bonsai_stdlib/src/file.cpp:8:0

// def (block_array)
// external/bonsai_stdlib/src/poof_functions.h:2724:0




struct file_traversal_node_block
{
  /* u32 Index; */
  umm At;
  file_traversal_node Elements[8];
};


struct file_traversal_node_block_array_index
{
  umm Index; 
};

struct file_traversal_node_block_array
poof(
  @collection
  
  
)
{
  file_traversal_node_block **BlockPtrs; poof(@array_length(Element->BlockCount))
  u32   BlockCount;
  u32   ElementCount;
  memory_arena *Memory; poof(@no_serialize)
  
};

link_internal file_traversal_node_block_array
FileTraversalNodeBlockArray(memory_arena *Memory)
{
  file_traversal_node_block_array Result = {};
  Result.Memory = Memory;
  return Result;
}

link_internal b32
AreEqual(file_traversal_node_block_array_index *Thing1, file_traversal_node_block_array_index *Thing2)
{
  if (Thing1 && Thing2)
  {
        b32 Result = MemoryIsEqual((u8*)Thing1, (u8*)Thing2, sizeof( file_traversal_node_block_array_index ) );

    return Result;
  }
  else
  {
    return (Thing1 == Thing2);
  }
}

link_internal b32
AreEqual(file_traversal_node_block_array_index Thing1, file_traversal_node_block_array_index Thing2)
{
    b32 Result = MemoryIsEqual((u8*)&Thing1, (u8*)&Thing2, sizeof( file_traversal_node_block_array_index ) );

  return Result;
}


typedef file_traversal_node_block_array file_traversal_node_paged_list;

link_internal file_traversal_node_block_array_index
operator++( file_traversal_node_block_array_index &I0 )
{
  I0.Index++;
  return I0;
}

link_internal b32
operator<( file_traversal_node_block_array_index I0, file_traversal_node_block_array_index I1 )
{
  b32 Result = I0.Index < I1.Index;
  return Result;
}

link_internal b32
operator==( file_traversal_node_block_array_index I0, file_traversal_node_block_array_index I1 )
{
  b32 Result = I0.Index == I1.Index;
  return Result;
}

link_inline umm
GetIndex( file_traversal_node_block_array_index *Index)
{
  umm Result = Index->Index;
  return Result;
}


link_internal file_traversal_node_block_array_index
ZerothIndex( file_traversal_node_block_array *Arr )
{
  return {};
}

link_internal file_traversal_node_block_array_index
Capacity( file_traversal_node_block_array *Arr )
{
  file_traversal_node_block_array_index Result = {Arr->BlockCount * 8};
  return Result;
}

link_internal file_traversal_node_block_array_index
AtElements( file_traversal_node_block_array *Arr )
{
  file_traversal_node_block_array_index Result = {Arr->ElementCount};
  return Result;
}


link_internal umm
TotalElements( file_traversal_node_block_array *Arr )
{
  umm Result = AtElements(Arr).Index;
  return Result;
}


link_internal file_traversal_node_block_array_index
LastIndex( file_traversal_node_block_array *Arr )
{
  file_traversal_node_block_array_index Result = {};
  umm Count = AtElements(Arr).Index;
  if (Count) Result.Index = Count-1;
  return Result;
}

link_internal umm
Count( file_traversal_node_block_array *Arr )
{
  auto Result = AtElements(Arr).Index;
  return Result;
}

link_internal file_traversal_node_block *
GetBlock( file_traversal_node_block_array *Arr, file_traversal_node_block_array_index Index )
{
  umm BlockIndex   = Index.Index / 8;
  Assert(BlockIndex < Arr->BlockCount);
  file_traversal_node_block *Block = Arr->BlockPtrs[BlockIndex];
  return Block;
}

link_internal file_traversal_node *
GetPtr( file_traversal_node_block_array *Arr, file_traversal_node_block_array_index Index )
{
  Assert(Arr->BlockPtrs);
  Assert(Index.Index < Capacity(Arr).Index);

  file_traversal_node_block *Block = GetBlock(Arr, Index);

  umm ElementIndex = Index.Index % 8;
  file_traversal_node *Result = (Block->Elements + ElementIndex);
  return Result;
}


link_internal file_traversal_node *
GetPtr( file_traversal_node_block_array *Arr, umm Index )
{
  file_traversal_node_block_array_index I = {Index};
  return GetPtr(Arr, I);
}


link_internal file_traversal_node *
TryGetPtr( file_traversal_node_block_array *Arr, file_traversal_node_block_array_index Index)
{
  file_traversal_node * Result = {};
  if (Arr->BlockPtrs && Index < AtElements(Arr))
  {
    Result = GetPtr(Arr, Index);
  }
  return Result;
}

link_internal file_traversal_node *
TryGetPtr( file_traversal_node_block_array *Arr, umm Index)
{
  auto Result = TryGetPtr(Arr, file_traversal_node_block_array_index{Index});
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
CS( file_traversal_node_block_array_index Index )
{
  return FSz("(%u)", Index.Index);
}

link_internal file_traversal_node *
Set( file_traversal_node_block_array *Arr,
  file_traversal_node *Element,
  file_traversal_node_block_array_index Index )
{
  Assert(Arr->BlockPtrs);
  Assert(Index.Index < Capacity(Arr).Index);
  file_traversal_node_block *Block = GetBlock(Arr, Index);
  umm ElementIndex = Index.Index % 8;
  auto Slot = Block->Elements+ElementIndex;
  *Slot = *Element;
  return Slot;
}

link_internal void
NewBlock( file_traversal_node_block_array *Arr )
{
  file_traversal_node_block  *NewBlock     = Allocate( file_traversal_node_block , Arr->Memory,                 1);
  file_traversal_node_block **NewBlockPtrs = Allocate( file_traversal_node_block*, Arr->Memory, Arr->BlockCount+1);

  RangeIterator_t(u32, BlockI, Arr->BlockCount)
  {
    NewBlockPtrs[BlockI] = Arr->BlockPtrs[BlockI];
  }

  NewBlockPtrs[Arr->BlockCount] = NewBlock;

  
  
  Arr->BlockPtrs = NewBlockPtrs;
  Arr->BlockCount += 1;
}

link_internal void
RemoveUnordered( file_traversal_node_block_array *Array, file_traversal_node_block_array_index Index)
{
  auto LastI = LastIndex(Array);
  Assert(Index.Index <= LastI.Index);

  auto LastElement = GetPtr(Array, LastI);
  Set(Array, LastElement, Index);
  Array->ElementCount -= 1;
}

link_internal void
RemoveOrdered( file_traversal_node_block_array *Array, file_traversal_node_block_array_index IndexToRemove)
{
  Assert(IndexToRemove.Index < Array->ElementCount);

  file_traversal_node *Prev = {};

  file_traversal_node_block_array_index Max = AtElements(Array);
  RangeIteratorRange_t(umm, Index, Max.Index, IndexToRemove.Index)
  {
    file_traversal_node *E = GetPtr(Array, Index);

    if (Prev)
    {
      *Prev = *E;
    }

    Prev = E;
  }

  Array->ElementCount -= 1;
}

link_internal void
RemoveOrdered( file_traversal_node_block_array *Array, file_traversal_node *Element )
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

link_internal file_traversal_node_block_array_index
Find( file_traversal_node_block_array *Array, file_traversal_node *Query)
{
  file_traversal_node_block_array_index Result = {INVALID_BLOCK_ARRAY_INDEX};
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
IsValid(file_traversal_node_block_array_index *Index)
{
  file_traversal_node_block_array_index Test = {INVALID_BLOCK_ARRAY_INDEX};
  b32 Result = (AreEqual(Index, &Test) == False);
  return Result;
}

link_internal file_traversal_node *
Push( file_traversal_node_block_array *Array, file_traversal_node *Element)
{
  Assert(Array->Memory);

  if (AtElements(Array) == Capacity(Array))
  {
    NewBlock(Array);
  }

  file_traversal_node *Result = Set(Array, Element, AtElements(Array));

  Array->ElementCount += 1;

  return Result;
}

link_internal file_traversal_node *
Push( file_traversal_node_block_array *Array )
{
  file_traversal_node Element = {};
  auto Result = Push(Array, &Element);
  return Result;
}

link_internal void
Insert( file_traversal_node_block_array *Array, file_traversal_node_block_array_index Index, file_traversal_node *Element )
{
  Assert(Index.Index <= LastIndex(Array).Index);
  Assert(Array->Memory);

  // Alocate a new thingy
  file_traversal_node *Prev = Push(Array);

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
Insert( file_traversal_node_block_array *Array, u32 Index, file_traversal_node *Element )
{
  Insert(Array, { .Index = Index }, Element);
}

link_internal void
Shift( file_traversal_node_block_array *Array, file_traversal_node *Element )
{
  Insert(Array, { .Index = 0 }, Element);
}

/* element_t.has_tag(do_editor_ui)? */
/* { */
/*   do_editor_ui_for_container( block_array_t ) */
/* } */


link_internal file_traversal_node *
Pop( file_traversal_node_block_array *Array )
{
  if (auto Result = TryGetPtr(Array, LastIndex(Array)))
  {
    Assert(Array->ElementCount > 0);
    Array->ElementCount -= 1;
    return Result;
  }
  return 0;
}



