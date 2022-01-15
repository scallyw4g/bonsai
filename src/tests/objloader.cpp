
#include <bonsai_types.h>
#include <tests/test_utils.cpp>

s32
main(s32 ArgCount, const char** Args)
{
  TestSuiteBegin("Obj Loader", ArgCount, Args);

  memory_arena *Memory = AllocateArena(Megabytes(1));

  {
    ansi_stream Stream = AnsiStreamFromFile(TEST_FIXTURES_PATH "/test.obj", Memory);
    mesh_metadata Stats = GetObjMetadata(Stream, Memory);
    TestThat(Stats.VertCount == 2);
    TestThat(Stats.NormalCount == 2);
    TestThat(Stats.FaceCount == 2);
    TestThat(Stats.UVCount == 0);
  }

  {
    ansi_stream Stream = AnsiStreamFromFile(TEST_FIXTURES_PATH "/test.obj", Memory);
    mesh_metadata Stats = GetObjMetadata(Stream, Memory);
    TestThat(Stats.VertCount == 2);
    TestThat(Stats.NormalCount == 2);
    TestThat(Stats.FaceCount == 2);
    TestThat(Stats.UVCount == 0);
  }

  TestSuiteEnd();
  exit(TestsFailed);
}
