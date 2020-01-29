
function counted_string
ToString(metaprogramming_directives Type)
{
  counted_string Result = {};
  switch (Type)
  {
    case generate_noop: { Result = CS("generate_noop"); } break;
    case generate_stream: { Result = CS("generate_stream"); } break;
    case generate_static_buffer: { Result = CS("generate_static_buffer"); } break;
    case generate_string_table: { Result = CS("generate_string_table"); } break;
  }
  return Result;
}


function counted_string
ToString(c_token_type Type)
{
  counted_string Result = {};
  switch (Type)
  {
    case CTokenType_Unknown: { Result = CS("CTokenType_Unknown"); } break;
    case CTokenType_Comment: { Result = CS("CTokenType_Comment"); } break;
    case CTokenType_Identifier: { Result = CS("CTokenType_Identifier"); } break;
    case CTokenType_String: { Result = CS("CTokenType_String"); } break;
    case CTokenType_Char: { Result = CS("CTokenType_Char"); } break;
    case CTokenType_MetaprogrammingDirective: { Result = CS("CTokenType_MetaprogrammingDirective"); } break;
    case CTokenType_OpenBracket: { Result = CS("CTokenType_OpenBracket"); } break;
    case CTokenType_CloseBracket: { Result = CS("CTokenType_CloseBracket"); } break;
    case CTokenType_OpenBrace: { Result = CS("CTokenType_OpenBrace"); } break;
    case CTokenType_CloseBrace: { Result = CS("CTokenType_CloseBrace"); } break;
    case CTokenType_OpenParen: { Result = CS("CTokenType_OpenParen"); } break;
    case CTokenType_CloseParen: { Result = CS("CTokenType_CloseParen"); } break;
    case CTokenType_Dot: { Result = CS("CTokenType_Dot"); } break;
    case CTokenType_Comma: { Result = CS("CTokenType_Comma"); } break;
    case CTokenType_Semicolon: { Result = CS("CTokenType_Semicolon"); } break;
    case CTokenType_Colon: { Result = CS("CTokenType_Colon"); } break;
    case CTokenType_Hash: { Result = CS("CTokenType_Hash"); } break;
    case CTokenType_At: { Result = CS("CTokenType_At"); } break;
    case CTokenType_Space: { Result = CS("CTokenType_Space"); } break;
    case CTokenType_Star: { Result = CS("CTokenType_Star"); } break;
    case CTokenType_Ampersand: { Result = CS("CTokenType_Ampersand"); } break;
    case CTokenType_SingleQuote: { Result = CS("CTokenType_SingleQuote"); } break;
    case CTokenType_DoubleQuote: { Result = CS("CTokenType_DoubleQuote"); } break;
    case CTokenType_Equals: { Result = CS("CTokenType_Equals"); } break;
    case CTokenType_LT: { Result = CS("CTokenType_LT"); } break;
    case CTokenType_GT: { Result = CS("CTokenType_GT"); } break;
    case CTokenType_Plus: { Result = CS("CTokenType_Plus"); } break;
    case CTokenType_Minus: { Result = CS("CTokenType_Minus"); } break;
    case CTokenType_Percent: { Result = CS("CTokenType_Percent"); } break;
    case CTokenType_Bang: { Result = CS("CTokenType_Bang"); } break;
    case CTokenType_Hat: { Result = CS("CTokenType_Hat"); } break;
    case CTokenType_Question: { Result = CS("CTokenType_Question"); } break;
    case CTokenType_FSlash: { Result = CS("CTokenType_FSlash"); } break;
    case CTokenType_BSlash: { Result = CS("CTokenType_BSlash"); } break;
    case CTokenType_Tilde: { Result = CS("CTokenType_Tilde"); } break;
    case CTokenType_Backtick: { Result = CS("CTokenType_Backtick"); } break;
    case CTokenType_Newline: { Result = CS("CTokenType_Newline"); } break;
    case CTokenType_CarrigeReturn: { Result = CS("CTokenType_CarrigeReturn"); } break;
    case CTokenType_EOF: { Result = CS("CTokenType_EOF"); } break;
  }
  return Result;
}

enum c_decl_type
{
  type_c_decl_noop,
  type_c_decl_variable,
  type_c_decl_function,
  type_c_decl_union,
};

struct c_decl
{
  c_decl_type Type;

  union
  {
    c_decl_variable c_decl_variable;
    c_decl_function c_decl_function;
    c_decl_union c_decl_union;
  };
};


struct enum_field_stream_chunk
{
  enum_field Element;
  enum_field_stream_chunk* Next;
};

struct enum_field_stream
{
  enum_field_stream_chunk* FirstChunk;
  enum_field_stream_chunk* LastChunk;
};


function void
Push(enum_field_stream* Stream, enum_field Element, memory_arena* Memory)
{
  // TODO(Jesse): Can we use Allocate() here instead?
  enum_field_stream_chunk* NextChunk = (enum_field_stream_chunk*)PushStruct(Memory, sizeof(enum_field_stream_chunk), 1, 1);
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


struct enum_field_iterator
{
  enum_field_stream* Stream;
  enum_field_stream_chunk* At;
};

function enum_field_iterator
Iterator(enum_field_stream* Stream)
{
  enum_field_iterator Iterator = {
    .Stream = Stream,
    .At = Stream->FirstChunk
  };
  return Iterator;
}

function b32
IsValid(enum_field_iterator* Iter)
{
  b32 Result = Iter->At != 0;
  return Result;
}

function void
Advance(enum_field_iterator* Iter)
{
  Iter->At = Iter->At->Next;
}


struct d_union_member_stream_chunk
{
  d_union_member Element;
  d_union_member_stream_chunk* Next;
};

struct d_union_member_stream
{
  d_union_member_stream_chunk* FirstChunk;
  d_union_member_stream_chunk* LastChunk;
};


function void
Push(d_union_member_stream* Stream, d_union_member Element, memory_arena* Memory)
{
  // TODO(Jesse): Can we use Allocate() here instead?
  d_union_member_stream_chunk* NextChunk = (d_union_member_stream_chunk*)PushStruct(Memory, sizeof(d_union_member_stream_chunk), 1, 1);
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


struct d_union_member_iterator
{
  d_union_member_stream* Stream;
  d_union_member_stream_chunk* At;
};

function d_union_member_iterator
Iterator(d_union_member_stream* Stream)
{
  d_union_member_iterator Iterator = {
    .Stream = Stream,
    .At = Stream->FirstChunk
  };
  return Iterator;
}

function b32
IsValid(d_union_member_iterator* Iter)
{
  b32 Result = Iter->At != 0;
  return Result;
}

function void
Advance(d_union_member_iterator* Iter)
{
  Iter->At = Iter->At->Next;
}


