struct asset_slot_buffer
{
  asset_slot *Start;
  umm Count;
};

link_internal asset_slot_buffer
AssetSlotBuffer( umm ElementCount, memory_arena* Memory)
{
  asset_slot_buffer Result = {};

  if (ElementCount)
  {
    Result.Start = Allocate( asset_slot, Memory, ElementCount );
    Result.Count = ElementCount;
  }
  else
  {
    Warn("Attempted to allocate asset_slot_buffer of 0 length.");
  }

  return Result;
}

link_inline umm
ZerothIndex(asset_slot_buffer *Buf)
{
  umm Result = 0;
  return Result;
}

link_inline umm
AtElements(asset_slot_buffer *Buf)
{
  umm Result = Buf->Count;
  return Result;
}

link_inline umm
TotalElements(asset_slot_buffer *Buf)
{
  umm Result = Buf->Count;
  return Result;
}

link_inline umm
CurrentCount(asset_slot_buffer *Buf)
{
  umm Result = Buf->Count;
  return Result;
}

link_inline asset_slot *
GetPtr(asset_slot_buffer *Buf, umm Index)
{
  asset_slot *Result = 0;
  if (Index < Buf->Count) { Result = Buf->Start + Index; }
  return Result;
}

link_inline asset_slot *
Get(asset_slot_buffer *Buf, umm Index)
{
  asset_slot *Result = GetPtr(Buf, Index);
  return Result;
}


