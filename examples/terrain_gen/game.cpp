#define BONSAI_DEBUG_SYSTEM_API 1

#include <bonsai_types.h>

#include "game_constants.h"
#include "game_types.h"

link_internal u32
Checkerboard( perlin_noise *Noise,
              world_chunk *Chunk,

              chunk_dimension Dim,
              chunk_dimension SrcToDest,

              u16 ColorIndex,

              s32 Frequency,
              s32 Amplitude,
              s64 zMin,

              chunk_dimension WorldChunkDim,
              void *Ignored )
{
  u32 Result = 0;

  auto AbsX = Abs(Chunk->WorldP.x);
  auto AbsY = Abs(Chunk->WorldP.y);
  if ( AbsX % 2 == 0 && AbsY % 2 == 1) { ColorIndex = RED; }
  if ( AbsX % 2 == 1 && AbsY % 2 == 0) { ColorIndex = BLUE; }

  for ( s32 z = 0; z < Dim.z; ++ z)
  {
    s64 WorldZ = z - zMin - SrcToDest.z + (WorldChunkDim.z*Chunk->WorldP.z);
    for ( s32 y = 0; y < Dim.y; ++ y)
    {
      for ( s32 x = 0; x < Dim.x; ++ x)
      {
        if (WorldZ < zMin)
        {
          s32 Index = GetIndex(Voxel_Position(x,y,z), Dim);
          Chunk->Voxels[Index].Flags = Voxel_Filled;
          Chunk->Voxels[Index].Color = ColorIndex;
          ++Result;
        }
      }
    }
  }

  return Result;;
}

link_internal u32
GrassyIslandTerrain( perlin_noise *Noise,
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

          NoiseValue += (N*Octave->Amp);
        }

        b32 NoiseChoice = r64(NoiseValue) > r64(WorldZBiased);

        /* if (NoiseValue > WorldZBiased) */
        /* { */
        /* } */

        /* u16 StartColorMin = GREY_4; */
        /* u16 StartColorMax = GREY_6; */
        /* u16 ThisColor = SafeTruncateToU16(umm(RandomBetween(StartColorMin, &GenColorEntropy, StartColorMax))); */
        u16 ThisColor = GRASS_GREEN;

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

#if 1
        GrowGrass( Chunk, V3i(x,y,z), NoiseValue, 1.f, SrcToDest, WorldChunkDim, WorldZBiased, &ThisColor, &NoiseChoice );
