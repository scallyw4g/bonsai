// callsite
// src/engine/asset.h:201:0

// def (buffer)
// external/bonsai_stdlib/src/poof_functions.h:1463:0
struct asset_buffer
{
  umm Count;
  asset *Start; poof(@array_length(Element->Count))
};

link_internal asset_buffer
AssetBuffer( umm ElementCount, memory_arena* Memory);

link_internal asset_buffer
AssetBuffer( asset *Start, umm ElementCount)
{
  asset_buffer Result = {ElementCount, Start};
  return Result;
}

link_inline umm
LastIndex(asset_buffer *Buf)
{
  umm Result = Buf->Count > 0? Buf->Count-1 : 0;
  return Result;
}

link_inline umm
ZerothIndex(asset_buffer *Buf)
{
  umm Result = 0;
  return Result;
}

link_inline umm
AtElements(asset_buffer *Buf)
{
  umm Result = Buf->Count;
  return Result;
}

link_inline umm
TotalElements(asset_buffer *Buf)
{
  umm Result = Buf->Count;
  return Result;
}

link_inline umm
CurrentCount(asset_buffer *Buf)
{
  umm Result = Buf->Count;
  return Result;
}

link_inline asset *
GetPtr(asset_buffer *Buf, umm Index)
{
  asset *Result = 0;
  if (Index < Buf->Count) { Result = Buf->Start + Index; }
  return Result;
}

link_inline asset *
TryGetPtr(asset_buffer *Buf, umm Index)
{
  return GetPtr(Buf, Index);
}

link_inline asset *
Get(asset_buffer *Buf, umm Index)
{
  asset *Result = GetPtr(Buf, Index);
  return Result;
}

link_internal asset_buffer
AssetBuffer( umm ElementCount, memory_arena* Memory)
{
  asset_buffer Result = {};

  if (ElementCount)
  {
    Result.Start = Allocate( asset, Memory, ElementCount );
    Result.Count = ElementCount;
  }
  else
  {
    Warn("Attempted to allocate asset_buffer of 0 length.");
  }

  return Result;
}


