
    struct c_decl_cursor
    {
      c_decl* Start;
      c_decl* End;
      c_decl* At;
    };

    function c_decl_cursor
    CDeclCursor(umm ElementCount, memory_arena* Memory)
    {
      c_decl* Start = (c_decl*)PushStruct(Memory, sizeof(c_decl), 1, 1);
      c_decl_cursor Result = {
        .Start = Start,
        .End = Start+ElementCount,
        .At = Start,
      };
      return Result;
    };

