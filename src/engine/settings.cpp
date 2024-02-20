link_internal engine_settings
ParseEngineSettings(cs SettingsFile, memory_arena *Memory = 0)
{
  engine_settings Settings = {};

  if (Memory == 0) { Memory = GetTranArena(); }
  parser *Parser = ParserForFile(0, SettingsFile, TokenCursorSource_RootFile, Memory);

  while (c_token *TSetting = PopTokenPointer(Parser))
  {
    switch (TSetting->Type)
    {
      case CTokenType_Identifier:
      {
        RequireToken(Parser, CTokenType_Equals);

        c_token *TSettingValue = PopTokenPointer(Parser);

        switch (TSetting->Value.Count)
        {
          case 10:
          {
            if (StringsMatch(CSz("resolution"), TSetting->Value)) { Settings.Graphics.Resolution = ResolutionSetting(TSettingValue->Value); }
          } break;

          case 14:
          {
            if (StringsMatch(CSz("shadow_quality"), TSetting->Value)) { Settings.Graphics.ShadowQuality = ShadowQualitySetting(TSettingValue->Value); }
          } break;

          case 15:
          {
            if (StringsMatch(CSz("shader_language"), TSetting->Value)) { Settings.Graphics.ShaderLanguage = ShaderLanguageSetting(TSettingValue->Value);  }
          } break;

          case 16:
          {
            if (StringsMatch(CSz("lighting_quality"), TSetting->Value)) { Settings.Graphics.LightingQuality = LightingQualitySetting(TSettingValue->Value); }
          } break;

          case 20:
          {
            if (StringsMatch(CSz("window_starting_size"), TSetting->Value)) { Settings.Graphics.WindowStartingSize = ResolutionSetting(TSettingValue->Value); }
          } break;

          InvalidDefaultCase;
        }

      } break;

      InvalidDefaultCase;
    }

    continue;
  }

  return Settings;
}
