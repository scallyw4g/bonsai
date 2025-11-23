// callsite
// src/engine/asset.h:141:0

// def (generate_string_table)
// external/bonsai_stdlib/src/poof_functions.h:1821:0
link_internal b32
IsValid(asset_load_state Value)
{
  b32 Result = False;
  switch (Value)
  {
        case AssetLoadState_Unloaded:
    case AssetLoadState_Allocated:
    case AssetLoadState_Queued:
    case AssetLoadState_Loaded:
    case AssetLoadState_Error:

    {
      Result = True;
    }
  }
  return Result;
}



link_internal counted_string
ToStringPrefixless(asset_load_state Type)
{
  cs Result = {};
  if (IsValid(Type))
  {
    switch (Type)
    {
            case AssetLoadState_Unloaded: { Result = CSz("Unloaded"); } break;
      case AssetLoadState_Allocated: { Result = CSz("Allocated"); } break;
      case AssetLoadState_Queued: { Result = CSz("Queued"); } break;
      case AssetLoadState_Loaded: { Result = CSz("Loaded"); } break;
      case AssetLoadState_Error: { Result = CSz("Error"); } break;


      
    }
  }
  else
  {
    Result = CSz("(CORRUPT ENUM VALUE)");
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (enum_t.name)", Type); } */
  return Result;
}

link_internal counted_string
ToString(asset_load_state Type)
{
  Assert(IsValid(Type));

  counted_string Result = {};
  switch (Type)
  {
        case AssetLoadState_Unloaded: { Result = CSz("AssetLoadState_Unloaded"); } break;
    case AssetLoadState_Allocated: { Result = CSz("AssetLoadState_Allocated"); } break;
    case AssetLoadState_Queued: { Result = CSz("AssetLoadState_Queued"); } break;
    case AssetLoadState_Loaded: { Result = CSz("AssetLoadState_Loaded"); } break;
    case AssetLoadState_Error: { Result = CSz("AssetLoadState_Error"); } break;


    
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (enum_t.name)", Type); } */
  return Result;
}

