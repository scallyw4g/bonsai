// external/bonsai_stdlib/src/texture.cpp:8:0





link_internal texture_ptr_block_array
TexturePtrBlockArray(memory_arena *Memory)
{
  texture_ptr_block_array Result = {};
  Result.Memory = Memory;
  return Result;
}

link_internal texture_ptr_block *
Allocate_texture_ptr_block(memory_arena *Memory)
{
  texture_ptr_block *Result = Allocate( texture_ptr_block, Memory, 1);
  Result->Elements = Allocate( texture_ptr, Memory, 8);
  return Result;
}

link_internal cs
CS( texture_ptr_block_array_index Index )
{
  return FSz("(%u)(%u)", Index.BlockIndex, Index.ElementIndex);
}

link_internal void
RemoveUnordered( texture_ptr_block_array *Array, texture_ptr_block_array_index Index)
{
  texture_ptr_block_array_index LastI = LastIndex(Array);

  texture_ptr Element = GetPtr(Array, Index);
  texture_ptr LastElement = GetPtr(Array, LastI);

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
}

link_internal texture_ptr_block_array_index
Find( texture_ptr_block_array *Array, texture_ptr Query)
{
  texture_ptr_block_array_index Result = INVALID_BLOCK_ARRAY_INDEX;
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
IsValid(texture_ptr_block_array_index *Index)
{
  texture_ptr_block_array_index Test = INVALID_BLOCK_ARRAY_INDEX;
  b32 Result = AreEqual(Index, &Test);
  /* b32 Result = False; */
  return Result;
}

link_internal texture_ptr *
Push( texture_ptr_block_array *Array, texture_ptr *Element)
{
  Assert(Array->Memory);

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

