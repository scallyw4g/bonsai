// callsite
// src/engine/serdes.h:697:0

// def (serdes_primitive)
// src/engine/serdes.h:56:0
link_internal b32
Serialize(u8_cursor_block_array *Bytes, u8 *Element, umm Count = 1)
{
  Assert(Count > 0);
  b32 Result = Write(Bytes, Cast(u8*, Element), sizeof(u8)*Count);
  return Result;
}

link_internal b32
Deserialize(u8_cursor *Bytes, u8 *Element, memory_arena *Ignored = 0, umm Count = 1)
{
  Assert(Count > 0);

  umm ByteCount = sizeof(u8) * Count;
  CopyMemory( Cast(u8*, Bytes->At), Cast(u8*, Element), ByteCount);
  Bytes->At += ByteCount;

  Assert(Bytes->At <= Bytes->End);
  return True;
}

/* serdes_array(type) */
link_internal b32
Serialize(u8_cursor_block_array *Bytes, s8 *Element, umm Count = 1)
{
  Assert(Count > 0);
  b32 Result = Write(Bytes, Cast(u8*, Element), sizeof(s8)*Count);
  return Result;
}

link_internal b32
Deserialize(u8_cursor *Bytes, s8 *Element, memory_arena *Ignored = 0, umm Count = 1)
{
  Assert(Count > 0);

  umm ByteCount = sizeof(s8) * Count;
  CopyMemory( Cast(u8*, Bytes->At), Cast(u8*, Element), ByteCount);
  Bytes->At += ByteCount;

  Assert(Bytes->At <= Bytes->End);
  return True;
}

/* serdes_array(type) */
link_internal b32
Serialize(u8_cursor_block_array *Bytes, u16 *Element, umm Count = 1)
{
  Assert(Count > 0);
  b32 Result = Write(Bytes, Cast(u8*, Element), sizeof(u16)*Count);
  return Result;
}

link_internal b32
Deserialize(u8_cursor *Bytes, u16 *Element, memory_arena *Ignored = 0, umm Count = 1)
{
  Assert(Count > 0);

  umm ByteCount = sizeof(u16) * Count;
  CopyMemory( Cast(u8*, Bytes->At), Cast(u8*, Element), ByteCount);
  Bytes->At += ByteCount;

  Assert(Bytes->At <= Bytes->End);
  return True;
}

/* serdes_array(type) */
link_internal b32
Serialize(u8_cursor_block_array *Bytes, s16 *Element, umm Count = 1)
{
  Assert(Count > 0);
  b32 Result = Write(Bytes, Cast(u8*, Element), sizeof(s16)*Count);
  return Result;
}

link_internal b32
Deserialize(u8_cursor *Bytes, s16 *Element, memory_arena *Ignored = 0, umm Count = 1)
{
  Assert(Count > 0);

  umm ByteCount = sizeof(s16) * Count;
  CopyMemory( Cast(u8*, Bytes->At), Cast(u8*, Element), ByteCount);
  Bytes->At += ByteCount;

  Assert(Bytes->At <= Bytes->End);
  return True;
}

/* serdes_array(type) */
link_internal b32
Serialize(u8_cursor_block_array *Bytes, u32 *Element, umm Count = 1)
{
  Assert(Count > 0);
  b32 Result = Write(Bytes, Cast(u8*, Element), sizeof(u32)*Count);
  return Result;
}

link_internal b32
Deserialize(u8_cursor *Bytes, u32 *Element, memory_arena *Ignored = 0, umm Count = 1)
{
  Assert(Count > 0);

  umm ByteCount = sizeof(u32) * Count;
  CopyMemory( Cast(u8*, Bytes->At), Cast(u8*, Element), ByteCount);
  Bytes->At += ByteCount;

  Assert(Bytes->At <= Bytes->End);
  return True;
}

/* serdes_array(type) */
link_internal b32
Serialize(u8_cursor_block_array *Bytes, s32 *Element, umm Count = 1)
{
  Assert(Count > 0);
  b32 Result = Write(Bytes, Cast(u8*, Element), sizeof(s32)*Count);
  return Result;
}

