// callsite
// src/engine/editor.h:1502:0

// def (gen_constructor)
// external/bonsai_stdlib/src/poof_functions.h:115:0
link_internal brush_settings
BrushSettings(  brush_layer_type   Type , noise_layer   Noise , shape_layer   Shape , world_edit_brush *  Brush , r32   ValueBias , world_edit_blend_mode_modifier   ValueModifier , world_edit_blend_mode   LayerBlendMode , world_edit_color_blend_mode   ColorMode , b8   Invert , rect3i   SelectionModifier , v3i   BasisOffset , v3   HSVColor , b32   Disabled  )
{
  brush_settings Reuslt = {
            .Type = Type

,        .Noise = Noise

,        .Shape = Shape

,        .Brush = Brush

,        .ValueBias = ValueBias

,        .ValueModifier = ValueModifier

,        .LayerBlendMode = LayerBlendMode

,        .ColorMode = ColorMode

,        .Invert = Invert

,        .SelectionModifier = SelectionModifier

,        .BasisOffset = BasisOffset

,        .HSVColor = HSVColor

,        .Disabled = Disabled


  };

    
  
  
  
  
  
  
  
  
  
  
  
  

  return Reuslt;
}

