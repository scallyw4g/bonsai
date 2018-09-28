
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

void
ParsingTest()
{
  memory_arena *Memory = PlatformAllocateArena(Megabytes(1));

  ansi_stream XmlStream = AnsiStreamFromFile("tests/fixtures/test_parsing.dae", Memory);

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


    xml_token XmlOpen   = XmlOpenToken(CS("xml"));
    xml_token XmlClose  = XmlCloseToken(CS("xml"));
    xml_token OuterOpen = XmlOpenToken(CS("outer"));
    xml_token InnerOpen = XmlOpenToken(CS("inner"));
    xml_token ValueOpen = XmlOpenToken(CS("value"));

    xml_token OuterClose = XmlCloseToken(CS("outer"));
    xml_token InnerClose = XmlCloseToken(CS("inner"));
    xml_token ValueClose = XmlCloseToken(CS("value"));

    xml_token CommentProperty = XmlStringProperty( CS("comment"), CS("a tag such as this '<value/>' fails right now.  Fix this!"""));

    Rewind(&XmlTokens);

    TestThat(*XmlTokens.At++ == XmlOpen);

    { // Well formatted
      TestThat(*XmlTokens.At++ == OuterOpen);
      TestThat(*XmlTokens.At++ == InnerOpen);
      TestThat(*XmlTokens.At++ == ValueOpen);

      TestThat(*XmlTokens.At++ == ValueClose);
      TestThat(*XmlTokens.At++ == InnerClose);
      TestThat(*XmlTokens.At++ == OuterClose);
    }
    { // Single line
      TestThat(*XmlTokens.At++ == OuterOpen);
      TestThat(*XmlTokens.At++ == InnerOpen);
      TestThat(*XmlTokens.At++ == ValueOpen);

      TestThat(*XmlTokens.At++ == ValueClose);
      TestThat(*XmlTokens.At++ == InnerClose);
      TestThat(*XmlTokens.At++ == OuterClose);
    }

    { // With the comment property
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
    { // With the comment property on a single line
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
  }

  VaporizeArena(Memory);

  return;
}

void
QueryingTest()
{
  memory_arena *Memory = PlatformAllocateArena(Megabytes(1));

  ansi_stream XmlStream = AnsiStreamFromFile("tests/fixtures/test_querying.dae", Memory);
  xml_token_stream XmlTokens = TokenizeXmlStream(&XmlStream, Memory);

  {
    ansi_stream SelectorStream = AnsiStream("xml outer inner value");
    xml_token_stream Selector = TokenizeSelector(&SelectorStream, Memory);
    xml_tag* ResultTag = GetFirstMatchingTag(&XmlTokens, &Selector);

    /* xml_token OpenExpected = XmlOpenToken(CS("value")); */
    /* TestThat(*ResultTag->Open == OpenExpected); */

    /* xml_token CloseExpected = XmlCloseToken(CS("value")); */
    /* TestThat(*ResultTag.Close == CloseExpected); */
  }

  {
    ansi_stream SelectorStream = AnsiStream("xml outer inner value");
    xml_token_stream Selector = TokenizeSelector(&SelectorStream, Memory);
    xml_tag* ResultTag = GetFirstMatchingTag(&XmlTokens, &Selector);

    /* xml_token OpenExpected = XmlOpenToken(CS("value")); */
    /* TestThat(*ResultTag->Open == OpenExpected); */
  }

  return;
}

s32
main()
{
  TestSuiteBegin("Collada Loader");

  ParsingTest();

  QueryingTest();

  TestSuiteEnd();
  exit(TestsFailed);
}
