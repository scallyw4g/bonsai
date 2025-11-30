// callsite
// external/bonsai_stdlib/src/file.cpp:11:0

// def (flatten_block_array)
// external/bonsai_stdlib/src/poof_functions.h:2702:0
link_internal file_traversal_node_buffer
Flatten(file_traversal_node_block_array *Array, memory_arena *Memory)
{
  // TODO(Jesse): This is MAJOR barf
  auto At = AtElements(Array);
  auto Count = GetIndex(&At);
  file_traversal_node_buffer Result = FileTraversalNodeBuffer(Count, Memory);
  IterateOver(Array, Element, ElementIndex)
  {
    // NOTE(Jesse): UGGGGGGHHH
    umm Index = GetIndex(&ElementIndex);
    Result.Start[Index] = *Element;
  }
  return Result;
}

