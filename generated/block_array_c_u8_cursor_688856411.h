// external/bonsai_stdlib/src/ansi_stream.cpp:3:0





link_internal u8_cursor_block_array
U8CursorBlockArray(memory_arena *Memory)
{
  u8_cursor_block_array Result = {};
  Result.Memory = Memory;
  return Result;
}

link_internal u8_cursor_block *
Allocate_u8_cursor_block(memory_arena *Memory)
{
  u8_cursor_block *Result = Allocate( u8_cursor_block, Memory, 1);
  Result->Elements = Allocate( u8_cursor, Memory, 8);
  return Result;
}

link_internal cs
CS( u8_cursor_block_array_index Index )
{
  return FSz("(%u)(%u)", Index.BlockIndex, Index.ElementIndex);
}

link_internal void
RemoveUnordered( u8_cursor_block_array *Array, u8_cursor_block_array_index Index)
{
  u8_cursor_block_array_index LastI = LastIndex(Array);

  u8_cursor *Element = GetPtr(Array, Index);
  u8_cursor *LastElement = GetPtr(Array, LastI);

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
      u8_cursor_block *Current = Array->First;
      u8_cursor_block *LastB = LastI.Block;

      while (Current->Next && Current->Next != LastB)
      {
        Current = Current->Next;
      }

      Assert(Current->Next == LastB || Current->Next == 0);
      Array->Current = Current;
    }
  }
}

link_internal u8_cursor_block_array_index
Find( u8_cursor_block_array *Array, u8_cursor *Query)
{
  u8_cursor_block_array_index Result = INVALID_BLOCK_ARRAY_INDEX;
  IterateOver(Array, E, Index)
  {
    if (E == Query)
    {
      Result = Index;
      break;
    }
  }
  return Result;
}

link_internal b32
IsValid(u8_cursor_block_array_index *Index)
{
  NotImplemented;
  u8_cursor_block_array_index Test = INVALID_BLOCK_ARRAY_INDEX;
  /* b32 Result = AreEqual(*Index, Test); */
  b32 Result = False;
  return Result;
}

link_internal u8_cursor *
Push( u8_cursor_block_array *Array, u8_cursor *Element)
{
  if (Array->Memory == 0) { Array->Memory = AllocateArena(); }

  if (Array->First == 0) { Array->First = Allocate_u8_cursor_block(Array->Memory); Array->Current = Array->First; }

  if (Array->Current->At == 8)
  {
    if (Array->Current->Next)
    {
      Array->Current = Array->Current->Next;
      Assert(Array->Current->At == 0);
    }
    else
    {
      u8_cursor_block *Next = Allocate_u8_cursor_block(Array->Memory);
      Next->Index = Array->Current->Index + 1;

      Array->Current->Next = Next;
      Array->Current = Next;
    }
  }

  u8_cursor *Result = Array->Current->Elements + Array->Current->At;

  Array->Current->Elements[Array->Current->At++] = *Element;

  return Result;
}

