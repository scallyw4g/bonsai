link_internal u32
Terrain_Flat( world_chunk *Chunk,
                      v3i  NoiseBasis,
                     void *NoiseParams,
                     void *UserData )
{
  UNPACK_NOISE_PARAMS(NoiseParams);

  u32 Result = 0;
  for ( s32 z = 0; z < Chunk->Dim.z; ++ z)
  {
    /* s64 WorldZ = s64(z - zMin + NoiseBasis.z + (GetWorldChunkDim().z*Chunk->WorldP.z)); */
    for ( s32 y = 0; y < Chunk->Dim.y; ++ y)
    {
      for ( s32 x = 0; x < Chunk->Dim.x; ++ x)
      {
        if ((z + NoiseBasis.z) < Thresh)
        {
          s32 Index = GetIndex(Voxel_Position(x,y,z), Chunk->Dim);
          SetOccupancyBit(Chunk, Index, VoxelOccupancy_Filled);
          /* Chunk->Voxels[Index].Flags = Voxel_Filled; */
          Chunk->Voxels[Index].Color = RGBtoPackedHSV(RGBColor);
          ++Result;
        }
      }
    }
  }

  return Result;;
}


global_variable random_series DEBUG_ENTROPY = {653765435432};

// 27.0m cycles | f32 ComputePerlinParameters 
//
// 30.0m cycles | u32 ComputePerlinParameters 
// 28.7m cycles | u32 ComputePerlinParameters 
// 27.8m cycles | u32 ComputePerlinParameters 
link_internal u32
Terrain_FBM2D( world_chunk *Chunk,
                       v3i  NoiseBasis,
                      void *NoiseParams,
                      void *OctaveCount )
{
  TIMED_FUNCTION();
  /* HISTOGRAM_FUNCTION(); */

  UNPACK_NOISE_PARAMS(NoiseParams);

  u32 ChunkSum = 0;

  random_series GenColorEntropy = {12653763234231};

  Period = Max(Period, V3(1.f));

  auto PrimeX = U32_8X(501125321);
  auto PrimeY = 1136930381u;
  auto PrimeZ = 1720413743u;

  u16 PackedHSVColorValue = RGBtoPackedHSV(RGBColor);


  s32 Octaves = *(s32*)OctaveCount;

#if 0
  s32 MinZ = Chunk->WorldP.z*WorldChunkDim.z;
  s32 MaxZ = MinZ+WorldChunkDim.z ;

  if (MaxZ < -Amplitude)
  {
    s32 MaxIndex = Volume(Dim);
    for ( s32 VoxIndex = 0; VoxIndex < MaxIndex; ++VoxIndex)
    {
      Chunk->Voxels[VoxIndex].Flags = Voxel_Filled;
      Chunk->Voxels[VoxIndex].Color = RGBtoPackedHSV(RGBColor);
    }
    return (u32)MaxIndex;
  }

  if (MinZ > Amplitude)
    return ChunkSum;
#endif

#if VOXEL_DEBUG_COLOR
  memory_arena *TempArena = GetThreadLocalState(ThreadLocal_ThreadIndex)->PermMemory;
#else
  memory_arena *TempArena = GetTranArena();
#endif

  s32 NoiseUpsampleFactor = 1;

  /* v3i NoiseDim = RoundUp(Chunk->Dim/2, V3i(8)); */
  /* v3i NoiseDim = RoundUp((Chunk->Dim+2)/NoiseUpsampleFactor, V3i(8, 1, 1)); */
  v3i NoiseDim = RoundUp((Chunk->Dim+V3i(2,0,0))/NoiseUpsampleFactor, V3i(8));
  /* v3i NoiseDim = Chunk->Dim; */
  // NOTE(Jesse): This must hold true for using any Noise_16x func
  Assert(NoiseDim.x % 8 == 0);


  /* v3i NoiseDim = Chunk->Dim + 2; */
  s32 NoiseValuesCount = Volume(NoiseDim);
  r32 *NoiseValues = Allocate(r32, TempArena, NoiseValuesCount);
  // NOTE(Jesse): Must be true to use _mm256_store_ps, which we do in the 16x version of perlin
  Assert(u64(NoiseValues) % 32 == 0);

  {
    /* v3i NormalDim = Chunk->Dim; */
    /* v3  *Normals     = Allocate( v3, TempArena, Volume(NormalDim)); */

#if VOXEL_DEBUG_COLOR
    Chunk->NoiseValues = NoiseValues;
    Chunk->NormalValues = Normals;
#endif

    u32_8x xChunkResolution = U32_8X(u32(Chunk->DimInChunks.x));
       u32 yChunkResolution = u32(Chunk->DimInChunks.y);
       u32 zChunkResolution = u32(Chunk->DimInChunks.z);

            u32 *zPeriods = Allocate(u32, TempArena, Octaves);
            u32 *yPeriods = Allocate(u32, TempArena, Octaves);
    avx_divisor *xPeriods = Allocate(avx_divisor, TempArena, Octaves);

    perlin_params *xParams = AllocateAligned(perlin_params, TempArena, NoiseDim.x*Octaves*2, 32);
    perlin_params *yParams = AllocateAligned(perlin_params, TempArena, NoiseDim.y*Octaves*2, 32);
    perlin_params *zParams = AllocateAligned(perlin_params, TempArena, NoiseDim.z*Octaves*2, 32);

    /* u32 Offset = 200'000'000; */
    /* v3i WorldBasis = V3i(Offset, Offset, 0) + NoiseBasis + (Chunk->WorldP*GetWorldChunkDim()); */
    v3i WorldBasis = NoiseBasis + (Chunk->WorldP*GetWorldChunkDim());



    v3i InteriorPeriod = V3i(Period);
    RangeIterator(OctaveIndex, Octaves)
    {
      zPeriods[OctaveIndex] = u32(InteriorPeriod.z);
      yPeriods[OctaveIndex] = u32(InteriorPeriod.y);
      xPeriods[OctaveIndex] = AvxDivisor(u32(InteriorPeriod.x));
      InteriorPeriod = Max(V3i(1), InteriorPeriod/2);
    }

    u64 StartingCycles = __rdtsc();
    {
      TIMED_NAMED_BLOCK(precalculate_parameters);

      for ( s32 zNoise = 0; zNoise < NoiseDim.z; ++ zNoise)
      {
        RangeIterator(OctaveIndex, Octaves)
        {
          s32 i = OctaveIndex+(zNoise*Octaves);
          zParams[i] = ComputePerlinParameters_scalar(u32(WorldBasis.z), u32(zNoise), zChunkResolution, zPeriods[OctaveIndex], PrimeZ);
        }
      }

      for ( s32 yNoise = 0; yNoise < NoiseDim.y; ++ yNoise)
      {
        RangeIterator(OctaveIndex, Octaves)
        {
          s32 i = OctaveIndex+(yNoise*Octaves);
          yParams[i] = ComputePerlinParameters_scalar(u32(WorldBasis.y), u32(yNoise), yChunkResolution, yPeriods[OctaveIndex], PrimeY);
        }
      }

      u32 *_xCoords = AllocateAligned(u32, TempArena, NoiseDim.x, 32);
      for ( s32 xNoise = 0; xNoise < NoiseDim.x; ++ xNoise )
      {
        _xCoords[xNoise] = u32(xNoise-1);
      }

      for ( s32 xNoise = 0; xNoise < NoiseDim.x; xNoise += 8 )
      {
        auto _x = U32_8X(_xCoords+xNoise);
        RangeIterator(OctaveIndex, Octaves)
        {
          s32 i = OctaveIndex+(xNoise*Octaves);
          xParams[i] = ComputePerlinParameters_vector(U32_8X(WorldBasis.x), _x, xChunkResolution, xPeriods[OctaveIndex], PrimeX);
        }
      }
    }

    {
      TIMED_NAMED_BLOCK(octaves);
      for ( s32 zNoise = 0; zNoise < NoiseDim.z; ++ zNoise)
      {
        for ( s32 yNoise = 0; yNoise < NoiseDim.y; ++ yNoise)
        {
          for ( s32 xNoise = 0; xNoise < NoiseDim.x; xNoise += 8 )
          {
            s32 NoiseIndex = GetIndex(xNoise, yNoise, zNoise, NoiseDim);

            r32 InteriorAmp = r32(Amplitude);
            RangeIterator(OctaveIndex, Octaves)
            {
              auto zParam = zParams+OctaveIndex+(zNoise*Octaves);
              auto yParam = yParams+OctaveIndex+(yNoise*Octaves);
              auto xParam = xParams+OctaveIndex+(xNoise*Octaves);
              PerlinNoise_8x_avx2(xParam, yParam, zParam, NoiseValues+NoiseIndex, InteriorAmp);
              InteriorAmp = Max(1.f, InteriorAmp/2.f);
            }
          }
        }
      }
    }

    u64 EndingCycles = __rdtsc();
    u64 ElapsedCycles = EndingCycles - StartingCycles;
    GetEngineDebug()->ChunkGenCyclesElapsed += ElapsedCycles;
    GetEngineDebug()->CellsGenerated += u64(Volume(NoiseDim))*u64(Octaves);

#define __COMPUTE_NOISE_INPUT(channel, basis_chunk_point, offset, chunk_dim_in_chunks)  \
      f32(basis_chunk_point.channel)                              \
    + f32(offset)*Chunk->DimInChunks.channel                                            \
    + SrcToDest.channel                                                                 \

    {
      TIMED_NAMED_BLOCK(finalize);
      for ( s32 zChunk = 0; zChunk < Chunk->Dim.z; ++ zChunk)
      {
        f32 zCoord = __COMPUTE_NOISE_INPUT(z, WorldBasis, (zChunk)*NoiseUpsampleFactor, Chunk->DimInChunks);
        /* f32 zCoord = 75.f; */
        f32 WorldZBiased = zCoord - zMin;
        for ( s32 yChunk = 0; yChunk < Chunk->Dim.y; ++ yChunk)
        {
          for ( s32 xChunk = 0; xChunk < Chunk->Dim.x; ++ xChunk)
          {
            v3i ChunkP = V3i(xChunk, yChunk, zChunk);
            /* v3i NoiseP = V3i(xChunk, yChunk, zChunk); */
            /* v3i NoiseP = V3i(xChunk+1, yChunk+1, zChunk+1)/NoiseUpsampleFactor; */
            v3i NoiseP = V3i(xChunk+1, yChunk, zChunk)/NoiseUpsampleFactor;

            s32 ChunkIndex = GetIndex(ChunkP, Chunk->Dim);
            s32 NoiseIndex = GetIndex(NoiseP, NoiseDim);

            r32 ThisNoiseV = NoiseValues[NoiseIndex];
            s32 NoiseChoice = ThisNoiseV > WorldZBiased;
            ChunkSum += u32(NoiseChoice);
            SetOccupancyBit(Chunk, ChunkIndex, NoiseChoice);

            Chunk->Voxels[ChunkIndex].Color = PackedHSVColorValue*u16(NoiseChoice);
            /* Chunk->Voxels[ChunkIndex].Color = u16(RandomU32(&DEBUG_ENTROPY)); */
            if (xChunk == 0) { Chunk->Voxels[ChunkIndex].Color = PackHSVColor(HSV_RED)*u16(NoiseChoice); }
            /* if (xChunk == 1) { Chunk->Voxels[ChunkIndex].Color = PackHSVColor(HSV_YELLOW)*u16(NoiseChoice); } */
            /* if (xChunk == Chunk->Dim.x-1) { Chunk->Voxels[ChunkIndex].Color = PackHSVColor(HSV_PINK)*u16(NoiseChoice); } */
            if (yChunk == 1) { Chunk->Voxels[ChunkIndex].Color = PackHSVColor(HSV_GREEN)*u16(NoiseChoice); }
            if (zChunk == 1) { Chunk->Voxels[ChunkIndex].Color = PackHSVColor(HSV_BLUE)*u16(NoiseChoice); }
          }
        }
      }

      Assert(NoiseDim.x >= 66);
      Assert(NoiseDim.y >= 66);
      Assert(NoiseDim.z >= 66);

      Assert(Chunk->Dim.x == 64);
      Assert(Chunk->Dim.y == 66);
      Assert(Chunk->Dim.z == 66);
      for ( s32 zNoise = 1; zNoise < 65; ++ zNoise)
      {
        /* f32 zCoord = 75.f; */
        f32 zCoord = __COMPUTE_NOISE_INPUT(z, WorldBasis, (zNoise)*NoiseUpsampleFactor, Chunk->DimInChunks);
        f32 WorldZBiased = zCoord - zMin;
        for ( s32 yNoise = 1; yNoise < 65; ++ yNoise)
        {

          {
            v3i BorderP = V3i(0, yNoise, zNoise)/NoiseUpsampleFactor;
            s32 BorderIndex = GetIndex(BorderP, NoiseDim);

            r32 ThisNoiseV = NoiseValues[BorderIndex];

            u64 NoiseChoice = ThisNoiseV > WorldZBiased;
            /* u64 NoiseChoice = 1ull; */
            /* u64 NoiseChoice = RandomU32(&DEBUG_ENTROPY) & 1; */
            Chunk->xOccupancyBorder[(zNoise-1)*2] |= NoiseChoice << (yNoise-1);
          }

          {
            v3i BorderP = V3i(65, yNoise, zNoise)/NoiseUpsampleFactor;
            s32 BorderIndex = GetIndex(BorderP, NoiseDim);

            r32 ThisNoiseV = NoiseValues[BorderIndex];

            u64 NoiseChoice = ThisNoiseV > WorldZBiased;
            /* u64 NoiseChoice = 1ull; */
            /* u64 NoiseChoice = RandomU32(&DEBUG_ENTROPY) & 1; */
            Chunk->xOccupancyBorder[((zNoise-1)*2)+1] |= NoiseChoice << (yNoise-1);
          }

        }
      }
    }
  }

  return ChunkSum;
}

