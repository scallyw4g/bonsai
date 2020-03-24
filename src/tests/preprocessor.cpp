#define EXCLUDE_PREPROCESSOR_MAIN 1
#include <metaprogramming/preprocessor.cpp>
#include <tests/test_utils.cpp>

function void
TestBasicTokenizationAndParsing(memory_arena* Memory)
{
  c_parse_result Parser_ = TokenizeFile(CS(TEST_FIXTURES_PATH "/preprocessor_basic.cpp"), Memory);
  c_parse_result* Parser = &Parser_;

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
    TestThat(T == CToken(CS("function")));
  }
  {
    c_token T = PopToken(Parser);
    TestThat(T == CToken(CS("void")));
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
    TestThat(T == CToken(CTokenType_String, CS("\"thing\\n\"")));
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
    TestThat(T == CToken(CS("int")));
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
    TestThat(T == CToken(CS("3")));
  }
  {
    c_token T = PopToken(Parser);
    TestThat(T.Type == CTokenType_FSlash);
  }
  {
    c_token T = PopToken(Parser);
    TestThat(T == CToken(CS("3")));
  }
  {
    c_token T = PopToken(Parser);
    TestThat(T.Type == CTokenType_Semicolon);
  }
  {
    c_token T = PopToken(Parser);
    TestThat(T == CToken(CS("return")));
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
    TestThat(T == CToken(CS("function")));
  }
  {
    c_token T = PopToken(Parser);
    TestThat(T == CToken(CS("void")));
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
    TestThat(T.Type == CTokenType_Minus);
  }
  {
    c_token T = PopToken(Parser);
    TestThat(T.Type == CTokenType_GT);
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
    TestThat(T == CToken(CTokenType_String, CS("\"\\n\"")));
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

  {
    c_token T = PopTokenRaw(Parser);
    TestThat(T.Type == CTokenType_Newline);
  }

  TestThat(Remaining(&Parser->Tokens) == 0);
}

function void
TestPeekAndPopTokens(memory_arena* Memory)
{
  c_parse_result Parser_ = TokenizeFile(CS(TEST_FIXTURES_PATH "/preprocessor_peek_pop.cpp"), Memory);
  c_parse_result* Parser = &Parser_;

  {
    c_token T = PeekToken(Parser, 0);
    TestThat(T == CToken(CS("function")));
  }

  {
    c_token T = PeekToken(Parser, 1);
    TestThat(T == CToken(CS("void")));
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
    TestThat(T == CToken(CS("function")));
  }
  {
    c_token T = PeekToken(Parser, 0);
    TestThat(T == CToken(CS("void")));
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
    TestThat(T == CToken(CS("void")));
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

  return;
}

function void
TestStructParsing(memory_arena* Memory)
{
  c_parse_result Parser_ = TokenizeFile(CS(TEST_FIXTURES_PATH "/preprocessor_datatypes.cpp"), Memory);
  c_parse_result* Parser = &Parser_;

  program_datatypes Datatypes = {};
  ParseDatatypes(Parser, &Datatypes, Memory);

  return;
}

function void
TestCommentSituation(memory_arena* Memory)
{
  c_parse_result Parser_ = TokenizeFile(CS(TEST_FIXTURES_PATH "/comments.cpp"), Memory);
  c_parse_result* Parser = &Parser_;

  {
    c_token T = PeekToken(Parser);
    TestThat(T == CToken(CSz("function")));
  }

  EatUntil(Parser, CTokenType_Newline);

  {
    c_token T = PeekToken(Parser);
    TestThat(T == CToken(CSz("void")));
  }

  {
    c_token T = PeekToken(Parser, 1);
    TestThat(T.Type == CTokenType_Unknown);
  }

  {
    c_token T = PeekToken(Parser);
    TestThat(T == CToken(CSz("void")));
  }




  {
    c_token T = PopToken(Parser);
    TestThat(T == CToken(CSz("void")));
  }


  {
    c_token T = PopToken(Parser);
    TestThat(T.Type == CTokenType_Unknown);
  }



  return;
}


s32
main()
{
  TestSuiteBegin("Preprocessor");

  memory_arena* Memory = AllocateArena();

  TestBasicTokenizationAndParsing(Memory);

  TestPeekAndPopTokens(Memory);

  TestStructParsing(Memory);

  TestCommentSituation(Memory);

  TestSuiteEnd();
  exit(TestsFailed);
}
