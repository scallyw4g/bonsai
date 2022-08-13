#define EXCLUDE_PREPROCESSOR_MAIN 1
#include <poof/preprocessor.cpp>
#include <tests/test_utils.cpp>
#include <tests/bug_defines.h>

#if 1
bonsai_function void
PositiveExponentTests(parser *Parser, c_token_type FloatingPointTokenType)
{
  {
    c_token T = PopToken(Parser);
    TestThat(T.Type == FloatingPointTokenType);
    TestThat(T.FloatValue == 42.0);
  }
  {
    c_token T = PopToken(Parser);
    TestThat(T.Type == FloatingPointTokenType);
    TestThat(T.FloatValue == 42.0 * 42.0);
  }

  {
    c_token T = PopToken(Parser);
    TestThat(T.Type == FloatingPointTokenType);
    TestThat(T.FloatValue == 42.0);
  }
  {
    c_token T = PopToken(Parser);
    TestThat(T.Type == FloatingPointTokenType);
    TestThat(T.FloatValue == 42.0 * 42.0);
  }

  {
    c_token T = PopToken(Parser);
    TestThat(T.Type == FloatingPointTokenType);
    TestThat(T.FloatValue == 42.0);
  }
  {
    c_token T = PopToken(Parser);
    TestThat(T.Type == FloatingPointTokenType);
    TestThat(T.FloatValue == 42.0 * 42.0);
  }
  {
    c_token T = PopToken(Parser);
    TestThat(T.Type == FloatingPointTokenType);
    TestThat(T.FloatValue == 1.0);
  }
  {
    c_token T = PopToken(Parser);
    TestThat(T.Type == FloatingPointTokenType);
    TestThat(T.FloatValue == 0.42);
  }
  {
    c_token T = PopToken(Parser);
    TestThat(T.Type == FloatingPointTokenType);
    TestThat(T.FloatValue == 0.42 * 0.42);
  }

  return;
}

bonsai_function void
ExponentTests(parser *Parser, c_token_type FloatingPointTokenType)
{

  PositiveExponentTests(Parser, FloatingPointTokenType); // Implicit positive exponent

  PositiveExponentTests(Parser, FloatingPointTokenType); // Explicit positive exponent

  //
  // Negative Exponent tests
  //

  {
    c_token T = PopToken(Parser);
    TestThat(T.Type == FloatingPointTokenType);
    TestThat(T.FloatValue == 1.0/42.0);
  }
  {
    c_token T = PopToken(Parser);
    TestThat(T.Type == FloatingPointTokenType);
    TestThat(T.FloatValue == (1.0/42.0)/42.0);
  }

  {
    c_token T = PopToken(Parser);
    TestThat(T.Type == FloatingPointTokenType);
    TestThat(T.FloatValue == 1.0/42.0);
  }
  {
    c_token T = PopToken(Parser);
    TestThat(T.Type == FloatingPointTokenType);
    TestThat(T.FloatValue == (1.0/42.0)/42.0);
  }


  {
    c_token T = PopToken(Parser);
    TestThat(T.Type == FloatingPointTokenType);
    TestThat(T.FloatValue == 1.0/42.0);
  }
  {
    c_token T = PopToken(Parser);
    TestThat(T.Type == FloatingPointTokenType);
    TestThat(T.FloatValue == (1.0/42.0)/42.0);
  }

  {
    c_token T = PopToken(Parser);
    TestThat(T.Type == FloatingPointTokenType);
    TestThat(T.FloatValue == 1.0/0.42);
  }
  {
    c_token T = PopToken(Parser);
    TestThat(T.Type == FloatingPointTokenType);
    TestThat(T.FloatValue == (1.0/0.42)/0.42);
  }


  return;
}

bonsai_function void
TestBasicTokenizationAndParsing(memory_arena* Memory)
{
  parse_context Ctx = AllocateParseContext(Memory);
  parser *Parser = PreprocessedParserForFile(&Ctx, CS(TEST_FIXTURES_PATH "/preprocessor_basic.cpp"), TokenCursorSource_RootFile, 0);

  if (Parser)
  {
#if 1
    {
      c_token T = PopTokenRaw(Parser);
      TestThat(T.Type == CTokenType_CommentSingleLine);
    }

    {
      c_token T = PopTokenRaw(Parser);
      TestThat(T.Type == CTokenType_Newline);
    }

    {
      c_token T = PopTokenRaw(Parser);
      TestThat(T.Type == CTokenType_CommentMultiLine);
    }
#endif



    {
      c_token T = PopToken(Parser);
      TestThat(T == CToken(CS("bonsai_function")));
    }
    {
      c_token T = PopToken(Parser);
      TestThat(T.Type == CTokenType_Void);
    }
    {
      c_token T = PopToken(Parser);
      TestThat(T == CToken(CS("FunctionName")));
    }
    {
      c_token T = PopToken(Parser);
      TestThat(T.Type == CTokenType_OpenParen);
    }
    {
      c_token T = PopToken(Parser);
      TestThat(T.Type == CTokenType_CloseParen);
    }
    {
      c_token T = PopToken(Parser);
      TestThat(T.Type == CTokenType_OpenBrace);
    }
    {
      c_token T = PopToken(Parser);
      TestThat(T == CToken(CS("counted_string")));
    }
    {
      c_token T = PopToken(Parser);
      TestThat(T == CToken(CS("S")));
    }
    {
      c_token T = PopToken(Parser);
      TestThat(T.Type == CTokenType_Equals);
    }
    {
      c_token T = PopToken(Parser);
      TestThat(T == CToken(CS("CS")));
    }
    {
      c_token T = PopToken(Parser);
      TestThat(T.Type == CTokenType_OpenParen);
    }
    {
      c_token T = PopToken(Parser);
      TestThat(T == CToken(CTokenType_StringLiteral, CS("\"thing\\n\"")));
    }
    {
      c_token T = PopToken(Parser);
      TestThat(T.Type == CTokenType_CloseParen);
    }
    {
      c_token T = PopToken(Parser);
      TestThat(T.Type == CTokenType_Semicolon);
    }
    {
      c_token T = PopToken(Parser);
      TestThat(T.Type == CTokenType_Int);
    }
    {
      c_token T = PopToken(Parser);
      TestThat(T == CToken(CS("foo")));
    }
    {
      c_token T = PopToken(Parser);
      TestThat(T.Type == CTokenType_Equals);
    }
    {
      c_token T = PopToken(Parser);
      TestThat(T.Type == CTokenType_IntLiteral);
      TestThat(T.UnsignedValue == 3);
    }
    {
      c_token T = PopToken(Parser);
      TestThat(T.Type == CTokenType_FSlash);
    }
    {
      c_token T = PopToken(Parser);
      TestThat(T.Type == CTokenType_IntLiteral);
      TestThat(T.UnsignedValue == 3);
    }
    {
      c_token T = PopToken(Parser);
      TestThat(T.Type == CTokenType_Semicolon);
    }
    {
      c_token T = PopToken(Parser);
      TestThat(T.Type == CTokenType_Return);
    }
    {
      c_token T = PopToken(Parser);
      TestThat(T.Type == CTokenType_Semicolon);
    }
    {
      c_token T = PopToken(Parser);
      TestThat(T.Type == CTokenType_CloseBrace);
    }

    {
      c_token T = PopToken(Parser);
      TestThat(T == CToken(CS("bonsai_function")));
    }
    {
      c_token T = PopToken(Parser);
      TestThat(T.Type == CTokenType_Void);
    }
    {
      c_token T = PopToken(Parser);
      TestThat(T == CToken(CS("WriteStructTo")));
    }
    {
      c_token T = PopToken(Parser);
      TestThat(T.Type == CTokenType_OpenParen);
    }
    {
      c_token T = PopToken(Parser);
      TestThat(T == CToken(CS("d_union_decl")));
    }
    {
      c_token T = PopToken(Parser);
      TestThat(T.Type == CTokenType_Star);
    }
    {
      c_token T = PopToken(Parser);
      TestThat(T == CToken(CS("dUnion")));
    }
    {
      c_token T = PopToken(Parser);
      TestThat(T.Type == CTokenType_Comma);
    }
    {
      c_token T = PopToken(Parser);
      TestThat(T == CToken(CS("native_file")));
    }
    {
      c_token T = PopToken(Parser);
      TestThat(T.Type == CTokenType_Star);
    }
    {
      c_token T = PopToken(Parser);
      TestThat(T == CToken(CS("OutFile")));
    }
    {
      c_token T = PopToken(Parser);
      TestThat(T.Type == CTokenType_Comma);
    }
    {
      c_token T = PopToken(Parser);
      TestThat(T == CToken(CS("memory_arena")));
    }
    {
      c_token T = PopToken(Parser);
      TestThat(T.Type == CTokenType_Star);
    }
    {
      c_token T = PopToken(Parser);
      TestThat(T == CToken(CS("Memory")));
    }
    {
      c_token T = PopToken(Parser);
      TestThat(T.Type == CTokenType_CloseParen);
    }
    {
      c_token T = PopToken(Parser);
      TestThat(T.Type == CTokenType_OpenBrace);
    }
    {
      c_token T = PopToken(Parser);
      TestThat(T == CToken(CS("counted_string")));
    }
    {
      c_token T = PopToken(Parser);
      TestThat(T == CToken(CS("UnionName")));
    }
    {
      c_token T = PopToken(Parser);
      TestThat(T.Type == CTokenType_Equals);
    }
    {
      c_token T = PopToken(Parser);
      TestThat(T == CToken(CS("dUnion")));
    }
    {
      c_token T = PopToken(Parser);
      TestThat(T.Type == CTokenType_Arrow);
    }
    {
      c_token T = PopToken(Parser);
      TestThat(T == CToken(CS("Name")));
    }
    {
      c_token T = PopToken(Parser);
      TestThat(T.Type == CTokenType_Semicolon);
    }
    {
      c_token T = PopToken(Parser);
      TestThat(T == CToken(CS("counted_string")));
    }
    {
      c_token T = PopToken(Parser);
      TestThat(T == CToken(CS("Decl")));
    }
    {
      c_token T = PopToken(Parser);
      TestThat(T.Type == CTokenType_Equals);
    }
    {
      c_token T = PopToken(Parser);
      TestThat(T == CToken(CS("FormatCountedString")));
    }
    {
      c_token T = PopToken(Parser);
      TestThat(T.Type == CTokenType_OpenParen);
    }
    {
      c_token T = PopToken(Parser);
      TestThat(T == CToken(CS("Memory")));
    }
    {
      c_token T = PopToken(Parser);
      TestThat(T.Type == CTokenType_Comma);
    }
    {
      c_token T = PopToken(Parser);
      TestThat(T == CToken(CTokenType_StringLiteral, CS("\"\\n\"")));
    }
    {
      c_token T = PopToken(Parser);
      TestThat(T.Type == CTokenType_CloseParen);
    }
    {
      c_token T = PopToken(Parser);
      TestThat(T.Type == CTokenType_Semicolon);
    }
    {
      c_token T = PopToken(Parser);
      TestThat(T.Type == CTokenType_CloseBrace);
    }

    //
    // Integral contant tests
    //

    for (u32 Count = 0;
        Count < 15;
        ++Count)
    {
      c_token T = PopToken(Parser);
      TestThat(T.Type == CTokenType_IntLiteral);
      TestThat(T.UnsignedValue == 42);
    }

    //
    // Hex constant tests
    //

    {
      c_token T = PopToken(Parser);
      TestThat(T.Type == CTokenType_IntLiteral);
      TestThat(T.UnsignedValue == 1);
    }
    {
      c_token T = PopToken(Parser);
      TestThat(T.Type == CTokenType_IntLiteral);
      TestThat(T.UnsignedValue == 1);
    }

    {
      c_token T = PopToken(Parser);
      TestThat(T.Type == CTokenType_IntLiteral);
      TestThat(T.UnsignedValue == 0xa);
    }

    {
      c_token T = PopToken(Parser);
      TestThat(T.Type == CTokenType_IntLiteral);
      TestThat(T.UnsignedValue == 0x7f);
    }
    {
      c_token T = PopToken(Parser);
      TestThat(T.Type == CTokenType_IntLiteral);
      TestThat(T.UnsignedValue == 0xff);
    }


    {
      c_token T = PopToken(Parser);
      TestThat(T.Type == CTokenType_IntLiteral);
      TestThat(T.UnsignedValue == 0xffff);
    }

    {
      c_token T = PopToken(Parser);
      TestThat(T.Type == CTokenType_IntLiteral);
      TestThat(T.UnsignedValue == 0xffFFff);
    }

    {
      c_token T = PopToken(Parser);
      TestThat(T.Type == CTokenType_IntLiteral);
      TestThat(T.UnsignedValue == 0xffFFffFF);
    }

    {
      c_token T = PopToken(Parser);
      TestThat(T.Type == CTokenType_IntLiteral);
      TestThat(T.UnsignedValue == 0xffFFffFFffFFffFF);
    }
    {
      c_token T = PopToken(Parser);
      TestThat(T.Type == CTokenType_IntLiteral);
      TestThat(T.UnsignedValue == 0x123456789abcdef1);
    }
    {
      c_token T = PopToken(Parser);
      TestThat(T.Type == CTokenType_IntLiteral);
      TestThat(T.UnsignedValue == 0x7fffffffffffffff);
    }



    //
    // Binary constant tests
    //
    {
      c_token T = PopToken(Parser);
      TestThat(T.Type == CTokenType_IntLiteral);
      TestThat(T.UnsignedValue == 0b1);
    }
    {
      c_token T = PopToken(Parser);
      TestThat(T.Type == CTokenType_IntLiteral);
      TestThat(T.UnsignedValue == 0b01);
    }
    {
      c_token T = PopToken(Parser);
      TestThat(T.Type == CTokenType_IntLiteral);
      TestThat(T.UnsignedValue == 0b101);
    }
    {
      c_token T = PopToken(Parser);
      TestThat(T.Type == CTokenType_IntLiteral);
      TestThat(T.UnsignedValue == 0b111);
    }

    {
      c_token T = PopToken(Parser);
      TestThat(T.Type == CTokenType_IntLiteral);
      TestThat(T.UnsignedValue == 0b011111111111111111111111111111111111111);
    }
    {
      c_token T = PopToken(Parser);
      TestThat(T.Type == CTokenType_IntLiteral);
      TestThat(T.UnsignedValue == 0b11011111111111111111111111111111111111111);
    }
    {
      c_token T = PopToken(Parser);
      TestThat(T.Type == CTokenType_IntLiteral);
      TestThat(T.UnsignedValue == 0b1000000000000000000000000000000000000000000000000000000000000000);
    }
    {
      c_token T = PopToken(Parser);
      TestThat(T.Type == CTokenType_IntLiteral);
      TestThat(T.UnsignedValue == 0b0000000000000000000000000000000000000000000000000000000000000001);
      TestThat(T.UnsignedValue == 1);
    }





    //
    // Float/Double Tests
    //

    {
      c_token T = PopToken(Parser);
      TestThat(T.Type == CTokenType_FloatLiteral);
      TestThat(T.FloatValue == 42.0);
    }
    {
      c_token T = PopToken(Parser);
      TestThat(T.Type == CTokenType_FloatLiteral);
      TestThat(T.FloatValue == 42.0);
    }
    {
      c_token T = PopToken(Parser);
      TestThat(T.Type == CTokenType_FloatLiteral);
      TestThat(T.FloatValue == 42.42);
    }
    {
      c_token T = PopToken(Parser);
      TestThat(T.Type == CTokenType_FloatLiteral);
      TestThat(T.FloatValue == .42);
    }

    {
      c_token T = PopToken(Parser);
      TestThat(T.Type == CTokenType_DoubleLiteral);
      TestThat(T.FloatValue == 42.0);
    }
    {
      c_token T = PopToken(Parser);
      TestThat(T.Type == CTokenType_DoubleLiteral);
      TestThat(T.FloatValue == 42.0);
    }
    {
      c_token T = PopToken(Parser);
      TestThat(T.Type == CTokenType_DoubleLiteral);
      TestThat(T.FloatValue == 42.42);
    }
    {
      c_token T = PopToken(Parser);
      TestThat(T.Type == CTokenType_DoubleLiteral);
      TestThat(T.FloatValue == .42);
    }


    ExponentTests(Parser, CTokenType_DoubleLiteral); // double, with lowercase 'e'

    ExponentTests(Parser, CTokenType_DoubleLiteral); // double, with uppercase 'E'

    ExponentTests(Parser, CTokenType_FloatLiteral); // float, with lowercase 'e'

    ExponentTests(Parser, CTokenType_FloatLiteral); // float, with uppercase 'E'


    EatWhitespaceAndComments(Parser);


#if 0
    {
      c_token *T = PopTokenRawPointer(Parser);
      TestThat(T->Type == CT_PreprocessorInclude);
    }
    {
      c_token *T = PopTokenRawPointer(Parser);
      TestThat(T->Type == CTokenType_Space);
    }
    {
      EatBetweenRaw(Parser, CTokenType_LT, CTokenType_GT);
    }


    {
      c_token *T = PeekTokenRawPointer(Parser, -1);
      TestThat(T->Type == CTokenType_GT);
    }
    {
      c_token *T = PeekTokenRawPointer(Parser, -13);
      TestThat(T->Type == CTokenType_LT);
    }
    {
      c_token *T = PeekTokenRawPointer(Parser, -15);
      TestThat(T->Type == CT_PreprocessorInclude);
    }


    {
      c_token *T = PopTokenRawPointer(Parser);
      TestThat(T->Type == CTokenType_Newline);
    }

    {
      c_token *T = PopTokenRawPointer(Parser);
      TestThat(T->Type == CTokenType_Identifier);
      TestThat(StringsMatch(T->Value, CSz("hi")));
    }
    {
      c_token *T = PopTokenRawPointer(Parser);
      TestThat(T->Type == CTokenType_Newline);
    }
    {
      c_token *T = PopTokenRawPointer(Parser);
      TestThat(T->Type == CTokenType_Newline);
    }

    {
      c_token *T = PeekTokenRawPointer(Parser, -1);
      TestThat(T->Type == CTokenType_Newline);
    }
    {
      c_token *T = PeekTokenRawPointer(Parser, -2);
      TestThat(T->Type == CTokenType_Newline);
    }
    {
      c_token *T = PeekTokenRawPointer(Parser, -3);
      TestThat(T->Type == CTokenType_Identifier);
      TestThat(StringsMatch(T->Value, CSz("hi")));
    }

    TestThat(Remaining(Parser->Tokens) == 0);
#endif
  }
  else
  {
    ++TestsFailed;
  }
}

