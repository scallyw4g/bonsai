#define PLATFORM_LIBRARY_AND_WINDOW_IMPLEMENTATIONS 1
#define PLATFORM_GL_IMPLEMENTATIONS 1

#define thing2(a) a
#define thing3(a) a thing2(thing3)

static int thing2(a_name) = 1;

#include <bonsai_types.h>

#define InvalidDefaultWhileParsing(P, ErrorMessage) \
    default: { ParseError(P, ErrorMessage, PeekTokenPointer(P)); } break;



#define DEBUG_PRINT (0)
#if DEBUG_PRINT
#include <bonsai_stdlib/headers/debug_print.h>

#else
_Pragma("clang diagnostic push")
_Pragma("clang diagnostic ignored \"-Wunused-macros\"")

#define DebugPrint(...)                                   \
  _Pragma("clang diagnostic push")                        \
  _Pragma("clang diagnostic ignored \"-Wunused-value\"")  \
  (__VA_ARGS__)                                           \
  _Pragma("clang diagnostic pop")

_Pragma("clang diagnostic pop") // unused-macros
#endif



// TODO(Jesse id: 293, tags: big): Constant expression evaluation for #if statements
// TODO(Jesse id: 291, tags: big): Macro functions
// TODO(Jesse id: 292, tags: big): Include graph traversal


/* #define TestMacro(a, b) a b */

/* TestMacro( */
/*     void Thing( TestMacro(int a1, int a2), );, */
/*     void Thing2( TestMacro(int a1, int a2) ); */
/* ) */


bonsai_function c_token * PeekTokenRawPointer(parser *Parser, u32 TokenLookahead = 0);
bonsai_function c_token   PeekTokenRaw(parser *Parser, u32 Lookahead = 0);
bonsai_function c_token * PeekTokenPointer(parser *Parser, u32 TokenLookahead = 0);
bonsai_function c_token   PeekToken(parser *Parser, u32 Lookahead = 0);
bonsai_function c_token   PopTokenRaw(parser *Parser);
bonsai_function c_token * PopTokenRawPointer(parser *Parser);
bonsai_function c_token   PopToken(parser *Parser);
bonsai_function b32       OptionalTokenRaw(parser *Parser, c_token_type Type);
bonsai_function b32       OptionalToken(parser *Parser, c_token T);
bonsai_function b32       OptionalToken(parser *Parser, c_token_type Type);
bonsai_function c_token   RequireToken(parser *Parser, c_token ExpectedToken);
bonsai_function c_token   RequireToken(parser *Parser, c_token_type ExpectedType);
bonsai_function c_token   RequireTokenRaw(parser *Parser, c_token Expected);
bonsai_function c_token   RequireTokenRaw(parser *Parser, c_token_type ExpectedType);

bonsai_function b32       TokensRemain(parser *Parser, u32 TokenLookahead = 0);
bonsai_function b32       RawTokensRemain(parser *Parser, u32 TokenLookahead = 0);
bonsai_function u32       OffsetOfNext(parser *Parser, u32 Offset, c_token_type Close);

bonsai_function b32       TokenIsOperator(c_token_type T);
bonsai_function b32       NextTokenIsOperator(parser *Parser);
bonsai_function void      RequireOperatorToken(parser *Parser);

bonsai_function void      TrimFirstToken(parser* Parser, c_token_type TokenType);
bonsai_function void      TrimLastToken(parser* Parser, c_token_type TokenType);
bonsai_function void      TrimLeadingWhitespace(parser* Parser);

bonsai_function void      EatBetween(parser* Parser, c_token_type Open, c_token_type Close);
bonsai_function b32       EatWhitespace(parser* Parser);
bonsai_function b32       EatSpacesTabsAndEscapedNewlines(parser *Parser);


//
// Preprocessor stuff
//

bonsai_function parser * ResolveInclude(parse_context *Ctx, parser *Parser);
bonsai_function parser * ExpandMacro(parse_context *Ctx, parser *Parser, macro_def *Macro, memory_arena *Memory, b32 ScanArgsForAdditionalMacros = False);
bonsai_function u64      ResolveMacroConstantExpression(parse_context *Ctx, parser *Parser, memory_arena *Memory, u64 PreviousValue, b32 LogicalNotNextValue);

bonsai_function macro_def * GetMacroDef(parse_context *Ctx, counted_string DefineValue) ;
bonsai_function c_token* EatIfBlock(parser *Parser);

bonsai_function void EraseToken(c_token *Token);
bonsai_function void EraseSectionOfParser(parser *Parser, c_token *StartToken, c_token *OnePastLastToken);
bonsai_function void SkipToEndOfCursor(c_token_cursor *At, c_token_cursor *ToSkip);

bonsai_function void DumpLocalTokens(parser *Parser);
bonsai_function void PrintTray(char_cursor *Dest, u32 LineNumber, u32 Columns);


bonsai_function void ParseError(parser* Parser, parse_error_code ErrorCode, counted_string ErrorMessage, c_token* ErrorToken = 0);
bonsai_function void ParseError(parser* Parser, counted_string ErrorMessage, c_token* ErrorToken = 0);

bonsai_function string_from_parser
StartStringFromParser(parser* Parser)
{
  c_token *T = PeekTokenRawPointer(Parser);

  string_from_parser Result = {};

  if (T)
  {
    Result.Parser = Parser;
    Result.StartToken = T;
  }

  return Result;
}

bonsai_function counted_string
FinalizeStringFromParser(string_from_parser* Builder)
{
  counted_string Result = {};

  parser *Parser = Builder->Parser;
  c_token *StartToken = Builder->StartToken;

  if (Parser)
  {
    if (Contains(Parser, StartToken))
    {
      umm Count = 0;
      // NOTE(Jesse): This would be better if it excluded the At token, but
      // unfortunately we wrote the calling code such that the At token is
      // implicitly included, so we have to have this weird check.
      if (Parser->Tokens.At == Parser->Tokens.End)
      {
        Count = (umm)(Parser->Tokens.At[-1].Value.Start - Builder->StartToken->Value.Start);
      }
      else
      {
        Count = (umm)(Parser->Tokens.At->Value.Start - Builder->StartToken->Value.Start);
      }

      Result = CS(Builder->StartToken->Value.Start, Count);
    }
    else
    {
      ParseError(Parser, CSz("shit"));
      RuntimeBreak();
      Warn(CSz("Unable to call FinalizeStringFromParser due to having spanned a parser chain link."));
    }
  }

  return Result;
}

bonsai_function void
DoublyLinkedListSwap(c_token_cursor *P0, c_token_cursor *P1)
{
  Assert(P0 != P1);

  b32 Colocated = P0->Next == P1;
  b32 ColocatedReversed = P1->Next == P0;

  // TODO(Jesse): I'm fairly sure that we don't need this boolean and should
  // be able to just go off the M pointer values .. but I didn't want to sit
  // around and figure it out.  Same goes for ColocatedReversed.  Good news is
  // that this routine is well tested so future-me can fearlessly modify it.
#if 0
  if (Colocated)
  {
    if (P1->Prev != P0)
    {
      Assert(false);

      DumpLocalTokens(P0);
      Debug("\n");
      DumpLocalTokens(P1);
      Debug("\n");
      DumpLocalTokens(P1->Next);
      Debug("\n");
      DumpLocalTokens(P1->Prev);
      Debug("\n");
    }
  }
#endif

  if (ColocatedReversed)
  {
    Assert(P0->Prev == P1);
    Assert(!Colocated);

    Colocated = True;
    c_token_cursor *Temp = P1;
    P1 = P0;
    P0 = Temp;
  }

  c_token_cursor M0 = *P0; // Mnemonic M0 == Memory0
  c_token_cursor M1 = *P1;

  *P0 = M1;
  *P1 = M0;

  P0->Next = M1.Next;
  P0->Prev = Colocated ? P1 : M1.Prev;

  P1->Next = Colocated ? P0 : M0.Next;
  P1->Prev = M0.Prev;

  if (M1.Next)
  {
    M1.Next->Prev = P0;
  }

  if (M0.Prev)
  {
    M0.Prev->Next = P1;
  }

  if (!Colocated)
  {
    if (M1.Prev)
    {
      M1.Prev->Next = P0;
    }
    if (M0.Next)
    {
      M0.Next->Prev = P1;
    }
  }

  return;
}

#if BONSAI_SLOW
bonsai_function void
SanityCheckParserChain(parser *Parser)
{
  // TODO(Jesse): This is _SUPER_ slow once the parser chain gets sufficiently
  // long.  Should we re-think our strat here?
  parser *FirstInChain = Parser;
  while (FirstInChain->Prev) FirstInChain = FirstInChain->Prev;

  parser *Current = FirstInChain;
  while (Current->Next)
  {
    Assert(Current->Next->Prev == Current);
    if (Current->Prev)
    {
      Assert(Current->Prev->Next == Current);
    }
    Current = Current->Next;
  }
}
#else
#define SanityCheckParserChain(...)
#endif

bonsai_function c_token *
PeekTokenRawReversePointer(c_token_cursor *Tokens)
{
  c_token *Result = 0;
  if (Tokens->At > Tokens->Start)
  {
    Result = Tokens->At -1;
  }
  else if (Tokens->Prev)
  {
    Result = PeekTokenRawReversePointer(Tokens->Prev);
  }

  return Result;
}

bonsai_function c_token *
PeekTokenRawReversePointer(parser *Parser)
{
  c_token *Result = PeekTokenRawReversePointer(&Parser->Tokens);
  return Result;
}

bonsai_function b32
TokenShouldModifyLineCount(c_token *T, token_cursor_source Source)
{
  b32 Result = False;

  if ( Source == TokenCursorSource_RootFile ||
       Source == TokenCursorSource_Include )
  {
    Result = T->Type == CTokenType_Newline ||
             T->Type == CTokenType_EscapedNewline;
  }

  return Result;
}

bonsai_function c_token *
RewindParserUntil(parser* Parser, c_token *T)
{
  c_token *Result = 0;
  if (T)
  {
    // TODO(Jesse): We can actually do a bounds check on the Start/End to check
    // if the supplied token lays on this parser and skip it if not.

    while (Parser->Tokens.At >= Parser->Tokens.Start)
    {
      // NOTE(Jesse): This has to be a manual check, not a call to
      // RawTokensRemain() because we only want to check if the At token is valid
      if (Parser->Tokens.At < Parser->Tokens.End)
      {
        if (TokenShouldModifyLineCount(Parser->Tokens.At, Parser->Tokens.Source))
        {
          Parser->LineNumber -= 1;
          // TODO(Jesse): Put this assertion back in.  It fires.
          // Assert(Parser->LineNumber > 0);
        }
        if (Parser->Tokens.At == T)
        {
          Result = T;
          break;
        }
      }

      if (Parser->Tokens.At > Parser->Tokens.Start)
      {
        --Parser->Tokens.At;
      }
      else
      {
        if (Parser->Tokens.Prev)
        {
          DoublyLinkedListSwap(&Parser->Tokens, Parser->Tokens.Prev);
          Parser->Tokens.At = Parser->Tokens.End;
        }
        else
        {
          break;
        }
      }
    }

    Assert(Parser->Tokens.At >= Parser->Tokens.Start);
    Assert(Parser->Tokens.At < Parser->Tokens.End);

    Assert( Parser->Tokens.At == T ||
           ( Parser->Tokens.At == Parser->Tokens.Start && !Parser->Tokens.Prev ) );

    SanityCheckParserChain(Parser);

    if ( Parser->Tokens.At != T )
    {
      Warn("Couldn't locate token during RewindParserUntil.  Token was %S(%S)", ToString(T->Type), T->Value);
    }

  }
  else
  {
    Warn("ptr(0) passed to RewindParserUntil");
  }

  return Result;
}

bonsai_function b32
RewindParserUntil(parser* Parser, c_token_type Type)
{
  b32 Result = False;
  while (Parser->Tokens.At >= Parser->Tokens.Start)
  {
    if (Parser->Tokens.At > Parser->Tokens.Start)
    {
      --Parser->Tokens.At;
      if (TokenShouldModifyLineCount(Parser->Tokens.At, Parser->Tokens.Source))
      {
        Parser->LineNumber -= 1;
        Assert(Parser->LineNumber > 0);
      }
    }
    else
    {
      if (Parser->Tokens.Prev)
      {
        DoublyLinkedListSwap(&Parser->Tokens, Parser->Tokens.Prev);
        Parser->Tokens.At = Parser->Tokens.End;
        Parser->LineNumber = Parser->Tokens.EndLine;
      }
      else
      {
        break;
      }
    }

    // Yes, this is in a different place than the one above.  Yes, that's for a reason.
    if (Parser->Tokens.At < Parser->Tokens.End)
    {
      if (Parser->Tokens.At->Type == Type)
      {
        Result = True;
        break;
      }
    }


  }

  Assert(Parser->Tokens.At >= Parser->Tokens.Start);
  Assert(Parser->Tokens.At <= Parser->Tokens.End);

  Assert( Parser->Tokens.At->Type == Type ||
         ( Parser->Tokens.At == Parser->Tokens.Start && !Parser->Tokens.Prev ) );

  SanityCheckParserChain(Parser);

  return Result;
}


bonsai_function void
Rewind(parser* Parser)
{
  Parser->LineNumber = 1;
  c_token_cursor *FirstInChain = &Parser->Tokens;
  while (FirstInChain->Prev) { FirstInChain = FirstInChain->Prev; }
  Assert(!FirstInChain->Prev);

  {
    c_token_cursor *Current = FirstInChain;
    while (Current)
    {
      Rewind(Current);
      Current = Current->Next;
    }
  }

  if (FirstInChain != &Parser->Tokens)
  {
    DoublyLinkedListSwap(&Parser->Tokens, FirstInChain);
  }

#if BONSAI_SLOW
  parser *Current = FirstInChain;
  while (Current)
  {
    Assert(Current->Tokens.At == Current->Tokens.Start);
    Current = Current->Next;
  }
#endif

  Assert(!Parser->Tokens.Prev);
}

bonsai_function b32
IsNewline(c_token_type Type)
{
  b32 Result = Type == CTokenType_Newline        ||
               Type == CTokenType_EscapedNewline ||
               Type == CTokenType_CarrigeReturn;

  return Result;
}

bonsai_function b32
IsWhitespace(c_token_type Type)
{
  b32 Result = Type == CTokenType_Newline        ||
               Type == CTokenType_EscapedNewline ||
               Type == CTokenType_CarrigeReturn  ||
               Type == CTokenType_Tab            ||
               Type == CTokenType_Space;

  return Result;
}

bonsai_function b32
IsWhitespace(c_token *T)
{
  b32 Result = IsWhitespace(T->Type);
  return Result;
}


bonsai_function b32
IsComment(c_token *T)
{
  b32 Result = (T->Type == CTokenType_CommentSingleLine) || (T->Type == CTokenType_CommentMultiLineStart);
  return Result;
}

bonsai_function umm
Remaining(parser* Parser)
{
  umm Result = 0;

  if (Parser->Valid)
  {
    Result = Remaining(&Parser->Tokens);

    if (!Result && Parser->Tokens.Next)
    {
      Result = TotalElements(Parser->Tokens.Next);
    }
  }

  return Result;
}

bonsai_function void
Advance(c_token_cursor* Tokens, u32 Lookahead = 0)
{
  if (Remaining(Tokens, Lookahead))
  {
    Tokens->At += Lookahead+1;
  }
  else
  {
    Warn("Attempted to advance token stream past its end.");
  }

  return;
}

bonsai_function void
AdvanceParser(parser* Parser)
{
  if (!Parser->Valid)
  {
    Warn("Advancing an invalid Parser!");
  }
  SanityCheckParserChain(Parser);
  Assert(Parser->Tokens.At >= Parser->Tokens.Start);
  Assert(Parser->Tokens.At <= Parser->Tokens.End);
  Assert(Parser->Tokens.Start <= Parser->Tokens.End);

  // NOTE(Jesse): This intentionally only checks the current token stream
  // because if it's at 0 we need to do a DoublyLinkedListSwap
  if (Remaining(&Parser->Tokens))
  {
    c_token *T = Parser->Tokens.At;
    if (TokenShouldModifyLineCount(T, Parser->Tokens.Source))
    {
      ++Parser->LineNumber;
    }

    if (Remaining(&Parser->OutputTokens))
    {
      Push(*T, &Parser->OutputTokens);
    }

    ++Parser->Tokens.At;
  }
  else if (Parser->Tokens.Next)
  {
    // NOTE(Jesse): This cannot be just an `if` block.  If this routine
    // automatically advances the parser to it's `Next` pointer when it hits
    // the end a bunch of code that assumes eating tokens will end on
    // immediately the next token breaks.  Specifically, I'm talking about
    // EatBetween() and friends.  They assume (for their return value) that
    // they'll be starting and ending in the same c_token_cursor block and
    // detecting that changeover is pretty annoying in those functions.
    //
    // Actually, this may not be true any longer.
    //
    // TODO(Jesse, immediate): Verify if the above statement still holds true
    Assert(Parser->Tokens.Next->At == Parser->Tokens.Next->Start);
    Assert(Parser->Tokens.Next != &Parser->Tokens);
    Assert(Parser->Tokens.Next->Next != &Parser->Tokens);
    Assert(Parser->Tokens.Next->Next != Parser->Tokens.Next);

    DoublyLinkedListSwap(&Parser->Tokens, Parser->Tokens.Next);
    Parser->Tokens.At = Parser->Tokens.Start;
    AdvanceParser(Parser);

    if (Parser->Tokens.Source == TokenCursorSource_Include)
    {
      Assert(Parser->Tokens.StartLine);
      Parser->LineNumber = Parser->Tokens.StartLine;
    }

    if (Parser->Tokens.Next)
    {
      Assert(Parser->Tokens.Next != &Parser->Tokens);
      Assert(Parser->Tokens.Next->Next != &Parser->Tokens);
      Assert(Parser->Tokens.Next->Next != Parser->Tokens.Next);
    }
  }
  else
  {
    // Attempted to advance parser past its last chunk
  }

  Assert(Parser->Tokens.At >= Parser->Tokens.Start);
  Assert(Parser->Tokens.At <= Parser->Tokens.End);
  Assert(Parser->Tokens.Start <= Parser->Tokens.End);

  SanityCheckParserChain(Parser);
  return;
}

bonsai_function c_token *
AdvanceTo(parser* Parser, c_token* T)
{
  c_token *Result = PeekTokenRawPointer(Parser);

  while (Result && Result != T)
  {
    AdvanceParser(Parser);
    Result = PeekTokenRawPointer(Parser);
  }

  if (Result != T && T != Parser->Tokens.End)
  {
    Error("Unable to find supplied token pointer on parser chain during AdvanceTo!");
  }
  else
  {
    // NOTE(Jesse): These are actually invalid assertions because we can be
    // peeking to the next parser in the chain.
    /* Assert(Parser->Tokens.At != Parser->Tokens.End); */
    /* Assert(Parser->Tokens.At == T); */
  }

  return Result;
}

bonsai_function void
ParseMacroArgument(parser* Parser, c_token_buffer *Result)
{
  Result->Start = Parser->Tokens.At;

  u32 Depth = 0;
  b32 Done = False;
  // TODO(Jesse, correctness): This should probably actually check for
  // Remaining(&Parser->Tokens) because I don't think valid code would ever
  // span a cursor chain link
  while (!Done && Remaining(Parser))
  {
    c_token T = PeekTokenRaw(Parser);
    if (Depth == 0 && T.Type == CTokenType_Comma)
    {
      Done = True;
    }
    else if (Depth > 0 && T.Type == CTokenType_CloseParen)
    {
      --Depth;
    }
    else if (T.Type == CTokenType_OpenParen)
    {
      ++Depth;
    }

    if (!Done) PopTokenRaw(Parser);
  }

  Result->Count = (umm)(Parser->Tokens.At - Result->Start);

  if (Remaining(Parser)) RequireToken(Parser, CTokenType_Comma);

  return;
}

bonsai_function void
EatUntilExcluding(parser* Parser, c_token_type Close)
{
  while (Remaining(Parser))
  {
    if(PeekTokenRaw(Parser).Type == Close)
    {
      break;
    }
    else
    {
      PopTokenRaw(Parser);
    }
  }
  return;
}

bonsai_function void
EatUntilIncluding(parser* Parser, c_token_type Close)
{
  while (Remaining(Parser))
  {
    if(PopTokenRaw(Parser).Type == Close)
    {
      break;
    }
  }
  return;
}




/*****************************                ********************************/
/*****************************  Error Output  ********************************/
/*****************************                ********************************/


bonsai_function void
DumpSingle(c_token_cursor *Cursor, c_token *At)
{
  u32 TokenCount = (u32)TotalElements(Cursor);

  for ( u32 TokenIndex = 0;
        TokenIndex  < TokenCount;
        ++TokenIndex)
  {
    c_token *T = Cursor->Start+TokenIndex;
    if (T == At)
    {
      DebugChars("%S>%S", TerminalColors.Green, TerminalColors.White);
    }
    PrintToken(T);
  }

  return;
}

bonsai_function void
DumpChain(parser* Parser, u32 LinesToDump = u32_MAX)
{
  c_token_cursor *Cursor = &Parser->Tokens;
  c_token *At = Cursor->At;

  while(Cursor->Prev) Cursor = Cursor->Prev;

  Debug("-------------------- Dumping Cursor -- At Line (%d) ----------------------------", Parser->LineNumber);

  u32 Index = 0;
  while (Cursor)
  {
    DumpSingle(Cursor, At);
    Debug("\n%S^%S Link(%d) Cursor Start/End (%d/%d) %S^%S",
        TerminalColors.Blue, TerminalColors.White,
        Index, Cursor->StartLine, Cursor->EndLine,
        TerminalColors.Blue, TerminalColors.White);
    Cursor = Cursor->Next;
    ++Index;
  }
}

bonsai_function void
DumpEntireParser(parser* Parser, u32 LinesToDump = u32_MAX)
{
  Debug("%S---%S", TerminalColors.Purple, TerminalColors.White);

  u32 StartingLineNumber = Parser->LineNumber;
  b32 WasValid    = Parser->Valid;
  Parser->Valid = True;

  c_token * WasAt = PeekTokenRawPointer(Parser);

  Rewind(Parser);

  char TempBuffer[32];
  counted_string TempString = CSz(TempBuffer);
  char_cursor TempCursor = CharCursor(TempString);

  PrintTray(&TempCursor, Parser->LineNumber, 5);
  DebugChars(CS(&TempCursor));
  TempCursor.At = TempCursor.Start;

  c_token *T = PopTokenRawPointer(Parser);
  while(T && LinesToDump > 0)
  {
    if (T == WasAt)
    {
      DebugChars("%S>%S", TerminalColors.Green, TerminalColors.White);
    }

    PrintToken(T);

    /* if (T == WasAt) */
    /* { */
    /*   DebugChars(TerminalColors.Green); */
    /*   DebugChars("<"); */
    /*   DebugChars(TerminalColors.White); */
    /* } */

    if (TokenShouldModifyLineCount(T, Parser->Tokens.Source))
    {
      --LinesToDump;
      PrintTray(&TempCursor, Parser->LineNumber, 5);
      DebugChars(CS(&TempCursor));
      TempCursor.At = TempCursor.Start;
    }

    T = PopTokenRawPointer(Parser);
  }

  if (WasAt)
  {
    RewindParserUntil(Parser, WasAt);
    if (PeekTokenRawPointer(Parser) != WasAt)
    {
      AdvanceTo(Parser, WasAt);
    }
    Assert(PeekTokenRawPointer(Parser) == WasAt);
  }

  Debug("\n%S---%S", TerminalColors.Purple, TerminalColors.White);
  Parser->Valid = WasValid;

  // TODO(Jesse): Put this assert back in
  /* Assert(Parser->LineNumber = StartingLineNumber); */
}

#if 0
bonsai_function void
TruncateAtNextLineEnd(c_token_cursor *Tokens, u32 Count)
{
  while (Remaining(Tokens))
  {
    if(PopTokenRaw(Tokens).Type == CTokenType_Newline)
    {
      if (Count == 0)
      {
        break;
      }
      --Count;
    }
  }

  Tokens->End = Tokens->At;
  Tokens->Next = 0;
}

bonsai_function void
TruncateAtPreviousLineStart(parser* Parser, u32 Count )
{
  while (Parser->Tokens.At > Parser->Tokens.Start)
  {
    if (Parser->Tokens.At->Type == CTokenType_Newline)
    {
      if (Count == 0)
      {
        ++Parser->Tokens.At;
        break;
      }
      --Count;
    }
    --Parser->Tokens.At;
  }

  Parser->Tokens.Start = Parser->Tokens.At;
  Parser->Prev = 0;
}
#endif




/*****************************                 *******************************/
/*****************************   Parse Error   *******************************/
/*****************************                 *******************************/



// TODO(Jesse): Remove this?  Maybe put it onto the parser?   I'm not crazy
// about that because it' bloats that struct and we create those things like
// crazy.. but I don't really like that it's a global either.
static const u32 Global_ParseErrorBufferSize = 4096;
static char Global_ParseErrorBuffer[Global_ParseErrorBufferSize] = {};

bonsai_function void
Indent(char_cursor *Dest, u32 TabCount, u32 SpaceCount)
{
  for (u32 ColumnIndex = 0;
      ColumnIndex < TabCount;
      ++ColumnIndex)
  {
    CopyToDest(Dest, '\t');
  }

  for (u32 ColumnIndex = 0;
      ColumnIndex < SpaceCount;
      ++ColumnIndex)
  {
    CopyToDest(Dest, ' ');
  }
}

bonsai_function void
OutputIdentifierUnderline(char_cursor *Dest, u32 IdentifierLength)
{
  u32 IdentifierLengthSubOne = IdentifierLength > 0 ? IdentifierLength-1 : IdentifierLength;
  b32 DoPipes = IdentifierLength > 3;
  for (u32 ColumnIndex = 0;
      ColumnIndex < IdentifierLength;
      ++ColumnIndex)
  {
    if (DoPipes)
    {
      if (ColumnIndex == 0)
      {
        CopyToDest(Dest, TerminalColors.Yellow);
        CopyToDest(Dest, '|');
        CopyToDest(Dest, TerminalColors.White);
      }
      else if (ColumnIndex == IdentifierLengthSubOne)
      {
        CopyToDest(Dest, TerminalColors.Yellow);
        CopyToDest(Dest, '|');
        CopyToDest(Dest, TerminalColors.White);
      }
      else
      {
        CopyToDest(Dest, TerminalColors.Yellow);
        CopyToDest(Dest, '~');
        CopyToDest(Dest, TerminalColors.White);
      }
    }
    else
    {
      CopyToDest(Dest, TerminalColors.Yellow);
      CopyToDest(Dest, '^');
      CopyToDest(Dest, TerminalColors.White);
    }
  }
}

bonsai_function void
Highlight(char_cursor *Dest, counted_string Str, counted_string Color)
{
  CopyToDest(Dest, Color);
  CopyToDest(Dest, Str);
  CopyToDest(Dest, TerminalColors.White);
}

bonsai_function void
Highlight(char_cursor *Dest, char C, counted_string Color)
{
  CopyToDest(Dest, Color);
  CopyToDest(Dest, C);
  CopyToDest(Dest, TerminalColors.White);
}

bonsai_function void
PrintTray(char_cursor *Dest, u32 LineNumber, u32 Columns)
{
  if (LineNumber)
  {
    FormatCountedString_(Dest, CSz("%*d |"), Columns, LineNumber);
  }
  else
  {
    FormatCountedString_(Dest, CSz("%*c |"), Columns, ' ');
  }
}

bonsai_function u32
GetLongestLineInCursor(char_cursor *Cursor)
{
  Assert(Cursor->At == Cursor->Start);
  u32 Result = 0;

  u32 CurrentLineLength = 0;
  while (Remaining(Cursor))
  {
    if (Cursor->At[0] == '\n')
    {
      if (CurrentLineLength > Result)
      {
        Result = CurrentLineLength;
      }
      CurrentLineLength = 0;
    }
    else if (Cursor->At[0] == '\x1b')
    {
      while (Remaining(Cursor))
      {
        Advance(Cursor);

        if (Cursor->At[0] == 'm')
        {
          break;
        }
      }
    }
    else
    {
      CurrentLineLength++;
    }

    Advance(Cursor);
  }

  if (CurrentLineLength > Result)
  {
    Result = CurrentLineLength;
  }

  return Result;
}

