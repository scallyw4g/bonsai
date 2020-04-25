
        switch (Current->Type)
        {
          
              case type_ast_node_expression:
              {
                
                   Traverse(Current->ast_node_expression.Value);

              } break;

              case type_ast_node_variable_def:
              {
                
                   Traverse(Current->ast_node_variable_def.Value);

              } break;


          default: {} break;
        }

