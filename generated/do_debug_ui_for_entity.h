link_internal void
DoDebugUiFor(renderer_2d *Ui, entity *Element)
{
  PushTableStart(Ui);
  DebugValue(Ui, &Element->Model);
  DebugValue(Ui, &Element->CollisionVolumeRadius);
  DebugValue(Ui, &Element->Emitter);
  DebugValue(Ui, &Element->Physics);
  DebugValue(Ui, &Element->P);
  DebugValue(Ui, &Element->Rotation);
  DebugValue(Ui, &Element->State);
  DebugValue(Ui, &Element->Type);
  DebugValue(Ui, &Element->Scale);
  DebugValue(Ui, &Element->RateOfFire);
  DebugValue(Ui, &Element->FireCooldown);
  DebugValue(Ui, &Element->Health);
  DebugValue(Ui, &Element->UserData);
  PushTableEnd(Ui);
}

