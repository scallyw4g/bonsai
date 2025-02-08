// src/engine/editor.cpp:80:0





link_internal asset_thumbnail_block_array
AssetThumbnailBlockArray(memory_arena *Memory)
{
  asset_thumbnail_block_array Result = {};
  Result.Memory = Memory;
  return Result;
}

link_internal asset_thumbnail_block *
Allocate_asset_thumbnail_block(memory_arena *Memory)
{
  asset_thumbnail_block *Result = Allocate( asset_thumbnail_block, Memory, 1);
  Result->Elements = Allocate( asset_thumbnail, Memory, 8);
  return Result;
}

link_internal cs
CS( asset_thumbnail_block_array_index Index )
{
  return FSz("(%u)(%u)", Index.BlockIndex, Index.ElementIndex);
}

link_internal void
RemoveUnordered( asset_thumbnail_block_array *Array, asset_thumbnail_block_array_index Index)
{
  asset_thumbnail_block_array_index LastI = LastIndex(Array);

  asset_thumbnail *Element = GetPtr(Array, Index);
  asset_thumbnail *LastElement = GetPtr(Array, LastI);

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
      asset_thumbnail_block *Current = Array->First;
      asset_thumbnail_block *LastB = LastI.Block;

      while (Current->Next && Current->Next != LastB)
      {
        Current = Current->Next;
      }

      Assert(Current->Next == LastB || Current->Next == 0);
      Array->Current = Current;
    }
  }
}

link_internal asset_thumbnail_block_array_index
Find( asset_thumbnail_block_array *Array, asset_thumbnail *Query)
{
  asset_thumbnail_block_array_index Result = INVALID_BLOCK_ARRAY_INDEX;
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
IsValid(asset_thumbnail_block_array_index *Index)
{
  asset_thumbnail_block_array_index Test = INVALID_BLOCK_ARRAY_INDEX;
  b32 Result = AreEqual(Index, &Test);
  /* b32 Result = False; */
  return Result;
}

link_internal asset_thumbnail *
Push( asset_thumbnail_block_array *Array, asset_thumbnail *Element)
{
  if (Array->Memory == 0) { Array->Memory = AllocateArena(); }

  if (Array->First == 0) { Array->First = Allocate_asset_thumbnail_block(Array->Memory); Array->Current = Array->First; }

  if (Array->Current->At == 8)
  {
    if (Array->Current->Next)
    {
      Array->Current = Array->Current->Next;
      Assert(Array->Current->At == 0);
    }
    else
    {
      asset_thumbnail_block *Next = Allocate_asset_thumbnail_block(Array->Memory);
      Next->Index = Array->Current->Index + 1;

      Array->Current->Next = Next;
      Array->Current = Next;
    }
  }

  asset_thumbnail *Result = Array->Current->Elements + Array->Current->At;

  Array->Current->Elements[Array->Current->At++] = *Element;

  return Result;
}

