link_internal b32
Serialize(native_file *File, u8 *Element)
{
  b32 Result = WriteToFile(File, Cast(u8*, Element), sizeof(u8));
  return Result;
}

link_internal u8
Deserialize_u8(u8_stream *Bytes)
{
  u8 Result = *Cast(u8*, Bytes->At);
  Bytes->At += sizeof(u8);
  Assert(Bytes->At <= Bytes->End);
  return Result;
}
link_internal b32
Serialize(native_file *File, s8 *Element)
{
  b32 Result = WriteToFile(File, Cast(u8*, Element), sizeof(s8));
  return Result;
}

link_internal s8
Deserialize_s8(u8_stream *Bytes)
{
  s8 Result = *Cast(s8*, Bytes->At);
  Bytes->At += sizeof(s8);
  Assert(Bytes->At <= Bytes->End);
  return Result;
}
link_internal b32
Serialize(native_file *File, u16 *Element)
{
  b32 Result = WriteToFile(File, Cast(u8*, Element), sizeof(u16));
  return Result;
}

link_internal u16
Deserialize_u16(u8_stream *Bytes)
{
  u16 Result = *Cast(u16*, Bytes->At);
  Bytes->At += sizeof(u16);
  Assert(Bytes->At <= Bytes->End);
  return Result;
}
link_internal b32
Serialize(native_file *File, s16 *Element)
{
  b32 Result = WriteToFile(File, Cast(u8*, Element), sizeof(s16));
  return Result;
}

link_internal s16
Deserialize_s16(u8_stream *Bytes)
{
  s16 Result = *Cast(s16*, Bytes->At);
  Bytes->At += sizeof(s16);
  Assert(Bytes->At <= Bytes->End);
  return Result;
}
link_internal b32
Serialize(native_file *File, u32 *Element)
{
  b32 Result = WriteToFile(File, Cast(u8*, Element), sizeof(u32));
  return Result;
}

link_internal u32
Deserialize_u32(u8_stream *Bytes)
{
  u32 Result = *Cast(u32*, Bytes->At);
  Bytes->At += sizeof(u32);
  Assert(Bytes->At <= Bytes->End);
  return Result;
}
link_internal b32
Serialize(native_file *File, s32 *Element)
{
  b32 Result = WriteToFile(File, Cast(u8*, Element), sizeof(s32));
  return Result;
}

link_internal s32
Deserialize_s32(u8_stream *Bytes)
{
  s32 Result = *Cast(s32*, Bytes->At);
  Bytes->At += sizeof(s32);
  Assert(Bytes->At <= Bytes->End);
  return Result;
}
link_internal b32
Serialize(native_file *File, u64 *Element)
{
  b32 Result = WriteToFile(File, Cast(u8*, Element), sizeof(u64));
  return Result;
}

link_internal u64
Deserialize_u64(u8_stream *Bytes)
{
  u64 Result = *Cast(u64*, Bytes->At);
  Bytes->At += sizeof(u64);
  Assert(Bytes->At <= Bytes->End);
  return Result;
}
link_internal b32
Serialize(native_file *File, s64 *Element)
{
  b32 Result = WriteToFile(File, Cast(u8*, Element), sizeof(s64));
  return Result;
}

link_internal s64
Deserialize_s64(u8_stream *Bytes)
{
  s64 Result = *Cast(s64*, Bytes->At);
  Bytes->At += sizeof(s64);
  Assert(Bytes->At <= Bytes->End);
  return Result;
}
link_internal b32
Serialize(native_file *File, b8 *Element)
{
  b32 Result = WriteToFile(File, Cast(u8*, Element), sizeof(b8));
  return Result;
}

link_internal b8
Deserialize_b8(u8_stream *Bytes)
{
  b8 Result = *Cast(b8*, Bytes->At);
  Bytes->At += sizeof(b8);
  Assert(Bytes->At <= Bytes->End);
  return Result;
}
link_internal b32
Serialize(native_file *File, r32 *Element)
{
  b32 Result = WriteToFile(File, Cast(u8*, Element), sizeof(r32));
  return Result;
}

link_internal r32
Deserialize_r32(u8_stream *Bytes)
{
  r32 Result = *Cast(r32*, Bytes->At);
  Bytes->At += sizeof(r32);
  Assert(Bytes->At <= Bytes->End);
  return Result;
}
link_internal b32
Serialize(native_file *File, r64 *Element)
{
  b32 Result = WriteToFile(File, Cast(u8*, Element), sizeof(r64));
  return Result;
}

link_internal r64
Deserialize_r64(u8_stream *Bytes)
{
  r64 Result = *Cast(r64*, Bytes->At);
  Bytes->At += sizeof(r64);
  Assert(Bytes->At <= Bytes->End);
  return Result;
}

