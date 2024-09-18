// src/engine/terrain.cpp:109:0

// NOTE(Jesse): This must hold true for using any Noise_8x func
Assert(Chunk->Dim % V3i(MIN_TERRAIN_NOISE_WIDTH) == V3i(0));

Period = Max(Period, V3(1.f));


/* s32 xChunkMax = Dim.x/MIN_TERRAIN_NOISE_WIDTH; */

for ( s32 z = 0; z < Dim.z; ++ z)
{
  f32 zCoord = COMPUTE_NOISE_INPUT(z, 0, Chunk);
  f32 WorldZBiased = zCoord - zMin;
  for ( s32 y = 0; y < Dim.y; ++ y)
  {
    f32 yCoord = COMPUTE_NOISE_INPUT(y, 0, Chunk);
    for ( s32 x = 0; x < Dim.x; x += MIN_TERRAIN_NOISE_WIDTH )
    {
      s32 VoxIndex = GetIndex(Voxel_Position(x,y,z), Dim);
      /* Chunk->Voxels[VoxIndex].Flags = Voxel_Empty; */

      
        u16 PackedHSVColorValue = RGBtoPackedHSV(RGBColor);

        {
          HISTOGRAM_FUNCTION();

          v3 InteriorPeriod = Period;
          r32 InteriorAmp = r32(Amplitude);
          f32 xCoords[MIN_TERRAIN_NOISE_WIDTH];
          for (u32 OctaveIndex = 0; OctaveIndex < Octaves; ++OctaveIndex)
          {
            Assert(Chunk->DimInChunks > V3i(0));

            RangeIterator(ValueIndex, MIN_TERRAIN_NOISE_WIDTH)
            {
              xCoords[ValueIndex] = (COMPUTE_NOISE_INPUT(x, ValueIndex, Chunk)) / InteriorPeriod.x;
            }

            f32 yIn = yCoord/InteriorPeriod.y;
            f32 zIn = zCoord/InteriorPeriod.z;

            // NOTE(Jesse): Important to use Tmp here so we don't stomp on the result already in NoiseValues
            f32 TmpPerlinResults[MIN_TERRAIN_NOISE_WIDTH];
            PerlinNoise_8x(xCoords, yIn, zIn, TmpPerlinResults);

            RangeIterator(ValueIndex, MIN_TERRAIN_NOISE_WIDTH)
            {
              NoiseValues[VoxIndex+ValueIndex] += TmpPerlinResults[ValueIndex]*InteriorAmp;
            }

            InteriorAmp = Max(1.f, InteriorAmp/2.f);
            InteriorPeriod = Max(V3(1.f), InteriorPeriod/2.f);
          }

        }
      

      u8 OccupancyByte = 0;
      RangeIterator(ValueIndex, MIN_TERRAIN_NOISE_WIDTH)
      {
        s32 ThisIndex = VoxIndex+ValueIndex;
        s32 NoiseChoice = NoiseValues[ThisIndex] > WorldZBiased;
        ChunkSum += u32(NoiseChoice);

        OccupancyByte |= (NoiseChoice << ValueIndex);
        Chunk->Voxels[ThisIndex].Color = PackedHSVColorValue*u16(NoiseChoice);
      }

      SetOccupancyByte(Chunk, VoxIndex, OccupancyByte);
      /* s32 ByteIndex = VoxIndex/8; */
      /* Chunk->Occupancy[ByteIndex] = OccupancyByte; */
    }
  }
}

