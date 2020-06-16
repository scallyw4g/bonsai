
    struct c_token_buffer_buffer
    {
      c_token_buffer* Start;
      umm Count;
    };

    function c_token_buffer_buffer
    CTokenBufferBuffer(umm ElementCount, memory_arena* Memory)
    {
      c_token_buffer_buffer Result = {};

      if (ElementCount)
      {
        Result.Start = Allocate( c_token_buffer, Memory, ElementCount );
        Result.Count = ElementCount;
      }
      else
      {
        Warn("Attempted to allocate c_token_buffer_buffer of 0 length.");
      }

      return Result;
    }

