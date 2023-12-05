link_internal asset_id
AssetId( file_traversal_node FileNode , u32 SlotIndex  )
{
  asset_id Reuslt = {
    .FileNode = FileNode,
    .SlotIndex = SlotIndex
  };
  return Reuslt;
}

