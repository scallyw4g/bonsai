#define DEBUG_PRINT (0)

#define PLATFORM_LIBRARY_AND_WINDOW_IMPLEMENTATIONS 1
#define PLATFORM_GL_IMPLEMENTATIONS 1
#define DESTRUCTOR_PARSING_TODO 0

#include <bonsai_types.h>

#define InvalidDefaultWhileParsing(Parser, ErrorMessage) \
    default: { OutputParsingError((Parser), PeekTokenPointer((Parser)), ErrorMessage); Assert(False); } break;


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

function void
Rewind(parser* Parser)
{
  Rewind(&Parser->OutputTokens);
  Rewind(&Parser->Tokens);
  Parser->LineNumber = 1;
}

function b32
IsWhitespace(c_token_type Type)
{
  b32 Result = Type == CTokenType_Newline        ||
               Type == CTokenType_EscapedNewline ||
               Type == CTokenType_CarrigeReturn  ||
               Type == CTokenType_Space;

  return Result;
}

function b32
IsWhitespace(c_token T)
{
  b32 Result = IsWhitespace(T.Type);
  return Result;
}

function b32
IsComment(c_token T)
{
  b32 Result = (T.Type == CTokenType_CommentSingleLine) || (T.Type == CTokenType_CommentMultiLineStart);
  return Result;
}

function umm
Remaining(parser* Parser)
{
  umm Result = Remaining(&Parser->Tokens);
  return Result;
}

function void
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

function void
AdvanceParser(parser* Parser)
{
  if (Remaining(&Parser->Tokens))
  {
    if (Parser->Tokens.At[0].Type == CTokenType_Newline)
    {
      ++Parser->LineNumber;
    }

    if (Remaining(&Parser->OutputTokens))
    {
      Push(*Parser->Tokens.At, &Parser->OutputTokens);
    }

    ++Parser->Tokens.At;
  }
  else
  {
    Warn("Attempted to advance parser past end of stream on file : %.*s", (u32)Parser->Filename.Count, Parser->Filename.Start);
  }
}

function void
AdvanceTo(parser* Parser, c_token* T)
{
  if (T >= Parser->Tokens.At)
  {
    if (T <= Parser->Tokens.End)
    {
      while (Parser->Tokens.At != T)
      {
        AdvanceParser(Parser);
      }
    }
    else
    {
      Warn("Called AdvanceTo with a token that was past the Parser->Tokens.End pointer on file : %.*s", (u32)Parser->Filename.Count, Parser->Filename.Start);
    }
  }
  else
  {
    Warn("Called AdvanceTo with a token that was behind the Parser->Tokens.At pointer on file : %.*s", (u32)Parser->Filename.Count, Parser->Filename.Start);
  }
}

function c_token*
PeekTokenRawPointer(parser* Parser, u32 Lookahead = 0)
{
  c_token* Result = {};
  if (Remaining(&Parser->Tokens, Lookahead))
  {
    Result = Parser->Tokens.At+Lookahead;
  }
  else
  {
    Warn("Tried to peek a token on an empty stream");
  }

  // TODO(Jesse id: 261): #if this out on prouction released builds.
  if (Result && Result->Type == CTokenType_Identifier) { Assert(Result->Value.Start); Assert(Result->Value.Count);  }

  return Result;
}

function c_token
PeekTokenRaw(parser* Parser, u32 Lookahead = 0)
{
  c_token* Pointer = PeekTokenRawPointer(Parser, Lookahead);
  c_token Result = {};
  if (Pointer) Result = *Pointer;
  return Result;
}

function u32
OffsetOfNext(parser* Parser, u32 Offset,  c_token_type Close)
{
  c_token* Next = PeekTokenRawPointer(Parser, Offset);

  while (Next && Next->Type != Close)
  {
    ++Offset;
    Next = PeekTokenRawPointer(Parser, Offset);
  }

  return Offset;
}

function c_token*
PeekTokenPointer(parser* Parser, u32 Lookahead = 0)
{
  u32 TokenHits = 0;
  u32 LocalLookahead = 0;

  c_token* Result = 0;
  while (Remaining(&Parser->Tokens, LocalLookahead))
  {
    Result = PeekTokenRawPointer(Parser, LocalLookahead);
    if ( Result->Type == CTokenType_CommentSingleLine)
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
    else if (IsWhitespace(Result->Type))
    {
    }
    else
    {
      if (TokenHits++ == Lookahead)
      {
        break;
      }
    }

    ++LocalLookahead;
  }

  if (Result && (IsWhitespace(*Result) || IsComment(*Result)))
  {
    Result = 0;
  }

  return Result;
}

function c_token
PeekToken(parser* Parser, u32 Lookahead = 0)
{
  c_token* Pointer = PeekTokenPointer(Parser, Lookahead);
  c_token Result = {};
  if (Pointer) Result = *Pointer;
  return Result;
}

function c_token
PopTokenRaw(parser* Parser)
{
  c_token Result = PeekTokenRaw(Parser);
  if (Remaining(&Parser->Tokens))
  {
    AdvanceParser(Parser);
  }

  if (Result.Type == CTokenType_Identifier && StringsMatch(CS("break_here"), Result.Value))
  {
    RuntimeBreak();
    if (Remaining(&Parser->Tokens)) { AdvanceParser(Parser); }
    Result = PeekTokenRaw(Parser);
  }

  return Result;
}

function b32
TokensRemain(parser *Parser, u32 Count = 0)
{
  b32 Result = PeekTokenPointer(Parser, Count) != 0;
  return Result;
}

function counted_string
EatUntil(parser* Parser, c_token_type Close)
{
  string_from_parser Builder = StartStringFromParser(Parser);
  while (Remaining(&Parser->Tokens))
  {
    if(PopTokenRaw(Parser).Type == Close)
    {
      break;
    }
  }
  counted_string Result = FinalizeStringFromParser(&Builder, Parser);
  return Result;
}

function counted_string
EatComment(parser* Parser, c_token_type CommentT)
{
  string_from_parser Builder = StartStringFromParser(Parser);

  switch (CommentT)
  {
    case CTokenType_CommentSingleLine:
    {
      EatUntil(Parser, CTokenType_Newline);
    } break;

    case CTokenType_CommentMultiLineStart:
    {
      EatUntil(Parser, CTokenType_CommentMultiLineEnd);
    } break;

    default:
    {
      Warn("Called EatComment on something that wasn't a comment!");
    } break;
  }

  counted_string Result = FinalizeStringFromParser(&Builder, Parser);
  return Result;
}

function counted_string
EatComment(parser* Parser)
{
  counted_string Result = EatComment(Parser, PeekTokenRaw(Parser).Type);
  return Result;
}

function void
EatWhitespaceAndComments(parser* Parser)
{
  c_token* T = PeekTokenPointer(Parser);
  if (!T) T = Parser->Tokens.End;
  AdvanceTo(Parser, T);
}

function void
EatWhitespace(parser* Parser)
{
  while (Remaining(&Parser->Tokens) &&
         IsWhitespace(PeekTokenRaw(Parser).Type))
  {
    PopTokenRaw(Parser);
  }

  return;
}

function c_token
PopToken(parser* Parser)
{
  c_token Result = {};

  c_token* Peek = PeekTokenPointer(Parser);
  if (Peek)
  {
    if (Remaining(&Parser->Tokens))
    {
      AdvanceTo(Parser, Peek);
      AdvanceParser(Parser);
    }

    Result = *Peek;
  }
  else
  {
    AdvanceTo(Parser, Parser->Tokens.End);
  }

  return Result;
}

function b32
OptionalTokenRaw(parser* Parser, c_token_type Type)
{
  b32 Result = (PeekTokenRaw(Parser).Type == Type);
  if (Result) { PopTokenRaw(Parser); }
  return Result;
}

function b32
OptionalToken(parser* Parser, c_token T)
{
  b32 Result = (PeekToken(Parser) == T);
  if (Result) { PopToken(Parser); }
  return Result;
}

function b32
OptionalToken(parser* Parser, c_token_type Type)
{
  b32 Result = (PeekToken(Parser).Type == Type);
  if (Result) { PopToken(Parser); }
  return Result;
}

function counted_string
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

function counted_string
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

function counted_string
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

function r64
ToFractional(counted_string S)
{
  r64 Result = 0;
  if (S.Count) { Result = (r64)ToU64(S) * Exp(10.0, -SafeTruncateToS32(S.Count)); }
  Assert(Result < 1.0f);
  return Result;
}

function u64
BinaryStringToU64(counted_string Bin)
{
  // TODO(Jesse id: 273): Implement me
  u64 Result = Bin.Count;
  return Result;
}

function u64
HexStringToU64(counted_string Hex)
{
  // TODO(Jesse id: 274): Implement me
  u64 Result = Hex.Count;
  return Result;
}

function r64
DealWithExponent(ansi_stream *Code, r64 OriginalValue)
{
  r64 Result = OriginalValue;

  if ( Remaining(Code) &&
       (*Code->At == 'e' || *Code->At == 'E') )
  {
    Advance(Code);

    s32 Exponent = 1;

    char Negation = *Code->At;
    if (Negation == '-')
    {
      Advance(Code);
      Exponent = -1;
    }

    Exponent = Exponent * SafeTruncateToS32(ToU64(PopNumeric(Code)));

    Result = Exp(Result, Exponent);
  }
  else
  {
    Error("Bad parser state while parsing exponential part of floating point literal.");
  }

  return Result;
}

function c_token
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
  }
  else if (IntegralPortion == 0 && *Code->At == 'b')
  {
    Advance(Code);
    Result.UnsignedValue = BinaryStringToU64(PopNumeric(Code));
  }
  else if ( *Code->At == '.' )
  {
    //
    // Float literal
    //

    Advance(Code);
    r64 Fractional = ToFractional(PopNumeric(Code));
    r64 FinalValue = (r64)IntegralPortion + Fractional; 

    if (*Code->At == 'e' || *Code->At == 'E')
    {
      FinalValue = DealWithExponent(Code,  FinalValue);
    }

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

    Result.FloatValue = FinalValue;
  }
  else if ( *Code->At == 'e' || *Code->At == 'E'  )
  {
    //
    // Float literal
    //

    Result.FloatValue = DealWithExponent(Code, (r64)IntegralPortion);
    Result.Type = CTokenType_DoubleLiteral;
  }
  else
  {
    //
    // Int literal
    //

    b32 Done = False;
    while (!Done)
    {
      char Suffix = *Code->At;
      switch (Suffix)
      {
        // For now, we just eat suffixes
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

  Result.Value.Start = Start;
  Result.Value.Count = (umm)(Code->At - Start);
  return Result;
}

function parser
TokenizeAnsiStream(ansi_stream Code, memory_arena* Memory, b32 IgnoreQuotes = False)
{
  parser Result = {
    .Filename = Code.Filename
  };

  if (!Code.Start)
  {
    Error("Input AnsiStream for %.*s is null.", (s32)Code.Filename.Count, Code.Filename.Start);
    return Result;
  }

  Result.Tokens = AllocateTokenBuffer(Memory, (u32)Megabytes(1));
  if (!Result.Tokens.Start)
  {
    Error("Allocating Token Buffer");
    return Result;
  }


  Result.OutputTokens = AllocateTokenBuffer(Memory, (u32)Megabytes(1));
  if (!Result.OutputTokens.Start)
  {
    Error("Allocating OutputTokens Buffer");
    return Result;
  }

  b32 ParsingSingleLineComment = False;
  b32 ParsingMultiLineComment = False;
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
        if (PeekToken(&Code, 1).Type == CTokenType_Newline)
        {
          PushT.Type = CTokenType_EscapedNewline;
          PushT.Value.Count = 2;
          Advance(&Code);
        }
        Advance(&Code);
      } break;

      case CTokenType_Newline:
      {
        ParsingSingleLineComment = False;
        Advance(&Code);
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

        }

      } break;

      default:
      {
        Advance(&Code);
      } break;
    }

    Assert(PushT.Type);
    Push(PushT, &Result.Tokens);

    continue;
  }


  TruncateToCurrentSize(&Result.Tokens);
  Rewind(&Result);
  Result.Valid = True;

#if 0
  while (Remaining(&Result.Tokens))
  {
    PrintToken(PopTokenRaw(&Result));
  }
  Result.Valid = False;
#endif

  return Result;
}

function parser
TokenizeString(counted_string Code, counted_string Filename, memory_arena* Memory, b32 IgnoreQuotes = False)
{
  parser Result = TokenizeAnsiStream(AnsiStream(Code, Filename), Memory, IgnoreQuotes);
  return Result;
}

function parser
TokenizeFile(counted_string Filename, memory_arena* Memory, b32 IgnoreQuotes = False)
{
  ansi_stream SourceFileStream = AnsiStreamFromFile(Filename, Memory);
  parser Result = TokenizeAnsiStream(SourceFileStream, Memory, IgnoreQuotes);
  return Result;
}

