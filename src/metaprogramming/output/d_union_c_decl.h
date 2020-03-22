enum struct_member_type
{
  type_struct_member_noop,
  type_variable_decl,
  type_struct_member_function,
  type_struct_member_union,
};

struct struct_member
{
  struct_member_type Type;

  union
  {
    variable_decl variable_decl;
    struct_member_function struct_member_function;
    struct_member_union struct_member_union;
  };
};


