#define BONSAI_NO_PUSH_METADATA

#include <bonsai_types.h>
#include <unix_platform.cpp>

#include <texture.cpp>
#include <shader.cpp>
#include <debug.cpp>
#include <objloader.cpp>

void
TestPopWord(stream_cursor *Stream, memory_arena *Memory)
{
  {
    char *Word = PopWord(Stream, Memory);
    Assert(StringsMatch(Word, "word1"));
  }
  {
    char *Word = PopWord(Stream, Memory);
    Assert(StringsMatch(Word, "word2"));
  }
  {
    char *Word = PopWord(Stream, Memory);
    Assert(StringsMatch(Word, "word3"));
  }

  return;
}

void
TestPopU32(stream_cursor *Stream, memory_arena *Memory)
{
  {
    u32 N = PopU32(Stream, Memory);
    Assert(N == 1);
  }
  {
    u32 N = PopU32(Stream, Memory);
    Assert(N == 2);
  }
  {
    u32 N = PopU32(Stream, Memory);
    Assert(N == 3);
  }

  return;
}

void
TestStreamCursor()
{
  memory_arena *Memory = PlatformAllocateArena(Megabytes(1));

  {
    const char *TestData = "word1 word2 word3";
    stream_cursor Stream = StreamCursor(TestData);
    TestPopWord(&Stream, Memory);
  }

  {
    const char *TestData = "word1\nword2\nword3";
    stream_cursor Stream = StreamCursor(TestData);
    TestPopWord(&Stream, Memory);
  }

  {
    const char *TestData = "word1    word2\nword3";
    stream_cursor Stream = StreamCursor(TestData);
    TestPopWord(&Stream, Memory);
  }

  {
    const char *TestData = "  word1    word2\nword3  ";
    stream_cursor Stream = StreamCursor(TestData);
    TestPopWord(&Stream, Memory);
  }


  {
    const char *TestData = "1 2 3";
    stream_cursor Stream = StreamCursor(TestData);
    TestPopU32(&Stream, Memory);
  }

  {
    const char *TestData = "1\n2\n3";
    stream_cursor Stream = StreamCursor(TestData);
    TestPopU32(&Stream, Memory);
  }

  {
    const char *TestData = "1    2\n3";
    stream_cursor Stream = StreamCursor(TestData);
    TestPopU32(&Stream, Memory);
  }

  {
    const char *TestData = "  1    2\n3  ";
    stream_cursor Stream = StreamCursor(TestData);
    TestPopU32(&Stream, Memory);
  }

  {
    const char *TestData = "1/2/3";
    stream_cursor Stream = StreamCursor(TestData);
    {
      u32 N = PopU32(&Stream, Memory, "/");
      Assert(N == 1);
    }
    {
      u32 N = PopU32(&Stream, Memory, "/");
      Assert(N == 2);
    }
    {
      u32 N = PopU32(&Stream, Memory);
      Assert(N == 3);
    }
  }

  {
    const char *TestData = "1//3//";
    stream_cursor Stream = StreamCursor(TestData);
    {
      u32 N = PopU32(&Stream, Memory, "/");
      Assert(N == 1);
    }
    {
      u32 N = PopU32(&Stream, Memory);
      Assert(N == 3);
    }
  }


  return;
}

s32
main()
{
  {
    const char *Test1 = "v";
    const char *Test2 = "v";
    Assert(StringsMatch(Test1, Test2));
  }

  {
    const char *Test1 = "vv";
    const char *Test2 = "vv";
    Assert(StringsMatch(Test1, Test2));
  }

  {
    const char *Test1 = "v v";
    const char *Test2 = "v v";
    Assert(StringsMatch(Test1, Test2));
  }

  {
    const char *Test1 = "vv";
    const char *Test2 = "v";
    Assert(!StringsMatch(Test1, Test2));
  }

  {
    const char *Test1 = "v";
    const char *Test2 = "vv";
    Assert(!StringsMatch(Test1, Test2));
  }

  {
    const char *TestData = "   \n   word1";
    const char *Word = EatAllCharacters(TestData, " \n");
    Assert(StringsMatch(Word, "word1"));
  }

  TestStreamCursor();



  return 0;
}


