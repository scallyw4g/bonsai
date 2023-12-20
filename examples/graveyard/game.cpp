#define DEBUG_SYSTEM_API 1

#include <bonsai_types.h>

#include <game_constants.h>
#include <game_types.h>



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
        Chunk->Flags = Chunk_Uninitialized;
      }
      else
      {

#if 1
        {
          // Custom FBM noise example generating slightly-more-complex game-world-like terrain
          s32 Frequency = 0; // Ignored
          s32 Amplititude = 0; // Ignored
          /* s32 StartingZDepth = -64; */
          s32 StartingZDepth = 0;
          u32 OctaveCount = 1;

          octave_buffer OctaveBuf = { OctaveCount, {} };
          OctaveBuf.Octaves = Allocate(octave, Thread->TempMemory, OctaveCount);

          OctaveBuf.Octaves[0] = {V3(300, 300, 100), 1.f, 1.0f, V3(1)};

          /* OctaveBuf.Octaves[0] = {V3(400, 400, 150), 150, 1.0f, V3(1)}; */
          /* OctaveBuf.Octaves[1] = {V3(35, 35, 50), 50, 0.2f, V3(2.f)}; */
          /* OctaveBuf.Octaves[2] = {V3(500, 500, 20), 200, V3(2.f)}; */
          /* OctaveBuf.Octaves[2] = {75, 60, 1}; */
          /* OctaveBuf.Octaves[3] = {37, 30, 0}; */


          /* chunk_init_flags InitFlags = ChunkInitFlag_ComputeStandingSpots; */
          /* chunk_init_flags InitFlags = ChunkInitFlag_GenMipMapLODs; */
          chunk_init_flags InitFlags = ChunkInitFlag_Noop;
          InitializeChunkWithNoise( GrassyTerrain, Thread, Chunk, Chunk->Dim, 0, Frequency, Amplititude, StartingZDepth, MeshBit_Lod0, InitFlags, (void*)&OctaveBuf);
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

  f32 dt = Plat->dt;
  f32 Speed = 80.f;
}
