
function meta_func_arg_type
MetaFuncArgType(counted_string S)
{
  meta_func_arg_type Result = {};
  if (StringsMatch(CS("arg_type_noop"), S)) { Result = arg_type_noop; }
  if (StringsMatch(CS("arg_type_enum"), S)) { Result = arg_type_enum; }
  if (StringsMatch(CS("arg_type_struct"), S)) { Result = arg_type_struct; }
  return Result;
}


