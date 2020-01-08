enum c_decl_type
{
  type_c_decl_noop,
  type_c_decl_function,
  type_c_decl_variable,
};

struct c_decl
{
  c_decl_type Type;

  union
  {
    c_decl_function c_decl_function;
    c_decl_variable c_decl_variable;
  };
};
