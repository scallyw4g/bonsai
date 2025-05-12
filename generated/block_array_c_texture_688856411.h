// external/bonsai_stdlib/src/texture.cpp:5:0





link_internal texture_block *
Allocate_texture_block(memory_arena *Memory)
{
  texture_block *Result = Allocate( texture_block, Memory, 1);
  Result->Elements = Allocate( texture, Memory, 8);
  return Result;
}

link_internal cs
CS( texture_block_array_index Index )
{
  return FSz("(%u)(%u)", Index.BlockIndex, Index.ElementIndex);
}

link_internal texture *
Set( texture_block_array *Arr,
  texture *Element,
  texture_block_array_index Index )
{
  texture *Result = {};
  if (Index.Block)
  {
    texture *Slot = &Index.Block->Elements[Index.ElementIndex];
    *Slot = *Element;

    Result = Slot;
  }

  return Result;
}

link_internal void
RemoveOrdered( texture_block_array *Array, texture_block_array_index Index)
{
  NotImplemented;
}

link_internal void
RemoveOrdered( texture_block_array *Array, texture *Element )
{
  NotImplemented;
}

link_internal void
RemoveUnordered( texture_block_array *Array, texture_block_array_index Index)
{
  texture_block_array_index LastI = LastIndex(Array);

  texture *Element = GetPtr(Array, Index);
  texture *LastElement = GetPtr(Array, LastI);

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
      texture_block *Current = Array->First;
      texture_block *LastB = LastI.Block;

      while (Current->Next && Current->Next != LastB)
      {
        Current = Current->Next;
      }

      Assert(Current->Next == LastB || Current->Next == 0);
      Array->Current = Current;
    }
  }
}

link_internal texture_block_array_index
Find( texture_block_array *Array, texture *Query)
{
  texture_block_array_index Result = INVALID_BLOCK_ARRAY_INDEX;
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
IsValid(texture_block_array_index *Index)
{
  texture_block_array_index Test = INVALID_BLOCK_ARRAY_INDEX;
  b32 Result = (AreEqual(Index, &Test) == False);
  return Result;
}

link_internal texture *
Push( texture_block_array *Array, texture *Element)
{
  Assert(Array->Memory);

  if (Array->First == 0) { Array->First = Allocate_texture_block(Array->Memory); Array->Current = Array->First; }

  if (Array->Current->At == 8)
  {
    if (Array->Current->Next)
    {
      Array->Current = Array->Current->Next;
      Assert(Array->Current->At == 0);
    }
    else
    {
      texture_block *Next = Allocate_texture_block(Array->Memory);
      Next->Index = Array->Current->Index + 1;

      Array->Current->Next = Next;
      Array->Current = Next;
    }
  }

  texture *Result = Array->Current->Elements + Array->Current->At;

  Array->Current->Elements[Array->Current->At++] = *Element;

  return Result;
}

link_internal texture *
Push( texture_block_array *Array )
{
  texture Element = {};
  auto Result = Push(Array, &Element);
  return Result;
}

