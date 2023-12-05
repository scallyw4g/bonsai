link_internal b32
AreEqual(asset_id *Thing1, asset_id *Thing2)
{
  b32 Result = MemoryIsEqual((u8*)Thing1, (u8*)Thing2, sizeof( asset_id ) );
  return Result;
}

