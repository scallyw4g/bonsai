// callsite
// external/bonsai_stdlib/src/thread.cpp:1:0

// def (buffer)
// external/bonsai_stdlib/src/poof_functions.h:1463:0
struct thread_main_callback_type_buffer
{
  umm Count;
  thread_main_callback_type *Start; poof(@array_length(Element->Count))
};

link_internal thread_main_callback_type_buffer
ThreadMainCallbackTypeBuffer( umm ElementCount, memory_arena* Memory);

link_internal thread_main_callback_type_buffer
ThreadMainCallbackTypeBuffer( thread_main_callback_type *Start, umm ElementCount)
{
  thread_main_callback_type_buffer Result = {ElementCount, Start};
  return Result;
}

link_inline umm
LastIndex(thread_main_callback_type_buffer *Buf)
{
  umm Result = Buf->Count > 0? Buf->Count-1 : 0;
  return Result;
}

link_inline umm
ZerothIndex(thread_main_callback_type_buffer *Buf)
{
  umm Result = 0;
  return Result;
}

link_inline umm
AtElements(thread_main_callback_type_buffer *Buf)
{
  umm Result = Buf->Count;
  return Result;
}

link_inline umm
TotalElements(thread_main_callback_type_buffer *Buf)
{
  umm Result = Buf->Count;
  return Result;
}

link_inline umm
CurrentCount(thread_main_callback_type_buffer *Buf)
{
  umm Result = Buf->Count;
  return Result;
}

link_inline thread_main_callback_type *
GetPtr(thread_main_callback_type_buffer *Buf, umm Index)
{
  thread_main_callback_type *Result = 0;
  if (Index < Buf->Count) { Result = Buf->Start + Index; }
  return Result;
}

link_inline thread_main_callback_type *
TryGetPtr(thread_main_callback_type_buffer *Buf, umm Index)
{
  return GetPtr(Buf, Index);
}

link_inline thread_main_callback_type *
Get(thread_main_callback_type_buffer *Buf, umm Index)
{
  thread_main_callback_type *Result = GetPtr(Buf, Index);
  return Result;
}

link_internal thread_main_callback_type_buffer
ThreadMainCallbackTypeBuffer( umm ElementCount, memory_arena* Memory)
{
  thread_main_callback_type_buffer Result = {};

  if (ElementCount)
  {
    Result.Start = Allocate( thread_main_callback_type, Memory, ElementCount );
    Result.Count = ElementCount;
  }
  else
  {
    Warn("Attempted to allocate thread_main_callback_type_buffer of 0 length.");
  }

  return Result;
}


