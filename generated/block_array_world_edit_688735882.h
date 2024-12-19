// src/engine/editor.h:1160:0

struct world_edit_block
{
  u32 Index;
  u32 At;
  world_edit *Elements;
  world_edit_block *Next;
};

struct world_edit_block_array_index
{
  world_edit_block *Block;
  u32 BlockIndex;
  u32 ElementIndex;
};

struct world_edit_block_array
{
  world_edit_block *First;
  world_edit_block *Current;
  memory_arena *Memory; poof(@no_serialize)
  
};

typedef world_edit_block_array world_edit_paged_list;

link_internal world_edit_block_array_index
operator++(world_edit_block_array_index &I0)
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
operator<(world_edit_block_array_index I0, world_edit_block_array_index I1)
{
  b32 Result = I0.BlockIndex < I1.BlockIndex || (I0.BlockIndex == I1.BlockIndex & I0.ElementIndex < I1.ElementIndex);
  return Result;
}

link_inline umm
GetIndex(world_edit_block_array_index *Index)
{
  umm Result = Index->ElementIndex + (Index->BlockIndex*8);
  return Result;
}

link_internal world_edit_block_array_index
ZerothIndex(world_edit_block_array *Arr)
{
  world_edit_block_array_index Result = {};
  Result.Block = Arr->First;
  /* Assert(Result.Block->Index == 0); */
  return Result;
}

link_internal umm
TotalElements(world_edit_block_array *Arr)
{
  umm Result = 0;
  if (Arr->Current)
  {
    Result = (Arr->Current->Index * 8) + Arr->Current->At;
  }
  return Result;
}

link_internal world_edit_block_array_index
LastIndex(world_edit_block_array *Arr)
{
  world_edit_block_array_index Result = {};
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

link_internal world_edit_block_array_index
AtElements(world_edit_block_array *Arr)
{
  world_edit_block_array_index Result = {};
  if (Arr->Current)
  {
    Result.Block = Arr->Current;
    Result.BlockIndex = Arr->Current->Index;
    Result.ElementIndex = Arr->Current->At;
  }
  return Result;
}

link_internal umm
Count(world_edit_block_array *Arr)
{
  auto Index = AtElements(Arr);
  umm Result = GetIndex(&Index);
  return Result;
}

link_internal world_edit *
GetPtr(world_edit_block_array *Arr, world_edit_block_array_index Index)
{
  world_edit *Result = {};
  if (Index.Block) { Result = Index.Block->Elements + Index.ElementIndex; }
  return Result;
}

link_internal world_edit *
GetPtr(world_edit_block *Block, umm Index)
{
  world_edit *Result = 0;
  if (Index < Block->At) { Result = Block->Elements + Index; }
  return Result;
}

link_internal world_edit *
GetPtr(world_edit_block_array *Arr, umm Index)
{
  umm BlockIndex = Index / 8;
  umm ElementIndex = Index % 8;

  umm AtBlock = 0;
  world_edit_block *Block = Arr->First;
  while (AtBlock++ < BlockIndex)
  {
    Block = Block->Next;
  }

  world_edit *Result = Block->Elements+ElementIndex;
  return Result;
}

link_internal world_edit *
TryGetPtr(world_edit_block_array *Arr, umm Index)
{
  umm BlockIndex = Index / 8;
  umm ElementIndex = Index % 8;

  auto AtE = AtElements(Arr);
  umm Total = GetIndex(&AtE);
  world_edit *Result = {};
  if (Index < Total) { Result = GetPtr(Arr, Index); }
  return Result;
}

link_internal u32
AtElements(world_edit_block *Block)
{
  return Block->At;
}


link_internal world_edit_block*
Allocate_world_edit_block(memory_arena *Memory)
{
  world_edit_block *Result = Allocate(world_edit_block, Memory, 1);
  Result->Elements = Allocate(world_edit, Memory, 8);
  return Result;
}

link_internal cs
CS(world_edit_block_array_index Index)
{
  return FSz("(%u)(%u)", Index.BlockIndex, Index.ElementIndex);
}

link_internal void
RemoveUnordered(world_edit_block_array *Array, world_edit_block_array_index Index)
{
  world_edit_block_array_index LastI = LastIndex(Array);

  world_edit *Element = GetPtr(Array, Index);
  world_edit *LastElement = GetPtr(Array, LastI);

  *Element = *LastElement;

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
      world_edit_block *Current = Array->First;
      world_edit_block *LastB = LastI.Block;

      while (Current->Next && Current->Next != LastB)
      {
        Current = Current->Next;
      }

      Assert(Current->Next == LastB || Current->Next == 0);
      Array->Current = Current;
    }
  }
}

link_internal world_edit *
Push(world_edit_block_array *Array, world_edit *Element)
{
  if (Array->Memory == 0) { Array->Memory = AllocateArena(); }

  if (Array->First == 0) { Array->First = Allocate_world_edit_block(Array->Memory); Array->Current = Array->First; }

  if (Array->Current->At == 8)
  {
    if (Array->Current->Next)
    {
      Array->Current = Array->Current->Next;
      Assert(Array->Current->At == 0);
    }
    else
    {
      world_edit_block *Next = Allocate_world_edit_block(Array->Memory);
      Next->Index = Array->Current->Index + 1;

      Array->Current->Next = Next;
      Array->Current = Next;
    }
  }

  world_edit *Result = Array->Current->Elements + Array->Current->At;

  Array->Current->Elements[Array->Current->At++] = *Element;

  return Result;
}


