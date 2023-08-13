void
DoLight(game_lights *Lights, v3 Position, v3 Color)
{
  Assert(Lights->Count < MAX_LIGHTS);

  if (Lights->Count < MAX_LIGHTS)
  {
    light *Light = Lights->Lights + Lights->Count++;
    Light->Position = Position;
    Light->Color = Color;
  }

 return;
}