bonsai_function void
ParseError(parser* Parser, parse_error_code ErrorCode, counted_string ErrorMessage, c_token* ErrorToken)
{
  if (!Parser->Valid) return;

  if (!ErrorToken) ErrorToken = Parser->Tokens.At;

  char_cursor ParseErrorCursor_ = {};
  char_cursor *ParseErrorCursor = &ParseErrorCursor_;
  ParseErrorCursor->Start = Global_ParseErrorBuffer;
  ParseErrorCursor->At = Global_ParseErrorBuffer;
  ParseErrorCursor->End = Global_ParseErrorBuffer+Global_ParseErrorBufferSize;

  u32 LinesOfContext = 5;

  counted_string ParserName = {};

  // TODO(Jesse): If we don't pass an ErrorToken in, does this do a bunch of
  // unnecessary work?  ie. Advance to the end of the chain, then rewind to
  // the token again?
  if (AdvanceTo(Parser, ErrorToken))
  {
  }
  else if (RewindParserUntil(Parser, ErrorToken))
  {
  }
  else
  {
    Error("Couldn't find specified token in parser chain.");
    Parser = 0;
  }


  if (Parser)
  {
    u32 ErrorLineNumber = Parser->LineNumber;
    u32 MaxTrayWidth = 1 + GetColumnsFor(ErrorLineNumber + LinesOfContext);
    ParserName = Parser->Tokens.Filename;

    Assert(PeekTokenRawPointer(Parser) == ErrorToken);

    u32 LinesReversed = 0;
    while (LinesReversed <= LinesOfContext)
    {
      RewindParserUntil(Parser, CTokenType_Newline);
      LinesReversed += 1;
    }
    OptionalTokenRaw(Parser, CTokenType_Newline);

    PrintTray(ParseErrorCursor, Parser->LineNumber, MaxTrayWidth);

    u32 SpaceCount = 0;
    u32 TabCount = 0;

    u32 ErrorIdentifierLength = (u32)ErrorToken->Value.Count;
    u32 ErrorIdentifierLengthSubOne = (u32)(ErrorToken->Value.Count > 0 ? ErrorToken->Value.Count-1 : ErrorToken->Value.Count);


    while (c_token *T = PopTokenRawPointer(Parser))
    {
        PrintToken(T, ParseErrorCursor);

        if (T == ErrorToken)
        {
          break;
        }

        if (T->Type == CTokenType_Newline ||
            T->Type == CTokenType_EscapedNewline)
        {
          TabCount = 0;
          SpaceCount = 0;
          if (TokenShouldModifyLineCount(T, Parser->Tokens.Source))
          {
            PrintTray(ParseErrorCursor, Parser->LineNumber, MaxTrayWidth);
          }
          else
          {
            PrintTray(ParseErrorCursor, 0, MaxTrayWidth);
          }
        }
        else if (T->Type == CTokenType_Tab)
        {
          ++TabCount;
        }
        else if (T->Value.Count)
        {
          SpaceCount += T->Value.Count;
        }
        else
        {
          ++SpaceCount;
        }
    }

    c_token *LastTokenBeforeErrorMessage = 0;
    token_cursor_source LastTokenBeforeErrorMessageSource = TokenCursorSource_Unknown;
    {
      c_token *T = PopTokenRawPointer(Parser);
      while (T)
      {
        LastTokenBeforeErrorMessage = T;
        LastTokenBeforeErrorMessageSource = Parser->Tokens.Source;
        PrintToken(T, ParseErrorCursor);

        if (T->Type == CTokenType_Newline || T->Type == CTokenType_EscapedNewline)
        {
          break;
        }

        T = PopTokenRawPointer(Parser);
      }
    }


    { // Output the error line message

      c_token *NextT = PeekTokenRawPointer(Parser);
      c_token *PrevT = PeekTokenRawReversePointer(Parser);
      if (!NextT && PrevT && PrevT->Type != CTokenType_Newline)
      {
        CopyToDest(ParseErrorCursor, '\n');
      }

      PrintTray(ParseErrorCursor, 0, MaxTrayWidth);
      Indent(ParseErrorCursor, TabCount, SpaceCount);
      OutputIdentifierUnderline(ParseErrorCursor, ErrorIdentifierLength);
      CopyToDest(ParseErrorCursor, '\n');

      PrintTray(ParseErrorCursor, 0, MaxTrayWidth);
      Indent(ParseErrorCursor, TabCount, SpaceCount+ErrorIdentifierLengthSubOne);
      Highlight(ParseErrorCursor, '|', TerminalColors.Yellow);
      CopyToDest(ParseErrorCursor, ' ');
      CopyToDest(ParseErrorCursor, ' ');

      Highlight(ParseErrorCursor, CSz("Parse Error\n"), TerminalColors.Yellow);

      PrintTray(ParseErrorCursor, 0, MaxTrayWidth);
      Indent(ParseErrorCursor, TabCount, SpaceCount+ErrorIdentifierLengthSubOne);
      Highlight(ParseErrorCursor, '|', TerminalColors.Yellow);
      CopyToDest(ParseErrorCursor, '\n');


      PrintTray(ParseErrorCursor, 0, MaxTrayWidth);
      Indent(ParseErrorCursor, TabCount, SpaceCount+ErrorIdentifierLengthSubOne);
      Highlight(ParseErrorCursor, '|', TerminalColors.Yellow);
      CopyToDest(ParseErrorCursor, ' ');
      CopyToDest(ParseErrorCursor, ' ');

      for (u32 ECharIndex = 0;
          ECharIndex < ErrorMessage.Count;
          ++ECharIndex)
      {
        char C = ErrorMessage.Start[ECharIndex];
        CopyToDest(ParseErrorCursor, C);

        if (C == '\n')
        {
          PrintTray(ParseErrorCursor, 0, MaxTrayWidth);
          Indent(ParseErrorCursor, TabCount, SpaceCount+ErrorIdentifierLengthSubOne);
          Highlight(ParseErrorCursor, '|', TerminalColors.Yellow);
          CopyToDest(ParseErrorCursor, ' ');
          CopyToDest(ParseErrorCursor, ' ');
        }
      }

      CopyToDest(ParseErrorCursor, '\n');


      { // Output the final underline
        char_cursor ErrorCursor = CharCursor(ErrorMessage);
        u32 LongestLine = GetLongestLineInCursor(&ErrorCursor);

        PrintTray(ParseErrorCursor, 0, MaxTrayWidth);
        Indent(ParseErrorCursor, TabCount, SpaceCount + ErrorIdentifierLengthSubOne);
        CopyToDest(ParseErrorCursor, TerminalColors.Yellow);
        CopyToDest(ParseErrorCursor, '|');
        for (u32 DashIndex = 0;
            DashIndex < LongestLine + 2;
            ++DashIndex)
        {
          CopyToDest(ParseErrorCursor, '_');
        }
        CopyToDest(ParseErrorCursor, '\n');
        CopyToDest(ParseErrorCursor, TerminalColors.White);
      }
    }









    if (TokenShouldModifyLineCount(LastTokenBeforeErrorMessage, LastTokenBeforeErrorMessageSource))
    {
      PrintTray(ParseErrorCursor, Parser->LineNumber, MaxTrayWidth);
    }
    else
    {
      PrintTray(ParseErrorCursor, 0, MaxTrayWidth);
    }

    u32 LinesToPrint = LinesOfContext;
    while (c_token *T = PopTokenRawPointer(Parser))
    {
      PrintToken(T, ParseErrorCursor);
      if (T->Type == CTokenType_EscapedNewline)
      {
        PrintTray(ParseErrorCursor, 0, MaxTrayWidth);
      }

      if (T->Type == CTokenType_Newline)
      {
        LinesToPrint -= 1;
        if (RawTokensRemain(Parser) == 0 || LinesToPrint == 0)
        {
          break;
        }
        else
        {
          PrintTray(ParseErrorCursor, Parser->LineNumber, MaxTrayWidth);
        }
      }
    }

    char *End = ParseErrorCursor->End;
    ParseErrorCursor->End = ParseErrorCursor->At;
    ParseErrorCursor->At = ParseErrorCursor->Start;
    u32 LongestLine = GetLongestLineInCursor(ParseErrorCursor);
    ParseErrorCursor->End = End;

    for (u32 DashIndex = 0;
        DashIndex < LongestLine;
        ++DashIndex)
    {
      DebugChars(CSz("-"));
    }
    DebugChars(CSz("\n"));

    for (u32 DashIndex = 0;
        DashIndex < LongestLine;
        ++DashIndex)
    {
      CopyToDest(ParseErrorCursor, '-');
    }

    CopyToDest(ParseErrorCursor, '\n');
    PrintTray(ParseErrorCursor, 0, MaxTrayWidth);
    CopyToDest(ParseErrorCursor, FormatCountedString(TranArena, CSz(" %S:%u:0\n"), ParserName, ErrorLineNumber));

    DebugChars(CSz("%S"), CS(ParseErrorCursor));



    RewindParserUntil(Parser, ErrorToken);
    Assert(Parser->Tokens.At == ErrorToken);
    Assert(Parser->LineNumber == ErrorLineNumber);

    Parser->ErrorMessage = ErrorMessage;
    Parser->ErrorCode = ErrorCode;
    Parser->Valid = False;
  }
  else
  {
    FormatCountedString_(ParseErrorCursor, CSz("Error determining where the error occurred\n"));
    FormatCountedString_(ParseErrorCursor, CSz("Error messsage was : %S\n"), ErrorMessage);
  }


  RuntimeBreak();

  return;
}

// TODO(Jesse): This should go away once we port all messaged to the new architecture
bonsai_function void
ParseError(parser* Parser, counted_string ErrorMessage, c_token* ErrorToken)
{
  ParseError(Parser, ParseErrorCode_Unknown, ErrorMessage, ErrorToken);
}


bonsai_function counted_string
ParseError_StreamEndedUnexpectedly(parser *Parser)
{
  counted_string Result = FormatCountedString(TranArena, CSz("Stream ended unexpectedly in file : %S"), Parser->Filename);
  ParseError(Parser, ParseErrorCode_StreamEndedUnexpectedly, Result);
  return Result;
}

#define ParseErrorTokenHelper(T) \
  (T) ? ToString((T)->Type) : Unknown, (T) ? (T)->Value : Unknown

bonsai_function counted_string
ParseError_ExpectedSemicolonOrEquals(parser *Parser, c_token *T)
{
  counted_string Unknown = CSz("unknown");
  counted_string Result =
    FormatCountedString_(TranArena, CSz("Got %S(%S)\n\nExpected %S(%c) or %S(%c) while parsing variable declaration."),
        ParseErrorTokenHelper(T),
        ToString(CTokenType_Semicolon), CTokenType_Semicolon,
        ToString(CTokenType_Equals), CTokenType_Equals );

  ParseError(Parser, ParseErrorCode_ExpectedSemicolonOrEquals, Result, PeekTokenPointer(Parser) );
  return Result;
}

bonsai_function counted_string
ParseError_RequireTokenFailed(parser *Parser, c_token *Got, c_token *Expected)
{
  counted_string Unknown = CSz("unknown");
  counted_string Result = FormatCountedString(TranArena,
                                                    CSz("Require Token Failed \n\nGot      %S(%S)\nExpected %S(%S)"),
                                                    ParseErrorTokenHelper(Got),
                                                    ParseErrorTokenHelper(Expected));

  ParseError(Parser, ParseErrorCode_RequireTokenFailed, Result, Got);
  return Result;
}

/*****************************                 *******************************/
/*****************************  Token Control  *******************************/
/*****************************                 *******************************/



#define DEBUG_CHECK_FOR_BREAK_HERE(Result) \
  ( (Result).Type == CTokenType_Identifier && StringsMatch(CSz("break_here"), (Result).Value) )

bonsai_function c_token*
PeekTokenRawPointer(c_token_cursor *Tokens, u32 Lookahead)
{
  Assert(Tokens->At >= Tokens->Start);
  Assert(Tokens->At <= Tokens->End);
  Assert(Tokens->Start <= Tokens->End);

  c_token* Result = {};

  u32 TokensRemaining = (u32)Remaining(Tokens);
  if (TokensRemaining > Lookahead)
  {
    Result = Tokens->At+Lookahead;
  }
  else
  {
    if (Tokens->Next)
    {
      Rewind(Tokens->Next);
      Assert( Tokens->Next->At == Tokens->Next->Start);
      Result = PeekTokenRawPointer(Tokens->Next, Lookahead - TokensRemaining);
    }
  }

#if BONSAI_INTERNAL
  if (Result && DEBUG_CHECK_FOR_BREAK_HERE(*Result))
  {
    RuntimeBreak();
    Result = PeekTokenRawPointer(Tokens, Lookahead + 1);
  }
#endif

#if BONSAI_INTERNAL
  if (Result && Result->Type == CTokenType_Identifier) { Assert(Result->Value.Start); Assert(Result->Value.Count);  }
  if (Result) { Assert(Result->Type); }
#endif

  Assert(Tokens->At >= Tokens->Start);
  Assert(Tokens->At <= Tokens->End);
  Assert(Tokens->Start <= Tokens->End);

  return Result;
}

bonsai_function c_token*
PeekTokenRawPointer(parser* Parser, u32 Lookahead)
{
  c_token *T = 0;
  if (Parser->Valid) { T = PeekTokenRawPointer(&Parser->Tokens, Lookahead); }
  return T;
}

bonsai_function c_token
PeekTokenRaw(parser* Parser, u32 Lookahead)
{
  c_token* Pointer = PeekTokenRawPointer(Parser, Lookahead);
  c_token Result = {};
  if (Pointer) Result = *Pointer;
  return Result;
}

bonsai_function u32
OffsetOfNext(parser* Parser, u32 Offset, c_token_type Close)
{
  c_token* Next = PeekTokenRawPointer(Parser, Offset);

  while (Next && Next->Type != Close)
  {
    ++Offset;
    Next = PeekTokenRawPointer(Parser, Offset);
  }

  return Offset;
}

bonsai_function c_token*
PeekTokenPointer(parser* Parser, u32 Lookahead)
{
  u32 TokenHits = 0;
  u32 LocalLookahead = 0;

  c_token* Result = PeekTokenRawPointer(Parser, LocalLookahead);
  while (Result)
  {
    if ( Result->Erased )
    {
    }
    else if ( Result->Type == CTokenType_CommentSingleLine)
    {
      /* TODO(Jesse, id: 213, tags: bug, parsing, needs_tests): There is a degenerate case here, what if the file ends without a newline?
       * While we're at it, add tests that make sure these functions return sane stuff when files end with comments!
       */
      LocalLookahead = OffsetOfNext(Parser, LocalLookahead, CTokenType_Newline);
    }
    else if ( Result->Type == CTokenType_CommentMultiLineStart)
    {
      /* TODO(Jesse, id: 214, tags: bug, parsing, needs_tests): There is a degenerate case here, what if the file ends with a malformed comment?
       * While we're at it, add tests that make sure these functions return sane stuff when files end with comments!
       */
      LocalLookahead = OffsetOfNext(Parser, LocalLookahead, CTokenType_CommentMultiLineEnd);
    }
    else if (IsWhitespace(Result))
    {
    }
    else if (Result->Type == CT_NameQualifier)
    {
      ++LocalLookahead; // Skip the scope resolution operator token -> '::'
    }
    else
    {
      if (TokenHits++ == Lookahead)
      {
        break;
      }
    }

    ++LocalLookahead;
    Result = PeekTokenRawPointer(Parser, LocalLookahead);
  }

  if (Result && (IsWhitespace(Result) || IsComment(Result))) { Result = 0; } // Fires if the stream ends with whitespace/comment

#if 0
#if BONSAI_INTERNAL
  if (Result && DEBUG_CHECK_FOR_BREAK_HERE(*Result))
  {
    Result = PeekTokenPointer(Parser, Lookahead+1);
  }

  if (Result) { Assert(!StringsMatch(Result->Value, CSz("break_here"))); }
#endif
#endif

  return Result;
}

bonsai_function c_token
PeekToken(parser* Parser, u32 Lookahead)
{
  c_token* Pointer = PeekTokenPointer(Parser, Lookahead);
  c_token Result = {};
  if (Pointer) Result = *Pointer;
  return Result;
}

bonsai_function c_token *
PopTokenRawPointer(parser* Parser)
{
  c_token *Result = PeekTokenRawPointer(Parser);
  if (Result)
  {
    AdvanceTo(Parser, Result);
    AdvanceParser(Parser);
  }

  if (Result && DEBUG_CHECK_FOR_BREAK_HERE(*Result))
  {
    RuntimeBreak();
    AdvanceParser(Parser);
    Result = PopTokenRawPointer(Parser);
  }

#if BONSAI_INTERNAL
  if (Result) { Assert(!StringsMatch(Result->Value, CSz("break_here"))); }
#endif

  return Result;
}

bonsai_function c_token
PopTokenRaw(parser* Parser)
{
  c_token Result = PeekTokenRaw(Parser);
  AdvanceParser(Parser);

#if BONSAI_INTERNAL
  if (DEBUG_CHECK_FOR_BREAK_HERE(Result))
  {
    RuntimeBreak();
    if (Remaining(Parser)) { AdvanceParser(Parser); }
    Result = PopTokenRaw(Parser);
  }

  Assert(!StringsMatch(Result.Value, CSz("break_here")));
#endif

  return Result;
}

bonsai_function b32
RawTokensRemain(parser *Parser, u32 Count)
{
  b32 Result = PeekTokenRawPointer(Parser, Count) != 0;
  return Result;
}

bonsai_function b32
TokensRemain(parser *Parser, u32 Count)
{
  b32 Result = PeekTokenPointer(Parser, Count) != 0;
  return Result;
}

bonsai_function c_token
PopToken(parser* Parser)
{
  c_token Result = {};

  c_token* Peek = PeekTokenPointer(Parser);
  if (Peek)
  {
    AdvanceTo(Parser, Peek);
    AdvanceParser(Parser);
    Result = *Peek;
  }
  else
  {
    AdvanceTo(Parser, Parser->Tokens.End);
  }

#if BONSAI_INTERNAL
  if (DEBUG_CHECK_FOR_BREAK_HERE(Result))
  {
    RuntimeBreak();
    if (Remaining(Parser)) { AdvanceParser(Parser); }
    Result = PopToken(Parser);
  }

  Assert(!StringsMatch(Result.Value, CSz("break_here")));
#endif

  return Result;
}

bonsai_function b32
OptionalTokenRaw(parser* Parser, c_token_type Type)
{
  b32 Result = False;
  c_token Peeked = PeekTokenRaw(Parser);
  if (Peeked.Type == Type)
  {
    Result = True;
    c_token Popped = PopTokenRaw(Parser);
    Assert(Popped.Type == Peeked.Type);
    Assert(Popped.Type == Type);
  }

  return Result;
}

bonsai_function b32
OptionalTokenRaw(parser* Parser, c_token T)
{
  b32 Result = False;
  c_token Peeked = PeekTokenRaw(Parser);
  if (Peeked == T)
  {
    Result = True;
    c_token Popped = PopTokenRaw(Parser);
    Assert(Popped == Peeked);
  }

  return Result;
}

bonsai_function b32
OptionalToken(parser* Parser, c_token T)
{
  b32 Result = (PeekToken(Parser) == T);
  if (Result) { PopToken(Parser); }
  return Result;
}

bonsai_function b32
OptionalToken(parser* Parser, c_token_type Type)
{
  b32 Result = (PeekToken(Parser).Type == Type);
  if (Result) { PopToken(Parser); }
  return Result;
}

bonsai_function c_token
RequireToken(parser* Parser, c_token ExpectedToken)
{
  c_token* PeekedToken = PeekTokenPointer(Parser);

  c_token Result = {};
  if (PeekedToken) Result = *PeekedToken;

  // TODO(Jesse, id: 348, tags: immediate, id_347) : This should go into an AreEqual bonsai_function I think..
  if ( Result.Type != ExpectedToken.Type ||
       (ExpectedToken.Value.Count > 0 && !StringsMatch(ExpectedToken.Value, Result.Value) ))
  {
    if (PeekedToken)
    {
      ParseError_RequireTokenFailed(Parser, PeekedToken, &ExpectedToken);
    }
    else
    {
      ParseError_StreamEndedUnexpectedly(Parser);
    }
  }
  else
  {
    Result = PopToken(Parser);
  }

#if BONSAI_INTERNAL
  Assert(!StringsMatch(Result.Value, CSz("break_here")));
#endif

  return Result;
}

bonsai_function c_token
RequireToken(parser* Parser, c_token_type ExpectedType)
{
  c_token Result = RequireToken(Parser, CToken(ExpectedType));
  return Result;
}

// @duplicated_require_token_raw
bonsai_function c_token
RequireTokenRaw(parser *Parser, c_token Expected )
{
  c_token Result = PeekTokenRaw(Parser);
  if (Result == Expected)  // TODO(Jesse id: 349, tags: id_347) : Change to a bonsai_function call
  {
    PopTokenRaw(Parser);
  }
  else
  {
    ParseError(Parser, ParseErrorCode_RequireTokenFailed, CSz("RequireTokenRaw Failed."));
    Result = {};
  }

  return Result;
}

// @duplicated_require_token_raw
bonsai_function c_token
RequireTokenRaw(parser *Parser, c_token_type Expected )
{
  c_token Result = PeekTokenRaw(Parser);
  if (Result.Type == Expected)  // TODO(Jesse tags:id_347) : Change to a bonsai_function call
  {
    PopTokenRaw(Parser);
  }
  else
  {
    ParseError(Parser, ParseErrorCode_RequireTokenFailed, CSz("RequireTokenRaw Failed."));
    Result = {};
  }

  return Result;
}

bonsai_function b32
TokenIsOperator(c_token_type T)
{
  b32 Result = False;
  switch (T)
  {
    case CTokenType_Ampersand:
    case CTokenType_Tilde:
    case CTokenType_Hat:
    case CTokenType_LT:
    case CTokenType_GT:
    case CTokenType_Bang:
    case CTokenType_LeftShift:
    case CTokenType_RightShift:
    case CTokenType_LessEqual:
    case CTokenType_GreaterEqual:
    case CTokenType_AreEqual:
    case CTokenType_NotEqual:
    case CTokenType_PlusEquals:
    case CTokenType_MinusEquals:
    case CTokenType_TimesEquals:
    case CTokenType_DivEquals:
    case CTokenType_ModEquals:
    case CTokenType_AndEquals:
    case CTokenType_OrEquals:
    case CTokenType_XorEquals:
    case CTokenType_Increment:
    case CTokenType_Decrement:
    case CTokenType_LogicalAnd:
    case CTokenType_LogicalOr:
    case CTokenType_Percent:
    case CTokenType_Pipe:
    case CTokenType_Plus:
    case CTokenType_Minus:
    case CTokenType_FSlash:
    case CTokenType_Question:
    case CTokenType_Star:
    case CTokenType_OpenBracket:
    case CTokenType_Arrow:
    case CTokenType_Dot:
    {
      Result = True;
    } break;

    default: {} break;
  }

  return Result;
}

bonsai_function b32
NextTokenIsOperator(parser* Parser)
{
  c_token T = PeekToken(Parser);
  b32 Result = TokenIsOperator(T.Type);
  return Result;
}

bonsai_function void
RequireOperatorToken(parser* Parser)
{
  c_token T = PeekToken(Parser);
  switch (T.Type)
  {
    case CTokenType_Ampersand:
    case CTokenType_Tilde:
    case CTokenType_Hat:
    case CTokenType_LT:
    case CTokenType_GT:
    case CTokenType_Bang:
    case CTokenType_LeftShift:
    case CTokenType_RightShift:
    case CTokenType_LessEqual:
    case CTokenType_GreaterEqual:
    case CTokenType_AreEqual:
    case CTokenType_NotEqual:
    case CTokenType_PlusEquals:
    case CTokenType_MinusEquals:
    case CTokenType_TimesEquals:
    case CTokenType_DivEquals:
    case CTokenType_ModEquals:
    case CTokenType_AndEquals:
    case CTokenType_OrEquals:
    case CTokenType_XorEquals:
    case CTokenType_Increment:
    case CTokenType_Decrement:
    case CTokenType_LogicalAnd:
    case CTokenType_LogicalOr:
    case CTokenType_Percent:
    case CTokenType_Pipe:
    case CTokenType_Plus:
    case CTokenType_Minus:
    case CTokenType_FSlash:
    case CTokenType_Question:
    case CTokenType_Star:
    case CTokenType_Arrow:
    case CTokenType_Dot:
    {
      RequireToken(Parser, T.Type);
    } break;

    case CTokenType_OpenBracket:
    {
      RequireToken(Parser, T.Type);
      RequireToken(Parser, CTokenType_CloseBracket);
    } break;

    default: { ParseError(Parser, CSz("Expected operator.")); } break;
  }

  return;
}








bonsai_function parser
ParserFromBuffer(c_token_buffer *TokenBuf, umm AtOffset = 0)
{
  Assert(AtOffset < TokenBuf->Count);
  parser Result = {
    .Tokens = {
      .Start = TokenBuf->Start,
      .End = TokenBuf->Start + TokenBuf->Count,
      .At = TokenBuf->Start + AtOffset,
    }
  };

  return Result;
}

bonsai_function void
CopyCursorIntoCursor(c_token_cursor *Src, c_token_cursor *Dest)
{
  u32 SrcElements = (u32)TotalElements(Src);
  u32 DestElements = (u32)Remaining(Dest);
  if ( SrcElements <= DestElements )
  {
    for ( u32 TokenIndex = 0;
          TokenIndex < SrcElements;
          ++TokenIndex)
    {
      Push(Src->Start[TokenIndex], Dest);
    }
  }
  else
  {
    Error("Dest c_token_cursor was too full to hold entire source buffer!" );
  }
}

bonsai_function void
CopyMacroArgIntoCursor(parse_context *Ctx, c_token_buffer *Src, c_token_cursor *Dest, memory_arena *Memory)
{
  parser TempParser = ParserFromBuffer(Src);
  while ( Remaining(&TempParser) )
  {
    c_token ArgT = PeekTokenRaw(&TempParser);
    if (ArgT.Type == CT_MacroLiteral)
    {
      parser *Expanded = ExpandMacro(Ctx, &TempParser, ArgT.Macro, Memory);
      CopyCursorIntoCursor(&Expanded->Tokens, Dest);
    }
    else
    {
      PopTokenRaw(&TempParser);
      Push(ArgT, Dest);
    }
  }
}

bonsai_function void
CopyBufferIntoCursor(c_token_buffer *Src, c_token_cursor *Dest)
{
  if ( Src->Count <= Remaining(Dest) )
  {
    for ( u32 TokenIndex = 0;
          TokenIndex < Src->Count;
          ++TokenIndex)
    {
      Push(Src->Start[TokenIndex], Dest);
    }
  }
  else
  {
    Error("Dest c_token_cursor was too full to hold entire source buffer!" );
  }
}

bonsai_function parser*
ExpandMacro(parse_context *Ctx, parser *Parser, macro_def *Macro, memory_arena *Memory, b32 ScanArgsForAdditionalMacros)
{
  /* if (Macro->IsExpanding) */
  /* { */
  /*   Info("Stopped expanding Macro (%S), it's already undergoing expansion.", Macro->Name); */
  /*   local_persist int Count = 0; */
  /*   if (++Count > 20) */
  /*   { */
  /*     RuntimeBreak(); */
  /*   } */

  /*   return Parser; */
  /* } */

  /* Macro->IsExpanding = True; */

  // @memory
  parser *Result = AllocateParserPtr(Parser->Filename, 0, (u32)Kilobytes(1), TokenCursorSource_MacroExpansion, 0, Memory);

  // TODO(Jesse): Think more carefully about how we want to track where macros
  // came from.
  Result->Tokens.EndLine = 0;

  Assert(Macro->Body.Tokens.At == Macro->Body.Tokens.Start);
  Assert(Macro->Body.Tokens.Next == 0);
  Assert(Macro->Body.Tokens.Prev == 0);

  RequireToken(Parser, CToken(CT_MacroLiteral, Macro->Name));

  switch (Macro->Type)
  {
    case type_macro_keyword:
    {
      parser BodyParser_ = Macro->Body;
      parser *BodyParser = &BodyParser_;
      while (c_token *T = PeekTokenRawPointer(BodyParser))
      {
        if (T->Type == CT_MacroLiteral)
        {
          parser *Expanded = ExpandMacro(Ctx, BodyParser, T->Macro, Memory);
          CopyCursorIntoCursor(&Expanded->Tokens, &Result->Tokens);
        }
        else
        {
          Assert(T->Type != CTokenType_Newline);
          PopTokenRaw(BodyParser);
          Push(*T, &Result->Tokens);
        }

      }

    } break;

    case type_macro_function:
    {
      c_token *Start = PeekTokenPointer(Parser);

      if (Start && Start->Type == CTokenType_OpenParen)
      {
        parser InstanceArgs_ =
        {
          .Valid = 1,
          .Tokens = { .Start = Start, .At = Start, .End = Start, },
        };
        parser *InstanceArgs = &InstanceArgs_;

        EatBetween(Parser, CTokenType_OpenParen, CTokenType_CloseParen);
        InstanceArgs->Tokens.End = Parser->Tokens.At;

        TrimFirstToken(InstanceArgs, CTokenType_OpenParen);
        TrimLastToken(InstanceArgs, CTokenType_CloseParen);

        Assert(Ctx);
#if 1
        if (ScanArgsForAdditionalMacros)
        {
          while (c_token *T = PopTokenRawPointer(InstanceArgs))
          {
            macro_def *M = GetMacroDef(Ctx, T->Value);
            if (M)
            {
              // TODO(Jesse): Can we call MacroShouldBeExpanded() here instead?
              // @call_MacroShouldBeExpanded
              if ( M->Type == type_macro_function &&
                   PeekTokenRaw(InstanceArgs).Type != CTokenType_OpenParen )
              {
                // If an identifier is a macro function but it's not invoked the
                // identifier is left unadultered.
              }
              else
              {
                T->Type = CT_MacroLiteral;
                T->Macro = M;
              }
            }
          }
        }
#endif
        Rewind(InstanceArgs);


        c_token_buffer_buffer ArgValues = {};
        if (Macro->NamedArguments.Count)
        {
          ArgValues = CTokenBufferBuffer(Macro->NamedArguments.Count, Memory);
          for ( u32 ArgIndex = 0;
                ArgIndex < ArgValues.Count;
                ++ArgIndex)
          {
            ParseMacroArgument(InstanceArgs, ArgValues.Start+ArgIndex);
          }
        }

        c_token_buffer_stream VarArgs = {};
        if (Macro->Variadic)
        {
          while (Remaining(InstanceArgs))
          {
            c_token_buffer *Arg = Push(&VarArgs, {}, Memory);
            ParseMacroArgument(InstanceArgs, Arg);
          }
        }

        Assert(Remaining(InstanceArgs) == 0);

        parser MacroBody_ = Macro->Body;
        parser *MacroBody = &MacroBody_;
        TrimLeadingWhitespace(MacroBody);
        Rewind(MacroBody);
        while (Remaining(MacroBody))
        {
          c_token T = PeekTokenRaw(MacroBody);
          switch (T.Type)
          {
            case CT_Preprocessor__VA_ARGS__:
            {
              RequireToken(MacroBody, T);
              ITERATE_OVER(&VarArgs)
              {
                c_token_buffer* Arg = GET_ELEMENT(Iter);
                CopyMacroArgIntoCursor(Ctx, Arg, &Result->Tokens, Memory);
              }
            } break;

            case CTokenType_Identifier:
            {
              RequireToken(MacroBody, T);
              u32 ArgIndex = (u32)IndexOf(&Macro->NamedArguments, T.Value);
              if (ArgIndex < Macro->NamedArguments.Count)
              {
                c_token_buffer *ArgBuffer = ArgValues.Start + ArgIndex;
                CopyMacroArgIntoCursor(Ctx, ArgBuffer, &Result->Tokens, Memory);
              }
              else
              {
                Push(T, &Result->Tokens);
              }
            } break;

            case CT_MacroLiteral:
            {
              parser *Expanded = ExpandMacro(Ctx, MacroBody, T.Macro, Memory);
              CopyCursorIntoCursor(&Expanded->Tokens, &Result->Tokens);
            } break;

            default:
            {
              PopTokenRaw(MacroBody);
              Push(T, &Result->Tokens);
            } break;
          }

        }
        Assert(Remaining(MacroBody) == 0);
      }

    } break;

    InvalidDefaultWhileParsing(Parser, CSz("Error expanding macro!"));
  }

  TruncateToCurrentSize(&Result->Tokens);
  Rewind(Result);

  return Result;
}



/*************************                         ***************************/
/*************************  Comments & Whitespace  ***************************/
/*************************                         ***************************/


bonsai_function b32
EatComment(parser* Parser, c_token_type CommentT)
{
  b32 Result = False;
  switch (CommentT)
  {
    case CTokenType_CommentSingleLine:
    {
      Result = True;
      EatUntilExcluding(Parser, CTokenType_Newline);
    } break;

    case CTokenType_CommentMultiLineStart:
    {
      Result = True;
      EatUntilIncluding(Parser, CTokenType_CommentMultiLineEnd);
    } break;

    default:
    {
    } break;
  }

  return Result;
}

bonsai_function b32
EatComment(parser* Parser)
{
  b32 Result = EatComment(Parser, PeekTokenRaw(Parser).Type);
  return Result;
}

bonsai_function c_token_cursor *
SeekToEndOfCursorChain(c_token_cursor *Cursor)
{
  c_token_cursor *Result = Cursor;
  while (Result->Next) Result = Result->Next;
  return Result;
}

bonsai_function void
EatWhitespaceAndComments(parser* Parser)
{
  while ( c_token *T = PeekTokenRawPointer(Parser) )
  {
    if (IsWhitespace(T))
    {
      PopTokenRawPointer(Parser);
    }
    else if (IsComment(T))
    {
      EatComment(Parser);
    }
    else
    {
      break;
    }
  }
}

// NOTE(Jesse): This is duplicated @duplicate_EatSpacesTabsAndEscapedNewlines
bonsai_function b32
EatSpacesTabsAndEscapedNewlines(parser *Parser)
{
  b32 Result = False;

  c_token_type Type = PeekTokenRaw(Parser).Type;
  while ( RawTokensRemain(Parser) &&
          ( Type == CTokenType_Space            ||
            Type == CTokenType_Tab              ||
            Type == CTokenType_EscapedNewline   ||
            Type == CTokenType_FSlash ) )
  {
    Result = True;
    if ( Type == CTokenType_FSlash )
    {
      // NOTE(Jesse): This is garbage and should actually never happen.  An excaped newline is '\n' .. which is a backslash!!!!
      Assert(False);
#if 0
      if ( PeekTokenRaw(Parser, 1).Type == CTokenType_Newline )
      {
        RequireTokenRaw(Parser, CTokenType_FSlash);
        RequireTokenRaw(Parser, CTokenType_Newline);
      }
      else
      {
        break;
      }
#endif
    }
    else
    {
      RequireTokenRaw(Parser, Type);
    }

    Type = PeekTokenRaw(Parser).Type;
  }

  return Result;
}

