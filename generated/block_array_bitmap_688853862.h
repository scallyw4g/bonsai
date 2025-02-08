// external/bonsai_stdlib/src/bitmap.cpp:182:0





struct bitmap_block
{
  u32 Index;
  u32 At;
  bitmap *Elements;
  bitmap_block *Next;
};

struct bitmap_block_array_index
{
  bitmap_block *Block;
  u32 BlockIndex;
  u32 ElementIndex;
};

struct bitmap_block_array
{
  bitmap_block *First;
  bitmap_block *Current;
  memory_arena *Memory; poof(@no_serialize)
  
};

link_internal b32
AreEqual(bitmap_block_array_index *Thing1, bitmap_block_array_index *Thing2)
{
  if (Thing1 && Thing2)
  {
        b32 Result = MemoryIsEqual((u8*)Thing1, (u8*)Thing2, sizeof( bitmap_block_array_index ) );

    return Result;
  }
  else
  {
    return (Thing1 == Thing2);
  }
}

link_internal b32
AreEqual(bitmap_block_array_index Thing1, bitmap_block_array_index Thing2)
{
    b32 Result = MemoryIsEqual((u8*)&Thing1, (u8*)&Thing2, sizeof( bitmap_block_array_index ) );

  return Result;
}


typedef bitmap_block_array bitmap_paged_list;

link_internal bitmap_block_array_index
operator++( bitmap_block_array_index &I0 )
{
  if (I0.Block)
  {
    if (I0.ElementIndex == 8-1)
    {
      I0.ElementIndex = 0;
      I0.BlockIndex++;
      I0.Block = I0.Block->Next;
    }
    else
    {
      I0.ElementIndex++;
    }
  }
  else
  {
    I0.ElementIndex++;
  }
  return I0;
}

link_internal b32
operator<( bitmap_block_array_index I0, bitmap_block_array_index I1 )
{
  b32 Result = I0.BlockIndex < I1.BlockIndex || (I0.BlockIndex == I1.BlockIndex & I0.ElementIndex < I1.ElementIndex);
  return Result;
}

link_inline umm
GetIndex( bitmap_block_array_index *Index)
{
  umm Result = Index->ElementIndex + (Index->BlockIndex*8);
  return Result;
}

link_internal bitmap_block_array_index
ZerothIndex( bitmap_block_array *Arr)
{
  bitmap_block_array_index Result = {};
  Result.Block = Arr->First;
  return Result;
}

link_internal umm
TotalElements( bitmap_block_array *Arr)
{
  umm Result = 0;
  if (Arr->Current)
  {
    Result = (Arr->Current->Index * 8) + Arr->Current->At;
  }
  return Result;
}

link_internal bitmap_block_array_index
LastIndex( bitmap_block_array *Arr)
{
  bitmap_block_array_index Result = {};
  if (Arr->Current)
  {
    Result.Block = Arr->Current;
    Result.BlockIndex = Arr->Current->Index;
    Result.ElementIndex = Arr->Current->At;
    Assert(Result.ElementIndex);
    Result.ElementIndex--;
  }
  return Result;
}

link_internal bitmap_block_array_index
AtElements( bitmap_block_array *Arr)
{
  bitmap_block_array_index Result = {};
  if (Arr->Current)
  {
    Result.Block = Arr->Current;
    Result.BlockIndex = Arr->Current->Index;
    Result.ElementIndex = Arr->Current->At;
  }
  return Result;
}

link_internal umm
Count( bitmap_block_array *Arr)
{
  auto Index = AtElements(Arr);
  umm Result = GetIndex(&Index);
  return Result;
}

link_internal bitmap *
Set( bitmap_block_array *Arr,
  bitmap *Element,
  bitmap_block_array_index Index )
{
  bitmap *Result = {};
  if (Index.Block)
  {
    Result = &Index.Block->Elements[Index.ElementIndex];
    *Result = *Element;
  }

  return Result;
}

link_internal bitmap *
GetPtr(bitmap_block_array *Arr, bitmap_block_array_index Index)
{
  bitmap *Result = {};
  if (Index.Block) { Result = (Index.Block->Elements + Index.ElementIndex); }
  return Result;
}

link_internal bitmap *
GetPtr(bitmap_block *Block, umm Index)
{
  bitmap *Result = {};
  if (Index < Block->At) { Result = (Block->Elements + Index); }
  return Result;
}