link_internal u32
Terrain_Perlin2D( world_chunk *Chunk,
                          v3i  NoiseBasis,
                         void *NoiseParams,
                         void *UserData )
{
  TIMED_FUNCTION();
  u32 SyntheticChunkSum = Terrain_FBM2D( Chunk,  NoiseBasis, NoiseParams, UserData );
  return SyntheticChunkSum;
}

link_internal u32
Terrain_Perlin3D( world_chunk *Chunk,
                          v3i  NoiseBasis,
                         void *NoiseParams,
                         void *UserData )
{
  TIMED_FUNCTION();

  UNPACK_NOISE_PARAMS(NoiseParams);

  u32 Result = 0;

  Assert(Chunk);

  for ( s32 z = 0; z < Dim.z; ++ z)
  {
    for ( s32 y = 0; y < Dim.y; ++ y)
    {
      for ( s32 x = 0; x < Dim.x; ++ x)
      {
        s32 i = GetIndex(Voxel_Position(x,y,z), Dim);
        NotImplemented;
        /* Chunk->Voxels[i].Flags = Voxel_Empty; */

        /* Assert( NotSet(&Chunk->Voxels[i], Voxel_Filled) ); */

        v3 NoiseInput = MapWorldPositionToNoiseInputValue(V3(NoiseBasis), V3(x,y,z), Period);
        r32 NoiseValue = PerlinNoise(NoiseInput);

        s32 NoiseChoice = NoiseValue*Amplitude > Thresh;
        Assert(NoiseChoice == 0 || NoiseChoice == 1);


        SetOccupancyBit(Chunk, i, NoiseChoice);
        /* SetFlag(&Chunk->Voxels[i], (voxel_flag)(NoiseChoice * Voxel_Filled)); */

        if (NoiseChoice)
        {
          Chunk->Voxels[i].Color = RGBtoPackedHSV(RGBColor);
          /* Assert( IsSet(&Chunk->Voxels[i], Voxel_Filled) ); */
          ++Result;
        }
        else
        {
          /* Assert( NotSet(&Chunk->Voxels[i], Voxel_Filled) ); */
        }

      }
    }
  }

  return Result;
}

link_internal u32
Terrain_WhiteNoise( world_chunk *Chunk,
                            v3i  NoiseBasis,
                           void *NoiseParams,
                           void *UserData )
{
  TIMED_FUNCTION();

  UNPACK_NOISE_PARAMS(NoiseParams);

  u32 Result = 0;

  Assert(Chunk);

  for ( s32 z = 0; z < Dim.z; ++ z)
  {
    for ( s32 y = 0; y < Dim.y; ++ y)
    {
      for ( s32 x = 0; x < Dim.x; ++ x)
      {
        s32 i = GetIndex(Voxel_Position(x,y,z), Dim);
        NotImplemented;
        /* Chunk->Voxels[i].Flags = Voxel_Empty; */

        /* Assert( NotSet(&Chunk->Voxels[i], Voxel_Filled) ); */

        v3 NoiseInput = MapWorldPositionToNoiseInputValue(V3(NoiseBasis), V3(x,y,z), Period);
        random_series Entropy = RandomSeriesFromV3(NoiseInput);

        r32 NoiseValue = RandomUnilateral(&Entropy);

        s32 NoiseChoice = NoiseValue > Thresh;
        Assert(NoiseChoice == 0 || NoiseChoice == 1);

        SetOccupancyBit(Chunk, i, NoiseChoice);
        /* SetFlag(&Chunk->Voxels[i], (voxel_flag)(NoiseChoice * Voxel_Filled)); */

        if (NoiseChoice)
        {
          Chunk->Voxels[i].Color = RGBtoPackedHSV(RGBColor);
          /* Assert( IsSet(&Chunk->Voxels[i], Voxel_Filled) ); */
          ++Result;
        }
        else
        {
          /* Assert( NotSet(&Chunk->Voxels[i], Voxel_Filled) ); */
        }

      }
    }
  }

  return Result;
}

link_internal void
GrowGrassPerlin( world_chunk *Chunk, v3i P, r32 NoiseValue, r32 MaskValue, v3i SrcToDest, v3i WorldChunkDim, r32 WorldZSubZMin, v3 *ThisColor, b32 *IsFilled )
{
  s32 x = P.x;
  s32 y = P.y;
  s32 z = P.z;

  // NOTE(Jesse): This is pretty henious .. the math to compute GrassAreaX here
  // (and, actually, maybe most of these calculations to turn positions into floats)
  // is generating a fucked up index for the voxels along the exterior edge.
  //
  // The thing that's wrong is that the 0th still gets the Chunk->WorldP position,
  // when it should get the previous chunks worldp.  Similar story with the max-edge
  //
  // I think this was actually also manifesting when doing mip-meshing, but I
  // wasn't aware of this tom-fuckery then, and didn't catch it.
  //
  // TODO(Jesse): Should go audit the terrain-gen functions and find a better
  // way of computing these float values.
  //
  if (x == 0 || y == 0 || x == Chunk->Dim.x-1 || y == Chunk->Dim.y-1) { return; }

  f32 GrassAreaX = (x + SrcToDest.x + (WorldChunkDim.x*Chunk->WorldP.x)) / 16.f;
  f32 GrassAreaY = (y + SrcToDest.y + (WorldChunkDim.y*Chunk->WorldP.y)) / 16.f;
  /* f32 GrassAreaX = (x + SrcToDest.x + (WorldChunkDim.x*Chunk->WorldP.x)); */
  /* f32 GrassAreaY = (y + SrcToDest.y + (WorldChunkDim.y*Chunk->WorldP.y)); */

  // TODO(Jesse): Does this actually help reduce stuff 'growing' in places
  // it shouldn't?
  /* f32 GrassAreaZ = 1.f; //(z + SrcToDest.z + (WorldChunkDim.z*Chunk->WorldP.z)) / 32.f; */
  f32 GrassAreaZ = (z + SrcToDest.z + (WorldChunkDim.z*Chunk->WorldP.z)) / 32.f;

  r32 GrassyAreaValue = MaskValue * PerlinNoise(GrassAreaX, GrassAreaY, GrassAreaZ);
  /* r32 GrassyAreaValue = MaskValue * (VoronoiNoise3D(V3(GrassAreaX, GrassAreaY, GrassAreaZ)/5.f)*2.f); */
  if (*ThisColor == RGB_GRASS_GREEN)
  {
    if (GrassyAreaValue > 0.4f)
    {
      *ThisColor = RGBLighten(RGB_GRASS_GREEN);

      if (GrassyAreaValue > 0.6f)
      {
        if ((NoiseValue+(GrassyAreaValue*3.f)) > WorldZSubZMin)
        {
          f32 HashX = hash_f32(GrassAreaX);
          f32 HashY = hash_f32(GrassAreaY);

          s32 iHashX = *(s32*)&HashX;
          s32 iHashY = *(s32*)&HashY;

          random_series S0 { u64(x + y) | u64( (iHashX | (iHashY << 31)) ^ (iHashY << 16)) };
          random_series S1 { RandomU32(&S0) | RandomU32(&S0)<<31 };

          if (RandomUnilateral(&S1) > 0.92f)
          {
            *IsFilled = True;

            r32 GrassColor = RandomUnilateral(&S1);
            if (GrassColor > 0.8f)
              *ThisColor = RGB_LONG_GREEN_GRASS0;

            if (GrassColor > 0.88f)
              *ThisColor = RGB_LONG_GREEN_GRASS1;

            if (GrassColor > 0.993f)
              *ThisColor = RGB_LONG_YELLOW_GRASS0;

            if (GrassColor > 0.995f)
              *ThisColor = RGB_LONG_YELLOW_GRASS1;

            if (GrassColor > 0.998f)
              *ThisColor = RGB_PINK;
          }
        }
      }
    }
  }
}

link_internal void
GrowGrassPerlin_8x( world_chunk *Chunk, v3i P, r32 *NoiseValue, v3 *Normal, r32 MaskValue, v3i SrcToDest, v3i WorldChunkDim, r32 WorldZSubZMin, v3 *ThisColor )
{
  RangeIterator(Index, 8)
  {
    s32 x = P.x + Index;
    s32 y = P.y;
    s32 z = P.z;

    // NOTE(Jesse): This is pretty henious .. the math to compute GrassAreaX here
    // (and, actually, maybe most of these calculations to turn positions into floats)
    // is generating a fucked up index for the voxels along the exterior edge.
    //
    // The thing that's wrong is that the 0th still gets the Chunk->WorldP position,
    // when it should get the previous chunks worldp.  Similar story with the max-edge
    //
    // I think this was actually also manifesting when doing mip-meshing, but I
    // wasn't aware of this tom-fuckery then, and didn't catch it.
    //
    // TODO(Jesse): Should go audit the terrain-gen functions and find a better
    // way of computing these float values.
    //
    if (x == 0 || y == 0 || x == Chunk->Dim.x-1 || y == Chunk->Dim.y-1) { return; }

    f32 GrassAreaX = (x + SrcToDest.x + (WorldChunkDim.x*Chunk->WorldP.x)) / 16.f;
    f32 GrassAreaY = (y + SrcToDest.y + (WorldChunkDim.y*Chunk->WorldP.y)) / 16.f;
    /* f32 GrassAreaX = (x + SrcToDest.x + (WorldChunkDim.x*Chunk->WorldP.x)); */
    /* f32 GrassAreaY = (y + SrcToDest.y + (WorldChunkDim.y*Chunk->WorldP.y)); */

    // TODO(Jesse): Does this actually help reduce stuff 'growing' in places
    // it shouldn't?
    /* f32 GrassAreaZ = 1.f; //(z + SrcToDest.z + (WorldChunkDim.z*Chunk->WorldP.z)) / 32.f; */
    f32 GrassAreaZ = (z + SrcToDest.z + (WorldChunkDim.z*Chunk->WorldP.z)) / 32.f;

    r32 GrassyAreaValue = MaskValue * PerlinNoise(GrassAreaX, GrassAreaY, GrassAreaZ);
    /* r32 GrassyAreaValue = MaskValue * (VoronoiNoise3D(V3(GrassAreaX, GrassAreaY, GrassAreaZ)/5.f)*2.f); */
    if (ThisColor[Index] == RGB_GRASS_GREEN)
    {
      if (GrassyAreaValue > 0.4f)
      {
        ThisColor[Index] = RGB_GRASS_GREEN-1;

        if (GrassyAreaValue > 0.6f)
        {
          if ((NoiseValue[Index]+(GrassyAreaValue*3.f)) > WorldZSubZMin)
          {
            f32 HashX = hash_f32(GrassAreaX);
            f32 HashY = hash_f32(GrassAreaY);

            s32 iHashX = *(s32*)&HashX;
            s32 iHashY = *(s32*)&HashY;

            random_series S0 { u64(x + y) | u64( (iHashX | (iHashY << 31)) ^ (iHashY << 16)) };
            random_series S1 { RandomU32(&S0) | RandomU32(&S0)<<31 };

            if (RandomUnilateral(&S1) > 0.92f)
            {
              NoiseValue[Index] += 1.f;

              r32 GrassColor = RandomUnilateral(&S1);
              if (GrassColor > 0.8f)
                ThisColor[Index] = RGB_LONG_GREEN_GRASS0;

              if (GrassColor > 0.88f)
                ThisColor[Index] = RGB_LONG_GREEN_GRASS1;

              if (GrassColor > 0.993f)
                ThisColor[Index] = RGB_LONG_YELLOW_GRASS0;

              if (GrassColor > 0.995f)
                ThisColor[Index] = RGB_LONG_YELLOW_GRASS1;

              if (GrassColor > 0.998f)
                ThisColor[Index] = RGB_PINK;
            }
          }
        }
      }
    }
  }
}

