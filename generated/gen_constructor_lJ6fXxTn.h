// callsite
// src/engine/editor.h:1777:0

// def (gen_constructor)
// external/bonsai_stdlib/src/poof_functions.h:115:0
link_internal layer_settings
LayerSettings(  brush_layer_type   Type , noise_layer   Noise , shape_layer   Shape , world_edit_brush *  Brush , v3   Offset , v3   Rotation , b8   Invert , b8   Normalized , b8   Reserved0 , b8   Reserved1 , r32   ValueBias , r32   Power , world_edit_blend_mode_modifier   ValueFunc , world_edit_blend_mode   BlendMode , smooth_blend_params   Smoothing , v3i   BasisOffset , world_edit_color_mode   ColorMode , file_traversal_node   ColorTextureFilePath , v3   HSVColor , b32   Disabled  )
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

,        .Reserved0 = Reserved0

,        .Reserved1 = Reserved1

,        .ValueBias = ValueBias

,        .Power = Power

,        .ValueFunc = ValueFunc

,        .BlendMode = BlendMode

,        .Smoothing = Smoothing

,        .BasisOffset = BasisOffset

,        .ColorMode = ColorMode

,        .ColorTextureFilePath = ColorTextureFilePath

,        .HSVColor = HSVColor

,        .Disabled = Disabled


  };

    
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  

  return Reuslt;
}

