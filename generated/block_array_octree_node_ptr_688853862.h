// src/engine/world.h:85:0





struct octree_node_ptr_block
{
  u32 Index;
  u32 At;
  octree_node_ptr *Elements;
  octree_node_ptr_block *Next;
};

struct octree_node_ptr_block_array_index
{
  octree_node_ptr_block *Block;
  u32 BlockIndex;
  u32 ElementIndex;
};

struct octree_node_ptr_block_array
{
  octree_node_ptr_block *First;
  octree_node_ptr_block *Current;
  memory_arena *Memory; poof(@no_serialize)
  
};

link_internal octree_node_ptr_block_array
OctreeNodePtrBlockArray(memory_arena *Memory)
{
  octree_node_ptr_block_array Result = {};
  Result.Memory = Memory;
  return Result;
}

link_internal b32
AreEqual(octree_node_ptr_block_array_index *Thing1, octree_node_ptr_block_array_index *Thing2)
{
  if (Thing1 && Thing2)
  {
        b32 Result = MemoryIsEqual((u8*)Thing1, (u8*)Thing2, sizeof( octree_node_ptr_block_array_index ) );

    return Result;
  }
  else
  {
    return (Thing1 == Thing2);
  }
}

link_internal b32
AreEqual(octree_node_ptr_block_array_index Thing1, octree_node_ptr_block_array_index Thing2)
{
    b32 Result = MemoryIsEqual((u8*)&Thing1, (u8*)&Thing2, sizeof( octree_node_ptr_block_array_index ) );

  return Result;
}


typedef octree_node_ptr_block_array octree_node_ptr_paged_list;

link_internal octree_node_ptr_block_array_index
operator++( octree_node_ptr_block_array_index &I0 )
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
operator<( octree_node_ptr_block_array_index I0, octree_node_ptr_block_array_index I1 )
{
  b32 Result = I0.BlockIndex < I1.BlockIndex || (I0.BlockIndex == I1.BlockIndex & I0.ElementIndex < I1.ElementIndex);
  return Result;
}

link_inline umm
GetIndex( octree_node_ptr_block_array_index *Index)
{
  umm Result = Index->ElementIndex + (Index->BlockIndex*8);
  return Result;
}

link_internal octree_node_ptr_block_array_index
ZerothIndex( octree_node_ptr_block_array *Arr)
{
  octree_node_ptr_block_array_index Result = {};
  Result.Block = Arr->First;
  return Result;
}

link_internal umm
TotalElements( octree_node_ptr_block_array *Arr)
{
  umm Result = 0;
  if (Arr->Current)
  {
    Result = (Arr->Current->Index * 8) + Arr->Current->At;
  }
  return Result;
}