link_internal void
RandomVoronoiRocks_8x( world_chunk *Chunk, v3i P, r32 *NoiseValue, v3 *Normals, v3i SrcToDest, v3i WorldChunkDim, r32 WorldZSubZMin, v3 *ThisColor )
{
  /* s32 GrassMaskCount = 0; */
  r32 GrassMasks[8];
  r32 _xGrassMapped[8];
  RangeIterator(Index, 8)
  {
    r32 DotNormal = Dot(Normals[Index], V3(0,0,1));
    r32 Thresh = 0.85f;
    r32 ClampedDotNormal = Clamp(Thresh, DotNormal, 1.f);
    GrassMasks[Index] = 1.f;// MapValueToUnilateral(Thresh, ClampedDotNormal, 1.f);
    /* if (GrassMasks[Index] > 0.f) ++GrassMaskCount; */

    _xGrassMapped[Index] = (Index + P.x + SrcToDest.x + (WorldChunkDim.x*Chunk->WorldP.x)) / 64.f;
  }

  /* if (GrassMaskCount == 0) return; */

  f32 yMapped = (P.y + SrcToDest.y + (WorldChunkDim.y*Chunk->WorldP.y)) / 64.f;
  f32 zMapped = (P.z + SrcToDest.z + (WorldChunkDim.z*Chunk->WorldP.z)) / 64.f;

  f32 VoronoiResults[8];
  VoronoiNoise3D_8x(VoronoiResults, _xGrassMapped, V2(yMapped, zMapped), 0.f, 0.1f);

  RangeIterator(Index, 8)
  {
    r32 GrassMask = GrassMasks[Index];

    if (GrassMask > 0.f)
    {
      /* f32 GrassAreaPower = 6.f; */
      f32 GrassAreaPower = 1.f;
      r32 MaskedVoronoi = GrassAreaPower * GrassMask * VoronoiResults[Index];
      if (ThisColor[Index] == RGB_GRASS_GREEN)
      {
        /* NoiseValue[Index] += 5.f; */
        if (MaskedVoronoi > 0.1f)
        {
          /* ThisColor[Index] = RED; */

          if (MaskedVoronoi > 0.2f)
          {
            f32 GrassHeightPower = 50.f;
            if ((NoiseValue[Index]+(MaskedVoronoi*GrassHeightPower)) > WorldZSubZMin)
            {
              ThisColor[Index] = RGB_STONE;
              /* NoiseValue[Index] = WorldZSubZMin + 10.f; */
              NoiseValue[Index] += 10.f;
            }
          }
        }
      }
    }
  }
}
link_internal void
GrowGrassVoronoi( world_chunk *Chunk, v3i P, r32 *NoiseValue, v3 *Normals, v3i SrcToDest, v3i WorldChunkDim, r32 WorldZSubZMin, v3 *ThisColor )
{
  s32 GrassMaskCount = 0;

  r32 GrassMasks[8];
  r32 _xGrassMapped[8];
  s32 Index= 0;
  /* RangeIterator(Index, 8) */
  {
    r32 DotNormal = Dot(Normals[Index], V3(0,0,1));
    r32 Thresh = 0.85f;
    r32 ClampedDotNormal = Clamp(Thresh, DotNormal, 1.f);
    GrassMasks[Index] = MapValueToUnilateral(Thresh, ClampedDotNormal, 1.f);
    if (GrassMasks[Index] > 0.f) ++GrassMaskCount;

    _xGrassMapped[Index] = (Index + P.x + SrcToDest.x + (WorldChunkDim.x*Chunk->WorldP.x)) / 16.f;
  }

  if (GrassMaskCount == 0) return;

  f32 yMapped = (P.y + SrcToDest.y + (WorldChunkDim.y*Chunk->WorldP.y)) / 16.f;
  f32 zMapped = (P.z + SrcToDest.z + (WorldChunkDim.z*Chunk->WorldP.z)) / 32.f;

  f32 VoronoiResults[8];
  VoronoiNoise3D_8x_Masked(VoronoiResults, -0.2f, GrassMasks, _xGrassMapped, V2(yMapped, zMapped));

  /* RangeIterator(Index, 8) */
  {
    r32 GrassMask = GrassMasks[Index];

    /* v3i Basis = V3i(P.x + Index + SrcToDest.x, P.y + SrcToDest.y, 0); */
    v3 Offset = V3(P) + SrcToDest + V3(Index,0,0);
    Offset.z = 0;
    v3i Basis = V3i(Canonical_Position(GetWorld()->ChunkDim, Offset, V3i(0)).Offset);
    random_series Entropy = RandomSeriesFromV3i(Basis);

    r32 MaskedVoronoi = GrassMask * VoronoiResults[Index];
    if (ThisColor[Index] == RGB_GRASS_GREEN)
    {
      /* NoiseValue[Index] += 5.f; */
      if (MaskedVoronoi > 0.1f)
      {
        /* ThisColor[Index] = RED; */

        if (MaskedVoronoi > 0.2f)
        {
          f32 GrassHeightPower = 50.f;
          if ((NoiseValue[Index]+(MaskedVoronoi*GrassHeightPower)) > WorldZSubZMin)
          {
            if (RandomUnilateral(&Entropy) > 0.7f)
            {
              ThisColor[Index] = RGB_GRASS_GREEN;
              if (MaskedVoronoi+RandomUnilateral(&Entropy) > 0.8f)
              {
                NoiseValue[Index] += MaskedVoronoi * 10.f;

                r32 GrassColor = RandomUnilateral(&Entropy);
                if (GrassColor > 0.8f)
                  ThisColor[Index] = RGB_LONG_GREEN_GRASS0;

                if (GrassColor > 0.88f)
                  ThisColor[Index] = RGB_LONG_GREEN_GRASS1;

                if (GrassColor > 0.993f)
                  ThisColor[Index] = RGB_LONG_YELLOW_GRASS0;

                if (GrassColor > 0.995f)
                  ThisColor[Index] = RGB_LONG_YELLOW_GRASS1;

                if (GrassColor > 0.998f)
                  ThisColor[Index] = RGB_PINK;
              }
            }
          }
        }
      }
    }
  }
}
link_internal void
GrowGrassVoronoi_8x( world_chunk *Chunk, v3i P, r32 *NoiseValue, v3 *Normals, v3i SrcToDest, v3i WorldChunkDim, r32 WorldZSubZMin, v3 *ThisColor )
{
  s32 GrassMaskCount = 0;

  r32 GrassMasks[8];
  r32 _xGrassMapped[8];
  RangeIterator(Index, 8)
  {
    r32 DotNormal = Dot(Normals[Index], V3(0,0,1));
    r32 Thresh = 0.85f;
    r32 ClampedDotNormal = Clamp(Thresh, DotNormal, 1.f);
    GrassMasks[Index] = MapValueToUnilateral(Thresh, ClampedDotNormal, 1.f);
    if (GrassMasks[Index] > 0.f) ++GrassMaskCount;

    _xGrassMapped[Index] = (Index + P.x + SrcToDest.x + (WorldChunkDim.x*Chunk->WorldP.x)) / 16.f;
  }

  if (GrassMaskCount == 0) return;

  f32 yMapped = (P.y + SrcToDest.y + (WorldChunkDim.y*Chunk->WorldP.y)) / 16.f;
  f32 zMapped = (P.z + SrcToDest.z + (WorldChunkDim.z*Chunk->WorldP.z)) / 32.f;

  f32 VoronoiResults[8];
  VoronoiNoise3D_8x_Masked(VoronoiResults, -0.2f, GrassMasks, _xGrassMapped, V2(yMapped, zMapped));

  RangeIterator(Index, 8)
  {
    r32 GrassMask = GrassMasks[Index];

    /* v3 Basis = V3(f32(P.x + Index + SrcToDest.x), f32(P.y + SrcToDest.y), 0.f); */
    /* v3i Basis = V3i(-1+P.x + Index + SrcToDest.x, P.y + SrcToDest.y, 0); */
    v3 Offset = V3(P) + SrcToDest + V3(Index,0,0);
    Offset.z = 0;
    v3i Basis = V3i(Canonical_Position(GetWorld()->ChunkDim, Offset, V3i(0)).Offset);
    /* v3i Basis = V3i(P.x + Index , P.y , 0); */
    random_series Entropy = RandomSeriesFromV3i(Basis);

    r32 MaskedVoronoi = GrassMask * VoronoiResults[Index];
    if (ThisColor[Index] == RGB_GRASS_GREEN)
    {
      /* NoiseValue[Index] += 5.f; */
      if (MaskedVoronoi > 0.1f)
      {
        /* ThisColor[Index] = RED; */

        if (MaskedVoronoi > 0.2f)
        {
          f32 GrassHeightPower = 50.f;
          if ((NoiseValue[Index]+(MaskedVoronoi*GrassHeightPower)) > WorldZSubZMin)
          {
            if (RandomUnilateral(&Entropy) > 0.7f)
            {
              ThisColor[Index] = RGBDesaturate(RGB_GRASS_GREEN);
              if (MaskedVoronoi+RandomUnilateral(&Entropy) > 0.8f)
              {
                NoiseValue[Index] += MaskedVoronoi * 10.f;

                r32 GrassColor = RandomUnilateral(&Entropy);
                if (GrassColor > 0.8f)
                  ThisColor[Index] = RGB_LONG_GREEN_GRASS0;

                if (GrassColor > 0.88f)
                  ThisColor[Index] = RGB_LONG_GREEN_GRASS1;

                if (GrassColor > 0.993f)
                  ThisColor[Index] = RGB_LONG_YELLOW_GRASS0;

                if (GrassColor > 0.995f)
                  ThisColor[Index] = RGB_LONG_YELLOW_GRASS1;

                if (GrassColor > 0.998f)
                  ThisColor[Index] = RGB_PINK;
              }
            }
          }
        }
      }
    }
  }
}


link_internal void
ComputeNormalsForChunkFromFilledFlag(world_chunk *Chunk, v3i WorldChunkDim, v3 *Normals)
{
  TIMED_FUNCTION();

  v3i ChunkDim = Chunk->Dim;

  for ( s32 z = 0; z < ChunkDim.z; ++ z)
  {
    for ( s32 y = 0; y < ChunkDim.y; ++ y)
    {
      for ( s32 x = 0; x < ChunkDim.x; ++ x)
      {
        s32 VoxIndex = GetIndex(V3i(x,y,z), ChunkDim);

        if ( GetOccupancyBit(Chunk, VoxIndex) )
        {
          v3 Normal = {};
          for ( s32 dz = -1; dz < 2; ++ dz)
          {
            for ( s32 dy = -1; dy < 2; ++ dy)
            {
              for ( s32 dx = -1; dx < 2; ++ dx)
              {
                if (dz == 0 && dy == 0 && dx == 0) continue; // Skip the middle-most voxel

                s32 dP = TryGetIndex(V3i(x+dx,y+dy,z+dz), ChunkDim);
                if (dP > -1)
                {
                  if ( GetOccupancyBit(Chunk, dP) )
                  {
                    Normal += V3(dx,dy,dz);
                  }
                }
              }
            }
          }

          Normals[VoxIndex] = Normalize(Normal) * -1.f; // Invert because we tally'd up all filled voxels
        }
      }
    }
  }

#if 0
  // TODO(Jesse): Parameterize this iteratin in terms of the min offset in case
  // we change it
  for ( s32 z = 1; z < WorldChunkDim.z+1; ++ z)
  {
    for ( s32 y = 1; y < WorldChunkDim.y+1; ++ y)
    {
      for ( s32 x = 1; x < WorldChunkDim.x+1; ++ x)
      {
        s32 VoxIndex = GetIndex(V3i(x,y,z), ChunkDim);

        if ( Chunk->Voxels[VoxIndex].Flags & Voxel_Filled )
        {
          v3 Normal = Normals[VoxIndex];
          for ( s32 dz = -1; dz < 2; ++ dz)
          {
            for ( s32 dy = -1; dy < 2; ++ dy)
            {
              for ( s32 dx = -1; dx < 2; ++ dx)
              {
                if (dz == 0 && dy == 0 && dx == 0) continue; // Skip the middle-most voxel

                s32 dP = GetIndex(V3i(x+dx,y+dy,z+dz), ChunkDim);
                Normal += 0.05f*Normals[dP];
              }
            }
          }
          Normals[VoxIndex] = Normalize(Normal);
        }

      }
    }
  }
#endif

}

