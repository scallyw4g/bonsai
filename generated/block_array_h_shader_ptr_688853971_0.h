// external/bonsai_stdlib/src/shader.h:130:0





struct shader_ptr_block
{
  u32 Index;
  u32 At;
  shader_ptr *Elements;
  shader_ptr_block *Next;
};

struct shader_ptr_block_array_index
{
  shader_ptr_block *Block;
  u32 BlockIndex;
  u32 ElementIndex;
};

struct shader_ptr_block_array
{
  shader_ptr_block *First;
  shader_ptr_block *Current;
  memory_arena *Memory; poof(@no_serialize)
  
};

link_internal b32
AreEqual(shader_ptr_block_array_index *Thing1, shader_ptr_block_array_index *Thing2)
{
  if (Thing1 && Thing2)
  {
        b32 Result = MemoryIsEqual((u8*)Thing1, (u8*)Thing2, sizeof( shader_ptr_block_array_index ) );

    return Result;
  }
  else
  {
    return (Thing1 == Thing2);
  }
}

link_internal b32
AreEqual(shader_ptr_block_array_index Thing1, shader_ptr_block_array_index Thing2)
{
    b32 Result = MemoryIsEqual((u8*)&Thing1, (u8*)&Thing2, sizeof( shader_ptr_block_array_index ) );

  return Result;
}


typedef shader_ptr_block_array shader_ptr_paged_list;

link_internal shader_ptr_block_array_index
operator++( shader_ptr_block_array_index &I0 )
{
  if (I0.Block)
  {
    if (I0.ElementIndex == 64-1)
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
operator<( shader_ptr_block_array_index I0, shader_ptr_block_array_index I1 )
{
  b32 Result = I0.BlockIndex < I1.BlockIndex || (I0.BlockIndex == I1.BlockIndex & I0.ElementIndex < I1.ElementIndex);
  return Result;
}

link_inline umm
GetIndex( shader_ptr_block_array_index *Index)
{
  umm Result = Index->ElementIndex + (Index->BlockIndex*64);
  return Result;
}

link_internal shader_ptr_block_array_index
ZerothIndex( shader_ptr_block_array *Arr)
{
  shader_ptr_block_array_index Result = {};
  Result.Block = Arr->First;
  return Result;
}

link_internal umm
TotalElements( shader_ptr_block_array *Arr)
{
  umm Result = 0;
  if (Arr->Current)
  {
    Result = (Arr->Current->Index * 64) + Arr->Current->At;
  }
  return Result;
}

link_internal shader_ptr_block_array_index
LastIndex( shader_ptr_block_array *Arr)
{
  shader_ptr_block_array_index Result = {};
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

link_internal shader_ptr_block_array_index
AtElements( shader_ptr_block_array *Arr)
{
  shader_ptr_block_array_index Result = {};
  if (Arr->Current)
  {
    Result.Block = Arr->Current;
    Result.BlockIndex = Arr->Current->Index;
    Result.ElementIndex = Arr->Current->At;
  }
  return Result;
}

link_internal umm
Count( shader_ptr_block_array *Arr)
{
  auto Index = AtElements(Arr);
  umm Result = GetIndex(&Index);
  return Result;
}

link_internal shader_ptr 
Set( shader_ptr_block_array *Arr,
  shader_ptr Element,
  shader_ptr_block_array_index Index )
{
  shader_ptr Result = {};
  if (Index.Block)
  {
    Result = Index.Block->Elements[Index.ElementIndex];
    Result = Element;
  }

  return Result;
}

link_internal shader_ptr 
GetPtr(shader_ptr_block_array *Arr, shader_ptr_block_array_index Index)
{
  shader_ptr Result = {};
  if (Index.Block) { Result = *(Index.Block->Elements + Index.ElementIndex); }
  return Result;
}

link_internal shader_ptr 
GetPtr(shader_ptr_block *Block, umm Index)
{
  shader_ptr Result = {};
  if (Index < Block->At) { Result = *(Block->Elements + Index); }
  return Result;
}

link_internal shader_ptr 
GetPtr(shader_ptr_block_array *Arr, umm Index)
{
  umm BlockIndex = Index / 64;
  umm ElementIndex = Index % 64;

  umm AtBlock = 0;
  shader_ptr_block *Block = Arr->First;
  while (AtBlock++ < BlockIndex)
  {
    Block = Block->Next;
  }

  shader_ptr Result = *(Block->Elements+ElementIndex);
  return Result;
}

link_internal shader_ptr 
TryGetPtr(shader_ptr_block_array *Arr, umm Index)
{
  umm BlockIndex = Index / 64;
  umm ElementIndex = Index % 64;

  auto AtE = AtElements(Arr);
  umm Total = GetIndex(&AtE);
  shader_ptr Result = {};
  if (Index < Total) { Result = GetPtr(Arr, Index); }
  return Result;
}

link_internal u32
AtElements(shader_ptr_block *Block)
{
  return Block->At;
}

