
    function counted_string
    ToString(c_token_type Type)
    {
      counted_string Result = {};
      switch (Type)
      {
        
          case CTokenType_Unknown: { Result = CSz("CTokenType_Unknown"); } break;

          case CTokenType_CommentSingleLine: { Result = CSz("CTokenType_CommentSingleLine"); } break;

          case CTokenType_CommentMultiLineStart: { Result = CSz("CTokenType_CommentMultiLineStart"); } break;

          case CTokenType_CommentMultiLineEnd: { Result = CSz("CTokenType_CommentMultiLineEnd"); } break;

          case CTokenType_Identifier: { Result = CSz("CTokenType_Identifier"); } break;

          case CTokenType_String: { Result = CSz("CTokenType_String"); } break;

          case CTokenType_Char: { Result = CSz("CTokenType_Char"); } break;

          case CTokenType_OpenBracket: { Result = CSz("CTokenType_OpenBracket"); } break;

          case CTokenType_CloseBracket: { Result = CSz("CTokenType_CloseBracket"); } break;

          case CTokenType_OpenBrace: { Result = CSz("CTokenType_OpenBrace"); } break;

          case CTokenType_CloseBrace: { Result = CSz("CTokenType_CloseBrace"); } break;

          case CTokenType_OpenParen: { Result = CSz("CTokenType_OpenParen"); } break;

          case CTokenType_CloseParen: { Result = CSz("CTokenType_CloseParen"); } break;

          case CTokenType_Dot: { Result = CSz("CTokenType_Dot"); } break;

          case CTokenType_Comma: { Result = CSz("CTokenType_Comma"); } break;

          case CTokenType_Semicolon: { Result = CSz("CTokenType_Semicolon"); } break;

          case CTokenType_Colon: { Result = CSz("CTokenType_Colon"); } break;

          case CTokenType_Hash: { Result = CSz("CTokenType_Hash"); } break;

          case CTokenType_At: { Result = CSz("CTokenType_At"); } break;

          case CTokenType_Dollar: { Result = CSz("CTokenType_Dollar"); } break;

          case CTokenType_Space: { Result = CSz("CTokenType_Space"); } break;

          case CTokenType_Star: { Result = CSz("CTokenType_Star"); } break;

          case CTokenType_Ampersand: { Result = CSz("CTokenType_Ampersand"); } break;

          case CTokenType_SingleQuote: { Result = CSz("CTokenType_SingleQuote"); } break;

          case CTokenType_DoubleQuote: { Result = CSz("CTokenType_DoubleQuote"); } break;

          case CTokenType_Equals: { Result = CSz("CTokenType_Equals"); } break;

          case CTokenType_LT: { Result = CSz("CTokenType_LT"); } break;

          case CTokenType_GT: { Result = CSz("CTokenType_GT"); } break;

          case CTokenType_Plus: { Result = CSz("CTokenType_Plus"); } break;

          case CTokenType_Minus: { Result = CSz("CTokenType_Minus"); } break;

          case CTokenType_Percent: { Result = CSz("CTokenType_Percent"); } break;

          case CTokenType_Bang: { Result = CSz("CTokenType_Bang"); } break;

          case CTokenType_Hat: { Result = CSz("CTokenType_Hat"); } break;

          case CTokenType_Question: { Result = CSz("CTokenType_Question"); } break;

          case CTokenType_FSlash: { Result = CSz("CTokenType_FSlash"); } break;

          case CTokenType_BSlash: { Result = CSz("CTokenType_BSlash"); } break;

          case CTokenType_Tilde: { Result = CSz("CTokenType_Tilde"); } break;

          case CTokenType_Backtick: { Result = CSz("CTokenType_Backtick"); } break;

          case CTokenType_Pipe: { Result = CSz("CTokenType_Pipe"); } break;

          case CTokenType_Newline: { Result = CSz("CTokenType_Newline"); } break;

          case CTokenType_CarrigeReturn: { Result = CSz("CTokenType_CarrigeReturn"); } break;

          case CTokenType_EOF: { Result = CSz("CTokenType_EOF"); } break;

      }
      return Result;
    }

