
    function meta_func_arg_type
    MetaFuncArgType(counted_string S)
    {
      meta_func_arg_type Result = {};

      
          if (StringsMatch(S, CSz("arg_type_noop"))) { return arg_type_noop; }

          if (StringsMatch(S, CSz("arg_type_enum"))) { return arg_type_enum; }

          if (StringsMatch(S, CSz("arg_type_struct"))) { return arg_type_struct; }


      return Result;
    }