bonsai_function void
TestPeekAndPopTokens(memory_arena* Memory)
{
  parse_context Ctx = AllocateParseContext(Memory);
  parser *Parser = PreprocessedParserForFile(&Ctx, CS(TEST_FIXTURES_PATH "/preprocessor_peek_pop.cpp"), TokenCursorSource_RootFile, 0);

  if (Parser)
  {
    /* DumpEntireParser(Parser, u32_MAX, 1); */
    /* RuntimeBreak(); */
    {
      c_token T = PeekToken(Parser, 0);
      TestThat(T == CToken(CS("bonsai_function")));
    }

    {
      c_token T = PeekToken(Parser, 1);
      TestThat(T.Type == CTokenType_Void);
    }
    {
      c_token T = PeekToken(Parser, 2);
      TestThat(T == CToken(CS("FunctionName")));
    }
    {
      c_token T = PeekToken(Parser, 3);
      TestThat(T.Type == CTokenType_OpenParen);
    }
    {
      c_token T = PeekToken(Parser, 4);
      TestThat(T.Type == CTokenType_CloseParen);
    }

    {
      c_token T = PopToken(Parser);
      TestThat(T == CToken(CS("bonsai_function")));
    }
    {
      c_token T = PeekToken(Parser, 0);
      TestThat(T.Type == CTokenType_Void);
    }
    {
      c_token T = PeekToken(Parser, 1);
      TestThat(T == CToken(CS("FunctionName")));
    }
    {
      c_token T = PeekToken(Parser, 2);
      TestThat(T.Type == CTokenType_OpenParen);
    }
    {
      c_token T = PeekToken(Parser, 3);
      TestThat(T.Type == CTokenType_CloseParen);
    }
    {
      c_token T = PopToken(Parser);
      TestThat(T.Type == CTokenType_Void);
    }
    {
      c_token T = PopToken(Parser);
      TestThat(T == CToken(CS("FunctionName")));
    }
    {
      c_token T = PopToken(Parser);
      TestThat(T.Type == CTokenType_OpenParen);
    }
    {
      c_token T = PopToken(Parser);
      TestThat(T.Type == CTokenType_CloseParen);
    }

    {
      c_token T = PeekToken(Parser, 0);
      TestThat(T == CToken(CS("debug_scope_tree")));
    }
    {
      c_token T = PeekToken(Parser, 1);
      TestThat(T.Type == CTokenType_Star);
    }
    {
      c_token T = PeekToken(Parser, 2);
      TestThat(T == CToken(CS("GetReadScopeTree")));
    }
    {
      c_token T = PeekToken(Parser, 3);
      TestThat(T.Type == CTokenType_OpenParen);
    }
    {
      c_token T = PeekToken(Parser, 4);
      TestThat(T == CToken(CS("u32")));
    }
    {
      c_token T = PeekToken(Parser, 5);
      TestThat(T == CToken(CS("ThreadIndex")));
    }
    {
      c_token T = PeekToken(Parser, 6);
      TestThat(T.Type == CTokenType_CloseParen);
    }

    {
      c_token T = PopToken(Parser);
      TestThat(T == CToken(CS("debug_scope_tree")));
    }
    {
      c_token T = PopToken(Parser);
      TestThat(T.Type == CTokenType_Star);
    }
    {
      c_token T = PopToken(Parser);
      TestThat(T == CToken(CS("GetReadScopeTree")));
    }
    {
      c_token T = PopToken(Parser);
      TestThat(T.Type == CTokenType_OpenParen);
    }
    {
      c_token T = PopToken(Parser);
      TestThat(T == CToken(CS("u32")));
    }
    {
      c_token T = PopToken(Parser);
      TestThat(T == CToken(CS("ThreadIndex")));
    }

    {
      c_token T = PopToken(Parser);
      TestThat(T.Type == CTokenType_CloseParen);
    }



    {
      c_token T = PeekToken(Parser);
      TestThat(T == CToken(CSz("counted_string")));
    }

    {
      c_token T = PopToken(Parser);
      TestThat(T == CToken(CSz("counted_string")));
    }

    {
      c_token T = PeekToken(Parser);
      TestThat(T == CToken(CSz("counted_string")));
    }

    {
      c_token T = PopToken(Parser);
      TestThat(T == CToken(CSz("counted_string")));
    }

    {
      c_token T = PopTokenRaw(Parser);
      TestThat( T == CToken(CTokenType_Newline, CSz("\n")));
    }

    {
      c_token T = PopTokenRaw(Parser);
      TestThat( T == CToken(CTokenType_Newline, CSz("\n")));
    }

    TestThat(PopTokenRaw(Parser) == CToken(CTokenType_CommentSingleLine, CSz("// single ") ));

    {
      c_token T = PeekTokenRaw(Parser);
      TestThat(T == CToken(CTokenType_EscapedNewline, CSz("\\\n")));
    }

    {
      c_token T = PopTokenRaw(Parser);
      TestThat(T == CToken(CTokenType_EscapedNewline, CSz("\\\n")));
    }

    /* TestThat(PopTokenRaw(Parser) == CToken(CTokenType_EscapedNewline, CSz("\\\n"))); */

    TestThat(PopTokenRaw(Parser) == CToken(CTokenType_CommentSingleLine, CSz("line")));
    TestThat(PopTokenRaw(Parser) == CToken(CTokenType_Newline, CSz("\n")));

  }
  else
  {
    ++TestsFailed;
  }

  return;
}

