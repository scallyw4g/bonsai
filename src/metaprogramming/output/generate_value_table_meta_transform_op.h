
    function meta_transform_op
    MetaTransformOp(counted_string S)
    {
      meta_transform_op Result = {};

      
        if (StringsMatch(S, CSz("meta_transform_op_noop"))) { return meta_transform_op_noop; }

        if (StringsMatch(S, CSz("to_capital_case"))) { return to_capital_case; }


      return Result;
    }

