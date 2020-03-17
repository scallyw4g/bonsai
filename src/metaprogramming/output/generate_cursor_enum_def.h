
    struct enum_def_cursor
    {
      enum_def* Start;
      enum_def* End;
      enum_def* At;
    };

    function enum_def_cursor
    EnumDefCursor(umm ElementCount, memory_arena* Memory)
    {
      enum_def* Start = (enum_def*)PushStruct(Memory, sizeof( enum_def ), 1, 1);
      enum_def_cursor Result = {
        .Start = Start,
        .End = Start+ElementCount,
        .At = Start,
      };
      return Result;
    };