function void
TruncateAtNextLineEnd(parser* Parser, u32 Count)
{
  c_token* StartingAt = Parser->Tokens.At;

  while (Remaining(&Parser->Tokens))
  {
    if(PopTokenRaw(Parser).Type == CTokenType_Newline)
    {
      if (Count == 0)
      {
        break;
      }
      --Count;
    }
  }

  Parser->Tokens.End = Parser->Tokens.At;
  Parser->Tokens.At = StartingAt;
}

function void
RewindUntil(parser* Parser, c_token_type Type)
{
  while (Parser->Tokens.At >= Parser->Tokens.Start)
  {
    if (Parser->Tokens.At->Type == Type)
    {
      if (Remaining(Parser))
      {
        ++Parser->Tokens.At;
      }
      break;
    }
    --Parser->Tokens.At;
  }
}

function void
TruncateAtPreviousLineStart(parser* Parser, u32 Count )
{
  c_token* StartingAt = Parser->Tokens.At;

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
  Parser->Tokens.At = StartingAt;
}

function void
Dump(parser* Parser, u32 LinesToDump = u32_MAX)
{
  parser LocalParser = *Parser;
  LocalParser.OutputTokens = {};

  Rewind(&LocalParser.Tokens);
  while(Remaining(&LocalParser.Tokens) && LinesToDump > 0)
  {
    c_token T = PopTokenRaw(&LocalParser);
    PrintToken(T);

    if (T.Type == CTokenType_Newline)
    {
      --LinesToDump;
    }
  }
}

function void
OutputErrorHelperLine(parser* Parser, c_token* ErrorToken, c_token Expected, counted_string ErrorString, u32 LineNumber)
{
  Rewind(&Parser->Tokens);

  u32 ColumnCount = 0;
  while (Remaining(&Parser->Tokens))
  {

    if (Parser->Tokens.At == ErrorToken)
    {
      for (u32 ColumnIndex = 0;
          ColumnIndex < ColumnCount;
          ++ColumnIndex)
      {
        Log(" ");
      }

      b32 DoPipes = ErrorToken->Value.Count > 3;
      u32 ErrorIdentifierLength = DoPipes ? (u32)ErrorToken->Value.Count-2 : (u32)ErrorToken->Value.Count;

      if (DoPipes)
      {
        Log("|");
      }

      for (u32 ColumnIndex = 0;
          ColumnIndex < ErrorIdentifierLength;
          ++ColumnIndex)
      {
        Log("^");
      }

      if (DoPipes)
      {
        Log("|");
      }

      counted_string TokenTypeName = ToString(ErrorToken->Type);
      Log(" --> Unexpected token : %.*s", TokenTypeName.Count, TokenTypeName.Start );

      if (ErrorToken->Value.Count)
      {
        Log("(%.*s)" , ErrorToken->Value.Count, ErrorToken->Value.Start);
      }

      counted_string ExpectedTypeName = ToString(Expected.Type);
      Log(". Expected: %.*s", ExpectedTypeName.Count, ExpectedTypeName.Start);

      if (Expected.Value.Count)
      {
        Log("(%.*s)" , Expected.Value.Count, Expected.Value.Start);
      }

      Log("\n");

      for (u32 ColumnIndex = 0;
          ColumnIndex < ColumnCount + ErrorToken->Value.Count;
          ++ColumnIndex)
      {
        Log(" ");
      }

      Log("     %.*s:%u:%u: %.*s\n", Parser->Filename.Count, Parser->Filename.Start, LineNumber, ColumnCount, ErrorString.Count, ErrorString.Start);

      break;
    }

    if (Parser->Tokens.At->Type == CTokenType_Newline)
    {
    }
    else if (Parser->Tokens.At->Type == CTokenType_EscapedNewline)
    {
      ColumnCount = 0;
    }
    else if (Parser->Tokens.At->Value.Count)
    {
      for (u32 ValueIndex = 0;
          ValueIndex < Parser->Tokens.At->Value.Count;
          ++ValueIndex)
      {
        ++ColumnCount;
      }
    }
    else
    {
      Log(" ");
      ++ColumnCount;
    }

    ++Parser->Tokens.At;
  }

  return;
}

function void
OutputParsingError(parser* Parser, c_token* ErrorToken, c_token ExpectedToken, counted_string ErrorString)
{
  Assert(ErrorToken);

  u32 LinesOfContext = 4;

  Log("----\n");

  /* u32 ColumnNumber = 0; */
  /* Log("%.*s:%u:%u: Error\n", Parser->Filename.Count, Parser->Filename.Start, Parser->LineNumber, ColumnNumber); */

  parser LocalParser = *Parser;
  LocalParser.OutputTokens = {};
  LocalParser.Tokens.At = ErrorToken;

  parser LeadingLines  = LocalParser;
  parser ErrorLine     = LocalParser;
  parser TrailingLines = LocalParser;

  {
    RewindUntil(&LeadingLines, CTokenType_Newline);
    LeadingLines.Tokens.End = LeadingLines.Tokens.At;
    TruncateAtPreviousLineStart(&LeadingLines, LinesOfContext);
    Dump(&LeadingLines);
  }

  {
    TruncateAtPreviousLineStart(&ErrorLine, 0);
    TruncateAtNextLineEnd(&ErrorLine, 0);
    Dump(&ErrorLine);

    if (ErrorToken >= ErrorLine.Tokens.Start &&
        ErrorToken < ErrorLine.Tokens.End)
    {
      OutputErrorHelperLine(&ErrorLine, ErrorToken, ExpectedToken, ErrorString, LocalParser.LineNumber);
    }
    else
    {
      Error("Determining where the error occured");
      Log("%s:%u:0\n", __FILE__, __LINE__);
      Log("Error was : %.*s\n", ErrorString.Count, ErrorString.Start);
    }

  }

  {
    EatUntil(&TrailingLines, CTokenType_Newline);
    TrailingLines.Tokens.Start = TrailingLines.Tokens.At;
    TruncateAtNextLineEnd(&TrailingLines, LinesOfContext);
    Dump(&TrailingLines);
  }

  Log("----\n");

  return;
}

function void
OutputParsingError(parser* Parser, counted_string ErrorString)
{
  OutputParsingError(Parser, Parser->Tokens.At, CToken(CTokenType_Unknown), ErrorString);
  return;
}

function void
OutputParsingError(parser* Parser, c_token* ErrorToken, counted_string ErrorString)
{
  OutputParsingError(Parser, ErrorToken, CToken(CTokenType_Unknown), ErrorString);
  return;
}

function c_token
RequireToken(parser* Parser, c_token ExpectedToken)
{
  c_token* PeekedToken = PeekTokenPointer(Parser);

  c_token Result = {};
  if (PeekedToken) Result = *PeekedToken;

  if (Result.Type != ExpectedToken.Type || (ExpectedToken.Value.Count > 0 && !StringsMatch(ExpectedToken.Value, Result.Value) ))
  {
    if (PeekedToken)
    {
      OutputParsingError(Parser, PeekedToken, ExpectedToken, CS("Require Token Failed"));
    }
    else
    {
      if (Parser->Tokens.At-1 >= Parser->Tokens.Start)
      {
        OutputParsingError(Parser, Parser->Tokens.At-1, ExpectedToken, FormatCountedString(TranArena, CSz("Stream ended unexpectedly in file : %S"), Parser->Filename));
      }
      else
      {
        OutputParsingError(Parser, Parser->Tokens.At, ExpectedToken, FormatCountedString(TranArena, CSz("Stream ended unexpectedly in file : %S"), Parser->Filename));
      }
    }

    Parser->Valid = False;
    RuntimeBreak();
  }
  else
  {
    PopToken(Parser);
  }

  return Result;
}

function c_token
RequireToken(parser* Parser, c_token_type ExpectedType)
{
  c_token Result = RequireToken(Parser, CToken(ExpectedType));
  return Result;
}

function c_token
PeekToken(parser_stack *Stack, u32 TokenLookahead = 0, u32 StackLookahead = 0)
{
  c_token Result = {};

  parser *Parser = Peek(Stack, StackLookahead);
  if (Parser)
  {
    if (TokensRemain(Parser, TokenLookahead))
    {
      Result = PeekToken(Parser, TokenLookahead);
    }
    else
    {
      if (TokenLookahead)
      {
        Result = PeekToken(Stack, TokenLookahead-1, StackLookahead+1);
      }
      else
      {
        Result = PeekToken(Stack, 0, StackLookahead+1);
      }
    }
  }

  return Result;
}

function b32
OptionalToken(parser_stack *Stack, c_token Token)
{
  b32 Result = {};

  parser *Parser = Peek(Stack);
  if (TokensRemain(Parser))
  {
    Result = OptionalToken(Parser, Token);
  }
  else
  {
    PopStack(Stack);
    Result = OptionalToken(Stack, Token);
  }

  return Result;
}

function b32
OptionalToken(parser_stack *Stack, c_token_type Type)
{
  b32 Result = {};

  parser *Parser = Peek(Stack);
  if (TokensRemain(Parser))
  {
    Result = OptionalToken(Parser, Type);
  }
  else
  {
    PopStack(Stack);
    Result = OptionalToken(Stack, Type);
  }

  return Result;
}

function c_token
PopToken(parser_stack *Stack)
{
  c_token Result = {};

  parser *Parser = Peek(Stack);
  if (TokensRemain(Parser))
  {
    Result = PopToken(Parser);
  }
  else
  {
    PopStack(Stack);
    Result = PopToken(Stack);
  }

  return Result;
}

function c_token
RequireToken(parser_stack *Stack, c_token_type Type)
{
  c_token Result = {};

  parser *Parser = Peek(Stack);
  if (TokensRemain(Parser))
  {
    Result = RequireToken(Parser, Type);
  }
  else
  {
    PopStack(Stack);
    Result = RequireToken(Stack, Type);
  }

  return Result;
}

function b32
TokensRemain(parser_stack *Stack)
{
  b32 Result = PeekToken(Stack).Type != CTokenType_Unknown;
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

function counted_string
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

function counted_string
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
    Assert(Member->Type == type_variable);
    Result =
      Concat(Result,
        FormatCountedString(Memory, CSz("  %S %S;\n"),
          Member->variable.Type.Name,
          Member->variable.Name),
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

function type_def*
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

function enum_def*
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

function struct_def*
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

function datatype
GetDatatypeByName(program_datatypes* Datatypes, counted_string Name)
{
  TIMED_FUNCTION();

  struct_def *S = GetStructByType(&Datatypes->Structs, Name);
  enum_def   *E = GetEnumByType(&Datatypes->Enums, Name);
  type_def   *T = GetTypedefByAlias(&Datatypes->Typedefs, Name);

  datatype Result = {};

  if (S)
  {
    Assert(!T && !E);
    Result = Datatype(S);
  }
  else if (E)
  {
    Assert(!T && !S);
    Result = Datatype(E);
  }
  else if (T)
  {
    Assert(!E && !S);
    Result = Datatype(T);
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
    while (!Complete && Remaining(&Parser->Tokens))
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
        .Type = type_variable,
        .variable = {
          .Type.Name = RequireToken(Parser, CTokenType_Identifier).Value,
          .Name      = RequireToken(Parser, CTokenType_Identifier).Value,
        }
      };
      Push(&dUnion.CommonMembers, Decl, Memory);
    }
  }

  return dUnion;
}

