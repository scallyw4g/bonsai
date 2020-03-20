
    
    struct counted_string_cursor
    {
      counted_string* Start;
      counted_string* End;
      counted_string* At;
    };

    function counted_string_cursor
    CountedStringCursor(umm ElementCount, memory_arena* Memory)
    {
      counted_string* Start = (counted_string*)PushStruct(Memory, sizeof( counted_string ), 1, 1);
      counted_string_cursor Result = {
        .Start = Start,
        .End = Start+ElementCount,
        .At = Start,
      };
      return Result;
    };

    
    
    struct counted_string_stream_chunk
    {
      counted_string Element;
      counted_string_stream_chunk* Next;
    };


    
    struct counted_string_stream
    {
      counted_string_stream_chunk* FirstChunk;
      counted_string_stream_chunk* LastChunk;
    };



    
    struct counted_string_iterator
    {
      counted_string_stream* Stream;
      counted_string_stream_chunk* At;
    };

    function counted_string_iterator
    Iterator(counted_string_stream* Stream)
    {
      counted_string_iterator Iterator = {
        .Stream = Stream,
        .At = Stream->FirstChunk
      };
      return Iterator;
    }

    function b32
    IsValid(counted_string_iterator* Iter)
    {
      b32 Result = Iter->At != 0;
      return Result;
    }

    function void
    Advance(counted_string_iterator* Iter)
    {
      Iter->At = Iter->At->Next;
    }



    
    function void
    Push(counted_string_stream* Stream, counted_string Element, memory_arena* Memory)
    {
      counted_string_stream_chunk* NextChunk = (counted_string_stream_chunk*)PushStruct(Memory, sizeof( counted_string_stream_chunk ), 1, 1);
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



