// src/engine/terrain.cpp:168:0

// NOTE(Jesse): This must hold true for using any Noise_16x func
Assert(Chunk->Dim % V3i(16) == V3i(0));

Period = Max(Period, V3(1.f));

auto PrimeX = U32_8X(501125321);
auto PrimeY = U32_8X(1136930381);
auto PrimeZ = U32_8X(1720413743);

u64 CycleCountStart = GetCycleCount();

// TODO(Jesse): Make this dynamic
Assert(Octaves < 8);
perlin_params zParams[16];
perlin_params yParams[16];
perlin_params xParams[16];

for ( s32 z = 0; z < Dim.z; z += 1)
{
  f32 zCoord0 = COMPUTE_NOISE_INPUT(z, 0, Chunk);
  /* f32 zCoord1 = COMPUTE_NOISE_INPUT(z, 1, Chunk); */

  f32 WorldZBiased0 = zCoord0 - zMin;
  /* f32 WorldZBiased1 = zCoord1 - zMin; */

  u32 ParamsIndex = 0;
  v3 InteriorPeriod = Period;
  RangeIterator_t(u32, OctaveIndex, Octaves)
  {
    zParams[ParamsIndex++] = ComputePerlinParameters(F32_8X(zCoord0/InteriorPeriod.z), PrimeZ);
    /* zParams[ParamsIndex++] = ComputePerlinParameters(F32_8X(zCoord1/InteriorPeriod.z), PrimeZ); */
    InteriorPeriod = Max(V3(1.f), InteriorPeriod/2.f);
  }
  /* Assert(ParamsIndex==16); */

  for ( s32 y = 0; y < Dim.y; y += 1)
  {
    f32 yCoord0 = COMPUTE_NOISE_INPUT(y, 0, Chunk);
    /* f32 yCoord1 = COMPUTE_NOISE_INPUT(y, 1, Chunk); */

    ParamsIndex = 0;
    InteriorPeriod = Period;
    RangeIterator_t(u32, OctaveIndex, Octaves)
    {
      yParams[ParamsIndex++] = ComputePerlinParameters(F32_8X(yCoord0/InteriorPeriod.y), PrimeY);
      /* yParams[ParamsIndex++] = ComputePerlinParameters(F32_8X(yCoord1/InteriorPeriod.y), PrimeY); */
      InteriorPeriod = Max(V3(1.f), InteriorPeriod/2.f);
    }
    /* Assert(ParamsIndex==16); */

    for ( s32 x = 0; x < Dim.x; x += 16 )
    {
      v3i VoxP = V3i(x,y,z);
      s32 VoxIndex = GetIndex(VoxP, Dim);
      
      u16 PackedHSVColorValue = RGBtoPackedHSV(RGBColor);

      f32 xCoords[16];
      ParamsIndex = 0;
      InteriorPeriod = Period;
      for (u32 OctaveIndex = 0; OctaveIndex < Octaves; ++OctaveIndex)
      {
        RangeIterator(ValueIndex, 16)
        {
          xCoords[ValueIndex] = (COMPUTE_NOISE_INPUT(x, ValueIndex, Chunk));
        }
        auto Index = 0;
        auto xCoord0 = F32_8X( xCoords[0], xCoords[1], xCoords[2], xCoords[3], xCoords[4], xCoords[5], xCoords[6], xCoords[7] );
        auto xCoord1 = F32_8X( xCoords[8], xCoords[9], xCoords[10], xCoords[11], xCoords[12], xCoords[13], xCoords[14], xCoords[15] );
        xParams[ParamsIndex++] = ComputePerlinParameters(xCoord0/F32_8X(InteriorPeriod.x), PrimeX);
        xParams[ParamsIndex++] = ComputePerlinParameters(xCoord1/F32_8X(InteriorPeriod.x), PrimeX);
        InteriorPeriod = Max(V3(1.f), InteriorPeriod/2.f);
      }
      /* Assert(ParamsIndex==16); */

      r32 InteriorAmp = r32(Amplitude);
      for (u32 OctaveIndex = 0; OctaveIndex < Octaves; ++OctaveIndex)
      {
        f32 TmpPerlinResults[16*2*2];
        PerlinNoise_16x1x1_avx2(xParams+(OctaveIndex*2), yParams+(OctaveIndex*2), zParams, TmpPerlinResults);

        RangeIterator(zIndex, 1)
        RangeIterator(yIndex, 1)
        RangeIterator(xIndex, 16)
        {
          s32 TmpIndex = GetIndex(V3i(xIndex, yIndex, zIndex), V3i(16,1,1));
          s32 NoiseIndex = GetIndex(VoxP+V3i(xIndex, yIndex, zIndex), Dim);
          NoiseValues[NoiseIndex] += TmpPerlinResults[TmpIndex]*InteriorAmp;
        }

        InteriorAmp = Max(1.f, InteriorAmp/2.f);
      }


      RangeIterator(zIndex, 1)
      {
        /* r32 WorldZBiased = zIndex == 0 ? WorldZBiased0 : WorldZBiased1; */
        r32 WorldZBiased =  WorldZBiased0;
        RangeIterator(yIndex, 1)
        {
          RangeIterator(xIndex, 16)
          {
            s32 ThisIndex = GetIndex(VoxP+V3i(xIndex, yIndex, zIndex), Dim);
            s32 NoiseChoice = NoiseValues[ThisIndex] > WorldZBiased;
            ChunkSum += u32(NoiseChoice);

            SetOccupancyBit(Chunk, ThisIndex, NoiseChoice);
            Chunk->Voxels[ThisIndex].Color = PackedHSVColorValue*u16(NoiseChoice);
          }
        }
      }
    }
  }
}

u64 CycleCountEnd = GetCycleCount();
u64 CyclesElapsed = CycleCountEnd-CycleCountStart;

engine_debug *ED = GetEngineDebug();
ED->ChunkGenCyclesElapsed += CyclesElapsed;
ED->CellsGenerated += u64(Volume(Chunk->Dim)*s32(Octaves));
