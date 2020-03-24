enum struct_member_type
{
  type_struct_member_noop,
  type_variable,
  type_struct_member_function,
  type_struct_member_anonymous,
};

struct struct_member
{
  struct_member_type Type;

  union
  {
    variable variable;
    struct_member_function struct_member_function;
    struct_member_anonymous struct_member_anonymous;
  };
};


