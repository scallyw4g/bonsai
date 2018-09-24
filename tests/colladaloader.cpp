
#define BONSAI_NO_PUSH_METADATA

#include <bonsai_types.h>
#include <unix_platform.cpp>

#include <debug_data_system.cpp>

#include <test_utils.cpp>

#include <perlin.cpp>
#include <chunk.cpp>
#include <bonsai_mesh.cpp>

global_variable memory_arena *TranArena = PlatformAllocateArena();
#include <loaders/common.h>
#include <loaders/collada.cpp>

s32
main()
{
  TestSuiteBegin("Collada Loader");

  memory_arena *Memory = PlatformAllocateArena(Megabytes(1));
  model Model = LoadCollada(Memory, "models/model.dae");

  TestSuiteEnd();
  exit(TestsFailed);
}
