// callsite
// src/engine/editor.cpp:511:0

// def (do_editor_ui_for_compound_type)
// src/engine/editor.h:486:0
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
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Id");

                                                                                                auto Member = Cast(entity_id*, &Element->Id);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("");

                                                                                                auto Member = Cast(cp*, &Element->P);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("EulerAngles");

                                                                                                auto Member = Cast(v3*, &Element->EulerAngles);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              , -PI32, PI32 );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Scale");

                                                                                                auto Member = Cast(r32*, &Element->Scale);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("_CollisionVolumeRadius");

                                                                                                auto Member = Cast(v3*, &Element->_CollisionVolumeRadius);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Physics");

                                                                                                auto Member = Cast(physics*, &Element->Physics);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("AssetId");

                                                                                                auto Member = Cast(asset_id*, &Element->AssetId);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("ModelIndex");

                                                                                                auto Member = Cast(u64*, &Element->ModelIndex);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("LastResolvedCollision");

                                                                                                auto Member = Cast(collision_event*, &Element->LastResolvedCollision);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("LastResolvedPosInfo");

                                                                                                auto Member = Cast(entity_position_info*, &Element->LastResolvedPosInfo);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Emitter");

                                                                                                auto Member = Cast(particle_system*, Element->Emitter);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("State");

                                                                                                auto Member = Cast(entity_state*, &Element->State);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Behavior");

                                                                                                auto Member = Cast(entity_behavior_flags*, &Element->Behavior);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("Carrying");

                                                                                                auto Member = Cast(entity_id*, &Element->Carrying);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params
              );







            


          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("UserType");

                                                              if (EntityUserDataEditorUi) {EntityUserDataEditorUi(Ui, Window, &Element->UserType, &Element->UserData, Name, ThisHash, EDITOR_UI_FUNCTION_INSTANCE_NAMES);}
                 else                        {DoEditorUi(Ui, Window, &Element->UserType, Name, ThisHash, EDITOR_UI_FUNCTION_INSTANCE_NAMES); }
    ;




                        PushNewRow(Ui);



          }
        }
      }
      {
        /* member.has_tag(ui_null_behavior)? */
        /* { */
        /*   auto Member = Cast((member.type)*, member.is_pointer?{}{&}Element->(member.name)); */
        /*   if (Member == 0) { member.tag_value(ui_null_behavior); } else */
        /* }{} */

        {
          
          { 
            
            
            
            cs MemberName = CSz("UserData");

                                                              if (EntityUserDataEditorUi) { /* User took control, skip this because it's intended */ }
                 else                        {DoEditorUi(Ui, Window, &Element->UserData, Name, ThisHash, EDITOR_UI_FUNCTION_INSTANCE_NAMES); }
    ;




                        PushNewRow(Ui);



          }
        }
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

