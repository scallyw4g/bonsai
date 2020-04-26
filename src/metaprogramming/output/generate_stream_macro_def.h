
    
    struct macro_def_stream_chunk
    {
      macro_def Element;
      macro_def_stream_chunk* Next;
    };

    
    struct macro_def_stream
    {
      macro_def_stream_chunk* FirstChunk;
      macro_def_stream_chunk* LastChunk;
    };


    
    struct macro_def_iterator
    {
      macro_def_stream* Stream;
      macro_def_stream_chunk* At;
    };

    function macro_def_iterator
    Iterator(macro_def_stream* Stream)
    {
      macro_def_iterator Iterator = {
        .Stream = Stream,
        .At = Stream->FirstChunk
      };
      return Iterator;
    }

    function b32
    IsValid(macro_def_iterator* Iter)
    {
      b32 Result = Iter->At != 0;
      return Result;
    }

    function void
    Advance(macro_def_iterator* Iter)
    {
      Iter->At = Iter->At->Next;
    }


    
    function void
    Push(macro_def_stream* Stream, macro_def Element, memory_arena* Memory)
    {
      macro_def_stream_chunk* NextChunk = (macro_def_stream_chunk*)PushStruct(Memory, sizeof( macro_def_stream_chunk ), 1, 1);
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


