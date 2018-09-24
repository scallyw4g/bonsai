
#define BONSAI_NO_PUSH_METADATA

#include <bonsai_types.h>
#include <unix_platform.cpp>

#include <debug_data_system.cpp>

#include <test_utils.cpp>

#include <perlin.cpp>
#include <chunk.cpp>
#include <bonsai_mesh.cpp>

global_variable memory_arena *TranArena = PlatformAllocateArena();
#include <loaders/obj.cpp>

s32
main()
{
  TestSuiteBegin("Obj Loader");

  memory_arena *Memory = PlatformAllocateArena(Megabytes(1));

  {
    ansi_stream Stream = AnsiStreamFromFile("test.obj", Memory);
    obj_stats Stats = GetObjStats(Stream, Memory);
    TestThat(Stats.VertCount == 2);
    TestThat(Stats.NormalCount == 2);
    TestThat(Stats.FaceCount == 2);
    TestThat(Stats.UVCount == 0);
  }

  {
    const char *TestData = "v whatever\nv whatever\nvn whatever\nvn whatever\nf whatever\nf whatever";
    ansi_stream Stream = AnsiStreamFromFile("test.obj", Memory);
    obj_stats Stats = GetObjStats(Stream, Memory);
    TestThat(Stats.VertCount == 2);
    TestThat(Stats.NormalCount == 2);
    TestThat(Stats.FaceCount == 2);
    TestThat(Stats.UVCount == 0);
  }

  TestSuiteEnd();
  exit(TestsFailed);
}
