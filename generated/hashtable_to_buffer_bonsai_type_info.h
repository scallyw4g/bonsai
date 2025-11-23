// callsite
// src/engine/bonsai_type_info.h:40:0

// def (hashtable_to_buffer)
// external/bonsai_stdlib/src/poof_functions.h:1114:0
bonsai_type_info_buffer
ToBuffer( bonsai_type_info_hashtable *Table, memory_arena *Memory )
{
  umm Count = 0;
  RangeIterator_t(umm, Index, Table->Size)
  {
    bonsai_type_info_linked_list_node *E = Table->Elements[Index];
    if (E) Count++;
  }

  bonsai_type_info_buffer Result = BonsaiTypeInfoBuffer(Count, Memory);

  Count = 0;
  RangeIterator_t(umm, Index, Table->Size)
  {
    bonsai_type_info_linked_list_node *E = Table->Elements[Index];
    if (E) { Result.Start[Count] = E->Element; Count++; }
  }
  Assert(Count == Result.Count);

  return Result;
}


