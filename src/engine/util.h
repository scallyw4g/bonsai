

link_internal maybe_file_traversal_node
EngineDrawFileNodesFilteredHelper(file_traversal_node Node, u64 Params)
{
  engine_resources *Engine = GetEngineResources();
  filtered_file_traversal_helper_params *HelperParams = ReinterpretCast(filtered_file_traversal_helper_params*, Params);
  maybe_file_traversal_node Result = DrawFileNodes(&Engine->Ui, Node, HelperParams);
  return Result;
}

link_internal b32 DefaultFileFilter(file_traversal_node *Node) { return True; }

link_internal maybe_file_traversal_node
EngineDrawFileNodesHelper(file_traversal_node Node, u64 Window)
{
  engine_resources *Engine = GetEngineResources();
  filtered_file_traversal_helper_params HelperParams = {(window_layout*)Window, DefaultFileFilter};
  maybe_file_traversal_node Result = DrawFileNodes(&Engine->Ui, Node, &HelperParams);
  return Result;
}

