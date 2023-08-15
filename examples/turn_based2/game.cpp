#define PLATFORM_GL_IMPLEMENTATIONS 1
#define DEBUG_SYSTEM_API 1

#include <bonsai_types.h>

#include <game_constants.h>
#include <game_types.h>

link_internal u32
HoodooTerrain( perlin_noise *Noise,
                      world_chunk *Chunk,
                      chunk_dimension Dim,
                      chunk_dimension SrcToDest,
                      u8 ColorIndex,
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
             {V3(15, 15, 300), 60, V3(1.f)},
             {V3(50, 50, 3),   18, V3(1.f)},
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
    InvalidCase(type_work_queue_entry_noop);

    InvalidCase(type_work_queue_entry__align_to_cache_line_helper);

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
           s32 Frequency = 300;
           s32 Amplititude = 220;
           s32 StartingZDepth = -100;
           u32 OctaveCount = 2;

           octave_buffer OctaveBuf = { OctaveCount, {} };
           OctaveBuf.Octaves = Allocate(octave, Thread->TempMemory, OctaveCount);

           OctaveBuf.Octaves[0] = {V3(350, 150, 50), 50, V3(1.f)};
           OctaveBuf.Octaves[1] = {V3(120, 60, 35),  15, V3(1.f)};
           /* OctaveBuf.Octaves[1] = {V3(90,  60, 35),  25, V3(1.f)}; */

           /* chunk_init_flags InitFlags = ChunkInitFlag_Noop; */
           chunk_init_flags InitFlags    = ChunkInitFlag_GenMipMapLODs;
           /* chunk_init_flags InitFlags = chunk_init_flags(ChunkInitFlag_ComputeStandingSpots | ChunkInitFlag_GenMipMapLODs); */
           InitializeChunkWithNoise( HoodooTerrain, Thread, Chunk, Chunk->Dim, 0, Frequency, Amplititude, StartingZDepth, InitFlags, (void*)&OctaveBuf);
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

    case type_work_queue_entry_copy_buffer_set:
    {
      volatile work_queue_entry_copy_buffer_set *CopySet = SafeAccess(work_queue_entry_copy_buffer_set, Entry);
      RangeIterator(CopyIndex, (s32)CopySet->Count)
      {
        work_queue_entry_copy_buffer_ref *CopyJob = (work_queue_entry_copy_buffer_ref *)CopySet->CopyTargets + CopyIndex;
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
  Offset.z = 0; // Constrain to XY plane

  if (Input->E.Pressed) { Offset.z += 1.f; }
  if (Input->Q.Pressed) { Offset.z -= 1.f; }

  Offset = Normalize(Offset);
  Resources->CameraTarget->P.Offset += Offset * dt * Speed;

  v3 EmissionColor = Normalize(V3(3,1,0));
  DoLight(Graphics->Lights, V3(0), EmissionColor);
}
