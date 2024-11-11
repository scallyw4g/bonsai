// src/engine/render_settings.h:94:0

link_internal b32
AreEqual(render_settings *Thing1, render_settings *Thing2)
{
  if (Thing1 && Thing2)
  {
    b32 Result = MemoryIsEqual((u8*)Thing1, (u8*)Thing2, sizeof( render_settings ) );

    return Result;
  }
  else
  {
    return (Thing1 == Thing2);
  }
}

link_internal b32
AreEqual(render_settings Thing1, render_settings Thing2)
{
  b32 Result = MemoryIsEqual((u8*)&Thing1, (u8*)&Thing2, sizeof( render_settings ) );

  return Result;
}