#if 1
link_internal void
ComputeNormalsForChunkFromNoiseValues_Opt(v3i Dim, r32 ChunkWorldZ, r32 *NoiseValues, v3 *Normals)
{
  TIMED_FUNCTION();
  /* HISTOGRAM_FUNCTION(); */

  s32 MaxIndex = Volume(Dim);
  s32 StartingIndex = 1 + Dim.x + (Dim.x*Dim.y);

  s32 VoxIndex = StartingIndex;
  for ( s32 z = 1; z < Dim.z-1; ++ z)
  {
    for ( s32 y = 1; y < Dim.y-1; ++ y)
    {
      for ( s32 x = 1; x < Dim.x-1; ++ x)
      {
        r32 CurrentNoiseValue = NoiseValues[VoxIndex];
        /* s32 TestVoxIndex = GetIndex(V3i(x,y,z), Dim); */
        /* Assert(VoxIndex == TestVoxIndex); */

        v3 Normal = {};
        s32 dPIndex = VoxIndex-StartingIndex;
        for ( s32 dz = -1; dz < 2; ++ dz)
        {
          for ( s32 dy = -1; dy < 2; ++ dy)
          {
            for ( s32 dx = -1; dx < 2; ++ dx)
            {
              if (dz == 0 && dy == 0 && dx == 0)
              {
                dPIndex += 1;
                continue; // Skip the middle-most voxel
              }

              /* s32 TestdPIndex = GetIndex(V3i(x+dx,y+dy,z+dz), Dim); */
              /* Assert(dPIndex == TestdPIndex); */
              /* if (dPIndex > -1) */
              {
                /* Assert(dPIndex < MaxIndex); */
                r32 Diff = NoiseValues[dPIndex]-dz - Truncate(CurrentNoiseValue);
                if ( Diff > 0.f )
                {
                  // TODO(Jesse): Recompute with a small random variance to the weight if this is 0?
                  Normal += V3(dx,dy,dz)*Diff;
                }
              }
              dPIndex += 1;
            }
            dPIndex += Dim.x-3; // Skip to the next row
          }
          dPIndex += (Dim.x*Dim.y)-(3*Dim.x); // Skip to the next plate
        }

        /* Assert(VoxIndex < MaxIndex); */
        Normals[VoxIndex] = Normalize(Normal) * -1.f;

        VoxIndex += 1;
      }

      VoxIndex += 2; // Skip the last one in the row, and the first one in the next
    }

    VoxIndex += (2*Dim.y); // Skip the last row, and the next
  }
}
#endif

link_internal void
ComputeNormalsForChunkFromNoiseValues( r32 ChunkWorldZ, r32 *NoiseValues, v3i NoiseDim, v3 *Normals, v3i NormalsDim)
{
  TIMED_FUNCTION();

  // NOTE(Jesse): For this function to work the noise has to be one voxel larger
  // than the normal data
  Assert(NoiseDim == NormalsDim+2);

#if 1
  for ( s32 z = 0; z < NormalsDim.z; ++ z)
  {
    for ( s32 y = 0; y < NormalsDim.y; ++ y)
    {
      for ( s32 x = 0; x < NormalsDim.x; ++ x)
      {
        s32 NoiseIndex = GetIndex(V3i(x,y,z)+1, NoiseDim);
        r32 CurrentNoiseValue = NoiseValues[NoiseIndex];

        v3 Normal = {};
        for ( s32 dz = -1; dz < 2; ++ dz)
        {
          for ( s32 dy = -1; dy < 2; ++ dy)
          {
            for ( s32 dx = -1; dx < 2; ++ dx)
            {
              if (dz == 0 && dy == 0 && dx == 0) continue; // Skip the middle-most voxel

              s32 dNoiseIndex = GetIndex(V3i(x+dx,y+dy,z+dz)+1, NoiseDim);
              {
                r32 Diff = NoiseValues[dNoiseIndex]-dz - Truncate(CurrentNoiseValue);
                if ( Diff > 0.f )
                {
                  // TODO(Jesse): Recompute with a small random variance to the weight if this is 0?
                  Normal += V3(dx,dy,dz)*Diff;
                }
              }
            }
          }
        }

        s32 NormalIndex = GetIndex(V3i(x,y,z), NormalsDim);
        Normals[NormalIndex] = Normalize(Normal) * -1.f;
      }
    }
  }
#else
  for ( s32 z = 1; z < NoiseDim.z-1; ++ z)
  {
    for ( s32 y = 1; y < NoiseDim.y-1; ++ y)
    {
      for ( s32 x = 1; x < NoiseDim.x-1; ++ x)
      {
        s32 NormalIndex = GetIndex(V3i(x,y,z)-1, NormalsDim);
        s32 NoiseIndex = GetIndex(V3i(x,y,z), NoiseDim);
        r32 CurrentNoiseValue = NoiseValues[NoiseIndex];

        v3 Normal = {};
        for ( s32 dz = -1; dz < 2; ++ dz)
        {
          for ( s32 dy = -1; dy < 2; ++ dy)
          {
            for ( s32 dx = -1; dx < 2; ++ dx)
            {
              if (dz == 0 && dy == 0 && dx == 0) continue; // Skip the middle-most voxel

              s32 dPIndex = GetIndex(V3i(x+dx,y+dy,z+dz), NoiseDim);
              {
                r32 Diff = NoiseValues[dPIndex]-dz - Truncate(CurrentNoiseValue);
                if ( Diff > 0.f )
                {
                  // TODO(Jesse): Recompute with a small random variance to the weight if this is 0?
                  Normal += V3(dx,dy,dz)*Diff;
                }
              }
            }
          }
        }
        Normals[NormalIndex] = Normalize(Normal) * -1.f;
      }
    }
  }
#endif
}


link_internal u32
HoodooTerrain( world_chunk *Chunk,
                       v3i  NoiseBasis,
                      void *NoiseParams,
                      void *UserData )
{
  TIMED_FUNCTION();
  UNPACK_NOISE_PARAMS(NoiseParams);

  u32 ChunkSum = 0;

  s32 MinZ = Chunk->WorldP.z*WorldChunkDim.z;
  s32 MaxZ = MinZ+WorldChunkDim.z ;

  octave_buffer *OctaveBuf = (octave_buffer*)UserData;
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
        s32 VoxIndex = GetIndex(V3i(x,y,z), Dim);
        NotImplemented;
        /* Chunk->Voxels[VoxIndex].Flags = Voxel_Empty; */
        /* Assert( NotSet(&Chunk->Voxels[VoxIndex], Voxel_Filled) ); */

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

        v3 ThisColor = RGBColor;

        s32 SnowThreshold = 100;
        if (IsFilled == True && WorldZ > SnowThreshold)
        {
          ThisColor = RGB_WHITE;
        }

        s32 SandThreshold = 3;
        if (IsFilled == True && WorldZ < SandThreshold)
        {
          ThisColor = RGB_YELLOW;
        }

        /* s32 WaterThreshold = 0; */
        /* if (IsFilled == False && WorldZ < WaterThreshold) */
        /* { */
        /*   IsFilled = True; */
        /*   ThisColor = BLUE; */
        /* } */

        /* SetFlag(&Chunk->Voxels[VoxIndex], (voxel_flag)(Voxel_Filled*IsFilled)); */
        SetOccupancyBit(Chunk, VoxIndex, s32(IsFilled));
        Chunk->Voxels[VoxIndex].Color = RGBtoPackedHSV(ThisColor)*u16(IsFilled);
        ChunkSum += IsFilled;

        NotImplemented;
        /* Assert( (Chunk->Voxels[VoxIndex].Flags&VoxelFaceMask) == 0); */
      }
    }
  }

  return ChunkSum;
}

link_internal u32
TerracedTerrain( world_chunk *Chunk,
                         v3i  NoiseBasis,
                        void *NoiseParams,
                        void *UserData )
{
  TIMED_FUNCTION();
  UNPACK_NOISE_PARAMS(NoiseParams);
  u32 ChunkSum = 0;

  s32 MinZ = Chunk->WorldP.z*WorldChunkDim.z;
  s32 MaxZ = MinZ+WorldChunkDim.z ;

  octave_buffer *OctaveBuf = (octave_buffer*)UserData;
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
        s32 VoxIndex = GetIndex(V3i(x,y,z), Dim);
        NotImplemented;
        /* Chunk->Voxels[VoxIndex].Flags = Voxel_Empty; */
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
        /* v3 ThisColor = SafeTruncateToU16(umm(RandomBetween(StartColorMin, &GenColorEntropy, StartColorMax))); */
        v3 ThisColor = RGB_DIRT;

        u8 ThisTransparency = 0;

        s32 SandThreshold   = 3;
        s32 GravelThreshold = 1;
        s32 WaterThreshold  = 0;

        r32 StoneThresh = r32(WorldZSubZMin) + 2.f;
        r32 DirtThresh = r32(WorldZSubZMin) + 1.f;

        if (IsFilled)
        {
          ThisColor = RGB_GRASS_GREEN;

          /* if (NoiseValue > DirtThresh) */
          /* { */
          /*   ThisColor = DIRT; */
          /* } */

          if (NoiseValue > StoneThresh)
          {
            ThisColor = RGB_DIRT;
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

        /* SetFlag(&Chunk->Voxels[VoxIndex], (voxel_flag)(Voxel_Filled*IsFilled)); */
        SetOccupancyBit(Chunk, VoxIndex, s32(IsFilled));
        Chunk->Voxels[VoxIndex].Color = RGBtoPackedHSV(ThisColor)*u16(IsFilled);
        Chunk->Voxels[VoxIndex].Transparency = ThisTransparency;
        ChunkSum += IsFilled;



        NotImplemented;
        /* Assert( (Chunk->Voxels[VoxIndex].Flags&VoxelFaceMask) == 0); */
        SetOccupancyBit(Chunk, VoxIndex, s32(IsFilled));
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

link_internal u32
GrassyTerracedTerrain2( world_chunk *Chunk,
                                v3i  NoiseBasis,
                               void *NoiseParams,
                               void *UserData )
{
  TIMED_FUNCTION();
  UNPACK_NOISE_PARAMS(NoiseParams);

  u32 ChunkSum = 0;

  s32 MinZ = Chunk->WorldP.z*WorldChunkDim.z;
  s32 MaxZ = MinZ+WorldChunkDim.z ;

  octave_buffer *OctaveBuf = (octave_buffer*)UserData;
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

        r32 TerraceMask = 0.f;
        {
          octave TerraceOctaves[] =
          {
            {V3(250, 250, 25), 15.f, V3(2.f)},
          };
          /* RangeIterator(HoodooOctaveIndex, (s32)ArrayCount(TerraceOctaves)) */
          {
            {
              octave O = TerraceOctaves[0];
              f32 OctaveX = (x + SrcToDest.x + ( WorldChunkDim.x*Chunk->WorldP.x)) / O.Freq.x;
              f32 OctaveY = (y + SrcToDest.y + ( WorldChunkDim.y*Chunk->WorldP.y)) / O.Freq.y;
              f32 OctaveZ = (z + SrcToDest.z + ( WorldChunkDim.z*Chunk->WorldP.z)) / O.Freq.z;
              /* f32 OctaveZ = 1.f; */
              TerraceMask = PerlinNoise(OctaveX, OctaveY, OctaveZ) * O.Amp;
            }
          }
        }

        /* s64 WorldX = x + SrcToDest.x + (WorldChunkDim.x*Chunk->WorldP.x); */
        s32 VoxIndex = GetIndex(V3i(x,y,z), Dim);
        NotImplemented;
        /* Chunk->Voxels[VoxIndex].Flags = Voxel_Empty; */
        /* Assert( NotSet(&Chunk->Voxels[VoxIndex], Voxel_Filled) ); */

        r32 NoiseValue = 0.f;
        r32 BaseNoiseValue = 0.f;
        /* v3 Normal = V3(0); */
        v3 Derivs = V3(0);
        for (u32 OctaveIndex = 0; OctaveIndex < OctaveCount; ++OctaveIndex)
        {
          octave *Octave = OctaveBuf->Octaves+OctaveIndex;

          f32 InX = (x + SrcToDest.x + (WorldChunkDim.x*Chunk->WorldP.x)) / Octave->Freq.x;
          f32 InY = (y + SrcToDest.y + (WorldChunkDim.y*Chunk->WorldP.y)) / Octave->Freq.y;
          f32 InZ = (z + SrcToDest.z + (WorldChunkDim.z*Chunk->WorldP.z)) / Octave->Freq.z;

          /* r32 Warp = PerlinNoise(InX, InY, InZ); */
          /* v3 WarpFactor = Warp*Octave->WarpStrength; */
          v3 WarpFactor = {};

          r32 N = PerlinNoise(InX+WarpFactor.x, InY+WarpFactor.y, InZ+WarpFactor.z);

          /* Assert(N <= 1.05f); */
          /* Assert(N > -1.05f); */

          BaseNoiseValue += (N*Octave->Amp);
        }

        NoiseValue = BaseNoiseValue / TerraceMask;
        b32 IsFilled = r32(NoiseValue) > r32(WorldZSubZMin) ;

        v3 ThisColor = RGB_DIRT;

        u8 ThisTransparency = 0;

        s32 SandThreshold   = 3;
        s32 GravelThreshold = 1;
        s32 WaterThreshold  = 0;

        r32 StoneThresh = r32(WorldZSubZMin) + 2.f;
        r32 DirtThresh = r32(WorldZSubZMin) + 1.f;

        /* if (IsFilled) */
        {
          ThisColor = RGB_GRASS_GREEN;

          if (NoiseValue > StoneThresh)
          {
            ThisColor = RGB_DIRT;
          }
        }

        if (!IsFilled)
        {
          f32 GrassAreaX = (x + SrcToDest.x + (WorldChunkDim.x*Chunk->WorldP.x)) / 32.f;
          f32 GrassAreaY = (y + SrcToDest.y + (WorldChunkDim.y*Chunk->WorldP.y)) / 32.f;
          f32 GrassAreaZ = (z + SrcToDest.z + (WorldChunkDim.z*Chunk->WorldP.z)) / 32.f;

          r32 GrassyAreaValue = PerlinNoise(GrassAreaX, GrassAreaY, GrassAreaZ);
          if (ThisColor == RGB_GRASS_GREEN)
          {
            if (GrassyAreaValue > 0.5f && NoiseValue+(GrassyAreaValue*5.f) > WorldZSubZMin)
            {
              f32 GrassX = (x + SrcToDest.x + (WorldChunkDim.x*Chunk->WorldP.x)) / 30.1f;
              f32 GrassY = (y + SrcToDest.y + (WorldChunkDim.y*Chunk->WorldP.y)) / 30.1f;
              f32 GrassZ = (z + SrcToDest.z + (WorldChunkDim.z*Chunk->WorldP.z)) / 450.f;

              r32 GrassValue = PerlinNoise(GrassX, GrassY, GrassZ);

              /* if (NoiseValue+(GrassValue*2.5f) > WorldZSubZMin) */
              {
                IsFilled = True;
              }
            }
          }
        }

        GrowGrassPerlin( Chunk, V3i(x,y,z), NoiseValue, 1.f-TerraceMask, SrcToDest, WorldChunkDim, WorldZSubZMin, &ThisColor, &IsFilled );

        /* SetFlag(&Chunk->Voxels[VoxIndex], (voxel_flag)(Voxel_Filled*IsFilled)); */
        SetOccupancyBit(Chunk, VoxIndex, s32(IsFilled));

        Chunk->Voxels[VoxIndex].Color = RGBtoPackedHSV(ThisColor)*u8(IsFilled);
        Chunk->Voxels[VoxIndex].Transparency = ThisTransparency;
        ChunkSum += IsFilled;

        NotImplemented;
        /* Assert( (Chunk->Voxels[VoxIndex].Flags&VoxelFaceMask) == 0); */
      }
    }
  }

#if 0
  RangeIterator(z, (s32)Chunk->Dim.z)
  RangeIterator(y, (s32)Chunk->Dim.y)
  RangeIterator(x, (s32)Chunk->Dim.x)
  {
    voxel *V   = GetVoxel(Chunk, V3i(x,y,z));
    voxel *VUp = TryGetVoxel(Chunk, V3i(x,y,z+1));

    if (VUp && (VUp->Flags & Voxel_Filled) == 0)
    {
      if (V->Flags & Voxel_Filled)
      {
      }
    }
  }
#endif


  return ChunkSum;
}

