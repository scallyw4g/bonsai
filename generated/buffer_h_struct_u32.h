// callsite
// external/bonsai_stdlib/src/shader.h:198:0

// def (buffer_h)
// external/bonsai_stdlib/src/poof_functions.h:1356:0
struct shader_uniform_buffer
{
  u32 Count;
  shader_uniform *Start; poof(@array_length(Element->Count))
};

link_internal shader_uniform_buffer
ShaderUniformBuffer( u32 ElementCount, memory_arena* Memory);

link_internal shader_uniform_buffer
ShaderUniformBuffer( shader_uniform *Start, u32 ElementCount)
{
  shader_uniform_buffer Result = {ElementCount, Start};
  return Result;
}

link_inline u32
LastIndex(shader_uniform_buffer *Buf)
{
  u32 Result = Buf->Count > 0? Buf->Count-1 : 0;
  return Result;
}

link_inline u32
ZerothIndex(shader_uniform_buffer *Buf)
{
  u32 Result = 0;
  return Result;
}

link_inline u32
AtElements(shader_uniform_buffer *Buf)
{
  u32 Result = Buf->Count;
  return Result;
}

link_inline u32
TotalElements(shader_uniform_buffer *Buf)
{
  u32 Result = Buf->Count;
  return Result;
}

link_inline u32
CurrentCount(shader_uniform_buffer *Buf)
{
  u32 Result = Buf->Count;
  return Result;
}

link_inline shader_uniform *
GetPtr(shader_uniform_buffer *Buf, u32 Index)
{
  shader_uniform *Result = 0;
  if (Index < Buf->Count) { Result = Buf->Start + Index; }
  return Result;
}

link_inline shader_uniform *
TryGetPtr(shader_uniform_buffer *Buf, u32 Index)
{
  return GetPtr(Buf, Index);
}

link_inline shader_uniform *
Get(shader_uniform_buffer *Buf, u32 Index)
{
  shader_uniform *Result = GetPtr(Buf, Index);
  return Result;
}

