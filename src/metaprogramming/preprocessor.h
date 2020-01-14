#define BONSAI_NO_MUTEX_TRACKING
#define BONSAI_NO_PUSH_METADATA
#define BONSAI_NO_DEBUG_MEMORY_ALLOCATOR

#include <bonsai_types.h>
#include <unix_platform.cpp>

#define InitSentinel(Sentinel) \
{ \
  Sentinel.Next = &Sentinel; \
  Sentinel.Prev = &Sentinel; \
}

#define DList_Push(Stream, Container)      \
{                                          \
  Container->Prev = &Stream->Sentinel;     \
  Container->Next = Stream->Sentinel.Next; \
  Stream->Sentinel.Next->Prev = Container; \
  Stream->Sentinel.Next = Container;       \
}

enum c_token_type
{
  CTokenType_Unknown,

  CTokenType_Comment,
  CTokenType_Identifier,
  CTokenType_String,

  CTokenType_OpenBracket   = '[',
  CTokenType_CloseBracket  = ']',
  CTokenType_OpenBrace     = '{',
  CTokenType_CloseBrace    = '}',
  CTokenType_OpenParen     = '(',
  CTokenType_CloseParen    = ')',
  CTokenType_Dot           = '.',
  CTokenType_Comma         = ',',
  CTokenType_Semicolon     = ';',
  CTokenType_Colon         = ':',
  CTokenType_Hash          = '#',
  CTokenType_Space         = ' ',
  CTokenType_Star          = '*',
  CTokenType_Ampersand     = '&',
  CTokenType_SingleQuote   = '\'',
  CTokenType_DoubleQuote   = '"',
  CTokenType_Equals        = '=',
  CTokenType_LT            = '<',
  CTokenType_GT            = '>',
  CTokenType_Plus          = '+',
  CTokenType_Minus         = '-',
  CTokenType_Percent       = '%',
  CTokenType_Bang          = '!',
  CTokenType_Hat           = '^',
  CTokenType_Question      = '?',
  CTokenType_FSlash        = '/',
  CTokenType_BSlash        = '\\',
  CTokenType_Tilde         = '~',
  CTokenType_Backtick      = '`',
  CTokenType_Newline       = '\n',
  CTokenType_CarrigeReturn = '\r',
  CTokenType_EOF           = EOF,
};

struct c_token
{
  c_token_type Type;
  counted_string Value;
};

struct c_token_buffer
{
  c_token* Start;
  c_token* At;
  c_token* End;
};

enum d_union_flags
{
  d_union_flag_none,
  d_union_flag_enum_only
};

struct d_union_member
{
  counted_string Type;
  counted_string Name;
  d_union_flags Flags;
  d_union_member* Next;
  d_union_member* Prev;
};

struct d_union_decl
{
  counted_string Name;
  d_union_member Sentinel;
};

// TODO(Jesse): Generate these using a directive
struct string_chunk
{
  counted_string String;
  string_chunk* Next;
  string_chunk* Prev;
};

// TODO(Jesse): Generate these using a directive
struct string_stream
{
  string_chunk Sentinel;
};

struct string_iterator
{
  string_stream* Stream;
  string_chunk* At;
};

struct c_parse_result
{
  b32 Valid;
  c_token_buffer Tokens;

  u32 StructCount;
  counted_string FileName;
};

struct static_string_buffer
{
  counted_string* Start;
  counted_string* End;
  counted_string* At;
};

struct arguments
{
  counted_string OutPath;
  static_string_buffer Files;
};

struct tokenized_files
{
  u32 StructCount;

  c_parse_result* Start;
  c_parse_result* End;
  c_parse_result* At;
};

enum c_decl_function_type
{
  CFunctionType_Normal,
  CFunctionType_Constructor,
  CFunctionType_Destructor,
};

struct c_decl_function
{
  c_decl_function_type Type;
  u32 CurrentlyUnused;
};

struct c_decl_variable
{
  counted_string Type;
  counted_string Name;
};

#include <metaprogramming/output/preprocessor.cpp>

d_union(c_decl,
{
  c_decl_variable;
  c_decl_function;
})

// TODO(Jesse): Generate these using a directive
struct c_decl_stream_chunk
{
  c_decl Element;
  c_decl_stream_chunk* Next;
  c_decl_stream_chunk* Prev;
};

// TODO(Jesse): Generate these using a directive
struct c_decl_stream
{
  c_decl_stream_chunk Sentinel;
};

struct struct_def
{
  counted_string Name;
  c_decl_stream Fields;
};

struct struct_cursor
{
  struct_def** Start;
  struct_def** End;
  struct_def** At;
};

struct struct_defs
{
  u32 Count;
  struct_def** Defs;
};

struct c_decl_iterator
{
  c_decl_stream* Stream;
  c_decl_stream_chunk* At;
};

