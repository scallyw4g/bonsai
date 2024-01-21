link_internal counted_string
ToString(text_encoding Type)
{
  counted_string Result = {};
  switch (Type)
  {
    case TextEncoding_ASCII: { Result = CSz("ASCII"); } break;
    case TextEncoding_UTF8: { Result = CSz("UTF8"); } break;
    case TextEncoding_UTF16LE: { Result = CSz("UTF16LE"); } break;
    case TextEncoding_UTF16BE: { Result = CSz("UTF16BE"); } break;
    case TextEncoding_UTF32LE: { Result = CSz("UTF32LE"); } break;
    case TextEncoding_UTF32BE: { Result = CSz("UTF32BE"); } break;
    case TextEncoding_UTF7: { Result = CSz("UTF7"); } break;
    case TextEncoding_UTF1: { Result = CSz("UTF1"); } break;
    case TextEncoding_EBCDIC: { Result = CSz("EBCDIC"); } break;
    case TextEncoding_CSCU: { Result = CSz("CSCU"); } break;
    case TextEncoding_BOCU: { Result = CSz("BOCU"); } break;
    case TextEncoding_GB18030: { Result = CSz("GB18030"); } break;

    
  }
  return Result;
}

