enum struct_member_type
{
  type_struct_member_noop,
  type_c_decl_variable,
  type_c_decl_function,
  type_c_decl_union,
};

struct struct_member
{
  struct_member_type Type;

  union
  {
    c_decl_variable c_decl_variable;
    c_decl_function c_decl_function;
    c_decl_union c_decl_union;
  };
};


