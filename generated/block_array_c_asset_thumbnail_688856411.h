
link_internal asset_thumbnail_block*
Allocate_asset_thumbnail_block(memory_arena *Memory)
{
  asset_thumbnail_block *Result = Allocate(asset_thumbnail_block, Memory, 1);
  Result->Elements = Allocate(asset_thumbnail, Memory, 8);
  return Result;
}

link_internal cs
CS(asset_thumbnail_block_array_index Index)
{
  return FSz("(%u)(%u)", Index.BlockIndex, Index.ElementIndex);
}

link_internal void
RemoveUnordered(asset_thumbnail_block_array *Array, asset_thumbnail_block_array_index Index)
{
  Leak("RemoveUnordered");
}

link_internal asset_thumbnail *
Push(asset_thumbnail_block_array *Array, asset_thumbnail *Element)
{
  if (Array->Memory == 0) { Array->Memory = AllocateArena(); }

  if (Array->Current == 0) { Array->First = *Allocate_asset_thumbnail_block(Array->Memory); Array->Current = &Array->First; }

  if (Array->Current->At == 8)
  {
    asset_thumbnail_block *Next = Allocate_asset_thumbnail_block(Array->Memory);
    Next->Index = Array->Current->Index + 1;

    Array->Current->Next = Next;
    Array->Current = Next;
    /* Array->At = 0; */
  }

  asset_thumbnail *Result = Array->Current->Elements + Array->Current->At;

  Array->Current->Elements[Array->Current->At++] = *Element;

  return Result;
}

