// external/bonsai_stdlib/src/shader.cpp:2:0


link_internal shader_block*
Allocate_shader_block(memory_arena *Memory)
{
  shader_block *Result = Allocate(shader_block, Memory, 1);
  Result->Elements = Allocate(shader, Memory, 64);
  return Result;
}

link_internal cs
CS(shader_block_array_index Index)
{
  return FSz("(%u)(%u)", Index.BlockIndex, Index.ElementIndex);
}

link_internal void
RemoveUnordered(shader_block_array *Array, shader_block_array_index Index)
{
  shader_block_array_index LastI = LastIndex(Array);

  shader *Element = GetPtr(Array, Index);
  shader *LastElement = GetPtr(Array, LastI);

  *Element = *LastElement;

  Assert(Array->Current->At);
  Array->Current->At -= 1;

  if (Array->Current->At == 0)
  {
    // Walk the chain till we get to the second-last one
    shader_block *Current = Array->First;
    shader_block *LastB = LastI.Block;

    while (Current->Next && Current->Next != LastB)
    {
      Current = Current->Next;
    }

    Assert(Current->Next == LastB || Current->Next == 0);
    Array->Current = Current;
  }
}

link_internal shader *
Push(shader_block_array *Array, shader *Element)
{
  if (Array->Memory == 0) { Array->Memory = AllocateArena(); }

  if (Array->First == 0) { Array->First = Allocate_shader_block(Array->Memory); Array->Current = Array->First; }

  if (Array->Current->At == 64)
  {
    if (Array->Current->Next)
    {
      Array->Current = Array->Current->Next;
      Assert(Array->Current->At == 0);
    }
    else
    {
      shader_block *Next = Allocate_shader_block(Array->Memory);
      Next->Index = Array->Current->Index + 1;

      Array->Current->Next = Next;
      Array->Current = Next;
    }
  }

  shader *Result = Array->Current->Elements + Array->Current->At;

  Array->Current->Elements[Array->Current->At++] = *Element;

  return Result;
}

