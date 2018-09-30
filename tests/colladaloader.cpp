
#define BONSAI_NO_PUSH_METADATA

#include <bonsai_types.h>
#include <unix_platform.cpp>

#include <debug_data_system.cpp>

#include <test_utils.cpp>

#include <perlin.cpp>
#include <chunk.cpp>
#include <bonsai_mesh.cpp>

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
    xml_token IdValueOpen = XmlOpenToken(CS("value"), CS("value-id"));

    xml_token OuterClose = XmlCloseToken(CS("outer"));
    xml_token InnerClose = XmlCloseToken(CS("inner"));
    xml_token ValueClose = XmlCloseToken(CS("value"));

    xml_token CommentProperty = XmlStringProperty( CS("comment"), CS("a tag such as this '<value/>' fails right now.  Fix this!"""));

    Rewind(&XmlTokens);

    TestThat(*XmlTokens.At++ == XmlOpen);

    TestThat(*XmlTokens.At++ == IdValueOpen);
    TestThat(*XmlTokens.At++ == ValueClose);

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

      TestThat(*XmlTokens.At++ == ValueClose);
      TestThat(*XmlTokens.At++ == InnerClose);
      TestThat(*XmlTokens.At++ == OuterClose);
    }
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

      TestThat(*XmlTokens.At++ == ValueClose);
      TestThat(*XmlTokens.At++ == InnerClose);
      TestThat(*XmlTokens.At++ == OuterClose);
    }
    {
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
DumpHashTable(hashtable<xml_tag*> * Hash)
{
  for (u32 ElementIndex = 0;
      ElementIndex < Hash->ElementCount;
      ++ElementIndex)
  {
    xml_tag* Element = Hash->Elements[ElementIndex];
    if (Element)
    {
      Log("");
      Print(ElementIndex);
      Print(Element);
      while (Element = Element->NextInHash)
      {
        Print(Element);
      }
    }
  }

  return;
}

void
XmlTests()
{
  xml_token RegularOpenToken = XmlOpenToken(CS("open"));
  xml_token OpenTokenWithId = XmlOpenToken(CS("open"), CS("id-value"));

  TestThat(RegularOpenToken == RegularOpenToken);
  TestThat(StringsMatch(RegularOpenToken.Property.Name, CS("open")));
  TestThat(RegularOpenToken.Property.Id.Count == 0);

  TestThat(OpenTokenWithId == OpenTokenWithId);
  TestThat(StringsMatch(OpenTokenWithId.Property.Name, CS("open")));
  TestThat(StringsMatch(OpenTokenWithId.Property.Id, CS("id-value")));

  return;
}

void
QueryingTest()
{
  memory_arena *Memory = PlatformAllocateArena(Megabytes(1));

  ansi_stream XmlStream = AnsiStreamFromFile("tests/fixtures/test_querying.dae", Memory);
  xml_token_stream XmlTokens = TokenizeXmlStream(&XmlStream, Memory);
  counted_string TheMeaningOfLifeTheUniverseAndEverything = CS("42");
  counted_string FourtyTwoTwice = CS("42 42");

  /* DumpHashTable(&XmlTokens.Hashes); */

  {
    ansi_stream SelectorStream = AnsiStream("xml first-value");
    xml_token_stream Selector = TokenizeSelector(&SelectorStream, Memory);
    xml_tag* ResultTag = GetFirstMatchingTag(&XmlTokens, &Selector);
    xml_token OpenExpected = XmlOpenToken(CS("first-value"));
    TestThat(*ResultTag->Open == OpenExpected);
    TestThat(StringsMatch(&ResultTag->Value, &FourtyTwoTwice));
  }

  {
    ansi_stream SelectorStream = AnsiStream("xml first-value#id-value");
    xml_token_stream Selector = TokenizeSelector(&SelectorStream, Memory);
    /* RuntimeBreak(); */
    xml_tag* ResultTag = GetFirstMatchingTag(&XmlTokens, &Selector);
    xml_token OpenExpected = XmlOpenToken(CS("first-value"), CS("id-value"));
    TestThat(*ResultTag->Open == OpenExpected);
    TestThat(StringsMatch(&ResultTag->Value, &TheMeaningOfLifeTheUniverseAndEverything));
  }

  {
    ansi_stream SelectorStream = AnsiStream("xml outer inner second-value");
    xml_token_stream Selector = TokenizeSelector(&SelectorStream, Memory);
    xml_tag* ResultTag = GetFirstMatchingTag(&XmlTokens, &Selector);
    xml_token OpenExpected = XmlOpenToken(CS("second-value"));
    TestThat(*ResultTag->Open == OpenExpected);
    TestThat(StringsMatch(&ResultTag->Value, &TheMeaningOfLifeTheUniverseAndEverything));
  }

  {
    ansi_stream SelectorStream = AnsiStream("xml third-value");
    xml_token_stream Selector = TokenizeSelector(&SelectorStream, Memory);
    xml_tag* ResultTag = GetFirstMatchingTag(&XmlTokens, &Selector);
    xml_token OpenExpected = XmlOpenToken(CS("third-value"));
    TestThat(*ResultTag->Open == OpenExpected);
    TestThat(StringsMatch(&ResultTag->Value, &TheMeaningOfLifeTheUniverseAndEverything));
  }

  {
    ansi_stream SelectorStream = AnsiStream("xml outer inner fourth-value");
    xml_token_stream Selector = TokenizeSelector(&SelectorStream, Memory);
    xml_tag* ResultTag = GetFirstMatchingTag(&XmlTokens, &Selector);
    xml_token OpenExpected = XmlOpenToken(CS("fourth-value"));
    TestThat(*ResultTag->Open == OpenExpected);
    TestThat(StringsMatch(&ResultTag->Value, &TheMeaningOfLifeTheUniverseAndEverything));
  }

  return;
}

s32
main()
{
  TestSuiteBegin("Collada Loader");

  XmlTests();

  ParsingTest();

  QueryingTest();


  memory_arena *Memory = PlatformAllocateArena(Megabytes(1));
  ansi_stream XmlStream = AnsiStreamFromFile("models/model.dae", Memory);
  xml_token_stream XmlTokens = TokenizeXmlStream(&XmlStream, Memory);

  {
    ansi_stream SelectorStream = AnsiStream("?xml COLLADA library_geometries geometry mesh source float_array#Cube-mesh-positions-array");
    xml_token_stream Selector = TokenizeSelector(&SelectorStream, Memory);
    xml_tag* ResultTag = GetFirstMatchingTag(&XmlTokens, &Selector);
  }

  TestSuiteEnd();
  exit(TestsFailed);
}