#else
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
#if 1
            if (Chunk->Voxels[Below].Flags&Voxel_Filled &&
                Chunk->Voxels[Below].Color == DIRT)
            {
              f32 InX = SafeDivide0((x + SrcToDest.x + ( WorldChunkDim.x*Chunk->WorldP.x)), 10.f);
              f32 InY = SafeDivide0((y + SrcToDest.y + ( WorldChunkDim.y*Chunk->WorldP.y)), 10.f);
              f32 InZ = SafeDivide0((z + SrcToDest.z + ( WorldChunkDim.z*Chunk->WorldP.z)), 10.f);

              u16 GrassColorMin = GRASS_GREEN;
              u16 GrassColorMax = GRASS_GREEN+2;
              ThisColor = SafeTruncateToU16(umm(RandomBetween(GrassColorMin, &GenColorEntropy, GrassColorMax)));
              NoiseChoice = True;

              r32 GrassChance = PerlinNoise(InX, InY, InZ);
              if (GrassChance > 0.45f) { ThisColor = GRASS_GREEN-1; }
            }

            if (Chunk->Voxels[Below].Flags&Voxel_Filled &&
                Chunk->Voxels[Below].Color == GRASS_GREEN-1)
            {
              r32 GrassChance = RandomUnilateral(&GenColorEntropy);
              if (GrassChance > 0.85f)
              {
                NoiseChoice = True;
                ThisColor = GRASS_GREEN-1;
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



#endif
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
#endif

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

link_internal u32
WarpedTerrain( perlin_noise *Noise,
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

          f32 InX = SafeDivide0((x + SrcToDest.x + ( WorldChunkDim.x*Chunk->WorldP.x)), Octave->Freq.x);
          f32 InY = SafeDivide0((y + SrcToDest.y + ( WorldChunkDim.y*Chunk->WorldP.y)), Octave->Freq.y);
          f32 InZ = SafeDivide0((z + SrcToDest.z + ( WorldChunkDim.z*Chunk->WorldP.z)), Octave->Freq.z);

          r32 Warp = PerlinNoise(InX, InY, InZ);
          v3 WarpFactor = Warp*Octave->WarpStrength;

          r32 N = PerlinNoise(InX+WarpFactor.x, InY+WarpFactor.y, InZ+WarpFactor.z);

          Assert(N <= 1.05f);
          Assert(N > -1.05f);

          NoiseValue += (N*Octave->Amp);
        }

        b32 NoiseChoice = r64(NoiseValue) > r64(WorldZBiased);

        u16 ThisColor = ColorIndex;

#if 0
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

        s32 WaterThreshold = 0;
        if (NoiseChoice == False && WorldZ < WaterThreshold)
        {
          NoiseChoice = True;
          ThisColor = BLUE;
        }
#endif

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

BONSAI_API_WORKER_THREAD_INIT_CALLBACK()
{
  Global_ThreadStates = AllThreads;
  SetThreadLocal_ThreadIndex(ThreadIndex);
}

BONSAI_API_WORKER_THREAD_CALLBACK()
{
  if (ThreadLocal_ThreadIndex == INVALID_THREAD_LOCAL_THREAD_INDEX) { SetThreadLocal_ThreadIndex(Thread->Index); }

  b32 Result = True;
  switch (Entry->Type)
  {
    default: { Result = False; } break;

    // NOTE(Jesse): Here we're going to provide an implementation for
    // initializing a world chunk.
    case type_work_queue_entry_init_world_chunk:
    {
      volatile work_queue_entry_init_world_chunk *Job = SafeAccess(work_queue_entry_init_world_chunk, Entry);
      world_chunk *Chunk = Job->Chunk;

      if (ChunkIsGarbage(Chunk))
      {
        // NOTE(Jesse): This is an optimization; the engine marks chunks that
        // have moved outside of the visible region as garbage.
        Chunk->Flags = Chunk_Uninitialized;
      }
      else
      {
        auto GenType = GetEngineResources()->GameState->TerrainGenType;

        auto Ignored = MeshBit_Lod0;

        /* Info("%S", ToString(GenType)); */
        switch (GenType)
        {
          case TerrainGenType_Flat:
          {
            // Flat Params
            s32 Frequency = 100;
            s32 Amplititude = 25;
            s32 StartingZDepth = -1;
            InitializeChunkWithNoise( Noise_Flat, Thread, Chunk, Chunk->Dim, 0, Frequency, Amplititude, StartingZDepth, Ignored, ChunkInitFlag_Noop, 0);
          } break;

          case TerrainGenType_SinCos:
          {
            // Bumpy Sin(x)+Cos(y) noise.  Useful for visualizing the polylines/splines mapping noise values to their final values.
            s32 Frequency = 100;
            s32 Amplititude = 50;
            s32 StartingZDepth = -1;
            chunk_init_flags InitFlags = ChunkInitFlag_Noop;
            InitializeChunkWithNoise( SinCosTerrain, Thread, Chunk, Chunk->Dim, 0, Frequency, Amplititude, StartingZDepth, Ignored, InitFlags, 0);
          } break;

          case TerrainGenType_Voronoi:
          {
            // Voronoi noise .. looks like rocks.
            s32 Frequency = 100;
            s32 Amplititude = 50;
            s32 StartingZDepth = -1;
            chunk_init_flags InitFlags = ChunkInitFlag_Noop;
            InitializeChunkWithNoise( VoronoiTerrain, Thread, Chunk, Chunk->Dim, 0, Frequency, Amplititude, StartingZDepth, Ignored, InitFlags, 0);
          } break;

          case TerrainGenType_Checkerboard:
          {
            // Custom flat noise function that produces a checkerboard
            s32 Frequency = 0;
            s32 Amplititude = 0;
            s32 StartingZDepth = -1;
            chunk_init_flags InitFlags = ChunkInitFlag_Noop;
            /* chunk_init_flags InitFlags = ChunkInitFlag_GenMipMapLODs; */
            InitializeChunkWithNoise( Checkerboard, Thread, Chunk, Chunk->Dim, 0, Frequency, Amplititude, StartingZDepth, Ignored, InitFlags, 0);
          } break;

          case TerrainGenType_Perlin2D:
          {
            // Perlin 2D Params
            s32 Frequency = 100;
            s32 Amplititude = 5;
            s32 StartingZDepth = 0;
            InitializeChunkWithNoise( Noise_Perlin2D, Thread, Chunk, Chunk->Dim, 0, Frequency, Amplititude, StartingZDepth, MeshBit_Lod0, ChunkInitFlag_Noop, 0);
          } break;

          case TerrainGenType_Perlin3D:
          {
            // Perlin 3D Params
            s32 Frequency = 100;
            s32 Amplititude = 5;
            s32 StartingZDepth = 0;
            InitializeChunkWithNoise( Noise_Perlin3D, Thread, Chunk, Chunk->Dim, 0, Frequency, Amplititude, StartingZDepth, MeshBit_Lod0, ChunkInitFlag_Noop, 0);
          } break;


          case TerrainGenType_FBM2D:
          {
            // FBM params
            s32 Frequency = 300;
            s32 Amplititude = 220;
            s32 StartingZDepth = -200;
            u32 Octaves = 4;
            /* chunk_init_flags InitFlags = ChunkInitFlag_ComputeStandingSpots; */
            chunk_init_flags InitFlags = ChunkInitFlag_Noop;
            InitializeChunkWithNoise( Noise_FBM2D, Thread, Chunk, Chunk->Dim, 0, Frequency, Amplititude, StartingZDepth, Ignored, InitFlags, (void*)&Octaves);
          } break;

          case TerrainGenType_GrassyTerracedTerrain:
          {
            // Custom FBM noise example generating slightly-more-complex game-world-like terrain
            s32 Frequency = 0; // Ignored
            s32 Amplititude = 0; // Ignored
            s32 StartingZDepth = -100;
            u32 OctaveCount = 1;

            octave_buffer OctaveBuf = { OctaveCount, {} };
            OctaveBuf.Octaves = Allocate(octave, Thread->TempMemory, OctaveCount);

            OctaveBuf.Octaves[0] = {V3(400, 400, 200), 150, V3(1)};
            /* OctaveBuf.Octaves[1] = {V3(35, 35, 50),      6, V3(2.f)}; */
            /* OctaveBuf.Octaves[2] = {V3(500, 500, 20), 200, V3(2.f)}; */
            /* OctaveBuf.Octaves[2] = {75, 60, 1}; */
            /* OctaveBuf.Octaves[3] = {37, 30, 0}; */


            /* chunk_init_flags InitFlags = ChunkInitFlag_ComputeStandingSpots; */
            /* chunk_init_flags InitFlags = ChunkInitFlag_GenMipMapLODs; */
            chunk_init_flags InitFlags = ChunkInitFlag_Noop;
            InitializeChunkWithNoise( GrassyTerracedTerrain, Thread, Chunk, Chunk->Dim, 0, Frequency, Amplititude, StartingZDepth, MeshBit_Lod0, InitFlags, (void*)&OctaveBuf);
          } break;

          case TerrainGenType_GrassyTerracedTerrain2:
          {
            // Custom FBM noise example generating slightly-more-complex game-world-like terrain
            s32 Frequency = 0; // Ignored
            s32 Amplititude = 0; // Ignored
            s32 StartingZDepth = -100;
            u32 OctaveCount = 1;

            octave_buffer OctaveBuf = { OctaveCount, {} };
            OctaveBuf.Octaves = Allocate(octave, Thread->TempMemory, OctaveCount);

            OctaveBuf.Octaves[0] = {V3(400, 400, 200), 150, V3(1)};
            /* OctaveBuf.Octaves[1] = {V3(35, 35, 50),     60, V3(2.f)}; */
            /* OctaveBuf.Octaves[2] = {V3(500, 500, 20), 200, V3(2.f)}; */
            /* OctaveBuf.Octaves[2] = {75, 60, 1}; */
            /* OctaveBuf.Octaves[3] = {37, 30, 0}; */


            /* chunk_init_flags InitFlags = ChunkInitFlag_ComputeStandingSpots; */
            /* chunk_init_flags InitFlags = ChunkInitFlag_GenMipMapLODs; */
            chunk_init_flags InitFlags = ChunkInitFlag_Noop;
            InitializeChunkWithNoise( GrassyTerracedTerrain2, Thread, Chunk, Chunk->Dim, 0, Frequency, Amplititude, StartingZDepth, MeshBit_Lod0, InitFlags, (void*)&OctaveBuf);
          } break;


          case TerrainGenType_GrassyTerracedTerrain3:
          {
            // Custom FBM noise example generating slightly-more-complex game-world-like terrain
            s32 Frequency = 0; // Ignored
            s32 Amplititude = 0; // Ignored
            s32 StartingZDepth = -100;
            u32 OctaveCount = 2;

            octave_buffer OctaveBuf = { OctaveCount, {} };
            OctaveBuf.Octaves = Allocate(octave, Thread->TempMemory, OctaveCount);

            OctaveBuf.Octaves[0] = {V3(1400, 1400, 800), 350, V3(1.f)};
            OctaveBuf.Octaves[1] = {V3(400, 400, 200),   150, V3(1.f)};
            /* OctaveBuf.Octaves[2] = {V3(35, 35, 50),        6, V3(2.f)}; */
            /* OctaveBuf.Octaves[2] = {V3(500, 500, 20), 200, V3(2.f)}; */
            /* OctaveBuf.Octaves[2] = {75, 60, 1}; */
            /* OctaveBuf.Octaves[3] = {37, 30, 0}; */


            /* chunk_init_flags InitFlags = ChunkInitFlag_ComputeStandingSpots; */
            /* chunk_init_flags InitFlags = ChunkInitFlag_GenMipMapLODs; */
            chunk_init_flags InitFlags = ChunkInitFlag_Noop;
            InitializeChunkWithNoise( GrassyTerracedTerrain3, Thread, Chunk, Chunk->Dim, 0, Frequency, Amplititude, StartingZDepth, MeshBit_Lod0, InitFlags, (void*)&OctaveBuf);
          } break;

          case TerrainGenType_GrassyTerracedTerrain4:
          {
            // Custom FBM noise example generating slightly-more-complex game-world-like terrain
            s32 Frequency = 0; // Ignored
            s32 Amplititude = 0; // Ignored
            s32 StartingZDepth = -100;
            u32 OctaveCount = 2;

            octave_buffer OctaveBuf = { OctaveCount, {} };
            OctaveBuf.Octaves = Allocate(octave, Thread->TempMemory, OctaveCount);

            OctaveBuf.Octaves[0] = {V3(1400, 1400, 800), 350, V3(1.f)};
            OctaveBuf.Octaves[1] = {V3(400, 400, 200),   150, V3(1.f)};
            /* OctaveBuf.Octaves[2] = {V3(35, 35, 50),        6, V3(2.f)}; */
            /* OctaveBuf.Octaves[2] = {V3(500, 500, 20), 200, V3(2.f)}; */
            /* OctaveBuf.Octaves[2] = {75, 60, 1}; */
            /* OctaveBuf.Octaves[3] = {37, 30, 0}; */

            /* chunk_init_flags InitFlags = ChunkInitFlag_ComputeStandingSpots; */
            /* chunk_init_flags InitFlags = ChunkInitFlag_GenMipMapLODs; */
            chunk_init_flags InitFlags = ChunkInitFlag_Noop;
            InitializeChunkWithNoise( GrassyTerracedTerrain4, Thread, Chunk, Chunk->Dim, 0, Frequency, Amplititude, StartingZDepth, MeshBit_Lod0, InitFlags, (void*)&OctaveBuf);
          } break;


          case TerrainGenType_TerracedTerrain:
          {
            // Custom FBM noise example generating slightly-more-complex game-world-like terrain
            s32 Frequency = 0; // Ignored
            s32 Amplititude = 0; // Ignored
            s32 StartingZDepth = -100;
            u32 OctaveCount = 1;

            octave_buffer OctaveBuf = { OctaveCount, {} };
            OctaveBuf.Octaves = Allocate(octave, Thread->TempMemory, OctaveCount);

            OctaveBuf.Octaves[0] = {V3(400, 400, 200), 150, V3(1)};
            /* OctaveBuf.Octaves[1] = {V3(35, 35, 50),      6, V3(2.f)}; */
            /* OctaveBuf.Octaves[2] = {V3(500, 500, 20), 200, V3(2.f)}; */
            /* OctaveBuf.Octaves[2] = {75, 60, 1}; */
            /* OctaveBuf.Octaves[3] = {37, 30, 0}; */


            /* chunk_init_flags InitFlags = ChunkInitFlag_ComputeStandingSpots; */
            /* chunk_init_flags InitFlags = ChunkInitFlag_GenMipMapLODs; */
            chunk_init_flags InitFlags = ChunkInitFlag_Noop;
            InitializeChunkWithNoise( TerracedTerrain, Thread, Chunk, Chunk->Dim, 0, Frequency, Amplititude, StartingZDepth, MeshBit_Lod0, InitFlags, (void*)&OctaveBuf);
          } break;

          case TerrainGenType_GrassyLargeTerracedTerrain:
          {
            // Custom FBM noise example generating slightly-more-complex game-world-like terrain
            s32 Frequency = 0; // Ignored
            s32 Amplititude = 0; // Ignored
            s32 StartingZDepth = -140;
            u32 OctaveCount = 4;

            octave_buffer OctaveBuf = { OctaveCount, {} };
            OctaveBuf.Octaves = Allocate(octave, Thread->TempMemory, OctaveCount);

            OctaveBuf.Octaves[0] = {V3(1200, 1200, 900), 500, V3(1)};
            OctaveBuf.Octaves[1] = {V3(1200, 1200, 900), 500, V3(1)};
            OctaveBuf.Octaves[2] = {V3(800, 800, 500), 125, V3(1)};
            OctaveBuf.Octaves[3] = {V3(35, 35, 25), 5, V3(2.f)};
            /* OctaveBuf.Octaves[2] = {V3(500, 500, 20), 200, V3(2.f)}; */
            /* OctaveBuf.Octaves[2] = {75, 60, 1}; */
            /* OctaveBuf.Octaves[3] = {37, 30, 0}; */


            /* chunk_init_flags InitFlags = ChunkInitFlag_ComputeStandingSpots; */
            chunk_init_flags InitFlags = ChunkInitFlag_GenLODs;
            /* chunk_init_flags InitFlags = ChunkInitFlag_Noop; */
            InitializeChunkWithNoise( GrassyLargeTerracedTerrain, Thread, Chunk, Chunk->Dim, 0, Frequency, Amplititude, StartingZDepth, MeshBit_Lod0, InitFlags, (void*)&OctaveBuf);
          } break;

          case TerrainGenType_GrassyIsland:
          {
            // Custom FBM noise example generating slightly-more-complex game-world-like terrain
            s32 Frequency = 0; // Ignored
            s32 Amplititude = 0; // Ignored
            s32 StartingZDepth = -140;
            u32 OctaveCount = 2;

            octave_buffer OctaveBuf = { OctaveCount, {} };
            OctaveBuf.Octaves = Allocate(octave, Thread->TempMemory, OctaveCount);

            OctaveBuf.Octaves[0] = {V3(800, 800, 300), 275, V3(1)};
            OctaveBuf.Octaves[1] = {V3(35, 35, 50), 25, V3(2.f)};
            /* OctaveBuf.Octaves[2] = {V3(500, 500, 20), 200, V3(2.f)}; */
            /* OctaveBuf.Octaves[2] = {75, 60, 1}; */
            /* OctaveBuf.Octaves[3] = {37, 30, 0}; */


            /* chunk_init_flags InitFlags = ChunkInitFlag_ComputeStandingSpots; */
            /* chunk_init_flags InitFlags = ChunkInitFlag_GenMipMapLODs; */
            chunk_init_flags InitFlags = ChunkInitFlag_Noop;
            InitializeChunkWithNoise( GrassyIslandTerrain, Thread, Chunk, Chunk->Dim, 0, Frequency, Amplititude, StartingZDepth, MeshBit_Lod0, InitFlags, (void*)&OctaveBuf);
          } break;

          case TerrainGenType_Hoodoo:
          {
            s32 Frequency = 300;
            s32 Amplititude = 220;
            s32 StartingZDepth = 70;
            u32 OctaveCount = 2;

            octave_buffer OctaveBuf = { OctaveCount, {} };
            OctaveBuf.Octaves = Allocate(octave, Thread->TempMemory, OctaveCount);

            OctaveBuf.Octaves[0] = {V3(350, 150, 50), 25, V3(1.f)};
            OctaveBuf.Octaves[1] = {V3(120, 60, 35),  7.5, V3(1.f)};
            /* OctaveBuf.Octaves[1] = {V3(90,  60, 35),  25, V3(1.f)}; */

            chunk_init_flags InitFlags = ChunkInitFlag_Noop;
            /* chunk_init_flags InitFlags    = ChunkInitFlag_GenMipMapLODs; */
            /* chunk_init_flags InitFlags = chunk_init_flags(ChunkInitFlag_ComputeStandingSpots | ChunkInitFlag_GenMipMapLODs); */
            InitializeChunkWithNoise( HoodooTerrain, Thread, Chunk, Chunk->Dim, 0, Frequency, Amplititude, StartingZDepth, MeshBit_Lod0, InitFlags, (void*)&OctaveBuf);
          } break;

          case TerrainGenType_Warped:
          {
            // Custom FBM noise example generating highly domain-warped terrain
            s32 Frequency = 0; // Ignored
            s32 Amplititude = 0; // Ignored
            s32 StartingZDepth = -200;
            u32 OctaveCount = 2;

            octave_buffer OctaveBuf = { OctaveCount, {} };
            OctaveBuf.Octaves = Allocate(octave, Thread->TempMemory, OctaveCount);

            OctaveBuf.Octaves[0] = {V3(800, 800, 300), 350, V3(1)};
            OctaveBuf.Octaves[1] = {V3(35, 35, 50), 20, V3(3.f)};
            /* OctaveBuf.Octaves[2] = {V3(500, 500, 20), 200, V3(2.f)}; */
            /* OctaveBuf.Octaves[2] = {V3(500, 500, 20), 200, V3(2.f)}; */
            /* OctaveBuf.Octaves[2] = {75, 60, 1}; */
            /* OctaveBuf.Octaves[3] = {37, 30, 0}; */


            /* chunk_init_flags InitFlags = ChunkInitFlag_ComputeStandingSpots; */
            /* chunk_init_flags InitFlags = ChunkInitFlag_GenMipMapLODs; */
            chunk_init_flags InitFlags = ChunkInitFlag_Noop;
            InitializeChunkWithNoise( WarpedTerrain, Thread, Chunk, Chunk->Dim, 0, Frequency, Amplititude, StartingZDepth, Ignored, InitFlags, (void*)&OctaveBuf);
          } break;


        }

      }

      FinalizeChunkInitialization(Chunk);
    } break;
  }

  return Result;
}

BONSAI_API_MAIN_THREAD_INIT_CALLBACK()
{
  UNPACK_ENGINE_RESOURCES(Resources);

  Global_AssetPrefixPath = CSz("examples/terrain_gen/assets");

  world_position WorldCenter = {};
  canonical_position CameraTargetP = {};

  StandardCamera(Graphics->Camera, 10000.0f, 5000.0f, DEFAULT_CAMERA_BLENDING, CameraTargetP);

  AllocateWorld(World, WorldCenter, WORLD_CHUNK_DIM, g_VisibleRegion);

  GameState = Allocate(game_state, Resources->Memory, 1);

  /* GameState->TerrainGenType = TerrainGenType_GrassyTerracedTerrain; */

  GameState->TerrainGenType = TerrainGenType_GrassyLargeTerracedTerrain;
  /* World->Center = V3i(-22, 101, 1); */

  Camera->GhostId = GetFreeEntity(EntityTable);
  entity *CameraGhost = GetEntity(EntityTable, Camera->GhostId);
  /* CameraGhost->P.WorldP = V3i(-53, -93, 2); */ 
  CameraGhost->P.WorldP = V3i(-5, -121, 2); 
  CameraGhost->Behavior = entity_behavior_flags(CameraGhost->Behavior|EntityBehaviorFlags_DefatulCameraGhostBehavior|EntityBehaviorFlags_WorldCenter);

  SpawnEntity(CameraGhost);

  return GameState;
}


BONSAI_API_MAIN_THREAD_CALLBACK()
{
  Assert(ThreadLocal_ThreadIndex == 0);

  TIMED_FUNCTION();
  UNPACK_ENGINE_RESOURCES(Resources);

  f32 dt = Plat->dt;
  f32 Speed = 80.f;

  global_variable window_layout Window = WindowLayout("Terrain Gen", WindowLayoutFlag_StartupAlign_Right);

  PushWindowStart(Ui, &Window);
    ui_toggle_button_group TerrainGenTypeRadio = RadioButtonGroup_terrain_gen_type(Ui, &Window, "terrain_gen", ToggleButtonGroupFlags_DrawVertical);
  PushWindowEnd(Ui, &Window);

  if (TerrainGenTypeRadio.AnyElementClicked)
  {
    GetRadioEnum(&TerrainGenTypeRadio, &GameState->TerrainGenType);
    SignalAndWaitForWorkers(&Plat->WorkerThreadsSuspendFutex);
    HardResetWorld(Resources, HardResetFlag_NoResetCamera);
    UnsignalFutex(&Plat->WorkerThreadsSuspendFutex);
  }
}

BONSAI_API_ON_LIBRARY_RELOAD()
{
  UNPACK_ENGINE_RESOURCES(Resources);

  // NOTE(Jesse): Engine suspends workers for us here.
  /* SignalAndWaitForWorkers(&Plat->WorkerThreadsSuspendFutex); */
  HardResetWorld(Resources, HardResetFlag_NoResetCamera);

#if 0
  if (entity *CameraGhost = GetEntity(EntityTable, Camera->GhostId))
  {
    CameraGhost->P.WorldP = V3i(-22, 101, 1);
    SpawnEntity(CameraGhost);
  }
  else
  {
    Camera->GhostId = GetFreeEntity(EntityTable);
    CameraGhost = GetEntity(EntityTable, Camera->GhostId);
    CameraGhost->P.WorldP = V3i(-22, 101, 1); 
    SpawnEntity(CameraGhost);
  }
#endif

  /* UnsignalFutex(&Plat->WorkerThreadsSuspendFutex); */
}
