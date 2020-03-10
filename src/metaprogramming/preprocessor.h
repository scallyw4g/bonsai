#include <bonsai_types.h>

enum d_union_flags
{
  d_union_flag_none,
  d_union_flag_enum_only
};

enum metaprogramming_directive
{
  noop                       = 0x0000,

  generate_stream            = 0x0001,
  generate_cursor            = 0x0002,
  generate_string_table      = 0x0004,
  generate_value_table       = 0x0008,

  for_enum_values            = 0x0010,
  for_members_in             = 0x0020,
  d_union                    = 0x0040,

  enum_only                  = 0x0080,
  member_is_or_contains_type = 0x0100,
};
meta(generate_string_table(metaprogramming_directive))
#include <metaprogramming/output/generate_string_table_metaprogramming_directive>

meta(generate_value_table(metaprogramming_directive))
#include <metaprogramming/output/generate_value_table_metaprogramming_directive>

enum c_token_type
{
  CTokenType_Unknown,

  CTokenType_CommentSingleLine,
  CTokenType_CommentMultiLineStart,
  CTokenType_CommentMultiLineEnd,

  CTokenType_Identifier,
  CTokenType_String,
  CTokenType_Char,

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
  CTokenType_Pipe          = '|',
  CTokenType_Newline       = '\n',
  CTokenType_CarrigeReturn = '\r',
  CTokenType_EOF           = EOF,
};
meta(generate_string_table(c_token_type))
#include <metaprogramming/output/generate_string_table_c_token_type>

meta(
  d_union(c_decl_function,
  {
    c_decl_function_normal      enum_only,
    c_decl_function_constructor enum_only,
    c_decl_function_destructor  enum_only,
  })
)
#include <metaprogramming/output/d_union_c_decl_function>

struct c_decl_variable
{
  counted_string Type;
  counted_string Name;
};

// TODO(Jesse, compiler_feature, metaprogramming): Do we care about generating these?
// We'd need to add a compiler feature to support outputting the datatypes to
// different files .. either that or tell it to not generate the datatypes,
// just the functions and only write these by hand.
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
meta(generate_stream(struct_def))
#include <metaprogramming/output/generate_stream_struct_def>

meta(generate_cursor(struct_def))
#include <metaprogramming/output/generate_cursor_struct_def>

struct c_decl_union
{
  struct_def Body;
};

meta(
  d_union(c_decl,
  {
    c_decl_variable,
    c_decl_function,
    c_decl_union,
  })
)
#include <metaprogramming/output/d_union_c_decl>

meta(generate_cursor(c_decl))
#include <metaprogramming/output/generate_cursor_c_decl>

struct enum_field
{
  counted_string Name;
  counted_string Value;
};
meta(generate_stream(enum_field))
#include <metaprogramming/output/generate_stream_enum_field>

struct d_union_member
{
  counted_string Type;
  counted_string Name;
  d_union_flags Flags;
};
meta(generate_stream(d_union_member))
#include <metaprogramming/output/generate_stream_d_union_member>

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
meta(generate_stream(enum_def))
#include <metaprogramming/output/generate_stream_enum_def>
meta(generate_cursor(enum_def))
#include <metaprogramming/output/generate_cursor_enum_def>

struct c_token
{
  c_token_type Type;
  counted_string Value;
};
meta(generate_cursor(c_token))
#include <metaprogramming/output/generate_cursor_c_token>

struct d_union_decl
{
  counted_string Name;

  d_union_member_stream Members;
  c_decl_stream CommonMembers;

  counted_string CustomEnumType;
};

struct c_parse_result
{
  b32 Valid;
  c_token_cursor Tokens;

  // NOTE(Jesse): This is pretty shitty because whenever we copy one of these
  // structs this field has to be manually zeroed out ..
  c_token_cursor OutputTokens;

  counted_string Filename;
  u32 LineNumber;
};
meta(generate_cursor(c_parse_result))
#include <metaprogramming/output/generate_cursor_c_parse_result>


struct todo
{
  counted_string Id;
  counted_string Value;
};
meta(generate_stream(todo))
#include <metaprogramming/output/generate_stream_todo>

