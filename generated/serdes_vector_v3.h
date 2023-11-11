link_internal b32
Serialize(native_file *File, v3 *Element)
{
  b32 Result = WriteToFile(File, Cast(u8*, Element), sizeof(v3));

  u64 Tag = LEVEL_FILE_OBJECT_DELIM;
  Ensure( Serialize(File, &Tag) );
  return Result;
}

link_internal b32
Deserialize(u8_stream *Bytes, v3* Element, memory_arena *Ignored)
{
  *Element = *Cast(v3*, Bytes->At);
  Bytes->At += sizeof(v3);
  Assert(Bytes->At <= Bytes->End);

  u64 Tag = Read_u64(Bytes);
  Ensure( Tag == LEVEL_FILE_OBJECT_DELIM );
  return True;
}


