
    
    function counted_string
    ToString( metaprogramming_directive Type)
    {
      counted_string Result = {};
      switch (Type)
      {
        
            case meta_directive_noop: { Result = CSz("meta_directive_noop"); } break;

            case generate_cursor_deprecated: { Result = CSz("generate_cursor_deprecated"); } break;

            case enum_only: { Result = CSz("enum_only"); } break;

            case d_union: { Result = CSz("d_union"); } break;

            case for_datatypes: { Result = CSz("for_datatypes"); } break;

            case named_list: { Result = CSz("named_list"); } break;

            case func: { Result = CSz("func"); } break;

      }
      return Result;
    }

    
    function metaprogramming_directive
    MetaprogrammingDirective(counted_string S)
    {
      metaprogramming_directive Result = {};

      
          if (StringsMatch(S, CSz("meta_directive_noop"))) { return meta_directive_noop; }

          if (StringsMatch(S, CSz("generate_cursor_deprecated"))) { return generate_cursor_deprecated; }

          if (StringsMatch(S, CSz("enum_only"))) { return enum_only; }

          if (StringsMatch(S, CSz("d_union"))) { return d_union; }

          if (StringsMatch(S, CSz("for_datatypes"))) { return for_datatypes; }

          if (StringsMatch(S, CSz("named_list"))) { return named_list; }

          if (StringsMatch(S, CSz("func"))) { return func; }


      return Result;
    }


