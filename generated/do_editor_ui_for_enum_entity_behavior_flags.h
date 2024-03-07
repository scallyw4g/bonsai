// src/engine/editor.cpp:101:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, entity_behavior_flags *Element, cs Name, ui_render_params *Params)
{
  if (Name) { PushColumn(Ui, CS(Name), &DefaultUiRenderParams_Column); }

  cs ElementName = ToString(*Element);
  if (ToggleButton(Ui, ElementName, ElementName, UiId(Window, "enum value.type value.name", Element), Params))
  {
    PushNewRow(Ui);
    PushColumn(Ui, CSz("|")); // Skip the first Name column
    if (Button(Ui, CSz("None"), UiId(Window, "enum EntityBehaviorFlags_None", Element), Params))
    {
      if (EntityBehaviorFlags_None == entity_behavior_flags(0))
      {
        *Element = entity_behavior_flags(0);
      }
      else
      {
        if ((*Element & EntityBehaviorFlags_None) == EntityBehaviorFlags_None)
        {
          *Element = entity_behavior_flags(*Element&~EntityBehaviorFlags_None);
        }
        else
        {
          *Element = entity_behavior_flags(*Element|EntityBehaviorFlags_None);
        }
      }

    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("|")); // Skip the first Name column
    if (Button(Ui, CSz("Gravity"), UiId(Window, "enum EntityBehaviorFlags_Gravity", Element), Params))
    {
      if (EntityBehaviorFlags_Gravity == entity_behavior_flags(0))
      {
        *Element = entity_behavior_flags(0);
      }
      else
      {
        if ((*Element & EntityBehaviorFlags_Gravity) == EntityBehaviorFlags_Gravity)
        {
          *Element = entity_behavior_flags(*Element&~EntityBehaviorFlags_Gravity);
        }
        else
        {
          *Element = entity_behavior_flags(*Element|EntityBehaviorFlags_Gravity);
        }
      }

    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("|")); // Skip the first Name column
    if (Button(Ui, CSz("WorldCollision"), UiId(Window, "enum EntityBehaviorFlags_WorldCollision", Element), Params))
    {
      if (EntityBehaviorFlags_WorldCollision == entity_behavior_flags(0))
      {
        *Element = entity_behavior_flags(0);
      }
      else
      {
        if ((*Element & EntityBehaviorFlags_WorldCollision) == EntityBehaviorFlags_WorldCollision)
        {
          *Element = entity_behavior_flags(*Element&~EntityBehaviorFlags_WorldCollision);
        }
        else
        {
          *Element = entity_behavior_flags(*Element|EntityBehaviorFlags_WorldCollision);
        }
      }

    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("|")); // Skip the first Name column
    if (Button(Ui, CSz("EntityCollision"), UiId(Window, "enum EntityBehaviorFlags_EntityCollision", Element), Params))
    {
      if (EntityBehaviorFlags_EntityCollision == entity_behavior_flags(0))
      {
        *Element = entity_behavior_flags(0);
      }
      else
      {
        if ((*Element & EntityBehaviorFlags_EntityCollision) == EntityBehaviorFlags_EntityCollision)
        {
          *Element = entity_behavior_flags(*Element&~EntityBehaviorFlags_EntityCollision);
        }
        else
        {
          *Element = entity_behavior_flags(*Element|EntityBehaviorFlags_EntityCollision);
        }
      }

    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("|")); // Skip the first Name column
    if (Button(Ui, CSz("UnspawnOnParticleSystemTerminate"), UiId(Window, "enum EntityBehaviorFlags_UnspawnOnParticleSystemTerminate", Element), Params))
    {
      if (EntityBehaviorFlags_UnspawnOnParticleSystemTerminate == entity_behavior_flags(0))
      {
        *Element = entity_behavior_flags(0);
      }
      else
      {
        if ((*Element & EntityBehaviorFlags_UnspawnOnParticleSystemTerminate) == EntityBehaviorFlags_UnspawnOnParticleSystemTerminate)
        {
          *Element = entity_behavior_flags(*Element&~EntityBehaviorFlags_UnspawnOnParticleSystemTerminate);
        }
        else
        {
          *Element = entity_behavior_flags(*Element|EntityBehaviorFlags_UnspawnOnParticleSystemTerminate);
        }
      }

    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("|")); // Skip the first Name column
    if (Button(Ui, CSz("FitCollisionVolumeToModel"), UiId(Window, "enum EntityBehaviorFlags_FitCollisionVolumeToModel", Element), Params))
    {
      if (EntityBehaviorFlags_FitCollisionVolumeToModel == entity_behavior_flags(0))
      {
        *Element = entity_behavior_flags(0);
      }
      else
      {
        if ((*Element & EntityBehaviorFlags_FitCollisionVolumeToModel) == EntityBehaviorFlags_FitCollisionVolumeToModel)
        {
          *Element = entity_behavior_flags(*Element&~EntityBehaviorFlags_FitCollisionVolumeToModel);
        }
        else
        {
          *Element = entity_behavior_flags(*Element|EntityBehaviorFlags_FitCollisionVolumeToModel);
        }
      }

    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("|")); // Skip the first Name column
    if (Button(Ui, CSz("DefatulCameraGhostBehavior"), UiId(Window, "enum EntityBehaviorFlags_DefatulCameraGhostBehavior", Element), Params))
    {
      if (EntityBehaviorFlags_DefatulCameraGhostBehavior == entity_behavior_flags(0))
      {
        *Element = entity_behavior_flags(0);
      }
      else
      {
        if ((*Element & EntityBehaviorFlags_DefatulCameraGhostBehavior) == EntityBehaviorFlags_DefatulCameraGhostBehavior)
        {
          *Element = entity_behavior_flags(*Element&~EntityBehaviorFlags_DefatulCameraGhostBehavior);
        }
        else
        {
          *Element = entity_behavior_flags(*Element|EntityBehaviorFlags_DefatulCameraGhostBehavior);
        }
      }

    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("|")); // Skip the first Name column
    if (Button(Ui, CSz("WorldCenter"), UiId(Window, "enum EntityBehaviorFlags_WorldCenter", Element), Params))
    {
      if (EntityBehaviorFlags_WorldCenter == entity_behavior_flags(0))
      {
        *Element = entity_behavior_flags(0);
      }
      else
      {
        if ((*Element & EntityBehaviorFlags_WorldCenter) == EntityBehaviorFlags_WorldCenter)
        {
          *Element = entity_behavior_flags(*Element&~EntityBehaviorFlags_WorldCenter);
        }
        else
        {
          *Element = entity_behavior_flags(*Element|EntityBehaviorFlags_WorldCenter);
        }
      }

    }
    PushNewRow(Ui);
    PushColumn(Ui, CSz("|")); // Skip the first Name column
    if (Button(Ui, CSz("Default"), UiId(Window, "enum EntityBehaviorFlags_Default", Element), Params))
    {
      if (EntityBehaviorFlags_Default == entity_behavior_flags(0))
      {
        *Element = entity_behavior_flags(0);
      }
      else
      {
        if ((*Element & EntityBehaviorFlags_Default) == EntityBehaviorFlags_Default)
        {
          *Element = entity_behavior_flags(*Element&~EntityBehaviorFlags_Default);
        }
        else
        {
          *Element = entity_behavior_flags(*Element|EntityBehaviorFlags_Default);
        }
      }

    }
    PushNewRow(Ui);
  }
  else
  {
    PushNewRow(Ui);
  }
}

