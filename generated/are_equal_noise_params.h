// src/engine/editor.h:679:0

link_internal b32
AreEqual(noise_params *Thing1, noise_params *Thing2)
{
  b32 Result = MemoryIsEqual((u8*)Thing1, (u8*)Thing2, sizeof( noise_params ) );

  return Result;
}

link_internal b32
AreEqual(noise_params Thing1, noise_params Thing2)
{
  b32 Result = MemoryIsEqual((u8*)&Thing1, (u8*)&Thing2, sizeof( noise_params ) );

  return Result;
}

