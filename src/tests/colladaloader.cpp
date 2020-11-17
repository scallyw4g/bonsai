
#include <bonsai_types.h>
#include <tests/test_utils.cpp>

void
TokenizingTest()
{
  memory_arena *Memory = AllocateArena(Megabytes(1));
  ansi_stream XmlStream = AnsiStreamFromFile(TEST_FIXTURES_PATH "/test_parsing.dae", Memory);

  {
    xml_token_stream XmlTokens = TokenizeXmlStream(&XmlStream, Memory);
    umm TokenCount = AtElements(&XmlTokens);

    xml_token XmlOpen   = XmlOpenToken(CS("xml"));
    xml_token XmlClose  = XmlCloseToken(CS("xml"), 0);
    xml_token OuterOpen = XmlOpenToken(CS("outer"));
    xml_token InnerOpen = XmlOpenToken(CS("inner"));

    xml_token ValueOpen = XmlOpenToken(CS("value"));
    xml_token IdValue   = XmlPropertyToken(CS("id"), CS("value-id"));

    xml_token OuterClose = XmlCloseToken(CS("outer"), 0);
    xml_token InnerClose = XmlCloseToken(CS("inner"), 0);
    xml_token ValueClose = XmlCloseToken(CS("value"), 0);

    xml_token CommentProperty = XmlPropertyToken( CS("comment"), CS("a tag such as this '<value/>' fails right now.  Fix this!"""));

    Rewind(&XmlTokens);

    TestThat(TokensAreEqual(XmlTokens.At++, &XmlOpen));

    TestThat(TokensAreEqual(XmlTokens.At++, &ValueOpen));
    TestThat(TokensAreEqual(XmlTokens.At++, &ValueClose));

    TestThat(TokensAreEqual(XmlTokens.At++, &ValueOpen));
    TestThat(TokensAreEqual(XmlTokens.At++, &IdValue));
    TestThat(TokensAreEqual(XmlTokens.At++, &ValueClose));

    {
      TestThat(TokensAreEqual(XmlTokens.At++, &OuterOpen));
      TestThat(TokensAreEqual(XmlTokens.At++, &InnerOpen));
      TestThat(TokensAreEqual(XmlTokens.At++, &ValueOpen));

      TestThat(TokensAreEqual(XmlTokens.At++, &ValueClose));
      TestThat(TokensAreEqual(XmlTokens.At++, &InnerClose));
      TestThat(TokensAreEqual(XmlTokens.At++, &OuterClose));
    }
    {
      TestThat(TokensAreEqual(XmlTokens.At++, &OuterOpen));
      TestThat(TokensAreEqual(XmlTokens.At++, &InnerOpen));
      TestThat(TokensAreEqual(XmlTokens.At++, &ValueOpen));

      TestThat(TokensAreEqual(XmlTokens.At++, &ValueClose));
      TestThat(TokensAreEqual(XmlTokens.At++, &InnerClose));
      TestThat(TokensAreEqual(XmlTokens.At++, &OuterClose));
    }
    {
      TestThat(TokensAreEqual(XmlTokens.At++, &OuterOpen));
      TestThat(TokensAreEqual(XmlTokens.At++, &InnerOpen));
      TestThat(TokensAreEqual(XmlTokens.At++, &ValueOpen));

      TestThat(TokensAreEqual(XmlTokens.At++, &ValueClose));
      TestThat(TokensAreEqual(XmlTokens.At++, &InnerClose));
      TestThat(TokensAreEqual(XmlTokens.At++, &OuterClose));
    }
    {
      TestThat(TokensAreEqual(XmlTokens.At++, &OuterOpen));
      TestThat(TokensAreEqual(XmlTokens.At++, &InnerOpen));
      TestThat(TokensAreEqual(XmlTokens.At++, &ValueOpen));

      TestThat(TokensAreEqual(XmlTokens.At++, &ValueClose));
      TestThat(TokensAreEqual(XmlTokens.At++, &InnerClose));
      TestThat(TokensAreEqual(XmlTokens.At++, &OuterClose));
    }
    {
      TestThat(TokensAreEqual(XmlTokens.At++, &OuterOpen));
      TestThat(TokensAreEqual(XmlTokens.At++, &InnerOpen));
      TestThat(TokensAreEqual(XmlTokens.At++, &ValueOpen));

      TestThat(TokensAreEqual(XmlTokens.At++, &ValueClose));
      TestThat(TokensAreEqual(XmlTokens.At++, &InnerClose));
      TestThat(TokensAreEqual(XmlTokens.At++, &OuterClose));
    }

    { // With the comment property
      TestThat(TokensAreEqual(XmlTokens.At++, &OuterOpen));
      TestThat(TokensAreEqual(XmlTokens.At++, &InnerOpen));

      TestThat(TokensAreEqual(XmlTokens.At++, &ValueOpen));
      TestThat(TokensAreEqual(XmlTokens.At++, &CommentProperty));
      TestThat(TokensAreEqual(XmlTokens.At++, &ValueClose));

      TestThat(TokensAreEqual(XmlTokens.At++, &ValueOpen));
      TestThat(TokensAreEqual(XmlTokens.At++, &ValueClose));

      TestThat(TokensAreEqual(XmlTokens.At++, &ValueOpen));
      TestThat(TokensAreEqual(XmlTokens.At++, &ValueClose));

      TestThat(TokensAreEqual(XmlTokens.At++, &InnerClose));
      TestThat(TokensAreEqual(XmlTokens.At++, &OuterClose));
    }
    { // With the comment property on a single line
      TestThat(TokensAreEqual(XmlTokens.At++, &OuterOpen));
      TestThat(TokensAreEqual(XmlTokens.At++, &InnerOpen));

      TestThat(TokensAreEqual(XmlTokens.At++, &ValueOpen));
      TestThat(TokensAreEqual(XmlTokens.At++, &CommentProperty));
      TestThat(TokensAreEqual(XmlTokens.At++, &ValueClose));

      TestThat(TokensAreEqual(XmlTokens.At++, &ValueOpen));
      TestThat(TokensAreEqual(XmlTokens.At++, &ValueClose));

      TestThat(TokensAreEqual(XmlTokens.At++, &ValueOpen));
      TestThat(TokensAreEqual(XmlTokens.At++, &ValueClose));

      TestThat(TokensAreEqual(XmlTokens.At++, &InnerClose));
      TestThat(TokensAreEqual(XmlTokens.At++, &OuterClose));
    }


    {
      xml_token BooleanProp = XmlBooleanToken(CS("boolean"));
      xml_token IntProp     = XmlIntToken(CS("int-property"), CS("42"));
      xml_token FloatProp   = XmlFloatToken(CS("float-property"), CS("42.42"));

      xml_token StringProp1   = XmlPropertyToken(CS("string-property-1"), CS("< text, yay! >"));
      xml_token StringProp2   = XmlPropertyToken(CS("string-property-2"), CS("> more text, yay! <"));


      TestThat(TokensAreEqual(XmlTokens.At++, &OuterOpen));

      TestThat(TokensAreEqual(XmlTokens.At++, &BooleanProp));
      TestThat(TokensAreEqual(XmlTokens.At++, &IntProp));
      TestThat(TokensAreEqual(XmlTokens.At++, &FloatProp));
      TestThat(TokensAreEqual(XmlTokens.At++, &StringProp1));
      TestThat(TokensAreEqual(XmlTokens.At++, &StringProp2));

      TestThat(TokensAreEqual(XmlTokens.At++, &InnerOpen));
      TestThat(TokensAreEqual(XmlTokens.At++, &ValueOpen));
      TestThat(TokensAreEqual(XmlTokens.At++, &ValueClose));
      TestThat(TokensAreEqual(XmlTokens.At++, &InnerClose));

      TestThat(TokensAreEqual(XmlTokens.At++, &OuterClose));
      TestThat(TokensAreEqual(XmlTokens.At++, &XmlClose));
    }

    {
      xml_token StringProp = XmlPropertyToken(CS("prop-name"), CS("prop-value\\\""));

      TestThat(TokensAreEqual(XmlTokens.At++, &OuterOpen));
      TestThat(TokensAreEqual(XmlTokens.At++, &StringProp));
      TestThat(TokensAreEqual(XmlTokens.At++, &OuterClose));
    }

    TestThat(AtElements(&XmlTokens) == TokenCount);
    Rewind(&XmlTokens);
  }

  VaporizeArena(Memory);

  return;
}

