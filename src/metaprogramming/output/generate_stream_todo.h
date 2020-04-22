
    
    struct todo_stream_chunk
    {
      todo Element;
      todo_stream_chunk* Next;
    };

    
    struct todo_stream
    {
      todo_stream_chunk* FirstChunk;
      todo_stream_chunk* LastChunk;
    };


    
    struct todo_iterator
    {
      todo_stream* Stream;
      todo_stream_chunk* At;
    };

    function todo_iterator
    Iterator(todo_stream* Stream)
    {
      todo_iterator Iterator = {
        .Stream = Stream,
        .At = Stream->FirstChunk
      };
      return Iterator;
    }

    function b32
    IsValid(todo_iterator* Iter)
    {
      b32 Result = Iter->At != 0;
      return Result;
    }

    function void
    Advance(todo_iterator* Iter)
    {
      Iter->At = Iter->At->Next;
    }


    
    function void
    Push(todo_stream* Stream, todo Element, memory_arena* Memory)
    {
      todo_stream_chunk* NextChunk = (todo_stream_chunk*)PushStruct(Memory, sizeof( todo_stream_chunk ), 1, 1);
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


