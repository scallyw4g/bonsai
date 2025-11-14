// callsite
// src/engine/editor.h:1306:0

// def (block_array_h)
// external/bonsai_stdlib/src/poof_functions.h:2266:0




struct simple_brush_block
{
  /* u32 Index; */
  umm At;
  simple_brush Elements[8];
};


struct simple_brush_block_array_index
{
  umm Index; 
};

struct simple_brush_block_array
{
  simple_brush_block **BlockPtrs; poof(@array_length(Element->BlockCount))
  u32   BlockCount;
  u32   ElementCount;
  memory_arena *Memory; poof(@no_serialize)
  
};

link_internal simple_brush_block_array
SimpleBrushBlockArray(memory_arena *Memory)
{
  simple_brush_block_array Result = {};
  Result.Memory = Memory;
  return Result;
}

link_internal b32
AreEqual(simple_brush_block_array_index *Thing1, simple_brush_block_array_index *Thing2)
{
  if (Thing1 && Thing2)
  {
        b32 Result = MemoryIsEqual((u8*)Thing1, (u8*)Thing2, sizeof( simple_brush_block_array_index ) );

    return Result;
  }
  else
  {
    return (Thing1 == Thing2);
  }
}

link_internal b32
AreEqual(simple_brush_block_array_index Thing1, simple_brush_block_array_index Thing2)
{
    b32 Result = MemoryIsEqual((u8*)&Thing1, (u8*)&Thing2, sizeof( simple_brush_block_array_index ) );

  return Result;
}


typedef simple_brush_block_array simple_brush_paged_list;

link_internal simple_brush_block_array_index
operator++( simple_brush_block_array_index &I0 )
{
  I0.Index++;
  return I0;
}

link_internal b32
operator<( simple_brush_block_array_index I0, simple_brush_block_array_index I1 )
{
  b32 Result = I0.Index < I1.Index;
  return Result;
}

link_internal b32
operator==( simple_brush_block_array_index I0, simple_brush_block_array_index I1 )
{
  b32 Result = I0.Index == I1.Index;
  return Result;
}

link_inline umm
GetIndex( simple_brush_block_array_index *Index)
{
  umm Result = Index->Index;
  return Result;
}


link_internal simple_brush_block_array_index
ZerothIndex( simple_brush_block_array *Arr )
{
  return {};
}

link_internal simple_brush_block_array_index
Capacity( simple_brush_block_array *Arr )
{
  simple_brush_block_array_index Result = {Arr->BlockCount * 8};
  return Result;
}

link_internal simple_brush_block_array_index
AtElements( simple_brush_block_array *Arr )
{
  simple_brush_block_array_index Result = {Arr->ElementCount};
  return Result;
}


link_internal umm
TotalElements( simple_brush_block_array *Arr )
{
  umm Result = AtElements(Arr).Index;
  return Result;
}


link_internal simple_brush_block_array_index
LastIndex( simple_brush_block_array *Arr )
{
  simple_brush_block_array_index Result = {};
  umm Count = AtElements(Arr).Index;
  if (Count) Result.Index = Count-1;
  return Result;
}

link_internal umm
Count( simple_brush_block_array *Arr )
{
  auto Result = AtElements(Arr).Index;
  return Result;
}

link_internal simple_brush_block *
GetBlock( simple_brush_block_array *Arr, simple_brush_block_array_index Index )
{
  umm BlockIndex   = Index.Index / 8;
  Assert(BlockIndex < Arr->BlockCount);
  simple_brush_block *Block = Arr->BlockPtrs[BlockIndex];
  return Block;
}

link_internal simple_brush *
GetPtr( simple_brush_block_array *Arr, simple_brush_block_array_index Index )
{
  Assert(Arr->BlockPtrs);
  Assert(Index.Index < Capacity(Arr).Index);

  simple_brush_block *Block = GetBlock(Arr, Index);

  umm ElementIndex = Index.Index % 8;
  simple_brush *Result = (Block->Elements + ElementIndex);
  return Result;
}


link_internal simple_brush *
GetPtr( simple_brush_block_array *Arr, umm Index )
{
  simple_brush_block_array_index I = {Index};
  return GetPtr(Arr, I);
}


link_internal simple_brush *
TryGetPtr( simple_brush_block_array *Arr, simple_brush_block_array_index Index)
{
  simple_brush * Result = {};
  if (Arr->BlockPtrs && Index < AtElements(Arr))
  {
    Result = GetPtr(Arr, Index);
  }
  return Result;
}

link_internal simple_brush *
TryGetPtr( simple_brush_block_array *Arr, umm Index)
{
  auto Result = TryGetPtr(Arr, simple_brush_block_array_index{Index});
  return Result;
}

