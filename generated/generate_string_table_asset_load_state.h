// src/engine/asset.h:140:0

link_internal counted_string
ToString(asset_load_state Type)
{
  counted_string Result = {};
  switch (Type)
  {
    case AssetLoadState_Unloaded: { Result = CSz("AssetLoadState_Unloaded"); } break;
    case AssetLoadState_Allocated: { Result = CSz("AssetLoadState_Allocated"); } break;
    case AssetLoadState_Queued: { Result = CSz("AssetLoadState_Queued"); } break;
    case AssetLoadState_Loaded: { Result = CSz("AssetLoadState_Loaded"); } break;
    case AssetLoadState_Error: { Result = CSz("AssetLoadState_Error"); } break;

    
  }
  return Result;
}

