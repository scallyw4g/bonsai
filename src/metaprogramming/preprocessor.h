meta(
  def_func generate_cursor(arg_type_struct $StructType)
  {
    struct $StructType.name$_cursor
    {
      $StructType.name* Start;
      $StructType.name* End;
      $StructType.name* At;
    };

    function $StructType.name$_cursor
    $StructType.name.to_capital_case$Cursor(umm ElementCount, memory_arena* Memory)
    {
      $StructType.name$* Start = ($StructType.name$*)PushStruct(Memory, sizeof($StructType.name), 1, 1);
      $StructType.name$_cursor Result = {
        .Start = Start,
        .End = Start+ElementCount,
        .At = Start,
      };
      return Result;
    };
  }
)

// TODO(Jesse id: 186, tags: metaprogramming, ast_needed, cleanup): This should be able to use the string 'enum' instead of 'arg_type_enum'
meta(
  def_func generate_string_table(arg_type_enum $EnumType)
  {
    function counted_string
    ToString($EnumType.name Type)
    {
      counted_string Result = {};
      switch (Type)
      {
        $EnumType.map_values ($EnumValue)
        {
          case $EnumValue.name: { Result = CSz("$EnumValue.name"); } break;
        }
      }
      return Result;
    }
  }
)

meta(
  def_func generate_value_table(arg_type_enum $EnumType)
  {
    function $EnumType.name
    $EnumType.name.to_capital_case(counted_string S)
    {
      $EnumType.name Result = {};

      $EnumType.map_values ($EnumValue)
      {
        if (StringsMatch(S, CSz("$EnumValue.name"))) { return $EnumValue.name; }
      }

      return Result;
    }
  }
)