link_internal u32
GrassyTerracedTerrain3( world_chunk *Chunk,
                                v3i  NoiseBasis,
                               void *NoiseParams,
                               void *UserData )
{
  TIMED_FUNCTION();
  UNPACK_NOISE_PARAMS(NoiseParams);
  u32 ChunkSum = 0;

  s32 MinZ = Chunk->WorldP.z*WorldChunkDim.z;
  s32 MaxZ = MinZ+WorldChunkDim.z ;

  octave_buffer *OctaveBuf = (octave_buffer*)UserData;
  u32 OctaveCount = OctaveBuf->Count;


  for ( s32 z = 0; z < Dim.z; ++ z)
  {
    s64 WorldZ = z + SrcToDest.z + (WorldChunkDim.z*Chunk->WorldP.z);
    s64 WorldZSubZMin = WorldZ - zMin;
    /* s64 WorldZSubZMin = zMin; */
    for ( s32 y = 0; y < Dim.y; ++ y)
    {
      s64 WorldY = y + SrcToDest.y + (WorldChunkDim.y*Chunk->WorldP.y);
      for ( s32 x = 0; x < Dim.x; ++ x)
      {

        r32 TerraceMask = 0.f;
#if 0
        {
          octave TerraceOctaves[] =
          {
            {V3(250, 250, 25), 15.f, V3(2.f)},
          };
          /* RangeIterator(HoodooOctaveIndex, (s32)ArrayCount(TerraceOctaves)) */
          {
            {
              octave O = TerraceOctaves[0];
              f32 OctaveX = (x + SrcToDest.x + ( WorldChunkDim.x*Chunk->WorldP.x)) / O.Freq.x;
              f32 OctaveY = (y + SrcToDest.y + ( WorldChunkDim.y*Chunk->WorldP.y)) / O.Freq.y;
              f32 OctaveZ = (z + SrcToDest.z + ( WorldChunkDim.z*Chunk->WorldP.z)) / O.Freq.z;
              /* f32 OctaveZ = 1.f; */
              TerraceMask = PerlinNoise(OctaveX, OctaveY, OctaveZ) * O.Amp;
            }
          }
        }
#endif

        s64 WorldX = x + SrcToDest.x + (WorldChunkDim.x*Chunk->WorldP.x);
        s32 VoxIndex = GetIndex(V3i(x,y,z), Dim);
        NotImplemented;
        /* Chunk->Voxels[VoxIndex].Flags = Voxel_Empty; */
        /* Assert( NotSet(&Chunk->Voxels[VoxIndex], Voxel_Filled) ); */

        r32 NoiseValue = 0.f;
        r32 BaseNoiseValue = 0.f;
        /* v3 Normal = V3(0); */
        v3 Derivs = V3(0);

        f32 MaxValue = 0.f;
        for (u32 OctaveIndex = 0; OctaveIndex < OctaveCount; ++OctaveIndex)
        {
          octave *Octave = OctaveBuf->Octaves+OctaveIndex;

          f32 InX = (x + SrcToDest.x + (WorldChunkDim.x*Chunk->WorldP.x)) / Octave->Freq.x;
          f32 InY = (y + SrcToDest.y + (WorldChunkDim.y*Chunk->WorldP.y)) / Octave->Freq.y;
          f32 InZ = (z + SrcToDest.z + (WorldChunkDim.z*Chunk->WorldP.z)) / Octave->Freq.z;

          /* r32 Warp = PerlinNoise(InX, InY, InZ); */
          /* v3 WarpFactor = Warp*Octave->WarpStrength; */
          v3 WarpFactor = {};

          r32 N = PerlinNoise(InX+WarpFactor.x, InY+WarpFactor.y, InZ+WarpFactor.z);
#if 1
          if (OctaveIndex == 0)
          {
            BaseNoiseValue += MapNoiseValueToFinal(N) * Octave->Amp;
          }
          else
          {
            BaseNoiseValue += N * Octave->Amp;
          }
#else
            BaseNoiseValue += MapNoiseValueToFinal(N) * Octave->Amp;
#endif

          MaxValue += Octave->Amp;
        }

        /* NoiseValue = BaseNoiseValue / TerraceMask; */
        NoiseValue = BaseNoiseValue;

        v3 ThisColor = MapNoiseValueToMaterial(BaseNoiseValue/MaxValue);

        u8 ThisTransparency = 0;

        if (ThisColor == RGB_STONE)
        {
          r32 Voronoi = VoronoiNoise3D(V3(s32(WorldX), s32(WorldY), s32(WorldZ)) * 0.04f) * 25.f;
          if (Voronoi < 2.f)
          {
            NoiseValue -= 2.f;
          }

          if (Voronoi < 5.f)
          {
            ThisColor = RGB_DARK_STONE;
          }
        }

        b32 IsFilled = r32(NoiseValue) > r32(WorldZSubZMin);

        GrowGrassPerlin( Chunk, V3i(x,y,z), NoiseValue, 1.f-TerraceMask, SrcToDest, WorldChunkDim, WorldZSubZMin, &ThisColor, &IsFilled );

        /* SetFlag(&Chunk->Voxels[VoxIndex], (voxel_flag)(Voxel_Filled*IsFilled)); */
        SetOccupancyBit(Chunk, VoxIndex, s32(IsFilled));
        Chunk->Voxels[VoxIndex].Color = RGBtoPackedHSV(ThisColor)*u16(IsFilled);
        Chunk->Voxels[VoxIndex].Transparency = ThisTransparency;
        ChunkSum += IsFilled;

        NotImplemented;
        /* Assert( (Chunk->Voxels[VoxIndex].Flags&VoxelFaceMask) == 0); */
      }
    }
  }

#if 0
  RangeIterator(z, (s32)Chunk->Dim.z)
  RangeIterator(y, (s32)Chunk->Dim.y)
  RangeIterator(x, (s32)Chunk->Dim.x)
  {
    voxel *V   = GetVoxel(Chunk, V3i(x,y,z));
    voxel *VUp = TryGetVoxel(Chunk, V3i(x,y,z+1));

    if (VUp && (VUp->Flags & Voxel_Filled) == 0)
    {
      if (V->Flags & Voxel_Filled)
      {
      }
    }
  }
#endif


  return ChunkSum;
}

link_internal s32
UnilateralToThesholdIndex(f32 Value, s32 ClipPoints)
{
  Assert(Value <= 1.f);
  Assert(Value >= 0.f);

  s32 Result = s32(Value*ClipPoints);
  return Result;
}

link_internal void
MakeCliffs(world_chunk *Chunk, s32 VoxIndex, s32 WorldX, s32 WorldY, s32 WorldZ, r32 *NoiseValue, v3 *Normal, v3 *ThisColor)
{
  random_series ColorEntropy = RandomSeriesFromV3i(V3i(WorldX, WorldY, WorldZ));
  s32 Index = 0;
  /* RangeIterator(Index, 8) */
  {
    r32 DotNormal = Dot(Normal[Index], V3(0,0,1));

    r32 Thresh = 0.43f; // Lower == more cliffy
    r32 ClampedDotNormal = Clamp(Thresh, 1.f-DotNormal, 1.f);

    // 1.f when the cliff is at it's most cliffy (terrain is vertical)
    r32 CliffBlend = MapValueToUnilateral(Thresh, ClampedDotNormal, 1.f);

    if (CliffBlend > 0.f)
    {
      /* Chunk->Voxels[NormalsIndex].DebugColor.x = CliffBlend; */

      /* r32 Squareness = 0.1f; */
      r32 Squareness = 0.25f;
      /* r32 Squareness = 0.55f; */
      /* r32 Squareness = 0.75f; */
      /* r32 Squareness = 0.8f; */
      /* r32 Squareness = 1.f; */

      v3 BaseCellSize = V3(5.f, 5.f, 5.f); // Increasing this increases the size of the cells

      v3 CellSizes[] = {
        {BaseCellSize},
        {BaseCellSize*V3(3.f, 3.f, 6.f)},
        {BaseCellSize*V3(7.f, 7.f, 18.f)},
      };

      s32 CellIndex = UnilateralToThesholdIndex(CliffBlend, ArrayCount(CellSizes)-1);
      Assert(CellIndex < s32(ArrayCount(CellSizes)));
      v3 CellSize = CellSizes[CellIndex];

      v3 CellMultiplier = (1.f/CellSize);
      v3 WorldP = V3(WorldX+Index, WorldY, WorldZ);
      /* r32 RockMaskChance = CliffBlend*6.f; // Higher = less rocks */
      r32 RockMaskChance = 0.f;
      r32 Voronoi = VoronoiNoise3D(WorldP * CellMultiplier, Squareness, RockMaskChance);
      NoiseValue[Index] += CliffBlend*Voronoi*10.f;

      r32 DarkStoneThresh = 0.08f;  // Increasing this increases the amount of darkness
      if (Voronoi < DarkStoneThresh)
      {
        if (CliffBlend > 0.1f)
        {
          if (RandomUnilateral(&ColorEntropy)-0.75f < CliffBlend)
          {
            NoiseValue[Index] -= 2.f*(CliffBlend);
            ThisColor[Index] = RGB_DARK_STONE;
          }
        }
        else
        {
          f32 ColorPick = RandomUnilateral(&ColorEntropy);
          if (ColorPick > 0.94f)
          {
            ThisColor[Index] = RGB_DARK_GRASS_GREEN;
          }
          else if (ColorPick > 0.88f)
          {
            ThisColor[Index] = RGB_MOSS_GREEN;
          }
          else if (ColorPick > 0.80f)
          {
            ThisColor[Index] = RGB_LIGHT_MOSS_GREEN;
          }
        }
      }
      else
      {
        ThisColor[Index] = RGB_STONE;
      }
    }
  }
}

