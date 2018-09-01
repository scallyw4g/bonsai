
#define BONSAI_NO_PUSH_METADATA

#include <bonsai_types.h>
#include <unix_platform.cpp>

#include <texture.cpp>
#include <shader.cpp>
#include <debug.cpp>

#include <test_utils.cpp>

#include <objloader.cpp>

s32
main()
{
  TestSuiteBegin("Obj Loader");

  memory_arena *Memory = PlatformAllocateArena(Megabytes(1));

  {
    const char *TestData = "v whatever\nv whatever\nvn whatever\nvn whatever\nf whatever\nf whatever\n";
    stream_cursor Stream = StreamCursor(TestData);
    obj_stats Stats = GetObjStats(Stream, Memory);
    TestThat(Stats.VertCount == 2);
    TestThat(Stats.NormalCount == 2);
    TestThat(Stats.FaceCount == 2);
    TestThat(Stats.UVCount == 0);
  }

  {
    const char *TestData = "v whatever\nv whatever\nvn whatever\nvn whatever\nf whatever\nf whatever";
    stream_cursor Stream = StreamCursor(TestData);
    obj_stats Stats = GetObjStats(Stream, Memory);
    TestThat(Stats.VertCount == 2);
    TestThat(Stats.NormalCount == 2);
    TestThat(Stats.FaceCount == 2);
    TestThat(Stats.UVCount == 0);
  }

  TestSuiteEnd();
  exit(TestsFailed);
}
