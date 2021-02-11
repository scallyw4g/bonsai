enum struct_member_function_type
{
  type_struct_member_function_noop,
  type_struct_member_function_normal,
  type_struct_member_function_operator,
  type_struct_member_function_constructor,
  type_struct_member_function_destructor,
};

struct struct_member_function
{
  struct_member_function_type Type;

  union
  {
  };
};


