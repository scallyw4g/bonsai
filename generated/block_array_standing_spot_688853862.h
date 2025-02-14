// src/engine/world_chunk.h:489:0





struct standing_spot_block
{
  u32 Index;
  u32 At;
  standing_spot *Elements;
  standing_spot_block *Next;
};

struct standing_spot_block_array_index
{
  standing_spot_block *Block;
  u32 BlockIndex;
  u32 ElementIndex;
};

struct standing_spot_block_array
{
  standing_spot_block *First;
  standing_spot_block *Current;
  memory_arena *Memory; poof(@no_serialize)
  
};

link_internal standing_spot_block_array
StandingSpotBlockArray(memory_arena *Memory)
{
  standing_spot_block_array Result = {};
  Result.Memory = Memory;
  return Result;
}

link_internal b32
AreEqual(standing_spot_block_array_index *Thing1, standing_spot_block_array_index *Thing2)
{
  if (Thing1 && Thing2)
  {
        b32 Result = MemoryIsEqual((u8*)Thing1, (u8*)Thing2, sizeof( standing_spot_block_array_index ) );

    return Result;
  }
  else
  {
    return (Thing1 == Thing2);
  }
}

link_internal b32
AreEqual(standing_spot_block_array_index Thing1, standing_spot_block_array_index Thing2)
{
    b32 Result = MemoryIsEqual((u8*)&Thing1, (u8*)&Thing2, sizeof( standing_spot_block_array_index ) );

  return Result;
}


typedef standing_spot_block_array standing_spot_paged_list;

link_internal standing_spot_block_array_index
operator++( standing_spot_block_array_index &I0 )
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
operator<( standing_spot_block_array_index I0, standing_spot_block_array_index I1 )
{
  b32 Result = I0.BlockIndex < I1.BlockIndex || (I0.BlockIndex == I1.BlockIndex & I0.ElementIndex < I1.ElementIndex);
  return Result;
}

link_inline umm
GetIndex( standing_spot_block_array_index *Index)
{
  umm Result = Index->ElementIndex + (Index->BlockIndex*8);
  return Result;
}

link_internal standing_spot_block_array_index
ZerothIndex( standing_spot_block_array *Arr)
{
  standing_spot_block_array_index Result = {};
  Result.Block = Arr->First;
  return Result;
}

link_internal umm
TotalElements( standing_spot_block_array *Arr)
{
  umm Result = 0;
  if (Arr->Current)
  {
    Result = (Arr->Current->Index * 8) + Arr->Current->At;
  }
  return Result;
}

link_internal standing_spot_block_array_index
LastIndex( standing_spot_block_array *Arr)
{
  standing_spot_block_array_index Result = {};
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

link_internal standing_spot_block_array_index
AtElements( standing_spot_block_array *Arr)
{
  standing_spot_block_array_index Result = {};
  if (Arr->Current)
  {
    Result.Block = Arr->Current;
    Result.BlockIndex = Arr->Current->Index;
    Result.ElementIndex = Arr->Current->At;
  }
  return Result;
}

link_internal umm
Count( standing_spot_block_array *Arr)
{
  auto Index = AtElements(Arr);
  umm Result = GetIndex(&Index);
  return Result;
}

link_internal standing_spot *
Set( standing_spot_block_array *Arr,
  standing_spot *Element,
  standing_spot_block_array_index Index )
{
  standing_spot *Result = {};
  if (Index.Block)
  {
    Result = &Index.Block->Elements[Index.ElementIndex];
    *Result = *Element;
  }

  return Result;
}

link_internal standing_spot *
GetPtr(standing_spot_block_array *Arr, standing_spot_block_array_index Index)
{
  standing_spot *Result = {};
  if (Index.Block) { Result = (Index.Block->Elements + Index.ElementIndex); }
  return Result;
}

link_internal standing_spot *
GetPtr(standing_spot_block *Block, umm Index)
{
  standing_spot *Result = {};
  if (Index < Block->At) { Result = (Block->Elements + Index); }
  return Result;
}

link_internal standing_spot *
GetPtr(standing_spot_block_array *Arr, umm Index)
{
  umm BlockIndex = Index / 8;
  umm ElementIndex = Index % 8;

  umm AtBlock = 0;
  standing_spot_block *Block = Arr->First;
  while (AtBlock++ < BlockIndex)
  {
    Block = Block->Next;
  }

  standing_spot *Result = (Block->Elements+ElementIndex);
  return Result;
}

link_internal standing_spot *
TryGetPtr(standing_spot_block_array *Arr, umm Index)
{
  umm BlockIndex = Index / 8;
  umm ElementIndex = Index % 8;

  auto AtE = AtElements(Arr);
  umm Total = GetIndex(&AtE);
  standing_spot *Result = {};
  if (Index < Total) { Result = GetPtr(Arr, Index); }
  return Result;
}

link_internal u32
AtElements(standing_spot_block *Block)
{
  return Block->At;
}





link_internal standing_spot_block *
Allocate_standing_spot_block(memory_arena *Memory)
{
  standing_spot_block *Result = Allocate( standing_spot_block, Memory, 1);
  Result->Elements = Allocate( standing_spot, Memory, 8);
  return Result;
}

link_internal cs
CS( standing_spot_block_array_index Index )
{
  return FSz("(%u)(%u)", Index.BlockIndex, Index.ElementIndex);
}

link_internal void
RemoveUnordered( standing_spot_block_array *Array, standing_spot_block_array_index Index)
{
  standing_spot_block_array_index LastI = LastIndex(Array);

  standing_spot *Element = GetPtr(Array, Index);
  standing_spot *LastElement = GetPtr(Array, LastI);

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
      standing_spot_block *Current = Array->First;
      standing_spot_block *LastB = LastI.Block;

      while (Current->Next && Current->Next != LastB)
      {
        Current = Current->Next;
      }

      Assert(Current->Next == LastB || Current->Next == 0);
      Array->Current = Current;
    }
  }
}

link_internal standing_spot_block_array_index
Find( standing_spot_block_array *Array, standing_spot *Query)
{
  standing_spot_block_array_index Result = INVALID_BLOCK_ARRAY_INDEX;
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
IsValid(standing_spot_block_array_index *Index)
{
  standing_spot_block_array_index Test = INVALID_BLOCK_ARRAY_INDEX;
  b32 Result = (AreEqual(Index, &Test) == False);
  return Result;
}

link_internal standing_spot *
Push( standing_spot_block_array *Array, standing_spot *Element)
{
  Assert(Array->Memory);

  if (Array->First == 0) { Array->First = Allocate_standing_spot_block(Array->Memory); Array->Current = Array->First; }

  if (Array->Current->At == 8)
  {
    if (Array->Current->Next)
    {
      Array->Current = Array->Current->Next;
      Assert(Array->Current->At == 0);
    }
    else
    {
      standing_spot_block *Next = Allocate_standing_spot_block(Array->Memory);
      Next->Index = Array->Current->Index + 1;

      Array->Current->Next = Next;
      Array->Current = Next;
    }
  }

  standing_spot *Result = Array->Current->Elements + Array->Current->At;

  Array->Current->Elements[Array->Current->At++] = *Element;

  return Result;
}


