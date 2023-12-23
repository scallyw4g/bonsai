#define DEBUG_SYSTEM_API 1

#include <bonsai_types.h>

#include <game_constants.h>
#include <game_types.h>


BONSAI_API_WORKER_THREAD_CALLBACK()
{
  if (ThreadLocal_ThreadIndex == INVALID_THREAD_LOCAL_THREAD_INDEX) { SetThreadLocal_ThreadIndex(Thread->Index); }

  b32 Result = True;
  switch (Entry->Type)
  {
    default: { Result = False; } break;

    case type_work_queue_entry_init_world_chunk:
    {
      volatile work_queue_entry_init_world_chunk *Job = SafeAccess(work_queue_entry_init_world_chunk, Entry);
      world_chunk *Chunk = Job->Chunk;

      if (ChunkIsGarbage(Chunk))
      {
        Chunk->Flags = Chunk_Uninitialized;
      }
      else
      {

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

  v3 EmissionColor = Normalize(V3(3.f, 4.f, 0.1f)) * 5.f;
  DoLight(&Lighting->Lights, V3(0.5f), EmissionColor);
}
