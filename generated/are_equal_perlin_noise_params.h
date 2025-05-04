// src/engine/editor.h:734:0

link_internal b32
AreEqual(perlin_noise_params *Thing1, perlin_noise_params *Thing2)
{
  if (Thing1 && Thing2)
  {
        b32 Result = MemoryIsEqual((u8*)Thing1, (u8*)Thing2, sizeof( perlin_noise_params ) );

    return Result;
  }
  else
  {
    return (Thing1 == Thing2);
  }
}

link_internal b32
AreEqual(perlin_noise_params Thing1, perlin_noise_params Thing2)
{
    b32 Result = MemoryIsEqual((u8*)&Thing1, (u8*)&Thing2, sizeof( perlin_noise_params ) );

  return Result;
}

