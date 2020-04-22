
    
    struct ast_node_variable_def_stream_chunk
    {
      ast_node_variable_def Element;
      ast_node_variable_def_stream_chunk* Next;
    };

    
    struct ast_node_variable_def_stream
    {
      ast_node_variable_def_stream_chunk* FirstChunk;
      ast_node_variable_def_stream_chunk* LastChunk;
    };


    
    struct ast_node_variable_def_iterator
    {
      ast_node_variable_def_stream* Stream;
      ast_node_variable_def_stream_chunk* At;
    };

    function ast_node_variable_def_iterator
    Iterator(ast_node_variable_def_stream* Stream)
    {
      ast_node_variable_def_iterator Iterator = {
        .Stream = Stream,
        .At = Stream->FirstChunk
      };
      return Iterator;
    }

    function b32
    IsValid(ast_node_variable_def_iterator* Iter)
    {
      b32 Result = Iter->At != 0;
      return Result;
    }

    function void
    Advance(ast_node_variable_def_iterator* Iter)
    {
      Iter->At = Iter->At->Next;
    }


    
    function void
    Push(ast_node_variable_def_stream* Stream, ast_node_variable_def Element, memory_arena* Memory)
    {
      ast_node_variable_def_stream_chunk* NextChunk = (ast_node_variable_def_stream_chunk*)PushStruct(Memory, sizeof( ast_node_variable_def_stream_chunk ), 1, 1);
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