bonsai_function b32
EatWhitespace(parser* Parser)
{
  b32 Result = False;

  c_token *T = PeekTokenRawPointer(Parser);
  while (T && IsWhitespace(T))
  {
    Result = True;
    PopTokenRawPointer(Parser);
    T = PeekTokenRawPointer(Parser);
  }

  return Result;
}

bonsai_function void
TrimLeadingWhitespace(parser* Parser)
{
  c_token *T = PeekTokenRawPointer(Parser);
  while (T && IsWhitespace(T))
  {
    PopTokenRawPointer(Parser);
    T = PeekTokenRawPointer(Parser);
  }
}

bonsai_function void
TrimTrailingWhitespace(parser* Parser)
{
  c_token* CurrentToken = Parser->Tokens.End-1;

  while (CurrentToken > Parser->Tokens.Start)
  {
    if ( CurrentToken->Type == CTokenType_Space ||
         CurrentToken->Type == CTokenType_Tab )
    {
      Parser->Tokens.End = CurrentToken;
      if (Parser->Tokens.At > CurrentToken)
      {
        Parser->Tokens.At = CurrentToken;
      }
    }
    else
    {
      break;
    }

    --CurrentToken;
  }
}






bonsai_function counted_string
PopHex(ansi_stream* SourceFileStream)
{
  counted_string Result = {
    .Start = SourceFileStream->At
  };

  while (Remaining(SourceFileStream))
  {
    if (IsHex(*SourceFileStream->At))
    {
      ++SourceFileStream->At;
    }
    else
    {
      break;
    }
  }

  Result.Count = (umm)SourceFileStream->At - (umm)Result.Start;
  return Result;
}

bonsai_function counted_string
PopNumeric(ansi_stream* SourceFileStream)
{
  counted_string Result = {
    .Start = SourceFileStream->At
  };

  while (Remaining(SourceFileStream))
  {
    if (IsNumeric(*SourceFileStream->At))
    {
      ++SourceFileStream->At;
    }
    else
    {
      break;
    }
  }

  Result.Count = (umm)SourceFileStream->At - (umm)Result.Start;
  return Result;
}

bonsai_function counted_string
PopIdentifier(ansi_stream* SourceFileStream)
{
  counted_string Result = {
    .Start = SourceFileStream->At
  };

  while (Remaining(SourceFileStream))
  {
    c_token T = PeekToken(SourceFileStream);
    if (T.Type == CTokenType_Unknown)
    {
      ++SourceFileStream->At;
    }
    else
    {
      break;
    }
  }

  Result.Count = (umm)SourceFileStream->At - (umm)Result.Start;
  return Result;
}

bonsai_function r64
ToFractional(counted_string S)
{
  r64 Result = 0;
  if (S.Count) { Result = (r64)ToU64(S) * Exp(10.0, -SafeTruncateToS32(S.Count)); }
  Assert(Result < 1.0f);
  return Result;
}

bonsai_function u64
BinaryStringToU64(counted_string Bin)
{
  u64 Result = 0;
  for (u64 CharIndex = 0;
      CharIndex < Bin.Count;
      ++CharIndex)
  {
    u64 Digit = (u64)(Bin.Start[CharIndex] - '0');
    Assert(Digit < 2);
    Result |= Digit << ((Bin.Count - CharIndex - 1L));
  }

  return Result;
}

bonsai_function u64
HexToU64(char C)
{
  u64 Result = 0;

  if (C >= '0' && C <= '9')
  {
    Result = ToU64(C);
  }
  else if (C >= 'a' && C <= 'f')
  {
    Result = (u64)(10 + C - 'a');
  }
  else if (C >= 'A' && C <= 'F')
  {
    Result = (u64)(10 + C - 'A');
  }
  else
  {
    InvalidCodePath();
  }

  Assert(Result < 16);
  return Result;
}

bonsai_function u64
HexStringToU64(counted_string S)
{
  u64 Result = 0;
  for (u64 CharIndex = 0;
      CharIndex < S.Count;
      ++CharIndex)
  {
    u64 Digit = HexToU64(S.Start[CharIndex]);
    Result |= Digit << ((S.Count - CharIndex - 1L) * 4);
  }

  return Result;
}

bonsai_function c_token
ParseExponentAndSuffixes(ansi_stream *Code, r64 OriginalValue)
{
  c_token Result = {};

  r64 FinalValue = OriginalValue;

  if ( Remaining(Code) &&
       (*Code->At == 'e' || *Code->At == 'E') )
  {
    Advance(Code);

    s32 Exponent = 1;

    char ExpSign = *Code->At;
    if (ExpSign == '-')
    {
      Advance(Code);
      Exponent = -1;
    }
    else if (ExpSign == '+')
    {
      Advance(Code);
      Exponent = 1;
    }

    Exponent = Exponent * SafeTruncateToS32(ToU64(PopNumeric(Code)));

    FinalValue = Exp(FinalValue, Exponent);
  }

  if (Remaining(Code))
  {
    char Suffix = *Code->At;
    switch (Suffix)
    {
      case 'f':
      case 'F':
      {
        Result.Type = CTokenType_FloatLiteral;
        Advance(Code);
      } break;

      case 'l':
      case 'L':
      {
        // Apparently `double` and `long double` are the same storage size (8 bytes), at least in MSVC:
        // https://docs.microsoft.com/en-us/cpp/c-language/storage-of-basic-types?view=vs-2019
        Result.Type = CTokenType_DoubleLiteral;
        Advance(Code);
      } break;

      default:
      {
        Result.Type = CTokenType_DoubleLiteral;
      } break;
    }
  }

  Result.FloatValue = FinalValue;
  return Result;
}

bonsai_function void
ParseIntegerSuffixes(ansi_stream *Code)
{
  b32 Done = False;
  while (!Done)
  {
    char Suffix = *Code->At;
    switch (Suffix)
    {
      // For now, we just eat suffixes
      // TODO(Jesse id: 278): Disallow invalid suffixes lul/LUL .. LUU .. ULLLL etc..
      // Maybe use a state machine / transition table
      case 'u':
      case 'U':
      case 'l':
      case 'L':
      {
        Advance(Code);
      } break;

      default:
      {
        Done = True;
      } break;
    }
  }
}

bonsai_function c_token
ParseNumericToken(ansi_stream *Code)
{
  const char *Start = Code->At;

  counted_string IntegralString = { .Start = Code->At };
  while (Remaining(Code) && IsNumeric(*Code->At)) { Advance(Code); }
  IntegralString.Count = (umm)(Code->At - IntegralString.Start);

  u64 IntegralPortion = ToU64(IntegralString);

  c_token Result = {
    .Type = CTokenType_IntLiteral,
    .UnsignedValue = IntegralPortion,
  };

  if ( IntegralPortion == 0 &&
       (*Code->At == 'x'|| *Code->At == 'X') )
  {
    Advance(Code);
    Result.UnsignedValue = HexStringToU64(PopHex(Code));
    ParseIntegerSuffixes(Code);
  }
  else if (IntegralPortion == 0 && *Code->At == 'b')
  {
    Advance(Code);
    Result.UnsignedValue = BinaryStringToU64(PopNumeric(Code));
  }
  else if ( *Code->At == '.' )
  {
    // Float literal
    Advance(Code);
    r64 Fractional = ToFractional(PopNumeric(Code));
    Result = ParseExponentAndSuffixes(Code, (r64)IntegralPortion + Fractional);
  }
  else if ( *Code->At == 'e' || *Code->At == 'E'  )
  {
    // Float literal
    Result = ParseExponentAndSuffixes(Code, (r64)IntegralPortion);
  }
  else
  {
    //
    // Int literal
    //

    ParseIntegerSuffixes(Code);
  }

  Result.Value.Start = Start;
  Result.Value.Count = (umm)(Code->At - Start);
  return Result;
}

// @duplicate_EatSpacesTabsAndEscapedNewlines
bonsai_function u32
EatSpacesTabsAndEscapedNewlines(ansi_stream *Code)
{
  u32 LinesEaten = 0;
  c_token_type Type = PeekToken(Code).Type;
  while ( Type == CTokenType_Space ||
          Type == CTokenType_Tab   ||
          Type == CTokenType_FSlash )
  {
    if ( Type == CTokenType_FSlash )
    {
      if ( PeekToken(Code, 1).Type == CTokenType_Newline )
      {
        ++LinesEaten;
        Advance(Code);
        Advance(Code);
      }
      else
      {
        break;
      }
    }
    else
    {
      Advance(Code);
    }

    Type = PeekToken(Code).Type;
  }
  return LinesEaten;
}

bonsai_function macro_def*
GetByName(macro_def_stream* Stream, counted_string Name)
{
  macro_def* Result = {};
  ITERATE_OVER(Stream)
  {
    macro_def* Current = GET_ELEMENT(Iter);
    if (!Current->Undefed && StringsMatch(Current->Name, Name))
    {
      Result = Current;
      break;
    }
  }

  return Result;
}

bonsai_function u32
CountTokensBeforeNext(parser *Parser, c_token_type T1, c_token_type T2)
{
  Assert(T1 != T2);
  u32 Result = 0;

  c_token *ResetToStart = Parser->Tokens.At;

  for (;;)
  {
    c_token AtT = PopTokenRaw(Parser);
    if (AtT.Type == T1)
    {
      ++Result;
    }

    if (AtT.Type == T2)
    {
      break;
    }
  }

  Parser->Tokens.At = ResetToStart;

  return Result;
}








bonsai_function c_token_cursor *
SplitAndInsertTokenCursor(c_token_cursor *CursorToSplit, c_token* SplitStart, u32 LineNumber, c_token_cursor *CursorToInsert, c_token* SplitEnd, memory_arena *Memory)
{
  c_token_cursor *Result = SeekToEndOfCursorChain(CursorToInsert);

  SanityCheckParserChain(CursorToSplit);
  SanityCheckParserChain(CursorToInsert);

  Assert(SplitStart <= SplitEnd);
  Assert(SplitStart >= CursorToSplit->Start);
  Assert(SplitStart <= CursorToSplit->End);
  Assert(SplitEnd >= CursorToSplit->Start);
  Assert(SplitEnd <= CursorToSplit->End);


  s64 SecondHalfLength = CursorToSplit->End - SplitEnd;
  Assert(SecondHalfLength >= 0);

  c_token_cursor *SecondHalfOfSplit = 0;
  if (SecondHalfLength)
  {
    SecondHalfOfSplit = AllocateProtection(c_token_cursor, Memory, 1, False);
    SecondHalfOfSplit->Source = CursorToSplit->Source;
    SecondHalfOfSplit->Filename = CursorToSplit->Filename;
    SecondHalfOfSplit->StartLine = LineNumber;
    SecondHalfOfSplit->EndLine = 0;

    SecondHalfOfSplit->Start = SplitEnd;
    SecondHalfOfSplit->At = SplitEnd;
    SecondHalfOfSplit->End = CursorToSplit->End;
  }

  CursorToSplit->At = SplitStart;
  CursorToSplit->End = SplitStart;
  CursorToSplit->EndLine = LineNumber;

  Assert(CursorToSplit->At == CursorToSplit->End);
  Assert(CursorToSplit->At == SplitStart);
  Assert(CursorToSplit->End == SplitStart);


  Assert(CursorToInsert->Prev == 0);
  Rewind(CursorToInsert);
  Assert(CursorToInsert->Prev == 0);

  c_token_cursor *ParserToInsertLastNext = CursorToInsert;
  while (ParserToInsertLastNext->Next) { ParserToInsertLastNext = ParserToInsertLastNext->Next; }
  ParserToInsertLastNext->Next = SecondHalfOfSplit;

  if (SecondHalfOfSplit)
  {
    SecondHalfOfSplit->Next = CursorToSplit->Next;

    if (SecondHalfOfSplit->Next) {SecondHalfOfSplit->Next->Prev = SecondHalfOfSplit;}

    SecondHalfOfSplit->Prev = ParserToInsertLastNext;
    Assert(SecondHalfOfSplit->At == SecondHalfOfSplit->Start);
  }

  CursorToInsert->Prev = CursorToSplit;
  CursorToSplit->Next = CursorToInsert; // Don't move me

#if BONSAI_INTERNAL
  { // Sanity checks
    Assert(CursorToInsert->At == CursorToInsert->Start);

    if (CursorToSplit->Next) { Assert(CursorToSplit->Next->Prev == CursorToSplit); }
    if (CursorToSplit->Prev) { Assert(CursorToSplit->Prev->Next == CursorToSplit); }

    if (CursorToInsert->Next) { Assert(CursorToInsert->Next->Prev == CursorToInsert); }
    if (CursorToInsert->Prev) { Assert(CursorToInsert->Prev->Next == CursorToInsert); }

    if (SecondHalfOfSplit && SecondHalfOfSplit->Next) { Assert(SecondHalfOfSplit->Next->Prev == SecondHalfOfSplit); }
    if (SecondHalfOfSplit && SecondHalfOfSplit->Prev) { Assert(SecondHalfOfSplit->Prev->Next == SecondHalfOfSplit); }

    Assert(CursorToSplit->Next == CursorToInsert);
    Assert(CursorToInsert->Prev == CursorToSplit);
    Assert(ParserToInsertLastNext->Next == SecondHalfOfSplit);

    if (SecondHalfOfSplit)
    {
      Assert(SecondHalfOfSplit->Prev == ParserToInsertLastNext);
    }
  }
#endif

  SanityCheckParserChain(CursorToSplit);
  SanityCheckParserChain(CursorToInsert);
  if (SecondHalfOfSplit)
  {
    SanityCheckParserChain(SecondHalfOfSplit);
  }

  Assert(CursorToSplit->StartLine);
  Assert(CursorToSplit->EndLine == LineNumber);

  // TODO(Jesse): Put these back in once we decide on what macro insertion
  // cursors actually track
  /* Assert(CursorToInsert->StartLine); */
  /* Assert(CursorToInsert->EndLine); */

  if (SecondHalfOfSplit)
  {
    Assert(SecondHalfOfSplit->StartLine == LineNumber);
    Assert(SecondHalfOfSplit->EndLine == 0);
  }

  return Result;
}

bonsai_function c_token_cursor *
SplitAndInsertTokenCursor(c_token_cursor *CursorToSplit, u32 LineNumber, c_token_cursor *CursorToInsert, memory_arena *Memory)
{
  return SplitAndInsertTokenCursor(CursorToSplit, CursorToSplit->At, LineNumber, CursorToInsert, CursorToSplit->At, Memory);
}

bonsai_function parser *
DuplicateParserTokens(parser *Parser, memory_arena *Memory)
{
  Assert(Parser->Tokens.At == Parser->Tokens.Start);

  umm TokenCount = (umm)(Parser->Tokens.End - Parser->Tokens.Start);
  c_token *NewBuffer = Allocate(c_token, Memory, TokenCount);

  c_token *At = NewBuffer;
  while (c_token *T = PopTokenRawPointer(Parser))
  {
    Assert(!T->Erased);
    *At++ = *T;
  }

  Assert(At == (NewBuffer+TokenCount));

  Parser->Tokens.Start = NewBuffer;
  Parser->Tokens.At = NewBuffer;
  Parser->Tokens.End = NewBuffer+TokenCount;

  return Parser;
}

bonsai_function void
ParseMacro(parse_context *Ctx, macro_def *Macro, memory_arena *Memory)
{
  Assert(Ctx);

  parser *MacroBody = DuplicateParserTokens(&Macro->Body, Memory);

  RequireToken(MacroBody, CToken(CT_MacroLiteral, Macro->Name));

  if (OptionalTokenRaw(MacroBody, CTokenType_OpenParen))
  {
    Macro->Type = type_macro_function;

    if (OptionalToken(MacroBody, CTokenType_CloseParen))
    {
      // No arguments
    }
    else
    {
      u32 ArgCount = 1 + CountTokensBeforeNext(MacroBody, CTokenType_Comma, CTokenType_CloseParen);
      Macro->NamedArguments = CountedStringBuffer(ArgCount, Memory);

      for ( u32 ArgIndex = 0;
            ArgIndex < ArgCount-1;
            ++ArgIndex)
      {
        counted_string ArgName = RequireToken(MacroBody, CTokenType_Identifier).Value;
        Macro->NamedArguments.Start[ArgIndex] = ArgName;
        RequireToken(MacroBody, CTokenType_Comma);
      }

      if (PeekToken(MacroBody).Type == CTokenType_Identifier)
      {
        counted_string ArgName = RequireToken(MacroBody, CTokenType_Identifier).Value;
        Macro->NamedArguments.Start[Macro->NamedArguments.Count-1] = ArgName;
      }

      if (OptionalToken(MacroBody, CTokenType_Ellipsis))
      {
        Macro->Variadic = True;
        Macro->NamedArguments.Count -= 1;
      }

      RequireToken(MacroBody, CTokenType_CloseParen);
    }
  }
  else
  {
    Macro->Type = type_macro_keyword;
  }
  MacroBody->Tokens.Start = MacroBody->Tokens.At;

  while (c_token *T = PeekTokenRawPointer(MacroBody))
  {
    switch (T->Type)
    {

      case CTokenType_Identifier:
      {
        RequireToken(MacroBody, T->Type);
        macro_def *M = GetMacroDef(Ctx, T->Value);
        if (M)
        {
          // TODO(Jesse): Can we call MacroShouldBeExpanded() here instead?
          // @call_MacroShouldBeExpanded
          b32 ThisMacroMatchesExpandingMacro = StringsMatch(M->Name, Macro->Name);
          b32 ShouldExpandMacro = Macro->Type == type_macro_function || !ThisMacroMatchesExpandingMacro;
          if (ShouldExpandMacro)
          {
            T->Type = CT_MacroLiteral;
            T->Macro = M;
          }
        }
      } break;

      case CTokenType_Newline: { InvalidCodePath(); } break;
      default: { PopTokenRaw(MacroBody); } break;
    }
  }

  TruncateToCurrentSize(&MacroBody->Tokens);
  Assert(MacroBody->Tokens.End);

  Rewind(&Macro->Body);
}

bonsai_function void
DefineMacro(parse_context *Ctx, parser *Parser, macro_def *Macro)
{
  RequireToken(Parser, CT_PreprocessorDefine);

  if (Macro->Body.Tokens.Start)
  {
    // The memory leak is in ParseMacro() when we duplicate the body tokens
    Warn("Leaking memory re-defining Macro (%S)", Macro->Name);
  }

  Macro->Body.Tokens.Start = Parser->Tokens.At;
  Macro->Body.Tokens.At = Parser->Tokens.At;

  EatUntilExcluding(Parser, CTokenType_Newline);
  Macro->Body.Tokens.End = Parser->Tokens.At;

  Macro->Body.Filename = Parser->Filename;
  Macro->Body.LineNumber = Parser->LineNumber;

  ParseMacro(Ctx, Macro, Ctx->Memory);
}

// NOTE(Jesse): This routine was written with the intent that it be
// substitituted in place of the conditionals marked by the following
// identifier.  Not 100% sure that it's appropriate, but there you go.
// @call_MacroShouldBeExpanded
bonsai_function b32
MacroShouldBeExpanded(parser *Parser, c_token *T, macro_def *Macro)
{
  Assert(PeekTokenPointer(Parser) == T);
  Assert(T->Type == CTokenType_Identifier);
  Assert(StringsMatch(T->Value, Macro->Name));

  b32 Result = False;

  switch (Macro->Type)
  {
    case type_macro_function:
    {
      if (PeekToken(Parser, 1).Type == CTokenType_OpenParen)
      {
        Result = True;
        T->Type = CT_MacroLiteral;
        T->Macro = Macro;
      }
    } break;

    case type_macro_keyword:
    {
      Result = True;
      T->Type = CT_MacroLiteral;
      T->Macro = Macro;
    } break;

    InvalidDefaultCase;
  }

  return Result;
}

bonsai_function void
SkipToEndOfCursor(c_token_cursor *At, c_token_cursor *ToSkip)
{
  if (ToSkip->Next)
  {
    DoublyLinkedListSwap(At, ToSkip->Next);
  }
  else
  {
    ToSkip->At = ToSkip->End;
    DoublyLinkedListSwap(At, ToSkip);
  }
}

