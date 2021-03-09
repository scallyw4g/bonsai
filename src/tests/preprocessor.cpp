#define EXCLUDE_PREPROCESSOR_MAIN 1
#include <metaprogramming/preprocessor.cpp>
#include <tests/test_utils.cpp>

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
  parser *Parser = ParserForFile(CS(TEST_FIXTURES_PATH "/preprocessor_basic.cpp"), Memory);

  if (Parser)
  {
    {
      c_token T = PopTokenRaw(Parser);
      TestThat(T.Type == CTokenType_CommentSingleLine);
    }
    {
      c_token T = PopToken(Parser);
      TestThat(T == CToken(CS("single")));
    }
    {
      c_token T = PopToken(Parser);
      TestThat(T == CToken(CS("line")));
    }
    {
      c_token T = PopToken(Parser);
      TestThat(T == CToken(CS("comment")));
    }
    {
      c_token T = PopToken(Parser);
      TestThat(T.Type == CTokenType_SingleQuote);
    }
    {
      c_token T = PopToken(Parser);
      TestThat(T == CToken(CS("s")));
    }
    {
      c_token T = PopToken(Parser);
      TestThat(T.Type == CTokenType_DoubleQuote);
    }
    {
      c_token T = PopToken(Parser);
      TestThat(T == CToken(CS("work")));
    }
    {
      c_token T = PopToken(Parser);
      TestThat(T.Type == CTokenType_DoubleQuote);
    }


    {
      c_token T = PopTokenRaw(Parser);
      TestThat(T.Type == CTokenType_Newline);
    }
    {
      c_token T = PopTokenRaw(Parser);
      TestThat(T.Type == CTokenType_CommentMultiLineStart);
    }
    {
      c_token T = PopToken(Parser);
      TestThat(T == CToken(CS("multi")));
    }
    {
      c_token T = PopToken(Parser);
      TestThat(T == CToken(CS("line")));
    }
    {
      c_token T = PopToken(Parser);
      TestThat(T == CToken(CS("comment")));
    }
    {
      c_token T = PopToken(Parser);
      TestThat(T.Type == CTokenType_SingleQuote);
    }
    {
      c_token T = PopToken(Parser);
      TestThat(T == CToken(CS("s")));
    }
    {
      c_token T = PopToken(Parser);
      TestThat(T.Type == CTokenType_DoubleQuote);
    }
    {
      c_token T = PopToken(Parser);
      TestThat(T == CToken(CS("work")));
    }
    {
      c_token T = PopToken(Parser);
      TestThat(T.Type == CTokenType_DoubleQuote);
    }
    {
      c_token T = PopTokenRaw(Parser);
      TestThat(T.Type == CTokenType_Space);
    }

    {
      c_token T = PopTokenRaw(Parser);
      TestThat(T.Type == CTokenType_CommentMultiLineEnd);
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

    TestThat(Remaining(&Parser->Tokens) == 0);
  }
  else
  {
    ++TestsFailed;
  }
}

bonsai_function void
TestPeekAndPopTokens(memory_arena* Memory)
{
  parser *Parser = ParserForFile(CS(TEST_FIXTURES_PATH "/preprocessor_peek_pop.cpp"), Memory);

  if (Parser)
  {
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
  }
  else
  {
    ++TestsFailed;
  }

  return;
}

