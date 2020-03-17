
    struct test_meta_struct_stream_chunk
    {
      test_meta_struct Element;
      test_meta_struct_stream_chunk* Next;
    };

    struct test_meta_struct_stream
    {
      test_meta_struct_stream_chunk* FirstChunk;
      test_meta_struct_stream_chunk* LastChunk;
    };

    function void
    Push(test_meta_struct_stream* Stream, test_meta_struct Element, memory_arena* Memory)
    {
      test_meta_struct_stream_chunk* NextChunk = (test_meta_struct_stream_chunk*)PushStruct(Memory, sizeof(test_meta_struct_stream_chunk), 1, 1);
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

    struct test_meta_struct_iterator
    {
      test_meta_struct_stream* Stream;
      test_meta_struct_stream_chunk* At;
    };

    function test_meta_struct_iterator
    Iterator(test_meta_struct_stream* Stream)
    {
      test_meta_struct_iterator Iterator = {
        .Stream = Stream,
        .At = Stream->FirstChunk
      };
      return Iterator;
    }

    function b32
    IsValid(test_meta_struct_iterator* Iter)
    {
      b32 Result = Iter->At != 0;
      return Result;
    }

    function void
    Advance(test_meta_struct_iterator* Iter)
    {
      Iter->At = Iter->At->Next;
    }


