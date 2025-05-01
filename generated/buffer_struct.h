// external/bonsai_stdlib/src/shader.h:111:0

struct shader_uniform_buffer
{
  umm Count;
  shader_uniform *Start; poof(@array_length(Element->Count))
};

link_internal shader_uniform_buffer
ShaderUniformBuffer( shader_uniform *Start, umm ElementCount)
{
  shader_uniform_buffer Result = {ElementCount, Start};
  return Result;
}

link_internal shader_uniform_buffer
ShaderUniformBuffer( umm ElementCount, memory_arena* Memory)
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

link_inline umm
LastIndex(shader_uniform_buffer *Buf)
{
  umm Result = Buf->Count > 0? Buf->Count-1 : 0;
  return Result;
}

link_inline umm
ZerothIndex(shader_uniform_buffer *Buf)
{
  umm Result = 0;
  return Result;
}

link_inline umm
AtElements(shader_uniform_buffer *Buf)
{
  umm Result = Buf->Count;
  return Result;
}

link_inline umm
TotalElements(shader_uniform_buffer *Buf)
{
  umm Result = Buf->Count;
  return Result;
}

link_inline umm
CurrentCount(shader_uniform_buffer *Buf)
{
  umm Result = Buf->Count;
  return Result;
}

link_inline shader_uniform *
GetPtr(shader_uniform_buffer *Buf, umm Index)
{
  shader_uniform *Result = 0;
  if (Index < Buf->Count) { Result = Buf->Start + Index; }
  return Result;
}

link_inline shader_uniform *
Get(shader_uniform_buffer *Buf, umm Index)
{
  shader_uniform *Result = GetPtr(Buf, Index);
  return Result;
}


