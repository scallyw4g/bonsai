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
            /* SoftError("TODO(Jesse): Ignoring window_starting_size"); */
          } break;

          // TODO(Jesse): Handle invalid settings
          InvalidDefaultCase;
        }

      } break;

      InvalidDefaultCase;
    }

    continue;
  }

  return Settings;
}


link_internal input_event *
ResolveHotkeyNameToInputPointer(cs InputMemberQuery, input *Input)
{
  input_event *Result = 0;

  poof(
    func (input I) @code_fragment
    {
      I.map_members(InputMember)
      {
        InputMember.is_type(input_event)?
        {
          if (StringsMatch(CSz("InputMember.name"), InputMemberQuery))
          {
            Assert(Result == 0);
            Result = &Input->InputMember.name;
          }
        }
      }
    }
  )
#include <generated/anonymous_HTSkPniD.h>

  if (Result == 0)
  {
    SoftError("(%S) not a valid input member name, hotkey not bound.", InputMemberQuery);
  }

  return Result;
}

link_internal b32
ParseEngineHotkeys(cs SettingsFile, hotkey_settings *Hotkeys, input *Input, memory_arena *Memory = 0)
{
  b32 Result = True;

  if (Memory == 0) { Memory = GetTranArena(); }
  parser *Parser = ParserForFile(0, SettingsFile, TokenCursorSource_RootFile, Memory);

  poof(
    func (hotkey_settings tHotkeys) @code_fragment
    {
      tHotkeys.map_members(InputMember)
      {
        InputMember.is_type(hotkey)?
        {
          Hotkeys->(InputMember.name).State = &NullInputEvent;
        }
      }
    }
  )
#include <generated/anonymous_rj3Fqbrr.h>

  while (TokensRemain(Parser))
  {
    RequireToken(Parser, CToken(CSz("hotkey")));
    c_token *TSettingName = RequireTokenPointer(Parser, CTokenType_Identifier);
    RequireToken(Parser, CTokenType_Equals);
    c_token *TSettingValue = RequireTokenPointer(Parser, CTokenType_Identifier);

    b32 BindingSuccess = False;
    poof(
      func (hotkey_settings tHotkeys) @code_fragment
      {
        tHotkeys.map_members(InputMember)
        {
          InputMember.is_type(hotkey)?
          {
            if (StringsMatch(CSz("InputMember.name"), TSettingName->Value))
            {
              Assert(BindingSuccess == False);
              Hotkeys->(InputMember.name).State = ResolveHotkeyNameToInputPointer(TSettingValue->Value, Input);
              BindingSuccess = True;
            }
          }
        }
      }
    )
#include <generated/anonymous_T13QDCor.h>

    if (BindingSuccess == False)
    {
      SoftError("Unknown Hotkey (%S)", TSettingName->Value);
      Result = False;
    }
  }

  poof(
    func (hotkey_settings tHotkeys) @code_fragment
    {
      tHotkeys.map_members(InputMember)
      {
        InputMember.is_type(hotkey)?
        {
          if (Hotkeys->(InputMember.name).State == &NullInputEvent)
          {
            SoftError("Hotkey ((InputMember.name)) remains unbound!");
            Result = False;
          }
        }
      }
    }
  )
#include <generated/anonymous_4HBcbhn0.h>

  return Result;
}

