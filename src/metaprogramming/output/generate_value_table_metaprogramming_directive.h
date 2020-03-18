
    function metaprogramming_directive
    MetaprogrammingDirective(counted_string S)
    {
      metaprogramming_directive Result = {};

      
          if (StringsMatch(S, CSz("meta_directive_noop"))) { return meta_directive_noop; }

          if (StringsMatch(S, CSz("generate_cursor_deprecated"))) { return generate_cursor_deprecated; }

          if (StringsMatch(S, CSz("d_union"))) { return d_union; }

          if (StringsMatch(S, CSz("enum_only"))) { return enum_only; }

          if (StringsMatch(S, CSz("member_is_or_contains_type"))) { return member_is_or_contains_type; }

          if (StringsMatch(S, CSz("for_all_datatypes"))) { return for_all_datatypes; }

          if (StringsMatch(S, CSz("named_list"))) { return named_list; }

          if (StringsMatch(S, CSz("func"))) { return func; }


      return Result;
    }

