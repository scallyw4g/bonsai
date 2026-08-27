// callsite
// src/engine/editor.cpp:575:0

// def (string_and_value_tables)
// external/bonsai_stdlib/src/poof_functions.h:2148:0
link_internal b32
IsValid(asset_type Value)
{
  b32 Result = False;
  switch (Value)
  {
            case AssetType_Undefined:

        case AssetType_Sound:

        case AssetType_Models:

        case AssetType_Texture:

        case AssetType_WorldChunk:


    {
      Result = True;
    }
  }
  return Result;
}



link_internal counted_string
ToStringPrefixless(asset_type Type)
{
  cs Result = {};
  if (IsValid(Type))
  {
    switch (Type)
    {
                  case AssetType_Undefined: { Result = CSz("Undefined"); } break;

            case AssetType_Sound: { Result = CSz("Sound"); } break;

            case AssetType_Models: { Result = CSz("Models"); } break;

            case AssetType_Texture: { Result = CSz("Texture"); } break;

            case AssetType_WorldChunk: { Result = CSz("WorldChunk"); } break;



      
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
ToString(asset_type Type)
{
  Assert(IsValid(Type));

  counted_string Result = {};
  switch (Type)
  {
            case AssetType_Undefined: { Result = CSz("AssetType_Undefined"); } break;

        case AssetType_Sound: { Result = CSz("AssetType_Sound"); } break;

        case AssetType_Models: { Result = CSz("AssetType_Models"); } break;

        case AssetType_Texture: { Result = CSz("AssetType_Texture"); } break;

        case AssetType_WorldChunk: { Result = CSz("AssetType_WorldChunk"); } break;



    
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (enum_t.name)", Type); } */
  return Result;
}

link_internal asset_type
AssetType(counted_string S)
{
  asset_type Result = {};

    if (StringsMatch(S, CSz("AssetType_Undefined"))) { return AssetType_Undefined; }
  if (StringsMatch(S, CSz("AssetType_Sound"))) { return AssetType_Sound; }
  if (StringsMatch(S, CSz("AssetType_Models"))) { return AssetType_Models; }
  if (StringsMatch(S, CSz("AssetType_Texture"))) { return AssetType_Texture; }
  if (StringsMatch(S, CSz("AssetType_WorldChunk"))) { return AssetType_WorldChunk; }


  return Result;
}


