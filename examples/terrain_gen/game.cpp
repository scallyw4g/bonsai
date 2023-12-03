#define DEBUG_SYSTEM_API 1

#include <bonsai_types.h>

#include <game_constants.h>
#include <game_types.h>

link_internal u32
Checkerboard( perlin_noise *Noise,
              world_chunk *Chunk,
              chunk_dimension Dim,

              chunk_dimension SrcToDest,

              u8 ColorIndex,
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
CustomTerrainExample( perlin_noise *Noise,
                      world_chunk *Chunk,
                      chunk_dimension Dim,
                      chunk_dimension SrcToDest,
                      u8 ColorIndex,

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

          v3 InteriorFreq = Octave->Freq;
          r32 InteriorAmp = Octave->Amp;

          f32 InX = SafeDivide0((x + SrcToDest.x + ( WorldChunkDim.x*Chunk->WorldP.x)), InteriorFreq.x);
          f32 InY = SafeDivide0((y + SrcToDest.y + ( WorldChunkDim.y*Chunk->WorldP.y)), InteriorFreq.y);
          f32 InZ = SafeDivide0((z + SrcToDest.z + ( WorldChunkDim.z*Chunk->WorldP.z)), InteriorFreq.z);

          r32 Warp = PerlinNoise(InX, InY, InZ);
          v3 WarpFactor = Warp*Octave->WarpStrength;

          r32 N = PerlinNoise(InX+WarpFactor.x, InY+WarpFactor.y, InZ+WarpFactor.z);

          Assert(N <= 1.05f);
          Assert(N > -1.05f);

          NoiseValue += (N*InteriorAmp) / OctaveCount;
        }

        b32 NoiseChoice = r64(NoiseValue) > r64(WorldZBiased);

        u8 ThisColor = ColorIndex;

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
        Chunk->Voxels[VoxIndex].Color = ThisColor*u8(NoiseChoice);
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
      }
      else
      {

#if 0
        {
          // Custom FBM noise example generating simple game-world-like terrain
          s32 Frequency = 0; // Ignored
          s32 Amplititude = 0; // Ignored
          s32 StartingZDepth = -200;
          u32 OctaveCount = 2;

          octave_buffer OctaveBuf = { OctaveCount, {} };
          OctaveBuf.Octaves = Allocate(octave, Thread->TempMemory, OctaveCount);

          OctaveBuf.Octaves[0] = {V3(800, 800, 300), 350, V3(1)};
          OctaveBuf.Octaves[1] = {V3(35, 35, 50), 50, V3(3.f)};
          /* OctaveBuf.Octaves[2] = {V3(500, 500, 20), 200, V3(2.f)}; */
          /* OctaveBuf.Octaves[2] = {75, 60, 1}; */
          /* OctaveBuf.Octaves[3] = {37, 30, 0}; */


          /* chunk_init_flags InitFlags = ChunkInitFlag_ComputeStandingSpots; */
          chunk_init_flags InitFlags = ChunkInitFlag_GenMipMapLODs;
          /* chunk_init_flags InitFlags = ChunkInitFlag_Noop; */
          InitializeChunkWithNoise( CustomTerrainExample, Thread, Chunk, Chunk->Dim, 0, Frequency, Amplititude, StartingZDepth, InitFlags, (void*)&OctaveBuf);
        }
#endif

#if 0
        {
          // Custom flat noise function that produces a checkerboard
          s32 Frequency = 0;
          s32 Amplititude = 0;
          s32 StartingZDepth = -1;
          chunk_init_flags InitFlags = ChunkInitFlag_Noop;
          /* chunk_init_flags InitFlags = ChunkInitFlag_GenMipMapLODs; */
          InitializeChunkWithNoise( Checkerboard, Thread, Chunk, Chunk->Dim, 0, Frequency, Amplititude, StartingZDepth, InitFlags, 0);
        }
#endif

#if 0
        {
          // FBM params
          s32 Frequency = 300;
          s32 Amplititude = 220;
          s32 StartingZDepth = -200;
          u32 Octaves = 4;
          /* chunk_init_flags InitFlags = ChunkInitFlag_ComputeStandingSpots; */
          chunk_init_flags InitFlags = ChunkInitFlag_Noop;
          InitializeChunkWithNoise( Noise_FBM2D, Thread, Chunk, Chunk->Dim, 0, Frequency, Amplititude, StartingZDepth, InitFlags, (void*)&Octaves);
        }
#endif

#if 1
        {
          // Perlin 2D Params
          s32 Frequency = 100;
          s32 Amplititude = 5;
          s32 StartingZDepth = 0;
          InitializeChunkWithNoise( Noise_Perlin2D, Thread, Chunk, Chunk->Dim, 0, Frequency, Amplititude, StartingZDepth, ChunkInitFlag_Noop, 0);
        }
#endif

#if 0
        {
          // Perlin 3D Params
          s32 Frequency = 100;
          s32 Amplititude = 5;
          s32 StartingZDepth = 0;
          InitializeChunkWithNoise( Noise_Perlin3D, Thread, Chunk, Chunk->Dim, 0, Frequency, Amplititude, StartingZDepth, ChunkInitFlag_Noop, 0);
        }
#endif

#if 0
        {
          // Flat Params
          s32 Frequency = 100;
          s32 Amplititude = 25;
          s32 StartingZDepth = -1;
          InitializeChunkWithNoise( Noise_Flat, Thread, Chunk, Chunk->Dim, 0, Frequency, Amplititude, StartingZDepth, ChunkInitFlag_Noop, 0);
        }
#endif

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

  StandardCamera(Graphics->Camera, 10000.0f, 1000.0f, CameraTargetP);

  AllocateWorld(World, WorldCenter, WORLD_CHUNK_DIM, g_VisibleRegion);

  World->Flags = WorldFlag_WorldCenterFollowsCameraTarget;

  entity *CameraGhost = GetFreeEntity(EntityTable);
  SpawnEntity( CameraGhost );

  Resources->CameraGhost = CameraGhost;

  GameState = Allocate(game_state, Resources->Memory, 1);
  return GameState;
}


BONSAI_API_MAIN_THREAD_CALLBACK()
{
  Assert(ThreadLocal_ThreadIndex == 0);

  TIMED_FUNCTION();
  UNPACK_ENGINE_RESOURCES(Resources);

  /* DrawFrustum(World, Graphics, Camera); */

  f32 dt = Plat->dt;
  f32 Speed = 80.f;

#if 0
  v3 Offset = GetCameraRelativeInput(Hotkeys, Camera);
  Offset.z = 0; // Constrain to XY plane

  if (Input->E.Pressed) { Offset.z += 1.f; }
  if (Input->Q.Pressed) { Offset.z -= 1.f; }

  Offset = Normalize(Offset);
  /* Camera->ViewingTarget.Offset += Offset; */
  Resources->CameraGhost->P.Offset += Offset * dt * Speed;
#endif
}
