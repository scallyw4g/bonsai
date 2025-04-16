// src/engine/render_settings.h:34:0

link_internal b32
AreEqual(lighting_settings *Thing1, lighting_settings *Thing2)
{
  if (Thing1 && Thing2)
  {
    b32 Result = MemoryIsEqual((u8*)Thing1, (u8*)Thing2, sizeof( lighting_settings ) );

    return Result;
  }
  else
  {
    return (Thing1 == Thing2);
  }
}

link_internal b32
AreEqual(lighting_settings Thing1, lighting_settings Thing2)
{
  b32 Result = MemoryIsEqual((u8*)&Thing1, (u8*)&Thing2, sizeof( lighting_settings ) );

  return Result;
}

