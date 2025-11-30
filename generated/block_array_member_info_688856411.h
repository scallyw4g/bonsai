// callsite
// src/engine/bonsai_type_info.h:11:0

// def (block_array)
// external/bonsai_stdlib/src/poof_functions.h:2724:0




struct member_info_block
{
  /* u32 Index; */
  umm At;
  member_info Elements[8];
};


struct member_info_block_array_index
{
  umm Index; 
};

struct member_info_block_array
poof(
  @collection
  
  
)
{
  member_info_block **BlockPtrs; poof(@array_length(Element->BlockCount))
  u32   BlockCount;
  u32   ElementCount;
  memory_arena *Memory; poof(@no_serialize)
  
};

link_internal member_info_block_array
MemberInfoBlockArray(memory_arena *Memory)
{
  member_info_block_array Result = {};
  Result.Memory = Memory;
  return Result;
}

link_internal b32
AreEqual(member_info_block_array_index *Thing1, member_info_block_array_index *Thing2)
{
  if (Thing1 && Thing2)
  {
        b32 Result = MemoryIsEqual((u8*)Thing1, (u8*)Thing2, sizeof( member_info_block_array_index ) );

    return Result;
  }
  else
  {
    return (Thing1 == Thing2);
  }
}

link_internal b32
AreEqual(member_info_block_array_index Thing1, member_info_block_array_index Thing2)
{
    b32 Result = MemoryIsEqual((u8*)&Thing1, (u8*)&Thing2, sizeof( member_info_block_array_index ) );

  return Result;
}


typedef member_info_block_array member_info_paged_list;

link_internal member_info_block_array_index
operator++( member_info_block_array_index &I0 )
{
  I0.Index++;
  return I0;
}

link_internal b32
operator<( member_info_block_array_index I0, member_info_block_array_index I1 )
{
  b32 Result = I0.Index < I1.Index;
  return Result;
}

link_internal b32
operator==( member_info_block_array_index I0, member_info_block_array_index I1 )
{
  b32 Result = I0.Index == I1.Index;
  return Result;
}

link_inline umm
GetIndex( member_info_block_array_index *Index)
{
  umm Result = Index->Index;
  return Result;
}


link_internal member_info_block_array_index
ZerothIndex( member_info_block_array *Arr )
{
  return {};
}

link_internal member_info_block_array_index
Capacity( member_info_block_array *Arr )
{
  member_info_block_array_index Result = {Arr->BlockCount * 8};
  return Result;
}

link_internal member_info_block_array_index
AtElements( member_info_block_array *Arr )
{
  member_info_block_array_index Result = {Arr->ElementCount};
  return Result;
}


link_internal umm
TotalElements( member_info_block_array *Arr )
{
  umm Result = AtElements(Arr).Index;
  return Result;
}


link_internal member_info_block_array_index
LastIndex( member_info_block_array *Arr )
{
  member_info_block_array_index Result = {};
  umm Count = AtElements(Arr).Index;
  if (Count) Result.Index = Count-1;
  return Result;
}

link_internal umm
Count( member_info_block_array *Arr )
{
  auto Result = AtElements(Arr).Index;
  return Result;
}

link_internal member_info_block *
GetBlock( member_info_block_array *Arr, member_info_block_array_index Index )
{
  umm BlockIndex   = Index.Index / 8;
  Assert(BlockIndex < Arr->BlockCount);
  member_info_block *Block = Arr->BlockPtrs[BlockIndex];
  return Block;
}

link_internal member_info *
GetPtr( member_info_block_array *Arr, member_info_block_array_index Index )
{
  Assert(Arr->BlockPtrs);
  Assert(Index.Index < Capacity(Arr).Index);

  member_info_block *Block = GetBlock(Arr, Index);

  umm ElementIndex = Index.Index % 8;
  member_info *Result = (Block->Elements + ElementIndex);
  return Result;
}


link_internal member_info *
GetPtr( member_info_block_array *Arr, umm Index )
{
  member_info_block_array_index I = {Index};
  return GetPtr(Arr, I);
}


link_internal member_info *
TryGetPtr( member_info_block_array *Arr, member_info_block_array_index Index)
{
  member_info * Result = {};
  if (Arr->BlockPtrs && Index < AtElements(Arr))
  {
    Result = GetPtr(Arr, Index);
  }
  return Result;
}