bonsai_function parser *
TokenizeAnsiStream(ansi_stream Code, memory_arena* Memory, b32 IgnoreQuotes, parse_context *Ctx, token_cursor_source Source)
{
  if (!Code.Start)
  {
    Error("Input AnsiStream for %S is null.", Code.Filename);
    return 0;
  }

  u32 LineNumber = 1;
  parser *Result = Ctx ? Push( &Ctx->AllParsers,
                               AllocateParser(Code.Filename, LineNumber, (u32)Megabytes(1), Source, (u32)Megabytes(1), Memory),
                               Memory)
                       : AllocateParserPtr(Code.Filename, LineNumber, (u32)Megabytes(1), Source, (u32)Megabytes(1), Memory);

  b32 ParsingSingleLineComment = False;
  b32 ParsingMultiLineComment = False;

  c_token *LastTokenPushed = 0;
  while(Remaining(&Code))
  {
    const c_token FirstT = PeekToken(&Code);
    c_token PushT = { .Type = FirstT.Type, .Value = CS(Code.At, 1) };

    switch (FirstT.Type)
    {
      case CTokenType_FSlash:
      {
        const c_token SecondT = PeekToken(&Code, 1);

        switch (SecondT.Type)
        {
          case CTokenType_FSlash:
          {
            ParsingSingleLineComment = True;
            PushT.Type = CTokenType_CommentSingleLine;
            PushT.Value = CS(Code.At, 2);
            Advance(&Code);
            Advance(&Code);
          } break;

          case CTokenType_Star:
          {
            ParsingMultiLineComment = True;
            PushT.Type = CTokenType_CommentMultiLineStart;
            PushT.Value = CS(Code.At, 2);
            Advance(&Code);
            Advance(&Code);
          } break;

          case CTokenType_Equals:
          {
            PushT.Type = CTokenType_DivEquals;
            PushT.Value = CS(Code.At, 2);
            Advance(&Code);
            Advance(&Code);
          } break;

          default:
          {
            Advance(&Code);
          } break;
        }
      } break;

      case CTokenType_LT:
      {
        if (PeekToken(&Code, 1).Type == CTokenType_Equals)
        {
          PushT.Type = CTokenType_LessEqual;
          PushT.Value = CS(Code.At, 2);
          Advance(&Code);
        }
        else if (PeekToken(&Code, 1).Type == CTokenType_LT)
        {
          PushT.Type = CTokenType_LeftShift;
          PushT.Value = CS(Code.At, 2);
          Advance(&Code);
        }
        Advance(&Code);
      } break;

      case CTokenType_GT:
      {
        if (PeekToken(&Code, 1).Type == CTokenType_Equals)
        {
          PushT.Type = CTokenType_GreaterEqual;
          PushT.Value = CS(Code.At, 2);
          Advance(&Code);
        }
        else if (PeekToken(&Code, 1).Type == CTokenType_GT)
        {
          PushT.Type = CTokenType_RightShift;
          PushT.Value = CS(Code.At, 2);
          Advance(&Code);
        }
        Advance(&Code);
      } break;

      case CTokenType_Equals:
      {
        if (PeekToken(&Code, 1).Type == CTokenType_Equals)
        {
          PushT.Type = CTokenType_AreEqual;
          PushT.Value = CS(Code.At, 2);
          Advance(&Code);
        }
        Advance(&Code);
      } break;

      case CTokenType_Dot:
      {
        if (PeekToken(&Code, 1).Type == CTokenType_Dot &&
            PeekToken(&Code, 2).Type == CTokenType_Dot)
        {
          PushT.Type = CTokenType_Ellipsis;
          PushT.Value = CS(Code.At, 3);
          Advance(&Code);
          Advance(&Code);
          Advance(&Code);
        }
        else if (Remaining(&Code) > 1 && IsNumeric(*(Code.At+1)))
        {
          PushT = ParseNumericToken(&Code);
        }
        else
        {
          Advance(&Code);
        }

      } break;

      case CTokenType_Bang:
      {
        if (PeekToken(&Code, 1).Type == CTokenType_Equals)
        {
          PushT.Type = CTokenType_NotEqual;
          PushT.Value = CS(Code.At, 2);
          Advance(&Code);
        }
        Advance(&Code);
      } break;


      case CTokenType_Hat:
      {
        if (PeekToken(&Code, 1).Type == CTokenType_Equals)
        {
          PushT.Type = CTokenType_XorEquals;
          PushT.Value = CS(Code.At, 2);
          Advance(&Code);
        }
        Advance(&Code);
      }break;

      case CTokenType_Pipe:
      {
        if (PeekToken(&Code, 1).Type == CTokenType_Pipe)
        {
          PushT.Type = CTokenType_LogicalOr;
          PushT.Value = CS(Code.At, 2);
          Advance(&Code);
        }
        else if (PeekToken(&Code, 1).Type == CTokenType_Equals)
        {
          PushT.Type = CTokenType_OrEquals;
          PushT.Value = CS(Code.At, 2);
          Advance(&Code);
        }
        Advance(&Code);
      }break;

      case CTokenType_Ampersand:
      {
        if (PeekToken(&Code, 1).Type == CTokenType_Ampersand)
        {
          PushT.Type = CTokenType_LogicalAnd;
          PushT.Value = CS(Code.At, 2);
          Advance(&Code);
        }
        else if (PeekToken(&Code, 1).Type == CTokenType_Equals)
        {
          PushT.Type = CTokenType_AndEquals;
          PushT.Value = CS(Code.At, 2);
          Advance(&Code);
        }
        Advance(&Code);
      }break;

      case CTokenType_Percent:
      {
        if (PeekToken(&Code, 1).Type == CTokenType_Equals)
        {
          PushT.Type = CTokenType_ModEquals;
          PushT.Value = CS(Code.At, 2);
          Advance(&Code);
        }
        Advance(&Code);
      }break;

      case CTokenType_Minus:
      {
        if (PeekToken(&Code, 1).Type == CTokenType_Minus)
        {
          PushT.Type = CTokenType_Decrement;
          PushT.Value = CS(Code.At, 2);
          Advance(&Code);
        }
        else if (PeekToken(&Code, 1).Type == CTokenType_Equals)
        {
          PushT.Type = CTokenType_MinusEquals;
          PushT.Value = CS(Code.At, 2);
          Advance(&Code);
        }
        else if (PeekToken(&Code, 1).Type == CTokenType_GT)
        {
          PushT.Type = CTokenType_Arrow;
          PushT.Value = CS(Code.At, 2);
          Advance(&Code);
        }
        Advance(&Code);
      }break;

      case CTokenType_Plus:
      {
        if (PeekToken(&Code, 1).Type == CTokenType_Equals)
        {
          PushT.Type = CTokenType_PlusEquals;
          PushT.Value = CS(Code.At, 2);
          Advance(&Code);
        }
        else if (PeekToken(&Code, 1).Type == CTokenType_Plus)
        {
          PushT.Type = CTokenType_Increment;
          PushT.Value = CS(Code.At, 2);
          Advance(&Code);
        }
        Advance(&Code);
      }break;

      case CTokenType_Star:
      {
        if (PeekToken(&Code, 1).Type == CTokenType_Equals)
        {
          PushT.Type = CTokenType_TimesEquals;
          PushT.Value = CS(Code.At, 2);
          Advance(&Code);
        }
        else if (PeekToken(&Code, 1).Type == CTokenType_FSlash)
        {
          ParsingMultiLineComment = False;
          PushT.Type = CTokenType_CommentMultiLineEnd;
          PushT.Value = CS(Code.At, 2);
          Advance(&Code);
        }
        else
        {
          PushT.Type = CTokenType_Star;
          PushT.Value = CS(Code.At, 1);
        }
        Advance(&Code);
      } break;

      case CTokenType_SingleQuote:
      {
        if (IgnoreQuotes || ParsingSingleLineComment || ParsingMultiLineComment)
        {
          Advance(&Code);
        }
        else
        {
          PushT.Type = CTokenType_CharLiteral;
          PushT.Value = PopQuotedCharLiteral(&Code, True);

          if(PushT.Value.Count > 2)
          {
            Assert(PushT.Value.Start[0] == '\'');
            Assert(PushT.Value.Start[PushT.Value.Count-1] == '\'');

            for ( u32 CharIndex = 1;
                  CharIndex < PushT.Value.Count-1;
                  ++CharIndex )
            {
              PushT.UnsignedValue += (umm)PushT.Value.Start[CharIndex];
            }
          }
          else
          {
            Error("Quoted char literal with length %u .. ???", (u32)PushT.Value.Count);
          }
        }
      } break;

      case CTokenType_DoubleQuote:
      {
        if (IgnoreQuotes || ParsingSingleLineComment || ParsingMultiLineComment)
        {
          Advance(&Code);
        }
        else
        {
          PushT.Type = CTokenType_StringLiteral;
          PushT.Value = PopQuotedString(&Code, True);
        }
      } break;

      case CTokenType_BSlash:
      {
        if (PeekToken(&Code, 1).Type == CTokenType_CarrigeReturn)
        {
          ++PushT.Value.Count;
          Advance(&Code);
          Assert(PeekToken(&Code, 1).Type == CTokenType_Newline);
        }

        if (PeekToken(&Code, 1).Type == CTokenType_Newline)
        {
          PushT.Type = CTokenType_EscapedNewline;
          ++PushT.Value.Count;
          Advance(&Code);
          ++LineNumber;
        }
        Advance(&Code);
      } break;

      case CTokenType_CarrigeReturn:
      {
        Advance(&Code);
        if (PeekToken(&Code).Type == CTokenType_Newline)
        {
          PushT.Type = CTokenType_Newline;
          PushT.Value.Count = 2;
          ++LineNumber;
          ParsingSingleLineComment = False;
          Advance(&Code);
        }
      } break;

      case CTokenType_Newline:
      {
        ++LineNumber;
        ParsingSingleLineComment = False;
        Advance(&Code);
      } break;

      case CTokenType_Colon:
      {
        if (PeekToken(&Code, 1).Type == CTokenType_Colon)
        {
          Advance(&Code);
          Advance(&Code);
          PushT.Type = CT_ScopeResolutionOperator;
          PushT.Value.Count = 2;

          if (LastTokenPushed && LastTokenPushed->Type == CTokenType_Identifier)
          {
            LastTokenPushed->Type = CT_NameQualifier;
            PushT.QualifierName = LastTokenPushed;
          }
        }
        else
        {
          Advance(&Code);
        }

      } break;

      case CTokenType_Hash:
      {
        if (PeekToken(&Code, 1).Type == CTokenType_Hash)
        {
          Advance(&Code);
          Advance(&Code);
          PushT.Type = CT_PreprocessorPaste;
          PushT.Value.Count = 2;
        }
        else
        {
          if (Ctx)
          {
            const char* HashCharacter = Code.At;
            Advance(&Code);
            const char* FirstAfterHash = Code.At;

            LineNumber += EatSpacesTabsAndEscapedNewlines(&Code);

            counted_string TempValue = PopIdentifier(&Code);

            if ( StringsMatch(TempValue, CSz("define")) )
            {
              PushT.Type = CT_PreprocessorDefine;
              PushT.Value.Count = (umm)(TempValue.Start + TempValue.Count - HashCharacter);
            }
            else if ( StringsMatch(TempValue, CSz("undef")) )
            {
              PushT.Type = CT_PreprocessorUndef;
              PushT.Value.Count = (umm)(TempValue.Start + TempValue.Count - HashCharacter);
            }
            else if ( StringsMatch(TempValue, CSz("if")) )
            {
              PushT.Type = CT_PreprocessorIf;
              PushT.Value.Count = (umm)(TempValue.Start + TempValue.Count - HashCharacter);
            }
            else if ( StringsMatch(TempValue, CSz("else")) )
            {
              PushT.Type = CT_PreprocessorElse;
              PushT.Value.Count = (umm)(TempValue.Start + TempValue.Count - HashCharacter);
            }
            else if ( StringsMatch(TempValue, CSz("elif")) )
            {
              PushT.Type = CT_PreprocessorElif;
              PushT.Value.Count = (umm)(TempValue.Start + TempValue.Count - HashCharacter);
            }
            else if ( StringsMatch(TempValue, CSz("endif")) )
            {
              PushT.Type = CT_PreprocessorEndif;;
              PushT.Value.Count = (umm)(TempValue.Start + TempValue.Count - HashCharacter);
            }
            else if ( StringsMatch(TempValue, CSz("ifndef")) )
            {
              PushT.Type = CT_PreprocessorIfNotDefined;
              PushT.Value.Count = (umm)(TempValue.Start + TempValue.Count - HashCharacter);
            }
            else if ( StringsMatch(TempValue, CSz("ifdef")) )
            {
              PushT.Type = CT_PreprocessorIfDefined;
              PushT.Value.Count = (umm)(TempValue.Start + TempValue.Count - HashCharacter);
            }
            else if ( StringsMatch(TempValue, CSz("include")) )
            {
              PushT.Type = CT_PreprocessorInclude;
              PushT.Value.Count = (umm)(TempValue.Start + TempValue.Count - HashCharacter);
            }
            else if ( StringsMatch(TempValue, CSz("error")) )
            {
              PushT.Type = CT_PreprocessorError;
              PushT.Value.Count = (umm)(TempValue.Start + TempValue.Count - HashCharacter);
            }
            else if ( StringsMatch(TempValue, CSz("warning")) )
            {
              PushT.Type = CT_PreprocessorWarning;
              PushT.Value.Count = (umm)(TempValue.Start + TempValue.Count - HashCharacter);
            }
            else if ( StringsMatch(TempValue, CSz("pragma")) )
            {
              PushT.Type = CT_PreprocessorPragma;
              PushT.Value.Count = (umm)(TempValue.Start + TempValue.Count - HashCharacter);
            }
            else
            {
              // The token is just a regular hash .. roll back our parsing to the start.
              Code.At = FirstAfterHash;
            }
          }
          else
          {
            Advance(&Code);
          }
        }

      } break;

      case CTokenType_Unknown:
      {
        if (IsNumeric(*Code.At))
        {
          PushT = ParseNumericToken(&Code);
        }
        else
        {
          PushT.Type = CTokenType_Identifier;
          PushT.Value = PopIdentifier(&Code);

          if ( StringsMatch(PushT.Value, CSz("if")) )
          {
            PushT.Type = CTokenType_If;
          }
          else if ( StringsMatch(PushT.Value, CSz("else")) )
          {
            PushT.Type = CTokenType_Else;
          }
          else if ( StringsMatch(PushT.Value, CSz("break")) )
          {
            PushT.Type = CTokenType_Break;
          }
          else if ( StringsMatch(PushT.Value, CSz("switch")) )
          {
            PushT.Type = CTokenType_Switch;
          }
          else if ( StringsMatch(PushT.Value, CSz("case")) )
          {
            PushT.Type = CTokenType_Case;
          }
          else if ( StringsMatch(PushT.Value, CSz("default")) )
          {
            PushT.Type = CTokenType_Default;
          }
          else if ( StringsMatch(PushT.Value, CSz("for")) )
          {
            PushT.Type = CTokenType_For;
          }
          else if ( StringsMatch(PushT.Value, CSz("while")) )
          {
            PushT.Type = CTokenType_While;
          }
          else if ( StringsMatch(PushT.Value, CSz("continue")) )
          {
            PushT.Type = CTokenType_Continue;
          }
          else if ( StringsMatch(PushT.Value, CSz("return")) )
          {
            PushT.Type = CTokenType_Return;
          }
          else if ( StringsMatch(PushT.Value, CSz("thread_local")) )
          {
            PushT.Type = CTokenType_ThreadLocal;
          }
          else if ( StringsMatch(PushT.Value, CSz("const")) )
          {
            PushT.Type = CTokenType_Const;
          }
          else if ( StringsMatch(PushT.Value, CSz("static")) )
          {
            PushT.Type = CTokenType_Static;
          }
          else if ( StringsMatch(PushT.Value, CSz("__volatile__")) )
          {
            PushT.Type = CTokenType_Volatile;
          }
          else if ( StringsMatch(PushT.Value, CSz("volatile")) )
          {
            PushT.Type = CTokenType_Volatile;
          }
          else if ( StringsMatch(PushT.Value, CSz("void")) )
          {
            PushT.Type = CTokenType_Void;
          }
          else if ( StringsMatch(PushT.Value, CSz("long")) )
          {
            PushT.Type = CTokenType_Long;
          }
          else if ( StringsMatch(PushT.Value, CSz("float")) )
          {
            PushT.Type = CTokenType_Float;
          }
          else if ( StringsMatch(PushT.Value, CSz("char")) )
          {
            PushT.Type = CTokenType_Char;
          }
          else if ( StringsMatch(PushT.Value, CSz("double")) )
          {
            PushT.Type = CTokenType_Double;
          }
          else if ( StringsMatch(PushT.Value, CSz("short")) )
          {
            PushT.Type = CTokenType_Short;
          }
          else if ( StringsMatch(PushT.Value, CSz("int")) )
          {
            PushT.Type = CTokenType_Int;
          }
          else if ( StringsMatch(PushT.Value, CSz("bool")) )
          {
            PushT.Type = CTokenType_Bool;
          }
          else if ( StringsMatch(PushT.Value, CSz("__m128")) )
          {
            PushT.Type = CTokenType_M128;
          }
          else if ( StringsMatch(PushT.Value, CSz("auto")) )
          {
            PushT.Type = CTokenType_Auto;
          }
          else if ( StringsMatch(PushT.Value, CSz("signed")) )
          {
            PushT.Type = CTokenType_Signed;
          }
          else if ( StringsMatch(PushT.Value, CSz("unsigned")) )
          {
            PushT.Type = CTokenType_Unsigned;
          }
          else if ( StringsMatch(PushT.Value, CSz("struct")) )
          {
            PushT.Type = CTokenType_Struct;
          }
          else if ( StringsMatch(PushT.Value, CSz("typedef")) )
          {
            PushT.Type = CTokenType_Typedef;
          }
          else if ( StringsMatch(PushT.Value, CSz("__asm__")) )
          {
            PushT.Type = CTokenType_Asm;
          }
          else if ( StringsMatch(PushT.Value, CSz("asm")) )
          {
            PushT.Type = CTokenType_Asm;
          }
          else if ( StringsMatch(PushT.Value, CSz("meta")) )
          {
            PushT.Type = CTokenType_Meta;
          }
          else if ( StringsMatch(PushT.Value, CSz("union")) )
          {
            PushT.Type = CTokenType_Union;
          }
          else if ( StringsMatch(PushT.Value, CSz("using")) )
          {
            PushT.Type = CTokenType_Using;
          }
          else if ( StringsMatch(PushT.Value, CSz("enum")) )
          {
            PushT.Type = CTokenType_Enum;
          }
          else if ( StringsMatch(PushT.Value, CSz("goto")) )
          {
            PushT.Type = CTokenType_Goto;
          }
          else if ( StringsMatch(PushT.Value, CSz("template")) )
          {
            PushT.Type = CTokenType_TemplateKeyword;
          }
          else if ( StringsMatch(PushT.Value, CSz("inline")) )
          {
            PushT.Type = CTokenType_Inline;
          }
          else if ( StringsMatch(PushT.Value, CSz("operator")) )
          {
            PushT.Type = CTokenType_OperatorKeyword;
          }
          else if ( StringsMatch(PushT.Value, CSz("static_assert")) )
          {
            PushT.Type = CT_StaticAssert;
          }
          else if ( StringsMatch(PushT.Value, CSz("_Pragma")) )
          {
            PushT.Type = CT_KeywordPragma;
          }
          else if ( StringsMatch(PushT.Value, CSz("__pragma")) )
          {
            PushT.Type = CT_KeywordPragma;
          }
          else if ( StringsMatch(PushT.Value, CSz("extern")) )
          {
            PushT.Type = CTokenType_Extern;
          }
          else if ( StringsMatch(PushT.Value, CSz("__VA_ARGS__")) )
          {
            PushT.Type = CT_Preprocessor__VA_ARGS__;
          }
          else
          {
            if (LastTokenPushed && LastTokenPushed->Type == CT_ScopeResolutionOperator)
            {
              PushT.QualifierName = LastTokenPushed->QualifierName;
            }
          }
        }

      } break;

      default: { Advance(&Code); } break;
    }

    Assert(PushT.Type);
    LastTokenPushed = Push(PushT, &Result->Tokens);

    continue;
  }

  TruncateToCurrentSize(&Result->Tokens);

  // TODO(Jesse): Is this really necessary?
  Rewind(Result);
  Result->Valid = True;
  Rewind(Result);


  //
  //
  // Finished Tokenization -- Run Preprocessor
  //
  //


  // Go through and do macro/include expansion as necessary
  c_token *LastT = 0;
  while (TokensRemain(Result))
  {
    c_token *T = PeekTokenPointer(Result);
    switch (T->Type)
    {
      case CTokenType_Meta:
      {
        RequireToken(Result, T->Type);
        EatBetween(Result, CTokenType_OpenParen, CTokenType_CloseParen);
      } break;

      case CT_PreprocessorPragma:
      {
        RequireToken(Result, T->Type);
        EatUntilExcluding(Result, CTokenType_Newline);
        EraseSectionOfParser(Result, T, Result->Tokens.At);
      } break;

      case CT_PreprocessorInclude:
      {
        debug_global u32 HitCount = 0;

        /* Debug("%SHITCOUNT == %d%S", TerminalColors.Red, HitCount, TerminalColors.White); */

        parser *IncludeParser = ResolveInclude(Ctx, Result);
        EraseSectionOfParser(Result, T, Result->Tokens.At);
        RequireTokenRaw(Result, CTokenType_Newline);

        if (IncludeParser)
        {
          /* if (HitCount == 2) */
          {
            /* DumpChain(IncludeParser); */
          }

          c_token_cursor *LastCursorOfChain = SplitAndInsertTokenCursor(&Result->Tokens, Result->LineNumber, &IncludeParser->Tokens, Memory);
          SkipToEndOfCursor(&Result->Tokens, LastCursorOfChain);

          /* if (HitCount == 1) */
          {
            /* DumpChain(Result); */
            /* exit(0); */
          }
        }

        ++HitCount;

      } break;

      case CTokenType_Identifier:
      {
        if (Ctx)
        {
          if (macro_def *Macro = GetMacroDef(Ctx, T->Value))
          {
            if (MacroShouldBeExpanded(Result, T, Macro))
            {
              parser *Expanded = ExpandMacro(Ctx, Result, Macro, Memory, True);

              EraseSectionOfParser(Result, T, Result->Tokens.At);

              c_token_cursor *LastCursorOfChain = SplitAndInsertTokenCursor(&Result->Tokens, Result->LineNumber, &Expanded->Tokens, Memory);
              SkipToEndOfCursor(&Result->Tokens, LastCursorOfChain);
            }
            else
            {
              RequireToken(Result, CToken(T->Value));
            }
          }
          else
          {
            RequireToken(Result, CToken(T->Value));
          }
        }
        else
        {
          RequireToken(Result, CToken(T->Value));
        }
      } break;

      case CT_MacroLiteral:
      {
        InvalidCodePath();
      } break;

      case CT_PreprocessorDefine:
      {
        c_token *MacroNameToken = PeekTokenPointer(Result, 1);

        if (Ctx)
        {
          macro_def *Macro = GetByName(&Ctx->Datatypes.Macros, MacroNameToken->Value);

          MacroNameToken->Type = CT_MacroLiteral;
          MacroNameToken->Macro = Macro;

          if (Macro)
          {
            if (!Macro->Undefed)
            {
              // TODO(Jesse): If the macro is redefined with a matching value
              // to the currently defined one, don't snow this warning.
              Warn(FormatCountedString(TranArena, CSz("Macro (%S) already defined"), Macro->Name));
            }

            DefineMacro(Ctx, Result, Macro);
          }
          else
          {
            macro_def *NewMacro = Push(&Ctx->Datatypes.Macros, { .Name = MacroNameToken->Value }, Ctx->Memory);
            DefineMacro(Ctx, Result, NewMacro);
          }

          /* RequireTokenRaw(Parser, CTokenType_Newline); */
          EraseSectionOfParser(Result, T, Result->Tokens.At);
          RequireTokenRaw(Result, CTokenType_Newline);
        }

      } break;

      case CT_PreprocessorUndef:
      {
        RequireToken(Result, T->Type);
        counted_string MacroName = PopToken(Result).Value;
        if (Ctx)
        {
          macro_def *Macro1 = GetMacroDef(Ctx, MacroName);
          if (Macro1) { Macro1->Undefed = True; }
        }
        EatUntilExcluding(Result, CTokenType_Newline);
        EraseSectionOfParser(Result, T, Result->Tokens.At);
        RequireTokenRaw(Result, CTokenType_Newline);
      } break;

      case CT_PreprocessorIf:
      case CT_PreprocessorElif:
      {
        RequireToken(Result, T->Type);
        if (ResolveMacroConstantExpression(Ctx, Result, Memory, 0, False))
        {
          EraseSectionOfParser(Result, T, Result->Tokens.At);
          c_token *NextPreprocessorToken = EatIfBlock(Result);
          EatUntilIncluding(Result, CT_PreprocessorEndif);
          EraseSectionOfParser(Result, NextPreprocessorToken, Result->Tokens.At);
          RewindParserUntil(Result, T);
        }
        else
        {
          EatIfBlock(Result);
          OptionalToken(Result, CT_PreprocessorEndif);
          EraseSectionOfParser(Result, T, Result->Tokens.At);
        }
      } break;

      case CT_PreprocessorIfDefined:
      {
        RequireToken(Result, T->Type);
        c_token *DefineValue = PeekTokenPointer(Result);
        if ( GetMacroDef(Ctx, DefineValue->Value) )
        {
          EraseToken(T);
          EraseToken(DefineValue);
          c_token *NextPreprocessorToken = EatIfBlock(Result);
          EatUntilIncluding(Result, CT_PreprocessorEndif);
          EraseSectionOfParser(Result, NextPreprocessorToken, Result->Tokens.At);
          RewindParserUntil(Result, T);
        }
        else
        {
          c_token *NextPreprocessorToken = EatIfBlock(Result);
          EraseSectionOfParser(Result, T, NextPreprocessorToken);
        }
      } break;

      case CT_PreprocessorIfNotDefined:
      {
        RequireToken(Result, T->Type);
        c_token *DefineValue = PeekTokenPointer(Result);
        if ( ! GetMacroDef(Ctx, DefineValue->Value) )
        {
          EraseToken(T);
          EraseToken(DefineValue);
          c_token *NextPreprocessorToken = EatIfBlock(Result);
          EatUntilIncluding(Result, CT_PreprocessorEndif);
          EraseSectionOfParser(Result, NextPreprocessorToken, Result->Tokens.At);
          RewindParserUntil(Result, T);
        }
        else
        {
          c_token *NextPreprocessorToken = EatIfBlock(Result);
          EraseSectionOfParser(Result, T, NextPreprocessorToken);
        }
      } break;

      // If we hit this case it must be the last one in an #if #elif chain because when
      // we hit a true clause we eat the rest of the clauses.
      case CT_PreprocessorElse:
      case CT_PreprocessorEndif:
      {
        EraseToken(T);
      } break;

      /* case CTokenType_Extern: */
      /* { */
      /*   RequireToken(Result, T->Type); */
      /*   RuntimeBreak(); */
      /* } break; */

      default:
      {
        RequireToken(Result, T->Type);
      } break;
    }
  }

  /* DumpChain(Result); */

#if 0
  // Go through and erase preprocessor crap once everything's parsed out
  Rewind(Result);

  while (RawTokensRemain(Result))
  {
    c_token *T = PeekTokenRawPointer(Result);
    switch (T->Type)
    {

      case CT_MacroLiteral:
      {
        RequireTokenRaw(Result, T->Type);
        if (T->Macro->Type == type_macro_function)
        {
          /* EatBetween(Result, CTokenType_OpenParen, CTokenType_CloseParen); */
          /* EraseSectionOfParser(Result, T, Result->Tokens.At); */
        }
        else if (T->Macro->Type == type_macro_keyword)
        {
          /* EraseToken(T); */
        }
        else
        {
          InvalidCodePath();
        }
      } break;

      case CT_PreprocessorDefine:
      case CT_PreprocessorUndef:
      {
        /* RequireTokenRaw(Result, T->Type); */
        /* EatUntilExcluding(Result, CTokenType_Newline); */
        /* EraseSectionOfParser(Result, T, Result->Tokens.At); */
      } break;

      default:
      {
        RequireTokenRaw(Result, T->Type);
      } break;
    }
  }
#endif


  if (Result->Tokens.EndLine == 0)
  {
    Result->Tokens.EndLine = LineNumber;
  }
  // TODO(Jesse): Pretty sure this assertion should hold up.  This case (I
  // think) is when we split a parser but there's no `SecondHalfOfSplit`
  // because it would be 0 length
  /* else */
  /* { */
  /*   Assert(Result->Tokens.EndLine == LineNumber); */
  /* } */

  Rewind(Result);

  return Result;
}

bonsai_function parser *
TokenizeAnsiStream(parse_context *Ctx, ansi_stream Code, token_cursor_source Source)
{
  parser *Result = TokenizeAnsiStream(Code, Ctx->Memory, False, Ctx, Source);
  return Result;
}

bonsai_function parser *
ParserForFile(parse_context *Ctx, counted_string Filename, token_cursor_source Source)
{
  parser *Result = 0;
  ansi_stream SourceFileStream = AnsiStreamFromFile(Filename, Ctx->Memory);

  if (SourceFileStream.Start)
  {
    Result = TokenizeAnsiStream(Ctx, SourceFileStream, Source);
  }

  return Result;
}





// TODO(Jesse id: 302, tags: id_301)
bonsai_function parser*
GetByFilename(parser_stream* Stream, counted_string Filename)
{
  TIMED_FUNCTION();

  parser* Result = 0;
  ITERATE_OVER(Stream)
  {
    parser *Parser = GET_ELEMENT(Iter);
    if (StringsMatch(Parser->Filename, Filename))
    {
      Result = Parser;
      break;
    }
  }

  return Result;
}

// TODO(Jesse): Should this return a c_token_cursor* ?
bonsai_function parser *
ResolveInclude(parse_context *Ctx, parser *Parser)
{
  counted_string FinalIncludePath = {};
  parser *Result = {};

  c_token *ErrorToken = PeekTokenPointer(Parser);

  RequireToken(Parser, CT_PreprocessorInclude);

  counted_string PartialPath = {};
  c_token NextToken = PeekToken(Parser);
  if (NextToken.Type == CTokenType_StringLiteral)
  {
    PartialPath = RequireToken(Parser, CTokenType_StringLiteral).Value;
    Error("Relative includes NOT SUPPORTED (%S)", PartialPath);
  }
  else
  {
    RequireToken(Parser, CTokenType_LT);
    string_from_parser Builder = StartStringFromParser(Parser);
    EatUntilExcluding(Parser, CTokenType_GT);
    PartialPath = FinalizeStringFromParser(&Builder);
    if (PartialPath.Count == 0)
    {
      ParseError(Parser, CSz("Include path must be specified."));
    }
    else
    {
      RequireToken(Parser, CTokenType_GT);
      b32 Continue = true;
      while (Continue)
      {
        b32 AteWhitespace = EatSpacesTabsAndEscapedNewlines(Parser);
        b32 AteComment = EatComment(Parser);
        Continue = AteWhitespace || AteComment;
      }
    }
  }

  counted_string_cursor *IncludePaths = Ctx->IncludePaths;
  if (IncludePaths)
  {
    if (!Result)
    {
      for ( u32 PrefixIndex = 0;
            PrefixIndex < Count(IncludePaths);
            ++PrefixIndex )
      {
        counted_string PrefixPath = IncludePaths->Start[PrefixIndex];
        counted_string FullPath = Concat(PrefixPath, PartialPath, TranArena); // TODO(Jesse id: 304): only do this work once

        if (FileExists(FullPath))
        {
          FinalIncludePath = FullPath;
          break;
        }
      }

    }
  }

  if ( ! FinalIncludePath.Count)
  {
    if (FileExists(PartialPath))
    {
      FinalIncludePath = PartialPath;
    }
  }

  if (FinalIncludePath.Count)
  {
    LogSuccess("Including (%S)", FinalIncludePath);
    Result = ParserForFile(Ctx, FinalIncludePath, TokenCursorSource_Include);
  }
  else
  {
    Warn("Unable to resolve include for file : (%S)", PartialPath);
  }

  return Result;
}







d_union_member
DUnionMember(counted_string Name, counted_string Type, d_union_flags Flags)
{
  d_union_member Result = {};
  Result.Name = Name;
  Result.Type = Type;
  Result.Flags = Flags;
  return Result;
}

void
PushMember(d_union_decl* dUnion, c_token MemberIdentifierToken, d_union_flags Flags, memory_arena* Memory)
{
  Assert(MemberIdentifierToken.Type == CTokenType_Identifier);
  d_union_member Member = DUnionMember(MemberIdentifierToken.Value, MemberIdentifierToken.Value, Flags);
  Push(&dUnion->Members, Member, Memory);
}

bonsai_function counted_string
GenerateEnumDef(d_union_decl* dUnion, memory_arena* Memory)
{
  TIMED_FUNCTION();

  counted_string Result = FormatCountedString(Memory, CSz("enum %.*s_type\n{\n  type_%.*s_noop,\n"), dUnion->Name.Count, dUnion->Name.Start, dUnion->Name.Count, dUnion->Name.Start);

  for (d_union_member_iterator Iter = Iterator(&dUnion->Members);
      IsValid(&Iter);
      Advance(&Iter))
  {
    d_union_member* Member = &Iter.At->Element;
    Result = Concat(Result, FormatCountedString(Memory, CSz("  type_%S,\n"), Member->Type), Memory);
  }

  Result = Concat(Result, CS("};\n\n"), Memory);
  return Result;
}

bonsai_function counted_string
GenerateStructDef(d_union_decl* dUnion, memory_arena* Memory)
{
  TIMED_FUNCTION();

  counted_string UnionName = dUnion->Name;
  counted_string TagType = dUnion->CustomEnumType.Count ?
    dUnion->CustomEnumType :
    FormatCountedString(Memory, CSz("%S_type"), UnionName);

  counted_string Result = FormatCountedString(Memory, CSz("struct %S\n{\n  %S Type;\n"),
      UnionName, TagType);

  ITERATE_OVER(&dUnion->CommonMembers)
  {
    struct_member* Member = GET_ELEMENT(Iter);
    Assert(Member->Type == type_variable_decl);
    Result =
      Concat(Result,
        FormatCountedString(Memory, CSz("  %S %S;\n"),
          Member->variable_decl.Type.Token.Value,
          Member->variable_decl.Name),
      Memory);
  }
  Result = Concat(Result, CS("\n  union\n  {\n"), Memory);

  for (d_union_member_iterator Iter = Iterator(&dUnion->Members);
      IsValid(&Iter);
      Advance(&Iter))
  {
    d_union_member* Member = &Iter.At->Element;
    if (Member->Flags != d_union_flag_enum_only)
    {
      Result = Concat(Result, FormatCountedString(Memory, CSz("    %.*s %.*s;\n"), Member->Type.Count, Member->Type.Start, Member->Name.Count, Member->Name.Start), Memory);
    }
  }

  Result = Concat(Result, CS("  };\n};\n\n"), Memory);

  return Result;
}

bonsai_function stl_container_def*
GetStlContainer(stl_container_def_stream* Stream, counted_string Name)
{
  stl_container_def *Result = 0;
  ITERATE_OVER(Stream)
  {
    auto* T = GET_ELEMENT(Iter);
    if (StringsMatch(T->Name, Name))
    {
      Result = T;
      break;
    }
  }

  return Result;
}

bonsai_function type_def*
GetTypedefByAlias(type_def_stream* Typedefs, counted_string Alias)
{
  type_def *Result = 0;
  ITERATE_OVER(Typedefs)
  {
    type_def* T = GET_ELEMENT(Iter);
    if (StringsMatch(T->Alias, Alias))
    {
      Result = T;
      break;
    }
  }

  return Result;
}


// TODO(Jesse id: 301, tags: metaprogramming):  These functions are super repetitive, generate them!
bonsai_function enum_def*
GetEnumByType(enum_def_stream* ProgramEnums, counted_string EnumType)
{
  TIMED_FUNCTION();

  enum_def* Result = 0;
  for (enum_def_iterator Iter = Iterator(ProgramEnums);
      IsValid(&Iter);
      Advance(&Iter))
  {
    enum_def* Enum = &Iter.At->Element;
    if (StringsMatch(Enum->Name, EnumType))
    {
      Result = Enum;
      break;
    }
  }

  return Result;
}

bonsai_function struct_def*
GetStructByType(struct_def_stream* ProgramStructs, counted_string StructType)
{
  TIMED_FUNCTION();

  struct_def* Result = 0;
  for (struct_def_iterator Iter = Iterator(ProgramStructs);
      IsValid(&Iter);
      Advance(&Iter))
  {
    struct_def* Struct = &Iter.At->Element;
    if (StringsMatch(Struct->Type, StructType))
    {
      Result = Struct;
      break;
    }
  }

  return Result;
}

bonsai_function datatype
GetDatatypeByName(program_datatypes* Datatypes, counted_string Name)
{
  TIMED_FUNCTION();

  // TODO(Jesse id: 295, tags: speed): This could be optimized significantly by shuffling the logic around, not to mention using hashtables.
  struct_def *S = GetStructByType(&Datatypes->Structs, Name);
  enum_def   *E = GetEnumByType(&Datatypes->Enums, Name);
  type_def   *T = GetTypedefByAlias(&Datatypes->Typedefs, Name);

  stl_container_def   *StlContainer = GetStlContainer(&Datatypes->StlContainers, Name);

  datatype Result = {};

  if (S)
  {
    Assert(!T && !E && !StlContainer);
    Result = Datatype(S);
  }
  else if (E)
  {
    Assert(!T && !S && !StlContainer);
    Result = Datatype(E);
  }
  else if (T)
  {
    Assert(!E && !S && !StlContainer);
    Result = Datatype(T);
  }
  else if (StlContainer)
  {
    Assert(!E && !S && !T);
    Result = Datatype(StlContainer);
  }

  return Result;
}

d_union_decl
ParseDiscriminatedUnion(parser* Parser, program_datatypes* Datatypes, counted_string Name, memory_arena* Memory)
{
  TIMED_FUNCTION();

  d_union_decl dUnion = {};

  dUnion.Name = Name;

  if (OptionalToken(Parser, CTokenType_OpenBrace))
  {
    b32 Complete = False;
    while (!Complete && Remaining(Parser))
    {
      c_token Interior = PeekToken(Parser);

      switch (Interior.Type)
      {
        case CTokenType_Identifier:
        {
          RequireToken(Parser, Interior);

          d_union_flags Flags = {};
          if ( OptionalToken(Parser, CToken(ToString(enum_only))) )
          {
            Flags = d_union_flag_enum_only;
          }

          PushMember(&dUnion, Interior, Flags, Memory);
        } break;

        case CTokenType_CloseBrace:
        {
          RequireToken(Parser, CTokenType_CloseBrace);
          Complete = True;
        } break;

        default:
        {
          Parser->Valid = False;
          Complete = True;
        } break;
      }
    }
  }
  else
  {
    dUnion.CustomEnumType = RequireToken(Parser, CTokenType_Identifier).Value;;

    enum_def* EnumDef = GetEnumByType(&Datatypes->Enums, dUnion.CustomEnumType);
    if (EnumDef)
    {
      ITERATE_OVER(&EnumDef->Members)
      {
        enum_member* Field = GET_ELEMENT(Iter);
        counted_string MemberName = Concat(Concat(dUnion.Name, CS("_"), Memory), Field->Name, Memory);
        PushMember(&dUnion, CToken(MemberName), d_union_flag_none, Memory);
      }
    }
    else
    {
      Error("Couldn't find enum %.*s", (s32)dUnion.CustomEnumType.Count, dUnion.CustomEnumType.Start);
    }
  }

  if (OptionalToken(Parser, CTokenType_Comma))
  {
    RequireToken(Parser, CTokenType_OpenBrace);
    while (!OptionalToken(Parser, CTokenType_CloseBrace))
    {
      struct_member Decl = {
        .Type = type_variable_decl,
        .variable_decl = {
          .Type.Token = RequireToken(Parser, CTokenType_Identifier),
          .Name       = RequireToken(Parser, CTokenType_Identifier).Value,
        }
      };
      Push(&dUnion.CommonMembers, Decl, Memory);
    }
  }

  return dUnion;
}

bonsai_function b32
DoDebugWindow(const char** ArgStrings, u32 ArgCount)
{
  b32 Result = False;
  for (u32 ArgIndex = 1;
      ArgIndex < ArgCount;
      ++ArgIndex)
  {
    counted_string Arg = CS(ArgStrings[ArgIndex]);
    if (StringsMatch(CS("-d"), Arg) ||
        StringsMatch(CS("--do-debug-window"), Arg) )
    {
      Result = True;
    }
  }

  return Result;
}

bonsai_function counted_string
PopArgString(const char** ArgStrings, u32 ArgStringCount, u32* ArgIndex)
{
  counted_string Result = {};

  if (*ArgIndex+1 < ArgStringCount)
  {
    *ArgIndex = *ArgIndex + 1;
    Result = CS(ArgStrings[*ArgIndex]);
  }
  else
  {
    Assert(*ArgIndex == ArgStringCount-1);
    Error("Argument required for '%s'.", ArgStrings[*ArgIndex]);
  }

  return Result;
}

