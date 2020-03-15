
function metaprogramming_directive
MetaprogrammingDirective(counted_string S)
{
  metaprogramming_directive Result = {};
  if (StringsMatch(CS("noop"), S)) { Result = noop; }
  if (StringsMatch(CS("generate_stream"), S)) { Result = generate_stream; }
  if (StringsMatch(CS("generate_cursor"), S)) { Result = generate_cursor; }
  if (StringsMatch(CS("generate_string_table"), S)) { Result = generate_string_table; }
  if (StringsMatch(CS("generate_value_table"), S)) { Result = generate_value_table; }
  if (StringsMatch(CS("for_enum_values"), S)) { Result = for_enum_values; }
  if (StringsMatch(CS("for_members_in"), S)) { Result = for_members_in; }
  if (StringsMatch(CS("d_union"), S)) { Result = d_union; }
  if (StringsMatch(CS("enum_only"), S)) { Result = enum_only; }
  if (StringsMatch(CS("member_is_or_contains_type"), S)) { Result = member_is_or_contains_type; }
  if (StringsMatch(CS("for_all_datatypes"), S)) { Result = for_all_datatypes; }
  if (StringsMatch(CS("named_list"), S)) { Result = named_list; }
  if (StringsMatch(CS("def_func"), S)) { Result = def_func; }
  return Result;
}