struct tag
{
  counted_string Tag;
  counted_string_stream Todos;
};
meta(generate_stream(tag))
#include <metaprogramming/output/generate_stream_tagged_todo>

struct person
{
  counted_string Name;
  tag_stream Tags;
};
meta(generate_stream(person))
#include <metaprogramming/output/generate_stream_person>



struct arguments
{
  counted_string OutPath;
  counted_string_cursor Files;

  b32 DoDebugWindow;
};

struct program_datatypes
{
  struct_def_stream Structs;
  enum_def_stream Enums;
};

struct c_decl_iterator
{
  c_decl_stream_chunk* At;
};

inline void
PrintToken(c_token Token)
{
  Log("%.*s", Token.Value.Count, Token.Value.Start);
}

b32
operator==(c_token T1, c_token T2)
{
  b32 Result = (T1.Type == T2.Type);
  if (Result && (T1.Value.Count > 1 || T2.Value.Count > 1) )
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
CToken(c_token_type Type, counted_string Value = CS(""))
{
  c_token Result = {
    .Type = Type,
    .Value = Value
  };

  return Result;
}

c_token_cursor
AllocateTokenBuffer(memory_arena* Memory, u32 Count)
{
  c_token_cursor Result = {};
  Result.Start = Allocate(c_token, Memory, Count);
  Result.At = Result.Start;
  Result.End = Result.Start + Count;

  return Result;
}

c_token
GetToken(ansi_stream* Stream, u32 Lookahead = 0)
{
  c_token Result = {};

  if (Stream->At+Lookahead < Stream->End)
  {
    char At = *(Stream->At+Lookahead);
    switch (At)
    {
      case CTokenType_OpenBracket:
      case CTokenType_CloseBracket:
      case CTokenType_OpenBrace:
      case CTokenType_CloseBrace:
      case CTokenType_OpenParen:
      case CTokenType_CloseParen:
      case CTokenType_Dot:
      case CTokenType_Comma:
      case CTokenType_Semicolon:
      case CTokenType_Colon:
      case CTokenType_Hash:
      case CTokenType_At:
      case CTokenType_Space:
      case CTokenType_Star:
      case CTokenType_Ampersand:
      case CTokenType_SingleQuote:
      case CTokenType_DoubleQuote:
      case CTokenType_Equals:
      case CTokenType_LT:
      case CTokenType_GT:
      case CTokenType_Plus:
      case CTokenType_Minus:
      case CTokenType_Percent:
      case CTokenType_Bang:
      case CTokenType_Hat:
      case CTokenType_Question:
      case CTokenType_FSlash:
      case CTokenType_BSlash:
      case CTokenType_Tilde:
      case CTokenType_Backtick:
      case CTokenType_Newline:
      case CTokenType_CarrigeReturn:
      case CTokenType_EOF:
      {
        Result = { .Type = (c_token_type)At };
      } break;
    }
  }
  else
  {
    Warn("Attempted to get token past end of stream on file : %.*s", (u32)Stream->Filename.Count, Stream->Filename.Start);
  }

  return Result;
}

function c_decl_iterator
Iterator(c_decl_stream* Stream)
{
  c_decl_iterator Iterator = {
    .At = Stream->FirstChunk
  };
  return Iterator;
}

function b32
IsValid(c_decl_iterator* Iter)
{
  b32 Result = (Iter->At != 0);
  return Result;
}

function void
Advance(c_decl_iterator* Iter)
{
  Iter->At = Iter->At->Next;
}

void
Push(c_decl_stream* Stream, c_decl Element, memory_arena* Memory)
{
  c_decl_stream_chunk* NextChunk = Allocate(c_decl_stream_chunk, Memory, 1);
  NextChunk->Element = Element;

  if (!Stream->FirstChunk)
  {
    Stream->FirstChunk = NextChunk;
    Stream->LastChunk = NextChunk;
  }
  else
  {
    Stream->LastChunk->Next = NextChunk;
    Stream->LastChunk = NextChunk;
  }

  Assert(NextChunk->Next == 0);
  Assert(Stream->LastChunk->Next == 0);

  return;
}

