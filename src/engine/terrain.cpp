
link_internal u32
HoodooTerrain( perlin_noise *Noise,
               world_chunk *Chunk,
               chunk_dimension Dim,
               chunk_dimension SrcToDest,
               u16 ColorIndex,
               s32 Frequency,
               s32 Amplitude,
               s64 zMin,
               chunk_dimension WorldChunkDim,
               void *BaseNoise )
{
  TIMED_FUNCTION();

  u32 ChunkSum = 0;

  s32 MinZ = Chunk->WorldP.z*WorldChunkDim.z;
  s32 MaxZ = MinZ+WorldChunkDim.z ;

  octave_buffer *OctaveBuf = (octave_buffer*)BaseNoise;
  u32 OctaveCount = OctaveBuf->Count;

  for ( s32 z = 0; z < Dim.z; ++ z)
  {
    s64 WorldZ = z - SrcToDest.z + (WorldChunkDim.z*Chunk->WorldP.z);
    s64 WorldZBiased = WorldZ - zMin;
    for ( s32 y = 0; y < Dim.y; ++ y)
    {
      s64 WorldY = y - SrcToDest.y + (WorldChunkDim.y*Chunk->WorldP.y);
      for ( s32 x = 0; x < Dim.x; ++ x)
      {
        s64 WorldX = x - SrcToDest.x + (WorldChunkDim.x*Chunk->WorldP.x);
        r32 NoiseValue = 0.f;
        s32 VoxIndex = GetIndex(Voxel_Position(x,y,z), Dim);
        Chunk->Voxels[VoxIndex].Flags = Voxel_Empty;
        Assert( NotSet(&Chunk->Voxels[VoxIndex], Voxel_Filled) );

        s32 Amp2 = Amplitude*2;
        r32 HighestNoise = 0.f;
        for (u32 OctaveIndex = 0; OctaveIndex < OctaveCount; ++OctaveIndex)
        {
          octave *Octave = OctaveBuf->Octaves+OctaveIndex;

          v3 OctaveFrequency = Octave->Freq;
          r32 OctaveAmplitude = Octave->Amp;

          /* r32 WarpFactor = 3.f; */

          f32 InX = SafeDivide0((x + SrcToDest.x + ( WorldChunkDim.x*Chunk->WorldP.x)), OctaveFrequency.x);
          f32 InY = SafeDivide0((y + SrcToDest.y + ( WorldChunkDim.y*Chunk->WorldP.y)), OctaveFrequency.y);
          f32 InZ = SafeDivide0((z + SrcToDest.z + ( WorldChunkDim.z*Chunk->WorldP.z)), OctaveFrequency.z);
          /* f32 InZ = 1.0; */


          r32 Warp = PerlinNoise(InX, InY, InZ);
          v3 WarpFactor = Octave->WarpStrength*Warp;
          /* Warp = PerlinNoise(InX, InY, InZ) * r32(1.f/(OctaveIndex+1)); */
          /* Warp = PerlinNoise(InX, InY, InZ); */

          /* r32 N = PerlinNoise(InX, InY, InZ); */
          /* r32 N = PerlinNoise(InX, InY+Warp, InZ); */
          r32 N = PerlinNoise(InX+WarpFactor.x, InY+WarpFactor.y, InZ+WarpFactor.z);
          /* r32 N = PerlinNoise(Warp, Warp, Warp); */

          /* r32 N2 = PerlinNoise(InX, InY, InZ); */

          Assert(N <= 1.05f);
          Assert(N > -1.05f);

          /* Assert(N2 <= 1.05f); */
          /* Assert(N2 > -1.05f); */

          /* b32 NoiseChoice = IsUnderground && NoiseValue > 0.20f; */
          /* s32 zValue = (s32)Abs( (N*OctaveAmplitude) ); */
          /* s32 zValue = (s32)Abs( (N*OctaveAmplitude) + (N2*Amp2) ); */
          /* s32 zValue =  s32(N*OctaveAmplitude) + s32(N2*Amplitude*Amplitude); */
          /* s32 zValue = s32(N*OctaveAmplitude); */

          r32 NoiseContrib = (N*OctaveAmplitude) / 1+OctaveIndex;
          NoiseValue += NoiseContrib;

          HighestNoise = Max(HighestNoise, N);

          /* if (OctaveIndex == 1) */

          /* b32 IsUnderground =  zValue < WorldZBiased; */
          /* b32 NoiseChoice = IsUnderground && N > 0.5f; */
          /* b32 NoiseChoice = IsUnderground; */

          /* if (NoiseChoice) */
          {
            /* NoiseValue += (N*OctaveAmplitude) / OctaveCount; */
            /* NoiseValue += (N*OctaveAmplitude) / 1+OctaveIndex; */
          }

          /* OctaveAmplitude = Max(1, OctaveAmplitude/2); */
          /* OctaveFrequency = Max(1, OctaveFrequency/2); */
        }

        {
           octave HoodooOctaves[2] = {
             {V3(15, 15, 300), 60, 0.5f, V3(1.f)},
             {V3(50, 50, 3),   18, 0.5f, V3(1.f)},
           };


           RangeIterator(HoodooOctaveIndex, (s32)ArrayCount(HoodooOctaves))
           {
             octave HoodooOctave = HoodooOctaves[HoodooOctaveIndex];
             f32 OctaveX = SafeDivide0((x + SrcToDest.x + ( WorldChunkDim.x*Chunk->WorldP.x)), HoodooOctave.Freq.x);
             f32 OctaveY = SafeDivide0((y + SrcToDest.y + ( WorldChunkDim.y*Chunk->WorldP.y)), HoodooOctave.Freq.y);
             f32 OctaveZ = SafeDivide0((z + SrcToDest.z + ( WorldChunkDim.z*Chunk->WorldP.z)), HoodooOctave.Freq.z);

             r32 N2 = PerlinNoise(OctaveX, OctaveY, OctaveZ);

             /* NoiseValue += N2*HoodooOctave.Amp * ((Sin(InX)+Cos(InY))/2.f); */
             /* NoiseValue += N2*HoodooOctave.Amp * Pow((Sin(InX) + Cos(InY))/20, 3); */

             // Pretty good hoodoos
             /* NoiseValue += Pow(ClampMin(N2 * Sin(InX)+Cos(InY)-0.95f, 0.f), 3) * HoodooOctave.Amp; */

             // Better Hoodoos
             /* NoiseValue += Pow(ClampMin(N2 * Sin(InX*17)+Cos(InY*17)-0.9f, 0.f), 4) * HoodooOctave.Amp; */
             /* NoiseValue += Pow(ClampMin(N2 * ((Sin(InX*17)+Cos(InY*3))-0.9f), 0.f), 4) * HoodooOctave.Amp; */

             // Hoodoos along ridges
             /* NoiseValue += Pow(ClampMin(N2 * N, 0.f), 4) * HoodooOctave.Amp; */

             NoiseValue += Pow(ClampMin(N2*HighestNoise, 0.f), 4) * HoodooOctave.Amp;
           }
        }



        /* NoiseValue /= r32(Octaves+Octaves); */
        /* Assert(NoiseValue <= 1.10f); */

        /* b32 NoiseChoice = NoiseValue > 0.5f;; */
        /* b32 NoiseChoice = NoiseValue > r32(Amplitude); //0.5f;; */
        b32 NoiseChoice = r64(NoiseValue) > r64(WorldZBiased);

        u16 ThisColor = ColorIndex;

        s32 SnowThreshold = 100;
        if (NoiseChoice == True && WorldZ > SnowThreshold)
        {
          ThisColor = WHITE;
        }

        s32 SandThreshold = 3;
        if (NoiseChoice == True && WorldZ < SandThreshold)
        {
          ThisColor = YELLOW;
        }

        /* s32 WaterThreshold = 0; */
        /* if (NoiseChoice == False && WorldZ < WaterThreshold) */
        /* { */
        /*   NoiseChoice = True; */
        /*   ThisColor = BLUE; */
        /* } */

        SetFlag(&Chunk->Voxels[VoxIndex], (voxel_flag)(Voxel_Filled*NoiseChoice));
        Chunk->Voxels[VoxIndex].Color = ThisColor*u16(NoiseChoice);
        ChunkSum += NoiseChoice;

        Assert( (Chunk->Voxels[VoxIndex].Flags&VoxelFaceMask) == 0);

        if (NoiseChoice)
        {
          Assert( IsSet(&Chunk->Voxels[VoxIndex], Voxel_Filled) );
        }
        else
        {
          Assert( NotSet(&Chunk->Voxels[VoxIndex], Voxel_Filled) );
        }
      }
    }
  }

  return ChunkSum;
}

