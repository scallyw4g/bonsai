
link_internal texture_ptr_block*
Allocate_texture_ptr_block(memory_arena *Memory)
{
  texture_ptr_block *Result = Allocate(texture_ptr_block, Memory, 1);
  Result->Elements = Allocate(texture_ptr, Memory, 8);
  return Result;
}

link_internal cs
CS(texture_ptr_block_array_index Index)
{
  return FSz("(%u)(%u)", Index.BlockIndex, Index.ElementIndex);
}

link_internal void
RemoveUnordered(texture_ptr_block_array *Array, texture_ptr_block_array_index Index)
{
  texture_ptr_block_array_index LastI = LastIndex(Array);

  texture_ptr *Element = GetPtr(Array, Index);
  texture_ptr *LastElement = GetPtr(Array, LastI);

  *Element = *LastElement;

  Assert(Array->Current->At);
  Array->Current->At -= 1;

  if (Array->Current->At == 0)
  {
    // Walk the chain till we get to the second-last one
    texture_ptr_block *Current = Array->First;
    texture_ptr_block *LastB = LastI.Block;

    while (Current->Next && Current->Next != LastB)
    {
      Current = Current->Next;
    }

    Assert(Current->Next == LastB || Current->Next == 0);
    Array->Current = Current;
  }
}

link_internal texture_ptr *
Push(texture_ptr_block_array *Array, texture_ptr *Element)
{
  if (Array->Memory == 0) { Array->Memory = AllocateArena(); }

  if (Array->First == 0) { Array->First = Allocate_texture_ptr_block(Array->Memory); Array->Current = Array->First; }

  if (Array->Current->At == 8)
  {
    if (Array->Current->Next)
    {
      Array->Current = Array->Current->Next;
      Assert(Array->Current->At == 0);
    }
    else
    {
      texture_ptr_block *Next = Allocate_texture_ptr_block(Array->Memory);
      Next->Index = Array->Current->Index + 1;

      Array->Current->Next = Next;
      Array->Current = Next;
    }
  }

  texture_ptr *Result = Array->Current->Elements + Array->Current->At;

  Array->Current->Elements[Array->Current->At++] = *Element;

  return Result;
}

