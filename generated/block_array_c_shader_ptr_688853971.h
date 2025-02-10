// external/bonsai_stdlib/src/shader.cpp:2:0





link_internal shader_ptr_block *
Allocate_shader_ptr_block(memory_arena *Memory)
{
  shader_ptr_block *Result = Allocate( shader_ptr_block, Memory, 1);
  Result->Elements = Allocate( shader_ptr, Memory, 64);
  return Result;
}

link_internal cs
CS( shader_ptr_block_array_index Index )
{
  return FSz("(%u)(%u)", Index.BlockIndex, Index.ElementIndex);
}

link_internal void
RemoveUnordered( shader_ptr_block_array *Array, shader_ptr_block_array_index Index)
{
  shader_ptr_block_array_index LastI = LastIndex(Array);

  shader_ptr Element = GetPtr(Array, Index);
  shader_ptr LastElement = GetPtr(Array, LastI);

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
      shader_ptr_block *Current = Array->First;
      shader_ptr_block *LastB = LastI.Block;

      while (Current->Next && Current->Next != LastB)
      {
        Current = Current->Next;
      }

      Assert(Current->Next == LastB || Current->Next == 0);
      Array->Current = Current;
    }
  }
}

link_internal shader_ptr_block_array_index
Find( shader_ptr_block_array *Array, shader_ptr Query)
{
  shader_ptr_block_array_index Result = INVALID_BLOCK_ARRAY_INDEX;
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
IsValid(shader_ptr_block_array_index *Index)
{
  shader_ptr_block_array_index Test = INVALID_BLOCK_ARRAY_INDEX;
  b32 Result = (AreEqual(Index, &Test) == False);
  return Result;
}

link_internal shader_ptr *
Push( shader_ptr_block_array *Array, shader_ptr *Element)
{
  Assert(Array->Memory);

  if (Array->First == 0) { Array->First = Allocate_shader_ptr_block(Array->Memory); Array->Current = Array->First; }

  if (Array->Current->At == 64)
  {
    if (Array->Current->Next)
    {
      Array->Current = Array->Current->Next;
      Assert(Array->Current->At == 0);
    }
    else
    {
      shader_ptr_block *Next = Allocate_shader_ptr_block(Array->Memory);
      Next->Index = Array->Current->Index + 1;

      Array->Current->Next = Next;
      Array->Current = Next;
    }
  }

  shader_ptr *Result = Array->Current->Elements + Array->Current->At;

  Array->Current->Elements[Array->Current->At++] = *Element;

  return Result;
}

