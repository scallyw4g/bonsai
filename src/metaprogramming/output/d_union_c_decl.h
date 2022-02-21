enum struct_member_type
{
  type_struct_member_noop,
  type_variable_decl,
  type_struct_decl,
  type_function_decl,
  type_struct_member_anonymous,
};

struct struct_member
{
  struct_member_type Type;

  union
  {
    variable_decl variable_decl;
    function_decl function_decl;
    struct_member_anonymous struct_member_anonymous;
  };
};


