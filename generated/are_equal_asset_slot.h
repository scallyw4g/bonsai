link_internal b32
AreEqual(asset_slot *Thing1, asset_slot *Thing2)
{
  b32 Result = MemoryIsEqual((u8*)Thing1, (u8*)Thing2, sizeof( asset_slot ) );


  return Result;
}

