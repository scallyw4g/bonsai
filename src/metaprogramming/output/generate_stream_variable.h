
    
    struct variable_stream_chunk
    {
      variable Element;
      variable_stream_chunk* Next;
    };


    
    struct variable_stream
    {
      variable_stream_chunk* FirstChunk;
      variable_stream_chunk* LastChunk;
    };



    
    struct variable_iterator
    {
      variable_stream* Stream;
      variable_stream_chunk* At;
    };

    function variable_iterator
    Iterator(variable_stream* Stream)
    {
      variable_iterator Iterator = {
        .Stream = Stream,
        .At = Stream->FirstChunk
      };
      return Iterator;
    }

    function b32
    IsValid(variable_iterator* Iter)
    {
      b32 Result = Iter->At != 0;
      return Result;
    }

    function void
    Advance(variable_iterator* Iter)
    {
      Iter->At = Iter->At->Next;
    }



    
    function void
    Push(variable_stream* Stream, variable Element, memory_arena* Memory)
    {
      variable_stream_chunk* NextChunk = (variable_stream_chunk*)PushStruct(Memory, sizeof( variable_stream_chunk ), 1, 1);
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


