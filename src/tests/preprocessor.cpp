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
      .CurrentParser = Parser,
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
    Ctx.CurrentParser = Parser;

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

    /* RequireToken(Parser, CT_PreprocessorDefine); */
    /* RequireToken(Parser, CToken(CT_MacroLiteral, CS("MacroKeyword"))); */
    /* RequireToken(Parser, CToken(CS("this_is_a_variable_name"))); */

    /* RequireToken(Parser, CT_PreprocessorDefine); */
    /* RequireToken(Parser, CToken(CT_MacroLiteral, CS("IndirectMacroKeyword"))); */
    /* RequireToken(Parser, CToken(CT_MacroLiteral, CS("MacroKeyword"))); */

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

    /* RequireToken(Parser, CT_PreprocessorDefine); */
    /* RequireToken(Parser, CToken(CT_MacroLiteral, CS("MacroFunction"))); */
    /* RequireToken(Parser, CToken(CTokenType_OpenParen)); */
    /* RequireToken(Parser, CToken(CS("a"))); */
    /* RequireToken(Parser, CToken(CTokenType_CloseParen)); */
    /* RequireToken(Parser, CToken(CS("a"))); */

    /* RequireToken(Parser, CT_PreprocessorDefine); */
    /* RequireToken(Parser, CToken(CT_MacroLiteral, CS("MacroFunction2"))); */
    /* RequireToken(Parser, CToken(CTokenType_OpenParen)); */
    /* RequireToken(Parser, CToken(CS("a"))); */
    /* RequireToken(Parser, CToken(CTokenType_CloseParen)); */
    /* RequireToken(Parser, CTokenType_Int); */
    /* RequireToken(Parser, CToken(CT_MacroLiteral, CS("MacroKeyword"))); */
    /* RequireToken(Parser, CTokenType_Equals); */
    /* RequireToken(Parser, CToken(CS("a"))); */

    /* RequireToken(Parser, CT_PreprocessorDefine); */
    /* RequireToken(Parser, CToken(CT_MacroLiteral, CS("MacroFunction3"))); */
    /* RequireToken(Parser, CToken(CTokenType_OpenParen)); */
    /* RequireToken(Parser, CToken(CS("a"))); */
    /* RequireToken(Parser, CToken(CTokenType_Comma)); */
    /* RequireToken(Parser, CToken(CS("b"))); */
    /* RequireToken(Parser, CToken(CTokenType_Comma)); */
    /* RequireToken(Parser, CToken(CS("c"))); */
    /* RequireToken(Parser, CToken(CTokenType_Comma)); */
    /* RequireToken(Parser, CToken(CS("d"))); */
    /* RequireToken(Parser, CToken(CTokenType_CloseParen)); */
    /* RequireToken(Parser, CToken(CS("a"))); */
    /* RequireToken(Parser, CToken(CS("b"))); */
    /* RequireToken(Parser, CToken(CS("c"))); */
    /* RequireToken(Parser, CToken(CS("d"))); */

    /* RequireToken(Parser, CT_PreprocessorDefine); */
    /* RequireToken(Parser, CToken(CT_MacroLiteral, CS("MacroFunction4"))); */
    /* RequireToken(Parser, CToken(CTokenType_OpenParen)); */
    /* RequireToken(Parser, CToken(CS("a"))); */
    /* RequireToken(Parser, CToken(CTokenType_Comma)); */
    /* RequireToken(Parser, CToken(CTokenType_Ellipsis)); */
    /* RequireToken(Parser, CToken(CTokenType_CloseParen)); */
    /* RequireToken(Parser, CToken(CS("a"))); */

    /* RequireToken(Parser, CT_PreprocessorDefine); */
    /* RequireToken(Parser, CToken(CT_MacroLiteral, CS("MacroFunction5"))); */
    /* RequireToken(Parser, CToken(CTokenType_OpenParen)); */
    /* RequireToken(Parser, CToken(CS("a"))); */
    /* RequireToken(Parser, CToken(CTokenType_Comma)); */
    /* RequireToken(Parser, CToken(CS("b"))); */
    /* RequireToken(Parser, CToken(CTokenType_Comma)); */
    /* RequireToken(Parser, CToken(CTokenType_Ellipsis)); */
    /* RequireToken(Parser, CToken(CTokenType_CloseParen)); */
    /* RequireToken(Parser, CToken(CS("a"))); */
    /* RequireToken(Parser, CToken(CS("b"))); */
    /* RequireToken(Parser, CToken(CT_Preprocessor__VA_ARGS__)); */


    /* RequireToken(Parser, CT_PreprocessorDefine); */
    /* RequireToken(Parser, CToken(CT_MacroLiteral, CS("MacroFunction6"))); */
    /* RequireToken(Parser, CToken(CTokenType_OpenParen)); */
    /* RequireToken(Parser, CToken(CTokenType_Ellipsis)); */
    /* RequireToken(Parser, CToken(CTokenType_CloseParen)); */
    /* RequireToken(Parser, CToken(CT_Preprocessor__VA_ARGS__)); */


    /* RequireToken(Parser, CT_PreprocessorDefine); */
    /* RequireToken(Parser, CToken(CT_MacroLiteral, CS("MacroFunction7"))); */
    /* RequireToken(Parser, CToken(CTokenType_OpenParen)); */
    /* RequireToken(Parser, CToken(CS("a"))); */
    /* RequireToken(Parser, CToken(CTokenType_Comma)); */
    /* RequireToken(Parser, CToken(CS("b"))); */
    /* RequireToken(Parser, CToken(CTokenType_CloseParen)); */
    /* RequireToken(Parser, CToken(CS("a"))); */
    /* RequireToken(Parser, CToken(CS("b"))); */


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

    /* RequireToken(Parser, CT_PreprocessorDefine); */
    /* RequireToken(Parser, CT_MacroLiteral); */
    /* RequireToken(Parser, CTokenType_OpenParen); */
    /* RequireToken(Parser, CToken(1u)); */
    /* RequireToken(Parser, CTokenType_Minus); */
    /* RequireToken(Parser, CToken(1u)); */
    /* RequireToken(Parser, CTokenType_CloseParen); */

    RequireToken(Parser, CToken(4u));
    RequireToken(Parser, CToken(2u));

    RequireToken(Parser, CToken(4u));
    RequireToken(Parser, CToken(2u));

    RequireToken(Parser, CToken(CSz("MacroFunction8")));
    RequireToken(Parser, CToken(CSz("MacroFunction8")));
    RequireToken(Parser, CToken(0u));

    TestThat( TokensRemain(Parser) == False );

    /* DumpEntireParser(Parser); */
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
    Ctx.CurrentParser = Parser;
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
    Ctx.CurrentParser = Parser;
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
    Ctx.CurrentParser = Parser;
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

