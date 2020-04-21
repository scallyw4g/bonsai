
    
    struct replacement_pattern_stream_chunk
    {
      replacement_pattern Element;
      replacement_pattern_stream_chunk* Next;
    };


    
    struct replacement_pattern_stream
    {
      replacement_pattern_stream_chunk* FirstChunk;
      replacement_pattern_stream_chunk* LastChunk;
    };



    
    struct replacement_pattern_iterator
    {
      replacement_pattern_stream* Stream;
      replacement_pattern_stream_chunk* At;
    };

    function replacement_pattern_iterator
    Iterator(replacement_pattern_stream* Stream)
    {
      replacement_pattern_iterator Iterator = {
        .Stream = Stream,
        .At = Stream->FirstChunk
      };
      return Iterator;
    }

    function b32
    IsValid(replacement_pattern_iterator* Iter)
    {
      b32 Result = Iter->At != 0;
      return Result;
    }

    function void
    Advance(replacement_pattern_iterator* Iter)
    {
      Iter->At = Iter->At->Next;
    }



    
    function void
    Push(replacement_pattern_stream* Stream, replacement_pattern Element, memory_arena* Memory)
    {
      replacement_pattern_stream_chunk* NextChunk = (replacement_pattern_stream_chunk*)PushStruct(Memory, sizeof( replacement_pattern_stream_chunk ), 1, 1);
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


