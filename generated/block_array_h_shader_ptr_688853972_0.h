// callsite
// external/bonsai_stdlib/src/shader.h:218:0

// def (block_array_h)
// external/bonsai_stdlib/src/poof_functions.h:2293:0




struct shader_ptr_block
{
  /* u32 Index; */
  umm At;
  shader_ptr Elements[64];
};


struct shader_ptr_block_array_index
{
  umm Index; 
};

struct shader_ptr_block_array
poof(
  @collection
  
  
)
{
  shader_ptr_block **BlockPtrs; poof(@array_length(Element->BlockCount))
  u32   BlockCount;
  u32   ElementCount;
  memory_arena *Memory; poof(@no_serialize)
  
};

link_internal shader_ptr_block_array
ShaderPtrBlockArray(memory_arena *Memory)
{
  shader_ptr_block_array Result = {};
  Result.Memory = Memory;
  return Result;
}

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
  I0.Index++;
  return I0;
}

link_internal b32
operator<( shader_ptr_block_array_index I0, shader_ptr_block_array_index I1 )
{
  b32 Result = I0.Index < I1.Index;
  return Result;
}

link_internal b32
operator==( shader_ptr_block_array_index I0, shader_ptr_block_array_index I1 )
{
  b32 Result = I0.Index == I1.Index;
  return Result;
}

link_inline umm
GetIndex( shader_ptr_block_array_index *Index)
{
  umm Result = Index->Index;
  return Result;
}


link_internal shader_ptr_block_array_index
ZerothIndex( shader_ptr_block_array *Arr )
{
  return {};
}

link_internal shader_ptr_block_array_index
Capacity( shader_ptr_block_array *Arr )
{
  shader_ptr_block_array_index Result = {Arr->BlockCount * 64};
  return Result;
}

link_internal shader_ptr_block_array_index
AtElements( shader_ptr_block_array *Arr )
{
  shader_ptr_block_array_index Result = {Arr->ElementCount};
  return Result;
}


link_internal umm
TotalElements( shader_ptr_block_array *Arr )
{
  umm Result = AtElements(Arr).Index;
  return Result;
}


link_internal shader_ptr_block_array_index
LastIndex( shader_ptr_block_array *Arr )
{
  shader_ptr_block_array_index Result = {};
  umm Count = AtElements(Arr).Index;
  if (Count) Result.Index = Count-1;
  return Result;
}

link_internal umm
Count( shader_ptr_block_array *Arr )
{
  auto Result = AtElements(Arr).Index;
  return Result;
}

link_internal shader_ptr_block *
GetBlock( shader_ptr_block_array *Arr, shader_ptr_block_array_index Index )
{
  umm BlockIndex   = Index.Index / 64;
  Assert(BlockIndex < Arr->BlockCount);
  shader_ptr_block *Block = Arr->BlockPtrs[BlockIndex];
  return Block;
}

link_internal shader_ptr 
GetPtr( shader_ptr_block_array *Arr, shader_ptr_block_array_index Index )
{
  Assert(Arr->BlockPtrs);
  Assert(Index.Index < Capacity(Arr).Index);

  shader_ptr_block *Block = GetBlock(Arr, Index);

  umm ElementIndex = Index.Index % 64;
  shader_ptr Result = *(Block->Elements + ElementIndex);
  return Result;
}


link_internal shader_ptr 
GetPtr( shader_ptr_block_array *Arr, umm Index )
{
  shader_ptr_block_array_index I = {Index};
  return GetPtr(Arr, I);
}


link_internal shader_ptr 
TryGetPtr( shader_ptr_block_array *Arr, shader_ptr_block_array_index Index)
{
  shader_ptr  Result = {};
  if (Arr->BlockPtrs && Index < AtElements(Arr))
  {
    Result = GetPtr(Arr, Index);
  }
  return Result;
}

link_internal shader_ptr 
TryGetPtr( shader_ptr_block_array *Arr, umm Index)
{
  auto Result = TryGetPtr(Arr, shader_ptr_block_array_index{Index});
  return Result;
}

/* link_internal void */
/* Swap( (element_t.name)_block_array *Arr, umm I0, umm I1) */
/* { */
/*   Assert(I0 < AtElements(Arr).Index); */
/*   Assert(I1 < AtElements(Arr).Index); */

/*   auto P0 = GetPtr(Arr, I0); */
/*   auto P1 = GetPtr(Arr, I1); */

/*   auto Tmp = *P0; */
/*   *P0 = *P1; */

/*   *P1 = Tmp; */
/* } */

