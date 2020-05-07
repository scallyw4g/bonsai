
    
    struct meta_func_arg_stream_chunk
    {
      meta_func_arg Element;
      meta_func_arg_stream_chunk* Next;
    };

    
    struct meta_func_arg_stream
    {
      meta_func_arg_stream_chunk* FirstChunk;
      meta_func_arg_stream_chunk* LastChunk;
    };


    
    struct meta_func_arg_iterator
    {
      meta_func_arg_stream* Stream;
      meta_func_arg_stream_chunk* At;
    };

    function meta_func_arg_iterator
    Iterator(meta_func_arg_stream* Stream)
    {
      meta_func_arg_iterator Iterator = {
        .Stream = Stream,
        .At = Stream->FirstChunk
      };
      return Iterator;
    }

    function b32
    IsValid(meta_func_arg_iterator* Iter)
    {
      b32 Result = Iter->At != 0;
      return Result;
    }

    function void
    Advance(meta_func_arg_iterator* Iter)
    {
      Iter->At = Iter->At->Next;
    }


    
    function void
    Push(meta_func_arg_stream* Stream, meta_func_arg Element, memory_arena* Memory)
    {
      meta_func_arg_stream_chunk* NextChunk = (meta_func_arg_stream_chunk*)PushStruct(Memory, sizeof( meta_func_arg_stream_chunk ), 1, 0);
      NextChunk->Element = Element;

      if (!Stream->FirstChunk)
      {
        Assert(!Stream->LastChunk);
        Stream->FirstChunk = NextChunk;
        Stream->LastChunk = NextChunk;
      }
      else
      {
        Stream->LastChunk->Next = NextChunk;
        Stream->LastChunk = NextChunk;
      }

      Assert(NextChunk->Next == 0);
      Assert(Stream->LastChunk->Next == 0);

      return;
    }


