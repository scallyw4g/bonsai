// src/engine/editor.cpp:270:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, entity *Element, cs Name, ui_render_params *Params = &DefaultUiRenderParams_Button)
{
  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle entity", Element), Params))
      {
        DidToggle = True;
        PushNewRow(Ui);
      }
      else
      {
        DrawChildren = False;
      }
    }

    if (DrawChildren)
    {
      PushTableStart(Ui);
      if (DidToggle) { OPEN_INDENT_FOR_TOGGLEABLE_REGION(); }
                                                DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(entity_id*, &Element->Id),
        CSz("Id"),
        Params
        );







      
                        DoEditorUi_entity_P(Ui, Window, Element, CSz("cp P"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);




      
                                          DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(v3*, &Element->EulerAngles),
        CSz("EulerAngles"),
        Params
        , -PI32, PI32 );







      
                                          DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(r32*, &Element->Scale),
        CSz("Scale"),
        Params
        );







            PushNewRow(Ui);

                                          DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(v3*, &Element->_CollisionVolumeRadius),
        CSz("_CollisionVolumeRadius"),
        Params
        );







      
                                          DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(physics*, &Element->Physics),
        CSz("Physics"),
        Params
        );







      
                                          DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(asset_id*, &Element->AssetId),
        CSz("AssetId"),
        Params
        );







      
                                          DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(u64*, &Element->ModelIndex),
        CSz("ModelIndex"),
        Params
        );







            PushNewRow(Ui);

                                          DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(collision_event*, &Element->LastResolvedCollision),
        CSz("LastResolvedCollision"),
        Params
        );







      
                                          DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(entity_position_info*, &Element->LastResolvedPosInfo),
        CSz("LastResolvedPosInfo"),
        Params
        );







      
                                          DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(particle_system*, Element->Emitter),
        CSz("Emitter"),
        Params
        );







      
                                          DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(entity_state*, &Element->State),
        CSz("State"),
        Params
        );







      
                                          DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(entity_behavior_flags*, &Element->Behavior),
        CSz("Behavior"),
        Params
        );







      
                                          DoEditorUi(Ui,
        Window,
        // Cast to remove const/volatile keywords if they're there
        Cast(entity_id*, &Element->Carrying),
        CSz("Carrying"),
        Params
        );







      
                          if (EntityUserDataEditorUi) {EntityUserDataEditorUi(Ui, Window, &Element->UserType, &Element->UserData, Name, EDITOR_UI_FUNCTION_INSTANCE_NAMES);}
                 else                        {DoEditorUi(Ui, Window, &Element->UserType, Name, EDITOR_UI_FUNCTION_INSTANCE_NAMES); }
    ;




            PushNewRow(Ui);

                          if (EntityUserDataEditorUi) { /* User took control, skip this because it's intended */ }
                 else                        {DoEditorUi(Ui, Window, &Element->UserData, Name, EDITOR_UI_FUNCTION_INSTANCE_NAMES); }
    ;




            PushNewRow(Ui);
      if (DidToggle) { CLOSE_INDENT_FOR_TOGGLEABLE_REGION(); }
      PushTableEnd(Ui);
    }
    else
    {
      PushNewRow(Ui);
    }

  }
  else
  {
    PushColumn(Ui, Name, Params);
    PushColumn(Ui, CSz("(null)"), Params);
    PushNewRow(Ui);
  }

}

