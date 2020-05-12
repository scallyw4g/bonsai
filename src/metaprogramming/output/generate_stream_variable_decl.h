
    
    struct variable_decl_stream_chunk
    {
      variable_decl Element;
      variable_decl_stream_chunk* Next;
    };

    
    struct variable_decl_stream
    {
      variable_decl_stream_chunk* FirstChunk;
      variable_decl_stream_chunk* LastChunk;
    };


    
    struct variable_decl_iterator
    {
      variable_decl_stream* Stream;
      variable_decl_stream_chunk* At;
    };

    function variable_decl_iterator
    Iterator(variable_decl_stream* Stream)
    {
      variable_decl_iterator Iterator = {
        .Stream = Stream,
        .At = Stream->FirstChunk
      };
      return Iterator;
    }

    function b32
    IsValid(variable_decl_iterator* Iter)
    {
      b32 Result = Iter->At != 0;
      return Result;
    }

    function void
    Advance(variable_decl_iterator* Iter)
    {
      Iter->At = Iter->At->Next;
    }


    
    function void
    Push(variable_decl_stream* Stream, variable_decl Element, memory_arena* Memory)
    {
      variable_decl_stream_chunk* NextChunk = (variable_decl_stream_chunk*)PushStruct(Memory, sizeof( variable_decl_stream_chunk ), 1, 0);
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


