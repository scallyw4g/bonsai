
    
    struct stl_container_def_stream_chunk
    {
      stl_container_def Element;
      stl_container_def_stream_chunk* Next;
    };

    
    struct stl_container_def_stream
    {
      stl_container_def_stream_chunk* FirstChunk;
      stl_container_def_stream_chunk* LastChunk;
    };


    
    struct stl_container_def_iterator
    {
      stl_container_def_stream* Stream;
      stl_container_def_stream_chunk* At;
    };

    function stl_container_def_iterator
    Iterator(stl_container_def_stream* Stream)
    {
      stl_container_def_iterator Iterator = {
        .Stream = Stream,
        .At = Stream->FirstChunk
      };
      return Iterator;
    }

    function b32
    IsValid(stl_container_def_iterator* Iter)
    {
      b32 Result = Iter->At != 0;
      return Result;
    }

    function void
    Advance(stl_container_def_iterator* Iter)
    {
      Iter->At = Iter->At->Next;
    }


    
    function void
    Push(stl_container_def_stream* Stream, stl_container_def Element, memory_arena* Memory)
    {
      stl_container_def_stream_chunk* NextChunk = (stl_container_def_stream_chunk*)PushStruct(Memory, sizeof( stl_container_def_stream_chunk ), 1, 0);
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