bonsai_function void
TestBoundaryConditions(memory_arena* Memory)
{
  parse_context Ctx = AllocateParseContext(Memory);
  parser *Parser = PreprocessedParserForFile(&Ctx, CS(TEST_FIXTURES_PATH "/preprocessor/boundary_conditions.cpp"), TokenCursorSource_RootFile, 0);

  c_token Hi = CToken(CSz("hi"));
  c_token NL = CToken(CTokenType_Newline);

  c_token *Prev = 0;
  c_token *Next = 0;

  TestThat( Remaining(Parser->Tokens) == 2 );

  Next = PeekTokenRawPointer(Parser);
  TestThat( *Next == Hi );

  Prev = PeekTokenPointer(Parser, -1);
  TestThat( Prev == 0 );

  RequireTokenRaw(Parser, Hi );

  Next = PeekTokenRawPointer(Parser);
  TestThat( *Next == NL );

  Prev = PeekTokenRawPointer(Parser, -1);
  TestThat( *Prev == Hi );

  RequireTokenRaw(Parser, NL );

  Next = PeekTokenRawPointer(Parser);
  TestThat( Next == 0 );

#if BUG_LAST_TOKEN_IN_CHAIN_PEEK_BACK
  Prev = PeekTokenRawPointer(Parser, -1);
  TestThat( *Prev == NL );
#endif

  TestThat( RawTokensRemain(Parser) == 0 );
}



bonsai_function void
TestStructParsing(memory_arena* Memory)
{
  parse_context Ctx = AllocateParseContext(Memory);
  parser *Parser = PreprocessedParserForFile(&Ctx, CS(TEST_FIXTURES_PATH "/preprocessor_datatypes.cpp"), TokenCursorSource_RootFile, 0);

  if (Parser)
  {
    Ctx.CurrentParser = Parser;
    ParseDatatypes(&Ctx);
    TestThat(Parser->ErrorCode == ParseErrorCode_None);
  }
  else
  {
    ++TestsFailed;
  }

  return;
}

bonsai_function void
TestCommentSituation(memory_arena* Memory)
{
  parse_context Ctx = AllocateParseContext(Memory);
  parser *Parser = PreprocessedParserForFile(&Ctx, CS(TEST_FIXTURES_PATH "/comments.cpp"), TokenCursorSource_RootFile, 0);

  if (Parser)
  {
    {
      c_token T = PeekToken(Parser);
      TestThat(T == CToken(CSz("bonsai_function")));
    }

    {
      c_token T = PopToken(Parser);
      TestThat(T == CToken(CSz("bonsai_function")));
    }


    {
      c_token T = PeekToken(Parser);
      TestThat(T.Type == CTokenType_Void);
    }

    {
      c_token T = PeekToken(Parser, 1);
      TestThat(T.Type == CTokenType_Unknown);
    }

    {
      c_token T = PeekToken(Parser);
      TestThat(T.Type == CTokenType_Void);
    }




    {
      c_token T = PopToken(Parser);
      TestThat(T.Type == CTokenType_Void);
    }


    {
      c_token T = PopToken(Parser);
      TestThat(T.Type == CTokenType_Unknown);
    }
  }
  else
  {
    ++TestsFailed;
  }

  return;
}

bonsai_function void
TestAst(memory_arena *Memory)
{
  parse_context Ctx = AllocateParseContext(Memory);
  parser *Parser = PreprocessedParserForFile(&Ctx, CS(TEST_FIXTURES_PATH "/preprocessor/should_parse.cpp"), TokenCursorSource_RootFile, 0);
  if (Parser)
  {
    /* DumpCursorSimple(Parser->Tokens, Parser->Tokens->At); */
    /* RuntimeBreak(); */

    Ctx.CurrentParser = Parser;

    ParseDatatypes(&Ctx);
    TestThat(Parser->ErrorCode == ParseErrorCode_None);

    ITERATE_OVER(&Ctx.Datatypes.Functions)
    {
      function_decl *Func = GET_ELEMENT(Iter);

      if (Func->ReturnType.TemplateSource.Count) { continue; }

      Func->Ast = ParseAllStatements(&Ctx);
      WalkAst(Func->Ast);
    }
  }
  else
  {
    ++TestsFailed;
  }
}

bonsai_function void
TestMacrosAndIncludes(memory_arena *Memory)
{
  parse_context Ctx = AllocateParseContext(Memory);
  parser *Parser = PreprocessedParserForFile(&Ctx, CS(TEST_FIXTURES_PATH "/preprocessor/macro_and_include_test.cpp"), TokenCursorSource_RootFile, 0);
  /* DumpEntireParser(Parser); */
  /* DumpEntireParser(Parser, u32_MAX, 1); */

  if (Parser)
  {
    FullRewind(Parser);

    /* DumpCursorSimple(Parser->Tokens); */
    /* FullRewind(Parser); */
    /* DumpEntireParser(Parser); */

    /* RuntimeBreak(); */

    TestThat(RequireToken(Parser, CToken(CSz("valid_path"))));
    TestThat(RequireToken(Parser, CToken(CSz("valid_path"))));
    TestThat(RequireToken(Parser, CToken(CSz("valid_path"))));


    // Include Test


    TestThat(RequireToken(Parser, CTokenType_Int));
    TestThat(RequireToken(Parser, CToken(CSz("ding"))));
    TestThat(RequireToken(Parser, CTokenType_Equals));
    TestThat(RequireToken(Parser, CToken(42u)));
    TestThat(RequireToken(Parser, CTokenType_Semicolon));

    TestThat(RequireToken(Parser, CTokenType_Int));
    TestThat(RequireToken(Parser, CToken(CSz("thing"))));
    TestThat(RequireToken(Parser, CTokenType_Equals));
    TestThat(RequireToken(Parser, CToken(1337u)));
    TestThat(RequireToken(Parser, CTokenType_Semicolon));

    TestThat(RequireToken(Parser, CTokenType_Int));
    TestThat(RequireToken(Parser, CToken(CSz("thing"))));
    TestThat(RequireToken(Parser, CTokenType_Equals));
    TestThat(RequireToken(Parser, CToken(1337u)));
    TestThat(RequireToken(Parser, CTokenType_Semicolon));


    // MacroKeyword


    TestThat(RequireToken(Parser, CTokenType_Int));
    TestThat(RequireToken(Parser, CToken(CSz("this_is_a_variable_name"))));
    TestThat(RequireToken(Parser, CTokenType_Equals));
    TestThat(RequireToken(Parser, CToken(42u)));
    TestThat(RequireToken(Parser, CTokenType_Semicolon));

    TestThat(RequireToken(Parser, CTokenType_Int));
    TestThat(RequireToken(Parser, CToken(CSz("this_is_a_variable_name"))));
    TestThat(RequireToken(Parser, CTokenType_Equals));
    TestThat(RequireToken(Parser, CToken(42u)));
    TestThat(RequireToken(Parser, CTokenType_Semicolon));

    TestThat(RequireToken(Parser, CTokenType_Int));
    TestThat(RequireToken(Parser, CToken(CSz("this_is_a_variable_name"))));
    TestThat(RequireToken(Parser, CTokenType_Equals));
    TestThat(RequireToken(Parser, CToken(42u)));
    TestThat(RequireToken(Parser, CTokenType_Semicolon));



    // MacroFunction


    TestThat(RequireToken(Parser, CTokenType_Int));
    TestThat(RequireToken(Parser, CToken(CSz("this_is_a_variable_name"))));
    TestThat(RequireToken(Parser, CTokenType_Equals));
    TestThat(RequireToken(Parser, CToken(42u)));
    TestThat(RequireToken(Parser, CTokenType_Semicolon));

    TestThat(RequireToken(Parser, CTokenType_Int));
    TestThat(RequireToken(Parser, CToken(CSz("this_is_a_variable_name"))));
    TestThat(RequireToken(Parser, CTokenType_Equals));
    TestThat(RequireToken(Parser, CToken(42u)));
    TestThat(RequireToken(Parser, CTokenType_Semicolon));

    TestThat(RequireToken(Parser, CToken(CSz("SomeRegularFunctionCall"))));
    TestThat(RequireToken(Parser, CTokenType_OpenParen));
    TestThat(RequireToken(Parser, CToken(42u)));
    TestThat(RequireToken(Parser, CTokenType_CloseParen));
    TestThat(RequireToken(Parser, CTokenType_Semicolon));


    // MacroFunction2


    TestThat(RequireToken(Parser, CTokenType_Int));
    TestThat(RequireToken(Parser, CToken(CSz("this_is_a_variable_name"))));
    TestThat(RequireToken(Parser, CTokenType_Equals));
    TestThat(RequireToken(Parser, CToken(42u)));
    TestThat(RequireToken(Parser, CTokenType_Semicolon));

    TestThat(RequireToken(Parser, CTokenType_Int));
    TestThat(RequireToken(Parser, CToken(CSz("this_is_a_variable_name"))));
    TestThat(RequireToken(Parser, CTokenType_Equals));
    TestThat(RequireToken(Parser, CToken(42u)));
    TestThat(RequireToken(Parser, CTokenType_Semicolon));

    TestThat(RequireToken(Parser, CTokenType_Int));
    TestThat(RequireToken(Parser, CToken(CSz("this_is_a_variable_name"))));
    TestThat(RequireToken(Parser, CTokenType_Equals));
    TestThat(RequireToken(Parser, CToken(42u)));
    TestThat(RequireToken(Parser, CTokenType_Semicolon));


    // MacroFunction3


    TestThat(RequireToken(Parser, CTokenType_Int));
    TestThat(RequireToken(Parser, CToken(CSz("this_is_a_variable_name"))));
    TestThat(RequireToken(Parser, CTokenType_Equals));
    TestThat(RequireToken(Parser, CToken(42u)));
    TestThat(RequireToken(Parser, CTokenType_Semicolon));

    TestThat(RequireToken(Parser, CTokenType_Int));
    TestThat(RequireToken(Parser, CToken(CSz("this_is_a_variable_name"))));
    TestThat(RequireToken(Parser, CTokenType_Equals));
    TestThat(RequireToken(Parser, CToken(42u)));
    TestThat(RequireToken(Parser, CTokenType_Semicolon));

    TestThat(RequireToken(Parser, CTokenType_Int));
    TestThat(RequireToken(Parser, CToken(CSz("this_is_a_variable_name"))));
    TestThat(RequireToken(Parser, CTokenType_Equals));
    TestThat(RequireToken(Parser, CToken(42u)));
    TestThat(RequireToken(Parser, CTokenType_Semicolon));


    // MacroFunction4


    TestThat(RequireToken(Parser, CTokenType_Int));
    TestThat(RequireToken(Parser, CToken(CSz("this_is_a_variable_name"))));
    TestThat(RequireToken(Parser, CTokenType_Equals));
    TestThat(RequireToken(Parser, CToken(42u)));
    TestThat(RequireToken(Parser, CTokenType_Semicolon));

    TestThat(RequireToken(Parser, CTokenType_Int));
    TestThat(RequireToken(Parser, CToken(CSz("this_is_a_variable_name"))));
    TestThat(RequireToken(Parser, CTokenType_Equals));
    TestThat(RequireToken(Parser, CToken(42u)));
    TestThat(RequireToken(Parser, CTokenType_Semicolon));


    // MacroFunction5


    TestThat(RequireToken(Parser, CTokenType_Int));
    TestThat(RequireToken(Parser, CToken(CSz("this_is_a_variable_name"))));
    TestThat(RequireToken(Parser, CTokenType_Equals));
    TestThat(RequireToken(Parser, CToken(42u)));
    TestThat(RequireToken(Parser, CTokenType_Semicolon));


    // MacroFunction6


    TestThat(RequireToken(Parser, CTokenType_Int));
    TestThat(RequireToken(Parser, CToken(CSz("this_is_a_variable_name"))));
    TestThat(RequireToken(Parser, CTokenType_Equals));
    TestThat(RequireToken(Parser, CToken(42u)));
    TestThat(RequireToken(Parser, CTokenType_Semicolon));

    TestThat(RequireToken(Parser, CTokenType_Int));
    TestThat(RequireToken(Parser, CToken(CSz("this_is_a_variable_name"))));
    TestThat(RequireToken(Parser, CTokenType_Equals));
    TestThat(RequireToken(Parser, CToken(42u)));
    TestThat(RequireToken(Parser, CTokenType_Semicolon));

    TestThat(RequireToken(Parser, CTokenType_Int));
    TestThat(RequireToken(Parser, CToken(CSz("this_is_a_variable_name"))));
    TestThat(RequireToken(Parser, CTokenType_Equals));
    TestThat(RequireToken(Parser, CToken(42u)));
    TestThat(RequireToken(Parser, CTokenType_Semicolon));

    TestThat(RequireToken(Parser, CTokenType_Int));
    TestThat(RequireToken(Parser, CToken(CSz("this_is_a_variable_name"))));
    TestThat(RequireToken(Parser, CTokenType_Equals));
    TestThat(RequireToken(Parser, CToken(42u)));
    TestThat(RequireToken(Parser, CTokenType_Semicolon));


    // MacroFunction7


    TestThat(RequireToken(Parser, CToken(CSz("SomeRegularFunctionCall"))));
    TestThat(RequireToken(Parser, CTokenType_OpenParen));
    TestThat(RequireToken(Parser, CToken(4u)));
    TestThat(RequireToken(Parser, CTokenType_Comma));
    TestThat(RequireToken(Parser, CToken(2u)));
    TestThat(RequireToken(Parser, CTokenType_CloseParen));
    TestThat(RequireToken(Parser, CTokenType_Semicolon));


    TestThat(RequireToken(Parser, CToken(CSz("SomeRegularFunctionCall"))));
    TestThat(RequireToken(Parser, CTokenType_OpenParen));
    TestThat(RequireToken(Parser, CToken(42u)));
    TestThat(RequireToken(Parser, CTokenType_CloseParen));
    TestThat(RequireToken(Parser, CTokenType_Semicolon));

    TestThat(RequireToken(Parser, CToken(4u)));
    TestThat(RequireToken(Parser, CToken(2u)));

    TestThat(RequireToken(Parser, CToken(4u)));
    TestThat(RequireToken(Parser, CToken(2u)));


    // MacroFunction8


    TestThat(RequireToken(Parser, CToken(CSz("MacroFunction8"))));
    TestThat(RequireToken(Parser, CToken(CSz("MacroFunction8"))));
    TestThat(RequireToken(Parser, CToken(0u)));


    // MacroFunction9


    TestThat(RequireToken(Parser, CToken(CSz("this_is_a_variable_name"))));
    TestThat(RequireToken(Parser, CToken(CSz("this_is_a_variable_name"))));
    TestThat(RequireToken(Parser, CToken(CSz("this_is_a_variable_name"))));

    TestThat(RequireToken(Parser, CToken(CSz("this_is_a_variable_name"))));
    TestThat(RequireToken(Parser, CToken(CSz("this_is_a_variable_name"))));
    TestThat(RequireToken(Parser, CToken(CSz("this_is_a_variable_name"))));

    TestThat(RequireToken(Parser, CToken(CSz("fooberdoober_thing"))));

    TestThat(RequireToken(Parser, CToken(CSz("this_is_a_variable_name"))));
    TestThat(RequireToken(Parser, CToken(CSz("some_thing"))));
    TestThat(RequireToken(Parser, CToken(CTokenType_Int, CSz("int"))));

    TestThat(RequireToken(Parser, CToken(CSz("this_is_a_variable_name"))));
    TestThat(RequireToken(Parser, CToken(CSz("this_is_a_variable_name"))));
    TestThat(RequireToken(Parser, CToken(CSz("this_is_a_variable_name"))));
    TestThat(RequireToken(Parser, CToken(CSz("this_is_a_variable_name"))));
    TestThat(RequireToken(Parser, CToken(CSz("this_is_a_variable_name"))));

#if BUG_OPERATOR_PASTE
    TestThat(RequireToken(Parser, CToken(CTokenType_Increment, CSz("++"))));
#endif


    // MacroFunction10


    TestThat(RequireToken(Parser, CToken(CTokenType_Int, CSz("int"))));
    TestThat(RequireToken(Parser, CToken(CSz("this_is_a_variable_name"))));
    TestThat(RequireToken(Parser, CToken(CSz("this_is_a_variable_name"))));
    TestThat(RequireToken(Parser, CToken(CSz("this_is_a_variable_name"))));
    TestThat(RequireToken(Parser, CToken(CSz("this_is_a_variable_name"))));
    TestThat(RequireToken(Parser, CToken(CSz("this_is_a_variable_name"))));



    ////////////////////////////



    TestThat(RequireToken(Parser, CToken(CSz("self_including_macro_keyword"))));
    TestThat(RequireToken(Parser, CToken(42u)));

#if BUG_RECURSIVE_MACRO_EXPANSION
    TestThat(RequireToken(Parser, CToken(CSz("self_including_macro_keyword"))));
    TestThat(RequireToken(Parser, CToken(42u)));

    /* DumpEntireParser(Parser); */

    TestThat(RequireToken(Parser, CToken(CSz("m2"))));
    TestThat(RequireToken(Parser, CTokenType_OpenParen));
    TestThat(RequireToken(Parser, CTokenType_CloseParen));
#endif

#if BUG_SELF_INCLUDING_MACRO_FUNCTION
    TestThat(RequireToken(Parser, CToken(CSz("temp__"))));
    TestThat(RequireToken(Parser, CTokenType_OpenParen));
    TestThat(RequireToken(Parser, CTokenType_CloseParen));
#endif

    TestThat(RequireToken(Parser, CToken(CSz("valid_path"))));

    TestThat(RequireToken(Parser, CToken(CSz("this_is_a_variable_name"))));

    TestThat( TokensRemain(Parser) == False );
  }
  else
  {
    ++TestsFailed;
  }

  return;
}