link_internal u32
GrassyTerrain( perlin_noise *Noise,
               world_chunk *Chunk,
               chunk_dimension Dim,
               chunk_dimension SrcToDest,
               u16 ColorIndex,

               s32 IgnoredFrequency,
               s32 IgnoredAmplitude,

               s64 zMin,
               chunk_dimension WorldChunkDim,
               void *OctavesIn )
{
  TIMED_FUNCTION();
  u32 ChunkSum = 0;

  s32 MinZ = Chunk->WorldP.z*WorldChunkDim.z;
  s32 MaxZ = MinZ+WorldChunkDim.z ;

  octave_buffer *OctaveBuf = (octave_buffer*)OctavesIn;
  u32 OctaveCount = OctaveBuf->Count;

  for ( s32 z = 0; z < Dim.z; ++ z)
  {
    s64 WorldZ = z - SrcToDest.z + (WorldChunkDim.z*Chunk->WorldP.z);
    s64 WorldZBiased = WorldZ - zMin;
    for ( s32 y = 0; y < Dim.y; ++ y)
    {
      s64 WorldY = y - SrcToDest.y + (WorldChunkDim.y*Chunk->WorldP.y);
      for ( s32 x = 0; x < Dim.x; ++ x)
      {
        s64 WorldX = x - SrcToDest.x + (WorldChunkDim.x*Chunk->WorldP.x);
        s32 VoxIndex = GetIndex(Voxel_Position(x,y,z), Dim);
        Chunk->Voxels[VoxIndex].Flags = Voxel_Empty;
        Assert( NotSet(&Chunk->Voxels[VoxIndex], Voxel_Filled) );

        r32 NoiseValue = 0.f;
        for (u32 OctaveIndex = 0; OctaveIndex < OctaveCount; ++OctaveIndex)
        {
          octave *Octave = OctaveBuf->Octaves+OctaveIndex;

          f32 InX = SafeDivide0((x + SrcToDest.x + (WorldChunkDim.x*Chunk->WorldP.x)), Octave->Freq.x);
          f32 InY = SafeDivide0((y + SrcToDest.y + (WorldChunkDim.y*Chunk->WorldP.y)), Octave->Freq.y);
          f32 InZ = SafeDivide0((z + SrcToDest.z + (WorldChunkDim.z*Chunk->WorldP.z)), Octave->Freq.z);

          r32 Warp = PerlinNoise(InX, InY, InZ);
          v3 WarpFactor = Warp*Octave->WarpStrength;

          r32 N = PerlinNoise(InX+WarpFactor.x, InY+WarpFactor.y, InZ+WarpFactor.z);

          Assert(N <= 1.05f);
          Assert(N > -1.05f);

          NoiseValue += (N*Octave->Amp)*Octave->Strength;
        }

        b32 NoiseChoice = r64(NoiseValue) > r64(WorldZBiased);

        u16 StartColorMin = GREY_4;
        u16 StartColorMax = GREY_6;
        u16 ThisColor = SafeTruncateToU16(umm(RandomBetween(StartColorMin, &GenColorEntropy, StartColorMax)));

        u8 ThisTransparency = 0;

        s32 SnowThreshold = 100;
        if (NoiseChoice == True && WorldZ > SnowThreshold)
        {
          ThisColor = WHITE;
        }

        s32 SandThreshold = 3;
        if (NoiseChoice == True && WorldZ < SandThreshold)
        {
          ThisColor = SAND;
        }

        s32 GravelThreshold = 1;
        if (NoiseChoice == True && WorldZ < GravelThreshold)
        {
          ThisColor = GRAVEL;
        }

        s32 WaterThreshold = 0;
        if (NoiseChoice == False && WorldZ < WaterThreshold)
        {
          NoiseChoice = True;
          ThisColor = BLUE;
          ThisTransparency = 255;
        }

        s32 Below = TryGetIndex(x, y, z-1, Dim);
        s32 B0 = TryGetIndex(x+1, y, z-1, Dim);
        s32 B1 = TryGetIndex(x-1, y, z-1, Dim);
        s32 B2 = TryGetIndex(x, y+1, z-1, Dim);
        s32 B3 = TryGetIndex(x, y-1, z-1, Dim);

        u32 CornersFilled = 0;
        if (B0 > -1) { CornersFilled += (Chunk->Voxels[B0].Flags & Voxel_Filled); }
        if (B1 > -1) { CornersFilled += (Chunk->Voxels[B1].Flags & Voxel_Filled); }
        if (B2 > -1) { CornersFilled += (Chunk->Voxels[B2].Flags & Voxel_Filled); }
        if (B3 > -1) { CornersFilled += (Chunk->Voxels[B3].Flags & Voxel_Filled); }

        if (NoiseChoice == False)
        {
          /* s32 Above = TryGetIndex(x, y, z+1, Dim); */
          if (Below > -1)
          {
            if ( CornersFilled > 3 &&
                 Chunk->Voxels[Below].Flags & Voxel_Filled &&
                 Chunk->Voxels[Below].Color >= StartColorMin && Chunk->Voxels[Below].Color <= StartColorMax )
            {
              NoiseChoice = True;
              ThisColor = DIRT;
            }

            f32 InX = SafeDivide0((x + SrcToDest.x + ( WorldChunkDim.x*Chunk->WorldP.x)), 10.f);
            f32 InY = SafeDivide0((y + SrcToDest.y + ( WorldChunkDim.y*Chunk->WorldP.y)), 10.f);
            f32 InZ = SafeDivide0((z + SrcToDest.z + ( WorldChunkDim.z*Chunk->WorldP.z)), 10.f);

            u16 GrassColorMin = GRASS_GREEN;
            u16 GrassColorMax = GRASS_GREEN+2;

            r32 GrassChance = PerlinNoise(InX, InY, InZ);
            if (Chunk->Voxels[Below].Flags&Voxel_Filled &&
                Chunk->Voxels[Below].Color == DIRT)
            {
              /* ThisColor = SafeTruncateToU16(umm(RandomBetween(GrassColorMin, &GenColorEntropy, GrassColorMax))); */
              NoiseChoice = True;

              ThisColor = GrassColorMin;
              if (GrassChance > 0.45f) { ThisColor = GrassColorMin+1; }
              if (GrassChance > 0.88f) { ThisColor = GrassColorMin+2; }
            }

            if (Chunk->Voxels[Below].Flags&Voxel_Filled &&
                Chunk->Voxels[Below].Color >= GrassColorMin && 
                Chunk->Voxels[Below].Color <= GrassColorMax )
            {
              r32 GrassFilter = RandomUnilateral(&GenColorEntropy);
              GrassChance = GrassFilter > 0.5f? GrassChance : 0.f;
              if (GrassChance > 0.70f)
              {
                ThisColor = Chunk->Voxels[Below].Color;
                NoiseChoice = True;
              }
              if (GrassChance > 0.997f)
              {
                ThisColor = RED;
              }
              if (GrassChance > 0.998f)
              {
                ThisColor = YELLOW;
              }
              if (GrassChance > 0.999f)
              {
                ThisColor = PINK;
              }
            }
          }
        }
        else // NoiseChoice == True
        {
          if (Below > -1)
          {
            if (ThisColor == StartColorMin && (Chunk->Voxels[Below].Flags&Voxel_Filled)==0 )
            {
              r32 VineChance = RandomUnilateral(&GenColorEntropy);
              if (CornersFilled <= 2 && VineChance > 0.9f)
              {
                ThisColor = GRASS_GREEN;
                while (RandomUnilateral(&GenColorEntropy) > 0.95f)
                {
                }
              }
            }
          }
        }

        SetFlag(&Chunk->Voxels[VoxIndex], (voxel_flag)(Voxel_Filled*NoiseChoice));
        Chunk->Voxels[VoxIndex].Color = ThisColor*u8(NoiseChoice);
        Chunk->Voxels[VoxIndex].Transparency = ThisTransparency;
        ChunkSum += NoiseChoice;



        Assert( (Chunk->Voxels[VoxIndex].Flags&VoxelFaceMask) == 0);

        if (NoiseChoice)
        {
          Assert( IsSet(&Chunk->Voxels[VoxIndex], Voxel_Filled) );
        }
        else
        {
          Assert( NotSet(&Chunk->Voxels[VoxIndex], Voxel_Filled) );
        }
      }
    }
  }

  return ChunkSum;
}
