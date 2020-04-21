
        switch (Current->Type)
        {
          
              case type_ast_node_scope:
              {
                
                   Traverse(Current->ast_node_scope.Children);

              } break;

              case type_ast_node_assignment:
              {
                
                   Traverse(Current->ast_node_assignment.RHS);

              } break;

              case type_ast_node_address_of:
              {
                
                   Traverse(Current->ast_node_address_of.Operand);

              } break;

              case type_ast_node_ignored:
              {
                
                   Traverse(Current->ast_node_ignored.Children);

              } break;

              case type_ast_node_preprocessor_directive:
              {
                
                   Traverse(Current->ast_node_preprocessor_directive.Children);

              } break;

              case type_ast_node_variable_def:
              {
                
                   Traverse(Current->ast_node_variable_def.Value);

              } break;


          default: {} break;
        }