bonsai_function void
TestIncludeGuards(memory_arena *Memory)
{
  {
    parse_context Ctx = AllocateParseContext(Memory);
    parser *Parser = PreprocessedParserForFile(&Ctx, CS(TEST_FIXTURES_PATH "/preprocessor/include_guard0.cpp"), TokenCursorSource_RootFile, 0);
    if (Parser)
    {
      Ctx.CurrentParser = Parser;
      /* DumpEntireParser(Parser); */
      /* RuntimeBreak(); */
      ast_node_statement *Ast = ParseAllStatements(&Ctx);
      WalkAst(Ast);
      /* DebugPrint(Ast); */
    }
    else
    {
      ++TestsFailed;
    }
  }

  {
    parse_context Ctx = AllocateParseContext(Memory);
    parser *Parser = PreprocessedParserForFile(&Ctx, CS(TEST_FIXTURES_PATH "/preprocessor/pragma_once0.cpp"), TokenCursorSource_RootFile, 0);
    if ( ! Parser )
    {
      ++TestsFailed;
    }
  }
}

bonsai_function void
TestDefinesAndConditionals(memory_arena *Memory)
{
  parse_context Ctx = AllocateParseContext(Memory);
  parser *Parser = PreprocessedParserForFile(&Ctx, CS(TEST_FIXTURES_PATH "/preprocessor/defines_and_conditionals.cpp"), TokenCursorSource_RootFile, 0);
  if (Parser)
  {
    Ctx.CurrentParser = Parser;
    /* DumpEntireParser(Parser); */

    while(TokensRemain(Parser))
    {
      TestThat(RequireToken(Parser, CToken(CSz("valid_path"))));
      TestThat(Parser->ErrorCode == ParseErrorCode_None);
    }

    /* RuntimeBreak(); */
    /* ast_node_statement *Ast = ParseAllStatements(&Ctx); */
    /* WalkAst(Ast); */
    /* DebugPrint(Ast); */
  }
  else
  {
    ++TestsFailed;
  }
}

bonsai_function void
TestLogicalOperators(memory_arena *Memory)
{
  parse_context Ctx = AllocateParseContext(Memory);

  parser *Parser = PreprocessedParserForFile(&Ctx, CS(TEST_FIXTURES_PATH "/preprocessor/logical_operators.cpp"), TokenCursorSource_RootFile, 0);
  if (Parser)
  {
    Ctx.CurrentParser = Parser;
    /* DumpEntireParser(Parser); */

    while(TokensRemain(Parser))
    {
      TestThat(RequireToken(Parser, CToken(CSz("valid_path"))));
      TestThat(Parser->ErrorCode == ParseErrorCode_None);
    }

  }
  else
  {
    ++TestsFailed;
  }
}

#define MAX_PARSERS_IN_PARSER_BLOCK 8
bonsai_function b32
ParserChainIntegrityPasses(d_list *Mem)
{
  CAssert(MAX_PARSERS_IN_PARSER_BLOCK == 8);
  b32 Result =
    Mem[0].Next == &Mem[1] &&
    Mem[1].Next == &Mem[2] &&
    Mem[2].Next == &Mem[3] &&
    Mem[3].Next == &Mem[4] &&
    Mem[4].Next == &Mem[5] &&
    Mem[5].Next == &Mem[6] &&
    Mem[6].Next == &Mem[7] &&
    Mem[7].Next == 0 &&
    Mem[0].Prev == 0 &&
    Mem[1].Prev == &Mem[0] &&
    Mem[2].Prev == &Mem[1] &&
    Mem[3].Prev == &Mem[2] &&
    Mem[4].Prev == &Mem[3] &&
    Mem[5].Prev == &Mem[4] &&
    Mem[6].Prev == &Mem[5] &&
    Mem[7].Prev == &Mem[6];

  return Result;
}

bonsai_function void
LinkParserBlocks(d_list *Mem, u32 ParserCount)
{
  d_list *Prev = 0;
  for (u32 ParserIndex = 0; ParserIndex < ParserCount ; ++ParserIndex)
  {
    d_list *At = Mem+ParserIndex;

    At->Next = 0;
    At->Prev = Prev;
    if (Prev) { Prev->Next = At; }

    Prev = At;
  }

  TestThat(ParserChainIntegrityPasses(Mem));
}

