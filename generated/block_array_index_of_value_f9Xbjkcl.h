// callsite
// src/engine/editor.h:1802:0

// def (block_array_index_of_value)
// external/bonsai_stdlib/src/poof_functions.h:2481:0
link_internal world_edit_block_array_index_block_array_index
IndexOfValue( world_edit_block_array_index_block_array *Array, world_edit_block_array_index *Query)
{
  world_edit_block_array_index_block_array_index Result = {INVALID_BLOCK_ARRAY_INDEX};
  IterateOver(Array, E, Index)
  {
    if (AreEqual(E, Query))
    {
      Result = Index;
      break;
    }
  }
  return Result;
}

