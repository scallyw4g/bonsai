
#define BONSAI_NO_PUSH_METADATA

#include <bonsai_types.h>
#include <unix_platform.cpp>

#include <debug_data_system.cpp>

#include <test_utils.cpp>

#include <perlin.cpp>
#include <chunk.cpp>
#include <bonsai_mesh.cpp>

global_variable memory_arena *TranArena = PlatformAllocateArena();

#include <bonsai_string.cpp>
#include <loaders/common.h>
#include <loaders/collada.cpp>

s32
main()
{
  TestSuiteBegin("Collada Loader");

  counted_string ZeroString = {};
  memory_arena *Memory = PlatformAllocateArena(Megabytes(1));

  ansi_stream XmlStream = AnsiStreamFromFile("tests/fixtures/test1.dae", Memory);
  TestThat(XmlStream.Start);

  {
    xml_token_stream XmlTokens = TokenizeXmlStream(&XmlStream, Memory);
    umm TokenCount = Count(&XmlTokens);

#if 0
    for (u32 TokenIndex = 0;
        TokenIndex < Count(&XmlTokens);
        ++TokenIndex)
    {
      Print(XmlTokens.Start + TokenIndex);
    }
#endif


    xml_token XmlOpen   = XmlOpenTag(CS("xml"));
    xml_token XmlClose  = XmlCloseTag(CS("xml"));
    xml_token OuterOpen = XmlOpenTag(CS("outer"));
    xml_token InnerOpen = XmlOpenTag(CS("inner"));
    xml_token ValueOpen = XmlOpenTag(CS("value"));

    xml_token OuterClose = XmlCloseTag(CS("outer"));
    xml_token InnerClose = XmlCloseTag(CS("inner"));
    xml_token ValueClose = XmlCloseTag(CS("value"));

    xml_token CommentProperty = XmlStringProperty( CS("comment"), CS("a tag such as this '<value/>' fails right now.  Fix this!"""));

    Rewind(&XmlTokens);

    TestThat(*XmlTokens.At++ == XmlOpen);

    {
      TestThat(*XmlTokens.At++ == OuterOpen);
      TestThat(*XmlTokens.At++ == InnerOpen);
      TestThat(*XmlTokens.At++ == ValueOpen);

      TestThat(*XmlTokens.At++ == ValueClose);
      TestThat(*XmlTokens.At++ == InnerClose);
      TestThat(*XmlTokens.At++ == OuterClose);
    }

    {
      TestThat(*XmlTokens.At++ == OuterOpen);
      TestThat(*XmlTokens.At++ == InnerOpen);

      TestThat(*XmlTokens.At++ == ValueOpen);
      TestThat(*XmlTokens.At++ == CommentProperty);
      TestThat(*XmlTokens.At++ == ValueClose);

      TestThat(*XmlTokens.At++ == ValueOpen);
      TestThat(*XmlTokens.At++ == ValueClose);

      TestThat(*XmlTokens.At++ == ValueOpen);
      TestThat(*XmlTokens.At++ == ValueClose);

      TestThat(*XmlTokens.At++ == InnerClose);
      TestThat(*XmlTokens.At++ == OuterClose);
    }

    {
      xml_token BooleanProp = XmlBooleanProperty(CS("boolean"));
      xml_token IntProp     = XmlIntProperty(CS("int-property"), CS("42"));
      xml_token FloatProp   = XmlFloatProperty(CS("float-property"), CS("42.42"));

      xml_token StringProp1   = XmlStringProperty(CS("string-property-1"), CS("< text, yay! >"));
      xml_token StringProp2   = XmlStringProperty(CS("string-property-2"), CS("> more text, yay! <"));


      TestThat(*XmlTokens.At++ == OuterOpen);

      TestThat(*XmlTokens.At++ == BooleanProp);
      TestThat(*XmlTokens.At++ == IntProp);
      TestThat(*XmlTokens.At++ == FloatProp);
      TestThat(*XmlTokens.At++ == StringProp1);
      TestThat(*XmlTokens.At++ == StringProp2);

      TestThat(*XmlTokens.At++ == InnerOpen);
      TestThat(*XmlTokens.At++ == ValueOpen);
      TestThat(*XmlTokens.At++ == ValueClose);
      TestThat(*XmlTokens.At++ == InnerClose);

      TestThat(*XmlTokens.At++ == OuterClose);
      TestThat(*XmlTokens.At++ == XmlClose);
    }

    TestThat(Count(&XmlTokens) == TokenCount);
    Rewind(&XmlTokens);

    {
      ansi_stream Selector = AnsiStream("xml outer-target inner-target value");

      xml_tag ResultTag = GetFirstMatchingTag(&XmlTokens, &Selector, &ZeroString);

      xml_token OpenExpected = XmlOpenTag(CS("value"));
      xml_token CloseExpected = XmlCloseTag(CS("value"));

      /* TestThat(*ResultTag.Open == OpenExpected); */
      /* TestThat(*ResultTag.Close == CloseExpected); */
    }

  }

  TestSuiteEnd();
  exit(TestsFailed);
}
