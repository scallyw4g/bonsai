
function counted_string
ToString(meta_func_arg_type Type)
{
  counted_string Result = {};
  switch (Type)
  {
    case arg_type_noop: { Result = CS("arg_type_noop"); } break;
    case arg_type_enum: { Result = CS("arg_type_enum"); } break;
    case arg_type_struct: { Result = CS("arg_type_struct"); } break;
  }
  return Result;
}


