// src/engine/editor.h:557:0

link_internal b32
AreEqual(perlin_noise_params *Thing1, perlin_noise_params *Thing2)
{
  b32 Result = MemoryIsEqual((u8*)Thing1, (u8*)Thing2, sizeof( perlin_noise_params ) );

  return Result;
}

link_internal b32
AreEqual(perlin_noise_params Thing1, perlin_noise_params Thing2)
{
  b32 Result = MemoryIsEqual((u8*)&Thing1, (u8*)&Thing2, sizeof( perlin_noise_params ) );

  return Result;
}

