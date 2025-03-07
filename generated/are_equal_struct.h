// src/engine/editor.h:1130:0

link_internal b32
AreEqual(world_edit_brush *Thing1, world_edit_brush *Thing2)
{
  if (Thing1 && Thing2)
  {
        b32 Result = MemoryIsEqual((u8*)Thing1, (u8*)Thing2, sizeof( world_edit_brush ) );

    return Result;
  }
  else
  {
    return (Thing1 == Thing2);
  }
}

link_internal b32
AreEqual(world_edit_brush Thing1, world_edit_brush Thing2)
{
    b32 Result = MemoryIsEqual((u8*)&Thing1, (u8*)&Thing2, sizeof( world_edit_brush ) );

  return Result;
}