link_internal member_info *
TryGetPtr( member_info_block_array *Arr, umm Index)
{
  auto Result = TryGetPtr(Arr, member_info_block_array_index{Index});
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
CS( member_info_block_array_index Index )
{
  return FSz("(%u)", Index.Index);
}

link_internal member_info *
Set( member_info_block_array *Arr,
  member_info *Element,
  member_info_block_array_index Index )
{
  Assert(Arr->BlockPtrs);
  Assert(Index.Index < Capacity(Arr).Index);
  member_info_block *Block = GetBlock(Arr, Index);
  umm ElementIndex = Index.Index % 8;
  auto Slot = Block->Elements+ElementIndex;
  *Slot = *Element;
  return Slot;
}

link_internal void
NewBlock( member_info_block_array *Arr )
{
  member_info_block  *NewBlock     = Allocate( member_info_block , Arr->Memory,                 1);
  member_info_block **NewBlockPtrs = Allocate( member_info_block*, Arr->Memory, Arr->BlockCount+1);

  RangeIterator_t(u32, BlockI, Arr->BlockCount)
  {
    NewBlockPtrs[BlockI] = Arr->BlockPtrs[BlockI];
  }

  NewBlockPtrs[Arr->BlockCount] = NewBlock;

  
  
  Arr->BlockPtrs = NewBlockPtrs;
  Arr->BlockCount += 1;
}

link_internal void
RemoveUnordered( member_info_block_array *Array, member_info_block_array_index Index)
{
  auto LastI = LastIndex(Array);
  Assert(Index.Index <= LastI.Index);

  auto LastElement = GetPtr(Array, LastI);
  Set(Array, LastElement, Index);
  Array->ElementCount -= 1;
}

link_internal void
RemoveOrdered( member_info_block_array *Array, member_info_block_array_index IndexToRemove)
{
  Assert(IndexToRemove.Index < Array->ElementCount);

  member_info *Prev = {};

  member_info_block_array_index Max = AtElements(Array);
  RangeIteratorRange_t(umm, Index, Max.Index, IndexToRemove.Index)
  {
    member_info *E = GetPtr(Array, Index);

    if (Prev)
    {
      *Prev = *E;
    }

    Prev = E;
  }

  Array->ElementCount -= 1;
}

link_internal void
RemoveOrdered( member_info_block_array *Array, member_info *Element )
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

link_internal member_info_block_array_index
Find( member_info_block_array *Array, member_info *Query)
{
  member_info_block_array_index Result = {INVALID_BLOCK_ARRAY_INDEX};
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
IsValid(member_info_block_array_index *Index)
{
  member_info_block_array_index Test = {INVALID_BLOCK_ARRAY_INDEX};
  b32 Result = (AreEqual(Index, &Test) == False);
  return Result;
}

link_internal member_info *
Push( member_info_block_array *Array, member_info *Element)
{
  Assert(Array->Memory);

  if (AtElements(Array) == Capacity(Array))
  {
    NewBlock(Array);
  }

  member_info *Result = Set(Array, Element, AtElements(Array));

  Array->ElementCount += 1;

  return Result;
}

link_internal member_info *
Push( member_info_block_array *Array )
{
  member_info Element = {};
  auto Result = Push(Array, &Element);
  return Result;
}

link_internal void
Insert( member_info_block_array *Array, member_info_block_array_index Index, member_info *Element )
{
  Assert(Index.Index <= LastIndex(Array).Index);
  Assert(Array->Memory);

  // Alocate a new thingy
  member_info *Prev = Push(Array);

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
Insert( member_info_block_array *Array, u32 Index, member_info *Element )
{
  Insert(Array, { .Index = Index }, Element);
}

link_internal void
Shift( member_info_block_array *Array, member_info *Element )
{
  Insert(Array, { .Index = 0 }, Element);
}

/* element_t.has_tag(do_editor_ui)? */
/* { */
/*   do_editor_ui_for_container( block_array_t ) */
/* } */


link_internal member_info *
Pop( member_info_block_array *Array )
{
  if (auto Result = TryGetPtr(Array, LastIndex(Array)))
  {
    Assert(Array->ElementCount > 0);
    Array->ElementCount -= 1;
    return Result;
  }
  return 0;
}



