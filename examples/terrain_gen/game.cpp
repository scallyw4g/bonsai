#define PLATFORM_GL_IMPLEMENTATIONS 1
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
                      s32 Frequency,
                      s32 Amplitude,
                      s64 zMin,
                      chunk_dimension WorldChunkDim,
                      void *OctaveCount )
{
  TIMED_FUNCTION();
  Assert(Frequency != s32_MIN);

  u32 ChunkSum = 0;

  s32 MinZ = Chunk->WorldP.z*WorldChunkDim.z;
  s32 MaxZ = MinZ+WorldChunkDim.z ;

  if (MaxZ < -Amplitude)
  {
    s32 MaxIndex = Volume(Dim);
    for ( s32 VoxIndex = 0; VoxIndex < MaxIndex; ++VoxIndex)
    {
      Chunk->Voxels[VoxIndex].Flags = Voxel_Filled;
      Chunk->Voxels[VoxIndex].Color = ColorIndex;
    }
    return (u32)MaxIndex;
  }

  if (MinZ > Amplitude)
    return ChunkSum;

  Frequency = Max(Frequency, 1);
  Assert(Frequency != s32_MIN);

  u32 Octaves = *(u32*)OctaveCount;
  for ( s32 z = 0; z < Dim.z; ++ z)
  {
    s64 WorldZ = z - SrcToDest.z + (WorldChunkDim.z*Chunk->WorldP.z);
    s64 WorldZBiased = WorldZ - zMin;
    for ( s32 y = 0; y < Dim.y; ++ y)
    {
      for ( s32 x = 0; x < Dim.x; ++ x)
      {
        r32 NoiseValue = 0.f;
        s32 VoxIndex = GetIndex(Voxel_Position(x,y,z), Dim);
        Chunk->Voxels[VoxIndex].Flags = Voxel_Empty;
        Assert( NotSet(&Chunk->Voxels[VoxIndex], Voxel_Filled) );

        Assert(Frequency != s32_MIN);
        s32 InteriorFreq = Frequency;
        s32 InteriorAmp = Amplitude;
        for (u32 OctaveIndex = 0; OctaveIndex < Octaves; ++OctaveIndex)
        {

          f32 InX = SafeDivide0((x + SrcToDest.x + ( WorldChunkDim.x*Chunk->WorldP.x)), f32(InteriorFreq));
          f32 InY = SafeDivide0((y + SrcToDest.y + ( WorldChunkDim.y*Chunk->WorldP.y)), f32(InteriorFreq));
          f32 InZ = SafeDivide0((z + SrcToDest.z + ( WorldChunkDim.z*Chunk->WorldP.z)), f32(InteriorFreq));
          /* f32 InZ = 1.0; */

          r32 WarpFactor = 4.f;

          r32 Warp = 0.f;
          Warp = PerlinNoise(InX, InY, InZ)*WarpFactor;
          /* Warp = PerlinNoise(InX, InY, InZ)*r32(OctaveIndex); */
          /* Warp2 = Noise->noise(InX, InY+Warp, InZ)*8.f; */

          r32 N = PerlinNoise(InX+Warp, InY+Warp, InZ+Warp);
          /* r32 N = Noise->noise(Warp, Warp, Warp); */
          Assert(N <= 1.05f);
          Assert(N > -1.05f);
          /* r32 NoiseValue = Noise->noise(InX, InY, InZ); */

          /* b32 NoiseChoice = IsUnderground && NoiseValue > 0.20f; */
          /* s32 zValue = (s32)Abs( (N*InteriorAmp) ); */
          s32 zValue =  s32(N*InteriorAmp);

          b32 IsUnderground =  zValue < WorldZBiased;
          /* b32 NoiseChoice = IsUnderground && N > 0.5f; */
          b32 NoiseChoice = IsUnderground;

          /* if (NoiseChoice) */
          {
            /* NoiseValue += N*(r32(OctaveIndex+1)); */
            NoiseValue += N*InteriorAmp;
            /* NoiseValue += N; */
          }

          InteriorAmp = Max(1, InteriorAmp/2);
          InteriorFreq = Max(1, InteriorFreq/2);
        }

        /* NoiseValue /= r32(Octaves+Octaves); */
        /* Assert(NoiseValue <= 1.10f); */

        /* b32 NoiseChoice = NoiseValue > 0.5f;; */
        /* b32 NoiseChoice = NoiseValue > r32(Amplitude); //0.5f;; */
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

        s32 WaterThreshold = 0;
        if (NoiseChoice == False && WorldZ < WaterThreshold)
        {
          NoiseChoice = True;
          ThisColor = BLUE;
        }

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

// NOTE(Jesse): This function gets called for each worker thread at engine
// startup, but not the main thread!
BONSAI_API_WORKER_THREAD_INIT_CALLBACK()
{
  Global_ThreadStates = AllThreads;
  SetThreadLocal_ThreadIndex(ThreadIndex);
}

// NOTE(Jesse): This is the worker thread loop.  These are a few default
// implementations of functions for copying data around in the engine.
//
// TODO(Jesse): Make these jobs opt-in, such that game code doesn't have to
// care about these unless it wants to.  Most of these jobs will never have a
// custom implementation in games until way into development, if ever.
//
BONSAI_API_WORKER_THREAD_CALLBACK()
{
  switch (Entry->Type)
  {
    // NOTE(Jesse): A noop entry is a bug; InvalidCodePath() crashes the process
    // in debug mode, and does nothing in release mode (in the hopes we handle
    // whatever else happens gracefully).
    case type_work_queue_entry_noop: { InvalidCodePath(); } break;

    case type_work_queue_entry_update_world_region:
    case type_work_queue_entry_rebuild_mesh:
    case type_work_queue_entry_init_asset:
    {
      NotImplemented;
    } break;

    case type_work_queue_entry_init_world_chunk:
     {
       volatile work_queue_entry_init_world_chunk *Job = SafeAccess(work_queue_entry_init_world_chunk, Entry);
       world_chunk *Chunk = Job->Chunk;

       if (ChunkIsGarbage(Chunk))
       {
       }
       else
       {

         {
           // Custom FBM noise example generating simple game-world-like terrain
           s32 Frequency = 300;
           s32 Amplititude = 220;
           s32 StartingZDepth = -200;
           u32 Octaves = 4;
           /* chunk_init_flags InitFlags = ChunkInitFlag_ComputeStandingSpots; */
           chunk_init_flags InitFlags = ChunkInitFlag_GenMipMapLODs;
           /* chunk_init_flags InitFlags = ChunkInitFlag_Noop; */
           InitializeChunkWithNoise( CustomTerrainExample, Thread, Chunk, Chunk->Dim, 0, Frequency, Amplititude, StartingZDepth, InitFlags, (void*)&Octaves);
         }

         /* { */
         /*   // Custom flat noise function that produces a checkerboard */
         /*   s32 Frequency = 0; */
         /*   s32 Amplititude = 0; */
         /*   s32 StartingZDepth = -1; */
         /*   chunk_init_flags InitFlags = ChunkInitFlag_Noop; */
         /*   /1* chunk_init_flags InitFlags = ChunkInitFlag_GenMipMapLODs; *1/ */
         /*   InitializeChunkWithNoise( Checkerboard, Thread, Chunk, Chunk->Dim, 0, Frequency, Amplititude, StartingZDepth, InitFlags, 0); */
         /* } */

         /* { */
         /*   // FBM params */
         /*   s32 Frequency = 300; */
         /*   s32 Amplititude = 220; */
         /*   s32 StartingZDepth = -200; */
         /*   u32 Octaves = 4; */
         /*   /1* chunk_init_flags InitFlags = ChunkInitFlag_ComputeStandingSpots; *1/ */
         /*   chunk_init_flags InitFlags = ChunkInitFlag_Noop; */
         /*   InitializeChunkWithNoise( Noise_FBM2D, Thread, Chunk, Chunk->Dim, 0, Frequency, Amplititude, StartingZDepth, InitFlags, (void*)&Octaves); */
         /* } */

         /* { */
         /*   // Perlin 2D Params */
         /*   s32 Frequency = 100; */
         /*   s32 Amplititude = 25; */
         /*   s32 StartingZDepth = 0; */
         /*   InitializeChunkWithNoise( Noise_Perlin2D, Thread, Chunk, Chunk->Dim, 0, Frequency, Amplititude, StartingZDepth, ChunkInitFlag_Noop, 0); */
         /* } */

         /* { */
         /*   // Perlin 3D Params */
         /*   s32 Frequency = 100; */
         /*   s32 Amplititude = 25; */
         /*   s32 StartingZDepth = 0; */
         /*   InitializeChunkWithNoise( Noise_Perlin3D, Thread, Chunk, Chunk->Dim, 0, Frequency, Amplititude, StartingZDepth, ChunkInitFlag_Noop, 0); */
         /* } */

         /* { */
         /*   // Flat Params */
         /*   s32 Frequency = 100; */
         /*   s32 Amplititude = 25; */
         /*   s32 StartingZDepth = -1; */
         /*   InitializeChunkWithNoise( Noise_Flat, Thread, Chunk, Chunk->Dim, 0, Frequency, Amplititude, StartingZDepth, ChunkInitFlag_Noop, 0); */
         /* } */

       }

       FinalizeChunkInitialization(Chunk);
     } break;

    case type_work_queue_entry_copy_buffer_ref:
    {
      work_queue_entry_copy_buffer_ref *CopyJob = SafeAccess(work_queue_entry_copy_buffer_ref, Entry);
      DoCopyJob(CopyJob, &Thread->EngineResources->MeshFreelist, Thread->PermMemory);
    } break;

    case type_work_queue_entry_copy_buffer:
    {
      volatile work_queue_entry_copy_buffer *CopyJob = SafeAccess(work_queue_entry_copy_buffer, Entry);
      DoCopyJob(CopyJob, &Thread->EngineResources->MeshFreelist, Thread->PermMemory);
    } break;

    case type_work_queue_entry_copy_buffer_set:
    {
      volatile work_queue_entry_copy_buffer_set *CopySet = SafeAccess(work_queue_entry_copy_buffer_set, Entry);
      RangeIterator(CopyIndex, (s32)CopySet->Count)
      {
        volatile work_queue_entry_copy_buffer *CopyJob = &CopySet->CopyTargets[CopyIndex];
        DoCopyJob(CopyJob, &Thread->EngineResources->MeshFreelist, Thread->PermMemory);
      }
    } break;

    case type_work_queue_entry_sim_particle_system:
    {
      work_queue_entry_sim_particle_system *Job = SafeAccess(work_queue_entry_sim_particle_system, Entry);
      SimulateParticleSystem(Job);
    } break;
  }
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

  entity *CameraTarget = GetFreeEntity(EntityTable);
  SpawnEntity( CameraTarget );

  Resources->CameraTarget = CameraTarget;

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

  v3 Offset = GetCameraRelativeInput(Hotkeys, Camera);
  Offset.z = 0; // Constraint to XY plane

  if (Input->E.Pressed) { Offset.z += 1.f; }
  if (Input->Q.Pressed) { Offset.z -= 1.f; }

  Offset = Normalize(Offset);
  Resources->CameraTarget->P.Offset += Offset * dt * Speed;
}