bonsai_function arguments
ParseArgs(const char** ArgStrings, u32 ArgCount, parse_context *Ctx, memory_arena* Memory)
{
  arguments Result = {
    .Outpath      = CS("src/metaprogramming/output"),
    .Files        = AllocateBuffer<counted_string_cursor, counted_string>((u32)ArgCount, Memory),
    .IncludePaths = AllocateBuffer<counted_string_cursor, counted_string>((u32)ArgCount, Memory),
  };

  for ( u32 ArgIndex = 1;
        ArgIndex < ArgCount;
        ++ArgIndex)
  {
    counted_string Arg = CS(ArgStrings[ArgIndex]);

    if (StringsMatch(CS("-d"), Arg) ||
        StringsMatch(CS("--do-debug-window"), Arg) )
    {
      Result.DoDebugWindow = True;
    }
    else if ( StringsMatch(CS("-I"), Arg) ||
              StringsMatch(CS("--include-path"), Arg) )
    {
      counted_string Include = PopArgString(ArgStrings, ArgCount, &ArgIndex);

      if (Include.Count)
      {
        if (Include.Start[Include.Count-1] == '/')
        {
        }
        else
        {
          Include = Concat(Include, CSz("/"), TranArena ); // Make sure we end with a '/'
        }

        Info("Include path added : (%.*s)", (u32)Include.Count, Include.Start);
        Push(Include, &Result.IncludePaths);
      }
    }
    else if ( StringsMatch(CS("-D"), Arg) ||
              StringsMatch(CS("--define"), Arg) )
    {
      if (ArgIndex+1 < ArgCount)
      {
        ArgIndex += 1;
        counted_string MacroName = CS(ArgStrings[ArgIndex]);

        if (Contains(CSz("="), MacroName))
        {
          Warn("Currently, custom define values are unsupported.  Please use `(--define/-D) DEFINE_NAME` to set DEFINE_NAME=1.");
        }

        macro_def M = {
          .Type = type_macro_keyword,
          .Name = MacroName,
        };

        // TODO(Jesse): Do we want to track where macro bodies came from more carefully?
        M.Body.Tokens = AllocateTokenBuffer(Memory, 1, TokenCursorSource_Unknown, 0);
        M.Body.Tokens.Start[0] = CToken(1u);

        macro_def *NewMacro = Push(&Ctx->Datatypes.Macros, M, Ctx->Memory);
      }
      else
      {
        Error("Macro name required when using (--define/-D) switches.");
      }

    }
#if 0
    // NOTE(Jesse): This has to come after the above -D path
    else if ( StartsWith(CSz("-D"), Arg) )
    {
      macro_def *NewMacro = Push(&Ctx->Datatypes.Macros, { .Name = MacroNameToken->Value }, Ctx->Memory);
      NewMacro->Type = type_macro_keyword;
      counted_string Name = Substring(Arg, 2);
      NewMacro->Name = Name;
    }
#endif
    else if ( StringsMatch(CS("-c0"), Arg) ||
              StringsMatch(CS("--colors-off"), Arg) )
    {
    }
    else if ( StringsMatch(CS("-o"), Arg) ||
              StringsMatch(CS("--output-path"), Arg) )
    {
      Result.Outpath = PopArgString(ArgStrings, ArgCount, &ArgIndex);
      Error("Output path _NOT_CURRENTLY_SUPPORTED_ : (%.*s)", (u32)Result.Outpath.Count, Result.Outpath.Start);
    }
    else
    {
      Push(Arg, &Result.Files);
    }
  }

  TruncateToCurrentSize(&Result.Files);
  Rewind(&Result.Files);

  TruncateToCurrentSize(&Result.IncludePaths);
  Rewind(&Result.IncludePaths);

  Info("%d total include paths added", (u32)Count(&Result.IncludePaths));

  return Result;
}

global_variable random_series TempFileEntropy = {3215432};

bonsai_function b32
Output(c_token_cursor Code, counted_string Filename, memory_arena* Memory)
{
  TIMED_FUNCTION();
  b32 Result = False;

  native_file TempFile = GetTempFile(&TempFileEntropy, Memory);
  if (TempFile.Handle)
  {
    Rewind(&Code);
    b32 FileWritesSucceeded = True;
    while(Remaining(&Code))
    {
      if (Code.At->Value.Count)
      {
        FileWritesSucceeded &= WriteToFile(&TempFile, Code.At->Value);
      }
      else
      {
        FileWritesSucceeded &= WriteToFile(&TempFile, CS((const char*)&Code.At->Type, 1));
      }

      Advance(&Code);
    }
    FileWritesSucceeded &= CloseFile(&TempFile);

    if (FileWritesSucceeded)
    {
      if (Rename(TempFile, Filename))
      {
        Result = True;
      }
      else
      {
        Error("Renaming tempfile: %.*s -> %.*s", (s32)TempFile.Path.Count, TempFile.Path.Start, (s32)Filename.Count, Filename.Start);
      }
    }
    else
    {
      Error("Writing to tempfile: %.*s", (s32)TempFile.Path.Count, TempFile.Path.Start);
    }
  }
  else
  {
    Error("Opening tempfile: %.*s", (s32)TempFile.Path.Count, TempFile.Path.Start);
  }

  return Result;
}

bonsai_function b32
Output(counted_string Code, counted_string OutputFilename, memory_arena* Memory, output_mode Mode = Output_NoOverwrite)
{
  TIMED_FUNCTION();
  Mode = Output_Unsafe;
  b32 Result = False;

  native_file TempFile = GetTempFile(&TempFileEntropy, Memory);
  if (TempFile.Handle)
  {
    b32 FileWritesSucceeded = WriteToFile(&TempFile, Code);
    FileWritesSucceeded &= WriteToFile(&TempFile, CS("\n"));
    FileWritesSucceeded &= CloseFile(&TempFile);

    if (FileWritesSucceeded)
    {
      if (Mode == Output_NoOverwrite)
      {
        if (FileExists(OutputFilename))
        {
          counted_string FileContents = ReadEntireFileIntoString(OutputFilename, Memory);
          if (StringsMatch(Trim(Code), Trim(FileContents)))
          {
            Info("File contents matched output for %.*s", (u32)OutputFilename.Count, OutputFilename.Start);
          }
          else
          {
            Error("File contents didn't match for %.*s", (u32)OutputFilename.Count, OutputFilename.Start);
            Error("TODO(Jesse): Should probably emit to a similar filname with an incremented extension or something..");
          }
        }
        else if (Rename(TempFile, OutputFilename))
        {
          Info("Generated and output %.*s", (u32)OutputFilename.Count, OutputFilename.Start);
          Result = True;
        }
        else
        {
          Error("Renaming tempfile: %.*s -> %.*s", (s32)TempFile.Path.Count, TempFile.Path.Start, (s32)OutputFilename.Count, OutputFilename.Start);
        }
      }
      else
      {
        if (Rename(TempFile, OutputFilename))
        {
          Info("Generated and output %.*s", (u32)OutputFilename.Count, OutputFilename.Start);
          Result = True;
        }
        else
        {
          Error("Renaming tempfile: %.*s -> %.*s", (s32)TempFile.Path.Count, TempFile.Path.Start, (s32)OutputFilename.Count, OutputFilename.Start);
        }
      }
    }
    else
    {
      Error("Writing to tempfile: %.*s", (s32)TempFile.Path.Count, TempFile.Path.Start);
    }
  }
  else
  {
    Error("Opening tempfile: %.*s", (s32)TempFile.Path.Count, TempFile.Path.Start);
  }

  return Result;
}

bonsai_function void
DumpStringStreamToConsole(counted_string_stream* Stream)
{
  for (counted_string_iterator Iter = Iterator(Stream);
      IsValid(&Iter);
      Advance(&Iter))
  {
    counted_string Message = Iter.At->Element;
    Debug("%S\n", Message);
  }
}

bonsai_function void
EatUntil_TrackingDepth(parser *Parser, c_token_type Open, c_token_type Close, c_token *StartToken)
{
  u32 Depth = 0;
  b32 Success = False;
  while ( Remaining(Parser) )
  {
    c_token T = PopToken(Parser);

    if (T.Type == Open)
    {
      ++Depth;
    }

    if (T.Type == Close)
    {
      if (Depth == 0)
      {
        Success = True;
        break;
      }
      --Depth;
    }
  }

  if (!Success)
  {
    RuntimeBreak();
    ParseError(Parser, FormatCountedString(TranArena, CSz("Unable to find closing token %S"), ToString(Close)), StartToken);
  }

  return;
}

bonsai_function void
EatBetween(parser* Parser, c_token_type Open, c_token_type Close)
{
  u32 Depth = 0;
  RequireToken(Parser, Open);

  b32 Success = False;
  while (Remaining(Parser))
  {
    c_token T = PopToken(Parser);

    if (T.Type == Open)
    {
      ++Depth;
    }

    if (T.Type == Close)
    {
      if (Depth == 0)
      {
        Success = True;
        break;
      }
      --Depth;
    }
  }

  if (!Success)
  {
    ParseError(Parser, FormatCountedString(TranArena, CSz("Unable to find closing token %S"), ToString(Close)));
  }

  return;
}

bonsai_function struct_def
StructDef(counted_string Type, counted_string Sourcefile)
{
  struct_def Result = {
    .Type = Type,
    .DefinedInFile = Sourcefile
  };

  return Result;
}

bonsai_function b32
EatStar(parser* Parser)
{
  b32 Result = OptionalToken(Parser, CTokenType_Star);
  return Result;
}

bonsai_function b32
NextTokenIsSpaceOrTab(parser *Parser)
{
  b32 Result = PeekTokenRaw(Parser).Type == CTokenType_Space ||
               PeekTokenRaw(Parser).Type == CTokenType_Tab;
  return Result;
}

bonsai_function void
TrimUntilNewline(parser* Parser)
{
  Assert(Parser->Tokens.At == Parser->Tokens.Start);

  while (NextTokenIsSpaceOrTab(Parser)) { PopTokenRaw(Parser); }
  OptionalTokenRaw(Parser, CTokenType_Newline);
  Parser->Tokens.Start = Parser->Tokens.At;
}

bonsai_function void
TrimFirstToken(parser* Parser, c_token_type TokenType)
{
  Assert(Parser->Tokens.At == Parser->Tokens.Start);
  RequireToken(Parser, TokenType);
  Parser->Tokens.Start = Parser->Tokens.At;
}

bonsai_function void
TrimLastToken(parser* Parser, c_token_type TokenType)
{
  c_token* CurrentToken = Parser->Tokens.End-1;

  while (CurrentToken >= Parser->Tokens.Start)
  {
    if (CurrentToken->Type == TokenType)
    {
      Parser->Tokens.End = CurrentToken;
      break;
    }

    --CurrentToken;
  }
}

bonsai_function parser
GetBodyTextForNextScope(parser* Parser)
{
  // TODO(Jesse, immediate): This should return c_token_cursor
  parser BodyText = *Parser;
  BodyText.OutputTokens = {};

  BodyText.Tokens.Start = BodyText.Tokens.At;
  EatBetween(Parser, CTokenType_OpenBrace, CTokenType_CloseBrace);
  BodyText.Tokens.End = Parser->Tokens.At;

  TrimFirstToken(&BodyText, CTokenType_OpenBrace);
  TrimUntilNewline(&BodyText);
  TrimLastToken(&BodyText, CTokenType_CloseBrace);
  TrimTrailingWhitespace(&BodyText);
  Rewind(&BodyText.Tokens);

  return BodyText;
}

bonsai_function void
ParseReferencesIndirectionAndPossibleFunctionPointerness(parser *Parser, type_spec *Result)
{
  b32 Done = False;
  while (TokensRemain(Parser) && Done == False)
  {
    c_token T = PeekToken(Parser);

    switch (T.Type)
    {
      case CTokenType_OpenParen:
      {
        RequireToken(Parser, CTokenType_OpenParen);
        RequireToken(Parser, CTokenType_Star);
        Result->FunctionPointerTypeName = RequireToken(Parser, CTokenType_Identifier).Value;
        if (PeekToken(Parser).Type == CTokenType_OpenParen)
           { EatBetween(Parser, CTokenType_OpenParen, CTokenType_CloseParen); }
        RequireToken(Parser, CTokenType_CloseParen);
        EatBetween(Parser, CTokenType_OpenParen, CTokenType_CloseParen);
        Result->IsFunctionPointer = True;
        Done = True;
      } break;

      case CTokenType_Ampersand:
      {
        RequireToken(Parser, CTokenType_Ampersand);
        ++Result->ReferenceLevel;
      } break;

      case CTokenType_Star:
      {
        RequireToken(Parser, CTokenType_Star);
        ++Result->IndirectionLevel;
      } break;

      case CTokenType_Const:
      {
        // Ignore const .. for now .. probably forever
        RequireToken(Parser, CTokenType_Const);
      } break;

      case CTokenType_Arrow: // Structs can have members with the same name as a type,
      case CTokenType_Dot:   // so these are valid

      case CTokenType_Comma: // This gets called on macro-functions too, so this is valid
      case CTokenType_CloseParen: // Valid closing token during a cast
      case CTokenType_Identifier:
      case CTokenType_OperatorKeyword: // Finish parsing the return type of an operator funciton
      {
        Done = True;
      } break;

      InvalidDefaultWhileParsing(Parser, CSz("Invalid token while parsing indirection and reference levels."));
    }
  }

  return;
}

bonsai_function b32
IsTypeIdentifier(counted_string TypeName, program_datatypes *Datatypes)
{
  b32 Result = GetDatatypeByName(Datatypes, TypeName).Type != type_datatype_noop;
  return Result;
}

bonsai_function b32
TryAndEatTemplateParameterList(parser *Parser, program_datatypes *Datatypes)
{
  b32 ValidTemplateList = PeekToken(Parser).Type == CTokenType_LT;
  b32 Done = False;
  u32 Lookahead = 1; // Skip the initial <

  while (ValidTemplateList && !Done)
  {
    c_token TemplateParamListTestT = PeekToken(Parser, Lookahead);

    switch (TemplateParamListTestT.Type)
    {
      case CTokenType_Const:
      case CTokenType_Static:
      case CTokenType_Volatile:
      case CTokenType_Long:
      case CTokenType_Unsigned:
      case CTokenType_Signed:
      case CTokenType_Bool:
      case CTokenType_M128:
      case CTokenType_Auto:
      case CTokenType_Void:
      case CTokenType_Double:
      case CTokenType_Float:
      case CTokenType_Char:
      case CTokenType_Int:
      case CTokenType_Short:
      case CTokenType_Comma:
      case CTokenType_Star:
      case CTokenType_Ampersand:
      {
      } break;

      case CTokenType_Identifier:
      {
        if (!IsTypeIdentifier(TemplateParamListTestT.Value, Datatypes))
        {
          ValidTemplateList = False;
          break;
        }
      } break;

      case CTokenType_GT:
      {
        Done = True;
      } break;

      default:
      {
        ValidTemplateList = False;
      } break;

    }

    ++Lookahead;
  }

  if (ValidTemplateList)
  {
    EatBetween(Parser, CTokenType_LT, CTokenType_GT);
  }

  return ValidTemplateList;
}

bonsai_function b32
IsConstructorFunctionName(c_token T)
{
  b32 Result = T.QualifierName && StringsMatch(T.QualifierName->Value, T.Value);
  return Result;
}

bonsai_function type_spec
ParseTypeSpecifier(parse_context *Ctx)
{
  parser *Parser = Ctx->CurrentParser;

  type_spec Result = {};

  b32 Done = False;
  while (!Done)
  {
    c_token T = PeekToken(Parser);

    switch (T.Type)
    {
      case CTokenType_At:
      {
        RequireToken(Parser, CTokenType_At);
        Result.IsMetaTemplateVar = True;
      } break;

      case CTokenType_ThreadLocal:
      {
        RequireToken(Parser, CTokenType_ThreadLocal);
        Result.ThreadLocal = True;
      } break;

      case CTokenType_Const:
      {
        RequireToken(Parser, CTokenType_Const);
        Result.Const = True;
      } break;

      case CTokenType_Static:
      {
        RequireToken(Parser, CTokenType_Static);
        Result.Static = True;
      } break;

      case CTokenType_Volatile:
      {
        RequireToken(Parser, CTokenType_Volatile);
        Result.Volatile = True;
      } break;

      case CTokenType_Signed:
      {
        RequireToken(Parser, CTokenType_Signed);
        Result.Signed = True;
      } break;

      case CTokenType_Unsigned:
      {
        RequireToken(Parser, CTokenType_Unsigned);
        Result.Unsigned = True;
      } break;

      case CTokenType_Enum:
      {
        RequireToken(Parser, CTokenType_Enum);
        Result.Enum = True;
      } break;

      case CTokenType_Struct:
      {
        RequireToken(Parser, CTokenType_Struct);
        Result.Struct = True;
      } break;

      case CTokenType_TemplateKeyword:
      {
        RequireToken(Parser, CTokenType_TemplateKeyword);
        string_from_parser Builder = StartStringFromParser(Parser);
        EatBetween(Parser, CTokenType_LT, CTokenType_GT);
        Result.TemplateSource = FinalizeStringFromParser(&Builder);
      } break;

      case CTokenType_Extern:
      {
        RequireToken(Parser, T.Type);
        Result.Linkage = linkage_extern;
        if ( OptionalToken(Parser, CToken(CTokenType_StringLiteral, CSz("\"C\""))) )
        {
          Result.Linkage = linkage_extern_c;
        }

      } break;

      case CTokenType_Inline:
      {
        // TODO(Jesse id: 282): Should we parse out the bonsai_function def explicitly here?
        RequireToken(Parser, T.Type);
        Result.Inline = True;
      } break;

      case CTokenType_Short:
      {
        RequireToken(Parser, T.Type);
        Result.Short = True;
        if (PeekToken(Parser).Type == CTokenType_Int)
        {
          Result.Token = RequireToken(Parser, CTokenType_Int);
        }
        Done = True;
      } break;

      case CTokenType_Long:
      {
        RequireToken(Parser, T.Type);

        Result.Long = True;

        if (OptionalToken(Parser, CTokenType_Long))
        {
          Result.LongLong = True;
        }

        if (PeekToken(Parser).Type == CTokenType_Int)
        {
          Result.Token = RequireToken(Parser, CTokenType_Int);
        }

        Done = True;
      } break;

      case CTokenType_Double:
      case CTokenType_Float:
      case CTokenType_Char:
      case CTokenType_Int:
      case CTokenType_Void:
      case CTokenType_M128:
      case CTokenType_Bool:
      case CTokenType_Auto:
      case CTokenType_Identifier:
      {
        if (T.Type == CTokenType_Identifier)
        {
          Result.Datatype = GetDatatypeByName(&Ctx->Datatypes, T.Value);
          // TODO(Jesse, id: 296, tags: immediate): When we properly traverse include graphs, this assert should not fail.
          // Assert(Result.Datatype.Type != type_datatype_noop);
        }

        Result.Token = RequireToken(Parser, T.Type);
        Result.HasTemplateArguments = TryAndEatTemplateParameterList(Parser, &Ctx->Datatypes);

        if (IsConstructorFunctionName(Result.Token))
        {
        }
        else
        {
          ParseReferencesIndirectionAndPossibleFunctionPointerness(Parser, &Result);
        }

        Done = True;
      } break;

      InvalidDefaultWhileParsing(Parser, CSz("Invalid token encountered while parsing a type specifier"));
    }

    EatWhitespaceAndComments(Parser);
    continue;
  }

  return Result;
}

bonsai_function ast_node*
ParseInitializerList(parser *Parser, memory_arena *Memory)
{
  ast_node* Result = {};
  ast_node_initializer_list *Node = AllocateAndCastTo(ast_node_initializer_list, &Result, Memory);
  EatBetween(Parser, CTokenType_OpenBrace, CTokenType_CloseBrace);
  return Result;
}

bonsai_function ast_node_expression*
ParseExpression(parse_context *Ctx);

bonsai_function void
ParseExpression(parse_context *Ctx, ast_node** Result);

bonsai_function variable_decl
ParseVariableDecl(parse_context *Ctx)
{
  parser *Parser = Ctx->CurrentParser;

  variable_decl Result = {
    .Type = ParseTypeSpecifier(Ctx),
    .Name = RequireToken(Parser, CTokenType_Identifier).Value,
  };

  if ( PeekToken(Parser).Type == CTokenType_OpenBracket )
  {
    ParseExpression(Ctx, &Result.StaticBufferSize);
  }

  if (OptionalToken(Parser, CTokenType_Equals))
  {
    if (PeekToken(Parser).Type == CTokenType_OpenBrace)
    {
      Result.Value = ParseInitializerList(Ctx->CurrentParser, Ctx->Memory);
    }
    else
    {
      ParseExpression(Ctx, &Result.Value);
    }
  }

  return Result;
}

bonsai_function function_decl
ParseAndPushFunctionPrototype(parse_context *Ctx, type_spec *ReturnType, counted_string *Name, function_type Type)
{
  parser *Parser = Ctx->CurrentParser;

  function_decl Result = {
    .Type = Type,
    .ReturnType = *ReturnType,
    .Name = *Name,
  };

  // Function definition args
  b32 DoneParsingArguments = PeekToken(Parser) == CToken(CTokenType_CloseParen);

  if ( PeekToken(Parser).Type == CTokenType_Void &&
       PeekToken(Parser, 1).Type == CTokenType_CloseParen )
  {
    RequireToken(Parser, CTokenType_Void);
    DoneParsingArguments = True;
  }

  while ( !DoneParsingArguments && TokensRemain(Parser) )
  {
    variable_decl Arg = ParseVariableDecl(Ctx);

    if (!OptionalToken(Parser, CTokenType_Comma))
    {
      DoneParsingArguments = True;
    }

    if (OptionalToken(Parser, CTokenType_Ellipsis))
    {
      Result.IsVariadic = True;
      DoneParsingArguments = True;
    }

    Push(&Result.Args, Arg, Ctx->Memory);
    continue;
  }
  RequireToken(Parser, CTokenType_CloseParen); // Ending paren for arguments

  if (PeekToken(Parser).Type == CTokenType_OpenBrace)
  {
    // void FunctionName( arg A1, arg, A2) { .. body text .. }
    Result.Body = GetBodyTextForNextScope(Parser);
    Push(&Ctx->Datatypes.Functions, Result, Ctx->Memory);
  }
  else
  {
    // void FunctionName( arg A1, arg, A2);
    RequireToken(Parser, CTokenType_Semicolon);
  }

  return Result;
}

bonsai_function struct_def
ParseStructBody(parse_context *Ctx, counted_string StructName);

bonsai_function declaration
ParseFunctionOrVariableDecl(parse_context *Ctx)
{
  parser *Parser = Ctx->CurrentParser;

  declaration Result = {};

  type_spec DeclType = ParseTypeSpecifier(Ctx);

  if (DeclType.IsFunctionPointer)
  {
    RequireToken(Parser, CTokenType_Semicolon);
  }
  else
  {
    if ( OptionalToken(Parser, CTokenType_OperatorKeyword) )
    {
      string_from_parser Builder = StartStringFromParser(Parser);
      RequireOperatorToken(Parser);
      counted_string OperatorName = FinalizeStringFromParser(&Builder);
      if ( OptionalToken(Parser, CTokenType_OpenParen) )
      {
        Result.Type = type_declaration_function_decl;
        Result.function_decl = ParseAndPushFunctionPrototype(Ctx, &DeclType, &OperatorName, function_type_operator);
      }
    }
    else if (IsConstructorFunctionName(DeclType.Token))
    {
      Result.Type = type_declaration_function_decl;
      RequireToken(Parser, CTokenType_OpenParen);
      Result.function_decl = ParseAndPushFunctionPrototype(Ctx, &DeclType, &DeclType.Token.Value, function_type_constructor);
    }
    else
    {
      c_token DeclNameToken = RequireToken(Parser, CTokenType_Identifier);

      macro_def *Macro = GetByName(&Ctx->Datatypes.Macros, DeclNameToken.Value);
      if ((!DeclType.Token.Type) && Macro && Macro->Type == type_macro_function)
      {
        // This is a random hack that'll go away once we have macro-bonsai_function expansion
        // TODO(Jesse id: 321 tags: id_320): Once this path goes away, the assertion with the label associated with this todo id should be put back in.
        EatBetween(Parser, CTokenType_OpenParen, CTokenType_CloseParen);
        OptionalToken(Parser, CTokenType_Semicolon);
      }
      else if ( OptionalToken(Parser, CTokenType_OpenParen) )
      {
        // TODO(Jesse): Should this push happen outside this function?  Probably, because
        // some of the code that calls this function ignores the result which feels weird.
        // ie. They call this function specifically for this side-effect
        Result.Type = type_declaration_function_decl;
        Result.function_decl = ParseAndPushFunctionPrototype(Ctx, &DeclType, &DeclNameToken.Value, function_type_normal);
      }
      else
      {
        Result.Type = type_declaration_variable_decl;
        Result.variable_decl.Type = DeclType;
        Result.variable_decl.Name = DeclNameToken.Value;

        if ( OptionalToken(Parser, CTokenType_OpenBracket) )
        {
          ParseExpression(Ctx, &Result.variable_decl.StaticBufferSize );
          RequireToken(Parser, CTokenType_CloseBracket);
        }

        if ( OptionalToken(Parser, CTokenType_Equals) )
        {
          if (PeekToken(Parser).Type == CTokenType_OpenBrace)
          {
            Result.variable_decl.Value = ParseInitializerList(Parser, Ctx->Memory);
          }
          else
          {
            ParseExpression(Ctx, &Result.variable_decl.Value);
            RequireToken(Parser, CTokenType_Semicolon);
          }
        }
        else if ( OptionalToken(Parser, CTokenType_Semicolon) )
        {
        }
        else
        {
          ParseError_ExpectedSemicolonOrEquals(Parser, PeekTokenPointer(Parser));
        }
      }
    }
  }

  // TODO(Jesse id: 323, tags: id_321)
  // Assert(Result.Type);
  return Result;
}

#define _a_TEST 'a'

#if _a_TEST

#endif

bonsai_function u64
ApplyOperator(parser *Parser, u64 LHS, c_token_type OperatorType, u64 RHS)
{
  u64 Result = 0;
  switch (OperatorType)
  {
    case CTokenType_LT:
    {
      Result = LHS < RHS;
    } break;

    case CTokenType_GT:
    {
      Result = LHS > RHS;
    } break;

    case CTokenType_Plus:
    {
      Result = LHS + RHS;
    } break;

    case CTokenType_Minus:
    {
      Result = LHS - RHS;
    } break;

    case CTokenType_Star:
    {
      Result = LHS * RHS;
    } break;

    case CTokenType_FSlash:
    {
      Result = LHS / RHS;
    } break;

    // TODO(Jesse id: 345): How do we differentiate between arithmetic/logical shifts here?
    case CTokenType_LeftShift:
    {
      if (RHS < 64)
      {
        Result = LHS << RHS;
      }
      else
      {
        ParseError(Parser, CSz("Attempted to left-shift by more than 64 bits!"));
      }
    } break;

    // TODO(Jesse id: 346, tags: id_345)
    case CTokenType_RightShift:
    {
      Result = LHS >> RHS;
    } break;

    case CTokenType_LessEqual:
    {
      Result = LHS <= RHS;
    } break;

    case CTokenType_GreaterEqual:
    {
      Result = LHS >= RHS;
    } break;

    case CTokenType_AreEqual:
    {
      Result = LHS == RHS;
    } break;

    case CTokenType_NotEqual:
    {
      Result = LHS != RHS;
    } break;

    case CTokenType_PlusEquals:
    {
      Result = LHS + RHS;
    } break;

    case CTokenType_MinusEquals:
    {
      Result = LHS - RHS;
    } break;

    case CTokenType_TimesEquals:
    {
      Result = LHS * RHS;
    } break;

    case CTokenType_DivEquals:
    {
      Result = LHS / RHS;
    } break;

    case CTokenType_ModEquals:
    {
      Result = LHS % RHS;
    } break;

    case CTokenType_AndEquals:
    {
      Result = LHS && RHS;
    } break;

    case CTokenType_OrEquals:
    {
      Result = LHS || RHS;
    } break;

    case CTokenType_XorEquals:
    {
      Result = LHS ^ RHS;
    } break;

    case CTokenType_Increment:
    {
      Assert(RHS == 0);
      Result = LHS+1;
    } break;

    case CTokenType_Decrement:
    {
      Assert(RHS == 0);
      Result = LHS - 1;
    } break;

    case CTokenType_LogicalAnd:
    {
      Result = LHS && RHS;
    } break;

    case CTokenType_LogicalOr:
    {
      Result = LHS || RHS;
    } break;

    InvalidDefaultWhileParsing(Parser, CSz("Invalid operator type passed to ApplyOperator."));
  }


  return Result;
}

bonsai_function b32
IsOfHigherPrecedenceThan(c_token_type O1, c_token_type O2)
{
  Warn("IsOfHigherPrecedenceThan is left un-implemented.");
  return False;
}

bonsai_function b32
IsNextTokenMacro(parse_context *Ctx, parser *Parser)
{
  c_token *T = PeekTokenRawPointer(Parser);
  b32 Result = (GetMacroDef(Ctx, T->Value) != 0);
  return Result;
}

// This function is confusing as fuck.  Sorry about that.  It's written
// recursively because when we encounter a macro literal we either had to call
// ResolveMacroConstantExpression again on the resulting expanded parser, or
// track each macro expansion and where we're at in each.  That seemed onerous
// and annoying, so I did it the 'lazy' recursive way.
bonsai_function u64
ResolveMacroConstantExpression(parse_context *Ctx, parser *Parser, memory_arena *Memory, u64 PreviousValue, b32 LogicalNotNextValue)
{
  u64 Result = PreviousValue;

  EatSpacesTabsAndEscapedNewlines(Parser);

  if (RawTokensRemain(Parser))
  {
    c_token T = PeekTokenRaw(Parser); // Has to be raw because newlines delimit the end of a macro expression
    switch (T.Type)
    {
      case CT_MacroLiteral:
      {
        InvalidCodePath();
      } break;

      case CTokenType_Bang:
      {
        RequireTokenRaw(Parser, CTokenType_Bang);
        Result = ResolveMacroConstantExpression(Ctx, Parser, Memory, Result, LogicalNotNextValue ? False : True);
     } break;

      case CTokenType_Identifier:
      {
        if (OptionalTokenRaw(Parser, CToken(CSz("defined"))))
        {
          EatSpacesTabsAndEscapedNewlines(Parser);
          c_token NextToken = PeekTokenRaw(Parser);
          switch (NextToken.Type)
          {
            case CTokenType_Identifier:
            {
              // TODO(Jesse): This path has to set the next token to CT_MacroLiteral
              b32 NextTokenIsMacro = IsNextTokenMacro(Ctx, Parser);
              RequireToken(Parser, CTokenType_Identifier);
              Result = ResolveMacroConstantExpression(Ctx, Parser, Memory, LogicalNotNextValue ? !NextTokenIsMacro : NextTokenIsMacro, False);
            } break;

            case CTokenType_OpenParen:
            {
              u32 ParenCount = 0;
              while (PeekTokenRaw(Parser).Type == CTokenType_OpenParen )
              {
                RequireTokenRaw(Parser, CTokenType_OpenParen);
                EatSpacesTabsAndEscapedNewlines(Parser);
                ++ParenCount;
              }

              // TODO(Jesse): This path has to set the next token to CT_MacroLiteral
              b32 NextTokenIsMacro = IsNextTokenMacro(Ctx, Parser);
              RequireToken(Parser, CTokenType_Identifier);

              while (ParenCount--)
              {
                EatSpacesTabsAndEscapedNewlines(Parser);
                RequireTokenRaw(Parser, CTokenType_CloseParen);
              }

              Result = ResolveMacroConstantExpression(Ctx, Parser, Memory, LogicalNotNextValue ? !NextTokenIsMacro : NextTokenIsMacro, False);
            } break;

            InvalidDefaultCase;
          }
        }
        else
        {
          macro_def *Macro = GetMacroDef(Ctx, T.Value);
          if (Macro)
          {
            c_token *pT = PeekTokenRawPointer(Parser);
            pT->Type = CT_MacroLiteral;
            pT->Macro = Macro;

            parser *Expanded = ExpandMacro(Ctx, Parser, Macro, Memory);
            u64 MacroExpansion = ResolveMacroConstantExpression(Ctx, Expanded, Memory, Result, LogicalNotNextValue);
            Result = ResolveMacroConstantExpression(Ctx, Parser, Memory, MacroExpansion, False);
          }
          else if (!Macro || Macro->Undefed)
          {
            RequireTokenRaw(Parser, CTokenType_Identifier);
            Result = ResolveMacroConstantExpression(Ctx, Parser, Memory, LogicalNotNextValue ? 1 : 0, False);
          }
          else
          {
            InvalidCodePath();
          }
        }
      } break;

      case CTokenType_CharLiteral:
      case CTokenType_IntLiteral:
      {
        RequireTokenRaw(Parser, T.Type);

        u64 ThisValue = LogicalNotNextValue ? !T.UnsignedValue : T.UnsignedValue;
        Result = ResolveMacroConstantExpression(Ctx, Parser, Memory, ThisValue, False);

      } break;

      case CTokenType_OpenParen:
      {
        RequireTokenRaw(Parser, T.Type);
        Result = ResolveMacroConstantExpression(Ctx, Parser, Memory, Result, False);
        if (LogicalNotNextValue)
        {
          Result = !Result;
        }
        Result = ResolveMacroConstantExpression(Ctx, Parser, Memory, Result, False);
        EatSpacesTabsAndEscapedNewlines(Parser);
        RequireTokenRaw(Parser, CTokenType_CloseParen);
      } break;

      case CTokenType_Plus:
      case CTokenType_Minus:
      case CTokenType_Star:
      case CTokenType_FSlash:
      case CTokenType_LT:
      case CTokenType_GT:
      case CTokenType_LeftShift:
      case CTokenType_RightShift:
      case CTokenType_LessEqual:
      case CTokenType_GreaterEqual:
      case CTokenType_AreEqual:
      case CTokenType_NotEqual:
      case CTokenType_PlusEquals:
      case CTokenType_MinusEquals:
      case CTokenType_TimesEquals:
      case CTokenType_DivEquals:
      case CTokenType_ModEquals:
      case CTokenType_AndEquals:
      case CTokenType_OrEquals:
      case CTokenType_XorEquals:
      case CTokenType_Increment:
      case CTokenType_Decrement:
      case CTokenType_LogicalAnd:
      case CTokenType_LogicalOr:
      {
        RequireTokenRaw(Parser, T.Type);
        c_token_type OperatorToApply = T.Type;
        u64 NextValue = ResolveMacroConstantExpression(Ctx, Parser, Memory, Result, False);
        u64 OperationResult = ApplyOperator(Parser, Result, OperatorToApply, NextValue);
        Result = ResolveMacroConstantExpression(Ctx, Parser, Memory, OperationResult, False);
      } break;

      case CTokenType_EscapedNewline:
      {
        RequireTokenRaw(Parser, T.Type);
        Result = ResolveMacroConstantExpression(Ctx, Parser, Memory, Result, LogicalNotNextValue);
      } break;

      case CTokenType_CommentSingleLine:
      case CTokenType_Newline:
      case CTokenType_CloseParen:
      {
        // We're done
      } break;

      default: { ParseError(Parser, CSz("Invalid Token :: ResolveMacroConstantExpression failed."), PeekTokenRawPointer(Parser)); } break;
    }
  }

  return Result;
}

