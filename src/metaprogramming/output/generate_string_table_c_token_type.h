
    function counted_string
    ToString( c_token_type Type)
    {
      counted_string Result = {};
      switch (Type)
      {
        
            case CTokenType_Unknown: { Result = CSz("CTokenType_Unknown"); } break;

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

            case CTokenType_CommentSingleLine: { Result = CSz("CTokenType_CommentSingleLine"); } break;

            case CTokenType_CommentMultiLineStart: { Result = CSz("CTokenType_CommentMultiLineStart"); } break;

            case CTokenType_CommentMultiLineEnd: { Result = CSz("CTokenType_CommentMultiLineEnd"); } break;

            case CTokenType_EscapedNewline: { Result = CSz("CTokenType_EscapedNewline"); } break;

            case CTokenType_Identifier: { Result = CSz("CTokenType_Identifier"); } break;

            case CTokenType_StringLiteral: { Result = CSz("CTokenType_StringLiteral"); } break;

            case CTokenType_CharLiteral: { Result = CSz("CTokenType_CharLiteral"); } break;

            case CTokenType_IntLiteral: { Result = CSz("CTokenType_IntLiteral"); } break;

            case CTokenType_DoubleLiteral: { Result = CSz("CTokenType_DoubleLiteral"); } break;

            case CTokenType_FloatLiteral: { Result = CSz("CTokenType_FloatLiteral"); } break;

            case CTokenType_Meta: { Result = CSz("CTokenType_Meta"); } break;

            case CTokenType_Struct: { Result = CSz("CTokenType_Struct"); } break;

            case CTokenType_Enum: { Result = CSz("CTokenType_Enum"); } break;

            case CTokenType_Union: { Result = CSz("CTokenType_Union"); } break;

            case CTokenType_Typedef: { Result = CSz("CTokenType_Typedef"); } break;

            case CTokenType_Using: { Result = CSz("CTokenType_Using"); } break;

            case CTokenType_ThreadLocal: { Result = CSz("CTokenType_ThreadLocal"); } break;

            case CTokenType_Const: { Result = CSz("CTokenType_Const"); } break;

            case CTokenType_Static: { Result = CSz("CTokenType_Static"); } break;

            case CTokenType_Volatile: { Result = CSz("CTokenType_Volatile"); } break;

            case CTokenType_Long: { Result = CSz("CTokenType_Long"); } break;

            case CTokenType_Unsigned: { Result = CSz("CTokenType_Unsigned"); } break;

            case CTokenType_Signed: { Result = CSz("CTokenType_Signed"); } break;

            case CTokenType_Bool: { Result = CSz("CTokenType_Bool"); } break;

            case CTokenType_M128: { Result = CSz("CTokenType_M128"); } break;

            case CTokenType_Auto: { Result = CSz("CTokenType_Auto"); } break;

            case CTokenType_Void: { Result = CSz("CTokenType_Void"); } break;

            case CTokenType_Double: { Result = CSz("CTokenType_Double"); } break;

            case CTokenType_Float: { Result = CSz("CTokenType_Float"); } break;

            case CTokenType_Char: { Result = CSz("CTokenType_Char"); } break;

            case CTokenType_Int: { Result = CSz("CTokenType_Int"); } break;

            case CTokenType_Short: { Result = CSz("CTokenType_Short"); } break;

            case CTokenType_Inline: { Result = CSz("CTokenType_Inline"); } break;

            case CTokenType_TemplateKeyword: { Result = CSz("CTokenType_TemplateKeyword"); } break;

            case CTokenType_OperatorKeyword: { Result = CSz("CTokenType_OperatorKeyword"); } break;

            case CTokenType_Extern: { Result = CSz("CTokenType_Extern"); } break;

            case CTokenType_Asm: { Result = CSz("CTokenType_Asm"); } break;

            case CTokenType_Goto: { Result = CSz("CTokenType_Goto"); } break;

            case CTokenType_Ellipsis: { Result = CSz("CTokenType_Ellipsis"); } break;

            case CTokenType_If: { Result = CSz("CTokenType_If"); } break;

            case CTokenType_Else: { Result = CSz("CTokenType_Else"); } break;

            case CTokenType_Break: { Result = CSz("CTokenType_Break"); } break;

            case CTokenType_Switch: { Result = CSz("CTokenType_Switch"); } break;

            case CTokenType_Case: { Result = CSz("CTokenType_Case"); } break;

            case CTokenType_Default: { Result = CSz("CTokenType_Default"); } break;

            case CTokenType_For: { Result = CSz("CTokenType_For"); } break;

            case CTokenType_While: { Result = CSz("CTokenType_While"); } break;

            case CTokenType_Continue: { Result = CSz("CTokenType_Continue"); } break;

            case CTokenType_Return: { Result = CSz("CTokenType_Return"); } break;

            case CTokenType_LeftShift: { Result = CSz("CTokenType_LeftShift"); } break;

            case CTokenType_RightShift: { Result = CSz("CTokenType_RightShift"); } break;

            case CTokenType_LessEqual: { Result = CSz("CTokenType_LessEqual"); } break;

            case CTokenType_GreaterEqual: { Result = CSz("CTokenType_GreaterEqual"); } break;

            case CTokenType_AreEqual: { Result = CSz("CTokenType_AreEqual"); } break;

            case CTokenType_NotEqual: { Result = CSz("CTokenType_NotEqual"); } break;

            case CTokenType_PlusEquals: { Result = CSz("CTokenType_PlusEquals"); } break;

            case CTokenType_MinusEquals: { Result = CSz("CTokenType_MinusEquals"); } break;

            case CTokenType_TimesEquals: { Result = CSz("CTokenType_TimesEquals"); } break;

            case CTokenType_DivEquals: { Result = CSz("CTokenType_DivEquals"); } break;

            case CTokenType_ModEquals: { Result = CSz("CTokenType_ModEquals"); } break;

            case CTokenType_AndEquals: { Result = CSz("CTokenType_AndEquals"); } break;

            case CTokenType_OrEquals: { Result = CSz("CTokenType_OrEquals"); } break;

            case CTokenType_XorEquals: { Result = CSz("CTokenType_XorEquals"); } break;

            case CTokenType_Increment: { Result = CSz("CTokenType_Increment"); } break;

            case CTokenType_Decrement: { Result = CSz("CTokenType_Decrement"); } break;

            case CTokenType_LogicalAnd: { Result = CSz("CTokenType_LogicalAnd"); } break;

            case CTokenType_LogicalOr: { Result = CSz("CTokenType_LogicalOr"); } break;

            case CTokenType_Arrow: { Result = CSz("CTokenType_Arrow"); } break;

      }
      return Result;
    }

