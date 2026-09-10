// callsite
// src/engine/editor.h:1275:0

// def (string_and_value_tables)
// external/bonsai_stdlib/src/poof_functions.h:2148:0
link_internal b32
IsValid(world_edit_color_mode Value)
{
  b32 Result = False;
  switch (Value)
  {
            case WorldEditColorMode_Color:

        case WorldEditColorMode_Texture:

        case WorldEditColorMode_TintedTexture:


    {
      Result = True;
    }
  }
  return Result;
}



link_internal counted_string
ToStringPrefixless(world_edit_color_mode Type)
{
  cs Result = {};
  if (IsValid(Type))
  {
    switch (Type)
    {
                  case WorldEditColorMode_Color: { Result = CSz("Color"); } break;

            case WorldEditColorMode_Texture: { Result = CSz("Texture"); } break;

            case WorldEditColorMode_TintedTexture: { Result = CSz("TintedTexture"); } break;



      
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
ToString(world_edit_color_mode Type)
{
  Assert(IsValid(Type));

  counted_string Result = {};
  switch (Type)
  {
            case WorldEditColorMode_Color: { Result = CSz("WorldEditColorMode_Color"); } break;

        case WorldEditColorMode_Texture: { Result = CSz("WorldEditColorMode_Texture"); } break;

        case WorldEditColorMode_TintedTexture: { Result = CSz("WorldEditColorMode_TintedTexture"); } break;



    
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (enum_t.name)", Type); } */
  return Result;
}

link_internal world_edit_color_mode
WorldEditColorMode(counted_string S)
{
  world_edit_color_mode Result = {};

    if (StringsMatch(S, CSz("WorldEditColorMode_Color"))) { return WorldEditColorMode_Color; }
  if (StringsMatch(S, CSz("WorldEditColorMode_Texture"))) { return WorldEditColorMode_Texture; }
  if (StringsMatch(S, CSz("WorldEditColorMode_TintedTexture"))) { return WorldEditColorMode_TintedTexture; }


  return Result;
}


