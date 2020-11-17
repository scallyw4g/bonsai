bonsai_function void
DebugPrint( ast_node *S, u32 Depth)
{
  if (S) { DebugPrint(*S, Depth); }
  else { DebugPrint("ptr(0)", Depth); }
}

bonsai_function void
DebugPrint( ast_node UnionStruct, u32 Depth)
{
  switch(UnionStruct.Type)
  {
    case type_ast_node_statement:
    {
      DebugPrint(CSz("type_ast_node_statement {\n"), Depth);
      DebugPrint(UnionStruct.ast_node_statement, Depth+4);
      DebugPrint(CSz("}\n"), Depth);
    } break;

    case type_ast_node_access:
    {
      DebugPrint(CSz("type_ast_node_access {\n"), Depth);
      DebugPrint(UnionStruct.ast_node_access, Depth+4);
      DebugPrint(CSz("}\n"), Depth);
    } break;

    case type_ast_node_literal:
    {
      DebugPrint(CSz("type_ast_node_literal {\n"), Depth);
      DebugPrint(UnionStruct.ast_node_literal, Depth+4);
      DebugPrint(CSz("}\n"), Depth);
    } break;

    case type_ast_node_expression:
    {
      DebugPrint(CSz("type_ast_node_expression {\n"), Depth);
      DebugPrint(UnionStruct.ast_node_expression, Depth+4);
      DebugPrint(CSz("}\n"), Depth);
    } break;

    case type_ast_node_parenthesized:
    {
      DebugPrint(CSz("type_ast_node_parenthesized {\n"), Depth);
      DebugPrint(UnionStruct.ast_node_parenthesized, Depth+4);
      DebugPrint(CSz("}\n"), Depth);
    } break;

    case type_ast_node_operator:
    {
      DebugPrint(CSz("type_ast_node_operator {\n"), Depth);
      DebugPrint(UnionStruct.ast_node_operator, Depth+4);
      DebugPrint(CSz("}\n"), Depth);
    } break;

    case type_ast_node_function_call:
    {
      DebugPrint(CSz("type_ast_node_function_call {\n"), Depth);
      DebugPrint(UnionStruct.ast_node_function_call, Depth+4);
      DebugPrint(CSz("}\n"), Depth);
    } break;

    case type_ast_node_return:
    {
      DebugPrint(CSz("type_ast_node_return {\n"), Depth);
      DebugPrint(UnionStruct.ast_node_return, Depth+4);
      DebugPrint(CSz("}\n"), Depth);
    } break;

    case type_ast_node_initializer_list:
    {
      DebugPrint(CSz("type_ast_node_initializer_list {\n"), Depth);
      DebugPrint(UnionStruct.ast_node_initializer_list, Depth+4);
      DebugPrint(CSz("}\n"), Depth);
    } break;

    case type_ast_node_symbol:
    {
      DebugPrint(CSz("type_ast_node_symbol {\n"), Depth);
      DebugPrint(UnionStruct.ast_node_symbol, Depth+4);
      DebugPrint(CSz("}\n"), Depth);
    } break;

    case type_ast_node_variable_def:
    {
      DebugPrint(CSz("type_ast_node_variable_def {\n"), Depth);
      DebugPrint(UnionStruct.ast_node_variable_def, Depth+4);
      DebugPrint(CSz("}\n"), Depth);
    } break;

    case type_ast_node_type_specifier:
    {
      DebugPrint(CSz("type_ast_node_type_specifier {\n"), Depth);
      DebugPrint(UnionStruct.ast_node_type_specifier, Depth+4);
      DebugPrint(CSz("}\n"), Depth);
    } break;

    case type_ast_node_noop:
    {
      InvalidCodePath();
    } break;
  }
}
