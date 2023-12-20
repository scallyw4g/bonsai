
link_internal void
ComputeNormalsForChunk(world_chunk *Chunk, v3i Dim)
{
  TIMED_FUNCTION();
  NotImplemented;
#if 0

  for ( s32 z = 0; z < Dim.z; ++ z)
  {
    for ( s32 y = 0; y < Dim.y; ++ y)
    {
      for ( s32 x = 0; x < Dim.x; ++ x)
      {
        s32 VoxIndex = GetIndex(Voxel_Position(x,y,z), Dim);

        if ( Chunk->Voxels[VoxIndex].Flags & Voxel_Filled )
        {
          v3 Normal = {};
          for ( s32 dz = -1; dz < 2; ++ dz)
          {
            for ( s32 dy = -1; dy < 2; ++ dy)
            {
              for ( s32 dx = -1; dx < 2; ++ dx)
              {
                if (dz == 0 && dy == 0 && dx == 0) continue; // Skip the middle-most voxel

                s32 dP = TryGetIndex(Voxel_Position(x+dx,y+dy,z+dz), Dim);
                if (dP > -1)
                {
                  if ( Chunk->Voxels[dP].Flags & Voxel_Filled )
                  {
                    /* Normal += Normalize(V3(dx,dy,dz)); */
                    Normal += V3(dx,dy,dz);
                  }
                }
              }
            }
          }
          Chunk->Voxels[VoxIndex].DebugColor = Normalize(Normal);
        }
      }
    }
  }

  for ( s32 z = 0; z < Dim.z; ++ z)
  {
    for ( s32 y = 0; y < Dim.y; ++ y)
    {
      for ( s32 x = 0; x < Dim.x; ++ x)
      {
        s32 VoxIndex = GetIndex(Voxel_Position(x,y,z), Dim);

        if ( Chunk->Voxels[VoxIndex].Flags & Voxel_Filled )
        {
          v3 Normal = Chunk->Voxels[VoxIndex].DebugColor;
          for ( s32 dz = -1; dz < 2; ++ dz)
          {
            for ( s32 dy = -1; dy < 2; ++ dy)
            {
              for ( s32 dx = -1; dx < 2; ++ dx)
              {
                if (dz == 0 && dy == 0 && dx == 0) continue; // Skip the middle-most voxel

                s32 dP = TryGetIndex(Voxel_Position(x+dx,y+dy,z+dz), Dim);
                if (dP > -1)
                {
                  Normal += 0.05f*Chunk->Voxels[dP].DebugColor;
                }
              }
            }
          }
          Chunk->Voxels[VoxIndex].DebugColor = Normalize(Normal);
        }

      }
    }
  }
#endif
}

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
    s64 WorldZ = z + SrcToDest.z + (WorldChunkDim.z*Chunk->WorldP.z);
    s64 WorldZSubZMin = WorldZ - zMin;
    for ( s32 y = 0; y < Dim.y; ++ y)
    {
      s64 WorldY = y + SrcToDest.y + (WorldChunkDim.y*Chunk->WorldP.y);
      for ( s32 x = 0; x < Dim.x; ++ x)
      {
        s64 WorldX = x + SrcToDest.x + (WorldChunkDim.x*Chunk->WorldP.x);
        r32 NoiseValue = 0.f;
        s32 VoxIndex = GetIndex(Voxel_Position(x,y,z), Dim);
        Chunk->Voxels[VoxIndex].Flags = Voxel_Empty;
        Assert( NotSet(&Chunk->Voxels[VoxIndex], Voxel_Filled) );

        /* s32 Amp2 = Amplitude*2; */
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

          /* b32 IsFilled = IsUnderground && NoiseValue > 0.20f; */
          /* s32 zValue = (s32)Abs( (N*OctaveAmplitude) ); */
          /* s32 zValue = (s32)Abs( (N*OctaveAmplitude) + (N2*Amp2) ); */
          /* s32 zValue =  s32(N*OctaveAmplitude) + s32(N2*Amplitude*Amplitude); */
          /* s32 zValue = s32(N*OctaveAmplitude); */

          r32 NoiseContrib = (N*OctaveAmplitude) / 1+OctaveIndex;
          NoiseValue += NoiseContrib;

          HighestNoise = Max(HighestNoise, N);

          /* if (OctaveIndex == 1) */

          /* b32 IsUnderground =  zValue < WorldZSubZMin; */
          /* b32 IsFilled = IsUnderground && N > 0.5f; */
          /* b32 IsFilled = IsUnderground; */

          /* if (IsFilled) */
          {
            /* NoiseValue += (N*OctaveAmplitude) / OctaveCount; */
            /* NoiseValue += (N*OctaveAmplitude) / 1+OctaveIndex; */
          }

          /* OctaveAmplitude = Max(1, OctaveAmplitude/2); */
          /* OctaveFrequency = Max(1, OctaveFrequency/2); */
        }

        {
           octave HoodooOctaves[2] = {
             {V3(15, 15, 300), 30, V3(1.f)},
             {V3(50, 50, 3),   9, V3(1.f)},
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

        /* b32 IsFilled = NoiseValue > 0.5f;; */
        /* b32 IsFilled = NoiseValue > r32(Amplitude); //0.5f;; */
        b32 IsFilled = r64(NoiseValue) > r64(WorldZSubZMin);

        u16 ThisColor = ColorIndex;

        s32 SnowThreshold = 100;
        if (IsFilled == True && WorldZ > SnowThreshold)
        {
          ThisColor = WHITE;
        }

        s32 SandThreshold = 3;
        if (IsFilled == True && WorldZ < SandThreshold)
        {
          ThisColor = YELLOW;
        }

        /* s32 WaterThreshold = 0; */
        /* if (IsFilled == False && WorldZ < WaterThreshold) */
        /* { */
        /*   IsFilled = True; */
        /*   ThisColor = BLUE; */
        /* } */

        SetFlag(&Chunk->Voxels[VoxIndex], (voxel_flag)(Voxel_Filled*IsFilled));
        Chunk->Voxels[VoxIndex].Color = ThisColor*u16(IsFilled);
        ChunkSum += IsFilled;

        Assert( (Chunk->Voxels[VoxIndex].Flags&VoxelFaceMask) == 0);

        if (IsFilled)
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
TerracedTerrain( perlin_noise *Noise,
                 world_chunk *Chunk,
                 v3i Dim,
                 v3i SrcToDest,
                 u16 ColorIndex,

                 s32 IgnoredFrequency,
                 s32 IgnoredAmplitude,

                 s64 zMin,
                 v3i WorldChunkDim,
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
    s64 WorldZ = z + SrcToDest.z + (WorldChunkDim.z*Chunk->WorldP.z);
    s64 WorldZSubZMin = WorldZ - zMin;
    /* s64 WorldZSubZMin = zMin; */
    for ( s32 y = 0; y < Dim.y; ++ y)
    {
      /* s64 WorldY = y + SrcToDest.y + (WorldChunkDim.y*Chunk->WorldP.y); */
      for ( s32 x = 0; x < Dim.x; ++ x)
      {
        /* s64 WorldX = x + SrcToDest.x + (WorldChunkDim.x*Chunk->WorldP.x); */
        s32 VoxIndex = GetIndex(Voxel_Position(x,y,z), Dim);
        Chunk->Voxels[VoxIndex].Flags = Voxel_Empty;
        /* Assert( NotSet(&Chunk->Voxels[VoxIndex], Voxel_Filled) ); */

        r32 NoiseValue = 0.f;
        r32 BaseNoiseValue = 0.f;
        /* v3 Normal = V3(0); */
        v3 Derivs = V3(0);
        for (u32 OctaveIndex = 0; OctaveIndex < OctaveCount; ++OctaveIndex)
        {
          octave *Octave = OctaveBuf->Octaves+OctaveIndex;
#if 1

          f32 InX = (x + SrcToDest.x + (WorldChunkDim.x*Chunk->WorldP.x)) / Octave->Freq.x;
          f32 InY = (y + SrcToDest.y + (WorldChunkDim.y*Chunk->WorldP.y)) / Octave->Freq.y;
          f32 InZ = (z + SrcToDest.z + (WorldChunkDim.z*Chunk->WorldP.z)) / Octave->Freq.z;

/* #define MAP_WIDTH 200000000 */
/* #define MAP_WIDTH 200000 */
/*           f32 InX = ((x + SrcToDest.x + (WorldChunkDim.x*Chunk->WorldP.x)) * Octave->Freq.x) / MAP_WIDTH; */
/*           f32 InY = ((y + SrcToDest.y + (WorldChunkDim.y*Chunk->WorldP.y)) * Octave->Freq.y) / MAP_WIDTH; */
/*           f32 InZ = ((z + SrcToDest.z + (WorldChunkDim.z*Chunk->WorldP.z)) * Octave->Freq.z) / MAP_WIDTH; */

          /* f32 InX = SafeDivide0((x + SrcToDest.x + (WorldChunkDim.x*Chunk->WorldP.x)), Octave->Freq.x); */
          /* f32 InY = SafeDivide0((y + SrcToDest.y + (WorldChunkDim.y*Chunk->WorldP.y)), Octave->Freq.y); */
          /* f32 InZ = SafeDivide0((z + SrcToDest.z + (WorldChunkDim.z*Chunk->WorldP.z)), Octave->Freq.z); */
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
          /* r32 N = PerlinNoise_Derivitives1(InX+WarpFactor.x, InY+WarpFactor.y, InZ+WarpFactor.z, &Derivs); */
          r32 N = PerlinNoise(InX+WarpFactor.x, InY+WarpFactor.y, InZ+WarpFactor.z);

          /* Assert(N <= 1.05f); */
          /* Assert(N > -1.05f); */

          BaseNoiseValue += (N*Octave->Amp);
          /* BaseNoiseValue = N; */

          /* Normal += CalcNormal(V3(InX, InY, InZ), N, PerlinNoise ); */
        }

#if 1
        {
          octave TerraceOctaves[] =
          {
            {V3(250, 250, 25), 15.f, V3(2.f)},
          };


          /* RangeIterator(HoodooOctaveIndex, (s32)ArrayCount(TerraceOctaves)) */
          {
            r32 Mask = 0.f;
            {
              octave O = TerraceOctaves[0];
              f32 OctaveX = (x + SrcToDest.x + ( WorldChunkDim.x*Chunk->WorldP.x)) / O.Freq.x;
              f32 OctaveY = (y + SrcToDest.y + ( WorldChunkDim.y*Chunk->WorldP.y)) / O.Freq.y;
              f32 OctaveZ = (z + SrcToDest.z + ( WorldChunkDim.z*Chunk->WorldP.z)) / O.Freq.z;
              /* f32 OctaveZ = 1.f; */
              Mask = PerlinNoise(OctaveX, OctaveY, OctaveZ) * O.Amp;
            }

            /* NoiseValue = Pow(BaseNoiseValue, Mask); */
            /* NoiseValue = BaseNoiseValue * Mask; */
            NoiseValue = BaseNoiseValue / Mask;
          }
        }
#endif

        /* Normal /= OctaveCount; */

        /* Chunk->Voxels[VoxIndex].Derivs = Derivs; */

        /* v3 Tangent, Bitangent, Normal; */
        /* CalculateTBN(Derivs, &Tangent, &Bitangent, &Normal); */

        /* Chunk->Voxels[VoxIndex].DebugColor = Derivs; */
        /* Chunk->Voxels[VoxIndex].DebugColor = Normalize(Normal); */

        /* Info("%f", r64(NoiseValue)); */
        /* b32 IsFilled = r32(NoiseValue) > 100.f; */
        /* b32 IsFilled = r32(NoiseValue) > 0.5f; */
        /* b32 IsFilled = r32(NoiseValue) > 0.f; */
        b32 IsFilled = r32(NoiseValue) > r32(WorldZSubZMin) ;

        /* u16 StartColorMin = GREY_4; */
        /* u16 StartColorMax = GREY_6; */
        /* u16 STONE = GREY_5; */
        /* u16 ThisColor = SafeTruncateToU16(umm(RandomBetween(StartColorMin, &GenColorEntropy, StartColorMax))); */
        u16 ThisColor = DIRT;

        u8 ThisTransparency = 0;

        s32 SandThreshold   = 3;
        s32 GravelThreshold = 1;
        s32 WaterThreshold  = 0;

        r32 StoneThresh = r32(WorldZSubZMin) + 2.f;
        r32 DirtThresh = r32(WorldZSubZMin) + 1.f;

        if (IsFilled)
        {
          ThisColor = GRASS_GREEN;

          /* if (NoiseValue > DirtThresh) */
          /* { */
          /*   ThisColor = DIRT; */
          /* } */

          if (NoiseValue > StoneThresh)
          {
            ThisColor = DIRT;
          }


          /* if (WorldZ < SandThreshold) */
          /* { */
          /*   ThisColor = SAND; */
          /* } */

          /* if (WorldZ < GravelThreshold) */
          /* { */
          /*   ThisColor = GRAVEL; */
          /* } */
        }

        if (IsFilled == False && WorldZ < WaterThreshold)
        {
          /* IsFilled = True; */
          /* ThisColor = BLUE; */
          /* ThisTransparency = 255; */
        }

        /* if (IsFilled && ThisColor == StartColorMin) */
#if 0
        if (IsFilled)
        {
          /* if (Dot(TerrainNormal, V3(0, 0, 1)) > 0.5f) */
          if (Dot(Normal, V3(0, 0, 1)) > 0.65f)
          {
            ThisColor = DIRT;
          }
        }
#endif

        SetFlag(&Chunk->Voxels[VoxIndex], (voxel_flag)(Voxel_Filled*IsFilled));
        Chunk->Voxels[VoxIndex].Color = ThisColor*u8(IsFilled);
        Chunk->Voxels[VoxIndex].Transparency = ThisTransparency;
        ChunkSum += IsFilled;



        Assert( (Chunk->Voxels[VoxIndex].Flags&VoxelFaceMask) == 0);

        if (IsFilled)
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

#if 0
  ComputeNormalsForChunk(Chunk, Dim);

  for ( s32 z = 0; z < Dim.z; ++ z)
  {
    for ( s32 y = 0; y < Dim.y; ++ y)
    {
      for ( s32 x = 0; x < Dim.x; ++ x)
      {
        s32 VoxIndex = GetIndex(x, y, z, Dim);
        voxel *V = Chunk->Voxels+VoxIndex;
        if (V->Flags & Voxel_Filled)
        {
          v3 Normal = V->DebugColor * -1.f;
          /* V->DebugColor = V3(Dot(Normal, V3(0, 0, 1)), 0.f, 0.f); */

          if (V->Color == DIRT &&
              Dot(Normal, V3(0, 0, 1)) > 0.7f)
          {
            V->Color = GRASS_GREEN;
          }
        }
      }
    }
  }
#endif

  return ChunkSum;
}

