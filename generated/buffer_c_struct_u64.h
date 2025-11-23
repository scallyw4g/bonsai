// callsite
// src/engine/model.cpp:1:0

// def (buffer_c)
// external/bonsai_stdlib/src/poof_functions.h:1432:0
link_internal model_buffer
ModelBuffer( u64 ElementCount, memory_arena* Memory)
{
  model_buffer Result = {};

  if (ElementCount)
  {
    Result.Start = Allocate( model, Memory, ElementCount );
    Result.Count = ElementCount;
  }
  else
  {
    Warn("Attempted to allocate model_buffer of 0 length.");
  }

  return Result;
}

