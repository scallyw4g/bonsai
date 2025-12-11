// callsite
// src/engine/editor.h:1676:0

// def (gen_constructor)
// external/bonsai_stdlib/src/poof_functions.h:115:0
link_internal layer_settings
LayerSettings(  brush_layer_type   Type , noise_layer   Noise , shape_layer   Shape , world_edit_brush *  Brush , v3   Offset , v3   Rotation , b8   Invert , b8   Normalized , b8 *  Reserved , r32   ValueBias , r32   Power , world_edit_blend_mode_modifier   ValueFunc , world_edit_blend_mode   BlendMode , smooth_blend_params   Smoothing , world_edit_color_blend_mode   ColorMode , v3i   BasisOffset , v3   HSVColor , b32   Disabled  )
{
  layer_settings Reuslt = {
            .Type = Type

,        .Noise = Noise

,        .Shape = Shape

,        .Brush = Brush

,        .Offset = Offset

,        .Rotation = Rotation

,        .Invert = Invert

,        .Normalized = Normalized

,        .Reserved = {}

,        .ValueBias = ValueBias

,        .Power = Power

,        .ValueFunc = ValueFunc

,        .BlendMode = BlendMode

,        .Smoothing = Smoothing

,        .ColorMode = ColorMode

,        .BasisOffset = BasisOffset

,        .HSVColor = HSVColor

,        .Disabled = Disabled


  };

    
  
  
  
  
  
  
  
    RangeIterator(Index, s32())
  {
    Reuslt.Reserved[Index] = Reserved[Index];
  }

  
  
  
  
  
  
  
  
  

  return Reuslt;
}