link_internal void
MakeCliffs_8x(world_chunk *Chunk, s32 NormalsIndex, s32 WorldX, s32 WorldY, s32 WorldZ, r32 *NoiseValue, v3 *Normal, v3 *ThisColor)
{
#if 0
  RangeIterator(Index, 8)
  {
    MakeCliffs(Chunk, NormalsIndex, WorldX+Index, WorldY, WorldZ, NoiseValue+Index, Normal+Index, ThisColor+Index);
  }
#else
  random_series ColorEntropy = RandomSeriesFromV3i(V3i(WorldX, WorldY, WorldZ));
  RangeIterator(Index, 8)
  {
    r32 DotNormal = Dot(Normal[Index], V3(0,0,1));

    r32 Thresh = 0.43f; // Lower == more cliffy
    r32 ClampedDotNormal = Clamp(Thresh, 1.f-DotNormal, 1.f);

    // 1.f when the cliff is at it's most cliffy (terrain is vertical)
    r32 CliffBlend = MapValueToUnilateral(Thresh, ClampedDotNormal, 1.f);

    if (CliffBlend > 0.f)
    {
      /* Chunk->Voxels[NormalsIndex].DebugColor.x = CliffBlend; */

      /* r32 Squareness = 0.1f; */
      r32 Squareness = 0.25f;
      /* r32 Squareness = 0.55f; */
      /* r32 Squareness = 0.75f; */
      /* r32 Squareness = 0.8f; */
      /* r32 Squareness = 1.f; */

      v3 BaseCellSize = V3(5.f, 5.f, 5.f); // Increasing this increases the size of the cells

      v3 CellSizes[] = {
        {BaseCellSize},
        {BaseCellSize*V3(3.f, 3.f, 6.f)},
        {BaseCellSize*V3(7.f, 7.f, 18.f)},
      };

      s32 CellIndex = UnilateralToThesholdIndex(CliffBlend, ArrayCount(CellSizes)-1);
      Assert(CellIndex < s32(ArrayCount(CellSizes)));
      v3 CellSize = CellSizes[CellIndex];

      v3 CellMultiplier = (1.f/CellSize);
      v3 WorldP = V3(WorldX+Index, WorldY, WorldZ);
      /* r32 RockMaskChance = CliffBlend*6.f; // Higher = less rocks */
      r32 RockMaskChance = 0.f;
      r32 Voronoi = VoronoiNoise3D(WorldP * CellMultiplier, Squareness, RockMaskChance);

      NoiseValue[Index] += CliffBlend*Voronoi*10.f;

      r32 DarkStoneThresh = 0.08f;  // Increasing this increases the amount of darkness
      if (Voronoi < DarkStoneThresh)
      {
        if (CliffBlend > 0.1f)
        {
          if (RandomUnilateral(&ColorEntropy)-0.75f < CliffBlend)
          {
            NoiseValue[Index] -= 2.f*(CliffBlend);
            ThisColor[Index] = RGB_DARK_STONE;
          }
        }
        else
        {
          f32 ColorPick = RandomUnilateral(&ColorEntropy);
          if (ColorPick > 0.94f)
          {
            ThisColor[Index] = RGB_DARK_GRASS_GREEN;
          }
          else if (ColorPick > 0.88f)
          {
            ThisColor[Index] = RGB_MOSS_GREEN;
          }
          else if (ColorPick > 0.80f)
          {
            ThisColor[Index] = RGB_LIGHT_MOSS_GREEN;
          }
        }
      }
      else
      {
        ThisColor[Index] = RGB_STONE;
      }
    }
  }
#endif
}

#if 0
link_internal u32
GrassyTerracedTerrain4( world_chunk *Chunk,
                                v3i  NoiseBasis,
                               void *NoiseParams,
                               void *UserData )
{
  TIMED_FUNCTION();
  /* HISTOGRAM_FUNCTION(); */
  UNPACK_NOISE_PARAMS(NoiseParams);
  u32 ChunkSum = 0;

  auto _Dim = Dim;
  Assert(_Dim == Chunk->Dim);

  octave_buffer *OctaveBuf = (octave_buffer*)UserData;
  u32 OctaveCount = OctaveBuf->Count;

  f32 MaxNoiseValue = 0.f;
  for (u32 OctaveIndex = 0; OctaveIndex < OctaveCount; ++OctaveIndex)
  {
    octave *Octave = OctaveBuf->Octaves+OctaveIndex;
    MaxNoiseValue += Octave->Amp;
  }

  v3i NormalDim = _Dim;
  v3i NoiseDim = _Dim+2;
  /* b32 *NoiseHit   = Allocate(b32, GetTranArena(), Volume(NoiseDim)); */
  /* b32 *NormalsHit = Allocate(b32, GetTranArena(), Volume(NormalDim)); */

#if VOXEL_DEBUG_COLOR
  memory_arena *TempArena = GetThreadLocalState(ThreadLocal_ThreadIndex)->PermMemory;
#else
  memory_arena *TempArena = GetTranArena();
#endif

  r32 *NoiseValues = Allocate(r32, TempArena, Volume(NoiseDim));
  v3  *Normals     = Allocate( v3, TempArena, Volume(NormalDim));

#if VOXEL_DEBUG_COLOR
  Chunk->NoiseValues = NoiseValues;
  Chunk->NormalValues = Normals;
#endif

  // NOTE(Jesse): Perlin_8x needs a multiple of 8 here.
  Assert(Dim.x % 8 == 0);

  s32 xNoiseBlockCount = NoiseDim.x / 8;
  {
    TIMED_NAMED_BLOCK(Octaves);
    for ( s32 z = 0; z < NoiseDim.z; ++ z)
    {
      f32 zCoord = (-1 + z + SrcToDest.z + (WorldChunkDim.z*Chunk->WorldP.z));
      for ( s32 y = 0; y < NoiseDim.y; ++ y)
      {
        f32 yCoord = (-1 + y + SrcToDest.y + (WorldChunkDim.y*Chunk->WorldP.y));
        for ( s32 xBlock = 0; xBlock < xNoiseBlockCount; ++xBlock )
        {
          s32 x = xBlock*8;

          s32 NoiseIndex = GetIndex(x,y,z, NoiseDim);
          r32 *NoiseValue = NoiseValues + NoiseIndex;
          /* RangeIterator(Index, 8) */
          /* { */
          /*   Assert(NoiseHit[NoiseIndex+Index] == 0); */
          /*   NoiseHit[NoiseIndex+Index] += 1; */
          /* } */

          for (u32 OctaveIndex = 0; OctaveIndex < OctaveCount; ++OctaveIndex)
          {
            octave *Octave = OctaveBuf->Octaves+OctaveIndex;

            f32 InZ = (-1 + z + SrcToDest.z + (WorldChunkDim.z*Chunk->WorldP.z)) / Octave->Freq.z;
            f32 InY = (-1 + y + SrcToDest.y + (WorldChunkDim.y*Chunk->WorldP.y)) / Octave->Freq.y;

            f32 xCoords[8] =
            {
              (-1 +     x + SrcToDest.x + (WorldChunkDim.x*Chunk->WorldP.x)) / Octave->Freq.x,
              (-1 + 1 + x + SrcToDest.x + (WorldChunkDim.x*Chunk->WorldP.x)) / Octave->Freq.x,
              (-1 + 2 + x + SrcToDest.x + (WorldChunkDim.x*Chunk->WorldP.x)) / Octave->Freq.x,
              (-1 + 3 + x + SrcToDest.x + (WorldChunkDim.x*Chunk->WorldP.x)) / Octave->Freq.x,
              (-1 + 4 + x + SrcToDest.x + (WorldChunkDim.x*Chunk->WorldP.x)) / Octave->Freq.x,
              (-1 + 5 + x + SrcToDest.x + (WorldChunkDim.x*Chunk->WorldP.x)) / Octave->Freq.x,
              (-1 + 6 + x + SrcToDest.x + (WorldChunkDim.x*Chunk->WorldP.x)) / Octave->Freq.x,
              (-1 + 7 + x + SrcToDest.x + (WorldChunkDim.x*Chunk->WorldP.x)) / Octave->Freq.x,
            };

            f32 TmpPerlinResults[8];
            PerlinNoise_8x_avx2(xCoords, InY, InZ, TmpPerlinResults);

            RangeIterator(Index, 8)
            {
              f32 N = TmpPerlinResults[Index];
              if (OctaveIndex == 0)
              {
                NoiseValue[Index] += MapNoiseValueToFinal(N) * Octave->Amp;
              }
              else
              {
                NoiseValue[Index] += N * Octave->Amp;
              }
            }

          }
        }

#if 1
        s32 xFixupBase = (xNoiseBlockCount * 8);
        for ( s32 x = xFixupBase; x < NoiseDim.x; ++x )
        {
          s32 NoiseIndex = GetIndex(x,y,z, NoiseDim);
          /* Assert(NoiseHit[NoiseIndex] == 0); */
          /* NoiseHit[NoiseIndex] += 1; */

          for (u32 OctaveIndex = 0; OctaveIndex < OctaveCount; ++OctaveIndex)
          {
            octave *Octave = OctaveBuf->Octaves+OctaveIndex;


            v3 In = (-1 + V3(x,y,z) + SrcToDest + (WorldChunkDim*Chunk->WorldP)) / Octave->Freq;
            f32 N = PerlinNoise(In.x, In.y, In.z);
            /* f32 N = PerlinNoise(InX, InY, InZ); */
            if (OctaveIndex == 0)
            {
              NoiseValues[NoiseIndex] += MapNoiseValueToFinal(N) * Octave->Amp;
            }
            else
            {
              NoiseValues[NoiseIndex] += N * Octave->Amp;
            }
          }
        }
#endif
      }
    }
  }

#if 0
  {
    TIMED_NAMED_BLOCK(OctavesLeftovers);
    s32 xFixupBase = (xNoiseBlockCount * 8);
    for ( s32 z = 0; z < NoiseDim.z; ++ z)
    {
      for ( s32 y = 0; y < NoiseDim.y; ++ y)
      {
        for ( s32 x = xFixupBase; x < NoiseDim.x; ++x )
        {
          s32 NoiseIndex = GetIndex(x,y,z, NoiseDim);
          Assert(NoiseHit[NoiseIndex] == 0);
          NoiseHit[NoiseIndex] = 1;

          for (u32 OctaveIndex = 0; OctaveIndex < OctaveCount; ++OctaveIndex)
          {
            octave *Octave = OctaveBuf->Octaves+OctaveIndex;

            f32 InY = (-1 + y + SrcToDest.y + (WorldChunkDim.y*Chunk->WorldP.y)) / Octave->Freq.y;
            f32 InZ = (-1 + z + SrcToDest.z + (WorldChunkDim.z*Chunk->WorldP.z)) / Octave->Freq.z;
            f32 InX = (-1 + x + SrcToDest.x + (WorldChunkDim.x*Chunk->WorldP.x)) / Octave->Freq.x;

            /* v3 In = (-1 + V3(x,y,z) + SrcToDest + (WorldChunkDim*Chunk->WorldP)) / Octave->Freq; */
            /* f32 N = PerlinNoise(In.x, In.y, In.z); */
            f32 N = PerlinNoise(InX, InY, InZ);
            if (OctaveIndex == 0)
            {
              NoiseValues[NoiseIndex] += MapNoiseValueToFinal(N) * Octave->Amp;
            }
            else
            {
              NoiseValues[NoiseIndex] += N * Octave->Amp;
            }
          }
        }
      }
    }
  }
#endif

  s64 ChunkWorldZThresh = SrcToDest.z + (WorldChunkDim.z*Chunk->WorldP.z) - zMin;
  ComputeNormalsForChunkFromNoiseValues(ChunkWorldZThresh, NoiseValues, NoiseDim, Normals, NormalDim);

  s32 xShapingBlockCount = NormalDim.x/8;
  {
    TIMED_NAMED_BLOCK(Shaping);
    for ( s32 z = 0; z < NormalDim.z; ++ z)
    {
      s64 WorldZ = z + SrcToDest.z + (WorldChunkDim.z*Chunk->WorldP.z);
      s64 WorldZSubZMin = WorldZ - zMin;
      for ( s32 y = 0; y < NormalDim.y; ++ y)
      {
        s64 WorldY = y + SrcToDest.y + (WorldChunkDim.y*Chunk->WorldP.y);
        for ( s32 xBlock = 0; xBlock < xShapingBlockCount; ++xBlock )
        {
          s32 x = xBlock * 8;
          s64 WorldX = x + SrcToDest.x + (WorldChunkDim.x*Chunk->WorldP.x);

          s32 NormalIndex = GetIndex(V3i(x,y,z), NormalDim);
          s32 NoiseIndex = GetIndex(V3i(x+1,y+1,z+1), NoiseDim);
          /* s32 TestVoxIndex = GetIndex(V3i(x,y,z), NormalDim); */
          /* Assert(NormalIndex == TestVoxIndex); */

          r32 *NoiseValue = NoiseValues + NoiseIndex;
          v3  *Normal     = Normals + NormalIndex;

          v3 ThisColor[8];// = { {RGB_GRASS_GREEN}, {RGB_GRASS_GREEN}, {RGB_GRASS_GREEN}, {RGB_GRASS_GREEN}, {RGB_GRASS_GREEN}, {RGB_GRASS_GREEN}, {RGB_GRASS_GREEN}, {RGB_GRASS_GREEN} };
          ThisColor[0] = RGB_GRASS_GREEN;
          ThisColor[1] = RGB_GRASS_GREEN;
          ThisColor[2] = RGB_GRASS_GREEN;
          ThisColor[3] = RGB_GRASS_GREEN;
          ThisColor[4] = RGB_GRASS_GREEN;
          ThisColor[5] = RGB_GRASS_GREEN;
          ThisColor[6] = RGB_GRASS_GREEN;
          ThisColor[7] = RGB_GRASS_GREEN;

#if 1
          /* Chunk->Voxels[NormalIndex].DebugColor.x = *NoiseValue; */
          MakeCliffs_8x(Chunk, NormalIndex, s32(WorldX), s32(WorldY), s32(WorldZ), NoiseValue, Normal, ThisColor);
          GrowGrassVoronoi_8x( Chunk, V3i(x,y,z), NoiseValue, Normal, SrcToDest, WorldChunkDim, WorldZSubZMin, ThisColor);
          /* RandomVoronoiRocks_8x( Chunk, V3i(x,y,z), NoiseValue, Normal, SrcToDest, WorldChunkDim, WorldZSubZMin, ThisColor); */
          /* GrowGrassPerlin_8x( Chunk, V3i(x,y,z), NoiseValue, Normal, 1.f, SrcToDest, WorldChunkDim, WorldZSubZMin, ThisColor); */
#endif

          RangeIterator(Index, 8)
          {
            /* Assert(NormalsHit[NormalIndex+Index] == 0); */
            /* NormalsHit[NormalIndex+Index] = 1; */

            b32 IsFilled = NoiseValues[NoiseIndex+Index] > r32(z+ChunkWorldZThresh);
            /* SetFlag(&Chunk->Voxels[NormalIndex+Index], (voxel_flag)(Voxel_Filled*IsFilled)); */
            SetOccupancyBit(Chunk, NormalIndex+Index, s32(IsFilled));
            Chunk->Voxels[NormalIndex+Index].Color = RGBtoPackedHSV(ThisColor[Index])*u16(IsFilled);
            /* Chunk->Voxels[NormalIndex].Transparency = ThisTransparency; */
            ChunkSum += IsFilled;
          }
        }

        s32 xFixupBase = (xShapingBlockCount * 8);
        for ( s32 x = xFixupBase; x < NormalDim.x; ++x )
        {
          s64 WorldX = x + SrcToDest.x + (WorldChunkDim.x*Chunk->WorldP.x);

          s32 NormalIndex = GetIndex(V3i(x,y,z), NormalDim);
          /* Assert(NormalsHit[NormalIndex] == 0); */
          /* NormalsHit[NormalIndex] = 1; */

          s32 NoiseIndex = GetIndex(V3i(x+1,y+1,z+1), NoiseDim);
          r32 *NoiseValue = NoiseValues + NoiseIndex;
          v3  *Normal = Normals + NormalIndex;

          v3 ThisColor = RGB_GRASS_GREEN;
          MakeCliffs(Chunk, NormalIndex, s32(WorldX), s32(WorldY), s32(WorldZ), NoiseValue, Normal, &ThisColor);
          GrowGrassVoronoi( Chunk, V3i(x,y,z), NoiseValue, Normal, SrcToDest, WorldChunkDim, WorldZSubZMin, &ThisColor);

          b32 IsFilled = NoiseValues[NoiseIndex] > r32(z+ChunkWorldZThresh);
          /* SetFlag(&Chunk->Voxels[NormalIndex], (voxel_flag)(Voxel_Filled*IsFilled)); */
          SetOccupancyBit(Chunk, NormalIndex, s32(IsFilled));
          Chunk->Voxels[NormalIndex].Color = RGBtoPackedHSV(ThisColor)*u8(IsFilled);
          /* Chunk->Voxels[NormalIndex].Transparency = ThisTransparency; */
          ChunkSum += IsFilled;
        }
      }
    }
  }

#if 0
  {
    TIMED_NAMED_BLOCK(ShapingLeftovers);
    s32 xFixupBase = (xShapingBlockCount * 8);
    for ( s32 z = 0; z < NormalDim.z; ++ z)
    {
      s64 WorldZ = z + SrcToDest.z + (WorldChunkDim.z*Chunk->WorldP.z);

      for ( s32 y = 0; y < NormalDim.y; ++ y)
      {
        s64 WorldY = y + SrcToDest.y + (WorldChunkDim.y*Chunk->WorldP.y);

        for ( s32 x = xFixupBase; x < NormalDim.x; ++x )
        {
          s64 WorldX = x + SrcToDest.x + (WorldChunkDim.x*Chunk->WorldP.x);

          s32 NormalIndex = GetIndex(V3i(x,y,z), NormalDim);
          Assert(NormalsHit[NormalIndex] == 0);
          NormalsHit[NormalIndex] = 1;
          s32 NoiseIndex = GetIndex(V3i(x+1,y+1,z+1), NoiseDim);

          v3 ThisColor = GRASS_GREEN;
          MakeCliffs_8x(Chunk, NormalIndex, s32(WorldX), s32(WorldY), s32(WorldZ), NoiseValue, Normal, ThisColor);

          b32 IsFilled = NoiseValues[NoiseIndex] > r32(z+ChunkWorldZThresh);
          SetFlag(&Chunk->Voxels[NormalIndex], (voxel_flag)(Voxel_Filled*IsFilled));
          Chunk->Voxels[NormalIndex].Color = ThisColor*u8(IsFilled);
          /* Chunk->Voxels[NormalIndex].Transparency = ThisTransparency; */
          ChunkSum += IsFilled;
        }
      }
    }
  }
#endif

  /* RangeIterator(Index, Volume(NoiseDim)) */
  /* { */
  /*   Assert(NoiseHit[Index] == 1); */
  /* } */


  /* RangeIterator(Index, Volume(NormalDim)) */
  /* { */
  /*   Assert(NormalsHit[Index] == 1); */
  /* } */


#if 0
  v3 *Normals = Allocate(v3, GetTranArena(), Volume(Dim));
  ComputeNormalsForChunkFromFilledFlag(Chunk, WorldChunkDim, Normals);
#endif

#if VOXEL_DEBUG_COLOR
#if 0
  for ( s32 z = 0; z < Chunk->Dim.z; ++ z)
  {
    for ( s32 y = 0; y < Chunk->Dim.y; ++ y)
    {
      for ( s32 x = 0; x < Chunk->Dim.x; ++ x)
      {
        /* s32 Index  = GetIndex(V3i(x,y,z), NormalDim); */
        s32 NormalIndex  = GetIndex(V3i(x,y,z), NormalDim);
        s32 NoiseIndex   = GetIndex(V3i(x,y,z)+1, NoiseDim);
        Chunk->Voxels[NormalIndex].DebugColor      = Normals[NormalIndex];
        Chunk->Voxels[NormalIndex].DebugNoiseValue = NoiseValues[NoiseIndex];
      }
    }
  }
#endif
#endif

  return ChunkSum;
}
#endif