#define MAX_PARSERS_IN_PARSER_BLOCK 8
bonsai_function b32
ParserChainIntegrityPasses(parser *Mem)
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
LinkParserBlocks(parser *Mem, u32 ParserCount)
{
  parser *Prev = 0;
  for (u32 ParserIndex = 0; ParserIndex < ParserCount ; ++ParserIndex)
  {
    parser *At = Mem+ParserIndex;

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
  parser Mem[MAX_PARSERS_IN_PARSER_BLOCK] = {};

  parser *Mem0 = Mem;
  parser *Mem1 = Mem+1;
  parser *Mem2 = Mem+2;
  parser *Mem3 = Mem+3;
  parser *Mem4 = Mem+4;
  parser *Mem5 = Mem+5;
  parser *Mem6 = Mem+6;
  parser *Mem7 = Mem+7;


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
  parse_context Ctx = {
    .Memory = Memory,
  };

  parser *Parser = ParserForFile(&Ctx, CS(TEST_FIXTURES_PATH "/preprocessor/parser_chain.cpp"));
  if (Parser)
  {
    RequireToken(Parser, CToken(CSz("parser_token_0")));
    RequireToken(Parser, CToken(CSz("parser_token_1")));
    RequireToken(Parser, CToken(CSz("parser_token_2")));
    RequireToken(Parser, CToken(CSz("parser_token_3")));
    RequireToken(Parser, CToken(CSz("parser_token_4")));

    Rewind(Parser);

    RequireToken(Parser, CToken(CSz("parser_token_0")));
    RequireToken(Parser, CToken(CSz("parser_token_1")));
    RequireToken(Parser, CToken(CSz("parser_token_2")));
    RequireToken(Parser, CToken(CSz("parser_token_3")));
    RequireToken(Parser, CToken(CSz("parser_token_4")));

    /* DumpEntireParser(Parser); */
  }


  return;
}

s32
main()
{
  TestSuiteBegin("Preprocessor");

/* #define thing(a) a */
/* thing(static int thing(foo)) = 42; */

/*   printf("foo %d\n", foo); */


  memory_arena* Memory = AllocateArena();

  TestDoublyLinkedListSwap();
  TestParserChain(Memory);
  TestBasicTokenizationAndParsing(Memory);
  TestPeekAndPopTokens(Memory);
  TestStructParsing(Memory);
  TestCommentSituation(Memory);
  TestMacrosAndIncludes(Memory);
  TestIncludeGuards(Memory);
  /* TestAst(Memory); */
  TestDefinesAndConditionals(Memory);
  TestLogicalOperators(Memory);


  TestSuiteEnd();
  exit(TestsFailed);
}
