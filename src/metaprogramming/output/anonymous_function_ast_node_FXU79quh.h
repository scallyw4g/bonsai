
        switch (Current->Type)
        {
          
              case type_ast_node_expression:
              {
                
                   Traverse(Current->ast_node_expression.Value);

                   Traverse(Current->ast_node_expression.Next);

              } break;

              case type_ast_node_symbol:
              {
                
                   Traverse(Current->ast_node_symbol.Access);

              } break;

              case type_ast_node_variable_def:
              {
                
                   Traverse(Current->ast_node_variable_def.Value);

              } break;


          default: {} break;
        }