link_internal octree_node_ptr_block_array_index
LastIndex( octree_node_ptr_block_array *Arr)
{
  octree_node_ptr_block_array_index Result = {};
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

link_internal octree_node_ptr_block_array_index
AtElements( octree_node_ptr_block_array *Arr)
{
  octree_node_ptr_block_array_index Result = {};
  if (Arr->Current)
  {
    Result.Block = Arr->Current;
    Result.BlockIndex = Arr->Current->Index;
    Result.ElementIndex = Arr->Current->At;
  }
  return Result;
}

link_internal umm
Count( octree_node_ptr_block_array *Arr)
{
  auto Index = AtElements(Arr);
  umm Result = GetIndex(&Index);
  return Result;
}

link_internal octree_node_ptr 
GetPtr(octree_node_ptr_block_array *Arr, octree_node_ptr_block_array_index Index)
{
  octree_node_ptr Result = {};
  if (Index.Block) { Result = *(Index.Block->Elements + Index.ElementIndex); }
  return Result;
}

link_internal octree_node_ptr 
GetPtr(octree_node_ptr_block *Block, umm Index)
{
  octree_node_ptr Result = {};
  if (Index < Block->At) { Result = *(Block->Elements + Index); }
  return Result;
}

link_internal octree_node_ptr 
GetPtr(octree_node_ptr_block_array *Arr, umm Index)
{
  umm BlockIndex = Index / 8;
  umm ElementIndex = Index % 8;

  umm AtBlock = 0;
  octree_node_ptr_block *Block = Arr->First;
  while (AtBlock++ < BlockIndex)
  {
    Block = Block->Next;
  }

  octree_node_ptr Result = *(Block->Elements+ElementIndex);
  return Result;
}

link_internal octree_node_ptr 
TryGetPtr(octree_node_ptr_block_array *Arr, umm Index)
{
  umm BlockIndex = Index / 8;
  umm ElementIndex = Index % 8;

  auto AtE = AtElements(Arr);
  umm Total = GetIndex(&AtE);
  octree_node_ptr Result = {};
  if (Index < Total) { Result = GetPtr(Arr, Index); }
  return Result;
}

link_internal u32
AtElements(octree_node_ptr_block *Block)
{
  return Block->At;
}





link_internal octree_node_ptr_block *
Allocate_octree_node_ptr_block(memory_arena *Memory)
{
  octree_node_ptr_block *Result = Allocate( octree_node_ptr_block, Memory, 1);
  Result->Elements = Allocate( octree_node_ptr, Memory, 8);
  return Result;
}

link_internal cs
CS( octree_node_ptr_block_array_index Index )
{
  return FSz("(%u)(%u)", Index.BlockIndex, Index.ElementIndex);
}

link_internal octree_node_ptr 
Set( octree_node_ptr_block_array *Arr,
  octree_node_ptr Element,
  octree_node_ptr_block_array_index Index )
{
  octree_node_ptr Result = {};
  if (Index.Block)
  {
    octree_node_ptr *Slot = &Index.Block->Elements[Index.ElementIndex];
    *Slot = Element;

    Result = *Slot;
  }

  return Result;
}

link_internal void
RemoveUnordered( octree_node_ptr_block_array *Array, octree_node_ptr_block_array_index Index)
{
  octree_node_ptr_block_array_index LastI = LastIndex(Array);

  octree_node_ptr Element = GetPtr(Array, Index);
  octree_node_ptr LastElement = GetPtr(Array, LastI);

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
      octree_node_ptr_block *Current = Array->First;
      octree_node_ptr_block *LastB = LastI.Block;

      while (Current->Next && Current->Next != LastB)
      {
        Current = Current->Next;
      }

      Assert(Current->Next == LastB || Current->Next == 0);
      Array->Current = Current;
    }
  }
}

link_internal void
RemoveOrdered( octree_node_ptr_block_array *Array, octree_node_ptr_block_array_index Index)
{
  auto End = AtElements(Array);
  auto   AtI = Index;
  auto NextI = Index;
  ++NextI;

  while (NextI < End)
  {
    auto At    =  GetPtr(Array, AtI);
    auto NextV = *GetPtr(Array, NextI);

    *At = NextV;

    ++AtI;
    ++NextI;
  }

  RemoveUnordered(Array, NextI);
}

link_internal void
RemoveOrdered( octree_node_ptr_block_array *Array, octree_node_ptr Element )
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

link_internal octree_node_ptr_block_array_index
Find( octree_node_ptr_block_array *Array, octree_node_ptr Query)
{
  octree_node_ptr_block_array_index Result = INVALID_BLOCK_ARRAY_INDEX;
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
IsValid(octree_node_ptr_block_array_index *Index)
{
  octree_node_ptr_block_array_index Test = INVALID_BLOCK_ARRAY_INDEX;
  b32 Result = (AreEqual(Index, &Test) == False);
  return Result;
}

link_internal octree_node_ptr *
Push( octree_node_ptr_block_array *Array, octree_node_ptr *Element)
{
  Assert(Array->Memory);

  if (Array->First == 0) { Array->First = Allocate_octree_node_ptr_block(Array->Memory); Array->Current = Array->First; }

  if (Array->Current->At == 8)
  {
    if (Array->Current->Next)
    {
      Array->Current = Array->Current->Next;
      Assert(Array->Current->At == 0);
    }
    else
    {
      octree_node_ptr_block *Next = Allocate_octree_node_ptr_block(Array->Memory);
      Next->Index = Array->Current->Index + 1;

      Array->Current->Next = Next;
      Array->Current = Next;
    }
  }

  octree_node_ptr *Result = Array->Current->Elements + Array->Current->At;

  Array->Current->Elements[Array->Current->At++] = *Element;

  return Result;
}

link_internal octree_node_ptr *
Push( octree_node_ptr_block_array *Array )
{
  octree_node_ptr Element = {};
  auto Result = Push(Array, &Element);
  return Result;
}


