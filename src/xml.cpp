b32
operator==(xml_token T1, xml_token T2)
{
  b32 Result = False;

  if (T1.Type == T2.Type)
  {
    switch (T1.Type)
    {
      case XmlTokenType_Float:
      case XmlTokenType_Int:
      case XmlTokenType_Property:
      {
        Result = StringsMatch(&T1.Property.Name, &T2.Property.Name) &&
                 StringsMatch(&T1.Property.Value, &T2.Property.Value);
      } break;

      case XmlTokenType_OpenTag:
      case XmlTokenType_CloseTag:
      case XmlTokenType_Boolean:
      {
        Assert(!T1.Property.Value.Count);
        Assert(!T2.Property.Value.Count);
        Result = StringsMatch(&T1.Property.Name, &T2.Property.Name);
      } break;

      InvalidDefaultCase;
    }
  }
  return Result;
}

xml_tag
GetFirstMatchingTag(xml_token_stream *XmlStream, ansi_stream* SelectorStream, counted_string *ParentSelectorName)
{
  xml_tag Result = {};


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

inline void
Rewind(xml_token_stream *Stream)
{
  Stream->At = Stream->Start;
  return;
}