bonsai_function void
EraseToken(c_token *Token)
{
  Token->Erased = True;
}

bonsai_function void
EraseSectionOfParser(parser *Parser, c_token *FirstToErase, c_token *OnePastLastToErase)
{
  b32 E0 = (FirstToErase > OnePastLastToErase);
  b32 E1 = (FirstToErase < Parser->Tokens.Start);
  b32 E2 = (FirstToErase >= Parser->Tokens.End);
  b32 E3 = (OnePastLastToErase <= Parser->Tokens.Start);
  b32 E4 = (OnePastLastToErase > Parser->Tokens.End);

  if (E0 || E1 || E2 || E3 || E4)
  {
    ParseError(Parser, CSz("Invalid token span passed to EraseSectionOfParser"));
  }
  else
  {
    c_token *At = FirstToErase;
    while (At < OnePastLastToErase)
    {
      EraseToken(At++);
    }

  }

  return;
}

bonsai_function c_token *
EatIfBlock(parser *Parser)
{
  c_token *StartToken = PeekTokenRawPointer(Parser);
  c_token *Result = StartToken;

  u32 Depth = 0;
  b32 Success = False;
  while ( RawTokensRemain(Parser) )
  {
    Result = PeekTokenRawPointer(Parser);

    if (Result->Type == CT_PreprocessorIf ||
        Result->Type == CT_PreprocessorIfNotDefined ||
        Result->Type == CT_PreprocessorIfDefined)
    {
      ++Depth;
    }

    if ( Result->Type == CT_PreprocessorElse ||
         Result->Type == CT_PreprocessorElif ||
         Result->Type == CT_PreprocessorEndif )
    {
      if (Depth == 0)
      {
        Success = True;
        break;
      }

      if (Result->Type == CT_PreprocessorEndif)
      {
        --Depth;
      }
    }

    RequireTokenRaw(Parser, Result->Type);
  }

  if (!Success)
  {
    ParseError(Parser, FormatCountedString(TranArena, CSz("Unable to find closing token for %S."), ToString(CT_PreprocessorIf)), StartToken);
  }

  Assert(Result == Parser->Tokens.At);
  return Result;
}

#if 0
bonsai_function counted_string
ParseIfDefinedValue(parser *Parser)
{
  u32 NumOpenParens = 0;
  while (OptionalToken(Parser, CTokenType_OpenParen))
  {
    ++NumOpenParens;
  }

  counted_string Result = RequireToken(Parser, CTokenType_Identifier).Value;

  while (NumOpenParens--)
  {
    RequireToken(Parser, CTokenType_CloseParen);
  }

  return Result;
}
#endif

bonsai_function macro_def *
GetMacroDef(parse_context *Ctx, counted_string DefineValue)
{
  macro_def *Macro = GetByName(&Ctx->Datatypes.Macros, DefineValue);

  macro_def *Result = 0;
  if (Macro && !Macro->Undefed)
  {
    Result = Macro;
  }
  return Result;
}

#if 0
bonsai_function macro_def *
GetMacroDef(parse_context *Ctx, counted_string *DefineValue)
{
  macro_def *Result = GetMacroDef(Ctx, *DefineValue);
  return Result;
}
#endif

bonsai_function struct_member
ParseStructMember(parse_context *Ctx, counted_string StructName)
{
  TIMED_FUNCTION();
  parser *Parser = Ctx->CurrentParser;

  struct_member Result = {};

  b32 Continue = False;
  do {
    Continue = False;

    c_token T = PeekToken(Parser);
    switch(T.Type)
    {
      case CTokenType_Tilde:
      {
        RequireToken(Parser, CTokenType_Tilde);

        if (StringsMatch(StructName, PeekToken(Parser).Value) &&
             PeekToken(Parser, 1).Type == CTokenType_OpenParen)
        {
          RequireToken(Parser, CTokenType_Identifier);
          RequireToken(Parser, CTokenType_OpenParen);

          Result.Type = type_function_decl;
          type_spec ReturnType = {};
          Result.function_decl = ParseAndPushFunctionPrototype(Ctx, &ReturnType, &StructName, function_type_destructor);
        }
        else
        {
          ParseError(Parser, CSz("Destructor name must match the struct name."));
        }
      } break;

      case CTokenType_Union:
      {
        RequireToken(Parser, CTokenType_Union);
        Result.Type = type_struct_member_anonymous;
        Result.struct_member_anonymous.Body = ParseStructBody(Ctx, CS("anonymous_union"));
        RequireToken(Parser, CTokenType_Semicolon);
      } break;

      case CTokenType_Struct:
      {
        RequireToken(Parser, CTokenType_Struct);
        Result.Type = type_struct_member_anonymous;
        Result.struct_member_anonymous.Body = ParseStructBody(Ctx, CS("anonymous_struct"));
        RequireToken(Parser, CTokenType_Semicolon);
      } break;

      case CTokenType_ThreadLocal:
      case CTokenType_Const:
      case CTokenType_Static:
      case CTokenType_Volatile:
      case CTokenType_Void:
      case CTokenType_Long:
      case CTokenType_M128:
      case CTokenType_Bool:
      case CTokenType_Auto:
      case CTokenType_Double:
      case CTokenType_Float:
      case CTokenType_Char:
      case CTokenType_Int:
      case CTokenType_Short:
      case CTokenType_Unsigned:
      case CTokenType_Signed:
      case CTokenType_Identifier:
      {
        u32 DefKeywordsEncountered = 0;
        if ( StringsMatch(StructName, T.Value) && PeekToken(Parser, 1).Type == CTokenType_OpenParen)
        {
          // Constructor bonsai_function
          RequireToken(Parser, CTokenType_Identifier);
          RequireToken(Parser, CTokenType_OpenParen);

          Result.Type = type_function_decl;
          type_spec ReturnType = {};
          Result.function_decl = ParseAndPushFunctionPrototype(Ctx, &ReturnType, &StructName, function_type_constructor);
        }
        else
        {
          declaration Decl = ParseFunctionOrVariableDecl(Ctx);
          switch (Decl.Type)
          {
            case type_declaration_variable_decl:
            {
              Result.Type = type_variable_decl;
              Result.variable_decl = Decl.variable_decl;
            } break;

            case type_declaration_function_decl:
            {
              Result.Type = type_function_decl;
              Result.function_decl = Decl.function_decl;
            } break;

            case type_declaration_noop:
            {
              InvalidCodePath();
            } break;
          }
        }

      } break;

      case CTokenType_CloseBrace:
      {
        // Done parsing struct members
      } break;

      InvalidDefaultWhileParsing(Parser, CSz("While parsing struct member."));
    }
  } while (Continue);

  return Result;
}

bonsai_function counted_string
ConcatTokensUntil(parser* Parser, c_token_type Close, memory_arena* Memory)
{
  // TODO(Jesse  id: 225, tags: todos, easy): Rewrite with string_from_parser
  string_builder Builder = {};
  while (Remaining(Parser) && PeekTokenRaw(Parser).Type != Close)
  {
    Append(&Builder, PopTokenRaw(Parser).Value);
  }
  counted_string Result = Finalize(&Builder, Memory);
  return Result;
}

bonsai_function counted_string
ConcatTokensUntilNewline(parser* Parser, memory_arena* Memory)
{
  counted_string Result = ConcatTokensUntil(Parser, CTokenType_Newline, Memory);
  return Result;
}

bonsai_function struct_member_stream
MembersOfType(struct_def* Struct, counted_string MemberType, memory_arena *Memory)
{
  struct_member_stream Result = {};
  if (MemberType.Start)
  {
    ITERATE_OVER(&Struct->Members)
    {
      struct_member *Member = GET_ELEMENT(Iter);
      switch (Member->Type)
      {
        case type_variable_decl:
        {
          if (StringsMatch(Member->variable_decl.Type.Token.Value, MemberType))
          {
            Push(&Result, *Member, Memory);
          }
        } break;

        InvalidDefaultCase;
      }
    }
  }

  return Result;
}

// TODO(Jesse id: 299): This could be improved by not taking the StructName, and
// filling it out internally.  It would have to check where the struct name is
//
// ie. at the start 'struct foo { ... };'
//
// or at the end 'typedef struct { ... } foo;
//
// This would clean up the calling code quite a bit and get rid of a
// bunch of redundant RequireTokens on Semicolons.

bonsai_function struct_def
ParseStructBody(parse_context *Ctx, counted_string StructName)
{
  TIMED_FUNCTION();
  parser *Parser = Ctx->CurrentParser;
  struct_def Result = StructDef(StructName, Parser->Filename);

  RequireToken(Parser, CTokenType_OpenBrace);

  struct_member Declaration = {};
  for (;;)
  {
    Declaration = ParseStructMember(Ctx, Result.Type);
    if (Declaration.Type == type_struct_member_noop)
    {
      break;
    }
    else
    {
      Push(&Result.Members, Declaration, Ctx->Memory);
    }
  }

  RequireToken(Parser, CTokenType_CloseBrace);

  return Result;
}

bonsai_function counted_string
ParseIntegerConstant(parser* Parser)
{
  c_token T = PeekToken(Parser);

  const char* Start = T.Value.Start;
  u32 Count = 0;

  if (T.Type == CTokenType_Minus)
  {
    ++Count;
    RequireToken(Parser, CTokenType_Minus);
  }

  c_token NumberT = RequireToken(Parser, CTokenType_Identifier);
  if (IsNumeric(NumberT.Value))
  {
    Count += NumberT.Value.Count;
  }
  else
  {
    ParseError(Parser, CSz("Expected integer constant expression."));
  }

  counted_string Result = {
    .Count = Count,
    .Start = Start,
  };

  return Result;
}

bonsai_function enum_def
ParseEnum(parse_context *Ctx)
{
  TIMED_FUNCTION();

  parser *Parser = Ctx->CurrentParser;
  program_datatypes *Datatypes = &Ctx->Datatypes;

  counted_string EnumName = RequireToken(Parser, CTokenType_Identifier).Value;

  enum_def Enum = {
    .Name = EnumName
  };

  RequireToken(Parser, CTokenType_OpenBrace);

  b32 Done = False;
  while (!Done && TokensRemain(Parser))
  {
    enum_member Field = {};
    Field.Name = RequireToken(Parser, CTokenType_Identifier).Value;

    if (OptionalToken(Parser, CTokenType_Equals))
    {
      Field.Value = ParseExpression(Ctx);
    }

    Push(&Enum.Members, Field, Ctx->Memory);

    if(OptionalToken(Parser, CTokenType_Comma))
    {
      if (OptionalToken(Parser, CTokenType_CloseBrace))
      {
        Done = True;
      }
    }
    else
    {
      RequireToken(Parser, CTokenType_CloseBrace);
      Done = True;
    }
  }

  return Enum;
}

bonsai_function b32
IsTypeQualifier(c_token T)
{
  b32 Result = False;
  switch (T.Type)
  {
    case CTokenType_LT:
    case CTokenType_Ampersand:
    case CTokenType_Star:
    { Result = True; } break;

    default : {} break;
  }

  return Result;
}

bonsai_function void
OptionalPrefixOperator(parser *Parser)
{
  c_token T = PeekToken(Parser);
  if ( T.Type == CTokenType_Increment ||
       T.Type == CTokenType_Decrement ||
       T.Type == CTokenType_Ampersand ||
       T.Type == CTokenType_Star
       )
  {
    PopToken(Parser);
  }
}

bonsai_function void
OptionalPostfixOperator(parser *Parser)
{
  c_token T = PeekToken(Parser);
  if ( T.Type == CTokenType_Increment ||
       T.Type == CTokenType_Decrement)
  {
    PopToken(Parser);
  }
}

bonsai_function void
ParseDatatypeDef(parse_context *Ctx)
{
  parser *Parser = Ctx->CurrentParser;
  c_token TypeSpecifier = PopToken(Parser);

  switch(TypeSpecifier.Type)
  {
    case CTokenType_Enum:
    {
      enum_def Enum = ParseEnum(Ctx);
      Push(&Ctx->Datatypes.Enums, Enum, Ctx->Memory);
    } break;

    case CTokenType_Struct:
    {
      counted_string StructName = RequireToken(Parser, CTokenType_Identifier).Value;
      if ( PeekToken(Parser).Type == CTokenType_OpenBrace )
      {
        struct_def S = ParseStructBody(Ctx, StructName);
        Push(&Ctx->Datatypes.Structs, S, Ctx->Memory);
      }
    } break;

    case CTokenType_Union:
    {
      counted_string UnionName = RequireToken(Parser, CTokenType_Identifier).Value;
      struct_def S = ParseStructBody(Ctx, UnionName);
      S.IsUnion = True;
      Push(&Ctx->Datatypes.Structs, S, Ctx->Memory);
    } break;

    InvalidDefaultWhileParsing(Parser, CSz("Tried parsing a datatypes that wasn't a struct, enum or union!"));
  }

  RequireToken(Parser, CTokenType_Semicolon);
}

bonsai_function void
ParseAndPushTypedef(parse_context *Ctx)
{
  parser *Parser = Ctx->CurrentParser;

  type_spec Type = ParseTypeSpecifier(Ctx);
  counted_string  Alias = {};

  if (Type.IsFunctionPointer)
  {
    Alias = Type.FunctionPointerTypeName;
  }
  else
  {
    Alias = RequireToken(Parser, CTokenType_Identifier).Value;
  }
  RequireToken(Parser, CTokenType_Semicolon);

  type_def Typedef = {
    .Type = Type,
    .Alias = Alias,
  };

  Push(&Ctx->Datatypes.Typedefs, Typedef, Ctx->Memory);
}

bonsai_function void
ParseTypedef(parse_context *Ctx)
{
  parser *Parser = Ctx->CurrentParser;

  RequireToken(Parser, CTokenType_Typedef);

  if ( OptionalToken(Parser, CTokenType_Struct) )
  {
    if (PeekToken(Parser).Type == CTokenType_OpenBrace)
    {
      struct_def S = ParseStructBody(Ctx, CS(""));
      S.Type = RequireToken(Parser, CTokenType_Identifier).Value;
      RequireToken(Parser, CTokenType_Semicolon);
      Push(&Ctx->Datatypes.Structs, S, Ctx->Memory);
    }
    else
    {
      ParseAndPushTypedef(Ctx);
    }
  }
  else
  {
    ParseAndPushTypedef(Ctx);
  }

  return;
}

bonsai_function function_decl*
GetByName(counted_string Name, function_decl_stream* Stream)
{
  function_decl *Result = {};
  ITERATE_OVER(Stream)
  {
    auto *Current = GET_ELEMENT(Iter);
    if (StringsMatch(Current->Name, Name))
    {
      Result = Current;
      break;
    }
  }

  return Result;
}

bonsai_function ast_node_variable_def*
GetByTypeName(counted_string Name, ast_node_variable_def_stream* Stream)
{
  ast_node_variable_def* Result = {};
  ITERATE_OVER(Stream)
  {
    ast_node_variable_def* Current = GET_ELEMENT(Iter);
    if (StringsMatch(Current->Decl.Name, Name))
    {
      Result = Current;
      break;
    }
  }
  return Result;
}

bonsai_function ast_node *
ParseFunctionArgument(parser *Parser, memory_arena *Memory, function_decl_stream *FunctionPrototypes);

bonsai_function ast_node_statement *
ParseAllStatements(parse_context *Ctx);

bonsai_function void
ParseSingleStatement(parse_context *Ctx, ast_node_statement *Result)
{
  parser *Parser = Ctx->CurrentParser;

  b32 Done = False;
  while (!Done && TokensRemain(Parser))
  {
    const c_token T = PeekToken(Parser);
    switch (T.Type)
    {
      case CTokenType_IntLiteral:
      case CTokenType_DoubleLiteral:
      case CTokenType_FloatLiteral:
      {
        Result->LHS = ParseExpression(Ctx);
      } break;

      case CTokenType_Identifier:
      {
        Assert(!Result->LHS);
        Result->LHS = ParseExpression(Ctx);
      } break;

      case CTokenType_Equals:
      {
        RequireToken(Parser, T.Type);
        if (PeekToken(Parser).Type == CTokenType_OpenBrace)
        {
          // Initializer List
          EatBetween(Parser, CTokenType_OpenBrace, CTokenType_CloseBrace);
        }
        else
        {
          Assert(!Result->RHS);
          Result->RHS = AllocateProtection(ast_node_statement, Ctx->Memory, 1, False);
          ParseSingleStatement(Ctx, Result->RHS);
        }
        Done = True;
      } break;

      case CTokenType_For:
      {
        RequireToken(Parser, T.Type);
#if 0
        RequireToken(Parser, CTokenType_OpenParen);
        Result->Next = AllocateProtection(ast_node_statement, Memory, 1, False);
        ParseSingleStatement(Parser, Memory, Datatypes, Result->Next);
        ParseExpression(Ctx);
        RequireToken(Parser, CTokenType_Semicolon);
        ParseExpression(Ctx);
        RequireToken(Parser, CTokenType_CloseParen);
#else
        EatBetween(Parser, CTokenType_OpenParen, CTokenType_CloseParen);
#endif
        Done = True;
      } break;

      case CTokenType_If:
      case CTokenType_While:
      case CTokenType_Switch:
      {
        RequireToken(Parser, T.Type);
        Result->LHS = ParseExpression(Ctx);
        Done = True;
      } break;

      case CTokenType_Case:
      {
        RequireToken(Parser, CTokenType_Case);
        Result->LHS = ParseExpression(Ctx);
        RequireToken(Parser, CTokenType_Colon);
        Done = True;
      } break;

      case CTokenType_Default:
      {
        RequireToken(Parser, CTokenType_Default);
        RequireToken(Parser, CTokenType_Colon);
      } break;

      // What do we do with these?
      case CTokenType_Else:
      case CTokenType_Continue:
      {
        RequireToken(Parser, T.Type);
      } break;

      case CTokenType_Return:
      {
        RequireToken(Parser, T.Type);
        Result->LHS = ParseExpression(Ctx);
      } break;

      case CTokenType_OpenBrace:
      {
        RequireToken(Parser, CTokenType_OpenBrace);
        Result->RHS = ParseAllStatements(Ctx);
        Done = True;
      } break;

      case CTokenType_ThreadLocal:
      case CTokenType_Const:
      case CTokenType_Static:
      case CTokenType_Volatile:
      case CTokenType_Void:
      case CTokenType_M128:
      case CTokenType_Bool:
      case CTokenType_Auto:
      case CTokenType_Double:
      case CTokenType_Float:
      case CTokenType_Char:
      case CTokenType_Int:
      case CTokenType_Short:
      case CTokenType_Unsigned:
      case CTokenType_Signed:

      case CTokenType_CharLiteral:
      case CTokenType_StringLiteral:

      case CTokenType_Minus:
      case CTokenType_LeftShift:
      case CTokenType_RightShift:
      case CTokenType_Increment:
      case CTokenType_Decrement:

      case CTokenType_Tilde:
      case CTokenType_Bang:
      case CTokenType_Ampersand:
      case CTokenType_Star:
      case CTokenType_OpenParen:
      {
        if (Result->LHS)
        {
          DebugPrint(Result);
          ParseError(Parser, CSz(""));
          RuntimeBreak();
        }

        Result->LHS = ParseExpression(Ctx);
        Done = True;
      } break;

      case CTokenType_Colon:
      case CTokenType_CloseParen:
      case CTokenType_Break:
      case CTokenType_CloseBrace:
      case CTokenType_Semicolon:
      {
        RequireToken(Parser, T.Type);
        Done = True;
      } break;

      case CTokenType_Goto:
      {
        RequireToken(Parser, T.Type);
        RequireToken(Parser, CTokenType_Identifier);
      } break;

      case CTokenType_Meta:
      {
        RequireToken(Parser, T.Type);
        EatBetween(Parser, CTokenType_OpenParen, CTokenType_CloseParen);
      } break;

      case CTokenType_Asm:
      {
        RequireToken(Parser, T.Type);
        OptionalToken(Parser, CTokenType_Volatile);
        EatBetween(Parser, CTokenType_OpenParen, CTokenType_CloseParen);
      } break;

      InvalidDefaultWhileParsing(Parser, CSz("Invalid token encountered while parsing statement."));
    }
  }

}

bonsai_function ast_node_statement*
ParseSingleStatement(parse_context *Ctx)
{
  ast_node_statement *Result = AllocateProtection(ast_node_statement, Ctx->Memory, 1, False);
  ParseSingleStatement(Ctx, Result);
  return Result;
}

bonsai_function ast_node_statement*
ParseAllStatements(parse_context *Ctx)
{
  ast_node_statement *Result = 0;

  ast_node_statement **Current = &Result;

  while ( TokensRemain(Ctx->CurrentParser) )
  {
    Assert(*Current == 0);

    *Current = ParseSingleStatement(Ctx);
    while (*Current) { Current = &(*Current)->Next; } // Walk to the end of the statement chain.
  }

  return Result;
}

bonsai_function ast_node*
ParseFunctionCall(parse_context *Ctx, counted_string FunctionName);

bonsai_function void
ParseTypeSpecifierNode(parse_context *Ctx, ast_node_expression *Result, datatype *Data = 0)
{
  ast_node_type_specifier *Node = AllocateAndCastTo(ast_node_type_specifier, &Result->Value, Ctx->Memory);

  Node->TypeSpec = ParseTypeSpecifier(Ctx);

  if (Data)
  {
    Node->Datatype = *Data;
    Assert(Node->Datatype.Type != type_datatype_noop);
  }
  else
  {
    if (Node->TypeSpec.Token.Type == CTokenType_Identifier)
    {
      Node->Datatype = GetDatatypeByName(&Ctx->Datatypes, Node->TypeSpec.Token.Value);
      if (Node->Datatype.Type == type_datatype_noop)
      {
        // TODO(Jesse id: 319, tags: id_320): Type-checking failed.
        //
        // This currently actually fails for stuff like std::vector that we
        // don't parse out definitions for .. what should be the strategy for
        // handling this stuff?  There's also a minor hack that can be cleaned
        // up at id: 320 when this is decided
      }
    }
    else
    {
      //
      // Primitive type .. whadda we do?
      //

      /* primitive_def TempDatatype = GetPrimitiveType(&Datatypes->Primitives, Node->TypeSpec); */
    }
  }

  Node->Name = ParseExpression(Ctx);

  if (Node->Name->Value)
  {
    switch (Node->Name->Value->Type)
    {
      case type_ast_node_function_call: // Constructor bonsai_function
      case type_ast_node_symbol:        // Regular variable definition
      {
      } break;

      // This case should go away once we can check what local varaibles are defined for the scope we're parsing
      case type_ast_node_access:
      {
        Error("BUG during ParseTypeSpecifierNode");
      } break;

      InvalidDefaultWhileParsing(Ctx->CurrentParser, CSz("Invalid syntax following type-specifier."));
    }
  }
  else
  {
    // Cast or macro bonsai_function call
  }
}

bonsai_function b32
ParseSymbol(parse_context *Ctx, ast_node_expression* Result, b32 SymbolIsNeverTypeSpecifier = True)
{
  parser *Parser = Ctx->CurrentParser;

  b32 PushedParser = False;

  c_token T = PeekToken(Parser);
  Assert(T.Type == CTokenType_Identifier);

#if 0
  macro_def *Macro = GetByName(&Ctx->Datatypes.Macros, T.Value);
  if (Macro)
  {
    c_token MacroNameToken = RequireToken(Parser, CTokenType_Identifier);
    switch (Macro->Type)
    {
      case type_macro_keyword:
      {
        PushParser(Parser, Macro->Parser, parser_push_type_macro);
        PushedParser = True;
      } break;

      case type_macro_function:
      {
        if (PeekToken(Parser).Type == CTokenType_OpenParen)
        {
          Assert(Result->Value == 0);
          Result->Value = ParseFunctionCall(Ctx, MacroNameToken.Value);
        }
        else
        {
          ast_node_symbol *Node = AllocateAndCastTo(ast_node_symbol, &Result->Value, Ctx->Memory);
          Node->Token = MacroNameToken;
        }
      } break;

      case type_macro_noop: { InvalidCodePath(); } break;
    }

    return PushedParser;
  }
#endif

  if ( PeekToken(Parser, 1).Type  == CTokenType_OpenParen )
  {
    c_token SymbolToken = RequireToken(Parser, CTokenType_Identifier);
    Assert(!Result->Value);
    Result->Value = ParseFunctionCall(Ctx, SymbolToken.Value);
  }
  else
  {
    datatype TestDatatype = GetDatatypeByName(&Ctx->Datatypes, T.Value);
    if ( SymbolIsNeverTypeSpecifier || TestDatatype.Type == type_datatype_noop )
    {
      ast_node_symbol *Node = AllocateAndCastTo(ast_node_symbol, &Result->Value, Ctx->Memory);
      Node->Token = RequireToken(Parser, CTokenType_Identifier);
    }
    else
    {
      ParseTypeSpecifierNode(Ctx, Result, &TestDatatype);
    }
  }

  return PushedParser;
}

bonsai_function void
ParseExpression(parse_context *Ctx, ast_node_expression* Result)
{
  parser *Parser = Ctx->CurrentParser;

  b32 ParsePushedParser = False;
  do
  {
    ParsePushedParser = False;
    const c_token T = PeekToken(Parser);
    switch (T.Type)
    {
      case CTokenType_Meta:
      {
        RequireToken(Parser, CTokenType_Meta);
        EatBetween(Parser, CTokenType_OpenParen, CTokenType_CloseParen);
      } break;

      case CTokenType_IntLiteral:
      case CTokenType_DoubleLiteral:
      case CTokenType_FloatLiteral:
      {
        ast_node_literal *Node = AllocateAndCastTo(ast_node_literal, &Result->Value, Ctx->Memory);
        Node->Token = RequireToken(Parser, T.Type);
      } break;

      case CTokenType_Identifier:
      {
        ParsePushedParser = ParseSymbol(Ctx, Result, False);
      } break;

      case CTokenType_Arrow:
      case CTokenType_Dot:
      {
        ast_node_access *Node = AllocateAndCastTo(ast_node_access, &Result->Value, Ctx->Memory);
        Node->AccessType = RequireToken(Parser, T.Type);

        Node->Symbol = AllocateProtection(ast_node_expression, Ctx->Memory, 1, False);
        ParsePushedParser = ParseSymbol(Ctx, Node->Symbol);
      } break;

      case CTokenType_ThreadLocal:
      case CTokenType_Const:
      case CTokenType_Static:
      case CTokenType_Volatile:
      case CTokenType_Void:
      case CTokenType_Long:
      case CTokenType_M128:
      case CTokenType_Bool:
      case CTokenType_Auto:
      case CTokenType_Double:
      case CTokenType_Float:
      case CTokenType_Char:
      case CTokenType_Int:
      case CTokenType_Short:
      case CTokenType_Unsigned:
      case CTokenType_Signed:
      {
        ParseTypeSpecifierNode(Ctx, Result);
      } break;

      case CTokenType_OpenParen:
      {
        RequireToken(Parser, T.Type);
        ast_node_parenthesized *Node = AllocateAndCastTo(ast_node_parenthesized, &Result->Value, Ctx->Memory);
        Node->Expr = ParseExpression(Ctx);
        RequireToken(Parser, CTokenType_CloseParen);

        if (Node->Expr->Value->Type == type_ast_node_type_specifier)
        {
          Node->IsCast = True;
          Node->CastValue = ParseExpression(Ctx);
        }

      } break;

      case CTokenType_Ampersand:
      case CTokenType_Tilde:
      case CTokenType_Hat:
      case CTokenType_LT:
      case CTokenType_GT:
      case CTokenType_Bang:
      case CTokenType_LeftShift:
      case CTokenType_RightShift:
      case CTokenType_LessEqual:
      case CTokenType_GreaterEqual:
      case CTokenType_AreEqual:
      case CTokenType_NotEqual:
      case CTokenType_PlusEquals:
      case CTokenType_MinusEquals:
      case CTokenType_TimesEquals:
      case CTokenType_DivEquals:
      case CTokenType_ModEquals:
      case CTokenType_AndEquals:
      case CTokenType_OrEquals:
      case CTokenType_XorEquals:
      case CTokenType_Increment:
      case CTokenType_Decrement:
      case CTokenType_LogicalAnd:
      case CTokenType_LogicalOr:
      case CTokenType_Percent:
      case CTokenType_Pipe:
      case CTokenType_Plus:
      case CTokenType_Minus:
      case CTokenType_FSlash:
      case CTokenType_Star:
      case CTokenType_OpenBracket:
      {
        Assert(!Result->Value);
        ast_node_operator* Node = AllocateAndCastTo(ast_node_operator, &Result->Value, Ctx->Memory);
        Node->Token = RequireToken(Parser, T.Type);
        Node->Operand = ParseExpression(Ctx);
        if (T.Type == CTokenType_OpenBracket)
          { RequireToken(Parser, CTokenType_CloseBracket); }
      } break;

      case CTokenType_Question:
      {
        ast_node_operator* Node = AllocateAndCastTo(ast_node_operator, &Result->Value, Ctx->Memory);
        Node->Token = RequireToken(Parser, T.Type);
        Node->Operand = ParseExpression(Ctx);
        RequireToken(Parser, CTokenType_Colon);
        ParseExpression(Ctx); // TODO(Jesse id: 260): Currently ignoring the second half of a ternary .. we should probably not do this
      } break;

      case CTokenType_CharLiteral:
      case CTokenType_StringLiteral:
      {
        Assert(!Result->Value);
        ast_node_literal *Node = AllocateAndCastTo(ast_node_literal, &Result->Value, Ctx->Memory);
        Node->Token = RequireToken(Parser, T.Type);

        while ( PeekToken(Parser).Type == CTokenType_StringLiteral ||
                PeekToken(Parser).Type == CTokenType_Identifier )
        {
          c_token NextT = PopToken(Parser);
          switch (NextT.Type)
          {
            case CTokenType_StringLiteral:
            {
              Node->Token = CToken(CTokenType_StringLiteral, Concat(Node->Token.Value, NextT.Value, Ctx->Memory)); // TODO(Jesse, id: 264, tags: memory_leak): This should use a string builder
            } break;

            case CTokenType_Identifier:
            {
              // TODO(Jesse id: 264): Once we have proper macro expansion, this can be expanded and concatenated to the string as well.
              macro_def *Macro = GetByName(&Ctx->Datatypes.Macros, NextT.Value);
              switch(Macro->Type)
              {
                case type_macro_keyword:
                {
                } break;

                case type_macro_function:
                {
                  EatBetween(Parser, CTokenType_OpenParen, CTokenType_CloseParen);
                } break;

                case type_macro_noop: { InvalidCodePath(); } break;
              }
            } break;

            InvalidDefaultWhileParsing(Parser, CSz("Expected macro identifier or string literal."));
          }
        }
      } break;

      case CTokenType_Colon:
      case CTokenType_Semicolon:
      case CTokenType_Comma:
      case CTokenType_CloseBracket:
      case CTokenType_CloseParen:
      case CTokenType_Equals:
      {
      } break;

      InvalidDefaultWhileParsing(Parser, CSz("Invalid token while parsing expression."));
    }
  } while (ParsePushedParser);

  if (NextTokenIsOperator(Parser))
  {
    Assert(!Result->Next);
    Result->Next = ParseExpression(Ctx);
  }
}

