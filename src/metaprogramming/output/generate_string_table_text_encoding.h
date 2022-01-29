    bonsai_function counted_string
    ToString( text_encoding Type)
    {
      counted_string Result = {};
      switch (Type)
      {
                    case TextEncoding_ASCII: { Result = CSz("TextEncoding_ASCII"); } break;
            case TextEncoding_UTF8: { Result = CSz("TextEncoding_UTF8"); } break;
            case TextEncoding_UTF16LE: { Result = CSz("TextEncoding_UTF16LE"); } break;
            case TextEncoding_UTF16BE: { Result = CSz("TextEncoding_UTF16BE"); } break;
            case TextEncoding_UTF32LE: { Result = CSz("TextEncoding_UTF32LE"); } break;
            case TextEncoding_UTF32BE: { Result = CSz("TextEncoding_UTF32BE"); } break;
            case TextEncoding_UTF7: { Result = CSz("TextEncoding_UTF7"); } break;
            case TextEncoding_UTF1: { Result = CSz("TextEncoding_UTF1"); } break;
            case TextEncoding_EBCDIC: { Result = CSz("TextEncoding_EBCDIC"); } break;
            case TextEncoding_CSCU: { Result = CSz("TextEncoding_CSCU"); } break;
            case TextEncoding_BOCU: { Result = CSz("TextEncoding_BOCU"); } break;
            case TextEncoding_GB18030: { Result = CSz("TextEncoding_GB18030"); } break;

      }
      return Result;
    }

