
  link_internal b32
  Deserialize(u8_stream *Bytes, aabb *Element, memory_arena *Memory)
  {
    b32 Result = True;
    // NOTE(Jesse): Unfortunately we can't check for primitives because
    // strings are considered primitive, but need memory to deserialize
    Result &= Deserialize(Bytes, &Element->Min, Memory);





    // NOTE(Jesse): Unfortunately we can't check for primitives because
    // strings are considered primitive, but need memory to deserialize
    Result &= Deserialize(Bytes, &Element->Max, Memory);

    

    MAYBE_READ_DEBUG_OBJECT_DELIM();
    return Result;
  }

  