// TODO(Jesse id: 316): These paths are exactly the same .. they should probably use the same API ..?
bonsai_function ast_node_expression*
ParseExpression(parse_context *Ctx)
{
  ast_node_expression *Result = AllocateProtection(ast_node_expression, Ctx->Memory, 1, False);
  ParseExpression(Ctx, Result);
  return Result;
}

bonsai_function void
ParseExpression(parse_context *Ctx, ast_node **Result)
{
  ast_node_expression *Node = AllocateAndCastTo(ast_node_expression, Result, Ctx->Memory);
  ParseExpression(Ctx, Node);
}

#if 0
bonsai_function type_spec
ReduceToTypeSpec(ast_node* InputNode, ast_node_variable_def_stream *Locals)
{
  type_spec Result = {};

  ast_node* Current = InputNode;

  while (Current)
  {
    switch (Current->Type)
    {
      InvalidDefaultCase;

      case type_ast_node_symbol:
      {
        ast_node_symbol *Symbol = SafeCast(ast_node_symbol, Current);
        Assert(Symbol->Token.Type == CTokenType_Identifier);
        ast_node_variable_def *LocalVarDecl = GetByTypeName(Symbol->Token.Value, Locals);

        if (ResultType->Name.Count)
        {
          if (LocalVarDecl)
          {
            if (StringsMatch(ResultType->Name, LocalVarDecl->Decl.Type.Name))
            {
            }
            else
            {
              Error("Type mismatch");
              RuntimeBreak();
            }
          }
          else
          {
            Error("Unable to find local variable decl");
          }
        }
        else
        {
          if (LocalVarDecl)
          {
            ResultType->Name = LocalVarDecl->Decl.Type.Name;
          }
          else
          {
            Error("Unable to find local variable decl");
          }
        }

      } break;

      case type_ast_node_function_call:
      {
        ast_node_function_call *Node = SafeCast(ast_node_function_call, Current);
      } return;

      case type_ast_node_scope:
      {
      } break;

      case type_ast_node_ignored:
      {
      } break;
    }
  }

  return Result;
}
#endif

bonsai_function ast_node*
ParseFunctionCall(parse_context *Ctx, counted_string FunctionName)
{
  parser *Parser = Ctx->CurrentParser;

  ast_node *Result = {};
  ast_node_function_call *Node = AllocateAndCastTo(ast_node_function_call, &Result, Ctx->Memory);

  Node->Name = FunctionName;
  Node->Prototype = GetByName(Node->Name, &Ctx->Datatypes.Functions);

  RequireToken(Parser, CTokenType_OpenParen);

  b32 Done = False;
  while (!Done)
  {
    ast_node_expression Arg = {};
    ParseExpression(Ctx, &Arg);
    Push(&Node->Args, Arg, Ctx->Memory);

    if(OptionalToken(Parser, CTokenType_Comma))
    {
    }
    else
    {
      RequireToken(Parser, CTokenType_CloseParen);
      Done = True;
    }
  }


  return Result;
}

bonsai_function void
ParseDatatypes(parse_context *Ctx)
{
  parser *Parser = Ctx->CurrentParser;
  program_datatypes* Datatypes = &Ctx->Datatypes;
  memory_arena* Memory = Ctx->Memory;

  while (TokensRemain(Parser))
  {
    c_token T = PeekToken(Parser);

    switch(T.Type)
    {
      case CT_KeywordPragma:
      case CTokenType_Meta:
      {
        RequireToken(Parser, T.Type);
        EatBetween(Parser, CTokenType_OpenParen, CTokenType_CloseParen);
      } break;

      case CT_StaticAssert:
      {
        RequireToken(Parser, CT_StaticAssert);
        EatBetween(Parser, CTokenType_OpenParen, CTokenType_CloseParen);
        RequireToken(Parser, CTokenType_Semicolon);
        /* EraseSectionOfParser(Result, T, Result->Tokens.At); */
      } break;


      case CTokenType_Semicolon:
      {
        RequireToken(Parser, CTokenType_Semicolon);
      } break;

      case CTokenType_Struct:
      case CTokenType_Enum:
      case CTokenType_Union:
      {
        ParseDatatypeDef(Ctx);
      } break;

      case CTokenType_Typedef:
      {
        ParseTypedef(Ctx);
      } break;

      case CTokenType_Using:
      {
        EatUntilIncluding(Parser, CTokenType_Semicolon);
      } break;

      case CTokenType_TemplateKeyword:
      case CTokenType_Extern:
      case CTokenType_Inline:
      case CTokenType_ThreadLocal:
      case CTokenType_Const:
      case CTokenType_Static:
      case CTokenType_Volatile:
      case CTokenType_Void:
      case CTokenType_Long:
      case CTokenType_M128:
      case CTokenType_Bool:
      case CTokenType_Auto:
      case CTokenType_Double:
      case CTokenType_Float:
      case CTokenType_Char:
      case CTokenType_Int:
      case CTokenType_Short:
      case CTokenType_Unsigned:
      case CTokenType_Signed:
      case CTokenType_Identifier:
      {
        // We ignore the result of this.  We're just looking to push functions
        // and push them onto the program_datatypes stream which this function
        // does internally.  Maybe we should change that?
        ParseFunctionOrVariableDecl(Ctx);

      } break;

      InvalidDefaultWhileParsing(Parser, CSz("Invalid token encountered while parsing a datatype."));
    }

    EatWhitespaceAndComments(Parser);
    continue;
  }

  return;
}

bonsai_function parser_cursor
AllocateTokenizedFiles(u32 Count, memory_arena* Memory)
{
  parser* Start = Allocate(parser, Memory, Count);
  parser_cursor Result = {
    .Start = Start,
    .At = Start,
    .End = Start + Count,
  };

  return Result;
}

bonsai_function void
GoGoGadgetMetaprogramming(parse_context* Ctx, todo_list_info* TodoInfo);

bonsai_function void
FlushOutputToDisk(parse_context *Ctx, counted_string OutputForThisParser, counted_string NewFilename, todo_list_info* TodoInfo, memory_arena* Memory, b32 IsInlineCode = False)
{
  TIMED_FUNCTION();

  parser *Parser = Ctx->CurrentParser;

  if (!Parser->Valid)
  {
    Error(CSz("Parse Error Encountered, not flushing to disk."));
    return;
  }

  counted_string OutputPath = {};

  // TODO(Jesse id: 182, tags: high_priority) This should respect Args.Outpath passed in!
  if ( PeekTokenRaw(Parser).Type == CTokenType_Newline &&
       PeekTokenRaw(Parser, 1).Type == CT_PreprocessorInclude )
  {
    RequireToken(Parser, CToken(CT_PreprocessorInclude));
    RequireToken(Parser, CTokenType_LT);
    RequireToken(Parser, CToken(CS("metaprogramming")));
    RequireToken(Parser, CTokenType_FSlash);
    RequireToken(Parser, CToken(CS("output")));
    RequireToken(Parser, CTokenType_FSlash);
    counted_string IncludeFilename = RequireToken(Parser, CTokenType_Identifier).Value;

    string_builder IncludePathBuilder = {};
    Append(&IncludePathBuilder, CS("src/metaprogramming/output/"));
    Append(&IncludePathBuilder, IncludeFilename);

    if (OptionalToken(Parser, CTokenType_Dot))
    {
      Append(&IncludePathBuilder, CS("."));
      counted_string Extension = RequireToken(Parser, CTokenType_Identifier).Value;
      Append(&IncludePathBuilder, Extension);
    }

    RequireToken(Parser, CTokenType_GT);

    OutputPath = Finalize(&IncludePathBuilder, Memory);
    Output(OutputForThisParser, OutputPath, Memory);
  }
  else
  {
    // TODO(Jesse id: 183, tags: high_priority) This should respect Args.OutPath passed in!
    OutputPath = Concat(CS("src/metaprogramming/output/"), NewFilename, Memory);
    Output(OutputForThisParser, OutputPath, Memory);

    Push(CToken(CTokenType_Newline),     &Parser->OutputTokens);
    Push(CToken(CTokenType_Hash),        &Parser->OutputTokens);
    Push(CToken(CS("include")),          &Parser->OutputTokens);
    Push(CToken(CTokenType_Space),       &Parser->OutputTokens);

    Push(CToken(CTokenType_LT),          &Parser->OutputTokens);
    Push(CToken(CS("metaprogramming")),  &Parser->OutputTokens);
    Push(CToken(CTokenType_FSlash),      &Parser->OutputTokens);
    Push(CToken(CS("output")),           &Parser->OutputTokens);
    Push(CToken(CTokenType_FSlash),      &Parser->OutputTokens);
    Push(CToken(NewFilename),            &Parser->OutputTokens);
    Push(CToken(CTokenType_GT),          &Parser->OutputTokens);
  }

  parser *OutputParse = TokenizeAnsiStream(Ctx, AnsiStream(OutputForThisParser, OutputPath), TokenCursorSource_Unknown);

  if (IsInlineCode)
  {
    // TODO(Jesse, id: 226, tags: metaprogramming, output): Should we handle this differently?
    Warn("Not parsing datatypes in inlined code for %.*s", (u32)OutputPath.Count, OutputPath.Start);
  }
  else
  {
    // What do we do here?
    NotImplemented;
    /* ParseDatatypes(Ctx); */
    /* Push(&Ctx->AllParsers, OutputParse, Memory); */
  }

  /* PushParser(Ctx->CurrentParser, OutputParse, parser_push_type_include); */
  /* GoGoGadgetMetaprogramming(Ctx, TodoInfo); */

  return;
}

// TODO(Jesse, id: 113, tags: cleanup): Remove these?
debug_global platform Plat = {};
debug_global os Os = {};

/* TODO(Jesse, id: 112, tags: bootstrap_debug_system, copy_paste): This is
 * copy-pasted from the callgraph tests .. should we be able to call this from
 * anywhere?  It's also in the platform layer
 */
bonsai_function b32
BootstrapDebugSystem()
{
  shared_lib DebugLib = OpenLibrary(DEFAULT_DEBUG_LIB);
  if (!DebugLib) { Error("Loading DebugLib :( "); return False; }

  GetDebugState = (get_debug_state_proc)GetProcFromLib(DebugLib, DebugLibName_GetDebugState);
  if (!GetDebugState) { Error("Retreiving GetDebugState from Debug Lib :( "); return False; }

  b32 WindowSuccess = OpenAndInitializeWindow(&Os, &Plat);
  if (!WindowSuccess) { Error("Initializing Window :( "); return False; }
  Assert(Os.Window);

  InitializeOpengl(&Os);

  init_debug_system_proc InitDebugSystem = (init_debug_system_proc)GetProcFromLib(DebugLib, DebugLibName_InitDebugSystem);
  if (!InitDebugSystem) { Error("Retreiving InitDebugSystem from Debug Lib :( "); return False; }

  GetDebugState = InitDebugSystem(&GL);

  debug_state* DebugState = GetDebugState();
  DebugState->DebugDoScopeProfiling = True;
  DebugState->Plat = &Plat;

  GL.ClearColor(0.2f, 0.2f, 0.2f, 1.0f);
  GL.ClearDepth(1.0f);

  GL.BindFramebuffer(GL_FRAMEBUFFER, DebugState->GameGeoFBO.ID);
  GL.Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  GL.BindFramebuffer(GL_FRAMEBUFFER, 0);
  GL.Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  return True;
}

bonsai_function meta_func_arg*
StreamContains(meta_func_arg_stream* Stream, counted_string Match)
{
  meta_func_arg* Result = {};
  ITERATE_OVER(Stream)
  {
    meta_func_arg* Current = GET_ELEMENT(Iter);
    if (StringsMatch(Current->Match, Match))
    {
      Result = Current;
      break;
    }
  }

  return Result;
}

bonsai_function meta_func*
StreamContains(meta_func_stream* Stream, counted_string Name)
{
  meta_func* Result = {};
  ITERATE_OVER(Stream)
  {
    meta_func* Current = GET_ELEMENT(Iter);
    if (StringsMatch(Current->Name, Name))
    {
      Result = Current;
      break;
    }
  }

  return Result;
}

bonsai_function counted_string*
StreamContains(counted_string_stream* Stream, counted_string Name)
{
  counted_string* Result = {};
  ITERATE_OVER(Stream)
  {
    counted_string* Current = GET_ELEMENT(Iter);
    if (StringsMatch(Current, Name))
    {
      Result = Current;
      break;
    }
  }

  return Result;
}

bonsai_function person*
StreamContains(person_stream* People, counted_string Name)
{
  person* Result = {};
  ITERATE_OVER(People)
  {
    person* Current = GET_ELEMENT(Iter);
    if (StringsMatch(Current->Name, Name))
    {
      Result = Current;
      break;
    }
  }

  return Result;
}

bonsai_function tagged_counted_string_stream*
StreamContains(tagged_counted_string_stream_stream* Stream, counted_string Tag)
{
  tagged_counted_string_stream* Result = {};
  ITERATE_OVER(Stream)
  {
    tagged_counted_string_stream* Current = GET_ELEMENT(Iter);
    if (StringsMatch(Current->Tag, Tag))
    {
      Result = Current;
      break;
    }
  }
  return Result;
}

bonsai_function todo*
StreamContains(todo_stream* Todos, counted_string TodoId)
{
  todo* Result = {};
  ITERATE_OVER(Todos)
  {
    todo* Current = GET_ELEMENT(Iter);
    if (StringsMatch(Current->Id, TodoId))
    {
      Result = Current;
      break;
    }
  }
  return Result;
}

bonsai_function tag*
StreamContains(tag_stream* TodoLists, counted_string Tag)
{
  tag* Result = {};
  ITERATE_OVER(TodoLists)
  {
    tag* Current = GET_ELEMENT(Iter);
    if (StringsMatch(Current->Tag, Tag))
    {
      Result = Current;
      break;
    }
  }
  return Result;
}

bonsai_function todo*
UpdateTodo(todo_stream* Stream, todo Todo, memory_arena* Memory)
{
  todo* Result = StreamContains(Stream, Todo.Id);
  if (Result)
  {
    Result->Value = Todo.Value;
    Result->FoundInCodebase = Todo.FoundInCodebase;
  }
  else
  {
    Push(Stream, Todo, Memory);
    Result = StreamContains(Stream, Todo.Id);
  }
  return Result;
}

bonsai_function tag*
GetExistingOrCreate(tag_stream* Stream, counted_string Tag, memory_arena* Memory)
{
  tag* Result = StreamContains(Stream, Tag);
  if (!Result)
  {
    tag NewTag = { .Tag = Tag };
    Push(Stream, NewTag, Memory);
    Result = StreamContains(Stream, Tag);
  }
  return Result;
}

bonsai_function person*
GetExistingOrCreate(person_stream* People, counted_string PersonName, memory_arena* Memory)
{
  person* Person = StreamContains(People, PersonName);
  if (!Person)
  {
    person NewPerson = { .Name = PersonName };
    Push(People, NewPerson, Memory);
    Person = StreamContains(People, PersonName);
  }
  return Person;
}

static u32 LargestIdFoundInFile = 0;

bonsai_function todo
Todo(counted_string Id, counted_string Value, b32 FoundInCodebase)
{
  todo Result = { .Id = Id, .Value = Value, .FoundInCodebase = FoundInCodebase };
  return Result;
}

bonsai_function person_stream
ParseAllTodosFromFile(counted_string Filename, memory_arena* Memory)
{
  person_stream People = {};

  parser *Parser = TokenizeAnsiStream(AnsiStreamFromFile(Filename, Memory), Memory, True, 0, TokenCursorSource_Unknown);

  while (TokensRemain(Parser))
  {
    RequireToken(Parser, CTokenType_Hash);
    counted_string PersonName = RequireToken(Parser, CTokenType_Identifier).Value;

    person* Person = GetExistingOrCreate(&People, PersonName, Memory);
    while (OptionalToken(Parser, CT_PreprocessorPaste))
    {
      counted_string TagName = RequireToken(Parser, CTokenType_Identifier).Value;

      tag* Tag = GetExistingOrCreate(&Person->Tags, TagName, Memory);
      while (OptionalToken(Parser, CTokenType_Minus))
      {
        RequireToken(Parser, CTokenType_Hash);
        c_token TodoId = RequireToken(Parser, CTokenType_IntLiteral);

        LargestIdFoundInFile = Max(LargestIdFoundInFile, (u32)TodoId.UnsignedValue);
        counted_string TodoValue = Trim(ConcatTokensUntilNewline(Parser, Memory));
        todo NewTodo = Todo(TodoId.Value, TodoValue, False);
        Push(&Tag->Todos, NewTodo, Memory);
        EatWhitespace(Parser);
      }

      EatWhitespace(Parser);
    }

    EatWhitespace(Parser);
  }

  return People;
}

bonsai_function counted_string
GenerateOutfileNameFor(counted_string Name, counted_string DatatypeName, memory_arena* Memory, counted_string Modifier = {})
{
  string_builder OutfileBuilder = {};
  Append(&OutfileBuilder, Name);
  Append(&OutfileBuilder, CSz("_"));
  Append(&OutfileBuilder, DatatypeName);
  if (Modifier.Count)
  {
    Append(&OutfileBuilder, CSz("_"));
    Append(&OutfileBuilder, Modifier);
  }
  Append(&OutfileBuilder, CSz(".h"));
  counted_string Result = Finalize(&OutfileBuilder, Memory);

  return Result;
}

bonsai_function counted_string
GetNameForStructMember(struct_member* Decl)
{
  counted_string Result = {};

  switch (Decl->Type)
  {
    case type_function_decl:
    {
      Result = Decl->function_decl.Name;
    } break;

    case type_variable_decl:
    {
      Result = Decl->variable_decl.Name;
    } break;

    case type_struct_member_anonymous:
    {
      Result = CSz("(anonymous struct or union)");
    } break;

    InvalidDefaultCase;
  }

  return Result;
}

bonsai_function counted_string
Transform(meta_transform_op Transformations, counted_string Input, memory_arena* Memory)
{
  counted_string Result = Input;
  while ( Transformations )
  {
    if ( Transformations & to_capital_case )
    {
      UnsetBitfield(meta_transform_op, Transformations, to_capital_case );
      Result = ToCapitalCase(Result, Memory);
    }
  }

  return Result;
}

bonsai_function counted_string
EscapeDoubleQuotes(counted_string S, memory_arena* Memory)
{
  string_builder B = {};
  for (u32 CharIndex = 0;
      CharIndex < S.Count;
      ++CharIndex)
  {
    if (S.Start[CharIndex] == '"')
    {
      Append(&B, CSz("\\"));
    }
    Append(&B, CS(S.Start+CharIndex, 1));
  }

  counted_string Result = Finalize(&B, Memory);
  return Result;
}

bonsai_function void
AppendAndEscapeInteriorOfDoubleQuotedString(string_builder* Builder, counted_string S)
{
  if (S.Count >= 2 &&
      S.Start[0] == '"' &&
      S.Start[S.Count-1] == '"')
  {
    S.Count -= 2;
    ++S.Start;
    Append(Builder, CSz("\""));
    Append(Builder, EscapeDoubleQuotes(S, Builder->Memory));
    Append(Builder, CSz("\""));
  }
  else
  {
    Append(Builder, EscapeDoubleQuotes(S, Builder->Memory));
  }
}

bonsai_function meta_transform_op
ParseTransformations(parser* Scope)
{
  meta_transform_op Result = {};

  while (OptionalToken(Scope, CTokenType_Dot))
  {
    meta_transform_op NextOp = MetaTransformOp(RequireToken(Scope, CTokenType_Identifier).Value);
    if (NextOp != meta_transform_op_noop)
    {
      Result = (meta_transform_op)(Result | NextOp);
    }
    else
    {
      Error("Parsing transform ops.");
    }
  }

  return Result;
}

bonsai_function meta_func_arg
ReplacementPattern(counted_string Match, datatype Data)
{
  meta_func_arg Result = {
    .Match = Match,
    .Data = Data
  };

  return Result;
}

bonsai_function meta_func_arg_stream
CopyStream(meta_func_arg_stream* Stream, memory_arena* Memory)
{
  meta_func_arg_stream Result = {};
  ITERATE_OVER(Stream)
  {
    meta_func_arg* Element = GET_ELEMENT(Iter);
    Push(&Result, *Element, Memory);
  }
  return Result;
}

bonsai_function counted_string
Execute(meta_func* Func, meta_func_arg_stream *Args, parse_context* Ctx, memory_arena* Memory);

// TODO(Jesse id: 222, tags: immediate, parsing, metaprogramming) : Re-add [[nodiscard]] here
bonsai_function counted_string
Execute(counted_string FuncName, parser Scope, meta_func_arg_stream* ReplacePatterns, parse_context* Ctx, memory_arena* Memory)
{
  program_datatypes* Datatypes = &Ctx->Datatypes;
  meta_func_stream* FunctionDefs = &Ctx->MetaFunctions;

  Rewind(&Scope.Tokens);

  string_builder OutputBuilder = {};
  while (Remaining(&Scope.Tokens))
  {
    c_token BodyToken = PopTokenRaw(&Scope);

    if ( BodyToken.Type == CTokenType_StringLiteral )
    {
      parser *StringParse = TokenizeAnsiStream(AnsiStream(BodyToken.Value, Scope.Filename), Memory, True, Ctx, TokenCursorSource_MetaprogrammingExpansion);

      counted_string Code = Execute(FuncName, *StringParse, ReplacePatterns, Ctx, Memory);
      AppendAndEscapeInteriorOfDoubleQuotedString(&OutputBuilder, Code);
    }
    else if ( BodyToken.Type == CTokenType_OpenParen )
    {
      b32 ExecutedChildFunc = False;
      ITERATE_OVER_AS(Replace, ReplacePatterns)
      {
        meta_func_arg* Replace = GET_ELEMENT(ReplaceIter);
        if ( OptionalToken(&Scope, CToken(Replace->Match)) )
        {
          ExecutedChildFunc = True;
          RequireToken(&Scope, CTokenType_Dot);
          meta_arg_operator Operator = MetaArgOperator( RequireToken(&Scope, CTokenType_Identifier).Value);

          switch (Operator)
          {
            case meta_arg_operator_noop:
            {
              ParseError(&Scope, CSz("Invalid operator encountered."));
            } break;

            case is_struct:
            {
              RequireToken(&Scope, CTokenType_Question);
              parser StructScope = GetBodyTextForNextScope(&Scope);
              if (Replace->Data.Type == type_struct_def)
              {
                counted_string Code = Execute(FuncName, StructScope, ReplacePatterns, Ctx, Memory);
                Append(&OutputBuilder, Code);
              }
            } break;

            case is_enum:
            {
              RequireToken(&Scope, CTokenType_Question);
              parser EnumScope = GetBodyTextForNextScope(&Scope);
              switch (Replace->Data.Type)
              {
                case type_enum_member:
                case type_enum_def:
                {
                  counted_string Code = Execute(FuncName, EnumScope, ReplacePatterns, Ctx, Memory);
                  Append(&OutputBuilder, Code);
                } break;

                case type_struct_member:
                {
                  switch (Replace->Data.struct_member->Type)
                  {
                    case type_variable_decl:
                    {
                      enum_def *E = GetEnumByType(&Datatypes->Enums, Replace->Data.struct_member->variable_decl.Type.Token.Value);
                      if (E)
                      {
                        meta_func_arg_stream NewArgs = CopyStream(ReplacePatterns, Memory);
                        ITERATE_OVER_AS(NewReplace, &NewArgs)
                        {
                          meta_func_arg* NewReplace = GET_ELEMENT(NewReplaceIter);
                          if (StringsMatch(Replace->Match, NewReplace->Match))
                          {
                            NewReplace->Data = Datatype(E);
                          }
                        }

                        counted_string Code = Execute(FuncName, EnumScope, &NewArgs, Ctx, Memory);
                        Append(&OutputBuilder, Code);
                      }
                    } break;

                    case type_struct_member_noop:
                    case type_struct_member_anonymous:
                    case type_function_decl:
                    {
                    }
                  }
                } break;

                case type_primitive_def:
                case type_struct_def:
                {
                } break;

                case type_stl_container_def:
                case type_type_def:
                {
                  NotImplemented;
                } break;

                case type_datatype_noop:
                {
                  InvalidCodePath();
                } break;
              }

            } break;

            case value:
            {
              if (Replace->Data.Type == type_enum_member)
              {
                // Changed enum_member::Value from counted_string to
                // ast_node_expression, and don't have a string representation
                // so this is going away for a bit..
                //

                NotImplemented;

                /* meta_transform_op Transformations = ParseTransformations(&Scope); */
                /* counted_string Name = Transform(Transformations, Replace->Data.enum_member->Value, Memory); */
                /* Append(&OutputBuilder, Name); */
              }
              else
              {
                Error("Called .value on non-enum_member type.");
              }
            } break;

            case type:
            {
              meta_transform_op Transformations = ParseTransformations(&Scope);
              counted_string TypeName = {};
              switch (Replace->Data.Type)
              {
                case type_datatype_noop: { InvalidCodePath(); } break;

                case  type_type_def:
                {
                  NotImplemented;
                } break;

                case type_stl_container_def:
                {
                  NotImplemented;
                } break;

                case type_primitive_def:
                {
                  NotImplemented;
                  // Does this actually work?
                  // TypeName = Replace->Data.primitive_def->Type.SourceText;
                } break;

                case type_struct_def:
                {
                  TypeName = Replace->Data.struct_def->Type;
                } break;

                case type_enum_def:
                {
                  TypeName = Replace->Data.enum_def->Name;
                } break;

                case type_enum_member:
                {
                  TypeName = Replace->Data.enum_member->Name;
                } break;

                case type_struct_member:
                {
                  switch (Replace->Data.struct_member->Type)
                  {
                    case type_struct_member_noop: { InvalidCodePath(); } break;

                    case type_variable_decl:
                    {
                      TypeName = Replace->Data.struct_member->variable_decl.Type.Token.Value;
                    } break;

                    case type_function_decl:
                    {
                      TypeName = Replace->Data.struct_member->function_decl.Name;
                    } break;

                    case type_struct_member_anonymous:
                    {
                      TypeName = CSz("(anonymous struct/union)");
                    } break;
                  }

                } break;

              }

              counted_string Name = Transform(Transformations, TypeName, Memory);
              Append(&OutputBuilder, Name);
            } break;

            case name:
            {
              switch (Replace->Data.Type)
              {
                case type_struct_member:
                {
                  counted_string MemberName = GetNameForStructMember(Replace->Data.struct_member);
                  meta_transform_op Transformations = ParseTransformations(&Scope);
                  counted_string Name = Transform(Transformations, MemberName, Memory);
                  Append(&OutputBuilder, Name);
                } break;

                case type_enum_member:
                {
                  meta_transform_op Transformations = ParseTransformations(&Scope);
                  counted_string Name = Transform(Transformations, Replace->Data.enum_member->Name, Memory);
                  Append(&OutputBuilder, Name);
                } break;

                case type_enum_def:
                {
                  meta_transform_op Transformations = ParseTransformations(&Scope);
                  counted_string Name = Transform(Transformations, Replace->Data.enum_def->Name, Memory);
                  Append(&OutputBuilder, Name);
                } break;

                case type_struct_def:
                {
                  meta_transform_op Transformations = ParseTransformations(&Scope);
                  counted_string Name = Transform(Transformations, Replace->Data.struct_def->Type, Memory);
                  Append(&OutputBuilder, Name);
                } break;

                InvalidDefaultCase;
              }

            } break;

            case map_members:
            {
              RequireToken(&Scope, CTokenType_OpenParen);
              counted_string MatchPattern  = RequireToken(&Scope, CTokenType_Identifier).Value;
              RequireToken(&Scope, CTokenType_CloseParen);

              counted_string ContainingConstraint = {};
              counted_string ChildName = {};
              if ( OptionalToken(&Scope, CTokenType_Dot) )
              {
                RequireToken(&Scope, CToken(CSz("containing")));
                RequireToken(&Scope, CTokenType_OpenParen);
                ContainingConstraint = RequireToken(&Scope, CTokenType_Identifier).Value;

                if ( OptionalToken(&Scope, CToken(CSz("as"))) )
                {
                  ChildName = RequireToken(&Scope, CTokenType_Identifier).Value;
                }

                RequireToken(&Scope, CTokenType_CloseParen);
              }

              parser MapMemberScope = GetBodyTextForNextScope(&Scope);

              if (Replace->Data.Type == type_struct_def)
              {
                ITERATE_OVER_AS(Member, &Replace->Data.struct_def->Members)
                {
                  struct_member* Member = GET_ELEMENT(MemberIter);

                  switch (Member->Type)
                  {
                    case type_struct_member_noop:
                    {
                      InvalidCodePath();
                    } break;

                    case type_function_decl:
                    {
                    } break;

                    case type_variable_decl:
                    {
                      if ( ContainingConstraint.Count &&
                           !StringsMatch(Member->variable_decl.Type.Token.Value, ContainingConstraint) )
                      {
                        // Containing constraint failed
                      }
                      else
                      {
                        meta_func_arg_stream NewArgs = CopyStream(ReplacePatterns, Memory);
                        Push(&NewArgs, ReplacementPattern(MatchPattern, Datatype(Member)), Memory);
                        counted_string StructFieldOutput = Execute(FuncName, MapMemberScope, &NewArgs, Ctx, Memory);
                        Append(&OutputBuilder, StructFieldOutput);
                      }

                    } break;

                    case type_struct_member_anonymous:
                    {
                      for (struct_member_iterator UnionMemberIter = Iterator(&Member->struct_member_anonymous.Body.Members);
                          IsValid(&UnionMemberIter);
                          Advance(&UnionMemberIter))
                      {
                        struct_member* UnionMember = GET_ELEMENT(UnionMemberIter);
                        if (UnionMember->Type == type_variable_decl)
                        {
                          struct_def* Struct = GetStructByType(&Datatypes->Structs, UnionMember->variable_decl.Type.Token.Value);
                          if (Struct)
                          {
                            struct_member_stream ContainedMembers = MembersOfType(Struct, ContainingConstraint, Memory);
                            if (ContainedMembers.FirstChunk)
                            {
                              meta_func_arg_stream NewArgs = CopyStream(ReplacePatterns, Memory);
                              Push(&NewArgs, ReplacementPattern(MatchPattern, Datatype(Struct)), Memory);
                              if (ChildName.Count) {
                                struct_def SyntheticStruct = {
                                  .Type = CSz("synthetically_created_struct"),
                                  .DefinedInFile = Struct->DefinedInFile,
                                  .Members = ContainedMembers,
                                };
                                Push(&NewArgs, ReplacementPattern(ChildName, Datatype(&SyntheticStruct)), Memory);
                              }
                              counted_string StructFieldOutput = Execute(FuncName, MapMemberScope, &NewArgs, Ctx, Memory);
                              Append(&OutputBuilder, StructFieldOutput);
                            }
                          }
                          else
                          {
                            counted_string Name = UnionMember->variable_decl.Type.Token.Value;
                            counted_string ParentStructName = Replace->Data.struct_def->Type;
                            Warn("Couldn't find struct type '%.*s' in union parent '%.*s'.",
                                (u32)Name.Count, Name.Start,
                                (u32)ParentStructName.Count, ParentStructName.Start);
                          }

                        }
                        else
                        {
                          Error("Nested structs/unions and bonsai_function pointers unsupported.");
                        }
                      }
                    } break;
                  }

                  continue;
                }
              }
              else
              {
                Error("Called map_members on a datatype that wasn't a struct - %.*s", (u32)Replace->Data.enum_def->Name.Count, Replace->Data.enum_def->Name.Start);
              }

            } break;

            case map_values:
            {
              RequireToken(&Scope, CTokenType_OpenParen);
              counted_string EnumValueMatch  = RequireToken(&Scope, CTokenType_Identifier).Value;
              RequireToken(&Scope, CTokenType_CloseParen);
              parser NextScope = GetBodyTextForNextScope(&Scope);

              if (Replace->Data.Type == type_enum_def)
              {
                ITERATE_OVER(&Replace->Data.enum_def->Members)
                {
                  enum_member* EnumMember = GET_ELEMENT(Iter);
                  meta_func_arg_stream NewArgs = CopyStream(ReplacePatterns, Memory);
                  Push(&NewArgs, ReplacementPattern(EnumValueMatch, Datatype(EnumMember)), Memory);
                  counted_string EnumFieldOutput = Execute(FuncName, NextScope, &NewArgs, Ctx, Memory);
                  Append(&OutputBuilder, EnumFieldOutput);
                  continue;
                }

              }
              else
              {
                Error("Called map_values on a datatype that wasn't an enum - %.*s", (u32)Replace->Data.struct_def->Type.Count, Replace->Data.struct_def->Type.Start);
              }

            } break;
          }

          RequireToken(&Scope, CTokenType_CloseParen);
        }
      }

      meta_func* NestedFunc = StreamContains( FunctionDefs, PeekToken(&Scope).Value );
      if (NestedFunc)
      {
        RequireToken(&Scope, CToken(NestedFunc->Name));

        RequireToken(&Scope, CTokenType_OpenParen);

        counted_string ArgName = RequireToken(&Scope, CTokenType_Identifier).Value;
        meta_func_arg* Arg = StreamContains(ReplacePatterns, ArgName);

        if (Arg)
        {
          RequireToken(&Scope, CTokenType_CloseParen);
          RequireToken(&Scope, CTokenType_CloseParen);

          if (!StringsMatch(NestedFunc->Name, FuncName))
          {
            meta_func_arg_stream NewArgs = {};
            Push(&NewArgs, ReplacementPattern(NestedFunc->ArgName, Arg->Data), Memory);
            counted_string NestedCode = Execute(NestedFunc, &NewArgs, Ctx, Memory);
            Append(&OutputBuilder, NestedCode);
          }
          else
          {
            Error("Recursive bonsai_function calls not supported.");
          }
        }
        else
        {
          ParseError(&Scope, CSz("Unable to resolve bonsai_function argument."));
        }

      }
      else if (ExecutedChildFunc)
      {
      }
      else
      {
        Append(&OutputBuilder, BodyToken.Value);
      }
    }
    else
    {
      Append(&OutputBuilder, BodyToken.Value);
    }

    continue;
  }

  counted_string Result = Finalize(&OutputBuilder, Memory);
  return Result;
}

