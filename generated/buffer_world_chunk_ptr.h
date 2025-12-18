// callsite
// src/engine/world_chunk.h:462:0

// def (buffer)
// external/bonsai_stdlib/src/poof_functions.h:1463:0
struct world_chunk_ptr_buffer
{
  umm Count;
  world_chunk_ptr *Start; poof(@array_length(Element->Count))
};

link_internal world_chunk_ptr_buffer
WorldChunkPtrBuffer( umm ElementCount, memory_arena* Memory);

link_internal world_chunk_ptr_buffer
WorldChunkPtrBuffer( world_chunk_ptr *Start, umm ElementCount)
{
  world_chunk_ptr_buffer Result = {ElementCount, Start};
  return Result;
}

link_inline umm
LastIndex(world_chunk_ptr_buffer *Buf)
{
  umm Result = Buf->Count > 0? Buf->Count-1 : 0;
  return Result;
}

link_inline umm
ZerothIndex(world_chunk_ptr_buffer *Buf)
{
  umm Result = 0;
  return Result;
}

link_inline umm
AtElements(world_chunk_ptr_buffer *Buf)
{
  umm Result = Buf->Count;
  return Result;
}

link_inline umm
TotalElements(world_chunk_ptr_buffer *Buf)
{
  umm Result = Buf->Count;
  return Result;
}

link_inline umm
CurrentCount(world_chunk_ptr_buffer *Buf)
{
  umm Result = Buf->Count;
  return Result;
}

link_inline world_chunk_ptr *
GetPtr(world_chunk_ptr_buffer *Buf, umm Index)
{
  world_chunk_ptr *Result = 0;
  if (Index < Buf->Count) { Result = Buf->Start + Index; }
  return Result;
}

link_inline world_chunk_ptr *
TryGetPtr(world_chunk_ptr_buffer *Buf, umm Index)
{
  return GetPtr(Buf, Index);
}

link_inline world_chunk_ptr *
Get(world_chunk_ptr_buffer *Buf, umm Index)
{
  world_chunk_ptr *Result = GetPtr(Buf, Index);
  return Result;
}

link_internal world_chunk_ptr_buffer
WorldChunkPtrBuffer( umm ElementCount, memory_arena* Memory)
{
  world_chunk_ptr_buffer Result = {};

  if (ElementCount)
  {
    Result.Start = Allocate( world_chunk_ptr, Memory, ElementCount );
    Result.Count = ElementCount;
  }
  else
  {
    Warn("Attempted to allocate world_chunk_ptr_buffer of 0 length.");
  }

  return Result;
}


