
        switch (Current->Type)
        {
          
              case type_ast_node_var:
              {
                
                   Traverse(Current->ast_node_var.Value);

                   Traverse(Current->ast_node_var.NextStatement);

              } break;

              case type_ast_node_scope:
              {
                
                   Traverse(Current->ast_node_scope.Children);

              } break;

              case type_ast_node_assignment:
              {
                
                   Traverse(Current->ast_node_assignment.LHS);

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


          default: {} break;
        }

