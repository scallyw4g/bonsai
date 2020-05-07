
    struct struct_member_cursor
    {
      struct_member* Start;
      struct_member* End;
      struct_member* At;
    };

    function struct_member_cursor
    StructMemberCursor(umm ElementCount, memory_arena* Memory)
    {
      struct_member* Start = (struct_member*)PushStruct(Memory, sizeof( struct_member ), 1, 0);
      struct_member_cursor Result = {
        .Start = Start,
        .End = Start+ElementCount,
        .At = Start,
      };
      return Result;
    }

