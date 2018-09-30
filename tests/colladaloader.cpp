
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
    ansi_stream SelectorStream = AnsiStream("?xml COLLADA library_geometries geometry#Cube-mesh mesh source#Cube-mesh-positions float_array#Cube-mesh-positions-array");
    xml_token_stream Selector = TokenizeSelector(&SelectorStream, Memory);
    xml_tag* ResultTag = GetFirstMatchingTag(&XmlTokens, &Selector);
    TestThat( StringsMatch(ResultTag->Value, CS("1 -1 -1 -1 -0.9999998 -1 0.9486669 -1.104168 1.050864 -0.687431 -0.8150698 0.9838108 1 -0.334378 -1.0002 -1 -0.3343777 -1.0002 0.9999997 -0.3343785 0.9998002 -1 -0.490265 0.9998002 0 -0.9999999 -1 -5.36442e-7 -1 1 0.01414924 -0.463192 1.581099 0 -0.3343778 -1.0002 0.666179 -0.4452514 1.351506 0.5 -1 -1 0.4999994 -1.384452 1.069946 0.5 -0.3343779 -1.0002 -0.5000001 -0.9999999 -1 -0.5000005 -0.9999999 1 -0.5 -0.3343778 -1.0002 -0.4022939 -0.4422637 1.400678 1 0 -1.000144 0.9999999 0 0.9998559 -0.9999999 0 -1.000144 -1 0 0.9998559 0 0 -1.000144 0 0 1.643503 0.6661792 0 1.35158 0.5 0 -1.000144 -0.4999999 0 -1.000144 -0.4369376 0 1.463367") ) );
  }

  TestSuiteEnd();
  exit(TestsFailed);
}
