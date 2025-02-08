// external/bonsai_stdlib/src/ansi_stream.h:6:0





struct u8_cursor_block
{
  u32 Index;
  u32 At;
  u8_cursor *Elements;
  u8_cursor_block *Next;
};

struct u8_cursor_block_array_index
{
  u8_cursor_block *Block;
  u32 BlockIndex;
  u32 ElementIndex;
};

struct u8_cursor_block_array
{
  u8_cursor_block *First;
  u8_cursor_block *Current;
  memory_arena *Memory; poof(@no_serialize)
  u64 BlockSize;
};

link_internal b32
AreEqual(u8_cursor_block_array_index *Thing1, u8_cursor_block_array_index *Thing2)
{
  if (Thing1 && Thing2)
  {
        b32 Result = MemoryIsEqual((u8*)Thing1, (u8*)Thing2, sizeof( u8_cursor_block_array_index ) );

    return Result;
  }
  else
  {
    return (Thing1 == Thing2);
  }
}

link_internal b32
AreEqual(u8_cursor_block_array_index Thing1, u8_cursor_block_array_index Thing2)
{
    b32 Result = MemoryIsEqual((u8*)&Thing1, (u8*)&Thing2, sizeof( u8_cursor_block_array_index ) );

  return Result;
}


typedef u8_cursor_block_array u8_cursor_paged_list;

link_internal u8_cursor_block_array_index
operator++( u8_cursor_block_array_index &I0 )
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
operator<( u8_cursor_block_array_index I0, u8_cursor_block_array_index I1 )
{
  b32 Result = I0.BlockIndex < I1.BlockIndex || (I0.BlockIndex == I1.BlockIndex & I0.ElementIndex < I1.ElementIndex);
  return Result;
}

link_inline umm
GetIndex( u8_cursor_block_array_index *Index)
{
  umm Result = Index->ElementIndex + (Index->BlockIndex*8);
  return Result;
}

link_internal u8_cursor_block_array_index
ZerothIndex( u8_cursor_block_array *Arr)
{
  u8_cursor_block_array_index Result = {};
  Result.Block = Arr->First;
  return Result;
}

link_internal umm
TotalElements( u8_cursor_block_array *Arr)
{
  umm Result = 0;
  if (Arr->Current)
  {
    Result = (Arr->Current->Index * 8) + Arr->Current->At;
  }
  return Result;
}

link_internal u8_cursor_block_array_index
LastIndex( u8_cursor_block_array *Arr)
{
  u8_cursor_block_array_index Result = {};
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

link_internal u8_cursor_block_array_index
AtElements( u8_cursor_block_array *Arr)
{
  u8_cursor_block_array_index Result = {};
  if (Arr->Current)
  {
    Result.Block = Arr->Current;
    Result.BlockIndex = Arr->Current->Index;
    Result.ElementIndex = Arr->Current->At;
  }
  return Result;
}

link_internal umm
Count( u8_cursor_block_array *Arr)
{
  auto Index = AtElements(Arr);
  umm Result = GetIndex(&Index);
  return Result;
}

link_internal u8_cursor *
Set( u8_cursor_block_array *Arr,
  u8_cursor *Element,
  u8_cursor_block_array_index Index )
{
  u8_cursor *Result = {};
  if (Index.Block)
  {
    Result = &Index.Block->Elements[Index.ElementIndex];
    *Result = *Element;
  }

  return Result;
}

link_internal u8_cursor *
GetPtr(u8_cursor_block_array *Arr, u8_cursor_block_array_index Index)
{
  u8_cursor *Result = {};
  if (Index.Block) { Result = (Index.Block->Elements + Index.ElementIndex); }
  return Result;
}

link_internal u8_cursor *
GetPtr(u8_cursor_block *Block, umm Index)
{
  u8_cursor *Result = {};
  if (Index < Block->At) { Result = (Block->Elements + Index); }
  return Result;
}

link_internal u8_cursor *
GetPtr(u8_cursor_block_array *Arr, umm Index)
{
  umm BlockIndex = Index / 8;
  umm ElementIndex = Index % 8;

  umm AtBlock = 0;
  u8_cursor_block *Block = Arr->First;
  while (AtBlock++ < BlockIndex)
  {
    Block = Block->Next;
  }

  u8_cursor *Result = (Block->Elements+ElementIndex);
  return Result;
}

link_internal u8_cursor *
TryGetPtr(u8_cursor_block_array *Arr, umm Index)
{
  umm BlockIndex = Index / 8;
  umm ElementIndex = Index % 8;

  auto AtE = AtElements(Arr);
  umm Total = GetIndex(&AtE);
  u8_cursor *Result = {};
  if (Index < Total) { Result = GetPtr(Arr, Index); }
  return Result;
}

link_internal u32
AtElements(u8_cursor_block *Block)
{
  return Block->At;
}