link_internal b32
Deserialize(u8_cursor *Bytes, s32 *Element, memory_arena *Ignored = 0, umm Count = 1)
{
  Assert(Count > 0);

  umm ByteCount = sizeof(s32) * Count;
  CopyMemory( Cast(u8*, Bytes->At), Cast(u8*, Element), ByteCount);
  Bytes->At += ByteCount;

  Assert(Bytes->At <= Bytes->End);
  return True;
}

/* serdes_array(type) */
link_internal b32
Serialize(u8_cursor_block_array *Bytes, u64 *Element, umm Count = 1)
{
  Assert(Count > 0);
  b32 Result = Write(Bytes, Cast(u8*, Element), sizeof(u64)*Count);
  return Result;
}

link_internal b32
Deserialize(u8_cursor *Bytes, u64 *Element, memory_arena *Ignored = 0, umm Count = 1)
{
  Assert(Count > 0);

  umm ByteCount = sizeof(u64) * Count;
  CopyMemory( Cast(u8*, Bytes->At), Cast(u8*, Element), ByteCount);
  Bytes->At += ByteCount;

  Assert(Bytes->At <= Bytes->End);
  return True;
}

/* serdes_array(type) */
link_internal b32
Serialize(u8_cursor_block_array *Bytes, s64 *Element, umm Count = 1)
{
  Assert(Count > 0);
  b32 Result = Write(Bytes, Cast(u8*, Element), sizeof(s64)*Count);
  return Result;
}

link_internal b32
Deserialize(u8_cursor *Bytes, s64 *Element, memory_arena *Ignored = 0, umm Count = 1)
{
  Assert(Count > 0);

  umm ByteCount = sizeof(s64) * Count;
  CopyMemory( Cast(u8*, Bytes->At), Cast(u8*, Element), ByteCount);
  Bytes->At += ByteCount;

  Assert(Bytes->At <= Bytes->End);
  return True;
}

/* serdes_array(type) */
link_internal b32
Serialize(u8_cursor_block_array *Bytes, b8 *Element, umm Count = 1)
{
  Assert(Count > 0);
  b32 Result = Write(Bytes, Cast(u8*, Element), sizeof(b8)*Count);
  return Result;
}

link_internal b32
Deserialize(u8_cursor *Bytes, b8 *Element, memory_arena *Ignored = 0, umm Count = 1)
{
  Assert(Count > 0);

  umm ByteCount = sizeof(b8) * Count;
  CopyMemory( Cast(u8*, Bytes->At), Cast(u8*, Element), ByteCount);
  Bytes->At += ByteCount;

  Assert(Bytes->At <= Bytes->End);
  return True;
}

/* serdes_array(type) */
link_internal b32
Serialize(u8_cursor_block_array *Bytes, r32 *Element, umm Count = 1)
{
  Assert(Count > 0);
  b32 Result = Write(Bytes, Cast(u8*, Element), sizeof(r32)*Count);
  return Result;
}

link_internal b32
Deserialize(u8_cursor *Bytes, r32 *Element, memory_arena *Ignored = 0, umm Count = 1)
{
  Assert(Count > 0);

  umm ByteCount = sizeof(r32) * Count;
  CopyMemory( Cast(u8*, Bytes->At), Cast(u8*, Element), ByteCount);
  Bytes->At += ByteCount;

  Assert(Bytes->At <= Bytes->End);
  return True;
}

/* serdes_array(type) */
link_internal b32
Serialize(u8_cursor_block_array *Bytes, r64 *Element, umm Count = 1)
{
  Assert(Count > 0);
  b32 Result = Write(Bytes, Cast(u8*, Element), sizeof(r64)*Count);
  return Result;
}

link_internal b32
Deserialize(u8_cursor *Bytes, r64 *Element, memory_arena *Ignored = 0, umm Count = 1)
{
  Assert(Count > 0);

  umm ByteCount = sizeof(r64) * Count;
  CopyMemory( Cast(u8*, Bytes->At), Cast(u8*, Element), ByteCount);
  Bytes->At += ByteCount;

  Assert(Bytes->At <= Bytes->End);
  return True;
}

/* serdes_array(type) */


