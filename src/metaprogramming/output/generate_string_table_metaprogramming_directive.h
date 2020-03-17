
    function counted_string
    ToString(metaprogramming_directive Type)
    {
      counted_string Result = {};
      switch (Type)
      {
        
          case meta_directive_noop: { Result = CSz("meta_directive_noop"); } break;

          case generate_cursor_deprecated: { Result = CSz("generate_cursor_deprecated"); } break;

          case for_members_in: { Result = CSz("for_members_in"); } break;

          case d_union: { Result = CSz("d_union"); } break;

          case enum_only: { Result = CSz("enum_only"); } break;

          case member_is_or_contains_type: { Result = CSz("member_is_or_contains_type"); } break;

          case for_all_datatypes: { Result = CSz("for_all_datatypes"); } break;

          case named_list: { Result = CSz("named_list"); } break;

          case def_func: { Result = CSz("def_func"); } break;

          case def_func_2: { Result = CSz("def_func_2"); } break;

      }
      return Result;
    }

