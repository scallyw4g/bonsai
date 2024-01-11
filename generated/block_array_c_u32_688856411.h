
link_internal u32_block*
Allocate_u32_block(memory_arena *Memory)
{
  u32_block *Result = Allocate(u32_block, Memory, 1);
  Result->Elements = Allocate(u32, Memory, 8);
  return Result;
}

link_internal cs
CS(u32_block_array_index Index)
{
  return FSz("(%u)(%u)", Index.BlockIndex, Index.ElementIndex);
}

link_internal void
RemoveUnordered(u32_block_array *Array, u32_block_array_index Index)
{
  u32_block_array_index LastI = LastIndex(Array);

  u32 *Element = GetPtr(Array, Index);
  u32 *LastElement = GetPtr(Array, LastI);

  *Element = *LastElement;

  Assert(Array->Current->At);
  Array->Current->At -= 1;

  if (Array->Current->At == 0)
  {
    // Walk the chain till we get to the second-last one
    u32_block *Current = Array->First;
    u32_block *LastB = LastI.Block;

    while (Current->Next && Current->Next != LastB)
    {
      Current = Current->Next;
    }

    Assert(Current->Next == LastB || Current->Next == 0);
    Array->Current = Current;
  }
}

link_internal u32 *
Push(u32_block_array *Array, u32 *Element)
{
  if (Array->Memory == 0) { Array->Memory = AllocateArena(); }

  if (Array->First == 0) { Array->First = Allocate_u32_block(Array->Memory); Array->Current = Array->First; }

  if (Array->Current->At == 8)
  {
    if (Array->Current->Next)
    {
      Array->Current = Array->Current->Next;
      Assert(Array->Current->At == 0);
    }
    else
    {
      u32_block *Next = Allocate_u32_block(Array->Memory);
      Next->Index = Array->Current->Index + 1;

      Array->Current->Next = Next;
      Array->Current = Next;
    }
  }

  u32 *Result = Array->Current->Elements + Array->Current->At;

  Array->Current->Elements[Array->Current->At++] = *Element;

  return Result;
}

