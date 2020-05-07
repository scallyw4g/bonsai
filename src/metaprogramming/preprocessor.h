meta(
  func dunion_debug_print(DUnionType)
  {
    function void
    DebugPrint( (DUnionType.type) UnionStruct, u32 Depth = 0)
    {
      switch(UnionStruct.Type)
      {
        (
          DUnionType.map_members (M)
          {
            (
              M.is_anonymous?
              {
                (
                  M.map_values (UnionMember)
                  {
                    case type_(UnionMember.type):
                    {
                      DebugPrint(UnionStruct.(UnionMember.type), Depth+4);
                    }
                  }
                )
              }
            )
          }
        )

        InvalidDefaultCase;
      }
    }
  }
)

meta(
  func generate_cursor(Type)
  {
    struct (Type.name)_cursor
    {
      (Type.name)* Start;
      (Type.name)* End;
      (Type.name)* At;
    };

    function (Type.name)_cursor
    (Type.name.to_capital_case)Cursor(umm ElementCount, memory_arena* Memory)
    {
      (Type.name)* Start = ((Type.name)*)PushStruct(Memory, sizeof( (Type.name) ), 1, 0);
      (Type.name)_cursor Result = {
        .Start = Start,
        .End = Start+ElementCount,
        .At = Start,
      };
      return Result;
    }
  }
)

meta(
  func generate_string_table(EnumType)
  {
    function counted_string
    ToString( (EnumType.name) Type)
    {
      counted_string Result = {};
      switch (Type)
      {
        (
          EnumType.map_values (EnumValue)
          {
            case (EnumValue.name): { Result = CSz("(EnumValue.name)"); } break;
          }
        )
      }
      return Result;
    }
  }
)

meta(
  func generate_value_table(EnumType)
  {
    function (EnumType.name)
    (EnumType.name.to_capital_case)(counted_string S)
    {
      (EnumType.name) Result = {};

      (
        EnumType.map_values(EnumValue)
        {
          if (StringsMatch(S, CSz("(EnumValue.name)"))) { return (EnumValue.name); }
        }
      )

      return Result;
    }
  }
)

meta(
  func generate_stream_chunk_struct(Type)
  {
    struct (Type.name)_stream_chunk
    {
      (Type.name) Element;
      (Type.name)_stream_chunk* Next;
    };
  }
)