meta(
  def_func generate_stream(arg_type_struct $StructType)
  {
    struct $StructType.name$_stream_chunk
    {
      $StructType.name Element;
      $StructType.name$_stream_chunk* Next;
    };

    struct $StructType.name$_stream
    {
      $StructType.name$_stream_chunk* FirstChunk;
      $StructType.name$_stream_chunk* LastChunk;
    };

    function void
    Push($StructType.name$_stream* Stream, $StructType.name$ Element, memory_arena* Memory)
    {
      $StructType.name$_stream_chunk* NextChunk = ($StructType.name$_stream_chunk*)PushStruct(Memory, sizeof($StructType.name$_stream_chunk), 1, 1);
      NextChunk->Element = Element;

      if (!Stream->FirstChunk)
      {
        Assert(!Stream->LastChunk);
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

    struct $StructType.name$_iterator
    {
      $StructType.name$_stream* Stream;
      $StructType.name$_stream_chunk* At;
    };

    function $StructType.name$_iterator
    Iterator($StructType.name$_stream* Stream)
    {
      $StructType.name$_iterator Iterator = {
        .Stream = Stream,
        .At = Stream->FirstChunk
      };
      return Iterator;
    }

    function b32
    IsValid($StructType.name$_iterator* Iter)
    {
      b32 Result = Iter->At != 0;
      return Result;
    }

    function void
    Advance($StructType.name$_iterator* Iter)
    {
      Iter->At = Iter->At->Next;
    }

  }
)

enum d_union_flags
{
  d_union_flag_none,
  d_union_flag_enum_only
};

enum metaprogramming_directive
{
  meta_directive_noop,

  generate_cursor_deprecated,

  for_members_in,
  d_union,

  enum_only,
  member_is_or_contains_type,

  for_all_datatypes,
  named_list,
  def_func,
  def_func_2,
};
meta(generate_string_table(metaprogramming_directive))
#include <metaprogramming/output/generate_string_table_metaprogramming_directive.h>

meta(generate_value_table(metaprogramming_directive))
#include <metaprogramming/output/generate_value_table_metaprogramming_directive.h>


enum meta_arg_operator
{
  meta_arg_operator_noop,

  name,
  map_values,
};
meta( generate_value_table(meta_arg_operator) )
#include <metaprogramming/output/generate_value_table_meta_arg_operator.h>

enum meta_transform_op
{
  meta_transform_op_noop,

  to_capital_case = (1 << 0),
};
meta(generate_value_table(meta_transform_op))
#include <metaprogramming/output/generate_value_table_meta_transform_op.h>


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
  CTokenType_Dollar        = '$',
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
#include <metaprogramming/output/generate_string_table_c_token_type.h>

meta(
  d_union(c_decl_function,
  {
    c_decl_function_normal      enum_only,
    c_decl_function_operator    enum_only,
    c_decl_function_constructor enum_only,
    c_decl_function_destructor  enum_only,
  })
)
#include <metaprogramming/output/d_union_c_decl_function.h>

struct c_decl_variable
{
  counted_string Type;
  counted_string Name;
};

// TODO(Jesse, id: 42, tags: compiler_feature, metaprogramming): Do we care about generating these?
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
  counted_string DefinedInFile;
  c_decl_stream Fields; // TODO(Jesse id: 159, tags: immediate, cleanup) Rename to Members
};
meta(generate_stream(struct_def))
#include <metaprogramming/output/generate_stream_struct_def.h>

meta(generate_cursor(struct_def))
#include <metaprogramming/output/generate_cursor_struct_def.h>

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
#include <metaprogramming/output/d_union_c_decl.h>

meta(generate_cursor(c_decl))
#include <metaprogramming/output/generate_cursor_c_decl.h>

struct enum_field
{
  counted_string Name;
  counted_string Value;
};
meta(generate_stream(enum_field))
#include <metaprogramming/output/generate_stream_enum_field.h>

struct d_union_member
{
  counted_string Type;
  counted_string Name;
  d_union_flags Flags;
};
meta(generate_stream(d_union_member))
#include <metaprogramming/output/generate_stream_d_union_member.h>

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
#include <metaprogramming/output/generate_stream_enum_def.h>
meta(generate_cursor(enum_def))
#include <metaprogramming/output/generate_cursor_enum_def.h>




enum datatype_type
{
  type_datatype_noop,

  type_struct_def,
  type_enum_def,
};

// TODO(Jesse, id: 188, tags: cleanup) This should have the name property, instead of
// having the struct and enum defs have seperate names
struct datatype
{
  datatype_type Type;

  union
  {
    struct_def* struct_def;
    enum_def* enum_def;
  };
};






struct c_token
{
  c_token_type Type;
  counted_string Value;
};
meta(generate_cursor(c_token))
#include <metaprogramming/output/generate_cursor_c_token.h>

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

  // TODO(Jesse id: 154) This is pretty shitty because whenever we copy one of these
  // structs this field has to be manually zeroed out ..
  c_token_cursor OutputTokens;

  counted_string Filename;
  u32 LineNumber;
};
meta(generate_cursor(c_parse_result))
#include <metaprogramming/output/generate_cursor_c_parse_result.h>





enum meta_func_arg_type
{
  arg_type_noop,

  arg_type_enum,
  arg_type_struct,
};
meta(generate_string_table(meta_func_arg_type))
#include <metaprogramming/output/generate_string_table_meta_func_arg_type.h>
meta(generate_value_table(meta_func_arg_type))
#include <metaprogramming/output/generate_value_table_meta_func_arg_type.h>

struct meta_func
{
  metaprogramming_directive Type;

  counted_string Name;
  meta_func_arg_type ArgType;
  counted_string ArgName;
  c_parse_result Body;
};
meta(generate_stream(meta_func))
#include <metaprogramming/output/generate_stream_meta_func.h>


struct todo
{
  counted_string Id;
  counted_string Value;
  b32 FoundInCodebase;
};
meta(generate_stream(todo))
#include <metaprogramming/output/generate_stream_todo.h>

struct tag
{
  counted_string Tag;
  todo_stream Todos;
};
meta(generate_stream(tag))
#include <metaprogramming/output/generate_stream_tag.h>

struct person
{
  counted_string Name;
  tag_stream Tags;
};
meta(generate_stream(person))
#include <metaprogramming/output/generate_stream_person.h>



struct arguments
{
  counted_string Outpath;
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

struct for_enum_constraints
{
  // Replacement Patterns
  counted_string TypeName;
  counted_string ValueName;
};

struct body_text_constraints
{
  counted_string MemberContains;

  // Replacement Patterns
  counted_string TypeTag;
  counted_string TypeName;
  counted_string ValueName;
};

struct replacement_pattern
{
  counted_string Match;
  counted_string Replace;
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
      case CTokenType_Dollar:
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

