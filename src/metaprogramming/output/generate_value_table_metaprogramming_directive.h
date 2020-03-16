
    _Pragma("GCC diagnostic push")
    _Pragma("GCC diagnostic ignored \"-Wunused-parameter\"")
    
      function metaprogramming_directive
      ToEnum(counted_string S, metaprogramming_directive Ignored = (metaprogramming_directive)0)
      {
        
          if (StringsMatch(S, CSz("meta_directive_noop"))) { return meta_directive_noop; }

          if (StringsMatch(S, CSz("generate_stream"))) { return generate_stream; }

          if (StringsMatch(S, CSz("generate_cursor"))) { return generate_cursor; }

          if (StringsMatch(S, CSz("generate_value_table"))) { return generate_value_table; }

          if (StringsMatch(S, CSz("for_enum_values"))) { return for_enum_values; }

          if (StringsMatch(S, CSz("for_members_in"))) { return for_members_in; }

          if (StringsMatch(S, CSz("d_union"))) { return d_union; }

          if (StringsMatch(S, CSz("enum_only"))) { return enum_only; }

          if (StringsMatch(S, CSz("member_is_or_contains_type"))) { return member_is_or_contains_type; }

          if (StringsMatch(S, CSz("for_all_datatypes"))) { return for_all_datatypes; }

          if (StringsMatch(S, CSz("named_list"))) { return named_list; }

          if (StringsMatch(S, CSz("def_func"))) { return def_func; }


        return (metaprogramming_directive)0;
      }

    _Pragma("GCC diagnostic pop")