bonsai_function void
TestDoublyLinkedListSwap()
{
  d_list Mem[MAX_PARSERS_IN_PARSER_BLOCK] = {};

  d_list *Mem0 = Mem;
  d_list *Mem1 = Mem+1;
  d_list *Mem2 = Mem+2;
  d_list *Mem3 = Mem+3;
  d_list *Mem4 = Mem+4;
  d_list *Mem5 = Mem+5;
  d_list *Mem6 = Mem+6;
  d_list *Mem7 = Mem+7;


  { // Test swaps work once
    {
      LinkParserBlocks(Mem, ArrayCount(Mem));
      DoublyLinkedListSwap(Mem0, Mem1);
      TestThat(Mem0->Next == Mem2);
      TestThat(Mem0->Prev == Mem1);
      TestThat(Mem1->Next == Mem0);
      TestThat(Mem1->Prev == 0);
      TestThat(Mem2->Next == Mem3);
      TestThat(Mem2->Prev == Mem0);
    }
    {
      LinkParserBlocks(Mem, ArrayCount(Mem));
      DoublyLinkedListSwap(Mem1, Mem0);
      TestThat(Mem0->Next == Mem2);
      TestThat(Mem0->Prev == Mem1);
      TestThat(Mem1->Next == Mem0);
      TestThat(Mem1->Prev == 0);
      TestThat(Mem2->Next == Mem3);
      TestThat(Mem2->Prev == Mem0);
    }

    {
      LinkParserBlocks(Mem, ArrayCount(Mem));
      DoublyLinkedListSwap(Mem1, Mem2);
      TestThat(Mem1->Next == Mem3);
      TestThat(Mem1->Prev == Mem2);
      TestThat(Mem2->Next == Mem1);
      TestThat(Mem2->Prev == Mem0);
      TestThat(Mem3->Next == Mem4);
      TestThat(Mem3->Prev == Mem1);
      TestThat(Mem0->Next == Mem2);
      TestThat(Mem0->Prev == 0);
    }
    {
      LinkParserBlocks(Mem, ArrayCount(Mem));
      DoublyLinkedListSwap(Mem2, Mem1);
      TestThat(Mem1->Next == Mem3);
      TestThat(Mem1->Prev == Mem2);
      TestThat(Mem2->Next == Mem1);
      TestThat(Mem2->Prev == Mem0);
      TestThat(Mem3->Next == Mem4);
      TestThat(Mem3->Prev == Mem1);
      TestThat(Mem0->Next == Mem2);
      TestThat(Mem0->Prev == 0);
    }


    {
      LinkParserBlocks(Mem, ArrayCount(Mem));
      DoublyLinkedListSwap(Mem1, Mem3);
      TestThat(Mem0->Next == Mem3);
      TestThat(Mem0->Prev == 0);
      TestThat(Mem1->Next == Mem4);
      TestThat(Mem1->Prev == Mem2);
      TestThat(Mem2->Next == Mem1);
      TestThat(Mem2->Prev == Mem3);
      TestThat(Mem3->Next == Mem2);
      TestThat(Mem3->Prev == Mem0);
      TestThat(Mem4->Next == Mem5);
      TestThat(Mem4->Prev == Mem1);
    }
    {
      LinkParserBlocks(Mem, ArrayCount(Mem));
      DoublyLinkedListSwap(Mem3, Mem1);
      TestThat(Mem0->Next == Mem3);
      TestThat(Mem0->Prev == 0);
      TestThat(Mem1->Next == Mem4);
      TestThat(Mem1->Prev == Mem2);
      TestThat(Mem2->Next == Mem1);
      TestThat(Mem2->Prev == Mem3);
      TestThat(Mem3->Next == Mem2);
      TestThat(Mem3->Prev == Mem0);
      TestThat(Mem4->Next == Mem5);
      TestThat(Mem4->Prev == Mem1);
    }

    {
      LinkParserBlocks(Mem, ArrayCount(Mem));
      DoublyLinkedListSwap(Mem1, Mem4);
      TestThat(Mem0->Next == Mem4);
      TestThat(Mem0->Prev == 0);
      TestThat(Mem1->Next == Mem5);
      TestThat(Mem1->Prev == Mem3);
      TestThat(Mem2->Next == Mem3);
      TestThat(Mem2->Prev == Mem4);
      TestThat(Mem3->Prev == Mem2);
      TestThat(Mem3->Next == Mem1);
      TestThat(Mem4->Next == Mem2);
      TestThat(Mem4->Prev == Mem0);
    }
    {
      LinkParserBlocks(Mem, ArrayCount(Mem));
      DoublyLinkedListSwap(Mem4, Mem1);
      TestThat(Mem0->Next == Mem4);
      TestThat(Mem0->Prev == 0);
      TestThat(Mem1->Next == Mem5);
      TestThat(Mem1->Prev == Mem3);
      TestThat(Mem2->Next == Mem3);
      TestThat(Mem2->Prev == Mem4);
      TestThat(Mem3->Prev == Mem2);
      TestThat(Mem3->Next == Mem1);
      TestThat(Mem4->Next == Mem2);
      TestThat(Mem4->Prev == Mem0);
    }


    {
      LinkParserBlocks(Mem, ArrayCount(Mem));
      DoublyLinkedListSwap(Mem0, Mem7);
      TestThat(Mem0->Next == 0);
      TestThat(Mem0->Prev == Mem6);
      TestThat(Mem1->Next == Mem2);
      TestThat(Mem1->Prev == Mem7);
      TestThat(Mem6->Next == Mem0);
      TestThat(Mem6->Prev == Mem5);
      TestThat(Mem7->Next == Mem1);
      TestThat(Mem7->Prev == 0);
    }
    {
      LinkParserBlocks(Mem, ArrayCount(Mem));
      DoublyLinkedListSwap(Mem7, Mem0);
      TestThat(Mem0->Next == 0);
      TestThat(Mem0->Prev == Mem6);
      TestThat(Mem1->Next == Mem2);
      TestThat(Mem1->Prev == Mem7);
      TestThat(Mem6->Next == Mem0);
      TestThat(Mem6->Prev == Mem5);
      TestThat(Mem7->Next == Mem1);
      TestThat(Mem7->Prev == 0);
    }

    {
      LinkParserBlocks(Mem, ArrayCount(Mem));
      DoublyLinkedListSwap(Mem1, Mem7);
      TestThat(Mem0->Next == Mem7);
      TestThat(Mem0->Prev == 0);
      TestThat(Mem1->Next == 0);
      TestThat(Mem1->Prev == Mem6);
      TestThat(Mem6->Next == Mem1);
      TestThat(Mem6->Prev == Mem5);
      TestThat(Mem7->Next == Mem2);
      TestThat(Mem7->Prev == Mem0);
    }
    {
      LinkParserBlocks(Mem, ArrayCount(Mem));
      DoublyLinkedListSwap(Mem7, Mem1);
      TestThat(Mem0->Next == Mem7);
      TestThat(Mem0->Prev == 0);
      TestThat(Mem1->Next == 0);
      TestThat(Mem1->Prev == Mem6);
      TestThat(Mem6->Next == Mem1);
      TestThat(Mem6->Prev == Mem5);
      TestThat(Mem7->Next == Mem2);
      TestThat(Mem7->Prev == Mem0);
    }
  }

  { // Test swaps work twice
    {
      LinkParserBlocks(Mem, ArrayCount(Mem));
      DoublyLinkedListSwap(Mem0, Mem1);
      DoublyLinkedListSwap(Mem0, Mem1);
      TestThat(ParserChainIntegrityPasses(Mem));
    }
    {
      LinkParserBlocks(Mem, ArrayCount(Mem));
      DoublyLinkedListSwap(Mem0, Mem1);
      DoublyLinkedListSwap(Mem1, Mem0);
      TestThat(ParserChainIntegrityPasses(Mem));
    }

    {
      LinkParserBlocks(Mem, ArrayCount(Mem));
      DoublyLinkedListSwap(Mem1, Mem2);
      DoublyLinkedListSwap(Mem1, Mem2);
      TestThat(ParserChainIntegrityPasses(Mem));
    }
    {
      LinkParserBlocks(Mem, ArrayCount(Mem));
      DoublyLinkedListSwap(Mem1, Mem2);
      DoublyLinkedListSwap(Mem2, Mem1);
      TestThat(ParserChainIntegrityPasses(Mem));
    }

    {
      LinkParserBlocks(Mem, ArrayCount(Mem));
      DoublyLinkedListSwap(Mem1, Mem3);
      DoublyLinkedListSwap(Mem1, Mem3);
      TestThat(ParserChainIntegrityPasses(Mem));
    }

    {
      LinkParserBlocks(Mem, ArrayCount(Mem));
      DoublyLinkedListSwap(Mem1, Mem3);
      DoublyLinkedListSwap(Mem3, Mem1);
      TestThat(ParserChainIntegrityPasses(Mem));
    }

    {
      LinkParserBlocks(Mem, ArrayCount(Mem));
      DoublyLinkedListSwap(Mem1, Mem4);
      DoublyLinkedListSwap(Mem1, Mem4);
      TestThat(ParserChainIntegrityPasses(Mem));
    }

    {
      LinkParserBlocks(Mem, ArrayCount(Mem));
      DoublyLinkedListSwap(Mem1, Mem4);
      DoublyLinkedListSwap(Mem4, Mem1);
      TestThat(ParserChainIntegrityPasses(Mem));
    }

    {
      LinkParserBlocks(Mem, ArrayCount(Mem));
      DoublyLinkedListSwap(Mem0, Mem7);
      DoublyLinkedListSwap(Mem0, Mem7);
      TestThat(ParserChainIntegrityPasses(Mem));
    }
    {
      LinkParserBlocks(Mem, ArrayCount(Mem));
      DoublyLinkedListSwap(Mem0, Mem7);
      DoublyLinkedListSwap(Mem7, Mem0);
      TestThat(ParserChainIntegrityPasses(Mem));
    }

    {
      LinkParserBlocks(Mem, ArrayCount(Mem));
      DoublyLinkedListSwap(Mem1, Mem7);
      DoublyLinkedListSwap(Mem1, Mem7);
      TestThat(ParserChainIntegrityPasses(Mem));
    }
    {
      LinkParserBlocks(Mem, ArrayCount(Mem));
      DoublyLinkedListSwap(Mem1, Mem7);
      DoublyLinkedListSwap(Mem7, Mem1);
      TestThat(ParserChainIntegrityPasses(Mem));
    }
  }
}

bonsai_function void
TestParserChain(memory_arena *Memory)
{
  parse_context Ctx = AllocateParseContext(Memory);

  parser *Parser = PreprocessedParserForFile(&Ctx, CS(TEST_FIXTURES_PATH "/preprocessor/parser_chain.cpp"), TokenCursorSource_RootFile, 0);
  if (Parser)
  {
    /* DumpCursorSimple(Parser); */

    /* RuntimeBreak(); */

    FullRewind(Parser);
    TestThat(RequireToken(Parser, CToken(CSz("parser_token_0"))));
    FullRewind(Parser);
    TestThat(RequireToken(Parser, CToken(CSz("parser_token_0"))));

    SanityCheckParserChain(Parser);

    FullRewind(Parser);
    TestThat(RequireToken(Parser, CToken(CSz("parser_token_0"))));
    TestThat(RequireToken(Parser, CToken(CSz("parser_token_1"))));
    FullRewind(Parser);
    TestThat(RequireToken(Parser, CToken(CSz("parser_token_0"))));
    TestThat(RequireToken(Parser, CToken(CSz("parser_token_1"))));

    SanityCheckParserChain(Parser);

    FullRewind(Parser);
    TestThat(RequireToken(Parser, CToken(CSz("parser_token_0"))));
    TestThat(RequireToken(Parser, CToken(CSz("parser_token_1"))));
    TestThat(RequireToken(Parser, CToken(CSz("parser_token_2"))));
    FullRewind(Parser);
    TestThat(RequireToken(Parser, CToken(CSz("parser_token_0"))));
    TestThat(RequireToken(Parser, CToken(CSz("parser_token_1"))));
    TestThat(RequireToken(Parser, CToken(CSz("parser_token_2"))));

    FullRewind(Parser);
    TestThat(RequireToken(Parser, CToken(CSz("parser_token_0"))));
    TestThat(RequireToken(Parser, CToken(CSz("parser_token_1"))));
    TestThat(RequireToken(Parser, CToken(CSz("parser_token_2"))));
    TestThat(RequireToken(Parser, CToken(CSz("parser_token_3"))));
    FullRewind(Parser);
    TestThat(RequireToken(Parser, CToken(CSz("parser_token_0"))));
    TestThat(RequireToken(Parser, CToken(CSz("parser_token_1"))));
    TestThat(RequireToken(Parser, CToken(CSz("parser_token_2"))));
    TestThat(RequireToken(Parser, CToken(CSz("parser_token_3"))));

    FullRewind(Parser);
    TestThat(RequireToken(Parser, CToken(CSz("parser_token_0"))));
    TestThat(RequireToken(Parser, CToken(CSz("parser_token_1"))));
    TestThat(RequireToken(Parser, CToken(CSz("parser_token_2"))));
    TestThat(RequireToken(Parser, CToken(CSz("parser_token_3"))));
    TestThat(RequireToken(Parser, CToken(CSz("parser_token_4"))));
    FullRewind(Parser);
    TestThat(RequireToken(Parser, CToken(CSz("parser_token_0"))));
    TestThat(RequireToken(Parser, CToken(CSz("parser_token_1"))));
    TestThat(RequireToken(Parser, CToken(CSz("parser_token_2"))));
    TestThat(RequireToken(Parser, CToken(CSz("parser_token_3"))));
    TestThat(RequireToken(Parser, CToken(CSz("parser_token_4"))));
  }


  return;
}

