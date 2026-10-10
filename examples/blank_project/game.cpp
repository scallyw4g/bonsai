// NOTE(Jesse): This includes implementations for performace profiling and debug tracing
#define BONSAI_DEBUG_SYSTEM_API 1

#include <bonsai_types.h>
#include <game_types.h>

// Worker callbacks are optional. Without an override, the engine dispatches
// each job's async-function-call tasks through its default worker implementation.

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

  StandardCamera(Graphics->Camera, 30000.0f, 1000.0f);

  auto WorldSize = VisibleRegionSize_512;
  AllocateWorld(World, WorldCenter, WorldSize);

  SnapCameraToCenterOfWorld(Resources, WorldSize);

  GameState = Allocate(game_state, Resources->GameMemory, 1);
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

#if 0
  if (Input->LMB.Clicked)
  {
    random_series E = {654367547654};
    if (Resources->MousedOverVoxel.Tag)
    {
      cp PickCP = Canonical_Position(&Resources->MousedOverVoxel.Value);
      DoSplotion( Resources, PickCP, 8.f, &E, GetTranArena());
    }
  }
#endif

}
