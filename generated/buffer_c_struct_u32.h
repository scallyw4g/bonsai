// callsite
// external/bonsai_stdlib/src/shader.cpp:1:0

// def (buffer_c)
// external/bonsai_stdlib/src/poof_functions.h:1432:0
link_internal shader_uniform_buffer
ShaderUniformBuffer( u32 ElementCount, memory_arena* Memory)
{
  shader_uniform_buffer Result = {};

  if (ElementCount)
  {
    Result.Start = Allocate( shader_uniform, Memory, ElementCount );
    Result.Count = ElementCount;
  }
  else
  {
    Warn("Attempted to allocate shader_uniform_buffer of 0 length.");
  }

  return Result;
}