bonsai_function void
TestErrors(memory_arena *Memory)
{

#if 1
  {
    /* RuntimeBreak(); */

    parse_context Ctx = AllocateParseContext(Memory);
    counted_string ParserFilename = CSz(TEST_FIXTURES_PATH "/preprocessor/errors/error0.cpp");
    parser *Parser = PreprocessedParserForFile(&Ctx, ParserFilename, TokenCursorSource_RootFile, 0);
    Ctx.CurrentParser = Parser;
    ParseDatatypes(&Ctx);
    TestThat(Parser->ErrorCode == ParseErrorCode_ExpectedSemicolonOrEquals);
    TestThat(Parser->ErrorToken->LineNumber == 1);

    Parser->ErrorCode = ParseErrorCode_None;
    TestThat( OptionalToken(Parser, CToken(132151u)) );
  }

  {
    parse_context Ctx = AllocateParseContext(Memory);
    counted_string ParserFilename = CSz(TEST_FIXTURES_PATH "/preprocessor/errors/error1.cpp");
    parser *Parser = PreprocessedParserForFile(&Ctx, ParserFilename, TokenCursorSource_RootFile, 0);
    Ctx.CurrentParser = Parser;
    ParseDatatypes(&Ctx);
    TestThat(Parser->ErrorCode == ParseErrorCode_ExpectedSemicolonOrEquals);
    TestThat(Parser->ErrorToken->LineNumber == 3);

    Parser->ErrorCode = ParseErrorCode_None;
    TestThat( OptionalToken(Parser, CToken(132151u)) );
  }

  {
    parse_context Ctx = AllocateParseContext(Memory);
    counted_string ParserFilename = CSz(TEST_FIXTURES_PATH "/preprocessor/errors/error2.cpp");
    parser *Parser = PreprocessedParserForFile(&Ctx, ParserFilename, TokenCursorSource_RootFile, 0);
    Ctx.CurrentParser = Parser;
    ParseDatatypes(&Ctx);
    TestThat(Parser->ErrorCode == ParseErrorCode_ExpectedSemicolonOrEquals);
    TestThat(Parser->ErrorToken->LineNumber == 3);

    Parser->ErrorCode = ParseErrorCode_None;
    TestThat( OptionalToken(Parser, CToken(132151u)) );
  }

  {
    parse_context Ctx = AllocateParseContext(Memory);
    counted_string ParserFilename = CSz(TEST_FIXTURES_PATH "/preprocessor/errors/error3.cpp");
    parser *Parser = PreprocessedParserForFile(&Ctx, ParserFilename, TokenCursorSource_RootFile, 0);
    Ctx.CurrentParser = Parser;
    ParseDatatypes(&Ctx);
    TestThat(Parser->ErrorCode == ParseErrorCode_ExpectedSemicolonOrEquals);
    TestThat(Parser->ErrorToken->LineNumber == 3);

    Parser->ErrorCode = ParseErrorCode_None;
    TestThat( OptionalToken(Parser, CToken(132151u)) );
  }

  {
    parse_context Ctx = AllocateParseContext(Memory);
    counted_string ParserFilename = CSz(TEST_FIXTURES_PATH "/preprocessor/errors/error4.cpp");
    parser *Parser = PreprocessedParserForFile(&Ctx, ParserFilename, TokenCursorSource_RootFile, 0);
    Ctx.CurrentParser = Parser;
    ParseDatatypes(&Ctx);
    TestThat(Parser->ErrorCode == ParseErrorCode_ExpectedSemicolonOrEquals);
    TestThat(Parser->ErrorToken->LineNumber == 3);

    Parser->ErrorCode = ParseErrorCode_None;
    TestThat( OptionalToken(Parser, CToken(132151u)) );
  }

  {
    parse_context Ctx = AllocateParseContext(Memory);
    counted_string ParserFilename = CSz(TEST_FIXTURES_PATH "/preprocessor/errors/error5.cpp");
    parser *Parser = PreprocessedParserForFile(&Ctx, ParserFilename, TokenCursorSource_RootFile, 0);
    Ctx.CurrentParser = Parser;
    ParseDatatypes(&Ctx);
    TestThat(Parser->ErrorCode == ParseErrorCode_ExpectedSemicolonOrEquals);
    TestThat(Parser->ErrorToken->LineNumber == 3);

    Parser->ErrorCode = ParseErrorCode_None;
    TestThat( OptionalToken(Parser, CToken(132151u)) );
  }

  {
    parse_context Ctx = AllocateParseContext(Memory);
    counted_string ParserFilename = CSz(TEST_FIXTURES_PATH "/preprocessor/errors/error6.cpp");
    parser *Parser = PreprocessedParserForFile(&Ctx, ParserFilename, TokenCursorSource_RootFile, 0);
    Ctx.CurrentParser = Parser;
    ParseDatatypes(&Ctx);
    TestThat(Parser->ErrorCode == ParseErrorCode_ExpectedSemicolonOrEquals);
    TestThat(Parser->ErrorToken->LineNumber == 3);

    Parser->ErrorCode = ParseErrorCode_None;
    TestThat( OptionalToken(Parser, CToken(132151u)) );
  }

  {
    parse_context Ctx = AllocateParseContext(Memory);
    counted_string ParserFilename = CSz(TEST_FIXTURES_PATH "/preprocessor/errors/error7.cpp");
    parser *Parser = PreprocessedParserForFile(&Ctx, ParserFilename, TokenCursorSource_RootFile, 0);
    Ctx.CurrentParser = Parser;
    ParseDatatypes(&Ctx);
    TestThat(Parser->ErrorCode == ParseErrorCode_ExpectedSemicolonOrEquals);
    TestThat(Parser->ErrorToken->LineNumber == 3);

    Parser->ErrorCode = ParseErrorCode_None;
    TestThat( OptionalToken(Parser, CToken(132151u)) );
  }

#if 0
  {
    // NOTE(Jesse): This test is still failing.
    parse_context Ctx = AllocateParseContext(Memory);
    counted_string ParserFilename = CSz(TEST_FIXTURES_PATH "/preprocessor/errors/error8.cpp");
    parser *Parser = PreprocessedParserForFile(&Ctx, ParserFilename, TokenCursorSource_RootFile);
    Ctx.CurrentParser = Parser;
    ParseDatatypes(&Ctx);
    TestThat(Parser->ErrorCode == ParseErrorCode_ExpectedSemicolonOrEquals);
    Parser->ErrorCode = ParseErrorCode_None;
    TestThat(PeekTokenRawPointer(Parser)->LineNumber == 3);
    TestThat( OptionalToken(Parser, CToken(132151u)) );
  }
#endif

  {
    parse_context Ctx = AllocateParseContext(Memory);
    counted_string ParserFilename = CSz(TEST_FIXTURES_PATH "/preprocessor/errors/error9.cpp");
    parser *Parser = PreprocessedParserForFile(&Ctx, ParserFilename, TokenCursorSource_RootFile, 0);
    Ctx.CurrentParser = Parser;
    ParseDatatypes(&Ctx);
    TestThat(Parser->ErrorCode == ParseErrorCode_None) // TODO(Jesse, tags: tests): Curious for TestErrors .. ?
  }

  {
    parse_context Ctx = AllocateParseContext(Memory);
    counted_string ParserFilename = CSz(TEST_FIXTURES_PATH "/preprocessor/errors/error11.cpp");
    parser *Parser = PreprocessedParserForFile(&Ctx, ParserFilename, TokenCursorSource_RootFile, 0);
    Ctx.CurrentParser = Parser;
    ParseDatatypes(&Ctx);

    // NOTE(Jesse, tags: error_message, clarity): This error message isn't
    // really what we want to tell the user but as long as it breaks here we're
    // fine for now.
    TestThat(Parser->ErrorCode == ParseErrorCode_ExpectedSemicolonOrEquals);

    /* TestThat(StringsMatch(Parser->Filename, ParserFilename)); */
    TestThat(Parser->ErrorToken->LineNumber == 2);
    /* TestThat( OptionalToken(Parser, CToken(132151u)) ); */
  }

  {
    parse_context Ctx = AllocateParseContext(Memory);
    counted_string ParserFilename = CSz(TEST_FIXTURES_PATH "/preprocessor/errors/error12.cpp");
    parser *Parser = PreprocessedParserForFile(&Ctx, ParserFilename, TokenCursorSource_RootFile, 0);
    TestThat(Parser->ErrorCode == ParseErrorCode_InvalidTokenGenerated);
    TestThat(Parser->ErrorToken->LineNumber == 5);
  }

  {
    parse_context Ctx = AllocateParseContext(Memory);
    counted_string ParserFilename = CSz(TEST_FIXTURES_PATH "/preprocessor/errors/error13.cpp");
    parser *Parser = PreprocessedParserForFile(&Ctx, ParserFilename, TokenCursorSource_RootFile, 0);
    TestThat(Parser->ErrorCode == ParseErrorCode_InvalidTokenGenerated);
    TestThat(Parser->ErrorToken->LineNumber == 5);
  }

#endif

  {
    parse_context Ctx = AllocateParseContext(Memory);
    counted_string ParserFilename = CSz(TEST_FIXTURES_PATH "/preprocessor/errors/error14.cpp");
    parser *Parser = PreprocessedParserForFile(&Ctx, ParserFilename, TokenCursorSource_RootFile, 0);
    Ctx.CurrentParser = Parser;
    ParseDatatypes(&Ctx);

    TestThat(Parser->ErrorCode == ParseErrorCode_ExpectedSemicolonOrEquals);

    /* TestThat(StringsMatch(Parser->Filename, ParserFilename)); */
    /* TestThat(Parser->ErrorToken->LineNumber == 2); */
    /* TestThat( OptionalToken(Parser, CToken(132151u)) ); */
  }




}

