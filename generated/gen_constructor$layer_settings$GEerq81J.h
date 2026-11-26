// callsite
// src/engine/editor.h:1328:0

// def (gen_constructor)
// external/bonsai_stdlib/src/poof_functions.h:126:0
link_internal layer_settings
LayerSettings(  brush_layer_type   Type , world_edit_blend_mode   BlendMode , world_edit_color_mode   ColorMode , file_traversal_node   ColorTextureFilePath , f32   TextureScale , v3   HSVColor , noise_layer   Noise , shape_layer   Shape , world_edit_brush *  Brush , v3   Offset , v3   Rotation , world_edit_op_flags   Flags , r32   ValueBias , r32   Power , world_edit_blend_mode_modifier   ValueFunc , smooth_blend_params   Smoothing , v3i   BasisOffset  )
{
  layer_settings Reuslt = {
            .Type = Type

,        .BlendMode = BlendMode

,        .ColorMode = ColorMode

,        .ColorTextureFilePath = ColorTextureFilePath

,        .TextureScale = TextureScale

,        .HSVColor = HSVColor

,        .Noise = Noise

,        .Shape = Shape

,        .Brush = Brush

,        .Offset = Offset

,        .Rotation = Rotation

,        .Flags = Flags

,        .ValueBias = ValueBias

,        .Power = Power

,        .ValueFunc = ValueFunc

,        .Smoothing = Smoothing

,        .BasisOffset = BasisOffset


  };

    
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  

  return Reuslt;
}

