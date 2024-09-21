// src/engine/terrain.cpp:168:0

// NOTE(Jesse): This must hold true for using any Noise_16x func
Assert(Chunk->Dim % V3i(16) == V3i(0));

Period = Max(Period, V3(1.f));

auto PrimeX = U32_8X(501125321);
auto PrimeY = U32_8X(1136930381);
auto PrimeZ = U32_8X(1720413743);

u64 CycleCountStart = GetCycleCount();

Assert(u64(NoiseValues) % 32 == 0);

// TODO(Jesse): Make this dynamic
Assert(Octaves < 8);
perlin_params zParams[8];
perlin_params yParams[8];
perlin_params xParams[16];

for ( s32 z = 0; z < Dim.z; ++ z)
{
  f32 zCoord = COMPUTE_NOISE_INPUT(z, 0, Chunk);
  f32 WorldZBiased = zCoord - zMin;

  v3 InteriorPeriod = Period;
  RangeIterator_t(u32, OctaveIndex, Octaves)
  {
    zParams[OctaveIndex] = ComputePerlinParameters(F32_8X(zCoord/InteriorPeriod.z), PrimeZ);
    InteriorPeriod = Max(V3(1.f), InteriorPeriod/2.f);
  }

  for ( s32 y = 0; y < Dim.y; ++ y)
  {
    f32 yCoord = COMPUTE_NOISE_INPUT(y, 0, Chunk);

    InteriorPeriod = Period;
    RangeIterator_t(u32, OctaveIndex, Octaves)
    {
      yParams[OctaveIndex] = ComputePerlinParameters(F32_8X(yCoord/InteriorPeriod.y), PrimeY);
      InteriorPeriod = Max(V3(1.f), InteriorPeriod/2.f);
    }

    for ( s32 x = 0; x < Dim.x; x += 16 )
    {
      s32 VoxIndex = GetIndex(Voxel_Position(x,y,z), Dim);
      
      u16 PackedHSVColorValue = RGBtoPackedHSV(RGBColor);

      r32 InteriorAmp = r32(Amplitude);
      f32 xCoords[16];
      u32 ParamsIndex = 0;
      InteriorPeriod = Period;
      for (u32 OctaveIndex = 0; OctaveIndex < Octaves; ++OctaveIndex)
      {
        RangeIterator(ValueIndex, 16)
        {
          xCoords[ValueIndex] = (COMPUTE_NOISE_INPUT(x, ValueIndex, Chunk)) / InteriorPeriod.x;
        }
        auto Index = 0;
        auto _x0 = F32_8X( xCoords[0], xCoords[1], xCoords[2], xCoords[3], xCoords[4], xCoords[5], xCoords[6], xCoords[7] );
        auto _x1 = F32_8X( xCoords[8], xCoords[9], xCoords[10], xCoords[11], xCoords[12], xCoords[13], xCoords[14], xCoords[15] );
        /* auto _x0 = F32_8X( xCoords[7], xCoords[6], xCoords[5], xCoords[4], xCoords[3], xCoords[2], xCoords[1],  xCoords[0] ); */
        /* auto _x1 = F32_8X( xCoords[15], xCoords[14], xCoords[13], xCoords[12], xCoords[11], xCoords[10], xCoords[9], xCoords[8] ); */
        xParams[ParamsIndex++] = ComputePerlinParameters(_x0, PrimeX);
        xParams[ParamsIndex++] = ComputePerlinParameters(_x1, PrimeX);
        InteriorPeriod = Max(V3(1.f), InteriorPeriod/2.f);
      }

      for (u32 OctaveIndex = 0; OctaveIndex < Octaves; ++OctaveIndex)
      {
        PerlinNoise_16x_avx2(xParams+(OctaveIndex*2), yParams+OctaveIndex, zParams+OctaveIndex, NoiseValues+VoxIndex, InteriorAmp);
        InteriorAmp = Max(1.f, InteriorAmp/2.f);
      }

    

      u16 OccupancyByte = 0;
      RangeIterator(ValueIndex, 16)
      {
        s32 ThisIndex = VoxIndex+ValueIndex;
        s32 NoiseChoice = NoiseValues[ThisIndex] > WorldZBiased;
        ChunkSum += u32(NoiseChoice);

        OccupancyByte |= (NoiseChoice << ValueIndex);
        Chunk->Voxels[ThisIndex].Color = PackedHSVColorValue*u16(NoiseChoice);
      }
      SetOccupancyByte(Chunk, VoxIndex, u8(OccupancyByte));
      SetOccupancyByte(Chunk, VoxIndex+8, u8(OccupancyByte>>8));
    }
  }
}

u64 CycleCountEnd = GetCycleCount();
u64 CyclesElapsed = CycleCountEnd-CycleCountStart;

engine_debug *ED = GetEngineDebug();
ED->ChunkGenCyclesElapsed += CyclesElapsed;
ED->CellsGenerated += u64(Volume(Chunk->Dim))*u64(Octaves);