void
TestLineNumbers(memory_arena *Memory)
{

#if 1
  {
    parse_context Ctx = AllocateParseContext(Memory);
    counted_string ParserFilename = CSz(TEST_FIXTURES_PATH "/preprocessor/errors/error5.cpp");
    parser *Parser = PreprocessedParserForFile(&Ctx, ParserFilename, TokenCursorSource_RootFile, 0);

    TestThat(PeekTokenRawPointer(Parser)->LineNumber == 1);
    TestThat(OptionalTokenRaw(Parser, CT_PreprocessorDefine));
    TestThat(OptionalTokenRaw(Parser, CTokenType_Space));
    TestThat(OptionalTokenRaw(Parser, CToken(CT_MacroLiteral, CSz("boo"))));
    TestThat(OptionalTokenRaw(Parser, CTokenType_Space));
    TestThat(OptionalTokenRaw(Parser, CToken(132151u)));

    TestThat(OptionalTokenRaw(Parser, CTokenType_Newline));
    TestThat(PeekTokenRawPointer(Parser)->LineNumber == 2);

    TestThat(OptionalTokenRaw(Parser, CTokenType_Newline));
    TestThat(PeekTokenRawPointer(Parser)->LineNumber == 3);

    TestThat(OptionalTokenRaw(Parser, CToken(CTokenType_Int, CSz("int"))));
    TestThat(OptionalTokenRaw(Parser, CTokenType_Space));
    TestThat(OptionalTokenRaw(Parser, CToken(CTokenType_Identifier, CSz("foo"))));
    TestThat(OptionalTokenRaw(Parser, CTokenType_Space));
    TestThat(OptionalTokenRaw(Parser, CToken(CT_MacroLiteral, CSz("boo"))));
    TestThat(OptionalTokenRaw(Parser, CTokenType_Space));
    TestThat(OptionalTokenRaw(Parser, CToken(132151u)));

    TestThat(OptionalTokenRaw(Parser, CTokenType_Newline));
    TestThat(PeekTokenRawPointer(Parser)->LineNumber == 4);

    // NOTE(Jesse): This is actually correct.
    //
    // `xxd src/tests/fixtures/preprocessor/errors/error5.cpp` shows that the
    // file ends with two newlines.  vim is responsible presumably.
    TestThat(OptionalTokenRaw(Parser, CTokenType_Newline));
    TestThat(PeekTokenRawPointer(Parser) == 0);


    TestThat(RewindTo(Parser, CTokenType_Newline));
    TestThat(PeekTokenRawPointer(Parser)->LineNumber == 4);

    TestThat(RewindTo(Parser, CTokenType_Newline));
    TestThat(PeekTokenRawPointer(Parser)->LineNumber == 3);

    TestThat(RewindTo(Parser, CTokenType_Newline));
    TestThat(PeekTokenRawPointer(Parser)->LineNumber == 2);

    TestThat(RewindTo(Parser, CTokenType_Newline));
    TestThat(PeekTokenRawPointer(Parser)->LineNumber == 1);

    TestThat(RewindTo(Parser, CTokenType_Newline) == 0);
    TestThat(PeekTokenRawPointer(Parser)->LineNumber == 1);

    TestThat(Parser->Tokens->At == Parser->Tokens->Start);
    TestThat(Parser->Tokens->Up.Tokens == 0);
  }

  {
    parse_context Ctx = AllocateParseContext(Memory);
    counted_string ParserFilename = CSz(TEST_FIXTURES_PATH "/preprocessor/errors/error6.cpp");
    parser *Parser = PreprocessedParserForFile(&Ctx, ParserFilename, TokenCursorSource_RootFile, 0);

    TestThat(PeekTokenRawPointer(Parser)->LineNumber == 1);

    TestThat(OptionalTokenRaw(Parser, CT_PreprocessorDefine));
    TestThat(OptionalTokenRaw(Parser, CTokenType_Space));
    TestThat(OptionalTokenRaw(Parser, CToken(CT_MacroLiteral, CSz("boo"))));
    TestThat(OptionalTokenRaw(Parser, CTokenType_Space));

    TestThat(OptionalTokenRaw(Parser, CTokenType_EscapedNewline));
    TestThat(PeekTokenRawPointer(Parser)->LineNumber == 2);

    TestThat(OptionalTokenRaw(Parser, CTokenType_Space));
    TestThat(OptionalTokenRaw(Parser, CTokenType_Space));
    TestThat(OptionalTokenRaw(Parser, CToken(132151u)));

    TestThat(OptionalTokenRaw(Parser, CTokenType_Newline));
    TestThat(PeekTokenRawPointer(Parser)->LineNumber == 3);

    TestThat(OptionalTokenRaw(Parser, CToken(CTokenType_Int, CSz("int"))));
    TestThat(OptionalTokenRaw(Parser, CTokenType_Space));
    TestThat(OptionalTokenRaw(Parser, CToken(CTokenType_Identifier, CSz("foo"))));
    TestThat(OptionalTokenRaw(Parser, CTokenType_Space));
    TestThat(OptionalTokenRaw(Parser, CToken(CT_MacroLiteral, CSz("boo"))));
    TestThat(OptionalTokenRaw(Parser, CTokenType_Space));
    TestThat(OptionalTokenRaw(Parser, CTokenType_EscapedNewline));
    TestThat(OptionalTokenRaw(Parser, CTokenType_Space));
    TestThat(OptionalTokenRaw(Parser, CTokenType_Space));
    TestThat(OptionalTokenRaw(Parser, CToken(132151u)));

    TestThat(OptionalTokenRaw(Parser, CTokenType_Newline));
    TestThat(PeekTokenRawPointer(Parser)->LineNumber == 4);

    // NOTE(Jesse): This is actually correct.
    //
    // `xxd src/tests/fixtures/preprocessor/errors/error5.cpp` shows that the
    // file ends with two newlines.  vim is responsible presumably.
    TestThat(OptionalTokenRaw(Parser, CTokenType_Newline));
    TestThat(PeekTokenRawPointer(Parser) == 0);


    TestThat(RewindTo(Parser, CTokenType_Newline));
    TestThat(PeekTokenRawPointer(Parser)->LineNumber == 4);

    TestThat(RewindTo(Parser, CTokenType_Newline));
    TestThat(PeekTokenRawPointer(Parser)->LineNumber == 3);

    TestThat(RewindTo(Parser, CTokenType_Newline));
    TestThat(PeekTokenRawPointer(Parser)->LineNumber == 2);

    TestThat(RewindTo(Parser, CTokenType_Newline) == 0);
    TestThat(PeekTokenRawPointer(Parser)->LineNumber == 1);

    TestThat(RewindTo(Parser, CTokenType_Newline) == 0);
    TestThat(PeekTokenRawPointer(Parser)->LineNumber == 1);

    TestThat(Parser->Tokens->At == Parser->Tokens->Start);
    TestThat(Parser->Tokens->Up.Tokens == 0);
  }

  {
    parse_context Ctx = AllocateParseContext(Memory);
    counted_string ParserFilename = CSz(TEST_FIXTURES_PATH "/preprocessor/errors/error7.cpp");
    parser *Parser = PreprocessedParserForFile(&Ctx, ParserFilename, TokenCursorSource_RootFile, 0);

    TestThat(PeekTokenRawPointer(Parser)->LineNumber == 1);

    TestThat(OptionalTokenRaw(Parser, CT_PreprocessorInclude));

    TestThat(OptionalTokenRaw(Parser, CT_InsertedCode));

      TestThat(PeekTokenRawPointer(Parser)->LineNumber == 1);
      TestThat(OptionalTokenRaw(Parser, CT_PreprocessorDefine));
      TestThat(OptionalTokenRaw(Parser, CTokenType_Space));
      TestThat(OptionalTokenRaw(Parser, CToken(CT_MacroLiteral, CSz("foo"))));
      TestThat(OptionalTokenRaw(Parser, CTokenType_Space));
      TestThat(OptionalTokenRaw(Parser, CToken(CSz("bar"))));

      TestThat(PeekTokenRawPointer(Parser)->LineNumber == 1);
      TestThat(OptionalTokenRaw(Parser, CTokenType_Newline)); // End of the include file


    TestThat(PeekTokenRawPointer(Parser)->LineNumber == 1);
    TestThat(OptionalTokenRaw(Parser, CTokenType_Newline));
    TestThat(PeekTokenRawPointer(Parser)->LineNumber == 2);

    TestThat(OptionalTokenRaw(Parser, CTokenType_Newline));
    TestThat(PeekTokenRawPointer(Parser)->LineNumber == 3);

    TestThat(OptionalTokenRaw(Parser, CToken(CTokenType_Int, CSz("int"))));
    TestThat(OptionalTokenRaw(Parser, CTokenType_Space));

    TestThat(OptionalTokenRaw(Parser, CToken(CT_MacroLiteral, CSz("foo"))));
    TestThat(OptionalTokenRaw(Parser, CTokenType_Space));
    TestThat(OptionalTokenRaw(Parser, CToken(CSz("bar"))));
    TestThat(OptionalTokenRaw(Parser, CTokenType_Space));
    TestThat(OptionalToken(Parser, CToken(132151u)));

    TestThat(OptionalTokenRaw(Parser, CTokenType_Newline));
    TestThat(PeekTokenRawPointer(Parser)->LineNumber == 4);

    TestThat(OptionalTokenRaw(Parser, CTokenType_Newline));

    TestThat(PeekTokenRawPointer(Parser) == 0);


    TestThat(RewindTo(Parser, CTokenType_Newline));
    TestThat(PeekTokenRawPointer(Parser)->LineNumber == 4);

    TestThat(RewindTo(Parser, CTokenType_Newline));
    TestThat(PeekTokenRawPointer(Parser)->LineNumber == 3);

    TestThat(RewindTo(Parser, CTokenType_Newline));
    TestThat(PeekTokenRawPointer(Parser)->LineNumber == 2);

    TestThat(RewindTo(Parser, CTokenType_Newline));
    TestThat(PeekTokenRawPointer(Parser)->LineNumber == 1);

    TestThat(RewindTo(Parser, CTokenType_Newline));
    TestThat(PeekTokenRawPointer(Parser)->LineNumber == 1);

    TestThat(RewindTo(Parser, CTokenType_Newline) == 0);
    TestThat(PeekTokenRawPointer(Parser)->LineNumber == 1);

    TestThat(Parser->Tokens->At == Parser->Tokens->Start);
    TestThat(Parser->Tokens->Up.Tokens == 0);
  }
#endif

#if 0
  {
    // NOTE(Jesse): Debug code
    parse_context Ctx = AllocateParseContext(Memory);
    counted_string ParserFilename = CSz(TEST_FIXTURES_PATH "/preprocessor/errors/error8.cpp");
    parser *Parser = PreprocessedParserForFile(&Ctx, ParserFilename, TokenCursorSource_RootFile);
    DumpChain(Parser);
  }
#endif

}

#endif

typedef c_token (*struct_peek_func)(parser *, s32);
void ValidateParser_Struct(struct_peek_func Peeker, parser *Parser, c_token *ExpectedTokens, s32 TokenCount)
{
  {
    c_token T = Peeker(Parser, -2);
    TestThat(T == 0);
  }
  {
    c_token T = Peeker(Parser, -1);
    TestThat(T == 0);
  }

  s32 At = 0;
  for (; At < TokenCount; ++At)
  {
    c_token T = Peeker(Parser, At);
    TestThat(T == ExpectedTokens[At]);
  }

  {
    c_token T = Peeker(Parser, At++);
    TestThat(T == 0);
  }
  {
    c_token T = Peeker(Parser, At++);
    TestThat(T == 0);
  }
}
typedef c_token* (*pop_struct_func)(parser *, s32);
typedef c_token* (*peek_struct_func)(parser *, s32);

typedef c_token* (*require_pointer_func)(parser *, c_token);
typedef c_token* (*pop_pointer_func)(parser *);
typedef c_token* (*peek_pointer_func)(parser *, s32);

