
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
        v3 Derivs = V3(0);
        v3 TerrainNormal = V3(0);
        /* for (u32 OctaveIndex = 0; OctaveIndex < OctaveCount; ++OctaveIndex) */
        OctaveCount = 1;
        /* for (u32 OctaveIndex = 0; OctaveIndex < OctaveCount; ++OctaveIndex) */
        /* { */
          /* octave *Octave = OctaveBuf->Octaves+OctaveIndex; */

#if 0
          f32 InX = SafeDivide0((x + SrcToDest.x + (WorldChunkDim.x*Chunk->WorldP.x)), Octave->Freq.x);
          f32 InY = SafeDivide0((y + SrcToDest.y + (WorldChunkDim.y*Chunk->WorldP.y)), Octave->Freq.y);
          /* f32 InY = 1.f; */
          f32 InZ = SafeDivide0((z + SrcToDest.z + (WorldChunkDim.z*Chunk->WorldP.z)), Octave->Freq.z);
          /* f32 InZ = 1.f; */
#else
          /* f32 InX = f32(WorldX); */
          /* f32 InY = f32(WorldY); */
          /* f32 InZ = f32(WorldZ); */

          /* f32 InX = f32(r64(WorldX)/100.0); */
          /* f32 InY = f32(r64(WorldY)/100.0); */
          /* f32 InZ = f32(r64(WorldZ)/100.0); */

          f32 InX = f32(x/32.f + (WorldChunkDim.x*Chunk->WorldP.x)/32.f) / 10.f;
          f32 InY = f32(y/32.f + (WorldChunkDim.y*Chunk->WorldP.y)/32.f) / 10.f;
          f32 InZ = f32(z/32.f + (WorldChunkDim.z*Chunk->WorldP.z)/32.f) / 10.f;

          /* f32 InX = f32(x/32.f + (WorldChunkDim.x*Chunk->WorldP.x)/32.f); */
          /* f32 InY = f32(y/32.f + (WorldChunkDim.y*Chunk->WorldP.y)/32.f); */
          /* f32 InZ = f32(z/32.f + (WorldChunkDim.z*Chunk->WorldP.z)/32.f); */

          /* InX = 1.f; */
          /* InY = 1.f; */
          /* InZ = 1.f; */

/*           f32 InX = f32(x/32.f + (WorldChunkDim.x*Chunk->WorldP.x)/32.f)/2.f; */
/*           f32 InY = f32(y/32.f + (WorldChunkDim.y*Chunk->WorldP.y)/32.f)/2.f; */
/*           f32 InZ = f32(z/32.f + (WorldChunkDim.z*Chunk->WorldP.z)/32.f)/2.f; */
#endif

          /* r32 Warp = PerlinNoise(InX, InY, InZ); */
          /* v3 WarpFactor = Warp*Octave->WarpStrength; */
          v3 WarpFactor = {};

          /* r32 N = IQ_QuinticGradientNoise(V3(InX+WarpFactor.x, InZ+WarpFactor.z, InY+WarpFactor.y)); */
          /* r32 N = IQ_ValueNoise_AnalyticNormals(InX+WarpFactor.x, InZ+WarpFactor.z, InY+WarpFactor.y,  &Derivs); */
          /* r32 N = IQ_ValueNoise_AnalyticNormals(InX+WarpFactor.x, InZ+WarpFactor.z, InY+WarpFactor.y,  &Derivs); */
          /* r32 N = PerlinNoise_Derivitives0(InX+WarpFactor.x, InY+WarpFactor.y, InZ+WarpFactor.z, &Derivs); */
          r32 N = PerlinNoise_Derivitives1(InX+WarpFactor.x, InY+WarpFactor.y, InZ+WarpFactor.z, &Derivs);

          /* Assert(N <= 1.05f); */
          /* Assert(N > -1.05f); */

          /* NoiseValue += (N*Octave->Amp)*Octave->Strength; */
          NoiseValue = N;

          /* break; */
        /* } */

        Chunk->Voxels[VoxIndex].Derivs = Derivs;

        /* v3 Tangent, Bitangent, Normal; */
        /* CalculateTBN(Derivs, &Tangent, &Bitangent, &Normal); */
        /* v3 Normal = Derivs; */
        v3 Normal = CalcNormal(V3(InX, InY, InZ), NoiseValue, PerlinNoise_Derivitives1 );

        /* Chunk->Voxels[VoxIndex].DebugColor = Derivs; */
        Chunk->Voxels[VoxIndex].DebugColor = Normal;

        /* Info("%f", NoiseValue); */
        b32 NoiseChoice = r32(NoiseValue) > (-0.15f); //r64(WorldZBiased);

        u16 StartColorMin = GREY_4;
        u16 StartColorMax = GREY_6;
        /* u16 ThisColor = SafeTruncateToU16(umm(RandomBetween(StartColorMin, &GenColorEntropy, StartColorMax))); */
        u16 ThisColor = StartColorMin;

        u8 ThisTransparency = 0;

        s32 SnowThreshold   = 100;
        s32 SandThreshold   = 3;
        s32 GravelThreshold = 1;
        s32 WaterThreshold  = 0;

        if (NoiseChoice == True && WorldZ > SnowThreshold)
        {
          ThisColor = WHITE;
        }

        if (NoiseChoice == True && WorldZ < SandThreshold)
        {
          ThisColor = SAND;
        }

        if (NoiseChoice == True && WorldZ < GravelThreshold)
        {
          ThisColor = GRAVEL;
        }

        if (NoiseChoice == False && WorldZ < WaterThreshold)
        {
          /* NoiseChoice = True; */
          /* ThisColor = BLUE; */
          /* ThisTransparency = 255; */
        }

        /* if (NoiseChoice && ThisColor == StartColorMin) */
        if (NoiseChoice)
        {
          /* if (Dot(TerrainNormal, V3(0, 0, 1)) > 0.5f) */
          if (Dot(Normal, V3(0, 0, 1)) > 0.65f)
          {
            ThisColor = DIRT;
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
