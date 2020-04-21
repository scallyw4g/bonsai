
        
            if (InputNode->ast_node_var.(Children))
            {
              Traverse(InputNode->ast_node_var.(Children));
            }

            if (InputNode->ast_node_scope.(Children))
            {
              Traverse(InputNode->ast_node_scope.(Children));
            }

            if (InputNode->ast_node_assignment.(Children))
            {
              Traverse(InputNode->ast_node_assignment.(Children));
            }

            if (InputNode->ast_node_address_of.(Children))
            {
              Traverse(InputNode->ast_node_address_of.(Children));
            }

            if (InputNode->ast_node_ignored.(Children))
            {
              Traverse(InputNode->ast_node_ignored.(Children));
            }

            if (InputNode->ast_node_preprocessor_directive.(Children))
            {
              Traverse(InputNode->ast_node_preprocessor_directive.(Children));
            }


