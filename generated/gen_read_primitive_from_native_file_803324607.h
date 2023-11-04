link_internal s8
Read_s8(native_file *File)
{
  s8 Result;
  Ensure(ReadBytesIntoBuffer(File, sizeof(s8), (u8*)&Result));
  return Result;
}
link_internal u8
Read_u8(native_file *File)
{
  u8 Result;
  Ensure(ReadBytesIntoBuffer(File, sizeof(u8), (u8*)&Result));
  return Result;
}
link_internal s16
Read_s16(native_file *File)
{
  s16 Result;
  Ensure(ReadBytesIntoBuffer(File, sizeof(s16), (u8*)&Result));
  return Result;
}
link_internal u16
Read_u16(native_file *File)
{
  u16 Result;
  Ensure(ReadBytesIntoBuffer(File, sizeof(u16), (u8*)&Result));
  return Result;
}
link_internal s32
Read_s32(native_file *File)
{
  s32 Result;
  Ensure(ReadBytesIntoBuffer(File, sizeof(s32), (u8*)&Result));
  return Result;
}
link_internal u32
Read_u32(native_file *File)
{
  u32 Result;
  Ensure(ReadBytesIntoBuffer(File, sizeof(u32), (u8*)&Result));
  return Result;
}
link_internal s64
Read_s64(native_file *File)
{
  s64 Result;
  Ensure(ReadBytesIntoBuffer(File, sizeof(s64), (u8*)&Result));
  return Result;
}
link_internal u64
Read_u64(native_file *File)
{
  u64 Result;
  Ensure(ReadBytesIntoBuffer(File, sizeof(u64), (u8*)&Result));
  return Result;
}

