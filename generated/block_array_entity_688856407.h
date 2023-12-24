struct entity_block
{
  u32 Index;
  u32 At;
  entity *Elements;
  entity_block *Next;
};

struct entity_block_array_index
{
  void *Block;
  u32 BlockIndex;
  u32 ElementIndex;
};

struct entity_block_array
{
  entity_block First;
  entity_block *Current;
  memory_arena *Memory;
};

link_internal entity_block_array_index
operator++(entity_block_array_index &I0)
{
  if (I0.Block)
  {
    if (I0.ElementIndex == 8-1)
    {
      I0.ElementIndex = 0;
      I0.BlockIndex++;
      I0.Block = Cast(entity_block*, I0.Block)->Next;
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
operator<(entity_block_array_index I0, entity_block_array_index I1)
{
  b32 Result = I0.BlockIndex < I1.BlockIndex || (I0.BlockIndex == I1.BlockIndex & I0.ElementIndex < I1.ElementIndex);
  return Result;
}

link_inline entity_block *
GetBlock(entity_block_array_index *Index)
{
  entity_block *Result = Cast(entity_block*, Index->Block);
  return Result;
}

link_inline umm
GetIndex(entity_block_array_index *Index)
{
  umm Result = Index->ElementIndex + (Index->BlockIndex*8);
  return Result;
}

link_internal entity_block_array_index
ZerothIndex(entity_block_array *Arr)
{
  entity_block_array_index Result = {};
  Result.Block = &Arr->First;
  Assert(GetBlock(&Result)->Index == 0);
  return Result;
}

link_internal umm
TotalElements(entity_block_array *Arr)
{
  umm Result = 0;
  if (Arr->Current)
  {
    Result = (Arr->Current->Index * 8) + Arr->Current->At;
  }
  return Result;
}

link_internal entity_block_array_index
LastIndex(entity_block_array *Arr)
{
  entity_block_array_index Result = {};
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

link_internal entity_block_array_index
AtElements(entity_block_array *Arr)
{
  entity_block_array_index Result = {};
  if (Arr->Current)
  {
    Result.Block = Arr->Current;
    Result.BlockIndex = Arr->Current->Index;
    Result.ElementIndex = Arr->Current->At;
    /* Assert(Result.ElementIndex); */
    /* Result.ElementIndex--; */
  }
  return Result;
}

link_internal entity *
GetPtr(entity_block_array *Arr, entity_block_array_index Index)
{
  entity *Result = {};
  if (Index.Block) { Result = GetBlock(&Index)->Elements + Index.ElementIndex; }
  return Result;
}

link_internal entity *
GetPtr(entity_block *Block, umm Index)
{
  entity *Result = 0;
  if (Index < Block->At) { Result = Block->Elements + Index; }
  return Result;
}

link_internal entity *
GetPtr(entity_block_array *Arr, umm Index)
{
  umm BlockIndex = Index / 8;
  umm ElementIndex = Index % 8;

  umm AtBlock = 0;
  entity_block *Block = &Arr->First;
  while (AtBlock++ < BlockIndex)
  {
    Block = Block->Next;
  }

  entity *Result = Block->Elements+ElementIndex;
  return Result;
}

link_internal u32
AtElements(entity_block *Block)
{
  return Block->At;
}


link_internal entity_block*
Allocate_entity_block(memory_arena *Memory)
{
  entity_block *Result = Allocate(entity_block, Memory, 1);
  Result->Elements = Allocate(entity, Memory, 8);
  return Result;
}

link_internal cs
CS(entity_block_array_index Index)
{
  return FSz("(%u)(%u)", Index.BlockIndex, Index.ElementIndex);
}

link_internal void
RemoveUnordered(entity_block_array *Array, entity_block_array_index Index)
{
  entity_block_array_index Last = LastIndex(Array);

  entity *Element = GetPtr(Array, Index);
  entity *LastElement = GetPtr(Array, Last);

  *Element = *LastElement;

  Assert(Array->Current->At);
  Array->Current->At -= 1;

  if (Array->Current->At == 0)
  {
    // Walk the chain till we get to the second-last one
    entity_block *Current = &Array->First;
    entity_block *LastB = GetBlock(&Last);

    if (Current != &Array->First)
    {
      while (Current->Next != LastB)
      {
        Current = Current->Next;
      }
    }

    Assert(Current->Next == LastB || Current->Next == 0);
    Array->Current = Current;
  }
}

link_internal entity *
Push(entity_block_array *Array, entity *Element)
{
  if (Array->Memory == 0) { Array->Memory = AllocateArena(); }

  if (Array->Current == 0) { Array->First = *Allocate_entity_block(Array->Memory); Array->Current = &Array->First; }

  if (Array->Current->At == 8)
  {
    if (Array->Current->Next)
    {
      Array->Current = Array->Current->Next;
      Assert(Array->Current->At == 0);
    }
    else
    {
      entity_block *Next = Allocate_entity_block(Array->Memory);
      Next->Index = Array->Current->Index + 1;

      Array->Current->Next = Next;
      Array->Current = Next;
    }
  }

  entity *Result = Array->Current->Elements + Array->Current->At;

  Array->Current->Elements[Array->Current->At++] = *Element;

  return Result;
}