link_internal u32
GrassyTerracedTerrain( world_chunk *Chunk,
                               v3i  NoiseBasis,
                              void *NoiseParams,
                              void *UserData )
{
  TIMED_FUNCTION();
  UNPACK_NOISE_PARAMS(NoiseParams);

  u32 ChunkSum = 0;

  s32 MinZ = Chunk->WorldP.z*WorldChunkDim.z;
  s32 MaxZ = MinZ+WorldChunkDim.z ;

  octave_buffer *OctaveBuf = (octave_buffer*)UserData;
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
        s32 VoxIndex = GetIndex(V3i(x,y,z), Dim);
        NotImplemented;
        /* Chunk->Voxels[VoxIndex].Flags = Voxel_Empty; */
        /* Assert( NotSet(&Chunk->Voxels[VoxIndex], Voxel_Filled) ); */

        r32 NoiseValue = 0.f;
        r32 BaseNoiseValue = 0.f;
        /* v3 Normal = V3(0); */
        v3 Derivs = V3(0);
        for (u32 OctaveIndex = 0; OctaveIndex < OctaveCount; ++OctaveIndex)
        {
          octave *Octave = OctaveBuf->Octaves+OctaveIndex;

          f32 InX = (x + SrcToDest.x + (WorldChunkDim.x*Chunk->WorldP.x)) / Octave->Freq.x;
          f32 InY = (y + SrcToDest.y + (WorldChunkDim.y*Chunk->WorldP.y)) / Octave->Freq.y;
          f32 InZ = (z + SrcToDest.z + (WorldChunkDim.z*Chunk->WorldP.z)) / Octave->Freq.z;

          /* r32 Warp = PerlinNoise(InX, InY, InZ); */
          /* v3 WarpFactor = Warp*Octave->WarpStrength; */
          v3 WarpFactor = {};

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
        /* v3 ThisColor = SafeTruncateToU16(umm(RandomBetween(StartColorMin, &GenColorEntropy, StartColorMax))); */
        v3 ThisColor = RGB_DIRT;

        u8 ThisTransparency = 0;

        s32 SandThreshold   = 3;
        s32 GravelThreshold = 1;
        s32 WaterThreshold  = 0;

        r32 StoneThresh = r32(WorldZSubZMin) + 2.f;
        /* r32 DirtThresh = r32(WorldZSubZMin) + 1.f; */

        /* if (IsFilled) */
        {
          ThisColor = RGB_GRASS_GREEN;

          if (NoiseValue > StoneThresh)
          {
            ThisColor = RGB_DIRT;
          }
        }

        GrowGrassPerlin( Chunk, V3i(x,y,z), NoiseValue, 1.f, SrcToDest, WorldChunkDim, WorldZSubZMin, &ThisColor, &IsFilled );

        /* SetFlag(&Chunk->Voxels[VoxIndex], (voxel_flag)(Voxel_Filled*IsFilled)); */
        SetOccupancyBit(Chunk, VoxIndex, s32(IsFilled));
        Chunk->Voxels[VoxIndex].Color = RGBtoPackedHSV(ThisColor)*u8(IsFilled);
        Chunk->Voxels[VoxIndex].Transparency = ThisTransparency;
        ChunkSum += IsFilled;

        NotImplemented;
        /* Assert( (Chunk->Voxels[VoxIndex].Flags&VoxelFaceMask) == 0); */
      }
    }
  }

#if 0
  RangeIterator(z, (s32)Chunk->Dim.z)
  RangeIterator(y, (s32)Chunk->Dim.y)
  RangeIterator(x, (s32)Chunk->Dim.x)
  {
    voxel *V   = GetVoxel(Chunk, V3i(x,y,z));
    voxel *VUp = TryGetVoxel(Chunk, V3i(x,y,z+1));

    if (VUp && (VUp->Flags & Voxel_Filled) == 0)
    {
      if (V->Flags & Voxel_Filled)
      {
      }
    }
  }
#endif


  return ChunkSum;
}

