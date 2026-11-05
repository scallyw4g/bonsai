// callsite
// external/bonsai_stdlib/src/thread.h:7:0

// def (buffer_h)
// external/bonsai_stdlib/src/poof_functions.h:1448:0
struct thread_main_callback_type_buffer
{
  u32 Count;
  thread_main_callback_type *Start; poof(@array_length(Element->Count))
};

link_internal thread_main_callback_type_buffer
ThreadMainCallbackTypeBuffer( u32 ElementCount, memory_arena* Memory);

link_internal thread_main_callback_type_buffer
ThreadMainCallbackTypeBuffer( thread_main_callback_type *Start, u32 ElementCount)
{
  thread_main_callback_type_buffer Result = {ElementCount, Start};
  return Result;
}

link_inline u32
LastIndex(thread_main_callback_type_buffer *Buf)
{
  u32 Result = Buf->Count > 0? Buf->Count-1 : 0;
  return Result;
}

link_inline u32
ZerothIndex(thread_main_callback_type_buffer *Buf)
{
  u32 Result = 0;
  return Result;
}

link_inline u32
AtElements(thread_main_callback_type_buffer *Buf)
{
  u32 Result = Buf->Count;
  return Result;
}

link_inline u32
TotalElements(thread_main_callback_type_buffer *Buf)
{
  u32 Result = Buf->Count;
  return Result;
}

link_inline u32
CurrentCount(thread_main_callback_type_buffer *Buf)
{
  u32 Result = Buf->Count;
  return Result;
}

link_inline thread_main_callback_type *
GetPtr(thread_main_callback_type_buffer *Buf, u32 Index)
{
  thread_main_callback_type *Result = 0;
  if (Index < Buf->Count) { Result = Buf->Start + Index; }
  return Result;
}

link_inline thread_main_callback_type *
TryGetPtr(thread_main_callback_type_buffer *Buf, u32 Index)
{
  return GetPtr(Buf, Index);
}

link_inline thread_main_callback_type *
Get(thread_main_callback_type_buffer *Buf, u32 Index)
{
  thread_main_callback_type *Result = GetPtr(Buf, Index);
  return Result;
}