void
HashingTest()
{
  memory_arena *Memory = AllocateArena(Megabytes(1));
  AnsiStreamFromFile(TEST_FIXTURES_PATH "/test_parsing.dae", Memory);

  return;
}

void
XmlTests()
{
  xml_token RegularOpenToken = XmlOpenToken(CS("open"));

  TestThat(TokensAreEqual(&RegularOpenToken, &RegularOpenToken));
  TestThat(StringsMatch(RegularOpenToken.Property.Name, CS("open")));
  TestThat(RegularOpenToken.Property.Value.Count == 0);

  return;
}

void
ContrivedQueryingTest(const char* IdSelector)
{
  memory_arena *Memory = AllocateArena(Megabytes(1));

  ansi_stream XmlStream = AnsiStreamFromFile(TEST_FIXTURES_PATH "/test_querying.dae", Memory);
  xml_token_stream XmlTokens = TokenizeXmlStream(&XmlStream, Memory);
  counted_string TheMeaningOfLifeTheUniverseAndEverything = CS("42");
  counted_string FourtyTwoTwice = CS("42 42");

  {
    ansi_stream SelectorStream = AnsiStream("xml first-value");
    xml_token_stream Selector = TokenizeSelector(&SelectorStream, Memory);
    xml_tag* ResultTag = GetFirstMatchingTag(&XmlTokens, &Selector);
    xml_token OpenExpected = XmlOpenToken(CS("first-value"));
    TestThat(TokensAreEqual(ResultTag->Open, &OpenExpected));
    TestThat(StringsMatch(&ResultTag->Value, &FourtyTwoTwice));
  }

  {
    counted_string ActualIdValue = CS("id-value");
    ansi_stream SelectorStream = AnsiStream(FormatCountedString(Memory, CSz("xml first-value%sid-value"), IdSelector));
    xml_token_stream Selector = TokenizeSelector(&SelectorStream, Memory);
    xml_tag* ResultTag = GetFirstMatchingTag(&XmlTokens, &Selector);
    xml_token OpenExpected = XmlOpenToken(CS("first-value"));
    TestThat(TokensAreEqual(ResultTag->Open, &OpenExpected));
    TestThat(StringsMatch(&ResultTag->Value, &TheMeaningOfLifeTheUniverseAndEverything));

    counted_string* ResultIdValue = GetPropertyValue(ResultTag, CS("id"));
    TestThat(StringsMatch(ResultIdValue, ActualIdValue));
  }

  {
    ansi_stream SelectorStream = AnsiStream(FormatCountedString(Memory, CSz("xml first-value%sid-value"), IdSelector));
    xml_token_stream Selector = TokenizeSelector(&SelectorStream, Memory);
    xml_tag* ResultTag = GetFirstMatchingTag(&XmlTokens, &Selector);
    xml_token OpenExpected = XmlOpenToken(CS("first-value"));
    TestThat(TokensAreEqual(ResultTag->Open, &OpenExpected));
    TestThat(StringsMatch(&ResultTag->Value, &TheMeaningOfLifeTheUniverseAndEverything));
  }

  {
    ansi_stream SelectorStream = AnsiStream("xml outer inner second-value");
    xml_token_stream Selector = TokenizeSelector(&SelectorStream, Memory);
    xml_tag* ResultTag = GetFirstMatchingTag(&XmlTokens, &Selector);
    xml_token OpenExpected = XmlOpenToken(CS("second-value"));
    TestThat(TokensAreEqual(ResultTag->Open, &OpenExpected));
    TestThat(StringsMatch(&ResultTag->Value, &TheMeaningOfLifeTheUniverseAndEverything));
  }

  {
    ansi_stream SelectorStream = AnsiStream("xml third-value");
    xml_token_stream Selector = TokenizeSelector(&SelectorStream, Memory);
    xml_tag* ResultTag = GetFirstMatchingTag(&XmlTokens, &Selector);
    xml_token OpenExpected = XmlOpenToken(CS("third-value"));
    TestThat(TokensAreEqual(ResultTag->Open, &OpenExpected));
    TestThat(StringsMatch(&ResultTag->Value, &TheMeaningOfLifeTheUniverseAndEverything));
  }

  {
    ansi_stream SelectorStream = AnsiStream("xml outer inner fourth-value");
    xml_token_stream Selector = TokenizeSelector(&SelectorStream, Memory);
    xml_tag* ResultTag = GetFirstMatchingTag(&XmlTokens, &Selector);
    xml_token OpenExpected = XmlOpenToken(CS("fourth-value"));
    TestThat(TokensAreEqual(ResultTag->Open, &OpenExpected));
    TestThat(StringsMatch(&ResultTag->Value, &TheMeaningOfLifeTheUniverseAndEverything));
  }

  {
    ansi_stream SelectorStream = AnsiStream(FormatCountedString(Memory, CSz("xml outer inner%ssecond-fourth-value fourth-value"), IdSelector));
    xml_token_stream Selector = TokenizeSelector(&SelectorStream, Memory);
    xml_tag* ResultTag = GetFirstMatchingTag(&XmlTokens, &Selector);
    xml_token OpenExpected = XmlOpenToken(CS("fourth-value"));
    TestThat(TokensAreEqual(ResultTag->Open, &OpenExpected));
    TestThat(StringsMatch(&ResultTag->Value, &TheMeaningOfLifeTheUniverseAndEverything));
  }


  {
    ansi_stream SelectorStream = AnsiStream(FormatCountedString(Memory, CSz("xml library_effects effect%smaterial-effect color"), IdSelector));
    xml_token_stream Selector = TokenizeSelector(&SelectorStream, Memory);
    xml_tag* ResultTag = GetFirstMatchingTag(&XmlTokens, &Selector);
    xml_token OpenExpected = XmlOpenToken(CS("color"));
    TestThat(TokensAreEqual(ResultTag->Open, &OpenExpected));
    TestThat(StringsMatch(&ResultTag->Value, CS("1 2 3 4")));
  }

  {
    ansi_stream SelectorStream = AnsiStream(FormatCountedString(Memory, CSz("xml library_lights light%sLamp-light"), IdSelector));
    xml_token_stream Selector = TokenizeSelector(&SelectorStream, Memory);
    xml_tag* ResultTag = GetFirstMatchingTag(&XmlTokens, &Selector);
    xml_token OpenExpected = XmlOpenToken(CS("light"));
    TestThat(TokensAreEqual(ResultTag->Open, &OpenExpected));

    counted_string NameValue = CS("Lamp");
    counted_string* ResultNameValue = GetPropertyValue(ResultTag, CS("name"));
    TestThat(StringsMatch(NameValue, ResultNameValue));
  }

  {
    ansi_stream SelectorStream = AnsiStream(FormatCountedString(Memory, CSz("xml outer%sfoo target"), IdSelector));
    xml_token_stream Selector = TokenizeSelector(&SelectorStream, Memory);
    xml_tag* ResultTag = GetFirstMatchingTag(&XmlTokens, &Selector);
    TestThat(!ResultTag);
  }

  {
    ansi_stream SelectorStream = AnsiStream("inner fourth-value");
    xml_token_stream Selector = TokenizeSelector(&SelectorStream, Memory);
    xml_tag* ResultTag = GetFirstMatchingTag(&XmlTokens, &Selector);
    TestThat(ResultTag);
    TestThat(StringsMatch(ResultTag->Value, CS("42")));
  }

  {
    ansi_stream SelectorStream = AnsiStream(FormatCountedString(Memory, CSz("inner%ssecond-fourth-value fourth-value"), IdSelector));
    xml_token_stream Selector = TokenizeSelector(&SelectorStream, Memory);
    xml_tag* ResultTag = GetFirstMatchingTag(&XmlTokens, &Selector);
    TestThat(ResultTag);
    TestThat(StringsMatch(ResultTag->Value, CS("42")));
  }


  {
    ansi_stream SelectorStream = AnsiStream("second-value");
    xml_token_stream Selector = TokenizeSelector(&SelectorStream, Memory);
    xml_tag_stream Results = GetAllMatchingTags(&XmlTokens, &Selector, Memory);
    TestThat(TotalElements(&Results) == 2);
    TestThat(StringsMatch(Results.Start[0]->Value, CS("42")));
    TestThat(StringsMatch(Results.Start[1]->Value, CS("43")));
  }

  {
    ansi_stream SelectorStream = AnsiStream(FormatCountedString(Memory, CSz("first-value%sid-value:name=bar"), IdSelector));
    xml_token_stream Selector = TokenizeSelector(&SelectorStream, Memory);
    xml_tag* Result = GetFirstMatchingTag(&XmlTokens, &Selector);
    TestThat(Result);
    TestThat( StringsMatch(GetPropertyValue(Result, CS("name")), CS("bar")) );
  }

  {
    ansi_stream SelectorStream = AnsiStream("light:name=Lamp");
    xml_token_stream Selector = TokenizeSelector(&SelectorStream, Memory);
    xml_tag* Result = GetFirstMatchingTag(&XmlTokens, &Selector);
    TestThat(Result);
    TestThat( GetPropertyValue(Result, CS("id")) == 0);
  }

  {
    ansi_stream SelectorStream = AnsiStream(FormatCountedString(Memory, CSz("light%sLamp-light:name=Lamp"), IdSelector));
    xml_token_stream Selector = TokenizeSelector(&SelectorStream, Memory);
    xml_tag* Result = GetFirstMatchingTag(&XmlTokens, &Selector);
    TestThat(Result);
    TestThat( StringsMatch(GetPropertyValue(Result, CS("id")), CS("Lamp-light")) );
  }

  return;
}

