// callsite
// external/bonsai_stdlib/src/file.cpp:5:0

// def (buffer)
// external/bonsai_stdlib/src/poof_functions.h:1463:0
struct file_traversal_node_buffer
{
  umm Count;
  file_traversal_node *Start; poof(@array_length(Element->Count))
};

link_internal file_traversal_node_buffer
FileTraversalNodeBuffer( umm ElementCount, memory_arena* Memory);

link_internal file_traversal_node_buffer
FileTraversalNodeBuffer( file_traversal_node *Start, umm ElementCount)
{
  file_traversal_node_buffer Result = {ElementCount, Start};
  return Result;
}

link_inline umm
LastIndex(file_traversal_node_buffer *Buf)
{
  umm Result = Buf->Count > 0? Buf->Count-1 : 0;
  return Result;
}

link_inline umm
ZerothIndex(file_traversal_node_buffer *Buf)
{
  umm Result = 0;
  return Result;
}

link_inline umm
AtElements(file_traversal_node_buffer *Buf)
{
  umm Result = Buf->Count;
  return Result;
}

link_inline umm
TotalElements(file_traversal_node_buffer *Buf)
{
  umm Result = Buf->Count;
  return Result;
}

link_inline umm
CurrentCount(file_traversal_node_buffer *Buf)
{
  umm Result = Buf->Count;
  return Result;
}

link_inline file_traversal_node *
GetPtr(file_traversal_node_buffer *Buf, umm Index)
{
  file_traversal_node *Result = 0;
  if (Index < Buf->Count) { Result = Buf->Start + Index; }
  return Result;
}

link_inline file_traversal_node *
TryGetPtr(file_traversal_node_buffer *Buf, umm Index)
{
  return GetPtr(Buf, Index);
}

link_inline file_traversal_node *
Get(file_traversal_node_buffer *Buf, umm Index)
{
  file_traversal_node *Result = GetPtr(Buf, Index);
  return Result;
}

link_internal file_traversal_node_buffer
FileTraversalNodeBuffer( umm ElementCount, memory_arena* Memory)
{
  file_traversal_node_buffer Result = {};

  if (ElementCount)
  {
    Result.Start = Allocate( file_traversal_node, Memory, ElementCount );
    Result.Count = ElementCount;
  }
  else
  {
    Warn("Attempted to allocate file_traversal_node_buffer of 0 length.");
  }

  return Result;
}


