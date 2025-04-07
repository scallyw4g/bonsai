// src/engine/editor.h:963:0

link_internal b32
AreEqual(brush_settings *Thing1, brush_settings *Thing2)
{
  if (Thing1 && Thing2)
  {
        b32 Result = MemoryIsEqual((u8*)Thing1, (u8*)Thing2, sizeof( brush_settings ) );

    return Result;
  }
  else
  {
    return (Thing1 == Thing2);
  }
}

link_internal b32
AreEqual(brush_settings Thing1, brush_settings Thing2)
{
    b32 Result = MemoryIsEqual((u8*)&Thing1, (u8*)&Thing2, sizeof( brush_settings ) );

  return Result;
}