void
BlenderCubeQueryTest(const char* IdSelector)
{
  memory_arena *Memory = AllocateArena(Megabytes(1));
  ansi_stream XmlStream = AnsiStreamFromFile(TEST_FIXTURES_PATH "/blender_cube.dae", Memory);
  xml_token_stream XmlTokens = TokenizeXmlStream(&XmlStream, Memory);

  {
    ansi_stream SelectorStream = AnsiStream(FormatCountedString(Memory, CSz("?xml COLLADA library_geometries geometry%sCube-mesh mesh source%sCube-mesh-positions float_array%sCube-mesh-positions-array"), IdSelector, IdSelector, IdSelector ));
    xml_token_stream Selector = TokenizeSelector(&SelectorStream, Memory);
    xml_tag* ResultTag = GetFirstMatchingTag(&XmlTokens, &Selector);
    TestThat( StringsMatch(ResultTag->Value, CS("1 1 -1 1 -1 -1 -1 -0.9999998 -1 -0.9999997 1 -1 1 0.9999995 1 0.9999994 -1.000001 1 -1 -0.9999997 1 -1 1 1") ) );
  }

  {
    ansi_stream SelectorStream = AnsiStream(FormatCountedString(Memory, CSz("?xml COLLADA library_geometries geometry%sCube-mesh mesh source%sCube-mesh-normals float_array%sCube-mesh-normals-array"), IdSelector, IdSelector, IdSelector));
    xml_token_stream Selector = TokenizeSelector(&SelectorStream, Memory);
    xml_tag* ResultTag = GetFirstMatchingTag(&XmlTokens, &Selector);
    TestThat( StringsMatch(ResultTag->Value, CS("0 0 -1 0 0 1 1 0 -2.38419e-7 0 -1 -4.76837e-7 -1 2.38419e-7 -1.49012e-7 2.68221e-7 1 2.38419e-7 0 0 -1 0 0 1 1 -5.96046e-7 3.27825e-7 -4.76837e-7 -1 0 -1 2.38419e-7 -1.19209e-7 2.08616e-7 1 0") ) );
  }

  {
    ansi_stream SelectorStream = AnsiStream(FormatCountedString(Memory, CSz("?xml COLLADA library_effects effect%sMaterial-effect profile_COMMON technique phong emission color"), IdSelector));
    xml_token_stream Selector = TokenizeSelector(&SelectorStream, Memory);
    xml_tag* ResultTag = GetFirstMatchingTag(&XmlTokens, &Selector);
    TestThat( StringsMatch(ResultTag->Value, CS("0 0 0 1") ) );
  }

  {
    ansi_stream SelectorStream = AnsiStream(FormatCountedString(Memory, CSz("?xml COLLADA library_effects effect%sMaterial-effect profile_COMMON technique phong ambient color"), IdSelector));
    xml_token_stream Selector = TokenizeSelector(&SelectorStream, Memory);
    xml_tag* ResultTag = GetFirstMatchingTag(&XmlTokens, &Selector);
    TestThat( StringsMatch(ResultTag->Value, CS("0 0 0 1") ) );
  }

  {
    ansi_stream SelectorStream = AnsiStream(FormatCountedString(Memory, CSz("?xml COLLADA library_effects effect%sMaterial-effect profile_COMMON technique phong shininess float"), IdSelector));
    xml_token_stream Selector = TokenizeSelector(&SelectorStream, Memory);
    xml_tag* ResultTag = GetFirstMatchingTag(&XmlTokens, &Selector);
    TestThat( StringsMatch(ResultTag->Value, CS("50") ) );
  }

  {
    ansi_stream SelectorStream = AnsiStream(FormatCountedString(Memory, CSz("?xml COLLADA library_effects effect%sMaterial-effect profile_COMMON technique phong index_of_refraction float"), IdSelector));
    xml_token_stream Selector = TokenizeSelector(&SelectorStream, Memory);
    xml_tag* ResultTag = GetFirstMatchingTag(&XmlTokens, &Selector);
    TestThat( StringsMatch(ResultTag->Value, CS("1") ) );
  }

  {
    ansi_stream SelectorStream = AnsiStream(FormatCountedString(Memory, CSz("?xml COLLADA library_visual_scenes visual_scene%sScene node%sCube instance_geometry bind_material technique_common instance_material"), IdSelector, IdSelector));
    xml_token_stream Selector = TokenizeSelector(&SelectorStream, Memory);
    xml_tag* ResultTag = GetFirstMatchingTag(&XmlTokens, &Selector);
    TestThat(ResultTag);
  }

  {
    ansi_stream SelectorStream = AnsiStream(FormatCountedString(Memory, CSz("?xml COLLADA library_visual_scenes visual_scene%sScene node%sCube instance_geometry bind_material technique_common"), IdSelector, IdSelector));
    xml_token_stream Selector = TokenizeSelector(&SelectorStream, Memory);
    xml_tag* ResultTag = GetFirstMatchingTag(&XmlTokens, &Selector);
    xml_token Expected = XmlOpenToken(CS("technique_common"));
    TestThat(TokensAreEqual(ResultTag->Open, &Expected));
    TestThat(ResultTag);
  }

  {
    ansi_stream SelectorStream = AnsiStream(FormatCountedString(Memory, CSz("?xml COLLADA library_lights light%sLamp-light extra technique ray_samp"), IdSelector));
    xml_token_stream Selector = TokenizeSelector(&SelectorStream, Memory);
    xml_tag* ResultTag = GetFirstMatchingTag(&XmlTokens, &Selector);
    xml_token Expected = XmlOpenToken(CS("ray_samp"));
    TestThat(TokensAreEqual(ResultTag->Open, &Expected));
    TestThat(StringsMatch(ResultTag->Value, CS("1")));
  }

  return;
}

s32
main()
{
  TestSuiteBegin("Collada Loader");

  XmlTests();

  TokenizingTest();

  HashingTest();

  const char* IdSelectorFormat[] = { ":id=", "#" };
  for (u32 IdSelectorIndex  = 0;
      IdSelectorIndex < ArrayCount(IdSelectorFormat);
      ++IdSelectorIndex )
  {
    ContrivedQueryingTest(IdSelectorFormat[IdSelectorIndex]);
    BlenderCubeQueryTest(IdSelectorFormat[IdSelectorIndex]);
  }

  memory_arena* Memory = AllocateArena();
  heap_allocator Heap = InitHeap(Gigabytes(1));
  LoadCollada(Memory, &Heap, TEST_FIXTURES_PATH "/blender_cube.dae");

  /* LoadCollada(Memory, &Heap, "models/one_thousand_museum.dae"); */

  TestSuiteEnd();
  exit(TestsFailed);
}
