#define BONSAI_NO_DEBUG_MEMORY_ALLOCATOR
#define EXCLUDE_PREPROCESSOR_MAIN 1
#include <metaprogramming/preprocessor.cpp>

#include <test_utils.cpp>

function void
TestBasicTokenizationAndParsing(memory_arena* Memory)
{
  c_parse_result Parser_ = TokenizeFile(CS("tests/fixtures/preprocessor_basic.cpp"), Memory);
  c_parse_result* Parser = &Parser_;

  TestThat(PopToken(Parser) == CToken(CS("function")));
  TestThat(PopToken(Parser) == CToken(CS("void")));
  TestThat(PopToken(Parser) == CToken(CS("FunctionName")));
  TestThat(PopToken(Parser) == CToken(CTokenType_OpenParen));
  TestThat(PopToken(Parser) == CToken(CTokenType_CloseParen));
  TestThat(PopToken(Parser) == CToken(CTokenType_OpenBrace));
  TestThat(PopToken(Parser) == CToken(CS("counted_string")));
  TestThat(PopToken(Parser) == CToken(CS("S")));
  TestThat(PopToken(Parser) == CToken(CTokenType_Equals));
  TestThat(PopToken(Parser) == CToken(CS("CS")));
  TestThat(PopToken(Parser) == CToken(CTokenType_OpenParen));
  TestThat(PopToken(Parser) == CToken(CTokenType_String, CS("thing\\n")));
  TestThat(PopToken(Parser) == CToken(CTokenType_CloseParen));
  TestThat(PopToken(Parser) == CToken(CTokenType_Semicolon));
  TestThat(PopToken(Parser) == CToken(CS("return")));
  TestThat(PopToken(Parser) == CToken(CTokenType_Semicolon));
  TestThat(PopToken(Parser) == CToken(CTokenType_CloseBrace));

  TestThat(PopToken(Parser) == CToken(CS("function")));
  TestThat(PopToken(Parser) == CToken(CS("void")));
  TestThat(PopToken(Parser) == CToken(CS("WriteStructTo")));
  TestThat(PopToken(Parser) == CToken(CTokenType_OpenParen));
  TestThat(PopToken(Parser) == CToken(CS("d_union_decl")));
  TestThat(PopToken(Parser) == CToken(CTokenType_Star));
  TestThat(PopToken(Parser) == CToken(CS("dUnion")));
  TestThat(PopToken(Parser) == CToken(CTokenType_Comma));
  TestThat(PopToken(Parser) == CToken(CS("native_file")));
  TestThat(PopToken(Parser) == CToken(CTokenType_Star));
  TestThat(PopToken(Parser) == CToken(CS("OutFile")));
  TestThat(PopToken(Parser) == CToken(CTokenType_Comma));
  TestThat(PopToken(Parser) == CToken(CS("memory_arena")));
  TestThat(PopToken(Parser) == CToken(CTokenType_Star));
  TestThat(PopToken(Parser) == CToken(CS("Memory")));
  TestThat(PopToken(Parser) == CToken(CTokenType_CloseParen));
  TestThat(PopToken(Parser) == CToken(CTokenType_OpenBrace));
  TestThat(PopToken(Parser) == CToken(CS("counted_string")));
  TestThat(PopToken(Parser) == CToken(CS("UnionName")));
  TestThat(PopToken(Parser) == CToken(CTokenType_Equals));
  TestThat(PopToken(Parser) == CToken(CS("dUnion")));
  TestThat(PopToken(Parser) == CToken(CTokenType_Minus));
  TestThat(PopToken(Parser) == CToken(CTokenType_GT));
  TestThat(PopToken(Parser) == CToken(CS("Name")));
  TestThat(PopToken(Parser) == CToken(CTokenType_Semicolon));
  TestThat(PopToken(Parser) == CToken(CS("counted_string")));
  TestThat(PopToken(Parser) == CToken(CS("Decl")));
  TestThat(PopToken(Parser) == CToken(CTokenType_Equals));
  TestThat(PopToken(Parser) == CToken(CS("FormatCountedString")));
  TestThat(PopToken(Parser) == CToken(CTokenType_OpenParen));
  TestThat(PopToken(Parser) == CToken(CS("Memory")));
  TestThat(PopToken(Parser) == CToken(CTokenType_Comma));
  TestThat(PopToken(Parser) == CToken(CTokenType_String, CS("\\n")));
  TestThat(PopToken(Parser) == CToken(CTokenType_CloseParen));
  TestThat(PopToken(Parser) == CToken(CTokenType_Semicolon));
  TestThat(PopToken(Parser) == CToken(CTokenType_CloseBrace));

  TestThat(PopTokenRaw(Parser) == CToken(CTokenType_Newline));
  TestThat(Remaining(&Parser->Tokens) == 0);

}

function void
TestPeekAndPopTokens(memory_arena* Memory)
{
  c_parse_result Parser_ = TokenizeFile(CS("tests/fixtures/preprocessor_peek_pop.cpp"), Memory);
  c_parse_result* Parser = &Parser_;

  TestThat(PeekToken(Parser, 0) == CToken(CS("function")));
  TestThat(PeekToken(Parser, 1) == CToken(CS("void")));
  TestThat(PeekToken(Parser, 2) == CToken(CS("FunctionName")));
  TestThat(PeekToken(Parser, 3) == CToken(CTokenType_OpenParen));
  TestThat(PeekToken(Parser, 4) == CToken(CTokenType_CloseParen));

  TestThat(PopToken(Parser) == CToken(CS("function")));
  TestThat(PopToken(Parser) == CToken(CS("void")));
  TestThat(PopToken(Parser) == CToken(CS("FunctionName")));
  TestThat(PopToken(Parser) == CToken(CTokenType_OpenParen));
  TestThat(PopToken(Parser) == CToken(CTokenType_CloseParen));

  TestThat(PeekToken(Parser, 0) == CToken(CS("debug_scope_tree")));
  TestThat(PeekToken(Parser, 1) == CToken(CTokenType_Star));
  TestThat(PeekToken(Parser, 2) == CToken(CS("GetReadScopeTree")));
  TestThat(PeekToken(Parser, 3) == CToken(CTokenType_OpenParen));
  TestThat(PeekToken(Parser, 4) == CToken(CS("u32")));
  TestThat(PeekToken(Parser, 5) == CToken(CS("ThreadIndex")));
  TestThat(PeekToken(Parser, 6) == CToken(CTokenType_CloseParen));

}

s32
main()
{
  TestSuiteBegin("Preprocessor");

  memory_arena* Memory = AllocateArena();

  TestBasicTokenizationAndParsing(Memory);

  TestPeekAndPopTokens(Memory);

  TestSuiteEnd();
  exit(TestsFailed);
}