bonsai_function counted_string
Execute(meta_func* Func, meta_func_arg_stream *Args, parse_context* Ctx, memory_arena* Memory)
{
  counted_string Result = Execute(Func->Name, Func->Body, Args, Ctx, Memory);
  return Result;
}

bonsai_function void
ConcatStreams(counted_string_stream* S1, counted_string_stream* S2, memory_arena* Memory)
{
  ITERATE_OVER(S2)
  {
    counted_string* Element = GET_ELEMENT(Iter);
    Push(S1, *Element, Memory);
  }
  return;
}

bonsai_function b32
IsMetaprogrammingOutput(counted_string Filename, counted_string OutputDirectory)
{
  b32 Result = Contains(Filename, OutputDirectory);
  return Result;
}

bonsai_function counted_string_stream
ParseDatatypeList(parser* Parser, program_datatypes* Datatypes, tagged_counted_string_stream_stream* NameLists, memory_arena* Memory)
{
  counted_string_stream Result = {};
  while (PeekToken(Parser).Type == CTokenType_Identifier)
  {
    counted_string DatatypeName    = RequireToken(Parser, CTokenType_Identifier).Value;

    struct_def* Struct                 = GetStructByType(&Datatypes->Structs, DatatypeName);
    enum_def* Enum                     = GetEnumByType(&Datatypes->Enums, DatatypeName);
    tagged_counted_string_stream* List = StreamContains(NameLists, DatatypeName);

    if (Struct || Enum)
    {
      Push(&Result, DatatypeName, Memory);
    }
    else if (List)
    {
      ConcatStreams(&Result, &List->Stream, Memory);
    }
    else
    {
      Warn("Type (%.*s) could not be resolved to a struct or named_list, ignoring it.", (u32)DatatypeName.Count, DatatypeName.Start);
    }

    OptionalToken(Parser, CTokenType_Comma);
  }

  return Result;
}

bonsai_function meta_func
ParseMetaFunctionDef(parser* Parser, counted_string FuncName)
{
  RequireToken(Parser, CTokenType_OpenParen);
  counted_string ArgName = RequireToken(Parser, CTokenType_Identifier).Value;
  RequireToken(Parser, CTokenType_CloseParen);
  parser Body = GetBodyTextForNextScope(Parser);

  meta_func Func = {
    .Name = FuncName,
    .ArgName = ArgName,
    .Body = Body,
  };

  return Func;
}

#if 0
bonsai_function void
RemoveAllMetaprogrammingOutput(parser_cursor* ParsedFiles, arguments* Args)
{

  for (u32 ParserIndex = 0;
      ParserIndex < Count(ParsedFiles);
      ++ParserIndex)
  {
    parser* Parser = ParsedFiles->Start+ParserIndex;
    if (IsMetaprogrammingOutput(Parser->Filename, Args->Outpath))
    {
      Info("Removing %.*s", (u32)Parser->Filename.Count, Parser->Filename.Start);
      Remove(Parser->Filename);
      continue;
    }
  }

}
#endif

bonsai_function counted_string
ParseMultiLineTodoValue(parser* Parser, memory_arena* Memory)
{
  string_builder Builder = {};

  while (PeekTokenRaw(Parser).Type != CTokenType_CommentMultiLineEnd)
  {
    c_token T = PopTokenRaw(Parser);

    if (T.Type == CTokenType_Newline)
    {
      EatWhitespace(Parser);
      if (OptionalToken(Parser, CTokenType_Star))
      {
        EatWhitespace(Parser);
      }
      Append(&Builder, CSz(" "));
    }
    else
    {
      Append(&Builder, T.Value);
    }

    continue;
  }

  Ensure( PopTokenRaw(Parser).Type == CTokenType_CommentMultiLineEnd );

  counted_string Result = Trim(Finalize(&Builder, Memory));
  return Result;
}

bonsai_function void
GoGoGadgetMetaprogramming(parse_context* Ctx, todo_list_info* TodoInfo)
{
  program_datatypes *Datatypes   = &Ctx->Datatypes;
  meta_func_stream *FunctionDefs = &Ctx->MetaFunctions;
  memory_arena *Memory           = Ctx->Memory;

  person_stream* People = &TodoInfo->People;
  tagged_counted_string_stream_stream* NameLists = &TodoInfo->NameLists;

  parser *Parser = Ctx->CurrentParser;
  while (TokensRemain(Parser))
  {
    const c_token T = PeekTokenRaw(Parser);

    switch( T.Type )
    {
      case CTokenType_CommentMultiLineStart:
      case CTokenType_CommentSingleLine:
      {
        c_token CommentStartToken = PopTokenRaw(Parser);
        c_token FirstInteriorT = PeekToken(Parser);
        if ( StringsMatch(FirstInteriorT.Value, CSz("TODO")) )
        {
          RequireToken(Parser, CToken(CSz("TODO")));

          if (OptionalToken(Parser, CTokenType_OpenParen))
          {
            counted_string PersonName = RequireToken(Parser, CTokenType_Identifier).Value;
            counted_string_stream TodoTags = {};
            b32 GotAnyTags = False;

            counted_string IdValue = {};
            OptionalToken(Parser, CTokenType_Comma);

            b32 GeneratedNewId = False;
            if (OptionalToken(Parser, CToken(CSz("id"))))
            {
              RequireToken(Parser, CTokenType_Colon);
              IdValue = RequireToken(Parser, CTokenType_IntLiteral).Value;
            }
            else
            {
              Assert(!IdValue.Count);
              GeneratedNewId = True;
              IdValue = CS(++LargestIdFoundInFile);
              Push(CToken(CS(" id: ")), &Parser->OutputTokens);
              Push(CToken(IdValue),     &Parser->OutputTokens);
            }

            OptionalToken(Parser, CTokenType_Comma);

            if (StringsMatch(PeekToken(Parser).Value, CSz("tags")))
            {
              if (GeneratedNewId)
              {
                Push(CToken(CTokenType_Comma), &Parser->OutputTokens);
              }

              RequireToken(Parser, CToken(CSz("tags")));
              GotAnyTags = True;
              RequireToken(Parser, CTokenType_Colon);
              Push(&TodoTags, RequireToken(Parser, CTokenType_Identifier).Value, Memory);

              while (OptionalToken(Parser, CTokenType_Comma))
              {
                Push(&TodoTags, RequireToken(Parser, CTokenType_Identifier).Value, Memory);
              }
            }

            if (!GotAnyTags)
            {
              counted_string Tag = CSz("untagged");
              Push(&TodoTags, Tag, Memory);
            }

            RequireToken(Parser, CTokenType_CloseParen);
            OptionalToken(Parser, CTokenType_Colon);

            counted_string TodoValue = {};
            if ( T.Type == CTokenType_CommentSingleLine )
            {
              string_from_parser Builder = StartStringFromParser(Parser);
                EatUntilIncluding(Parser, CTokenType_Newline);
              TodoValue = FinalizeStringFromParser(&Builder);

              TodoValue = Trim(TodoValue);
            }
            else
            {
              TodoValue = ParseMultiLineTodoValue(Parser, Memory);
            }

            person* Person = GetExistingOrCreate(People, PersonName, Memory);

            ITERATE_OVER(&TodoTags)
            {
              counted_string* TodoTag = GET_ELEMENT(Iter);
              tag* Tag = GetExistingOrCreate(&Person->Tags, *TodoTag, Memory);
              UpdateTodo(&Tag->Todos, Todo(IdValue, TodoValue, True), Memory);
            }

          }
        }
        else
        {
          EatComment(Parser, CommentStartToken.Type);
        }

      } break;

      case CTokenType_Meta:
      {
        c_token MetaToken = RequireToken(Parser, CTokenType_Meta);
        RequireToken(Parser, CTokenType_OpenParen);

        if (OptionalToken(Parser, CTokenType_Ellipsis)) // Just ignore meta tags containing (...) .. It's probably the '#define meta(...)' thing
        {
          RequireToken(Parser, CTokenType_CloseParen);
          break;
        }

        counted_string DirectiveString = RequireToken(Parser, CTokenType_Identifier).Value;
        metaprogramming_directive Directive = MetaprogrammingDirective(DirectiveString);
        switch (Directive)
        {
          case polymorphic_func:
          {
            /* function_decl F = ParseFunctionDef(Parser, Memory); */
            /* DebugPrint(F); */
          } break;

          case func:
          {
            if (OptionalToken(Parser, CTokenType_OpenParen))
            {
              counted_string ArgType = RequireToken(Parser, CTokenType_Identifier).Value;
              counted_string ArgName = RequireToken(Parser, CTokenType_Identifier).Value;
              RequireToken(Parser, CTokenType_CloseParen);


              parser Body = GetBodyTextForNextScope(Parser);

              meta_func Func = {
                .Name = CSz("anonymous_function"),
                .ArgName = ArgName,
                .Body = Body,
              };

              datatype Arg = GetDatatypeByName(&Ctx->Datatypes, ArgType);
              meta_func_arg_stream Args = {};
              Push(&Args, ReplacementPattern(ArgName, Arg), Memory);
              counted_string Code = Execute(&Func, &Args, Ctx, Memory);

              RequireToken(Parser, CTokenType_CloseParen);
              if (Code.Count)
              {
                counted_string OutfileName = GenerateOutfileNameFor(Func.Name, ArgType, Memory, GetRandomString(8, Hash(&Code), Memory));
                FlushOutputToDisk(Ctx, Code, OutfileName, TodoInfo, Memory, True);
              }
              else
              {
                Warn("Unable to generate code for meta_func %.*s", (u32)Func.Name.Count, Func.Name.Start);
              }
            }
            else
            {
              counted_string FuncName = RequireToken(Parser, CTokenType_Identifier).Value;
              meta_func Func = ParseMetaFunctionDef(Parser, FuncName);
              Push(FunctionDefs, Func, Memory);
            }

          } break;

          case named_list:
          {
            RequireToken(Parser, CTokenType_OpenParen);

            tagged_counted_string_stream NameList = {
              .Tag = RequireToken(Parser, CTokenType_Identifier).Value
            };

            RequireToken(Parser, CTokenType_CloseParen);

            RequireToken(Parser, CTokenType_OpenBrace);
            while (PeekToken(Parser).Type == CTokenType_Identifier)
            {
              counted_string Name = RequireToken(Parser, CTokenType_Identifier).Value;
              Push(&NameList.Stream, Name, Memory);
              OptionalToken(Parser, CTokenType_Comma);
            }

            RequireToken(Parser, CTokenType_CloseBrace);

            Push(&TodoInfo->NameLists, NameList, Memory);

          } break;

          case for_datatypes:
          {
            RequireToken(Parser, CTokenType_OpenParen);
            RequireToken(Parser, CToken(CSz("all")));
            RequireToken(Parser, CTokenType_CloseParen);

            counted_string_stream Excludes = {};
            if (OptionalToken(Parser, CTokenType_Dot))
            {
              RequireToken(Parser, CToken(CSz("exclude")));
              RequireToken(Parser, CTokenType_OpenParen);
              Excludes = ParseDatatypeList(Parser, Datatypes, NameLists, Memory);
              RequireToken(Parser, CTokenType_CloseParen);
            }

            RequireToken(Parser, CToken(ToString(func)));
            meta_func StructFunc = ParseMetaFunctionDef(Parser, CSz("for_datatypes_struct_callback"));

            RequireToken(Parser, CToken(ToString(func)));
            meta_func EnumFunc = ParseMetaFunctionDef(Parser, CSz("for_datatypes_enum_callback"));

            RequireToken(Parser, CTokenType_CloseParen);

            string_builder OutputBuilder = {};

            for (struct_def_iterator Iter = Iterator(&Datatypes->Structs);
                IsValid(&Iter);
                Advance(&Iter))
            {
              struct_def* Struct = &Iter.At->Element;

              if (!StreamContains(&Excludes, Struct->Type))
              {
                meta_func_arg_stream Args = {};
                Push(&Args, ReplacementPattern(StructFunc.ArgName, Datatype(Struct)), Memory);
                counted_string Code = Execute(&StructFunc, &Args, Ctx, Memory);
                Append(&OutputBuilder, Code);
              }
            }

            for (enum_def_iterator Iter = Iterator(&Datatypes->Enums);
                IsValid(&Iter);
                Advance(&Iter))
            {
              enum_def* Enum = &Iter.At->Element;
              if (!StreamContains(&Excludes, Enum->Name))
              {
                meta_func_arg_stream Args = {};
                Push(&Args, ReplacementPattern(EnumFunc.ArgName, Datatype(Enum)), Memory);
                counted_string Code = Execute(&EnumFunc, &Args, Ctx, Memory);
                Append(&OutputBuilder, Code);
              }
            }


            counted_string Code = Finalize(&OutputBuilder, Memory);
            counted_string OutfileName = GenerateOutfileNameFor(ToString(Directive), CSz("debug_print"), Memory);

            FlushOutputToDisk(Ctx, Code, OutfileName, TodoInfo, Memory);
          } break;

          case d_union:
          {
            counted_string DatatypeName = RequireToken(Parser, CTokenType_Identifier).Value;
            d_union_decl dUnion = ParseDiscriminatedUnion(Parser, Datatypes, DatatypeName, Memory);
            if (Parser->Valid)
            {
              counted_string OutfileName = GenerateOutfileNameFor(ToString(Directive), DatatypeName, Memory);

              string_builder CodeBuilder = {};
              if (!dUnion.CustomEnumType.Count)
              {
                counted_string EnumString = GenerateEnumDef(&dUnion, Memory);
                Append(&CodeBuilder, EnumString);
              }

              {
                counted_string StructString = GenerateStructDef(&dUnion, Memory);
                Append(&CodeBuilder, StructString);
              }

              counted_string Code = Finalize(&CodeBuilder, Memory);

              RequireToken(Parser, CTokenType_CloseParen);
              FlushOutputToDisk(Ctx, Code, OutfileName, TodoInfo, Memory);

            }
            else
            {
              Error("Parsing d_union declaration");
            }
          } break;

          default:
          {
            meta_func* Func = StreamContains(FunctionDefs, DirectiveString);
            if (Func)
            {
              RequireToken(Parser, CTokenType_OpenParen);
              counted_string DatatypeName = RequireToken(Parser, CTokenType_Identifier).Value;
              RequireToken(Parser, CTokenType_CloseParen);
              RequireToken(Parser, CTokenType_CloseParen);

              /* Info("Calling bonsai_function : %.*s(%.*s)", (u32)Func->Name.Count, Func->Name.Start, (u32)DatatypeName.Count, DatatypeName.Start); */
              datatype Arg = GetDatatypeByName(&Ctx->Datatypes, DatatypeName);
              meta_func_arg_stream Args = {};
              Push(&Args, ReplacementPattern(Func->ArgName, Arg), Memory);
              counted_string Code = Execute(Func, &Args, Ctx, Memory);

              if (Code.Count)
              {
                counted_string OutfileName = GenerateOutfileNameFor(Func->Name, DatatypeName, Memory);
                FlushOutputToDisk(Ctx, Code, OutfileName, TodoInfo, Memory);
              }
              else
              {
                Warn("Unable to generate code for meta_func %.*s", (u32)Func->Name.Count, Func->Name.Start);
              }
            }
            else
            {
              Error("Couldn't resolve %.*s to a metaprogramming directive or bonsai_function name", (u32)DirectiveString.Count, DirectiveString.Start);
            }
          }

        }
      } break;

      default:
      {
        PopTokenRaw(Parser);
      } break;
    }

    continue;
  }
}

bonsai_function void
WriteTodosToFile(person_stream* People, memory_arena* Memory)
{
  random_series Rng = {.Seed = 123125};
  native_file TempFile = GetTempFile(&Rng, Memory);
  b32 AllWritesSucceeded = TempFile.Handle ? True : False;
  if (AllWritesSucceeded)
  {
    ITERATE_OVER_AS(person, People)
    {
      person* Person = GET_ELEMENT(personIter);
      AllWritesSucceeded &= WriteToFile(&TempFile, CSz("# "));
      AllWritesSucceeded &= WriteToFile(&TempFile, Person->Name);
      AllWritesSucceeded &= WriteToFile(&TempFile, CSz("\n"));
      ITERATE_OVER(&Person->Tags)
      {
        tag* Tag = GET_ELEMENT(Iter);

        todo_iterator InnerIter = Iterator(&Tag->Todos);
        if (IsValid(&InnerIter))
        {
          AllWritesSucceeded &= WriteToFile(&TempFile, CSz("  ## "));
          AllWritesSucceeded &= WriteToFile(&TempFile, Tag->Tag);
          AllWritesSucceeded &= WriteToFile(&TempFile, CSz("\n"));

          for (;
              IsValid(&InnerIter);
              Advance(&InnerIter))
          {
            todo* Todo = GET_ELEMENT(InnerIter);
            if (Todo->FoundInCodebase)
            {
              AllWritesSucceeded &= WriteToFile(&TempFile, CSz("    - #"));
              AllWritesSucceeded &= WriteToFile(&TempFile, Todo->Id);
              AllWritesSucceeded &= WriteToFile(&TempFile, CSz(" "));
              AllWritesSucceeded &= WriteToFile(&TempFile, Todo->Value);
              AllWritesSucceeded &= WriteToFile(&TempFile, CSz("\n"));
            }
          }

          AllWritesSucceeded &= WriteToFile(&TempFile, CSz("\n"));
        }
      }
    }
  }
  else
  {
    Error("Opening Tempfile");
  }

  Rename(TempFile, CSz("todos.md"));
  CloseFile(&TempFile);

  return;
}

bonsai_function void WalkAst(ast_node* Ast);
bonsai_function void WalkAst(ast_node_statement* Ast);

bonsai_function void
WalkAst(ast_node_expression* Ast)
{
  if (Ast)
  {
    WalkAst(Ast->Value);
    WalkAst(Ast->Next);
  }
}

bonsai_function void
WalkAst(ast_node_statement* Ast)
{
  if (Ast)
  {
    WalkAst(Ast->LHS);
    WalkAst(Ast->RHS);
    WalkAst(Ast->Next);
  }
}

bonsai_function void
WalkAst(ast_node* Ast)
{
  if (Ast)
  {
    switch(Ast->Type)
    {
      case type_ast_node_function_call:
      {
        auto Child = SafeAccess(ast_node_function_call, Ast);
      } break;

      case type_ast_node_statement:
      {
        auto Child = SafeAccess(ast_node_statement, Ast);
        WalkAst(Child);
      } break;

      case type_ast_node_variable_def:
      {
        auto Child = SafeAccess(ast_node_variable_def, Ast);
        WalkAst(Child->Value);
      } break;

      case type_ast_node_access:
      {
        auto Child = SafeAccess(ast_node_access, Ast);
        WalkAst(Child->Symbol);
      } break;

      case type_ast_node_expression:
      {
        auto Child = SafeAccess(ast_node_expression, Ast);
        WalkAst(Child);
      } break;

      case type_ast_node_parenthesized:
      {
        auto Child = SafeAccess(ast_node_parenthesized, Ast);
        WalkAst(Child->Expr);
        if (Child->IsCast)
        {
          Assert(Child->CastValue);
          WalkAst(Child->CastValue);
        }
        else
        {
          Assert(!Child->CastValue);
        }
      } break;

      case type_ast_node_operator:
      {
        auto Child = SafeAccess(ast_node_operator, Ast);
        WalkAst(Child->Operand);
      } break;

      case type_ast_node_return:
      {
        auto Child = SafeAccess(ast_node_return, Ast);
        WalkAst(Child->Value);
      } break;

      case type_ast_node_initializer_list:
      case type_ast_node_symbol:
      case type_ast_node_type_specifier:
      case type_ast_node_literal:
      {
      } break;

      case type_ast_node_noop:
      {
        InvalidCodePath();
      } break;

      InvalidDefaultCase;
    }
  }
}

#if 0
bonsai_function void
RegisterPrimitiveDatatypes(program_datatypes *Datatypes, memory_arena *Memory)
{
  primitive_def P = {};

  type_spec *Type = &P.Type;

  //
  // Integral Types
  //

#if 0
  Type->Name = CSz("short"); Type->SourceText = CSz("short");
  Push(&Datatypes->Primitives, P, Memory); Type->Signed = true;

  Type->SourceText = CSz("short int");
  Push(&Datatypes->Primitives, P, Memory);

  Type->SourceText = CSz("signed short int");
  Push(&Datatypes->Primitives, P, Memory);

  Type->SourceText = CSz("signed short int");
  Push(&Datatypes->Primitives, P, Memory);

  Type->Token.Value = CSz("float"); Type->SourceText = CSz("float");
  Push(&Datatypes->Primitives, P, Memory);

  Type->Name = CSz("double"); Type->SourceText = CSz("double");
  Push(&Datatypes->Primitives, P, Memory);
#endif


}
#endif

bonsai_function void
RegisterUnparsedCxxTypes(program_datatypes *Datatypes, memory_arena *Memory)
{
  stl_container_def Container = {};

  // Got this list of STL containers from : http://www.cplusplus.com/reference/stl/
  // I have no real idea if it's complete, or accurate, and I don't really care.

  Container.Name = CSz("array");
  Push(&Datatypes->StlContainers, Container, Memory);

  Container.Name = CSz("list");
  Push(&Datatypes->StlContainers, Container, Memory);

  Container.Name = CSz("forward_list");
  Push(&Datatypes->StlContainers, Container, Memory);

  Container.Name = CSz("dequeue");
  Push(&Datatypes->StlContainers, Container, Memory);

  Container.Name = CSz("vector");
  Push(&Datatypes->StlContainers, Container, Memory);

  Container.Name = CSz("stack");
  Push(&Datatypes->StlContainers, Container, Memory);

  Container.Name = CSz("queue");
  Push(&Datatypes->StlContainers, Container, Memory);

  Container.Name = CSz("priority_queue");
  Push(&Datatypes->StlContainers, Container, Memory);

  Container.Name = CSz("multiset");
  Push(&Datatypes->StlContainers, Container, Memory);

  Container.Name = CSz("multimap");
  Push(&Datatypes->StlContainers, Container, Memory);

  Container.Name = CSz("unordered_set");
  Push(&Datatypes->StlContainers, Container, Memory);

  Container.Name = CSz("unordered_map");
  Push(&Datatypes->StlContainers, Container, Memory);

  Container.Name = CSz("unordered_multiset");
  Push(&Datatypes->StlContainers, Container, Memory);

  Container.Name = CSz("unordered_multimap");
  Push(&Datatypes->StlContainers, Container, Memory);

  Container.Name = CSz("map");
  Push(&Datatypes->StlContainers, Container, Memory);

  Container.Name = CSz("set");
  Push(&Datatypes->StlContainers, Container, Memory);

  Container.Name = CSz("default_random_engine");
  Push(&Datatypes->StlContainers, Container, Memory);
}

#ifndef EXCLUDE_PREPROCESSOR_MAIN
#define SUCCESS_EXIT_CODE 0
#define FAILURE_EXIT_CODE 1

s32
main(s32 ArgCount_, const char** ArgStrings)
{
  memory_arena Memory_ = {};
  memory_arena* Memory = &Memory_;

  Assert(ArgCount_ > 0);
  u32 ArgCount = (u32)ArgCount_;

  parse_context Ctx = {
    .Memory = Memory,
  };

  SetupStdout(ArgCount, ArgStrings);

  arguments Args = ParseArgs(ArgStrings, ArgCount, &Ctx, Memory);


  if (!SearchForProjectRoot()) {
    Error("Couldn't find root dir, exiting."); return False;
    return FAILURE_EXIT_CODE;
  }


  if (Args.DoDebugWindow)
  {
    if (BootstrapDebugSystem() == 0)
    {
      Error("Booting debug system");
      return FAILURE_EXIT_CODE;
    }
  }

  b32 Success = True;
  if (ArgCount > 1)
  {
    Assert(Args.Files.Start == Args.Files.At);

    // TODO(Jesse): Make ParseArgs operate on the parse context directly?
    Ctx.IncludePaths = &Args.IncludePaths;

#if 0
    todo_list_info TodoInfo = {
      .People = ParseAllTodosFromFile(CSz("todos.md"), Memory),
    };
#endif

    RegisterUnparsedCxxTypes(&Ctx.Datatypes, Memory);

    while ( Args.Files.At < Args.Files.End )
    {
      counted_string CurrentFileName = *Args.Files.At;

      parser *Parser = ParserForFile(&Ctx, CurrentFileName, TokenCursorSource_RootFile);
      if (!Parser->Valid)
      {
        Error("Tokenizing File: %.*s", (s32)CurrentFileName.Count, CurrentFileName.Start);
      }

      Ctx.CurrentParser = Parser;
      ParseDatatypes(&Ctx);

      ++Args.Files.At;
    }

    Rewind(&Args.Files);

    /* RemoveAllMetaprogrammingOutput(&Ctx.AllParsers, &Args); */

    /* RegisterPrimitiveDatatypes(&Ctx.Datatypes, Memory); */


#if 0
    ITERATE_OVER(&Ctx.AllParsers)
    {
      // TODO(Jesse id: 340): We should only traverse files that were passed to us on the CLI

      parser* Parser = GET_ELEMENT(Iter);
      Rewind(Parser);

      Ctx.CurrentParser = Parser;

      if (IsMetaprogrammingOutput(Parser->Filename, Args.Outpath))
      {
        Info("Skipping %S.", Parser->Filename);
      }
      else
      {
        GoGoGadgetMetaprogramming(&Ctx, &TodoInfo);

        if (Parser->Valid)
        {
          if (Parser->OutputTokens.At != Parser->OutputTokens.Start)
          {
            TruncateToCurrentSize(&Parser->OutputTokens);
            Output(Parser->OutputTokens, Parser->Filename, Memory);
            LogSuccess("Output '%S'.", Parser->Filename);
          }
          else
          {
            Error("Tried to output an OutputTokens stream of 0 length!");
          }
        }
      }

      continue;
    }
#endif

#if 0
    ITERATE_OVER(&Ctx.Datatypes.Functions)
    {
      function_decl *Func = GET_ELEMENT(Iter);

      if (Func->ReturnType.TemplateSource.Count) { continue; }

      PushParser(&Ctx.Parser, Func->Body, parser_push_type_root);
      Func->Ast = ParseAllStatements(&Ctx);
      WalkAst(Func->Ast);

      Ctx.Parser.Depth = 0;
    }
#endif

#if 0
    WriteTodosToFile(&TodoInfo.People, Memory);
#endif
  }
  else
  {
    Warn("No files passed, exiting.");
  }


  if (Args.DoDebugWindow)
  {
    debug_state* DebugState = GetDebugState();

    DebugState->UIType = DebugUIType_CallGraph;
    DebugState->DisplayDebugMenu = True;

    DebugState->MainThreadAdvanceDebugSystem();

    while (Os.ContinueRunning)
    {
      v2 LastMouseP = Plat.MouseP;
      while ( ProcessOsMessages(&Os, &Plat) );
      Plat.MouseDP = LastMouseP - Plat.MouseP;

      DebugState->OpenDebugWindowAndLetUsDoStuff();
      BonsaiSwapBuffers(&Os);

      /* glBindFramebuffer(GL_FRAMEBUFFER, DebugState->GameGeoFBO.ID); */
      /* glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); */

      GL.BindFramebuffer(GL_FRAMEBUFFER, 0);
      GL.Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
  }

  s32 Result = !Success; // ? SUCCESS_EXIT_CODE : FAILURE_EXIT_CODE ;
  return Result;
}
#endif
