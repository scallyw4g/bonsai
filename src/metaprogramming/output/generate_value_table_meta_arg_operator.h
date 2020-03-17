
function meta_arg_operator
MetaArgOperator(counted_string S)
{
  meta_arg_operator Result = {};
  if (StringsMatch(CS("meta_arg_operator_noop"), S)) { Result = meta_arg_operator_noop; }
  if (StringsMatch(CS("name"), S)) { Result = name; }
  if (StringsMatch(CS("map_values"), S)) { Result = map_values; }
  return Result;
}


