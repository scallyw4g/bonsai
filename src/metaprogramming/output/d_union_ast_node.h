enum ast_node_type
{
  type_ast_node_noop,
  type_ast_node_access,
  type_ast_node_function_call,
  type_ast_node_scope,
  type_ast_node_assignment,
  type_ast_node_address_of,
  type_ast_node_initializer_list,
  type_ast_node_ignored,
  type_ast_node_preprocessor_directive,
  type_ast_node_variable_def,
};

struct ast_node
{
  ast_node_type Type;
  umm Next;

  union
  {
    ast_node_access ast_node_access;
    ast_node_function_call ast_node_function_call;
    ast_node_scope ast_node_scope;
    ast_node_assignment ast_node_assignment;
    ast_node_address_of ast_node_address_of;
    ast_node_initializer_list ast_node_initializer_list;
    ast_node_ignored ast_node_ignored;
    ast_node_preprocessor_directive ast_node_preprocessor_directive;
    ast_node_variable_def ast_node_variable_def;
  };
};