bonsai_function void
TestStructParsing(memory_arena* Memory)
{
  parser *Parser = ParserForFile(CS(TEST_FIXTURES_PATH "/preprocessor_datatypes.cpp"), Memory);

  if (Parser)
  {
    parse_context Ctx = {
      .CurrentParser = *Parser,
      .Memory = Memory,
    };

    ParseDatatypes(&Ctx);
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
  parser *Parser = ParserForFile(CS(TEST_FIXTURES_PATH "/comments.cpp"), Memory);

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
  parse_context Ctx = {
    .Memory = Memory,
  };

  parser *Parser = ParserForFile(&Ctx, CS(TEST_FIXTURES_PATH "/preprocessor/should_parse.cpp"));
  if (Parser)
  {
    Ctx.CurrentParser = *Parser;

    ParseDatatypes(&Ctx);

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
  parse_context Ctx = {
    .Memory = Memory,
  };

  parser *Parser = ParserForFile(&Ctx, CS(TEST_FIXTURES_PATH "/preprocessor/macro_and_include_test.cpp"));

  if (Parser)
  {
    DumpEntireParser(Parser);
    return;

    TestThat(Parser->Next);
    TestThat(Parser->Next->Next);
    TestThat(Parser->Next->Next);

    RequireToken(Parser, CTokenType_Int);
    RequireToken(Parser, CToken(CSz("ding")));
    RequireToken(Parser, CTokenType_Equals);
    RequireToken(Parser, CToken(42u));
    RequireToken(Parser, CTokenType_Semicolon);

    RequireToken(Parser, CTokenType_Int);
    RequireToken(Parser, CToken(CSz("thing")));
    RequireToken(Parser, CTokenType_Equals);
    RequireToken(Parser, CToken(1337u));
    RequireToken(Parser, CTokenType_Semicolon);

    RequireToken(Parser, CT_PreprocessorDefine);
    RequireToken(Parser, CToken(CT_MacroLiteral, CS("MacroKeyword")));
    RequireToken(Parser, CToken(CS("this_is_a_variable_name")));

    RequireToken(Parser, CT_PreprocessorDefine);
    RequireToken(Parser, CToken(CT_MacroLiteral, CS("IndirectMacroKeyword")));
    RequireToken(Parser, CToken(CT_MacroLiteral, CS("MacroKeyword")));

    RequireToken(Parser, CTokenType_Int);
    RequireToken(Parser, CToken(CSz("this_is_a_variable_name")));
    RequireToken(Parser, CTokenType_Equals);
    RequireToken(Parser, CToken(42u));
    RequireToken(Parser, CTokenType_Semicolon);

    RequireToken(Parser, CTokenType_Int);
    RequireToken(Parser, CToken(CSz("this_is_a_variable_name")));
    RequireToken(Parser, CTokenType_Equals);
    RequireToken(Parser, CToken(42u));
    RequireToken(Parser, CTokenType_Semicolon);

    RequireToken(Parser, CT_PreprocessorDefine);
    RequireToken(Parser, CToken(CT_MacroLiteral, CS("MacroFunction")));
    RequireToken(Parser, CToken(CTokenType_OpenParen));
    RequireToken(Parser, CToken(CS("a")));
    RequireToken(Parser, CToken(CTokenType_CloseParen));
    RequireToken(Parser, CToken(CS("a")));

    RequireToken(Parser, CT_PreprocessorDefine);
    RequireToken(Parser, CToken(CT_MacroLiteral, CS("MacroFunction2")));
    RequireToken(Parser, CToken(CTokenType_OpenParen));
    RequireToken(Parser, CToken(CS("a")));
    RequireToken(Parser, CToken(CTokenType_CloseParen));
    RequireToken(Parser, CTokenType_Int);
    RequireToken(Parser, CToken(CT_MacroLiteral, CS("MacroKeyword")));
    RequireToken(Parser, CTokenType_Equals);
    RequireToken(Parser, CToken(CS("a")));

    RequireToken(Parser, CT_PreprocessorDefine);
    RequireToken(Parser, CToken(CT_MacroLiteral, CS("MacroFunction3")));
    RequireToken(Parser, CToken(CTokenType_OpenParen));
    RequireToken(Parser, CToken(CS("a")));
    RequireToken(Parser, CToken(CTokenType_Comma));
    RequireToken(Parser, CToken(CS("b")));
    RequireToken(Parser, CToken(CTokenType_Comma));
    RequireToken(Parser, CToken(CS("c")));
    RequireToken(Parser, CToken(CTokenType_Comma));
    RequireToken(Parser, CToken(CS("d")));
    RequireToken(Parser, CToken(CTokenType_CloseParen));
    RequireToken(Parser, CToken(CS("a")));
    RequireToken(Parser, CToken(CS("b")));
    RequireToken(Parser, CToken(CS("c")));
    RequireToken(Parser, CToken(CS("d")));

    RequireToken(Parser, CT_PreprocessorDefine);
    RequireToken(Parser, CToken(CT_MacroLiteral, CS("MacroFunction4")));
    RequireToken(Parser, CToken(CTokenType_OpenParen));
    RequireToken(Parser, CToken(CS("a")));
    RequireToken(Parser, CToken(CTokenType_Comma));
    RequireToken(Parser, CToken(CTokenType_Ellipsis));
    RequireToken(Parser, CToken(CTokenType_CloseParen));
    RequireToken(Parser, CToken(CS("a")));

    RequireToken(Parser, CT_PreprocessorDefine);
    RequireToken(Parser, CToken(CT_MacroLiteral, CS("MacroFunction5")));
    RequireToken(Parser, CToken(CTokenType_OpenParen));
    RequireToken(Parser, CToken(CS("a")));
    RequireToken(Parser, CToken(CTokenType_Comma));
    RequireToken(Parser, CToken(CS("b")));
    RequireToken(Parser, CToken(CTokenType_Comma));
    RequireToken(Parser, CToken(CTokenType_Ellipsis));
    RequireToken(Parser, CToken(CTokenType_CloseParen));
    RequireToken(Parser, CToken(CS("a")));
    RequireToken(Parser, CToken(CS("b")));
    RequireToken(Parser, CToken(CT_Preprocessor__VA_ARGS__));


    RequireToken(Parser, CT_PreprocessorDefine);
    RequireToken(Parser, CToken(CT_MacroLiteral, CS("MacroFunction6")));
    RequireToken(Parser, CToken(CTokenType_OpenParen));
    RequireToken(Parser, CToken(CTokenType_Ellipsis));
    RequireToken(Parser, CToken(CTokenType_CloseParen));
    RequireToken(Parser, CToken(CT_Preprocessor__VA_ARGS__));


    RequireToken(Parser, CT_PreprocessorDefine);
    RequireToken(Parser, CToken(CT_MacroLiteral, CS("MacroFunction7")));
    RequireToken(Parser, CToken(CTokenType_OpenParen));
    RequireToken(Parser, CToken(CS("a")));
    RequireToken(Parser, CToken(CTokenType_Comma));
    RequireToken(Parser, CToken(CS("b")));
    RequireToken(Parser, CToken(CTokenType_CloseParen));
    RequireToken(Parser, CToken(CS("a")));
    RequireToken(Parser, CToken(CS("b")));


    // MacroFunction


    RequireToken(Parser, CTokenType_Int);
    RequireToken(Parser, CToken(CSz("this_is_a_variable_name")));
    RequireToken(Parser, CTokenType_Equals);
    RequireToken(Parser, CToken(42u));
    RequireToken(Parser, CTokenType_Semicolon);

    RequireToken(Parser, CTokenType_Int);
    RequireToken(Parser, CToken(CSz("this_is_a_variable_name")));
    RequireToken(Parser, CTokenType_Equals);
    RequireToken(Parser, CToken(42u));
    RequireToken(Parser, CTokenType_Semicolon);

    RequireToken(Parser, CToken(CSz("SomeRegularFunctionCall")));
    RequireToken(Parser, CTokenType_OpenParen);
    RequireToken(Parser, CToken(42u));
    RequireToken(Parser, CTokenType_CloseParen);
    RequireToken(Parser, CTokenType_Semicolon);


    // MacroFunction2


    RequireToken(Parser, CTokenType_Int);
    RequireToken(Parser, CToken(CSz("this_is_a_variable_name")));
    RequireToken(Parser, CTokenType_Equals);
    RequireToken(Parser, CToken(42u));
    RequireToken(Parser, CTokenType_Semicolon);

    RequireToken(Parser, CTokenType_Int);
    RequireToken(Parser, CToken(CSz("this_is_a_variable_name")));
    RequireToken(Parser, CTokenType_Equals);
    RequireToken(Parser, CToken(42u));
    RequireToken(Parser, CTokenType_Semicolon);

    RequireToken(Parser, CTokenType_Int);
    RequireToken(Parser, CToken(CSz("this_is_a_variable_name")));
    RequireToken(Parser, CTokenType_Equals);
    RequireToken(Parser, CToken(42u));
    RequireToken(Parser, CTokenType_Semicolon);


    // MacroFunction3


    RequireToken(Parser, CTokenType_Int);
    RequireToken(Parser, CToken(CSz("this_is_a_variable_name")));
    RequireToken(Parser, CTokenType_Equals);
    RequireToken(Parser, CToken(42u));
    RequireToken(Parser, CTokenType_Semicolon);

    RequireToken(Parser, CTokenType_Int);
    RequireToken(Parser, CToken(CSz("this_is_a_variable_name")));
    RequireToken(Parser, CTokenType_Equals);
    RequireToken(Parser, CToken(42u));
    RequireToken(Parser, CTokenType_Semicolon);

    RequireToken(Parser, CTokenType_Int);
    RequireToken(Parser, CToken(CSz("this_is_a_variable_name")));
    RequireToken(Parser, CTokenType_Equals);
    RequireToken(Parser, CToken(42u));
    RequireToken(Parser, CTokenType_Semicolon);


    // MacroFunction4


    RequireToken(Parser, CTokenType_Int);
    RequireToken(Parser, CToken(CSz("this_is_a_variable_name")));
    RequireToken(Parser, CTokenType_Equals);
    RequireToken(Parser, CToken(42u));
    RequireToken(Parser, CTokenType_Semicolon);

    RequireToken(Parser, CTokenType_Int);
    RequireToken(Parser, CToken(CSz("this_is_a_variable_name")));
    RequireToken(Parser, CTokenType_Equals);
    RequireToken(Parser, CToken(42u));
    RequireToken(Parser, CTokenType_Semicolon);


    // MacroFunction5


    RequireToken(Parser, CTokenType_Int);
    RequireToken(Parser, CToken(CSz("this_is_a_variable_name")));
    RequireToken(Parser, CTokenType_Equals);
    RequireToken(Parser, CToken(42u));
    RequireToken(Parser, CTokenType_Semicolon);


    // MacroFunction6


    RequireToken(Parser, CTokenType_Int);
    RequireToken(Parser, CToken(CSz("this_is_a_variable_name")));
    RequireToken(Parser, CTokenType_Equals);
    RequireToken(Parser, CToken(42u));
    RequireToken(Parser, CTokenType_Semicolon);

    RequireToken(Parser, CTokenType_Int);
    RequireToken(Parser, CToken(CSz("this_is_a_variable_name")));
    RequireToken(Parser, CTokenType_Equals);
    RequireToken(Parser, CToken(42u));
    RequireToken(Parser, CTokenType_Semicolon);

    RequireToken(Parser, CTokenType_Int);
    RequireToken(Parser, CToken(CSz("this_is_a_variable_name")));
    RequireToken(Parser, CTokenType_Equals);
    RequireToken(Parser, CToken(42u));
    RequireToken(Parser, CTokenType_Semicolon);

    RequireToken(Parser, CTokenType_Int);
    RequireToken(Parser, CToken(CSz("this_is_a_variable_name")));
    RequireToken(Parser, CTokenType_Equals);
    RequireToken(Parser, CToken(42u));
    RequireToken(Parser, CTokenType_Semicolon);


    // MacroFunction7


    RequireToken(Parser, CToken(CSz("SomeRegularFunctionCall")));
    RequireToken(Parser, CTokenType_OpenParen);
    RequireToken(Parser, CToken(4u));
    RequireToken(Parser, CTokenType_Comma);
    RequireToken(Parser, CToken(2u));
    RequireToken(Parser, CTokenType_CloseParen);
    RequireToken(Parser, CTokenType_Semicolon);


    RequireToken(Parser, CToken(CSz("SomeRegularFunctionCall")));
    RequireToken(Parser, CTokenType_OpenParen);
    RequireToken(Parser, CToken(42u));
    RequireToken(Parser, CTokenType_CloseParen);
    RequireToken(Parser, CTokenType_Semicolon);

    RequireToken(Parser, CT_PreprocessorDefine);
    RequireToken(Parser, CT_MacroLiteral);
    RequireToken(Parser, CTokenType_OpenParen);
    RequireToken(Parser, CToken(1u));
    RequireToken(Parser, CTokenType_Minus);
    RequireToken(Parser, CToken(1u));
    RequireToken(Parser, CTokenType_CloseParen);

    RequireToken(Parser, CT_PreprocessorIf);
    TestThat(ResolveMacroConstantExpression(Parser, Memory, 0, False) == 0);
    EatIfBlock(Parser);

    RequireToken(Parser, CT_PreprocessorIf);
    TestThat(ResolveMacroConstantExpression(Parser, Memory, 0, False) == 0);
    EatIfBlock(Parser);

    RequireToken(Parser, CT_PreprocessorIf);
    TestThat(ResolveMacroConstantExpression(Parser, Memory, 0, False) == 0);
    EatIfBlock(Parser);

    RequireToken(Parser, CT_PreprocessorIf);
    TestThat(ResolveMacroConstantExpression(Parser, Memory, 0, False) == 0);
    EatIfBlock(Parser);

    RequireToken(Parser, CT_PreprocessorIf);
    TestThat(ResolveMacroConstantExpression(Parser, Memory, 0, False) == 0);
    EatIfBlock(Parser);

    RequireToken(Parser, CT_PreprocessorIf);
    TestThat(ResolveMacroConstantExpression(Parser, Memory, 0, False) == 0);
    EatIfBlock(Parser);

    RequireToken(Parser, CT_PreprocessorIf);
    TestThat(ResolveMacroConstantExpression(Parser, Memory, 0, False) == 0);
    EatIfBlock(Parser);


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
  parse_context Ctx = {
    .Memory = Memory,
  };

  parser *Parser = ParserForFile(&Ctx, CS(TEST_FIXTURES_PATH "/preprocessor/include_guard0.cpp"));
  if (Parser)
  {
    Ctx.CurrentParser = *Parser;
    /* DumpEntireParser(Parser); */
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
TestDefinesAndConditionals(memory_arena *Memory)
{
  parse_context Ctx = {
    .Memory = Memory,
  };

  parser *Parser = ParserForFile(&Ctx, CS(TEST_FIXTURES_PATH "/preprocessor/defines_and_conditionals.cpp"));
  if (Parser)
  {
    Ctx.CurrentParser = *Parser;
    /* DumpEntireParser(Parser); */

    while(TokensRemain(Parser))
    {
      RequireToken(Parser, CToken(CSz("valid_path")));
      TestThat(Parser->Valid);
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
  parse_context Ctx = {
    .Memory = Memory,
  };

  parser *Parser = ParserForFile(&Ctx, CS(TEST_FIXTURES_PATH "/preprocessor/logical_operators.cpp"));
  if (Parser)
  {
    Ctx.CurrentParser = *Parser;
    /* DumpEntireParser(Parser); */

    while(TokensRemain(Parser))
    {
      RequireToken(Parser, CToken(CSz("valid_path")));
      TestThat(Parser->Valid);
    }

  }
  else
  {
    ++TestsFailed;
  }
}

s32
main()
{
  TestSuiteBegin("Preprocessor");

  memory_arena* Memory = AllocateArena();

  /* TestBasicTokenizationAndParsing(Memory); */

  /* TestPeekAndPopTokens(Memory); */

  /* TestStructParsing(Memory); */

  /* TestCommentSituation(Memory); */

  TestMacrosAndIncludes(Memory);

  /* TestIncludeGuards(Memory); */

  /* TestAst(Memory); */

  /* TestDefinesAndConditionals(Memory); */

  /* TestLogicalOperators(Memory); */

  TestSuiteEnd();
  exit(TestsFailed);
}
