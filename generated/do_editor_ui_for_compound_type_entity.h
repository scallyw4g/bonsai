// callsite
// src/engine/editor.cpp:554:0

// def (do_editor_ui_for_compound_type)
// src/engine/editor.h:495:0
struct entity;
link_internal void DoEditorUi(renderer_2d *Ui, window_layout *Window, entity *Element, cs Name, u32 ParentHash, ui_render_params *Params = &DefaultUiRenderParams_Button, primitive_value_changed_record_block_array *ChangeRecords = 0)


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
        {
          
          { 
            
            
            
            cs MemberName = CSz("Id");

                                                                                                // Regular struct member
                        auto Member = Cast(entity_id*, &Element->Id);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params,
              ChangeRecords
              );








            


          }
        }
      }
      {
        {
          
          { 
            
            
            
            cs MemberName = CSz("");

                                                                                                // Regular struct member
                        auto Member = Cast(cp*, &Element->P);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params,
              ChangeRecords
              );








            


          }
        }
      }
      {
        {
          
          { 
            
            
            
            cs MemberName = CSz("EulerAngles");

                                                                                                auto Member = Cast(v3*, &Element->EulerAngles);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params,
              ChangeRecords
              , -PI32, PI32 );







            


          }
        }
      }
      {
        {
          
          { 
            
            
            
            cs MemberName = CSz("Scale");

                                                                                                // Regular struct member
                        auto Member = Cast(r32*, &Element->Scale);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params,
              ChangeRecords
              );








                        PushNewRow(Ui);



          }
        }
      }
      {
        {
          
          { 
            
            
            
            cs MemberName = CSz("_CollisionVolumeRadius");

                                                                                                auto Member = Cast(v3*, &Element->_CollisionVolumeRadius);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params,
              ChangeRecords
              );







            


          }
        }
      }
      {
        {
          
          { 
            
            
            
            cs MemberName = CSz("Physics");

                                                                                                // Regular struct member
                        auto Member = Cast(physics*, &Element->Physics);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params,
              ChangeRecords
              );








            


          }
        }
      }
      {
        {
          
          { 
            
            
            
            cs MemberName = CSz("AssetId");

                                                                                                // Regular struct member
                        auto Member = Cast(asset_id*, &Element->AssetId);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params,
              ChangeRecords
              );








            


          }
        }
      }
      {
        {
          
          { 
            
            
            
            cs MemberName = CSz("ModelIndex");

                                                                                                // Regular struct member
                        auto Member = Cast(u64*, &Element->ModelIndex);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params,
              ChangeRecords
              );








                        PushNewRow(Ui);



          }
        }
      }
      {
        {
          
          { 
            
            
            
            cs MemberName = CSz("LastResolvedCollision");

                                                                                                // Regular struct member
                        auto Member = Cast(collision_event*, &Element->LastResolvedCollision);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params,
              ChangeRecords
              );








            


          }
        }
      }
      {
        {
          
          { 
            
            
            
            cs MemberName = CSz("LastResolvedPosInfo");

                                                                                                // Regular struct member
                        auto Member = Cast(entity_position_info*, &Element->LastResolvedPosInfo);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params,
              ChangeRecords
              );








            


          }
        }
      }
      {
        {
          
          { 
            
            
            
            cs MemberName = CSz("Emitter");

                                                                                                // Regular struct member
                        auto Member = Cast(particle_system*, Element->Emitter);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params,
              ChangeRecords
              );








            


          }
        }
      }
      {
        {
          
          { 
            
            
            
            cs MemberName = CSz("State");

                                                                                                // Regular struct member
                        auto Member = Cast(entity_state*, &Element->State);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params,
              ChangeRecords
              );








            


          }
        }
      }
      {
        {
          
          { 
            
            
            
            cs MemberName = CSz("Behavior");

                                                                                                // Regular struct member
                        auto Member = Cast(entity_behavior_flags*, &Element->Behavior);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params,
              ChangeRecords
              );








            


          }
        }
      }
      {
        {
          
          { 
            
            
            
            cs MemberName = CSz("Carrying");

                                                                                                // Regular struct member
                        auto Member = Cast(entity_id*, &Element->Carrying);
            DoEditorUi(Ui,
              Window,
              Member,
              MemberName,
              ThisHash,
              Params,
              ChangeRecords
              );








            


          }
        }
      }
      {
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

