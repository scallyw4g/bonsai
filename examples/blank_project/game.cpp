// NOTE(Jesse): This includes implementations for performace profiling and debug tracing
#define DEBUG_SYSTEM_API 1

#include <bonsai_types.h>

#include <game_constants.h>
#include <game_types.h>

// NOTE(Jesse): This is an optional function that gets called for each worker
// thread at engine startup, but not the main thread!
BONSAI_API_WORKER_THREAD_INIT_CALLBACK()
{
}

// NOTE(Jesse): This is an optional function that you may use to override
// default implementations of the following jobs.  The init_world_chunk may be
// of particular interest and several example implementations can be found in
// the terrain_gen example.  If you handle a job, return True, otherwise return False.
BONSAI_API_WORKER_THREAD_CALLBACK()
{
  switch (Entry->Type)
  {
    // NOTE(Jesse): A noop entry is a bug; InvalidCase() crashes the process
    // in debug mode, and does nothing in release mode (in the hopes we handle
    // whatever else happens gracefully).
    InvalidCase(type_work_queue_entry_noop);

    // NOTE(Jesse): This is hack to get the compiler to align our structs to
    // cache line sizes.  No program should recieve a message of this type.
    InvalidCase(type_work_queue_entry__align_to_cache_line_helper);

    case type_work_queue_entry_update_world_region:
    case type_work_queue_entry_rebuild_mesh:
    case type_work_queue_entry_init_asset:
    case type_work_queue_entry_init_world_chunk:
    case type_work_queue_entry_copy_buffer_ref:
    case type_work_queue_entry_copy_buffer_set:
    case type_work_queue_entry_sim_particle_system: {} break;
  }

  return False;
}

// NOTE(Jesse): This gets called once on the main thread at engine startup.
// This is a bare-bones example of the code you'll need to spawn a camera,
// and spawn an entity for it to follow.
//
// The movement code for the entity is left as an excercise for the reader and
// should be implemented in the main thread callback.
//
// This function must return a pointer to a GameState struct, as defined by the
// game.  In this example, the definition is in `examples/blank_project/game_types.h`
BONSAI_API_MAIN_THREAD_INIT_CALLBACK()
{
  // NOTE(Jesse): This is a convenience macro for unpacking all the information
  // the engine passes around.  It nominally reduces the amount of typing you have to do.
  //
  UNPACK_ENGINE_RESOURCES(Resources);

  // NOTE(Jesse): Update this path if you copy this project to your new project path
  //
  Global_AssetPrefixPath = CSz("examples/blank_project/assets");

  world_position WorldCenter = {};
  canonical_position CameraTargetP = {};

  StandardCamera(Graphics->Camera, 10000.0f, 1000.0f, CameraTargetP);

  AllocateWorld(World, WorldCenter, WORLD_CHUNK_DIM, g_VisibleRegion);

  World->Flags = WorldFlag_WorldCenterFollowsCameraTarget;

  entity *CameraTarget = GetFreeEntity(EntityTable);
  SpawnEntity(CameraTarget);

  Resources->CameraTarget = CameraTarget;

  GameState = Allocate(game_state, Resources->Memory, 1);
  return GameState;
}

// NOTE(Jesse): This is the main game loop.  Put your game update logic here!
//
BONSAI_API_MAIN_THREAD_CALLBACK()
{
  Assert(ThreadLocal_ThreadIndex == 0);

  TIMED_FUNCTION();
  UNPACK_ENGINE_RESOURCES(Resources);

  f32 dt = Plat->dt;

  // Update camera position
  if (Input->W.Pressed || Input->S.Pressed || Input->A.Pressed || Input->D.Pressed)
  {
    v3 Offset = GetCameraRelativeInput(Hotkeys, Camera);

    // Constrain the camera update to the XY plane
    Offset.z = 0;
    Offset = Normalize(Offset, 1.f);

    Resources->CameraTarget->P.Offset += Offset;
  }

}
