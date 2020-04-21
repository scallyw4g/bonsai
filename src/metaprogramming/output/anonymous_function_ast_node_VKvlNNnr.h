
          
              case type_ast_node_var:
              {
                
                   Traverse(InputNode->ast_node_var.Value);

                   Traverse(InputNode->ast_node_var.NextStatement);

              } break;

              case type_ast_node_scope:
              {
                
                   Traverse(InputNode->ast_node_scope.Children);

              } break;

              case type_ast_node_assignment:
              {
                
                   Traverse(InputNode->ast_node_assignment.LHS);

                   Traverse(InputNode->ast_node_assignment.RHS);

              } break;

              case type_ast_node_address_of:
              {
                
                   Traverse(InputNode->ast_node_address_of.Operand);

              } break;

              case type_ast_node_ignored:
              {
                
                   Traverse(InputNode->ast_node_ignored.Children);

              } break;

              case type_ast_node_preprocessor_directive:
              {
                
                   Traverse(InputNode->ast_node_preprocessor_directive.Children);

              } break;


