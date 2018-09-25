
xml_tag
AdvanceStreamToEndOfOpeningTag(ansi_stream *XmlStream, counted_string XmlStreamTag)
{
  xml_tag Result = {};

  Result.Selector = XmlStreamTag;
  Result.OpeningStartOffset = XmlStreamTag.Start - XmlStream->Start;

  while (Remaining(XmlStream))
  {
    counted_string EndTag = PopWordCounted(XmlStream);
    if (EndTag.Start[EndTag.Count-1] == '>')
    {
      // Result.OpeningEndOffset = (EndTag.Start - XmlStream->Start) - Result.OpeningStartOffset + EndTag.Count;
      break;
    }
  }

  return Result;
}

xml_tag
GetFirstMatchingTag(ansi_stream *XmlStream, ansi_stream* SelectorStream, counted_string *ParentSelectorName)
{
  xml_tag Result = {};

#if 0
  counted_string SearchSelector = PopWordCounted(SelectorStream);

  if (!SearchSelector.Count )
    return Result;

  while (Remaining(XmlStream))
  {
    counted_string MatchTagName = PopXmlTag(XmlStream);

    if (StringsMatch(&SearchSelector, &MatchTagName))
    {
      Result.Selector = MatchTagName;
      // NOTE(Jesse): This omits the opening '<' in favor of not having to add
      // it every time we pop a selector off the selector stream.  Furthermore,
      // having any whitespace between the slash and the ending tag name breaks
      // this.  FURTHERMORE, the end tag brace is not included..feelsbadman
      char* ParentClosingTag = FormatString("/%.*s", SearchSelector.Count, SearchSelector.Start);
      counted_string Parent = CountedString(ParentClosingTag);

      xml_tag ChildTag = GetFirstMatchingTag(XmlStream, SelectorStream, &Parent);
      if (ChildTag.Selector.Count)
      {
        Result = ChildTag;
      }
    }

    if (StringsMatch(ParentSelectorName, &MatchTagName))
    {
      break;
    }
  }
#endif

  return Result;
}

xml_token_stream
TokenizeXmlStream(ansi_stream* Xml, memory_arena* Memory)
{
  xml_token_stream Result = {};

  EatWhitespace(Xml);
  if (*Xml->At != '<')
  {
    Error("Invalid XML Stream");
    return Result;;
  }
  // TODO(Jesse): Better way of allocating this?
  Result = AllocateXmlTokenStream(10000, Memory);

  while ( Remaining(Xml) )
  {
    const char* NameDelimeters = "\n> ";

    ++Xml->At; // Skip the first '<'

    b32 ClosingTag = *Xml->At == '/';
    ClosingTag ?  ++Xml->At : Xml->At ;

    counted_string TagName = CountedString(ReadUntilTerminatorList(Xml, NameDelimeters, Memory));

    if (ClosingTag)
    {
      PushToken(&Result, XmlCloseTag(TagName));
    }
    else
    {
      PushToken(&Result, XmlOpenTag(TagName));
    }

    const char* PropertyDelimeters = "\n> =";
    while ( *(Xml->At-1) != '>' )
    {
      EatWhitespace(Xml);
      counted_string PropertyName = CountedString(ReadUntilTerminatorList(Xml, PropertyDelimeters, Memory));

      if (PropertyName.Count)
      {
        char TerminatorFound = *(Xml->At-1);
        char NextTokenStartChar = *Xml->At;

        switch(TerminatorFound)
        {
          case '>':
          case ' ':
          case '\n':
          {
            if (PropertyName.Count == 1 && PropertyName.Start[0] == '/')
            {
              PushToken(&Result, XmlCloseTag(TagName));
            }
            else
            {
              PushToken(&Result, XmlBooleanProperty(PropertyName) );
            }
          } break;

          case '=':
          {
            switch(NextTokenStartChar)
            {
              case '"':
              case '\'':
              {
                counted_string PropValue = PopQuotedString(Xml, Memory);
                PushToken(&Result, XmlStringProperty(PropertyName, PropValue));
              } break;

              default:
              {
                counted_string PropValue = PopWordCounted(Xml);
                if( Contains(PropValue, '.') )
                  PushToken(&Result, XmlFloatProperty(PropertyName, PropValue));
                else
                  PushToken(&Result, XmlIntProperty(PropertyName, PropValue));
              } break;

            }
          } break;

          InvalidDefaultCase;
        }
      }

    }

    EatWhitespace(Xml);
  }

  return Result;
}