function b32
DoDebugWindow(const char** ArgStrings, s32 ArgCount)
{
  b32 Result = False;
  for (s32 ArgIndex = 1;
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

function arguments
ParseArgs(const char** ArgStrings, s32 ArgCount, memory_arena* Memory)
{
  arguments Result = {
    .Outpath = CS("src/metaprogramming/output"),
    .Files = AllocateBuffer<counted_string_cursor, counted_string>((u32)ArgCount, Memory),
  };

  for (s32 ArgIndex = 1;
      ArgIndex < ArgCount;
      ++ArgIndex)
  {
    counted_string Arg = CS(ArgStrings[ArgIndex]);
    if (StringsMatch(CS("-d"), Arg) ||
        StringsMatch(CS("--do-debug-window"), Arg) )
    {
      Result.DoDebugWindow = True;
    }
    else if (StringsMatch(CS("-o"), Arg) ||
        StringsMatch(CS("--output-path"), Arg) )
    {
      if (++ArgIndex < ArgCount)
      {
        Result.Outpath = CS(ArgStrings[ArgIndex]);
      }
      else
      {
        Error("Please pass an argument to -o | --output-path");
      }

    }
    else
    {
      Push(Arg, &Result.Files);
    }
  }

  TruncateToCurrentSize(&Result.Files);
  Rewind(&Result.Files);

  return Result;
}

global_variable random_series TempFileEntropy = {3215432};

function b32
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

function b32
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

function void
DumpStringStreamToConsole(counted_string_stream* Stream)
{
  for (counted_string_iterator Iter = Iterator(Stream);
      IsValid(&Iter);
      Advance(&Iter))
  {
    counted_string Message = Iter.At->Element;
    Log("%.*s\n", Message.Count, Message.Start);
  }
}

function counted_string
EatBetween(parser* Parser, c_token_type Open, c_token_type Close)
{
  string_from_parser Builder = StartStringFromParser(Parser);

  c_token *StartToken = PeekTokenPointer(Parser);

  u32 Depth = 0;
  RequireToken(Parser, Open);

  b32 Success = False;
  while (Parser->Valid && Remaining(&Parser->Tokens))
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
    OutputParsingError(Parser, StartToken, FormatCountedString(TranArena, CSz("Unable to find closing token %S"), ToString(Close)));
    Parser->Valid = False;
  }

  counted_string Result = FinalizeStringFromParser(&Builder, Parser);
  return Result;
}

function void
EatNextScope(parser* Parser)
{
  EatBetween(Parser, CTokenType_OpenBrace, CTokenType_CloseBrace);
  return;
}

function void
EatUnionDef(parser* Parser)
{
  EatNextScope(Parser);
  OptionalToken(Parser, CTokenType_Semicolon);
  return;
}

function struct_def
StructDef(counted_string Type, counted_string Sourcefile)
{
  struct_def Result = {
    .Type = Type,
    .DefinedInFile = Sourcefile
  };

  return Result;
}

function b32
EatStar(parser* Parser)
{
  b32 Result = OptionalToken(Parser, CTokenType_Star);
  return Result;
}

function void
EatFunctionDecl(parser* Parser)
{
  EatBetween(Parser, CTokenType_OpenParen, CTokenType_CloseParen);

  if (PeekToken(Parser).Type == CTokenType_Semicolon)
  {
    RequireToken(Parser, CTokenType_Semicolon);
  }
  else
  {
    EatNextScope(Parser);
  }

  return;
}

function void
TrimUntilNewline(parser* Parser)
{
  Assert(Parser->Tokens.At == Parser->Tokens.Start);

  while (OptionalTokenRaw(Parser, CTokenType_Space));
  OptionalTokenRaw(Parser, CTokenType_Newline);
  Parser->Tokens.Start = Parser->Tokens.At;
}

function void
TrimFirstToken(parser* Parser, c_token_type TokenType)
{
  Assert(Parser->Tokens.At == Parser->Tokens.Start);
  RequireToken(Parser, TokenType);
  Parser->Tokens.Start = Parser->Tokens.At;
}

function void
TrimLastToken(parser* Parser, c_token_type TokenType)
{
  c_token* CurrentToken = Parser->Tokens.End-1;

  while (CurrentToken > Parser->Tokens.Start)
  {
    if (CurrentToken->Type == TokenType)
    {
      Parser->Tokens.End = CurrentToken-1;
      Parser->Tokens.At = Parser->Tokens.End;
      break;
    }

    --CurrentToken;
  }
}

function void
TrimTrailingWhitespace(parser* Parser)
{
  c_token* CurrentToken = Parser->Tokens.End-1;

  while (CurrentToken > Parser->Tokens.Start)
  {
    if (CurrentToken->Type == CTokenType_Space)
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

function parser
GetBodyTextForNextScope(parser* Parser)
{
  parser BodyText = *Parser;
  BodyText.OutputTokens = {};

  BodyText.Tokens.Start = BodyText.Tokens.At;
  EatNextScope(Parser);
  BodyText.Tokens.End = Parser->Tokens.At;

  TrimFirstToken(&BodyText, CTokenType_OpenBrace);
  /* TrimUntilNewline(&BodyText, CTokenType_OpenBrace); */
  TrimLastToken(&BodyText, CTokenType_CloseBrace);
  TrimTrailingWhitespace(&BodyText);
  Rewind(&BodyText.Tokens);

  return BodyText;
}

function struct_def
ParseStructBody(parser* Parser, counted_string StructName, memory_arena* Memory, program_datatypes* Datatypes);

function variable
ParseDeclaration(parser* Parser);

function void
ParseFunctionDefArgs(parser* Parser, memory_arena* Memory, function_def* Result);

function counted_string
ParseDeclarationValue(parser* Parser, variable* Decl, program_datatypes* Datatypes, memory_arena* Memory);

function struct_member
ParseStructMember(parser* Parser, counted_string StructName, memory_arena* Memory, program_datatypes* Datatypes)
{
  TIMED_FUNCTION();
  struct_member Result = {};

  c_token FirstToken = PeekToken(Parser);
  b32 Unnamed = False;
  switch(FirstToken.Type)
  {
    case CTokenType_Tilde:
    {
      RequireToken(Parser, CTokenType_Tilde);
      counted_string Name = RequireToken(Parser, CTokenType_Identifier).Value;
      Assert(StringsMatch(StructName, Name));
      EatFunctionDecl(Parser);
      Result.Type = type_struct_member_function;
      Result.struct_member_function.Type = type_struct_member_function_destructor;
    } break;

    case CTokenType_Union:
    {
      RequireToken(Parser, CTokenType_Union);
      Result.Type = type_struct_member_anonymous;
      Result.struct_member_anonymous.Body = ParseStructBody(Parser, CS("anonymous_union"), Memory, Datatypes);
      RequireToken(Parser, CTokenType_Semicolon);
      Unnamed = True;
    } break;

    case CTokenType_Struct:
    {
      RequireToken(Parser, CTokenType_Struct);
      Result.Type = type_struct_member_anonymous;
      Result.struct_member_anonymous.Body = ParseStructBody(Parser, CS("anonymous_struct"), Memory, Datatypes);
      RequireToken(Parser, CTokenType_Semicolon);
      Unnamed = True;
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
    case CTokenType_Unsigned:
    case CTokenType_Signed:
    case CTokenType_Identifier:
    {
      u32 DefKeywordsEncountered = 0;
      c_token NextToken = PeekToken(Parser);
      if ( StringsMatch(NextToken.Value, StructName) &&
           PeekToken(Parser, 1).Type == CTokenType_OpenParen)
      {
        RequireToken(Parser, CToken(StructName));
        EatFunctionDecl(Parser);
        Result.Type = type_struct_member_function;
        Result.struct_member_function.Type = type_struct_member_function_constructor;
      }
      else
      {
        variable Decl = ParseDeclaration(Parser);
        counted_string Value = ParseDeclarationValue(Parser, &Decl, Datatypes, Memory);

        if (Decl.Type.IsFunction)
        {
          Result.Type = type_struct_member_function;
          if (Decl.Type.IsOperator)
          {
            Result.struct_member_function.Type = type_struct_member_function_operator;
          }
          else if (Decl.Type.IsConstructor)
          {
            Result.struct_member_function.Type = type_struct_member_function_constructor;
          }
#if DESTRUCTOR_PARSING_TODO
          else if (Decl.IsDestructor)
          {
            Result.struct_member_function.Type = type_struct_member_function_destructor;
          }
#endif
          else
          {
            Result.struct_member_function.Type = type_struct_member_function_normal;
          }
        }
        else
        {
          Result.Type = type_variable;
          Result.variable = Decl;
        }
      }

    } break;

    InvalidDefaultWhileParsing(Parser, CS("While parsing struct member."));
  }

  Assert(Result.Type != type_struct_member_noop);
  return Result;
}

#if 0
function void
DumpCDeclStreamToConsole(struct_member_stream* Stream)
{
  for (struct_member_iterator Iter = Iterator(Stream);
      IsValid(&Iter);
      Advance(&Iter))
  {
    switch(Iter.At->Element.Type)
    {
      case type_struct_member_function:
      {
        switch(Iter.At->Element.struct_member_function.Type)
        {
          case type_struct_member_function_normal:
          {
            Log("  Function\n");
          } break;
          case type_struct_member_function_constructor:
          {
            Log("  Constructor\n");
          } break;
          case type_struct_member_function_destructor:
          {
            Log("  Destructor\n");
          } break;
          InvalidDefaultCase;
        }
      } break;

      case type_variable:
      {
        counted_string Type = Iter.At->Element.variable.Type;
        counted_string Name = Iter.At->Element.variable.Name;
        Log("  %.*s %.*s\n", Type.Count, Type.Start, Name.Count, Name.Start);
      } break;

      InvalidDefaultCase;
    }
  }
}

function void
DumpStruct(struct_def* Struct)
{
  Log("%.*s\n", Struct->Name.Count, Struct->Name.Start);
  DumpCDeclStreamToConsole(&Struct->Members);
}

function void
PrintCDecl(struct_member* Decl, struct_def_stream* ProgramStructs)
{
  switch (Decl->Type)
  {
    case type_variable:
    {
      Log("%.*s %.*s",
          Decl->variable.Type.Count, Decl->variable.Type.Start,
          Decl->variable.Name.Count, Decl->variable.Name.Start);

      Log("\n");
    } break;

    case type_struct_member_union:
    {
      for (struct_member_iterator Iter = Iterator(&Decl->struct_member_anonymous.Body.Members);
          IsValid(&Iter);
          Advance(&Iter))
      {
        if (Iter.At->Element.Type == type_variable)
        {
          struct_def* Struct = GetStructByType(ProgramStructs, Iter.At->Element.variable.Type);
          if (Struct)
          {
            PrintCDecl(&Iter.At->Element, ProgramStructs);
            DumpStruct(Struct);
          }
          else
          {
            Error("Couldn't find struct type %*.s", (u32)Iter.At->Element.variable.Name.Count, Iter.At->Element.variable.Name.Start);
          }
        }
        else
        {
          Error("Nested structs/unions and function pointers unsupported.");
        }
      }
    } break;

    InvalidDefaultCase;
  }

}
#endif

function counted_string
ConcatTokensUntil(parser* Parser, c_token_type Close, memory_arena* Memory)
{
  // TODO(Jesse  id: 225, tags: todos, easy): Rewrite with string_from_parser
  string_builder Builder = {};
  while (Remaining(&Parser->Tokens) && PeekTokenRaw(Parser).Type != Close)
  {
    Append(&Builder, PopTokenRaw(Parser).Value);
  }
  counted_string Result = Finalize(&Builder, Memory);
  return Result;
}

function counted_string
ConcatTokensUntilNewline(parser* Parser, memory_arena* Memory)
{
  counted_string Result = ConcatTokensUntil(Parser, CTokenType_Newline, Memory);
  return Result;
}

function struct_member_stream
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
        case type_variable:
        {
          if (StringsMatch(Member->variable.Type.Name, MemberType))
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

function struct_def
ParseStructBody(parser* Parser, counted_string StructName, memory_arena* Memory, program_datatypes* Datatypes)
{
  TIMED_FUNCTION();
  struct_def Result = StructDef(StructName, Parser->Filename);

  RequireToken(Parser, CTokenType_OpenBrace);

  c_token NextToken = PeekToken(Parser);

  while (Remaining(&Parser->Tokens) && NextToken.Type != CTokenType_CloseBrace)
  {
    if (NextToken.Type == CTokenType_Hash)
    {
      RequireToken(Parser, CTokenType_Hash);
      EatUntil(Parser, CTokenType_Newline);
    }
    else
    {
      struct_member Declaration = ParseStructMember(Parser, Result.Type, Memory, Datatypes);
      Push(&Result.Members, Declaration, Memory);
    }

    NextToken = PeekToken(Parser);
  }

  RequireToken(Parser, CTokenType_CloseBrace);

  return Result;
}

function void
ParseError(parser* Parser, counted_string ErrorMessage)
{
  Parser->Valid = False;
  OutputParsingError(Parser, ErrorMessage);

  RuntimeBreak();
}

function counted_string
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

function b32
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

function b32
NextTokenIsOperator(parser_stack* Stack)
{
  c_token T = PeekToken(Stack);
  b32 Result = TokenIsOperator(T.Type);
  return Result;
}

function b32
NextTokenIsOperator(parser* Parser)
{
  c_token T = PeekToken(Parser);
  b32 Result = TokenIsOperator(T.Type);
  return Result;
}

function counted_string
ParseOperator(parser* Parser)
{
  string_from_parser Builder = StartStringFromParser(Parser);

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

  counted_string Result = FinalizeStringFromParser(&Builder, Parser);
  return Result;
}

function counted_string
ParseConstantExpression(parser* Parser)
{
  string_from_parser Builder = StartStringFromParser(Parser);

  c_token NextT = PeekToken(Parser);
  switch (NextT.Type)
  {
    case CTokenType_OpenParen:
    {
      EatBetween(Parser, CTokenType_OpenParen, CTokenType_CloseParen);
    } break;

    case CTokenType_Identifier:
    {
      RequireToken(Parser, CTokenType_Identifier);

      if (PeekToken(Parser).Type == CTokenType_OpenParen)
      {
        // TODO(Jesse id: 192, tags: metaprogramming, parsing, completeness, robustness): This is a function call or macro .. make sure it's actually constant.
        EatBetween(Parser, CTokenType_OpenParen, CTokenType_CloseParen);
      }

      while ( NextTokenIsOperator(Parser) )
      {
        ParseOperator(Parser);
        RequireToken(Parser, CTokenType_Identifier);
      }

    } break;

    case CTokenType_StringLiteral:
    case CTokenType_CharLiteral:
    case CTokenType_IntLiteral:
    case CTokenType_DoubleLiteral:
    case CTokenType_FloatLiteral:
    {
      RequireToken(Parser, NextT.Type);
    } break;

    case CTokenType_Minus:
    {
      ParseIntegerConstant(Parser);
    } break;

    InvalidDefaultWhileParsing(Parser, CSz(__FUNCTION__))
  }

  counted_string Result = Trim(FinalizeStringFromParser(&Builder, Parser));
  return Result;
}

function ast_node_expression*
ParseExpression(parser_stack *Stack, memory_arena *Memory, program_datatypes *Datatypes);

function enum_def
ParseEnum(parser_stack* Stack, memory_arena* Memory, program_datatypes *Datatypes)
{
  TIMED_FUNCTION();

  counted_string EnumName = RequireToken(Stack, CTokenType_Identifier).Value;

  enum_def Enum = {
    .Name = EnumName
  };

  RequireToken(Stack, CTokenType_OpenBrace);

  c_token NextToken = {};
  b32 Done = False;
  while (!Done && TokensRemain(Stack))
  {
    enum_member Field = {};
    Field.Name = RequireToken(Stack, CTokenType_Identifier).Value;

    if (OptionalToken(Stack, CTokenType_Equals))
    {
      Field.Value = ParseExpression(Stack, Memory, Datatypes);
    }

    Push(&Enum.Members, Field, Memory);

    if(OptionalToken(Stack, CTokenType_Comma))
    {
      if (OptionalToken(Stack, CTokenType_CloseBrace))
      {
        Done = True;
      }
    }
    else
    {
      RequireToken(Stack, CTokenType_CloseBrace);
      Done = True;
    }
  }

  return Enum;
}

function b32
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

function void
ParseReferencesIndirectionAndPossibleFunctionPointerness(parser_stack *Stack, type_spec_2 *Result)
{
  b32 Done = False;
  while (!Done)
  {
    c_token T = PeekToken(Stack);

    switch (T.Type)
    {
      case CTokenType_OpenParen:
      {
        RequireToken(Stack, CTokenType_OpenParen);
        RequireToken(Stack, CTokenType_Star);
        Result->FunctionPointerTypeName = RequireToken(Stack, CTokenType_Identifier).Value;
        RequireToken(Stack, CTokenType_CloseParen);

        // TODO(Jesse, id: 256, tags: immediate): How do we handle this?
        EatBetween(Peek(Stack), CTokenType_OpenParen, CTokenType_CloseParen);
        Result->IsFunctionPointer = True;
        Done = True;
      } break;

      case CTokenType_Ampersand:
      {
        RequireToken(Stack, CTokenType_Ampersand);
        ++Result->ReferenceLevel;
      } break;

      case CTokenType_Star:
      {
        RequireToken(Stack, CTokenType_Star);
        ++Result->IndirectionLevel;
      } break;

      case CTokenType_Arrow: // Structs can have the same 
      case CTokenType_Dot:

      case CTokenType_Comma: // This gets called on macro-functions too, so this is valid
      case CTokenType_CloseParen: // Valid closing token during a cast
      case CTokenType_Identifier:
      {
        Done = True;
      } break;

      InvalidDefaultWhileParsing(Peek(Stack), CSz("Invalid token while parsing indirection and reference levels."));
    }
  }

  return;
}

function type_spec_2
ParseTypeSpecifier2(parser_stack *Stack)
{
  type_spec_2 Result = {};

  b32 Done = False;
  while (!Done)
  {
    c_token T = PeekToken(Stack);

    switch (T.Type)
    {
      case CTokenType_At:
      {
        RequireToken(Stack, CTokenType_At);
        Result.IsMetaTemplateVar = True;
      } break;

      case CTokenType_Colon:
      {
        Assert(!Result.Namespace.Count);
        RequireToken(Stack, CTokenType_Colon);
        RequireToken(Stack, CTokenType_Colon);
        Result.Namespace = Result.Name;
        Result.Name.Count = 0;
      } break;

      case CTokenType_ThreadLocal:
      {
        RequireToken(Stack, CTokenType_ThreadLocal);
        Result.ThreadLocal = True;
      } break;

      case CTokenType_Const:
      {
        RequireToken(Stack, CTokenType_Const);
        Result.Const = True;
      } break;

      case CTokenType_Static:
      {
        RequireToken(Stack, CTokenType_Static);
        Result.Static = True;
      } break;

      case CTokenType_Volatile:
      {
        RequireToken(Stack, CTokenType_Volatile);
        Result.Volatile = True;
      } break;

      case CTokenType_Long:
      {
        RequireToken(Stack, CTokenType_Long);
        Result.Long = True;
      } break;

      case CTokenType_Signed:
      {
        RequireToken(Stack, CTokenType_Signed);
        Result.Signed = True;
      } break;

      case CTokenType_Unsigned:
      {
        RequireToken(Stack, CTokenType_Unsigned);
        Result.Unsigned = True;
      } break;

      case CTokenType_Enum:
      {
        RequireToken(Stack, CTokenType_Enum);
        Result.Enum = True;
      } break;

      case CTokenType_Struct:
      {
        RequireToken(Stack, CTokenType_Struct);
        Result.Struct = True;
      } break;

      case CTokenType_Double:
      case CTokenType_Float:
      case CTokenType_Char:
      case CTokenType_Int:
      case CTokenType_Void:
      case CTokenType_M128:
      case CTokenType_Bool:
      case CTokenType_Auto:
      case CTokenType_Identifier: // TODO(Jesse id: 263): In the case of identifiers, should we check that it's actually a datatype here?
      {
        Result.Name = RequireToken(Stack, T.Type).Value;
        ParseReferencesIndirectionAndPossibleFunctionPointerness(Stack, &Result);
        Done = True;
      } break;

      InvalidDefaultWhileParsing(Peek(Stack), CSz("Invalid token encountered while parsing a type specifier"));
    }

      // TODO(Jesse, id: 257, tags: immediate): How do we handle this?
    EatWhitespaceAndComments(Peek(Stack));
    continue;
  }

  return Result;
}

function type_spec
ParseTypeSpecifier(parser *Parser)
{
  type_spec Result = {};

  string_from_parser Builder = StartStringFromParser(Parser);

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

      case CTokenType_OpenParen:
      {
        if (PeekToken(Parser, 1).Type == CTokenType_Star)
        {
          // Function pointer
          EatBetween(Parser, CTokenType_OpenParen, CTokenType_CloseParen);
          EatBetween(Parser, CTokenType_OpenParen, CTokenType_CloseParen);
          break;
        }
        else
        {
          Done = True;
        }

        Result.IsFunction = True;
      } break;

      case CTokenType_Colon:
      {
        // TODO(Jesse id: 217): Disallow namespaces?
        RequireToken(Parser, CTokenType_Colon);
        RequireToken(Parser, CTokenType_Colon);
        Result.Namespace = Result.Name;
        Result.Name.Count = 0;
      } break;

      case CTokenType_LT:
      {
        Result.TemplateSource = EatBetween(Parser, CTokenType_LT, CTokenType_GT);
      } break;

      case CTokenType_Ampersand:
      {
        RequireToken(Parser, CTokenType_Ampersand);
        ++Result.ReferenceLevel;
      } break;

      case CTokenType_Star:
      {
        RequireToken(Parser, CTokenType_Star);
        ++Result.IndirectionLevel;
      } break;

      case CTokenType_Const:
      {
        RequireToken(Parser, CTokenType_Const);
        Result.Const = True;
      } break;

      case CTokenType_Static:
      {
        RequireToken(Parser, CTokenType_Static);
        Result.IsStatic = True;
      } break;

      case CTokenType_Volatile:
      {
        RequireToken(Parser, CTokenType_Volatile);
        Result.Volatile = True;
      } break;

      case CTokenType_Long:
      {
        RequireToken(Parser, CTokenType_Long);
      } break;

      case CTokenType_Unsigned:
      {
        RequireToken(Parser, CTokenType_Unsigned);
        Result.Unsigned = True;
      } break;

      case CTokenType_ThreadLocal:
      {
        RequireToken(Parser, CTokenType_ThreadLocal);
        Result.ThreadLocal = True;
      } break;

      case CTokenType_Struct:
      {
        // TODO(Jesse id: 198): Do we ignore this?
        RequireToken(Parser, CTokenType_Struct);
      } break;

      case CTokenType_Auto:
      case CTokenType_M128:
      case CTokenType_Bool:
      case CTokenType_Void:
      case CTokenType_Double:
      case CTokenType_Float:
      case CTokenType_Char:
      case CTokenType_Int:
      case CTokenType_Identifier:
      {
        if (StringsMatch(T.Value, CS("unsigned")))
        {
          // TODO(Jesse id: 197): Have some way of coalescing this to our internal types?
          Result.Unsigned = True;
          RequireToken(Parser, CTokenType_Identifier);
        }
        else if (StringsMatch(T.Value, CSz("operator")) )
        {
          Result.IsOperator = True;
          Result.IsFunction = True;
          RequireToken(Parser, CTokenType_Identifier);
          Result.Name = ParseOperator(Parser);
          Done = true;
        }
        else if (StringsMatch(T.Value, CSz("APIENTRY")) )
        {
          // TODO(Jesse id: 218,  tags: id_205): See #205
          RequireToken(Parser, CTokenType_Identifier);
        }
        else if (StringsMatch(T.Value, CSz("exported_function")) )
        {
          // TODO(Jesse id: 227, tags: id_205): See #205
          Result.IsFunction = True;
          Result.IsExported = True;
          RequireToken(Parser, CTokenType_Identifier);
        }
        else if (StringsMatch(T.Value, CSz("function")) )
        {
          // TODO(Jesse id: 219,  tags: id_205): See #205
          Result.IsFunction = True;
          Result.IsStatic = True;
          RequireToken(Parser, CTokenType_Identifier);
        }
        else if (StringsMatch(T.Value, CSz("inline")) )
        {
          Result.Inline = True;
          RequireToken(Parser, CTokenType_Identifier);
        }
        else if (StringsMatch(T.Value, CSz("debug_global")) )
        {
          // TODO(Jesse id: 208, tags: id_205): See #205
          Result.IsStatic = True;
          RequireToken(Parser, CTokenType_Identifier);
        }
        else if (StringsMatch(T.Value, CSz("local_persist")) )
        {
          // TODO(Jesse id: 203, tags: id_205): See #205
          Result.IsStatic = True;
          RequireToken(Parser, CTokenType_Identifier);
        }
        else if (StringsMatch(T.Value, CSz("global_variable")) )
        {
          // TODO(Jesse id: 204 tags: id_205): See #205
          Result.IsStatic = True;
          RequireToken(Parser, CTokenType_Identifier);
        }
        else if (StringsMatch(T.Value, CSz("template")) )
        {
          Result.IsTemplateFunction = True;
          RequireToken(Parser, CTokenType_Identifier);
        }
        else
        {
          if (Result.Name.Count)
          {
            Done = True;
          }
          else
          {
            Result.Name = RequireToken(Parser, T.Type).Value; // TODO(Jesse id: 232, tags: metaprogramming, function_parsing): Should this be the name if it's a constructor?
            if (StringsMatch(Result.Name, Result.Namespace))
            {
              Result.IsFunction = True;
              Result.IsConstructor = True;
              Done = True;
            }
          }
        }

      } break;

      case CTokenType_Comma:
      case CTokenType_Semicolon:
      case CTokenType_CloseParen:
      {
        Done = True;
      } break;

      InvalidDefaultWhileParsing(Parser, CSz("Invalid token encountered while parsing a type specifier"));
    }

    EatWhitespaceAndComments(Parser);
    continue;
  }

  Result.SourceText = FinalizeStringFromParser(&Builder, Parser);

  return Result;
}

function variable
ParseDeclaration(parser *Parser)
{
  variable Result = {
    .Type = ParseTypeSpecifier(Parser),
  };

  string_from_parser Builder = StartStringFromParser(Parser);

  b32 Done = False;
  while (!Done)
  {
    c_token NextToken = PeekToken(Parser);

    switch (NextToken.Type)
    {
      case CTokenType_OpenParen:
      {
        Assert(Result.Type.IsFunction == True);
        Done = True;
      } break;

      case CTokenType_Colon:
      {
        // TODO(Jesse id: 217): Disallow namespaces?
        RequireToken(Parser, CTokenType_Colon);
        RequireToken(Parser, CTokenType_Colon);
        Result.Namespace = Result.Name;
        Result.Name.Count = 0;
      } break;


#if DESTRUCTOR_PARSING_TODO
      case CTokenType_Tilde:
      {
        RequireToken(Parser, CTokenType_Tilde);
        Result.Type = RequireToken(Parser, CTokenType_Identifier).Value; // TODO(Jesse id: 231, tags: metaprogramming, function_parsing): Does this make sense to put this into the type?
        Result.IsFunction = True;
        Result.IsDestructor = True;
      } break;
#endif

      case CTokenType_Dot:
      {
        RequireToken(Parser, NextToken.Type);
        // TODO(Jesse id: 240): Do we want to store this value?
        RequireToken(Parser, CTokenType_Identifier);
      } break;

      case CTokenType_OpenBracket:
      {
        RequireToken(Parser, CTokenType_OpenBracket);
        if ( OptionalToken(Parser, CTokenType_CloseBracket) )
        {
          Result.StaticBufferSize = CSz("(dynamic)");
        }
        else
        {
          Result.StaticBufferSize = ParseConstantExpression(Parser);
          RequireToken(Parser, CTokenType_CloseBracket);
        }
      } break;

      case CTokenType_Identifier:
      {
        Assert (Result.Type.Name.Count);
        Result.Name = RequireToken(Parser, CTokenType_Identifier).Value;

        c_token_type PeekedType = PeekToken(Parser).Type;
        if ( PeekedType == CTokenType_OpenBracket ||
             PeekedType == CTokenType_Colon )
        {
        }
        else
        {
          Done = True;
        }

      } break;

      case CTokenType_Equals:
      case CTokenType_Semicolon:
      case CTokenType_Comma:
      case CTokenType_CloseParen:
      {
        Done = True;
      } break;

      InvalidDefaultWhileParsing(Parser, CSz("Invalid token encountered while parsing variable name"));
    }

    EatWhitespaceAndComments(Parser);
    continue;
  }

  Result.SourceText = FinalizeStringFromParser(&Builder, Parser);

  return Result;
}

function void
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

function void
OptionalPostfixOperator(parser *Parser)
{
  c_token T = PeekToken(Parser);
  if ( T.Type == CTokenType_Increment ||
       T.Type == CTokenType_Decrement)
  {
    PopToken(Parser);
  }
}

function void
ParseFunctionDefArgs(parser* Parser, memory_arena* Memory, function_def* Result)
{
  b32 Done = False;

  if (PeekToken(Parser) == CToken(CTokenType_CloseParen))
  {
    Done = True;
  }

  if ( PeekToken(Parser).Type == CTokenType_Void &&
       PeekToken(Parser, 1) == CToken(CTokenType_CloseParen) )
  {
    RequireToken(Parser, CTokenType_Void);
    Done = True;
  }

  while ( !Done && Remaining(&Parser->Tokens) )
  {
    variable Arg = ParseDeclaration(Parser);
    if (Memory)
    {
      Push(&Result->Args, Arg, Memory);
    }

    if ( PeekToken(Parser).Type == CTokenType_Equals )
    {
      EatUntil(Parser, CTokenType_Semicolon);
    }

    if (!OptionalToken(Parser, CTokenType_Comma))
    {
      Done = True;
    }

    if (OptionalToken(Parser, CTokenType_Ellipsis))
    {
      Result->Prototype.IsVariadic = True;
      Done = True;
    }

    continue;
  }

  RequireToken(Parser, CTokenType_CloseParen);

  return;
}

function void
EatStructDef(parser* Parser)
{
  RequireToken(Parser, CTokenType_Identifier);
  if ( !OptionalToken(Parser, CTokenType_Semicolon) )
  {
    EatNextScope(Parser);
  }

  RequireToken(Parser, CTokenType_Semicolon);
}

function void
ParseDatatypeDef(parser_stack *Stack, program_datatypes* Datatypes, memory_arena* Memory)
{
  parser *Parser = Peek(Stack);
  c_token TypeSpecifier = PopToken(Parser);

  switch(TypeSpecifier.Type)
  {
    case CTokenType_Enum:
    {
      enum_def Enum = ParseEnum(Stack, Memory, Datatypes);
      Push(&Datatypes->Enums, Enum, Memory);
    } break;

    case CTokenType_Struct:
    {
      counted_string StructName = RequireToken(Parser, CTokenType_Identifier).Value;
      if ( PeekToken(Parser).Type == CTokenType_OpenBrace )
      {
        struct_def S = ParseStructBody(Parser, StructName, Memory, Datatypes);
        Push(&Datatypes->Structs, S, Memory);
      }
    } break;

    case CTokenType_Union:
    {
      counted_string UnionName = RequireToken(Parser, CTokenType_Identifier).Value;
      struct_def S = ParseStructBody(Parser, UnionName, Memory, Datatypes);
      S.IsUnion = True;
      Push(&Datatypes->Structs, S, Memory);
    } break;

    InvalidDefaultWhileParsing(Parser, CSz("Tried parsing a datatypes that wasn't a struct, enum or union!"));
  }

  RequireToken(Parser, CTokenType_Semicolon);
}

function void
ParseAndPushTypedef(parser_stack *Stack, type_def_stream* Typedefs, memory_arena* Memory)
{
  type_spec_2 Type = ParseTypeSpecifier2(Stack);
  counted_string  Alias = {};

  if (Type.IsFunctionPointer)
  {
    Alias = Type.FunctionPointerTypeName;
  }
  else
  {
    Alias = RequireToken(Stack, CTokenType_Identifier).Value;
  }
  RequireToken(Stack, CTokenType_Semicolon);

  type_def Typedef = {
    .Type = Type,
    .Alias = Alias,
  };

  Push(Typedefs, Typedef, Memory);
}

function void
ParseTypedef(parser_stack *Stack, program_datatypes* Datatypes, memory_arena* Memory)
{
  RequireToken(Stack, CTokenType_Typedef);

  if ( OptionalToken(Stack, CTokenType_Struct) )
  {
    if (PeekToken(Stack).Type == CTokenType_OpenBrace)
    {
      // TODO(Jesse, id: 259, tags: immediate): Make ParseStructBody take the stack
      struct_def S = ParseStructBody(Peek(Stack), CS(""), Memory, Datatypes);
      S.Type = RequireToken(Stack, CTokenType_Identifier).Value;
      RequireToken(Stack, CTokenType_Semicolon);
      Push(&Datatypes->Structs, S, Memory);
    }
    else
    {
      ParseAndPushTypedef(Stack, &Datatypes->Typedefs, Memory);
    }
  }
  else
  {
    ParseAndPushTypedef(Stack, &Datatypes->Typedefs, Memory);
  }

  return;
}


function function_def*
GetByPrototypeName(counted_string Name, function_def_stream* Stream)
{
  function_def* Result = {};
  ITERATE_OVER(Stream)
  {
    function_def* Current = GET_ELEMENT(Iter);
    if (StringsMatch(Current->Prototype.Name, Name))
    {
      Result = Current;
      break;
    }
  }

  return Result;
}

function ast_node_variable_def*
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

function macro_def*
GetByName(macro_def_stream* Stream, counted_string Name)
{
  macro_def* Result = {};
  ITERATE_OVER(Stream)
  {
    macro_def* Current = GET_ELEMENT(Iter);
    if (StringsMatch(Current->Name, Name))
    {
      Result = Current;
      Assert(Result->Parser.Tokens.At == Result->Parser.Tokens.Start);
      break;
    }
  }

  return Result;
}

function variable*
GetByName(counted_string Name, variable_stream* Stream)
{
  variable* Result = {};
  ITERATE_OVER(Stream)
  {
    variable* Current = GET_ELEMENT(Iter);
    if (StringsMatch(Current->Name, Name))
    {
      Result = Current;
      break;
    }
  }
  return Result;
}

function ast_node*
ParseSymbol(parser *Parser, memory_arena *Memory)
{
  ast_node *Result = {};
  ast_node_symbol *Node = AllocateAndCastTo(ast_node_symbol, &Result, Memory);
  Node->Token = RequireToken(Parser, CTokenType_Identifier);
  return Result;
}

function ast_node*
ParseIgnoredNode(parser *Parser, memory_arena *Memory)
{
  ast_node *Result = {};
  ast_node_ignored *Node = AllocateAndCastTo(ast_node_ignored, &Result, Memory);
  Node->Token = PopToken(Parser);
  return Result;
}

function ast_node*
ParseFunctionArgument(parser *Parser, memory_arena *Memory, function_def_stream *FunctionPrototypes);

#if 0
function void
ReduceToTypeSpec(ast_node* InputNode, ast_node_variable_def_stream *Locals, type_spec *ResultType)
{
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

  return;
}
#endif

function ast_node*
ParseInitializerList(parser *Parser, memory_arena *Memory)
{
  ast_node* Result = {};

  ast_node_initializer_list *Node = AllocateAndCastTo(ast_node_initializer_list, &Result, Memory);

  EatNextScope(Parser);

  return Result;
}

function b32
IsTypeIdentifier(counted_string TypeName, program_datatypes *Datatypes)
{
  b32 Result = GetDatatypeByName(Datatypes, TypeName).Type != type_datatype_noop;
  return Result;
}

function statement_list*
ParseAllStatements(parser_stack *Stack, memory_arena *Memory, program_datatypes *Datatypes);

function statement_list*
ParseScope(parser_stack *Stack, memory_arena *Memory, program_datatypes *Datatypes)
{
  RequireToken(Stack, CTokenType_OpenBrace);
  statement_list *Result = ParseAllStatements(Stack, Memory, Datatypes);
  return Result;
}

function void
ParseSingleStatement(parser_stack *Stack, memory_arena *Memory, program_datatypes *Datatypes, statement_list *Result)
{
  b32 Done = False;
  while (!Done && TokensRemain(Stack))
  {
    const c_token T = PeekToken(Stack);
    switch (T.Type)
    {
      case CTokenType_IntLiteral:
      case CTokenType_DoubleLiteral:
      case CTokenType_FloatLiteral:
      {
        Result->LHS = ParseExpression(Stack, Memory, Datatypes);
      } break;

      case CTokenType_Identifier:
      {
        macro_def *Macro = GetByName(&Datatypes->Macros, T.Value);
        if (Macro)
        {

          /* if ( StringsMatch(Macro->Name, CSz("BONSAI_API_WORKER_THREAD_CALLBACK_NAME")) ) */
          /* { */
          /*   RuntimeBreak(); */
          /* } */

          switch (Macro->Type)
          {
            case type_macro_keyword:
            {
              RequireToken(Stack, CTokenType_Identifier);
              PushStack(Stack, Macro->Parser);
              Done = False;
            } break;

            case type_macro_function:
            {
              Result->LHS = ParseExpression(Stack, Memory, Datatypes);
            } break;

            case type_macro_noop: { InvalidCodePath(); } break;
          }
        }
        else
        {
          if (Result->LHS)
          {
            DebugPrint(Result->LHS);
            Assert(False);
          }

          Result->LHS = ParseExpression(Stack, Memory, Datatypes);
        }
      } break;

      case CTokenType_Equals:
      {
        RequireToken(Stack, T.Type);
        if (PeekToken(Stack).Type == CTokenType_OpenBrace)
        {
          // Initializer List
          EatNextScope(Peek(Stack));
        }
        else
        {
          Assert(!Result->RHS);
          Result->RHS = AllocateProtection(statement_list, Memory, 1, False);
          ParseSingleStatement(Stack, Memory, Datatypes, Result->RHS);
        }
        Done = True;
      } break;

      case CTokenType_For:
      {
        RequireToken(Stack, T.Type);
#if 0
        RequireToken(Stack, CTokenType_OpenParen);
        Result->Next = AllocateProtection(statement_list, Memory, 1, False);
        ParseSingleStatement(Stack, Memory, Datatypes, Result->Next);
        ParseExpression(Stack, Memory, Datatypes);
        RequireToken(Stack, CTokenType_Semicolon);
        ParseExpression(Stack, Memory, Datatypes);
        RequireToken(Stack, CTokenType_CloseParen);
#else
        EatBetween(Peek(Stack), CTokenType_OpenParen, CTokenType_CloseParen);
#endif
        Done = True;
      } break;

      case CTokenType_If:
      case CTokenType_While:
      case CTokenType_Switch:
      {
        RequireToken(Stack, T.Type);
        Result->LHS = ParseExpression(Stack, Memory, Datatypes);
        Done = True;
      } break;

      case CTokenType_Case:
      {
        RequireToken(Stack, CTokenType_Case);
        Result->LHS = ParseExpression(Stack, Memory, Datatypes);
        RequireToken(Stack, CTokenType_Colon);
        Done = True;
      } break;

      case CTokenType_Default:
      {
        RequireToken(Stack, CTokenType_Default);
        RequireToken(Stack, CTokenType_Colon);
      } break;

      // What do we do with these?
      case CTokenType_Else:
      case CTokenType_Continue:
      {
        RequireToken(Stack, T.Type);
      } break;

      case CTokenType_Return:
      {
        RequireToken(Stack, T.Type);
        Result->LHS = ParseExpression(Stack, Memory, Datatypes);
      } break;

      case CTokenType_OpenBrace:
      {
        Result->RHS = ParseScope(Stack, Memory, Datatypes);
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
          OutputParsingError(Peek(Stack), CSz(""));
          RuntimeBreak();
        }

        Result->LHS = ParseExpression(Stack, Memory, Datatypes);
        Done = True;
      } break;

      case CTokenType_Colon:
      case CTokenType_CloseParen:
      case CTokenType_Break:
      case CTokenType_CloseBrace:
      case CTokenType_Semicolon:
      {
        RequireToken(Stack, T.Type);
        Done = True;
      } break;

      case CTokenType_Goto:
      {
        RequireToken(Stack, T.Type);
        RequireToken(Stack, CTokenType_Identifier);
      } break;

      case CTokenType_Hash:
      {
        RequireToken(Stack, T.Type);
        EatUntil(Peek(Stack), CTokenType_Newline);
      } break;

      case CTokenType_Meta:
      {
        RequireToken(Stack, T.Type);
        EatBetween(Peek(Stack), CTokenType_OpenParen, CTokenType_CloseParen);
      } break;

      case CTokenType_Asm:
      {
        RequireToken(Stack, T.Type);
        OptionalToken(Stack, CTokenType_Volatile);
        EatBetween(Peek(Stack), CTokenType_OpenParen, CTokenType_CloseParen);
      } break;

      InvalidDefaultWhileParsing(Peek(Stack), CSz("Invalid token encountered while parsing statement."));
    }
  }

}

function statement_list*
ParseSingleStatement(parser_stack *Stack, memory_arena *Memory, program_datatypes *Datatypes)
{
  statement_list *Result = AllocateProtection(statement_list, Memory, 1, False);
  ParseSingleStatement(Stack, Memory, Datatypes, Result);
  return Result;
}

function statement_list*
ParseAllStatements(parser_stack *Stack, memory_arena *Memory, program_datatypes *Datatypes)
{
  statement_list *Result = 0;

  statement_list **Current = &Result;

  parser *Parser = Peek(Stack); // TODO(Jesse id: 254): Remove this
  while ( Remaining(&Parser->Tokens) )
  {
    Parser = Peek(Stack); // TODO(Jesse id: 255): Remove this

    Assert(!(*Current));
    *Current = ParseSingleStatement(Stack, Memory, Datatypes);
    while (*Current)
    {
      Current = &(*Current)->Next;
    }
    Assert(!(*Current));
    EatWhitespaceAndComments(Parser);
    continue;
  }

  return Result;
}

// #define SafeConvert(T, Node) Node->T; Assert(Node->Type == type_##T); Node->Type = type_##T

function ast_node*
ParseFunctionCall(parser_stack *Stack, counted_string FunctionName, memory_arena* Memory, program_datatypes *Datatypes);

function ast_node_type_specifier*
ParseTypeSpecifierNode(parser_stack *Stack, memory_arena *Memory, ast_node_expression *Result, program_datatypes *Datatypes, datatype *Data = 0)
{
  Assert(Result && !Result->Value);
  ast_node_type_specifier *Node = AllocateAndCastTo(ast_node_type_specifier, &Result->Value, Memory);

  Node->Datatype = Data ? *Data :
                          GetDatatypeByName(Datatypes, Node->TypeSpec.Name);

  Node->TypeSpec = ParseTypeSpecifier2(Stack);
  if (PeekToken(Stack).Type == CTokenType_Identifier) // Macro functions and casts can contain type specifiers that don't have a named value
  {
    Node->Name = ParseExpression(Stack, Memory, Datatypes);
  }

  return Node;
}

function b32
ParseSymbol(parser_stack *Stack, memory_arena *Memory, program_datatypes *Datatypes, ast_node_expression* Result, b32 SymbolIsNeverTypeSpecifier = True)
{
  b32 PushedStack = False;

  c_token T = PeekToken(Stack);
  Assert(T.Type == CTokenType_Identifier);

  macro_def *Macro = GetByName(&Datatypes->Macros, T.Value);
  if (Macro)
  {

    c_token MacroNameToken = RequireToken(Stack, CTokenType_Identifier);

    /* if ( StringsMatch(MacroNameToken.Value, CSz("BONSAI_API_WORKER_THREAD_CALLBACK_NAME")) ) */
    /* { */
    /*   RuntimeBreak(); */
    /* } */


    switch (Macro->Type)
    {
      case type_macro_keyword:
      {
        PushStack(Stack, Macro->Parser);
        PushedStack = True;
      } break;

      case type_macro_function:
      {
        Assert(!Result->Value);
        if (PeekToken(Stack).Type == CTokenType_OpenParen)
        {
          Result->Value = ParseFunctionCall(Stack, MacroNameToken.Value, Memory, Datatypes);
        }
        else
        {
          ast_node_symbol *Node = AllocateAndCastTo(ast_node_symbol, &Result->Value, Memory);
          Node->Token = MacroNameToken;
        }
      } break;

      case type_macro_noop: { InvalidCodePath(); } break;
    }

    return PushedStack;
  }

  if ( PeekToken(Stack, 1).Type == CTokenType_LT )
  {
    b32 ValidTemplateList = True;
    u32 Lookahead = 1;
    while (++Lookahead) // Intentionally starts at 2
    {
      c_token TemplateParamListTestT = PeekToken(Stack, Lookahead);
      if (TemplateParamListTestT.Type == CTokenType_Identifier)
      {
        if (!IsTypeIdentifier(TemplateParamListTestT.Value, Datatypes))
        {
          ValidTemplateList = False;
          break;
        }
      }
      else if (TemplateParamListTestT.Type == CTokenType_Comma)
      {
      }
      else if (TemplateParamListTestT.Type == CTokenType_GT)
      {
        break;
      }
      else
      {
        ValidTemplateList = False;
        break;
      }
    }

    if (ValidTemplateList)
    {
      EatBetween(Peek(Stack), CTokenType_LT, CTokenType_GT);
    }
  }

  if ( PeekToken(Stack, 1).Type  == CTokenType_OpenParen )
  {
    c_token SymbolToken = RequireToken(Stack, CTokenType_Identifier);
    Assert(!Result->Value);
    Result->Value = ParseFunctionCall(Stack, SymbolToken.Value, Memory, Datatypes);
  }
  else
  {
    datatype TestDatatype = GetDatatypeByName(Datatypes, T.Value);
    if (SymbolIsNeverTypeSpecifier || TestDatatype.Type == type_datatype_noop)
    {
      if (Result->Value)
      {
        DebugPrint(Result);
        Assert(False);
      }
      ast_node_symbol *Node = AllocateAndCastTo(ast_node_symbol, &Result->Value, Memory);
      Node->Token = RequireToken(Stack, CTokenType_Identifier);
    }
    else
    {
      ParseTypeSpecifierNode(Stack, Memory, Result, Datatypes, &TestDatatype);
    }
  }

  return PushedStack;
}

function void
ParseExpression(parser_stack *Stack, memory_arena *Memory, program_datatypes *Datatypes, ast_node_expression* Result)
{
  b32 ParsePushedStack = False;
  do
  {
    ParsePushedStack = False;
    const c_token T = PeekToken(Stack);
    switch (T.Type)
    {
      case CTokenType_Meta:
      {
        RequireToken(Stack, CTokenType_Meta);
        EatBetween(Peek(Stack), CTokenType_OpenParen, CTokenType_CloseParen);
      } break;

      case CTokenType_IntLiteral:
      case CTokenType_DoubleLiteral:
      case CTokenType_FloatLiteral:
      {
        ast_node_literal *Node = AllocateAndCastTo(ast_node_literal, &Result->Value, Memory);
        Node->Token = RequireToken(Stack, T.Type);
      } break;

      case CTokenType_Identifier:
      {
        ParsePushedStack = ParseSymbol(Stack, Memory, Datatypes, Result, False);
      } break;

      case CTokenType_Arrow:
      case CTokenType_Dot:
      {
        ast_node_access *Node = AllocateAndCastTo(ast_node_access, &Result->Value, Memory);
        Node->AccessType = RequireToken(Stack, T.Type);

        Node->Symbol = AllocateProtection(ast_node_expression, Memory, 1, False);
        ParsePushedStack = ParseSymbol(Stack, Memory, Datatypes, Node->Symbol);
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
      case CTokenType_Unsigned:
      case CTokenType_Signed:
      {
        ParseTypeSpecifierNode(Stack, Memory, Result, Datatypes);
      } break;

      case CTokenType_OpenParen:
      {
        RequireToken(Stack, T.Type);
        Assert(!Result->Value);
        ast_node_parenthesized *Node = AllocateAndCastTo(ast_node_parenthesized, &Result->Value, Memory);
        Node->Expr = ParseExpression(Stack, Memory, Datatypes);
        RequireToken(Stack, CTokenType_CloseParen);

        if (Node->Expr->Value->Type == type_ast_node_type_specifier)
        {
          Node->IsCast = True;
          Node->CastValue = ParseExpression(Stack, Memory, Datatypes);
        }

      } break;

      case CTokenType_Hash:
      {
        RequireToken(Stack, T.Type);
        EatUntil( Peek(Stack), CTokenType_Newline);
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
        ast_node_operator* Node = AllocateAndCastTo(ast_node_operator, &Result->Value, Memory);
        Node->Token = RequireToken(Stack, T.Type);
        Node->Operand = ParseExpression(Stack, Memory, Datatypes);
        if (T.Type == CTokenType_OpenBracket)
          { RequireToken(Stack, CTokenType_CloseBracket); }
      } break;

      case CTokenType_Question:
      {
        ast_node_operator* Node = AllocateAndCastTo(ast_node_operator, &Result->Value, Memory);
        Node->Token = RequireToken(Stack, T.Type);
        Node->Operand = ParseExpression(Stack, Memory, Datatypes);
        RequireToken(Stack, CTokenType_Colon);
        ParseExpression(Stack, Memory, Datatypes); // TODO(Jesse id: 260): Currently ignoring the second half of a ternary .. we should probably not do this
      } break;

      case CTokenType_CharLiteral:
      case CTokenType_StringLiteral:
      {
        Assert(!Result->Value);
        ast_node_literal *Node = AllocateAndCastTo(ast_node_literal, &Result->Value, Memory);
        Node->Token = RequireToken(Stack, T.Type);

        while ( PeekToken(Stack).Type == CTokenType_StringLiteral ||
                PeekToken(Stack).Type == CTokenType_Identifier )
        {
          c_token NextT = PopToken(Stack);
          switch (NextT.Type)
          {
            case CTokenType_StringLiteral:
            {
              Node->Token = CToken(CTokenType_StringLiteral, Concat(Node->Token.Value, NextT.Value, Memory)); // TODO(Jesse, id: 264, tags: memory_leak): This should use a string builder
            } break;

            case CTokenType_Identifier:
            {
              // TODO(Jesse id: 264): Once we have proper macro expansion, this can be expanded and concatenated to the string as well.
              macro_def *Macro = GetByName(&Datatypes->Macros, NextT.Value);
              switch(Macro->Type)
              {
                case type_macro_keyword:
                {
                } break;

                case type_macro_function:
                {
                  EatBetween(Peek(Stack), CTokenType_OpenParen, CTokenType_CloseParen);
                } break;

                case type_macro_noop: { InvalidCodePath(); } break;
              }
            } break;

            InvalidDefaultWhileParsing(Peek(Stack), CSz("Expected macro identifier or string literal."));
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

      InvalidDefaultWhileParsing(Peek(Stack), CSz("Invalid token while parsing expression."));
    }
  } while (ParsePushedStack);

  if (NextTokenIsOperator(Stack))
  {
    Assert(!Result->Next);
    Result->Next = ParseExpression(Stack, Memory, Datatypes);
  }
}

function ast_node_expression*
ParseExpression(parser_stack *Stack, memory_arena *Memory, program_datatypes *Datatypes)
{
  ast_node_expression *Result = AllocateProtection(ast_node_expression, Memory, 1, False);
  ParseExpression(Stack, Memory, Datatypes, Result);
  return Result;
}

function ast_node*
ParseFunctionCall(parser_stack *Stack, counted_string FunctionName, memory_arena* Memory, program_datatypes *Datatypes)
{
  ast_node *Result = {};
  ast_node_function_call *Node = AllocateAndCastTo(ast_node_function_call, &Result, Memory);
  Node->Name = FunctionName;
  Node->Prototype = GetByPrototypeName(FunctionName, &Datatypes->Functions);

  RequireToken(Stack, CTokenType_OpenParen);

  b32 Done = False;
  while (!Done)
  {
    ast_node_expression *Arg = ParseExpression(Stack, Memory, Datatypes);
    Push(&Node->Args, *Arg, Memory); // TODO(Jesse id: 253): This leaks the allocation .. do we care?

    if(OptionalToken(Stack, CTokenType_Comma))
    {
    }
    else
    {
      RequireToken(Stack, CTokenType_CloseParen);
      Done = True;
    }
  }
 
  return Result;
}

function counted_string
ParseDeclarationValue(parser* Parser, variable* Decl, program_datatypes* Datatypes, memory_arena* Memory)
{
  string_from_parser Builder = StartStringFromParser(Parser);

  b32 EatSemicolon = False;
  if ( PeekToken(Parser).Type == CTokenType_Equals )
  {
    EatUntil(Parser, CTokenType_Semicolon);
  }
  else if (OptionalToken(Parser, CTokenType_OpenParen))
  {
    // TODO(Jesse id: 212): Should we check that the function was defined using the 'function' or 'exported_function' keywords and emit a warning otherwise?
    function_def Func = { .Prototype = *Decl };
    ParseFunctionDefArgs(Parser, Memory, &Func);
    if (PeekToken(Parser).Type == CTokenType_OpenBrace)
    {
      // Function declaration : void FunctionNmae( arg A1, arg, A2) { .. function_body .. }
      Func.Body = GetBodyTextForNextScope(Parser);
      Push(&Datatypes->Functions, Func, Memory);
    }
    else
    {
      // Pre-declaration : void FunctionName(arg Arg1, arg Arg2);
      EatSemicolon = True;
    }
  }
  else
  {
    // Uninitialized declaration
    EatSemicolon = True;
  }

  counted_string Result = FinalizeStringFromParser(&Builder, Parser);

  if (EatSemicolon) RequireToken(Parser, CTokenType_Semicolon);

  return Result;
}

function void
ParseDatatypes(parser_stack *Stack, program_datatypes* Datatypes, memory_arena* Memory)
{
  while (Remaining(Peek(Stack)))
  {
    c_token T = PeekToken(Stack);

    switch(T.Type)
    {
      case CTokenType_Meta:
      {
        RequireToken(Stack, CTokenType_Meta);
        EatBetween(Peek(Stack), CTokenType_OpenParen, CTokenType_CloseParen);
      } break;

      case CTokenType_Hash:
      {
        RequireToken(Stack, CTokenType_Hash);

        if ( OptionalToken(Stack, CToken(CSz("define"))) )

        {
          macro_def Macro = {
            .Parser = {
              .Valid = True,
              .Filename = Peek(Stack)->Filename,
              .LineNumber = Peek(Stack)->LineNumber,
            }
          };

          Macro.Name = RequireToken(Stack, CTokenType_Identifier).Value;

          if (PeekTokenRaw(Peek(Stack)).Type == CTokenType_OpenParen)
          {
            Macro.Type = type_macro_function;
            EatBetween(Peek(Stack), CTokenType_OpenParen, CTokenType_CloseParen);
          }
          else
          {
            Macro.Type = type_macro_keyword;
          }

          parser *Parser = Peek(Stack);
          Macro.Parser.Tokens.Start = Parser->Tokens.At;
          Macro.Parser.Tokens.At = Parser->Tokens.At;
          EatUntil(Parser, CTokenType_Newline);
          Macro.Parser.Tokens.End = Parser->Tokens.At;

          Push(&Datatypes->Macros, Macro, Memory);
        }
        else
        {
          EatUntil(Peek(Stack), CTokenType_Newline);
        }
      } break;

      case CTokenType_Semicolon:
      {
        RequireToken(Stack, CTokenType_Semicolon);
      } break;

      case CTokenType_Struct:
      case CTokenType_Enum:
      case CTokenType_Union:
      {
        ParseDatatypeDef(Stack, Datatypes, Memory);
      } break;

      case CTokenType_Typedef:
      {
        ParseTypedef(Stack, Datatypes, Memory);
      } break;

      case CTokenType_Using:
      {
        EatUntil(Peek(Stack), CTokenType_Semicolon);
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
      case CTokenType_Unsigned:
      case CTokenType_Signed:
      {
        // Global variable decl
        variable Decl = ParseDeclaration(Peek(Stack));
        counted_string Value = ParseDeclarationValue(Peek(Stack), &Decl, Datatypes, Memory);
      } break;

      case CTokenType_Identifier:
      {
        if (PeekToken(Stack, 1).Type == CTokenType_OpenParen)
        {
          // TODO(Jesse id: 210, tags: id_205, metaprogramming, completeness): This is a function-macro call .. I think always..?
          RequireToken(Stack, CTokenType_Identifier);
          EatBetween(Peek(Stack), CTokenType_OpenParen, CTokenType_CloseParen);
          OptionalToken(Stack, CTokenType_Semicolon);
        }
        else
        {
          // Global variable decl
          variable Decl = ParseDeclaration(Peek(Stack));
          counted_string Value = ParseDeclarationValue(Peek(Stack), &Decl, Datatypes, Memory);
        }
      } break;

      InvalidDefaultWhileParsing(Peek(Stack), CSz("Invalid token encountered while parsing."));
    }

    EatWhitespaceAndComments(Peek(Stack));
    continue;
  }

  return;
}

function parser_cursor
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

function parser_cursor
TokenizeAllFiles(counted_string_cursor* Filenames, memory_arena* Memory)
{
  TIMED_FUNCTION();
  Assert(Filenames->At == Filenames->Start);

  parser_cursor Result = AllocateTokenizedFiles((u32)Count(Filenames), Memory);
  while ( Filenames->At < Filenames->End )
  {
    counted_string CurrentFile = *Filenames->At;

    parser Parser = TokenizeFile(CurrentFile, Memory);
    if (Parser.Valid)
    {
      Rewind(&Parser);
      Push(Parser, &Result);
    }
    else
    {
      Error("Tokenizing File: %.*s", (s32)CurrentFile.Count, CurrentFile.Start);
    }

    ++Filenames->At;
  }

  TruncateToCurrentSize(&Result);
  Rewind(&Result);

  return Result;
}

function void
DoMetaprogramming(parser* Parser, metaprogramming_info* MetaInfo, todo_list_info* TodoInfo, memory_arena* Memory);

function void
DoWorkToOutputThisStuff(parser* Parser, counted_string OutputForThisParser, counted_string NewFilename, metaprogramming_info* MetaInfo, todo_list_info* TodoInfo, memory_arena* Memory, b32 IsInlineCode = False)
{
  TIMED_FUNCTION();

  if (!Parser->Valid)
  {
    Error("Bad parser state");
    return;
  }

  counted_string OutputPath = {};

  // TODO(Jesse id: 182, tags: high_priority) This should respect Args.Outpath passed in!
  if (PeekTokenRaw(Parser).Type == CTokenType_Newline &&
      PeekTokenRaw(Parser, 1).Type == CTokenType_Hash &&
      PeekTokenRaw(Parser, 2) == CToken(CS("include")))
  {
    RequireToken(Parser, CToken(CTokenType_Hash));
    RequireToken(Parser, CToken(CS("include")));
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

    Push(CToken(CTokenType_Newline), &Parser->OutputTokens);
    Push(CToken(CTokenType_Hash), &Parser->OutputTokens);
    Push(CToken(CS("include")), &Parser->OutputTokens);
    Push(CToken(CTokenType_Space), &Parser->OutputTokens);

    Push(CToken(CTokenType_LT), &Parser->OutputTokens);
    Push(CToken(CS("metaprogramming")), &Parser->OutputTokens);
    Push(CToken(CTokenType_FSlash), &Parser->OutputTokens);
    Push(CToken(CS("output")), &Parser->OutputTokens);
    Push(CToken(CTokenType_FSlash), &Parser->OutputTokens);
    Push(CToken(NewFilename), &Parser->OutputTokens);
    Push(CToken(CTokenType_GT), &Parser->OutputTokens);
  }


  parser OutputParse = TokenizeString(OutputForThisParser, OutputPath, Memory);

  if (!IsInlineCode)
  {
    parser_stack Stack = {};
    PushStack(&Stack, OutputParse);
    ParseDatatypes(&Stack, &MetaInfo->Datatypes, Memory);
  }
  else
  {
    // TODO(Jesse, id: 226, tags: metaprogramming, output): Should we handle this differently?
    Warn("Not parsing inlined code for %.*s", (u32)OutputPath.Count, OutputPath.Start);
  }

  DoMetaprogramming(&OutputParse, MetaInfo, TodoInfo, Memory);

  return;
}

// TODO(Jesse, id: 113, tags: cleanup): Remove these?
debug_global platform Plat = {};
debug_global os Os = {};

/* TODO(Jesse, id: 112, tags: bootstrap_debug_system, copy_paste): This is
 * copy-pasted from the callgraph tests .. should we be able to call this from
 * anywhere?  It's also in the platform layer
 */
function b32
BootstrapDebugSystem(b32 OpenDebugWindow)
{
  shared_lib DebugLib = OpenLibrary(DEFAULT_DEBUG_LIB);
  if (!DebugLib) { Error("Loading DebugLib :( "); return False; }

  GetDebugState = (get_debug_state_proc)GetProcFromLib(DebugLib, "GetDebugState_Internal");
  if (!GetDebugState) { Error("Retreiving GetDebugState from Debug Lib :( "); return False; }

  if (OpenDebugWindow)
  {
    s32 DebugFlags = GLX_CONTEXT_DEBUG_BIT_ARB;
    b32 WindowSuccess = OpenAndInitializeWindow(&Os, &Plat, DebugFlags);
    if (!WindowSuccess) { Error("Initializing Window :( "); return False; }
    Assert(Os.Window);
    AssertNoGlErrors;

    InitializeOpenGlExtensions(&Os);

    b32 ShadingLanguageIsRecentEnough = CheckShadingLanguageVersion();
    if (!ShadingLanguageIsRecentEnough) {  return False; }
  }

  debug_init_debug_system_proc InitDebugSystem = (debug_init_debug_system_proc)GetProcFromLib(DebugLib, "InitDebugSystem");
  if (!InitDebugSystem) { Error("Retreiving InitDebugSystem from Debug Lib :( "); return False; }
  InitDebugSystem(OpenDebugWindow);

  debug_state* DebugState = GetDebugState();
  DebugState->DebugDoScopeProfiling = True;
  DebugState->Plat = &Plat;

  glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
  glClearDepth(1.0f);

  glBindFramebuffer(GL_FRAMEBUFFER, DebugState->GameGeoFBO.ID);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  return True;
}

function meta_func_arg*
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

function meta_func*
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

function counted_string*
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

function person*
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

function tagged_counted_string_stream*
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

function todo*
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

function tag*
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

function todo*
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

function tag*
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

function person*
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

function todo
Todo(counted_string Id, counted_string Value, b32 FoundInCodebase)
{
  todo Result = { .Id = Id, .Value = Value, .FoundInCodebase = FoundInCodebase };
  return Result;
}

function person_stream
ParseAllTodosFromFile(counted_string Filename, memory_arena* Memory)
{
  person_stream People = {};

  parser Parser_ = TokenizeFile(Filename, Memory, True);
  parser* Parser = &Parser_;

  while (Remaining(&Parser->Tokens))
  {
    RequireToken(Parser, CTokenType_Hash);
    counted_string PersonName = RequireToken(Parser, CTokenType_Identifier).Value;

    person* Person = GetExistingOrCreate(&People, PersonName, Memory);
    while (OptionalToken(Parser, CTokenType_Hash))
    {
      RequireToken(Parser, CTokenType_Hash);
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

function counted_string
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

function counted_string
GetNameForStructMember(struct_member* Decl)
{
  counted_string Result = {};

  switch (Decl->Type)
  {
    case type_struct_member_function:
    {
      // TODO(Jesse id: 189 tags: metaprogramming, parsing): Parse out function names?
      Result = CSz("(unnamed function)");
    } break;

    case type_variable:
    {
      Result = Decl->variable.Name;
    } break;

    case type_struct_member_anonymous:
    {
      Result = CSz("(anonymous struct or union)");
    } break;

    InvalidDefaultCase;
  }

  return Result;
}

function counted_string
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

function counted_string
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

function void
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

function meta_transform_op
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

function meta_func_arg
ReplacementPattern(counted_string Match, datatype Data)
{
  meta_func_arg Result = {
    .Match = Match,
    .Data = Data
  };

  return Result;
}

function meta_func_arg_stream
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

function counted_string
Execute(meta_func* Func, meta_func_arg_stream *Args, metaprogramming_info* MetaInfo, memory_arena* Memory);

// TODO(Jesse id: 222, tags: immediate, parsing, metaprogramming) : Re-add [[nodiscard]] here
function counted_string
Execute(counted_string FuncName, parser Scope, meta_func_arg_stream* ReplacePatterns, metaprogramming_info* MetaInfo, memory_arena* Memory)
{
  program_datatypes* Datatypes = &MetaInfo->Datatypes;
  meta_func_stream* FunctionDefs = &MetaInfo->FunctionDefs;

  Rewind(&Scope.Tokens);

  string_builder OutputBuilder = {};
  while (Remaining(&Scope.Tokens))
  {
    c_token BodyToken = PopTokenRaw(&Scope);

    if ( BodyToken.Type == CTokenType_StringLiteral )
    {
      parser StringParse = TokenizeString(BodyToken.Value, Scope.Filename, Memory, True);
      counted_string Code = Execute(FuncName, StringParse, ReplacePatterns, MetaInfo, Memory);
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
                counted_string Code = Execute(FuncName, StructScope, ReplacePatterns, MetaInfo, Memory);
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
                  counted_string Code = Execute(FuncName, EnumScope, ReplacePatterns, MetaInfo, Memory);
                  Append(&OutputBuilder, Code);
                } break;

                case type_struct_member:
                {
                  switch (Replace->Data.struct_member->Type)
                  {
                    case type_variable:
                    {
                      enum_def *E = GetEnumByType(&Datatypes->Enums, Replace->Data.struct_member->variable.Type.Name);
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

                        counted_string Code = Execute(FuncName, EnumScope, &NewArgs, MetaInfo, Memory);
                        Append(&OutputBuilder, Code);
                      }
                    } break;

                    case type_struct_member_noop:
                    case type_struct_member_function:
                    case type_struct_member_anonymous:
                    {
                    }
                  }
                } break;

                case type_struct_def:
                {
                } break;

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

                case  type_struct_def:
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

                    case type_variable:
                    {
                      TypeName = Replace->Data.struct_member->variable.Type.Name;
                    } break;

                    case type_struct_member_function:
                    {
                      TypeName = CSz("(function)");
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

                    case type_struct_member_function:
                    {
                    } break;

                    case type_variable:
                    {
                      if ( ContainingConstraint.Count &&
                           !StringsMatch(Member->variable.Type.Name, ContainingConstraint) )
                      {
                        // Containing constraint failed
                      }
                      else
                      {
                        meta_func_arg_stream NewArgs = CopyStream(ReplacePatterns, Memory);
                        Push(&NewArgs, ReplacementPattern(MatchPattern, Datatype(Member)), Memory);
                        counted_string StructFieldOutput = Execute(FuncName, MapMemberScope, &NewArgs, MetaInfo, Memory);
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
                        if (UnionMember->Type == type_variable)
                        {
                          struct_def* Struct = GetStructByType(&Datatypes->Structs, UnionMember->variable.Type.Name);
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
                              counted_string StructFieldOutput = Execute(FuncName, MapMemberScope, &NewArgs, MetaInfo, Memory);
                              Append(&OutputBuilder, StructFieldOutput);
                            }
                          }
                          else
                          {
                            counted_string Name = UnionMember->variable.Type.Name;
                            counted_string ParentStructName = Replace->Data.struct_def->Type;
                            Warn("Couldn't find struct type '%.*s' in union parent '%.*s'.", (u32)Name.Count, Name.Start, (u32)ParentStructName.Count, ParentStructName.Start);
                          }

                        }
                        else
                        {
                          Error("Nested structs/unions and function pointers unsupported.");
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
                  counted_string EnumFieldOutput = Execute(FuncName, NextScope, &NewArgs, MetaInfo, Memory);
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
            counted_string NestedCode = Execute(NestedFunc, &NewArgs, MetaInfo, Memory);
            Append(&OutputBuilder, NestedCode);
          }
          else
          {
            Error("Recursive function calls not supported.");
          }
        }
        else
        {
          ParseError(&Scope, CSz("Unable to resolve function argument."));
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

function counted_string
Execute(meta_func* Func, meta_func_arg_stream *Args, metaprogramming_info* MetaInfo, memory_arena* Memory)
{
  counted_string Result = Execute(Func->Name, Func->Body, Args, MetaInfo, Memory);
  return Result;
}

function void
ConcatStreams(counted_string_stream* S1, counted_string_stream* S2, memory_arena* Memory)
{
  ITERATE_OVER(S2)
  {
    counted_string* Element = GET_ELEMENT(Iter);
    Push(S1, *Element, Memory);
  }
  return;
}

function b32
IsMetaprogrammingOutput(counted_string Filename, counted_string OutputDirectory)
{
  b32 Result = Contains(Filename, OutputDirectory);
  return Result;
}

function counted_string_stream
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

function meta_func
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

function void
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

function counted_string
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

function void
DoMetaprogramming(parser* Parser, metaprogramming_info* MetaInfo, todo_list_info* TodoInfo, memory_arena* Memory)
{
  program_datatypes* Datatypes = &MetaInfo->Datatypes;
  meta_func_stream* FunctionDefs = &MetaInfo->FunctionDefs;

  person_stream* People = &TodoInfo->People;
  tagged_counted_string_stream_stream* NameLists = &TodoInfo->NameLists;

  Rewind(Parser);
  while (Parser->Valid && Remaining(&Parser->Tokens))
  {
    const c_token NextToken = PeekTokenRaw(Parser);

    switch( NextToken.Type )
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
              IdValue = RequireToken(Parser, CTokenType_Identifier).Value;
            }
            else
            {
              if (!IdValue.Count)
              {
                GeneratedNewId = True;
                IdValue = CS(++LargestIdFoundInFile);
                Push(CToken(CS(" id: ")), &Parser->OutputTokens);
                Push(CToken(IdValue), &Parser->OutputTokens);
              }

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

            counted_string TodoValue = NextToken.Type == CTokenType_CommentSingleLine ?
              Trim(EatUntil(Parser, CTokenType_Newline)) :
              ParseMultiLineTodoValue(Parser, Memory);

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
        {
          RequireToken(Parser, CTokenType_OpenParen);

          counted_string DirectiveString = RequireToken(Parser, CTokenType_Identifier).Value;
          metaprogramming_directive Directive = MetaprogrammingDirective(DirectiveString);
          switch (Directive)
          {
            case polymorphic_func:
            {
              /* function_def F = ParseFunctionDef(Parser, Memory); */
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

                datatype Arg = GetDatatypeByName(&MetaInfo->Datatypes, ArgType);
                meta_func_arg_stream Args = {};
                Push(&Args, ReplacementPattern(ArgName, Arg), Memory);
                counted_string Code = Execute(&Func, &Args, MetaInfo, Memory);

                RequireToken(Parser, CTokenType_CloseParen);
                if (Code.Count)
                {
                  counted_string OutfileName = GenerateOutfileNameFor(Func.Name, ArgType, Memory, GetRandomString(8, Hash(&Code), Memory));
                  DoWorkToOutputThisStuff(Parser, Code, OutfileName, MetaInfo, TodoInfo, Memory, True);
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
                  counted_string Code = Execute(&StructFunc, &Args, MetaInfo, Memory);
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
                  counted_string Code = Execute(&EnumFunc, &Args, MetaInfo, Memory);
                  Append(&OutputBuilder, Code);
                }
              }


              counted_string Code = Finalize(&OutputBuilder, Memory);
              counted_string OutfileName = GenerateOutfileNameFor(ToString(Directive), CSz("debug_print"), Memory);

              DoWorkToOutputThisStuff(Parser, Code, OutfileName, MetaInfo, TodoInfo, Memory);

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
                DoWorkToOutputThisStuff(Parser, Code, OutfileName, MetaInfo, TodoInfo, Memory);

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

                /* Info("Calling function : %.*s(%.*s)", (u32)Func->Name.Count, Func->Name.Start, (u32)DatatypeName.Count, DatatypeName.Start); */
                datatype Arg = GetDatatypeByName(&MetaInfo->Datatypes, DatatypeName);
                meta_func_arg_stream Args = {};
                Push(&Args, ReplacementPattern(Func->ArgName, Arg), Memory);
                counted_string Code = Execute(Func, &Args, MetaInfo, Memory);

                if (Code.Count)
                {
                  counted_string OutfileName = GenerateOutfileNameFor(Func->Name, DatatypeName, Memory);
                  DoWorkToOutputThisStuff(Parser, Code, OutfileName, MetaInfo, TodoInfo, Memory);
                }
                else
                {
                  Warn("Unable to generate code for meta_func %.*s", (u32)Func->Name.Count, Func->Name.Start);
                }
              }
              else
              {
                Error("Couldn't resolve %.*s to a metaprogramming directive or function name", (u32)DirectiveString.Count, DirectiveString.Start);
              }
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

function void
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

function void
ParseDatatypes(parser_cursor Files_in, program_datatypes* Datatypes, memory_arena* Memory)
{
  parser_cursor* Files = &Files_in;

  for (u32 ParserIndex = 0;
      ParserIndex < (u32)Count(Files);
      ++ParserIndex)
  {
    parser* Parser = Files->Start+ParserIndex;

    parser_stack Stack = {};
    PushStack(&Stack, *Parser);
    ParseDatatypes(&Stack, Datatypes, Memory);
  }

  return;
}

function void
ParseFunctionBodiesIntoAsts(program_datatypes *Datatypes, memory_arena *Memory)
{
  ITERATE_OVER(&Datatypes->Functions)
  {
    function_def *Func = GET_ELEMENT(Iter);

    parser_stack Stack = {};
    PushStack(&Stack, Func->Body);

#if 0
    if (StringsMatch(Func->Prototype.Name, CSz("PickChunk")))
    {
      RuntimeBreak();
    }
#endif

    Func->Ast = ParseAllStatements(&Stack, Memory, Datatypes);
    Assert(Func->Ast);
  }
}

#if 0
function void
Traverse(ast_node* InputNode)
{
  ast_node* Current = InputNode;

  while (Current)
  {
    switch (Current->Type)
    {
      case type_ast_node_noop:
      {
        InvalidCodePath();
      } break;

      case type_ast_node_symbol:
      {
      } break;

      case type_ast_node_function_call:
      {
        ast_node_function_call *Node = SafeCast(ast_node_function_call, Current);
#if 0
        DebugPrint(Node);
#endif
      } return;

      case type_ast_node_expression:
      {
      } break;

      case type_ast_node_expression:
      {
      } break;

      case type_ast_node_literal:
      {
      } break;

      case type_ast_node_scope:
      {
      } break;

      case type_ast_node_initializer_list:
      {
      } break;

      case type_ast_node_ignored:
      {
      } break;

      case type_ast_node_variable_def:
      {
      } break;
    }

    meta(
      func (ast_node AstNodeDef)
      {
        switch (Current->Type)
        {
          (
            AstNodeDef.map_members(Member).containing(ast_node as ChildNodes)
            {
              case type_(Member.type):
              {
                (
                 ChildNodes.map_members(ChildMember)
                 {
                   Traverse(Current->(Member.name).(ChildMember.name));
                 }
                )
              } break;
            }
          )

          default: {} break;
        }
      }
    )
#include <metaprogramming/output/anonymous_function_ast_node_FXU79quh.h>

    Current = (ast_node*)Current->Next;
  }

  return;
}
#endif

#ifndef EXCLUDE_PREPROCESSOR_MAIN
#define SUCCESS_EXIT_CODE 0
#define FAILURE_EXIT_CODE 1

s32
main(s32 ArgCount, const char** ArgStrings)
{
  setbuf(stdout, NULL);
  setbuf(stderr, NULL);

  b32 ShouldOpenDebugWindow = DoDebugWindow(ArgStrings, ArgCount);
  if (ShouldOpenDebugWindow)
  {
    if (!BootstrapDebugSystem(ShouldOpenDebugWindow))
    {
      Error("Booting debug system");
      return FAILURE_EXIT_CODE;
    }
  }

  b32 Success = True;
  if (ArgCount > 1)
  {
    memory_arena Memory_ = {};
    memory_arena* Memory = &Memory_;

    arguments Args = ParseArgs(ArgStrings, ArgCount, Memory);
    Assert(Args.Files.Start == Args.Files.At);
    Assert(Args.DoDebugWindow == ShouldOpenDebugWindow);

    parser_cursor ParsedFiles = TokenizeAllFiles(&Args.Files, Memory);

    metaprogramming_info MetaInfo = {};

    todo_list_info TodoInfo = {
      .People = ParseAllTodosFromFile(CSz("todos.md"), Memory),
    };

    RemoveAllMetaprogrammingOutput(&ParsedFiles, &Args);

    ParseDatatypes(ParsedFiles, &MetaInfo.Datatypes, Memory);

    for (u32 ParserIndex = 0;
        ParserIndex < Count(&ParsedFiles);
        ++ParserIndex)
    {
      parser* Parser = ParsedFiles.Start+ParserIndex;
      Assert(Parser->Valid);

      if (IsMetaprogrammingOutput(Parser->Filename, Args.Outpath))
      {
        Info("Skipping %.*s", (u32)Parser->Filename.Count, Parser->Filename.Start);
        continue;
      }

      DoMetaprogramming(Parser, &MetaInfo, &TodoInfo, Memory);

      if (Parser->Valid)
      {
        TruncateToCurrentSize(&Parser->OutputTokens);
        Output(Parser->OutputTokens, Parser->Filename, Memory);
      }

      continue;
    }

    ParseFunctionBodiesIntoAsts(&MetaInfo.Datatypes, Memory);

    WriteTodosToFile(&TodoInfo.People, Memory);
  }
  else
  {
    Warn("No files passed, exiting.");
  }


  if (ShouldOpenDebugWindow)
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

      glBindFramebuffer(GL_FRAMEBUFFER, 0);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
  }

  s32 Result = Success ? SUCCESS_EXIT_CODE : FAILURE_EXIT_CODE ;
  return Result;
}
#endif
