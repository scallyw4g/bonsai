// callsite
// src/engine/editor.h:1236:0

// def (gen_constructor)
// external/bonsai_stdlib/src/poof_functions.h:115:0
link_internal brush_settings
BrushSettings(  brush_layer_type   Type , noise_layer   Noise , shape_layer   Shape , world_edit_brush *  Brush , f32   Power , r32   ValueBias , f32   Threshold , world_edit_blend_mode_modifier   ValueModifier , world_edit_blend_mode   BlendMode , world_edit_color_blend_mode   ColorMode , b8   Invert , rect3i   SelectionModifier , v3i   BasisOffset , v3   HSVColor  )
{
  brush_settings Reuslt = {
            .Type = Type

,        .Noise = Noise

,        .Shape = Shape

,        .Brush = Brush

,        .Power = Power

,        .ValueBias = ValueBias

,        .Threshold = Threshold

,        .ValueModifier = ValueModifier

,        .BlendMode = BlendMode

,        .ColorMode = ColorMode

,        .Invert = Invert

,        .SelectionModifier = SelectionModifier

,        .BasisOffset = BasisOffset

,        .HSVColor = HSVColor


  };

    
  
  
  
  
  
  
  
  
  
  
  
  
  

  return Reuslt;
}

