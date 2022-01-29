    bonsai_function meta_arg_operator
    MetaArgOperator(counted_string S)
    {
      meta_arg_operator Result = {};

                if (StringsMatch(S, CSz("meta_arg_operator_noop"))) { return meta_arg_operator_noop; }
          if (StringsMatch(S, CSz("name"))) { return name; }
          if (StringsMatch(S, CSz("type"))) { return type; }
          if (StringsMatch(S, CSz("value"))) { return value; }
          if (StringsMatch(S, CSz("map_values"))) { return map_values; }
          if (StringsMatch(S, CSz("map_members"))) { return map_members; }
          if (StringsMatch(S, CSz("is_enum"))) { return is_enum; }
          if (StringsMatch(S, CSz("is_struct"))) { return is_struct; }


      return Result;
    }

