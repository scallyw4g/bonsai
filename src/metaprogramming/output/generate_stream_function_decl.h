
    
    struct function_decl_stream_chunk
    {
      function_decl Element;
      function_decl_stream_chunk* Next;
    };

    
    struct function_decl_stream
    {
      function_decl_stream_chunk* FirstChunk;
      function_decl_stream_chunk* LastChunk;
    };


    
    struct function_decl_iterator
    {
      function_decl_stream* Stream;
      function_decl_stream_chunk* At;
    };

    function function_decl_iterator
    Iterator(function_decl_stream* Stream)
    {
      function_decl_iterator Iterator = {
        .Stream = Stream,
        .At = Stream->FirstChunk
      };
      return Iterator;
    }

    function b32
    IsValid(function_decl_iterator* Iter)
    {
      b32 Result = Iter->At != 0;
      return Result;
    }

    function void
    Advance(function_decl_iterator* Iter)
    {
      Iter->At = Iter->At->Next;
    }


    
    function void
    Push(function_decl_stream* Stream, function_decl Element, memory_arena* Memory)
    {
      function_decl_stream_chunk* NextChunk = (function_decl_stream_chunk*)PushStruct(Memory, sizeof( function_decl_stream_chunk ), 1, 0);
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


