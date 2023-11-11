link_internal b32
Serialize(native_file *File, keyframe *Element)
{
  

  b32 Result = WriteToFile(File, Cast(u8*, Element), sizeof(keyframe));

  

  

  return Result;
}

link_internal keyframe *
Deserialize_keyframe(u8_stream *Bytes)
{
  keyframe *Result = Cast(keyframe*, Bytes->At);
  Bytes->At += sizeof(keyframe);
  Assert(Bytes->At <= Bytes->End);

  
  return Result;
}


