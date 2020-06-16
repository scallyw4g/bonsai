
    struct c_token_buffer
    {
      c_token* Start;
      umm Count;
    };

    function c_token_buffer
    CTokenBuffer(umm ElementCount, memory_arena* Memory)
    {
      c_token_buffer Result = {};

      if (ElementCount)
      {
        Result.Start = Allocate( c_token, Memory, ElementCount );
        Result.Count = ElementCount;
      }
      else
      {
        Warn("Attempted to allocate c_token_buffer of 0 length.");
      }

      return Result;
    }

