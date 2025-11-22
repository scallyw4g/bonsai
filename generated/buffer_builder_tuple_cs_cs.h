// callsite
// external/bonsai_stdlib/src/counted_string.cpp:57:0

// def (buffer_builder)
// external/bonsai_stdlib/src/poof_functions.h:727:0
struct tuple_cs_cs_buffer_builder
{
  tuple_cs_cs_stream Chunks;
};

link_internal void
Append( tuple_cs_cs_buffer_builder* Builder, tuple_cs_cs E)
{
  Push(&Builder->Chunks, E);
}

struct tuple_cs_cs_buffer
{
  umm Count;
  tuple_cs_cs *E;
};

link_internal tuple_cs_cs_buffer
TupleCsCsBuffer(umm TotalElements, memory_arena *PermMemory)
{
  tuple_cs_cs_buffer Result = {};
  Result.Count = TotalElements;
  Result.E = Allocate( tuple_cs_cs, PermMemory, TotalElements);
  return Result;
}

link_internal tuple_cs_cs_buffer
Finalize( tuple_cs_cs_buffer_builder *Builder, memory_arena *PermMemory)
{
  TIMED_FUNCTION();

  u32 TotalElements = 0;

  // TODO(Jesse): Keep track of # of chunks?
  ITERATE_OVER(&Builder->Chunks)
  {
    ++TotalElements;
  }

  auto Result = TupleCsCsBuffer(TotalElements, PermMemory);

  u32 ElementIndex = 0;
  ITERATE_OVER(&Builder->Chunks)
  {
    auto At = GET_ELEMENT(Iter);
    Result.E[ElementIndex] = *At;
    Assert(ElementIndex < Result.Count);
    ++ElementIndex;
  }
  Assert(ElementIndex == Result.Count);

  return Result;
}

