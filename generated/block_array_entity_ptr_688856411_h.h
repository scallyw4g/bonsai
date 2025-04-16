// src/engine/world_chunk.h:307:0





struct entity_ptr_block
{
  u32 Index;
  u32 At;
  entity_ptr *Elements;
  entity_ptr_block *Next;
};

struct entity_ptr_block_array_index
{
  entity_ptr_block *Block;
  u32 BlockIndex;
  u32 ElementIndex;
};

struct entity_ptr_block_array
{
  entity_ptr_block *First;
  entity_ptr_block *Current;
  memory_arena *Memory; poof(@no_serialize)
  
};

link_internal entity_ptr_block_array
EntityPtrBlockArray(memory_arena *Memory)
{
  entity_ptr_block_array Result = {};
  Result.Memory = Memory;
  return Result;
}

link_internal b32
AreEqual(entity_ptr_block_array_index *Thing1, entity_ptr_block_array_index *Thing2)
{
  if (Thing1 && Thing2)
  {
        b32 Result = MemoryIsEqual((u8*)Thing1, (u8*)Thing2, sizeof( entity_ptr_block_array_index ) );

    return Result;
  }
  else
  {
    return (Thing1 == Thing2);
  }
}

link_internal b32
AreEqual(entity_ptr_block_array_index Thing1, entity_ptr_block_array_index Thing2)
{
    b32 Result = MemoryIsEqual((u8*)&Thing1, (u8*)&Thing2, sizeof( entity_ptr_block_array_index ) );

  return Result;
}


typedef entity_ptr_block_array entity_ptr_paged_list;

link_internal entity_ptr_block_array_index
operator++( entity_ptr_block_array_index &I0 )
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
operator<( entity_ptr_block_array_index I0, entity_ptr_block_array_index I1 )
{
  b32 Result = I0.BlockIndex < I1.BlockIndex || (I0.BlockIndex == I1.BlockIndex & I0.ElementIndex < I1.ElementIndex);
  return Result;
}

link_inline umm
GetIndex( entity_ptr_block_array_index *Index)
{
  umm Result = Index->ElementIndex + (Index->BlockIndex*8);
  return Result;
}

link_internal entity_ptr_block_array_index
ZerothIndex( entity_ptr_block_array *Arr)
{
  entity_ptr_block_array_index Result = {};
  Result.Block = Arr->First;
  return Result;
}

link_internal umm
TotalElements( entity_ptr_block_array *Arr)
{
  umm Result = 0;
  if (Arr->Current)
  {
    Result = (Arr->Current->Index * 8) + Arr->Current->At;
  }
  return Result;
}

link_internal entity_ptr_block_array_index
LastIndex( entity_ptr_block_array *Arr)
{
  entity_ptr_block_array_index Result = {};
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

link_internal entity_ptr_block_array_index
AtElements( entity_ptr_block_array *Arr)
{
  entity_ptr_block_array_index Result = {};
  if (Arr->Current)
  {
    Result.Block = Arr->Current;
    Result.BlockIndex = Arr->Current->Index;
    Result.ElementIndex = Arr->Current->At;
  }
  return Result;
}

link_internal umm
Count( entity_ptr_block_array *Arr)
{
  auto Index = AtElements(Arr);
  umm Result = GetIndex(&Index);
  return Result;
}

link_internal entity_ptr 
GetPtr(entity_ptr_block_array *Arr, entity_ptr_block_array_index Index)
{
  entity_ptr Result = {};
  if (Index.Block) { Result = *(Index.Block->Elements + Index.ElementIndex); }
  return Result;
}

link_internal entity_ptr 
GetPtr(entity_ptr_block *Block, umm Index)
{
  entity_ptr Result = {};
  if (Index < Block->At) { Result = *(Block->Elements + Index); }
  return Result;
}

link_internal entity_ptr 
GetPtr(entity_ptr_block_array *Arr, umm Index)
{
  umm BlockIndex = Index / 8;
  umm ElementIndex = Index % 8;

  umm AtBlock = 0;
  entity_ptr_block *Block = Arr->First;
  while (AtBlock++ < BlockIndex)
  {
    Block = Block->Next;
  }

  entity_ptr Result = *(Block->Elements+ElementIndex);
  return Result;
}

link_internal entity_ptr 
TryGetPtr(entity_ptr_block_array *Arr, umm Index)
{
  umm BlockIndex = Index / 8;
  umm ElementIndex = Index % 8;

  auto AtE = AtElements(Arr);
  umm Total = GetIndex(&AtE);
  entity_ptr Result = {};
  if (Index < Total) { Result = GetPtr(Arr, Index); }
  return Result;
}

link_internal u32
AtElements(entity_ptr_block *Block)
{
  return Block->At;
}