function const char*
ToString(c_token_type T)
{
  const char* Result = "CTokenType_Unknown";
  switch (T)
  {
    case CTokenType_Unknown:
    {
      Result = "CTokenType_Unknown";
    } break;
    case CTokenType_Comment:
    {
      Result = "CTokenType_Comment";
    } break;
    case CTokenType_Identifier:
    {
      Result = "CTokenType_Identifier";
    } break;
    case CTokenType_String:
    {
      Result = "CTokenType_String";
    } break;
    case CTokenType_OpenBracket:
    {
      Result = "CTokenType_OpenBracket";
    } break;
    case CTokenType_CloseBracket:
    {
      Result = "CTokenType_CloseBracket";
    } break;
    case CTokenType_OpenBrace:
    {
      Result = "CTokenType_OpenBrace";
    } break;
    case CTokenType_CloseBrace:
    {
      Result = "CTokenType_CloseBrace";
    } break;
    case CTokenType_OpenParen:
    {
      Result = "CTokenType_OpenParen";
    } break;
    case CTokenType_CloseParen:
    {
      Result = "CTokenType_CloseParen";
    } break;
    case CTokenType_Dot:
    {
      Result = "CTokenType_Dot";
    } break;
    case CTokenType_Comma:
    {
      Result = "CTokenType_Comma";
    } break;
    case CTokenType_Semicolon:
    {
      Result = "CTokenType_Semicolon";
    } break;
    case CTokenType_Colon:
    {
      Result = "CTokenType_Colon";
    } break;
    case CTokenType_Hash:
    {
      Result = "CTokenType_Hash";
    } break;
    case CTokenType_Space:
    {
      Result = "CTokenType_Space";
    } break;
    case CTokenType_Star:
    {
      Result = "CTokenType_Star";
    } break;
    case CTokenType_Ampersand:
    {
      Result = "CTokenType_Ampersand";
    } break;
    case CTokenType_SingleQuote:
    {
      Result = "CTokenType_SingleQuote";
    } break;
    case CTokenType_DoubleQuote:
    {
      Result = "CTokenType_DoubleQuote";
    } break;
    case CTokenType_Equals:
    {
      Result = "CTokenType_Equals";
    } break;
    case CTokenType_LT:
    {
      Result = "CTokenType_LT";
    } break;
    case CTokenType_GT:
    {
      Result = "CTokenType_GT";
    } break;
    case CTokenType_Plus:
    {
      Result = "CTokenType_Plus";
    } break;
    case CTokenType_Minus:
    {
      Result = "CTokenType_Minus";
    } break;
    case CTokenType_Percent:
    {
      Result = "CTokenType_Percent";
    } break;
    case CTokenType_Bang:
    {
      Result = "CTokenType_Bang";
    } break;
    case CTokenType_Hat:
    {
      Result = "CTokenType_Hat";
    } break;
    case CTokenType_Question:
    {
      Result = "CTokenType_Question";
    } break;
    case CTokenType_FSlash:
    {
      Result = "CTokenType_FSlash";
    } break;
    case CTokenType_BSlash:
    {
      Result = "CTokenType_BSlash";
    } break;
    case CTokenType_Tilde:
    {
      Result = "CTokenType_Tilde";
    } break;
    case CTokenType_Backtick:
    {
      Result = "CTokenType_Backtick";
    } break;
    case CTokenType_Newline:
    {
      Result = "CTokenType_Newline";
    } break;
    case CTokenType_CarrigeReturn:
    {
      Result = "CTokenType_CarrigeReturn";
    } break;
    case CTokenType_EOF:
    {
      Result = "CTokenType_EOF";
    } break;

    InvalidDefaultCase;
  }

  return Result;
};

inline void
PrintToken(c_token Token)
{
  switch (Token.Type)
  {
    case CTokenType_Comment:
    case CTokenType_Identifier:
    {
      Log("%.*s", Token.Value.Count, Token.Value.Start);
    } break;

    case CTokenType_String:
    {
      Log("\"%.*s\"", Token.Value.Count, Token.Value.Start);
    } break;

    default:
    {
      Log("%c", Token.Type);
    }
  }
}

b32
operator==(c_token T1, c_token T2)
{
  b32 Result = (T1.Type == T2.Type);
  if (Result && T1.Value.Count > 0)
  {
    Result &= StringsMatch(T1.Value, T2.Value);
  }
  return Result;
}

b32
operator!=(c_token T1, c_token T2)
{
  b32 Result = !(T1==T2);
  return Result;
}

inline c_token
CToken(counted_string Value)
{
  c_token Result = {
    .Type = CTokenType_Identifier,
    .Value = Value
  };
  return Result;
}

inline c_token
CToken(c_token_type Type, counted_string Value = CountedString(""))
{
  c_token Result = {
    .Type = Type,
    .Value = Value
  };
  return Result;
}

c_token_buffer
AllocateTokenBuffer(memory_arena* Memory, u32 Count)
{
  c_token_buffer Result = {};
  Result.Start = Allocate(c_token, Memory, Count);
  Result.At = Result.Start;
  Result.End = Result.Start + Count;

  return Result;
}

function string_stream
StringStream()
{
  string_stream Result = {};
  InitSentinel(Result.Sentinel);
  return Result;
}

function string_iterator
SSIterator(string_stream* Stream)
{
  string_iterator Iterator = {
    .Stream = Stream,
    .At = Stream->Sentinel.Next
  };
  return Iterator;
}

function b32
IsValid(string_iterator* Iter)
{
  b32 Result = (Iter->At != &Iter->Stream->Sentinel);
  return Result;
}

function void
Advance(string_iterator* Iter)
{
  Iter->At = Iter->At->Next;
}