meta(
  func generate_stream_push(Type)
  {
    function void
    Push((Type.name)_stream* Stream, (Type.name) Element, memory_arena* Memory)
    {
      (Type.name)_stream_chunk* NextChunk = ((Type.name)_stream_chunk*)PushStruct(Memory, sizeof( (Type.name)_stream_chunk ), 1, 0);
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
  }
)

meta(
  func generate_stream_struct(Type)
  {
    struct (Type.name)_stream
    {
      (Type.name)_stream_chunk* FirstChunk;
      (Type.name)_stream_chunk* LastChunk;
    };

  }
)

meta(
  func generate_stream_iterator(Type)
  {
    struct (Type.name)_iterator
    {
      (Type.name)_stream* Stream;
      (Type.name)_stream_chunk* At;
    };

    function (Type.name)_iterator
    Iterator((Type.name)_stream* Stream)
    {
      (Type.name)_iterator Iterator = {
        .Stream = Stream,
        .At = Stream->FirstChunk
      };
      return Iterator;
    }

    function b32
    IsValid((Type.name)_iterator* Iter)
    {
      b32 Result = Iter->At != 0;
      return Result;
    }

    function void
    Advance((Type.name)_iterator* Iter)
    {
      Iter->At = Iter->At->Next;
    }

  }
)

meta(
  func generate_stream_getters(InputTypeDef)
  {
    InputTypeDef.map_members (Member) {
      function InputTypeDef.type
      GetBy(Member.name)( (Member.Type) Needle, (InputTypeDef.type)_stream *Haystack)
      {
        // TODO : Implement matching!
      }
    }
  }
)

meta(
  func generate_stream(Type)
  {
    (generate_stream_chunk_struct(Type))
    (generate_stream_struct(Type))
    (generate_stream_iterator(Type))
    (generate_stream_push(Type))
  }
)

meta(
  func string_and_value_tables(Def)
  {
    ( generate_string_table(Def) )
    ( generate_value_table(Def) )
  }
)

meta(
  func stream_and_cursor(Def)
  {
    ( generate_cursor(Def) )
    ( generate_stream(Def) )
  }
)

meta(
  polymorphic_func void DebugPrint( @TypeDef RuntimeValue, u32 Depth)
  {
    DebugPrint("(TypeDef.name): ", Depth);
    TypeDef.is_enum?
    {
      TypeDef.map_values (ValueDef)
      {
        DebugPrint("(ValueDef.name) (ValueDef.value)", Depth+1);
        DebugPrint(ToString(RuntimeValue), Depth+1);
      }
    }

    TypeDef.is_struct?
    {
      TypeDef.map_members (MemberDef)
      {
        DebugPrint("(MemberDef.type) (MemberDef.name): ", Depth);
        DebugPrint(RuntimeValue.(MemberDef.name), Depth+1);
        DebugPrint("\n");
      }
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
  enum_only,
  d_union,
  named_list,
  for_datatypes,

  func,
  polymorphic_func,
};
meta( string_and_value_tables(metaprogramming_directive) )
#include <metaprogramming/output/string_and_value_tables_metaprogramming_directive.h>

enum meta_arg_operator
{
  meta_arg_operator_noop,

  name,
  type,
  value,
  map_values,
  map_members,
  is_enum,
  is_struct,
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
  CTokenType_Unknown = 0,

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
  CTokenType_Ampersand     = '&', // TODO(Jesse id: 238, tags: immediate, cleanup): Change name to BitwiseAnd
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
  CTokenType_Pipe          = '|', // TODO(Jesse id: 239, tags: immediate, cleanup): Change name to BitwiseOr
  CTokenType_Newline       = '\n',
  CTokenType_CarrigeReturn = '\r',
  CTokenType_EOF           = EOF,

  CTokenType_CommentSingleLine = 256, // Presumably, we'll never need to parse anything that's not ascii, so start the non-ascii tokens at 256
  CTokenType_CommentMultiLineStart,
  CTokenType_CommentMultiLineEnd,

  CTokenType_EscapedNewline,

  CTokenType_Identifier,
  CTokenType_StringLiteral,
  CTokenType_CharLiteral,
  CTokenType_IntLiteral,
  CTokenType_DoubleLiteral,
  CTokenType_FloatLiteral,
  CTokenType_LongDoubleLiteral,

  CTokenType_Meta,

  CTokenType_Struct,
  CTokenType_Enum,
  CTokenType_Union,
  CTokenType_Typedef,

  CTokenType_Using,

  CTokenType_ThreadLocal,
  CTokenType_Const,
  CTokenType_Static,
  CTokenType_Volatile,
  CTokenType_Long,
  CTokenType_Unsigned,
  CTokenType_Signed,

  CTokenType_Bool,
  CTokenType_M128,
  CTokenType_Auto,
  CTokenType_Void,
  CTokenType_Double,
  CTokenType_Float,
  CTokenType_Char,
  CTokenType_Int,

  CTokenType_Asm,
  CTokenType_Goto,
  CTokenType_Ellipsis,

  CTokenType_If,
  CTokenType_Else,
  CTokenType_Break,
  CTokenType_Switch,
  CTokenType_Case,
  CTokenType_Default,
  CTokenType_For,
  CTokenType_While,
  CTokenType_Continue,
  CTokenType_Return,

  CTokenType_LeftShift,
  CTokenType_RightShift,

  CTokenType_LessEqual,
  CTokenType_GreaterEqual,
  CTokenType_AreEqual,
  CTokenType_NotEqual,

  CTokenType_PlusEquals,
  CTokenType_MinusEquals,
  CTokenType_TimesEquals,
  CTokenType_DivEquals,
  CTokenType_ModEquals,
  CTokenType_AndEquals,
  CTokenType_OrEquals,
  CTokenType_XorEquals,

  CTokenType_Increment,
  CTokenType_Decrement,

  CTokenType_LogicalAnd,
  CTokenType_LogicalOr,

  CTokenType_Arrow,
};
meta(generate_string_table(c_token_type))
#include <metaprogramming/output/generate_string_table_c_token_type.h>

meta(
  d_union struct_member_function
  {
    struct_member_function_normal      enum_only
    struct_member_function_operator    enum_only
    struct_member_function_constructor enum_only
    struct_member_function_destructor  enum_only
  }
)
#include <metaprogramming/output/d_union_c_decl_function.h>

struct struct_member_stream_chunk;

/* TODO(Jesse id: 191, tags: metaprogramming): This can be generated, but it
 * requires lazily evaluating functions once their dependant types have been
 * generated.  Not sure if this is a good idea or not, but we could do it.
 *
 * meta( generate_stream_struct(struct_member) )
 *
 * Currently the invariant of being able to remove and re-generate all meta
 * output in a single run of the metaprogramming system is broken by this.
 */

struct struct_member_stream
{
  struct_member_stream_chunk *FirstChunk;
  struct_member_stream_chunk *LastChunk;
};

struct struct_def
{
  counted_string Type;
  counted_string DefinedInFile;
  struct_member_stream Members;

  b32 IsUnion;
};
meta(stream_and_cursor(struct_def))
#include <metaprogramming/output/stream_and_cursor_struct_def.h>

struct struct_member_anonymous
{
  struct_def Body;
};

struct type_spec
{
  counted_string Name;

  u32 IndirectionLevel;
  u32 ReferenceLevel;

  // TODO(Jesse id: 194, tags: metaprogramming, parsing): Use bitflags
  b32 Unsigned;
  b32 Volatile;
  b32 Const;
  b32 ThreadLocal;

  b32 IsMetaTemplateVar;

  b32 IsFunction;
  b32 IsOperator;
  b32 IsConstructor;
  b32 Inline;

  b32 IsStatic; // TODO(Jesse id: 252): These are mutually exclusive .. merge them?
  b32 IsExported;

  b32 IsTemplateFunction;        // Ick..
  counted_string TemplateSource; // Ick..
  counted_string Namespace;      // Ick..

  counted_string SourceText;
};

struct type_spec_2
{
  counted_string Namespace;
  counted_string Name;

  counted_string SourceText;

  u32 ReferenceLevel;
  u32 IndirectionLevel;

  b32 IsMetaTemplateVar;

  b32 ThreadLocal;
  b32 Const;
  b32 Static;
  b32 Volatile;
  b32 Unsigned;
  b32 Signed;
  b32 Long;
  b32 Struct;
  b32 Enum;

  b32 IsFunctionPointer;
  counted_string FunctionPointerTypeName;
};

struct variable // TODO(Jesse id: 245): Change to variable_def or variable_decl
{
  type_spec Type;
  counted_string Name;

  b32 IsVariadic;

  counted_string Namespace; // Ick..

  counted_string FnPointerDef;
  counted_string StaticBufferSize;
  counted_string SourceText;
};
meta(generate_stream(variable))
#include <metaprogramming/output/generate_stream_variable.h>

meta(
  d_union struct_member
  {
    variable
    struct_member_function
    struct_member_anonymous
  }
)
#include <metaprogramming/output/d_union_c_decl.h>
meta(generate_cursor(struct_member))
#include <metaprogramming/output/generate_cursor_c_decl.h>

meta( generate_stream_chunk_struct(struct_member) )
#include <metaprogramming/output/generate_stream_chunk_struct_c_decl.h>

struct ast_node_expression;
struct enum_member
{
  counted_string Name;
  ast_node_expression *Value;
};
meta(generate_stream(enum_member))
#include <metaprogramming/output/generate_stream_enum_field.h>

struct d_union_member
{
  counted_string Type;
  counted_string Name;
  d_union_flags Flags;
};
meta(generate_stream(d_union_member))
#include <metaprogramming/output/generate_stream_d_union_member.h>

struct enum_def
{
  counted_string Name;
  enum_member_stream Members;
};
meta(stream_and_cursor(enum_def))
#include <metaprogramming/output/stream_and_cursor_enum_def.h>

struct type_def
{
  type_spec_2 Type;
  counted_string Alias;
};
meta(generate_stream(type_def))
#include <metaprogramming/output/generate_stream_type_def.h>


enum datatype_type
{
  type_datatype_noop,

  type_struct_def,
  type_struct_member,

  type_enum_def,
  type_enum_member,

  type_type_def,
};

/* TODO(Jesse, id: 188, tags: cleanup) This should have the name property,
 * instead of having the struct and enum defs have seperate names
 */
struct datatype
{
  datatype_type Type;

  union
  {
    struct_def     *struct_def;
    struct_member  *struct_member;

    enum_def       *enum_def;
    enum_member    *enum_member;

    type_def       *type_def;
  };
};


function datatype
Datatype(struct_member* M)
{
  datatype Result = {
    .Type = type_struct_member,
    .struct_member = M,
  };
  return Result;
}

function datatype
Datatype(enum_member* E)
{
  datatype Result = {
    .Type = type_enum_member,
    .enum_member = E,
  };
  return Result;
}


function datatype
Datatype(struct_def* S)
{
  datatype Result = {
    .Type = type_struct_def,
    .struct_def = S,
  };
  return Result;
}

function datatype
Datatype(type_def* E)
{
  datatype Result = {
    .Type = type_type_def,
    .type_def = E,
  };
  return Result;
}

function datatype
Datatype(enum_def* E)
{
  datatype Result = {
    .Type = type_enum_def,
    .enum_def = E,
  };
  return Result;
}

struct meta_func_arg
{
  counted_string Match;
  datatype Data;
};
meta(generate_stream(meta_func_arg))
#include <metaprogramming/output/generate_stream_meta_func_arg.h>






struct c_token
{
  c_token_type Type;

  union
  {
    counted_string Value;
    r32 FloatValue;
    r64 DoubleValue;
  };
};
meta(generate_cursor(c_token))
#include <metaprogramming/output/generate_cursor_c_token.h>

struct d_union_decl
{
  counted_string Name;

  d_union_member_stream Members;
  struct_member_stream CommonMembers;

  counted_string CustomEnumType;
};

struct parser
{
  b32 Valid;
  c_token_cursor Tokens;

  /* TODO(Jesse id: 154) This is pretty shitty because whenever we copy one of
   * these structs this field has to be manually zeroed out ..
   */
  c_token_cursor OutputTokens;

  counted_string Filename;
  u32 LineNumber;
};
meta(generate_cursor(parser))
#include <metaprogramming/output/generate_cursor_parser.h>

enum macro_type
{
  type_macro_noop,

  type_macro_keyword,
  type_macro_function,
};

struct macro_def
{
  macro_type Type;
  counted_string Name;
  parser Parser;
};
meta(generate_stream(macro_def))
#include <metaprogramming/output/generate_stream_macro_def.h>





struct meta_func
{
  counted_string Name;
  counted_string ArgName;
  parser Body;
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

#define SafeCast(T, Ptr) (&(Ptr)->T); Assert((Ptr)->Type == type_##T)

struct ast_node;
struct function_def;

struct ast_node_expression
{
  ast_node *Value;
  ast_node_expression *Next;
};
meta(generate_stream(ast_node_expression))
#include <metaprogramming/output/generate_stream_ast_node_expression.h>

struct statement_list
{
  ast_node_expression *LHS;
  statement_list *RHS;
  statement_list *Next;
};

struct ast_node_function_call
{
  counted_string Name;
  function_def *Prototype;
  ast_node_expression_stream Args;
};

struct ast_node_type_specifier
{
  datatype Datatype;
  type_spec_2 TypeSpec;
  ast_node_expression *Name;
};

struct ast_node_variable_def
{
  datatype Type;
  variable Decl;
  ast_node *Value;
};
meta(generate_stream(ast_node_variable_def))
#include <metaprogramming/output/generate_stream_ast_node_variable_def.h>

struct ast_node_access
{
  c_token AccessType; // Arrow or Dot
  ast_node_expression *Symbol;
};

struct ast_node_scope
{
  statement_list *FirstStatement;
};

struct ast_node_parenthesized
{
  ast_node_expression *Expr;

  // TODO(Jesse id: 262): These should probably be in their own type_ast_node_cast .. ?
  b32 IsCast;
  ast_node_expression *CastValue;
};

struct ast_node_initializer_list
{
  u32 Ignored;
};

struct ast_node_operator
{
  ast_node_expression *Operand;
  c_token Token;
};

struct ast_node_literal
{
  c_token Token;
};

struct ast_node_symbol
{
  c_token Token;
};

struct ast_node_ignored
{
  c_token Token;
};

struct ast_node_return
{
  ast_node_expression *Value;
};

struct ast_node_predicated
{
  c_token Token;
  ast_node_expression *Predicate;
};

meta(
  d_union ast_node
  {
    ast_node_access
    ast_node_literal
    ast_node_expression
    ast_node_parenthesized
    ast_node_operator
    ast_node_function_call
    ast_node_return
    ast_node_scope
    ast_node_initializer_list
    ast_node_ignored
    ast_node_symbol
    ast_node_variable_def
    ast_node_type_specifier
  }
)
#include <metaprogramming/output/d_union_ast_node.h>

meta(generate_stream(ast_node))
#include <metaprogramming/output/generate_stream_ast_node.h>

function ast_node*
AllocateAstNode(ast_node_type T, ast_node **Result, memory_arena* Memory)
{
  Assert(Result && !(*Result)); // We got a valid pointer, and it hasn't been allocated yet.

  *Result = AllocateProtection(ast_node, Memory, 1, False);
  (*Result)->Type = T;

  return *Result;
}

#define AllocateAndCastTo(T, NodeDest, Memory) (&AllocateAstNode(type_##T, (NodeDest), (Memory))->T)

struct arguments
{
  counted_string Outpath;
  counted_string_cursor Files;

  b32 DoDebugWindow;
};

struct scope
{
  variable_stream Decls;
  scope* Parent;
};

struct function_def
{
  variable Prototype;
  variable_stream Args;
  ast_node_variable_def_stream Locals;

  parser Body;
  statement_list* Ast;
};
meta(generate_stream(function_def))
#include <metaprogramming/output/generate_stream_function_def.h>

struct program_datatypes
{
  struct_def_stream   Structs;
  enum_def_stream     Enums;
  function_def_stream Functions;
  type_def_stream     Typedefs;
  macro_def_stream    Macros;
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

struct metaprogramming_info
{
  meta_func_stream FunctionDefs;
  program_datatypes Datatypes;
};

struct todo_list_info
{
  person_stream People;
  tagged_counted_string_stream_stream NameLists;
};



struct string_from_parser
{
  const char* Start;
};

enum output_mode
{
  Output_NoOverwrite,
  Output_Unsafe,
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
CToken(c_token_type Type, counted_string Value = CSz(""))
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
PeekToken(ansi_stream* Stream, u32 Lookahead = 0)
{
  c_token Result = {};

  if (Stream->At+Lookahead < Stream->End)
  {
    char At = *(Stream->At+Lookahead);
    // TODO(Jesse, id: 193, tags: metaprogramming): Metaprogram this.  I've had bugs multiple times because of it.
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
      case CTokenType_Pipe:
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

meta(generate_stream_iterator(struct_member))
#include <metaprogramming/output/generate_stream_iterator_c_decl.h>

meta(generate_stream_push(struct_member))
#include <metaprogramming/output/generate_stream_push_c_decl.h>

function string_from_parser
StartStringFromParser(parser* Parser)
{
  string_from_parser Result = {
    .Start = Parser->Tokens.At->Value.Start
  };
  return Result;
}

function counted_string
FinalizeStringFromParser(string_from_parser* Builder, parser* Parser)
{
  umm Count = (umm)(Parser->Tokens.At->Value.Start - Builder->Start);
  counted_string Result = { .Start = Builder->Start, .Count = Count };
  return Result;
}

function c_token_type
CloseTokenFor(c_token_type T)
{
  c_token_type Result = {};

  switch (T)
  {
    case CTokenType_OpenParen:
      Result = CTokenType_CloseParen; break;

    case CTokenType_OpenBrace:
      Result = CTokenType_CloseBrace; break;

    case CTokenType_OpenBracket:
      Result = CTokenType_CloseBracket; break;

    InvalidDefaultCase;
  }

  return Result;
}

#define MAX_PARSER_STACK_DEPTH (32)
struct parser_stack
{
  u32 Depth;
  parser Parsers[MAX_PARSER_STACK_DEPTH];
};

function parser*
Peek(parser_stack *Stack, u32 StackLookahead = 0)
{
  parser *Result = {};
  if (Stack->Depth > StackLookahead)
  {
    Result = Stack->Parsers + (Stack->Depth-StackLookahead-1);
  }
  else
  {
    Error("Tried peeking an empty parser stack!");
  }
  return Result;
}

function parser*
PushStack(parser_stack *Stack, parser Parser)
{
  if (Stack->Depth == MAX_PARSER_STACK_DEPTH)
  {
    Error("Max parser stack depth exceeded.");
  }
  else
  {
    Stack->Parsers[Stack->Depth] = Parser;
    ++Stack->Depth;
  }

  parser *Result = Peek(Stack);
  return Result;
}

function parser*
PopStack(parser_stack *Stack)
{
  parser *Result = {};
  if (Stack->Depth > 0)
  {
    --Stack->Depth;
    Result = Stack->Parsers + (Stack->Depth-1);
  }
  else
  {
    Error("Tried popping an empty parser stack!");
  }
  return Result;
}


