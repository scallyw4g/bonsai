link_internal b32
Serialize(native_file *File, Quaternion *Element)
{
  b32 Result = WriteToFile(File, Cast(u8*, Element), sizeof(Quaternion));
  return Result;
}

link_internal Quaternion *
Deserialize_Quaternion(u8_stream *Bytes)
{
  Quaternion *Result = Cast(Quaternion*, Bytes->At);
  Bytes->At += sizeof(Quaternion);
  Assert(Bytes->At <= Bytes->End);
  return Result;
}