link_internal bitmap *
GetPtr(bitmap_block_array *Arr, umm Index)
{
  umm BlockIndex = Index / 8;
  umm ElementIndex = Index % 8;

  umm AtBlock = 0;
  bitmap_block *Block = Arr->First;
  while (AtBlock++ < BlockIndex)
  {
    Block = Block->Next;
  }

  bitmap *Result = (Block->Elements+ElementIndex);
  return Result;
}

link_internal bitmap *
TryGetPtr(bitmap_block_array *Arr, umm Index)
{
  umm BlockIndex = Index / 8;
  umm ElementIndex = Index % 8;

  auto AtE = AtElements(Arr);
  umm Total = GetIndex(&AtE);
  bitmap *Result = {};
  if (Index < Total) { Result = GetPtr(Arr, Index); }
  return Result;
}

link_internal u32
AtElements(bitmap_block *Block)
{
  return Block->At;
}





link_internal bitmap_block_array
BitmapBlockArray(memory_arena *Memory)
{
  bitmap_block_array Result = {};
  Result.Memory = Memory;
  return Result;
}

link_internal bitmap_block *
Allocate_bitmap_block(memory_arena *Memory)
{
  bitmap_block *Result = Allocate( bitmap_block, Memory, 1);
  Result->Elements = Allocate( bitmap, Memory, 8);
  return Result;
}

link_internal cs
CS( bitmap_block_array_index Index )
{
  return FSz("(%u)(%u)", Index.BlockIndex, Index.ElementIndex);
}

link_internal void
RemoveUnordered( bitmap_block_array *Array, bitmap_block_array_index Index)
{
  bitmap_block_array_index LastI = LastIndex(Array);

  bitmap *Element = GetPtr(Array, Index);
  bitmap *LastElement = GetPtr(Array, LastI);

  Set(Array, LastElement, Index);

  Assert(Array->Current->At);
  Array->Current->At -= 1;

  if (Array->Current->At == 0)
  {
    // TODO(Jesse): There's obviously a way better way to do this ..
    auto AtE = AtElements(Array);
    s32 Count = s32(GetIndex(&AtE));

    if (Count == 0)
    {
      // Nothing to be done, we've popping the last thing off the array
      Assert(Index.Block == Array->First);
      Assert(Index.Block == Array->Current);
      Assert(Index.BlockIndex == 0);
      Assert(Index.ElementIndex == 0);
    }
    else
    {
      // Walk the chain till we get to the second-last one
      bitmap_block *Current = Array->First;
      bitmap_block *LastB = LastI.Block;

      while (Current->Next && Current->Next != LastB)
      {
        Current = Current->Next;
      }

      Assert(Current->Next == LastB || Current->Next == 0);
      Array->Current = Current;
    }
  }
}

link_internal bitmap_block_array_index
Find( bitmap_block_array *Array, bitmap *Query)
{
  bitmap_block_array_index Result = INVALID_BLOCK_ARRAY_INDEX;
  IterateOver(Array, E, Index)
  {
    if ( E == Query)
    {
      Result = Index;
      break;
    }
  }
  return Result;
}

link_internal b32
IsValid(bitmap_block_array_index *Index)
{
  bitmap_block_array_index Test = INVALID_BLOCK_ARRAY_INDEX;
  b32 Result = AreEqual(Index, &Test);
  /* b32 Result = False; */
  return Result;
}

link_internal bitmap *
Push( bitmap_block_array *Array, bitmap *Element)
{
  if (Array->Memory == 0) { Array->Memory = AllocateArena(); }

  if (Array->First == 0) { Array->First = Allocate_bitmap_block(Array->Memory); Array->Current = Array->First; }

  if (Array->Current->At == 8)
  {
    if (Array->Current->Next)
    {
      Array->Current = Array->Current->Next;
      Assert(Array->Current->At == 0);
    }
    else
    {
      bitmap_block *Next = Allocate_bitmap_block(Array->Memory);
      Next->Index = Array->Current->Index + 1;

      Array->Current->Next = Next;
      Array->Current = Next;
    }
  }

  bitmap *Result = Array->Current->Elements + Array->Current->At;

  Array->Current->Elements[Array->Current->At++] = *Element;

  return Result;
}


