
    
    struct function_def_stream_chunk
    {
      function_def Element;
      function_def_stream_chunk* Next;
    };


    
    struct function_def_stream
    {
      function_def_stream_chunk* FirstChunk;
      function_def_stream_chunk* LastChunk;
    };



    
    struct function_def_iterator
    {
      function_def_stream* Stream;
      function_def_stream_chunk* At;
    };

    function function_def_iterator
    Iterator(function_def_stream* Stream)
    {
      function_def_iterator Iterator = {
        .Stream = Stream,
        .At = Stream->FirstChunk
      };
      return Iterator;
    }

    function b32
    IsValid(function_def_iterator* Iter)
    {
      b32 Result = Iter->At != 0;
      return Result;
    }

    function void
    Advance(function_def_iterator* Iter)
    {
      Iter->At = Iter->At->Next;
    }



    
    function void
    Push(function_def_stream* Stream, function_def Element, memory_arena* Memory)
    {
      function_def_stream_chunk* NextChunk = (function_def_stream_chunk*)PushStruct(Memory, sizeof( function_def_stream_chunk ), 1, 1);
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


