#define BONSAI_NO_MUTEX_TRACKING
#define BONSAI_NO_PUSH_METADATA
#define BONSAI_NO_DEBUG_MEMORY_ALLOCATOR

#include <bonsai_types.h>
#include <unix_platform.cpp>

#define DList_Push(Stream, Container)      \
{                                          \
  Container->Prev = &Stream->Sentinel;     \
  Container->Next = Stream->Sentinel.Next; \
  Stream->Sentinel.Next->Prev = Container; \
  Stream->Sentinel.Next = Container;       \
}

enum d_union_flags
{
  d_union_flag_none,
  d_union_flag_enum_only
};

enum c_decl_function_type
{
  CFunctionType_Normal,
  CFunctionType_Constructor,
  CFunctionType_Destructor,
};

// generate_string_table
// generate_value_table
enum metaprogramming_directives
{
  generate_noop          = 0x0,
  generate_stream        = 0x1,
  generate_static_buffer = 0x2,
  generate_string_table  = 0x4,
  generate_value_table   = 0x8,
};

// TODO(Jesse): Add vertical pipe |
// generate_string_table
enum c_token_type
{
  CTokenType_Unknown,

  CTokenType_Comment,
  CTokenType_Identifier,
  CTokenType_String,
  CTokenType_Char,

  CTokenType_MetaprogrammingDirective,

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
  CTokenType_At            = '@',
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

d_union(c_decl,
{
  c_decl_variable;
  c_decl_function;
  c_decl_union;
})

// TODO(Jesse): Nested d-unions??! @mind-blown
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

struct c_decl_stream_chunk;
struct c_decl_stream
{
  c_decl_stream_chunk* FirstChunk;
  c_decl_stream_chunk* LastChunk;
};

struct struct_def
{
  counted_string Name;
  c_decl_stream Fields;
};

struct enum_def;
struct enum_cursor
{
  enum_def* Start;
  enum_def* End;
  enum_def* At;
};

struct struct_cursor
{
  struct_def* Start;
  struct_def* End;
  struct_def* At;
};

struct struct_defs
{
  u32 Count;
  struct_def* Defs;
};

struct c_decl_union
{
  struct_def Body;
};

// generate_stream
struct enum_field
{
  counted_string Name;
  counted_string Value;
};

// generate_stream
struct d_union_member
{
  counted_string Type;
  counted_string Name;
  d_union_flags Flags;
};

#include <metaprogramming/output/preprocessor.h>

struct c_decl_stream_chunk
{
  c_decl Element;
  c_decl_stream_chunk* Next;
};

struct enum_def
{
  counted_string Name;
  enum_field_stream Fields;
};

struct c_token
{
  c_token_type Type;

  // TODO(Jesse): Is this d_union-able? .. Or worth it?
  union {
    counted_string Value;
    metaprogramming_directives Directive;
  };
};

struct c_token_buffer
{
  c_token* Start;
  c_token* At;
  c_token* End;
};

struct d_union_decl
{
  counted_string Name;
  d_union_member_stream Members;
};

struct c_parse_result
{
  b32 Valid;
  c_token_buffer Tokens;

  u32 StructCount;
  u32 EnumCount;

  counted_string FileName;

  u32 LineNumber;
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
  u32 EnumCount;

  c_parse_result* Start;
  c_parse_result* End;
  c_parse_result* At;
};

struct enum_defs
{
  u32 Count;
  enum_def* Defs;
};

struct program_datatypes
{
  struct_defs Structs;
  enum_defs Enums;
};

struct c_decl_iterator
{
  c_decl_stream_chunk* At;
};

inline counted_string
ToString(c_token Token, memory_arena* Memory)
{
  counted_string Result = {};
  switch (Token.Type)
  {
    case CTokenType_Comment:
    case CTokenType_Identifier:
    {
      Result = CS(FormatString(Memory, "%.*s", Token.Value.Count, Token.Value.Start));
    } break;

    case CTokenType_String:
    {
      Result = CS(FormatString(Memory, "\"%.*s\"", Token.Value.Count, Token.Value.Start));
    } break;

    default:
    {
      Result = CS(FormatString(Memory, "%c", Token.Type));
    }
  }
  return Result;
}

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

    case CTokenType_Char:
    {
      Log("'%.*s'", Token.Value.Count, Token.Value.Start);
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
  if (Result && (T1.Value.Count > 0 || T2.Value.Count) )
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