link_internal u32
GrassyLargeTerracedTerrain( world_chunk *Chunk,
                                v3i  NoiseBasis,
                               void *NoiseParams,
                               void *UserData )
{
  TIMED_FUNCTION();
  UNPACK_NOISE_PARAMS(NoiseParams);
  u32 ChunkSum = 0;

  s32 MinZ = Chunk->WorldP.z*WorldChunkDim.z;
  s32 MaxZ = MinZ+WorldChunkDim.z ;

  octave_buffer *OctaveBuf = (octave_buffer*)UserData;
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
        r32 TerraceMask = 0.f;
        r32 TerraceNoiseValue = 0.f;
        {
          octave TerraceOctaves[] =
          {
            {V3(500, 500, 100), 15.f, V3(2.f)},
          };

          {
            octave O = TerraceOctaves[0];
            f32 OctaveX = (x + SrcToDest.x + ( WorldChunkDim.x*Chunk->WorldP.x)) / O.Freq.x;
            f32 OctaveY = (y + SrcToDest.y + ( WorldChunkDim.y*Chunk->WorldP.y)) / O.Freq.y;
            f32 OctaveZ = (z + SrcToDest.z + ( WorldChunkDim.z*Chunk->WorldP.z)) / O.Freq.z;
            /* f32 OctaveZ = 1.f; */
            TerraceMask = PerlinNoise(OctaveX, OctaveY, OctaveZ);
            TerraceNoiseValue = TerraceMask * O.Amp;
          }

        }

        /* s64 WorldX = x + SrcToDest.x + (WorldChunkDim.x*Chunk->WorldP.x); */
        s32 VoxIndex = GetIndex(V3i(x,y,z), Dim);
        NotImplemented;
        /* Chunk->Voxels[VoxIndex].Flags = Voxel_Empty; */
        /* Assert( NotSet(&Chunk->Voxels[VoxIndex], Voxel_Filled) ); */

        r32 NoiseValue = 0.f;
        r32 BaseNoiseValue = 0.f;
        /* v3 Normal = V3(0); */
        v3 Derivs = V3(0);
        for (u32 OctaveIndex = 0; OctaveIndex < OctaveCount; ++OctaveIndex)
        {
          octave *Octave = OctaveBuf->Octaves+OctaveIndex;

          f32 InX = (x + SrcToDest.x + (WorldChunkDim.x*Chunk->WorldP.x)) / Octave->Freq.x;
          f32 InY = (y + SrcToDest.y + (WorldChunkDim.y*Chunk->WorldP.y)) / Octave->Freq.y;
          f32 InZ = (z + SrcToDest.z + (WorldChunkDim.z*Chunk->WorldP.z)) / Octave->Freq.z;

          /* r32 Warp = PerlinNoise(InX, InY, InZ); */
          /* v3 WarpFactor = Warp*Octave->WarpStrength; */
          v3 WarpFactor = {};

          r32 N = PerlinNoise(InX+WarpFactor.x, InY+WarpFactor.y, InZ+WarpFactor.z);

          /* Assert(N <= 1.05f); */
          /* Assert(N > -1.05f); */

          /* BaseNoiseValue = N; */

          /* Normal += CalcNormal(V3(InX, InY, InZ), N, PerlinNoise ); */
          if (OctaveIndex < 2)
          {
            BaseNoiseValue += (N*Octave->Amp) / TerraceNoiseValue;
            /* BaseNoiseValue += (N*Octave->Amp); */
          }
          else
          {
            BaseNoiseValue += (N*Octave->Amp);
          }
        }

        NoiseValue = BaseNoiseValue;

        b32 IsFilled = r32(NoiseValue) > r32(WorldZSubZMin) ;

        v3 ThisColor = RGB_DIRT;

        u8 ThisTransparency = 0;

        s32 SandThreshold   = 3;
        s32 GravelThreshold = 1;
        s32 WaterThreshold  = 0;

        r32 StoneThresh = r32(WorldZSubZMin) + 2.f;
        /* r32 DirtThresh = r32(WorldZSubZMin) + 1.f; */

        /* if (IsFilled) */
        {
          ThisColor = RGB_GRASS_GREEN;

          if (NoiseValue > StoneThresh)
          {
            ThisColor = RGB_DIRT;
          }
        }

        Assert(TerraceMask >= 0.f);
        Assert(TerraceMask <= 1.1f);
        /* GrowGrassPerlin( Chunk, V3i(x,y,z), NoiseValue, 0.7f-TerraceMask, SrcToDest, WorldChunkDim, WorldZSubZMin, &ThisColor, &IsFilled ); */
        GrowGrassPerlin( Chunk, V3i(x,y,z), NoiseValue, 1.f-Clamp01(TerraceMask), SrcToDest, WorldChunkDim, WorldZSubZMin, &ThisColor, &IsFilled );
        /* GrowGrassPerlin( Chunk, V3i(x,y,z), NoiseValue, Clamp01(TerraceMask), SrcToDest, WorldChunkDim, WorldZSubZMin, &ThisColor, &IsFilled ); */

        /* SetFlag(&Chunk->Voxels[VoxIndex], (voxel_flag)(Voxel_Filled*IsFilled)); */
        SetOccupancyBit(Chunk, VoxIndex, s32(IsFilled));
        Chunk->Voxels[VoxIndex].Color = RGBtoPackedHSV(ThisColor)*u8(IsFilled);
        Chunk->Voxels[VoxIndex].Transparency = ThisTransparency;
        ChunkSum += IsFilled;

        NotImplemented;
        /* Assert( (Chunk->Voxels[VoxIndex].Flags&VoxelFaceMask) == 0); */
      }
    }
  }

#if 0
  RangeIterator(z, (s32)Chunk->Dim.z)
  RangeIterator(y, (s32)Chunk->Dim.y)
  RangeIterator(x, (s32)Chunk->Dim.x)
  {
    voxel *V   = GetVoxel(Chunk, V3i(x,y,z));
    voxel *VUp = TryGetVoxel(Chunk, V3i(x,y,z+1));

    if (VUp && (VUp->Flags & Voxel_Filled) == 0)
    {
      if (V->Flags & Voxel_Filled)
      {
      }
    }
  }
#endif


  return ChunkSum;
}

link_internal u32
Terrain_SinCos( world_chunk *Chunk,
                v3i NoiseBasisOffset,
                void *NoiseParams,
                void *UserData )
{
  TIMED_FUNCTION();

  UNPACK_NOISE_PARAMS(NoiseParams);

  r32 *NoiseValues = Allocate(r32, GetTranArena(), Volume(Dim));
  v3  *Normals     = Allocate( v3, GetTranArena(), Volume(Dim));

  u32 ChunkSum = 0;

  for ( s32 z = 0; z < Dim.z; ++ z)
  {
    s64 WorldZ = (Chunk->DimInChunks.z/2) + (z*Chunk->DimInChunks.z) + (Chunk->WorldP.z*WorldChunkDim.z) + SrcToDest.z ;
    s64 WorldZSubZMin = WorldZ - zMin;
    for ( s32 y = 0; y < Dim.y; ++ y)
    {
      s64 WorldY = (Chunk->DimInChunks.y/2) + (y*Chunk->DimInChunks.y) + SrcToDest.y + (WorldChunkDim.y*Chunk->WorldP.y);
      for ( s32 x = 0; x < Dim.x; ++ x)
      {
        s64 WorldX = (Chunk->DimInChunks.x/2) + (x*Chunk->DimInChunks.x) + SrcToDest.x + (WorldChunkDim.x*Chunk->WorldP.x);
        s32 VoxIndex = GetIndex(V3i(x,y,z), Dim);
        r32 NoiseValue = ((Sin(r32(WorldX)/Period.x)+1.f)/2.f) + ((Cos(r32(WorldY)/Period.y)+1.f)/4.f);

        /* NoiseValue = MapNoiseValueToFinal(NoiseValue); */
        NoiseValue *= Amplitude;

        NoiseValues[VoxIndex] = NoiseValue;
      }
    }
  }


  for ( s32 z = 0; z < Dim.z; ++ z)
  {
    s64 WorldZ = (Chunk->DimInChunks.z/2) + (z*Chunk->DimInChunks.z) + (Chunk->WorldP.z*WorldChunkDim.z) + SrcToDest.z ;
    s64 WorldZSubZMin = WorldZ - zMin;
    for ( s32 y = 0; y < Dim.y; ++ y)
    {
      s64 WorldY = (Chunk->DimInChunks.y/2) + (y*Chunk->DimInChunks.y) + SrcToDest.y + (WorldChunkDim.y*Chunk->WorldP.y);
      for ( s32 x = 0; x < Dim.x; ++ x)
      {
        s64 WorldX = (Chunk->DimInChunks.x/2) + (x*Chunk->DimInChunks.x) + SrcToDest.x + (WorldChunkDim.x*Chunk->WorldP.x);

        s32 VoxIndex = GetIndex(V3i(x,y,z), Dim);
        r32 *NoiseValue = NoiseValues+VoxIndex;
        v3  *Normal     = Normals+VoxIndex;

        v3 ThisColor = RGB_GRASS_GREEN;
        /* MakeCliffs(Chunk, VoxIndex, s32(WorldX), s32(WorldY), s32(WorldZ), NoiseValue, Normal, &ThisColor); */

        b32 IsFilled = r32(*NoiseValue) > r32(WorldZSubZMin);

        /* SetFlag(&Chunk->Voxels[VoxIndex], (voxel_flag)(Voxel_Filled*IsFilled)); */
        SetOccupancyBit(Chunk, VoxIndex, s32(IsFilled));
        Chunk->Voxels[VoxIndex].Color = RGBtoPackedHSV(ThisColor);
        ChunkSum += IsFilled;
      }
    }
  }

  return ChunkSum;
}

link_internal u32
Terrain_Voronoi3D( world_chunk *Chunk,
                           v3i  NoiseBasis,
                          void *NoiseParams,
                          void *UserData )
{
  TIMED_FUNCTION();

  UNPACK_NOISE_PARAMS(NoiseParams);

  u32 ChunkSum = 0;

  s32 MinZ = Chunk->WorldP.z*WorldChunkDim.z;
  s32 MaxZ = MinZ+WorldChunkDim.z;

  voronoi_noise_params *VoronoiParams = Cast(voronoi_noise_params*, UserData);

  r32 Squareness = VoronoiParams ? VoronoiParams->Squareness : 0.f;
  r32 MaskChance = VoronoiParams ? VoronoiParams->MaskChance : 0.f;

  for ( s32 z = 0; z < Dim.z; ++ z)
  {
    for ( s32 y = 0; y < Dim.y; ++ y)
    {
      for ( s32 x = 0; x < Dim.x; ++ x)
      {
        s32 VoxIndex = GetIndex(V3i(x,y,z), Dim);
        NotImplemented;
        /* Chunk->Voxels[VoxIndex].Flags = Voxel_Empty; */

        v3 RepeatBasisInCells = V3(8);
        v3  NoiseInput = NoiseMod((V3(x,y,z) + NoiseBasis)/Period, RepeatBasisInCells);

        r32 NoiseValue = VoronoiNoise3D_Tiled(NoiseInput, RepeatBasisInCells, Squareness, MaskChance);
        NoiseValue = Clamp01(NoiseValue);
        NoiseValue *= Amplitude;

        b32 IsFilled = r32(NoiseValue) > r32(zMin) ;

        /* SetFlag(&Chunk->Voxels[VoxIndex], (voxel_flag)(Voxel_Filled*IsFilled)); */
        SetOccupancyBit(Chunk, VoxIndex, s32(IsFilled));
        Chunk->Voxels[VoxIndex].Color = PackHSVColor(RGBColor)*u8(IsFilled);
        ChunkSum += IsFilled;
      }
    }
  }
  return ChunkSum;
}

link_internal u32
Terrain_Voronoi2D( world_chunk *Chunk,
                           v3i  NoiseBasis,
                          void *NoiseParams,
                          void *UserData )
{
  TIMED_FUNCTION();

  UNPACK_NOISE_PARAMS(NoiseParams);

  u32 ChunkSum = 0;

  s32 MinZ = Chunk->WorldP.z*WorldChunkDim.z;
  s32 MaxZ = MinZ+WorldChunkDim.z ;

  voronoi_noise_params *VoronoiParams = Cast(voronoi_noise_params*, UserData);

  r32 Squareness = VoronoiParams ? VoronoiParams->Squareness : 0.f;
  r32 MaskChance = VoronoiParams ? VoronoiParams->MaskChance : 0.f;

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
        s32 VoxIndex = GetIndex(V3i(x,y,z), Dim);
        NotImplemented;
        /* Chunk->Voxels[VoxIndex].Flags = Voxel_Empty; */

        r32 NoiseValue = VoronoiNoise3D(V3(f32(WorldX), f32(WorldY), f32(WorldZ)) / Period, Squareness, MaskChance);
        /* r32 NoiseValue =  VoronoiNoise3D(V3(s32(WorldX), s32(WorldY), s32(WorldZ)) * 0.04f) * 25.f; */
        NoiseValue = Clamp01(NoiseValue);
        /* NoiseValue = MapNoiseValueToFinal(NoiseValue); */ // Crazyballs
        NoiseValue *= Amplitude;

        b32 IsFilled = r32(NoiseValue) > r32(WorldZSubZMin) ;

        /* SetFlag(&Chunk->Voxels[VoxIndex], (voxel_flag)(Voxel_Filled*IsFilled)); */
        SetOccupancyBit(Chunk, VoxIndex, s32(IsFilled));
        Chunk->Voxels[VoxIndex].Color = RGBtoPackedHSV(RGBColor)*u16(IsFilled);
        ChunkSum += IsFilled;
      }
    }
  }
  return ChunkSum;
}
