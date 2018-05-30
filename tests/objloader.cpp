
#define BONSAI_NO_PUSH_METADATA

#include <bonsai_types.h>
#include <unix_platform.cpp>

#include <texture.cpp>
#include <shader.cpp>
#include <debug.cpp>
#include <objloader.cpp>

s32
main()
{
  memory_arena *Memory = PlatformAllocateArena(Megabytes(1));

  {
    const char *TestData = "v whatever\nv whatever\nvn whatever\nvn whatever\nf whatever\nf whatever\n";
    stream_cursor Stream = StreamCursor(TestData);
    obj_stats Stats = GetObjStats(Stream, Memory);
    Assert(Stats.VertCount == 2);
    Assert(Stats.NormalCount == 2);
    Assert(Stats.FaceCount == 2);
    Assert(Stats.UVCount == 0);
  }

  {
    const char *TestData = "v whatever\nv whatever\nvn whatever\nvn whatever\nf whatever\nf whatever";
    stream_cursor Stream = StreamCursor(TestData);
    obj_stats Stats = GetObjStats(Stream, Memory);
    Assert(Stats.VertCount == 2);
    Assert(Stats.NormalCount == 2);
    Assert(Stats.FaceCount == 2);
    Assert(Stats.UVCount == 0);
  }

  return 0;
}


