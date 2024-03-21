// src/engine/serdes.h:436:0

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
  *Element = *Cast(u8*, Bytes->At);
  Bytes->At += sizeof(u8) * Count;
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
  *Element = *Cast(s8*, Bytes->At);
  Bytes->At += sizeof(s8) * Count;
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
  *Element = *Cast(u16*, Bytes->At);
  Bytes->At += sizeof(u16) * Count;
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
  *Element = *Cast(s16*, Bytes->At);
  Bytes->At += sizeof(s16) * Count;
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
  *Element = *Cast(u32*, Bytes->At);
  Bytes->At += sizeof(u32) * Count;
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
  *Element = *Cast(s32*, Bytes->At);
  Bytes->At += sizeof(s32) * Count;
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
  *Element = *Cast(u64*, Bytes->At);
  Bytes->At += sizeof(u64) * Count;
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
  *Element = *Cast(s64*, Bytes->At);
  Bytes->At += sizeof(s64) * Count;
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
  *Element = *Cast(b8*, Bytes->At);
  Bytes->At += sizeof(b8) * Count;
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
  *Element = *Cast(r32*, Bytes->At);
  Bytes->At += sizeof(r32) * Count;
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
  *Element = *Cast(r64*, Bytes->At);
  Bytes->At += sizeof(r64) * Count;
  Assert(Bytes->At <= Bytes->End);
  return True;
}

/* serdes_array(type) */

