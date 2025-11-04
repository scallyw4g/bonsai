// src/engine/editor.h:303:0
struct entity;
link_internal void DoEditorUi(renderer_2d *Ui, window_layout *Window, entity *Element, cs Name, u32 ParentHash, ui_render_params *Params = &DefaultUiRenderParams_Button)


{
  u32 ThisHash = ChrisWellonsIntegerHash_lowbias32(ParentHash ^ 0x2D655F50);

  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to support
    // not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle entity", Element, ThisHash), Params))
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
      if (Name.Count) { PushTableStart(Ui); }

      if (DidToggle) { OPEN_INDENT_FOR_TOGGLEABLE_REGION(); }
            

      { 
        
        
        
        cs MemberName = CSz("Id");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(entity_id*, &Element->Id),
          MemberName,
          ThisHash,
          Params
          );







        


      }
      

      { 
        
        
        
        cs MemberName = CSz("P");
                                        DoEditorUi_entity_P(Ui, Window, Element, CSz("cp P"), ThisHash, EDITOR_UI_FUNCTION_INSTANCE_NAMES);




        


      }
      

      { 
        
        
        
        cs MemberName = CSz("EulerAngles");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(v3*, &Element->EulerAngles),
          MemberName,
          ThisHash,
          Params
          , -PI32, PI32 );







        


      }
      

      { 
        
        
        
        cs MemberName = CSz("Scale");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(r32*, &Element->Scale),
          MemberName,
          ThisHash,
          Params
          );







                PushNewRow(Ui);



      }
      

      { 
        
        
        
        cs MemberName = CSz("_CollisionVolumeRadius");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(v3*, &Element->_CollisionVolumeRadius),
          MemberName,
          ThisHash,
          Params
          );







        


      }
      

      { 
        
        
        
        cs MemberName = CSz("Physics");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(physics*, &Element->Physics),
          MemberName,
          ThisHash,
          Params
          );







        


      }
      

      { 
        
        
        
        cs MemberName = CSz("AssetId");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(asset_id*, &Element->AssetId),
          MemberName,
          ThisHash,
          Params
          );







        


      }
      

      { 
        
        
        
        cs MemberName = CSz("ModelIndex");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(u64*, &Element->ModelIndex),
          MemberName,
          ThisHash,
          Params
          );







                PushNewRow(Ui);



      }
      

      { 
        
        
        
        cs MemberName = CSz("LastResolvedCollision");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(collision_event*, &Element->LastResolvedCollision),
          MemberName,
          ThisHash,
          Params
          );







        


      }
      

      { 
        
        
        
        cs MemberName = CSz("LastResolvedPosInfo");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(entity_position_info*, &Element->LastResolvedPosInfo),
          MemberName,
          ThisHash,
          Params
          );







        


      }
      

      { 
        
        
        
        cs MemberName = CSz("Emitter");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(particle_system*, Element->Emitter),
          MemberName,
          ThisHash,
          Params
          );







        


      }
      

      { 
        
        
        
        cs MemberName = CSz("State");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(entity_state*, &Element->State),
          MemberName,
          ThisHash,
          Params
          );







        


      }
      

      { 
        
        
        
        cs MemberName = CSz("Behavior");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(entity_behavior_flags*, &Element->Behavior),
          MemberName,
          ThisHash,
          Params
          );







        


      }
      

      { 
        
        
        
        cs MemberName = CSz("Carrying");
                                                                DoEditorUi(Ui,
          Window,
          // Cast to remove const/volatile keywords if they're there
          Cast(entity_id*, &Element->Carrying),
          MemberName,
          ThisHash,
          Params
          );







        


      }
      

      { 
        
        
        
        cs MemberName = CSz("UserType");
                                          if (EntityUserDataEditorUi) {EntityUserDataEditorUi(Ui, Window, &Element->UserType, &Element->UserData, Name, ThisHash, EDITOR_UI_FUNCTION_INSTANCE_NAMES);}
                 else                        {DoEditorUi(Ui, Window, &Element->UserType, Name, ThisHash, EDITOR_UI_FUNCTION_INSTANCE_NAMES); }
    ;




                PushNewRow(Ui);



      }
      

      { 
        
        
        
        cs MemberName = CSz("UserData");
                                          if (EntityUserDataEditorUi) { /* User took control, skip this because it's intended */ }
                 else                        {DoEditorUi(Ui, Window, &Element->UserData, Name, ThisHash, EDITOR_UI_FUNCTION_INSTANCE_NAMES); }
    ;




                PushNewRow(Ui);



      }

      if (DidToggle) { CLOSE_INDENT_FOR_TOGGLEABLE_REGION(); }
      if (Name.Count) { PushTableEnd(Ui); }
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

