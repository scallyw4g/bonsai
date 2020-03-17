
    function meta_arg_operator
    MetaArgOperator(counted_string S)
    {
      meta_arg_operator Result = {};

      
        if (StringsMatch(S, CSz("meta_arg_operator_noop"))) { return meta_arg_operator_noop; }

        if (StringsMatch(S, CSz("name"))) { return name; }

        if (StringsMatch(S, CSz("map_values"))) { return map_values; }


      return Result;
    }

