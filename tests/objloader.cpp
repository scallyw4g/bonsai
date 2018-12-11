#define BONSAI_NO_PUSH_METADATA
#define BONSAI_NO_DEBUG_MEMORY_ALLOCATOR

#include <bonsai_types.h>
#include <unix_platform.cpp>

#include <test_utils.cpp>

#include <heap_memory_types.cpp>

#include <perlin.cpp>
#include <chunk.cpp>
#include <bonsai_mesh.cpp>


global_variable memory_arena* TranArena = {};
#include <stream.cpp>
#include <loaders/obj.cpp>

s32
main()
{
  TestSuiteBegin("Obj Loader");

  memory_arena *Memory = PlatformAllocateArena(Megabytes(1));

  {
    ansi_stream Stream = AnsiStreamFromFile("test.obj", Memory);
    mesh_metadata Stats = GetObjMetadata(Stream, Memory);
    TestThat(Stats.VertCount == 2);
    TestThat(Stats.NormalCount == 2);
    TestThat(Stats.FaceCount == 2);
    TestThat(Stats.UVCount == 0);
  }

  {
    ansi_stream Stream = AnsiStreamFromFile("test.obj", Memory);
    mesh_metadata Stats = GetObjMetadata(Stream, Memory);
    TestThat(Stats.VertCount == 2);
    TestThat(Stats.NormalCount == 2);
    TestThat(Stats.FaceCount == 2);
    TestThat(Stats.UVCount == 0);
  }

  TestSuiteEnd();
  exit(TestsFailed);
}
