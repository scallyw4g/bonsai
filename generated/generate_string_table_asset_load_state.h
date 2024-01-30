// src/engine/asset.h:140:0

link_internal counted_string
ToString(asset_load_state Type)
{
  counted_string Result = {};
  switch (Type)
  {
    case AssetLoadState_Unloaded: { Result = CSz("Unloaded"); } break;
    case AssetLoadState_Allocated: { Result = CSz("Allocated"); } break;
    case AssetLoadState_Queued: { Result = CSz("Queued"); } break;
    case AssetLoadState_Loaded: { Result = CSz("Loaded"); } break;
    case AssetLoadState_Error: { Result = CSz("Error"); } break;

    
  }
  return Result;
}

