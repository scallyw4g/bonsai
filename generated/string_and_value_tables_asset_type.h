// src/engine/editor.cpp:469:0

link_internal counted_string
ToStringPrefixless(asset_type Type)
{
  counted_string Result = {};
  switch (Type)
  {
        case AssetType_Undefined: { Result = CSz("Undefined"); } break;
    case AssetType_Models: { Result = CSz("Models"); } break;
    case AssetType_WorldChunk: { Result = CSz("WorldChunk"); } break;

    
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (EnumType.name)", Type); } */
  return Result;
}

link_internal counted_string
ToString(asset_type Type)
{
  counted_string Result = {};
  switch (Type)
  {
        case AssetType_Undefined: { Result = CSz("AssetType_Undefined"); } break;
    case AssetType_Models: { Result = CSz("AssetType_Models"); } break;
    case AssetType_WorldChunk: { Result = CSz("AssetType_WorldChunk"); } break;

    
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (EnumType.name)", Type); } */
  return Result;
}

link_internal asset_type
AssetType(counted_string S)
{
  asset_type Result = {};

    if (StringsMatch(S, CSz("AssetType_Undefined"))) { return AssetType_Undefined; }
  if (StringsMatch(S, CSz("AssetType_Models"))) { return AssetType_Models; }
  if (StringsMatch(S, CSz("AssetType_WorldChunk"))) { return AssetType_WorldChunk; }

  return Result;
}


