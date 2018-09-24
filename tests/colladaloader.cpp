
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

  counted_string ZeroString = {};
  memory_arena *Memory = PlatformAllocateArena(Megabytes(1));

  binary_stream_u8 FileStream = BinaryStreamFromFile("tests/fixtures/test1.dae", Memory);
  TestThat(FileStream.Start);

#if 0
  {
    counted_string Selector = CountedString("xml");
    ansi_stream XmlStream = AnsiStream(&FileStream);
    ansi_stream SelectorStream = AnsiStream(Selector.Start);

    xml_tag Tag = GetFirstMatchingTag(&XmlStream, &SelectorStream, &ZeroString);
    TestThat( StringsMatch(&Tag.Selector, &Selector) );
  }

  {
    counted_string Selector = CountedString("xml outer-target");
    ansi_stream XmlStream = AnsiStream(&FileStream);
    ansi_stream SelectorStream = AnsiStream(Selector.Start);

    xml_tag Tag = GetFirstMatchingTag(&XmlStream, &SelectorStream, &ZeroString);
    counted_string Expected = CountedString("outer-target");
    TestThat( StringsMatch(&Tag.Selector, &Expected) );
  }

  {
    counted_string Selector = CountedString("xml outer-target inner-target value");
    ansi_stream XmlStream = AnsiStream(&FileStream);
    ansi_stream SelectorStream = AnsiStream(Selector.Start);

    xml_tag Tag = GetFirstMatchingTag(&XmlStream, &SelectorStream, &ZeroString);
    counted_string Expected = CountedString("value");
    TestThat( StringsMatch(&Tag.Selector, &Expected) );
  }
#endif

  TestSuiteEnd();
  exit(TestsFailed);
}
