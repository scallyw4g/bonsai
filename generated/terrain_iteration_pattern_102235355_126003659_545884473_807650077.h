// src/engine/terrain.cpp:112:0

// NOTE(Jesse): This must hold true for using any Noise_8x func
Assert(Chunk->Dim % V3i(8) == V3i(0));

Period = Max(Period, V3(1.f));

for ( s32 z = 0; z < Dim.z; ++ z)
{
  f32 zCoord = COMPUTE_NOISE_INPUT(z, 0, Chunk);
  f32 WorldZBiased = zCoord - zMin;
  for ( s32 y = 0; y < Dim.y; ++ y)
  {
    f32 yCoord = COMPUTE_NOISE_INPUT(y, 0, Chunk);
    for ( s32 x = 0; x < Dim.x; x += 8 )
    {
      s32 VoxIndex = GetIndex(Voxel_Position(x,y,z), Dim);

      
        u16 PackedHSVColorValue = RGBtoPackedHSV(RGBColor);

        {
          /* HISTOGRAM_FUNCTION(); */

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

            // best 102.7 million points/sec
            // best 26.15 cycles/cell
            /* PerlinNoise_8x_sse(xCoords, yIn, zIn, TmpPerlinResults); */

            // best 143 million points/sec
            // best 18.75 cycles/cell
            PerlinNoise_8x_avx2(xCoords, yIn, zIn, TmpPerlinResults);

            /* TmpPerlinResults[0] = f32(PackedHSVColorValue); */ 
            /* TmpPerlinResults[1] = f32(PackedHSVColorValue); */
            /* TmpPerlinResults[2] = f32(PackedHSVColorValue); */
            /* TmpPerlinResults[3] = f32(PackedHSVColorValue); */
            /* TmpPerlinResults[4] = f32(PackedHSVColorValue); */
            /* TmpPerlinResults[5] = f32(PackedHSVColorValue); */
            /* TmpPerlinResults[6] = f32(PackedHSVColorValue); */
            /* TmpPerlinResults[7] = f32(PackedHSVColorValue); */

            RangeIterator(ValueIndex, MIN_TERRAIN_NOISE_WIDTH)
            {
              NoiseValues[VoxIndex+ValueIndex] += TmpPerlinResults[ValueIndex]*InteriorAmp;
            }

            InteriorAmp = Max(1.f, InteriorAmp/2.f);
            InteriorPeriod = Max(V3(1.f), InteriorPeriod/2.f);
          }

        }
      

      u8 OccupancyByte = 0;
      RangeIterator(ValueIndex, 8)
      {
        s32 ThisIndex = VoxIndex+ValueIndex;
        s32 NoiseChoice = NoiseValues[ThisIndex] > WorldZBiased;
        ChunkSum += u32(NoiseChoice);

        OccupancyByte |= (NoiseChoice << ValueIndex);
        Chunk->Voxels[ThisIndex].Color = PackedHSVColorValue*u16(NoiseChoice);
      }

      SetOccupancyByte(Chunk, VoxIndex, OccupancyByte);
    }
  }
}