void ValidateParser(peek_pointer_func Peeker, pop_pointer_func Popper, parser *Parser, c_token *ExpectedTokens, s32 TokenCount)
{
  {
    auto T = Peeker(Parser, -2);
    TestThat(T == 0);
    Peeker(Parser, -2);
  }
  {
    auto T = Peeker(Parser, -1);
    TestThat(T == 0);
    Peeker(Parser, -1);
  }

  s32 At = 0;
  for (; At < TokenCount; At++)
  {
    for (s32 AbsOffset = 0; AbsOffset < TokenCount; AbsOffset++)
    {
      Assert(AbsOffset < TokenCount);
      int PeekOffset = AbsOffset - At;
      auto T = Peeker(Parser, PeekOffset);
      TestThat( AreEqual(T, ExpectedTokens[AbsOffset]) );
      Peeker(Parser, PeekOffset);
    }

    TestThat(ExpectedTokens[At] == *(Popper(Parser)));
  }

  At = TokenCount;
  {
    auto T = Peeker(Parser, At++);
    TestThat(T == 0);
    Peeker(Parser, At);
  }
  {
    auto T = Peeker(Parser, At++);
    TestThat(T == 0);
    Peeker(Parser, At);
  }

  FullRewind(Parser);
}

void TestThings(memory_arena *Memory)
{
  parse_context Ctx = AllocateParseContext(Memory);
  counted_string ParserFilename = CSz(TEST_FIXTURES_PATH "/preprocessor/thing.cpp");
  parser *Parser = PreprocessedParserForFile(&Ctx, ParserFilename, TokenCursorSource_RootFile, 0);

#if 0
  {
    c_token T = PeekTokenRaw(Parser);
    TestThat(T == CToken(CT_PreprocessorDefine, CSz("#define")));
  }
  {
    c_token T = PeekTokenRaw(Parser, 1);
    TestThat(T == CToken(CTokenType_Space, CSz(" ")));
  }
  {
    c_token T = PeekTokenRaw(Parser, 2);
    TestThat(T == CToken(CT_MacroLiteral, CSz("foo")));
  }
  {
    c_token T = PeekTokenRaw(Parser, 3);
    TestThat(T == CToken(CTokenType_Space, CSz(" ")));
  }
  {
    c_token T = PeekTokenRaw(Parser, 4);
    TestThat(T == CToken(CTokenType_Identifier, CSz("bar")));
  }
  {
    c_token T = PeekTokenRaw(Parser, 5);
    TestThat(T == CToken(CTokenType_Newline, CSz("\n")));
  }
  {
    c_token T = PeekTokenRaw(Parser, 6);
    TestThat(T == CToken(CT_MacroLiteral, CSz("foo")));
  }
  {
    c_token T = PeekTokenRaw(Parser, 7);
    TestThat(T == CToken(CTokenType_Space, CSz(" ")));
  }
  {
    c_token T = PeekTokenRaw(Parser, 8);
    TestThat(T == CToken(CTokenType_Identifier, CSz("bar")));
  }
  {
    c_token T = PeekTokenRaw(Parser, 9);
    TestThat(T == CToken(CTokenType_Space, CSz(" ")));
  }
  {
    c_token T = PeekTokenRaw(Parser, 10);
    TestThat(T == CToken(CTokenType_Identifier, CSz("boo")));
  }
#endif




  {
    c_token *T = PeekTokenPointer(Parser, 0);
    TestThat(*T == CToken(CTokenType_Identifier, CSz("bar")));
  }
  {
    c_token *T = PeekTokenPointer(Parser, 1);
    TestThat(*T == CToken(CTokenType_Identifier, CSz("boo")));
  }
#if 0
  {
    c_token *T = PeekTokenPointer(Parser, 2).At;
    TestThat(*T == CToken(CTokenType_Identifier, CSz("bar")));
  }
  {
    c_token *T = PeekTokenPointer(Parser, 3).At;
    TestThat(*T == CToken(CTokenType_Identifier, CSz("bar")));
  }
  {
    c_token *T = PeekTokenPointer(Parser, 4).At;
    TestThat(*T == CToken(CTokenType_Identifier, CSz("boo")));
  }
  {
    c_token *T = PeekTokenPointer(Parser, 5).At;
    TestThat(T == 0);
  }
#endif



}


void TestSingleCursorTokenControl(memory_arena *Memory)
{
  parse_context Ctx = AllocateParseContext(Memory);

  parser *Parser = ParserForFile( &Ctx,
                                  CSz(TEST_FIXTURES_PATH "/preprocessor/single_cursor_token_control.cpp"),
                                  TokenCursorSource_RootFile);
  {
    c_token ExpectedTokens[] =
    {
      CToken(CTokenType_Identifier, CSz("foo")),
      CToken(CTokenType_Space, CSz(" ")),
      CToken(CTokenType_Identifier, CSz("bar")),
      CToken(CTokenType_Space, CSz(" ")),
      CToken(CTokenType_Identifier, CSz("baz")),
      CToken(CTokenType_Newline, CSz("\n")),
    };
    ValidateParser(PeekTokenRawPointer, PopTokenRawPointer, Parser, ExpectedTokens, ArrayCount(ExpectedTokens));

  }

  FullRewind(Parser);

  {
    c_token ExpectedTokens[] =
    {
      CToken(CTokenType_Identifier, CSz("foo")),
      CToken(CTokenType_Identifier, CSz("bar")),
      CToken(CTokenType_Identifier, CSz("baz")),
    };

    s32 At = 0;
    for (; At < (s32)ArrayCount(ExpectedTokens); At++)
    {
      for (s32 AbsOffset = 0; AbsOffset < (s32)ArrayCount(ExpectedTokens); AbsOffset++)
      {
        int PeekOffset = AbsOffset - At;
        c_token *T = PeekTokenPointer(Parser, PeekOffset);
        TestThat(*T == ExpectedTokens[AbsOffset]);
      }

      RequireToken(Parser, ExpectedTokens[At]);
    }

    FullRewind(Parser);

    ValidateParser(PeekTokenPointer, PopTokenPointer, Parser, ExpectedTokens, ArrayCount(ExpectedTokens));
  }
}

void TestMultiCursorTokenControl(memory_arena *Memory)
{
  parse_context Ctx = AllocateParseContext(Memory);

  parser *Parser = ParserForFile( &Ctx,
                                  CSz(TEST_FIXTURES_PATH "/preprocessor/multi_cursor_token_control.cpp"),
                                  TokenCursorSource_RootFile);

  // TODO(Jesse): Make an actual function that does this?  Seems like that
  // would be slightly better factored so we don't have to leak the parser
  // struct here.  This also happens in real code at: @leak_parser_struct
  c_token_cursor *DownTokens = ParserForFile( &Ctx,
                                              CSz(TEST_FIXTURES_PATH "/preprocessor/single_cursor_token_control.cpp"),
                                              TokenCursorSource_RootFile)->Tokens;

  {
    c_token *T = PeekTokenPointer(Parser, 1);
    TestThat(*T == CToken(CTokenType_Identifier, CSz("insert_code_here")));

    T->Type = CT_InsertedCode;
    T->Value = CSz("");
    T->Down = DownTokens;
    T->Erased = True;

    DownTokens->Up.Tokens = Parser->Tokens;
    DownTokens->Up.At = T;
  }

  {
    c_token ExpectedTokens[] =
    {
      CToken(CTokenType_Identifier, CSz("foo_outer")),
      CToken(CTokenType_Space, CSz(" ")),
      CToken(CT_InsertedCode),
      CToken(CTokenType_Identifier, CSz("foo")),
      CToken(CTokenType_Space, CSz(" ")),
      CToken(CTokenType_Identifier, CSz("bar")),
      CToken(CTokenType_Space, CSz(" ")),
      CToken(CTokenType_Identifier, CSz("baz")),
      CToken(CTokenType_Newline, CSz("\n")),
      CToken(CTokenType_Space, CSz(" ")),
      CToken(CTokenType_Identifier, CSz("baz_outer")),
      CToken(CTokenType_Newline, CSz("\n")),
    };

    ValidateParser(PeekTokenRawPointer, PopTokenRawPointer, Parser, ExpectedTokens, ArrayCount(ExpectedTokens));
    ValidateParser_Struct(PeekTokenRaw, Parser, ExpectedTokens, ArrayCount(ExpectedTokens));
  }

  {
    c_token ExpectedTokens[] =
    {
      CToken(CTokenType_Identifier, CSz("foo_outer")),
      CToken(CTokenType_Identifier, CSz("foo")),
      CToken(CTokenType_Identifier, CSz("bar")),
      CToken(CTokenType_Identifier, CSz("baz")),
      CToken(CTokenType_Identifier, CSz("baz_outer")),
    };

    ValidateParser(PeekTokenPointer, PopTokenPointer, Parser, ExpectedTokens, ArrayCount(ExpectedTokens));
    ValidateParser_Struct(PeekToken, Parser, ExpectedTokens, ArrayCount(ExpectedTokens));
  }

#if 0
  // Peeking 
  s32 At = 0;
  {
    c_token *T = PeekTokenRawPointer(Parser, At++);
    TestThat(*T == CToken(CTokenType_Identifier, CSz("foo")));
  }
  {
    c_token *T = PeekTokenRawPointer(Parser, At++);
    TestThat(*T == CToken(CTokenType_Space, CSz(" ")));
  }
  {
    c_token *T = PeekTokenRawPointer(Parser, At++);
    TestThat(*T == CToken(CT_InsertedCode));
  }
  {
    c_token *T = PeekTokenRawPointer(Parser, At++);
    TestThat(*T == CToken(CTokenType_Identifier, CSz("foo")));
  }
  {
    c_token *T = PeekTokenRawPointer(Parser, At++);
    TestThat(*T == CToken(CTokenType_Space, CSz(" ")));
  }
  {
    c_token *T = PeekTokenRawPointer(Parser, At++);
    TestThat(*T == CToken(CTokenType_Identifier, CSz("bar")));
  }
  {
    c_token *T = PeekTokenRawPointer(Parser, At++);
    TestThat(*T == CToken(CTokenType_Space, CSz(" ")));
  }
  {
    c_token *T = PeekTokenRawPointer(Parser, At++);
    TestThat(*T == CToken(CTokenType_Identifier, CSz("baz")));
  }
  {
    c_token *T = PeekTokenRawPointer(Parser, At++);
    TestThat(*T == CToken(CTokenType_Newline, CSz("\n")));
  }
  {
    c_token *T = PeekTokenRawPointer(Parser, At++);
    TestThat(T == 0);
  }
  {
    c_token *T = PeekTokenRawPointer(Parser, At++);
    TestThat(T == 0);
  }
  {
    c_token *T = PeekTokenRawPointer(Parser, -1);
    TestThat(T == 0);
  }
#endif

}

s32
main(s32 ArgCount, const char** Args)
{
  TestSuiteBegin("Preprocessor", ArgCount, Args);

  /* Global_LogLevel = LogLevel_Debug; */
  /* Global_LogLevel = LogLevel_Error; */
  /* Global_LogLevel = LogLevel_Shush; */


  memory_arena* Memory = AllocateArena();


#if 1
  TestSingleCursorTokenControl(Memory);
  TestMultiCursorTokenControl(Memory);
  // TODO(Jesse): Axe this or turn it into something more meaningful
  TestThings(Memory);
  TestDoublyLinkedListSwap();
  TestBoundaryConditions(Memory);
  TestPeekAndPopTokens(Memory);
  TestParserChain(Memory);
  TestBasicTokenizationAndParsing(Memory);
  TestStructParsing(Memory);
  TestCommentSituation(Memory);
  TestMacrosAndIncludes(Memory);
  TestIncludeGuards(Memory);
  TestDefinesAndConditionals(Memory);
  TestLogicalOperators(Memory);
  TestLineNumbers(Memory);
  TestErrors(Memory);
#endif
  TestAst(Memory);

  TestSuiteEnd();
  exit(TestsFailed);
}
