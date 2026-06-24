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


link_internal hotkey_chord *
ResolveNameToChordPointer(cs Name, hotkey_settings *Hotkeys)
{
  hotkey_chord *Result = 0;

  poof(
    func (hotkey_settings Settings) @code_fragment
    {
      Settings.map_members(Member)
      {
        Member.is_type(hotkey_chord)?
        {
          if (StringsMatch(CSz("Member.name"), Name))
          {
            Assert(Result == 0);
            Result = &Hotkeys->Member.name;
          }
        }
      }
    }
  )
#include <generated/anonymous_c6TERWvE.h>

  return Result;
}

link_internal input_event **
ResolveNameToHotkeySlot(cs Name, hotkey_settings *Hotkeys)
{
  input_event **Result = 0;

  poof(
    func (hotkey_settings Settings) @code_fragment
    {
      Settings.map_members(Member)
      {
        Member.is_type(input_event)?
        {
          if (StringsMatch(CSz("Member.name"), Name))
          {
            Assert(Result == 0);
            Result = &Hotkeys->Member.name;
          }
        }
      }
    }
  )
#include <generated/anonymous_6ENTWvLh.h>

  return Result;
}

link_internal input_event *
ResolveNameToInputPointer(cs Name, input *Input)
{
  input_event *Result = 0;

  poof(
    func (input I) @code_fragment
    {
      I.map_members(InputMember)
      {
        InputMember.is_type(input_event)?
        {
          if (StringsMatch(CSz("InputMember.name"), Name))
          {
            Assert(Result == 0);
            Result = &Input->InputMember.name;
          }
        }
      }
    }
  )
#include <generated/anonymous_HTSkPniD.h>

  return Result;
}

link_internal b32
ParseEngineHotkeys(cs SettingsFile, hotkey_settings *Hotkeys, input *Input, heap_allocator *Heap, memory_arena *Memory = 0)
{
  b32 Result = True;

  if (Memory == 0) { Memory = GetTranArena(); }
  parser *Parser = ParserForFile(0, SettingsFile, TokenCursorSource_RootFile, Memory);

  while (TokensRemain(Parser))
  {
    if (OptionalToken(Parser, CToken(CSz("hotkey"))))
    {
      c_token *TSettingName = RequireTokenPointer(Parser, CTokenType_Identifier);
      RequireToken(Parser, CTokenType_Equals);
      c_token *TSettingValue = RequireTokenPointer(Parser, CTokenType_Identifier);

      input_event **HotkeySlot = ResolveNameToHotkeySlot(TSettingName->Value, Hotkeys);
      input_event  *HotkeyBinding = ResolveNameToInputPointer(TSettingValue->Value, Input);

      if ( StringsMatch(TSettingName->Value, CSz("Zoom")) &&
           StringsMatch(TSettingValue->Value, CSz("Scrollwheel")) )
      {
        Assert(HotkeyBinding == 0);
        HotkeyBinding = &NullInputEvent;
        Hotkeys->ZoomType = ZoomType_Scrollwheel;
      }

      if (HotkeySlot && HotkeyBinding)
      {
        if (*HotkeySlot == 0)
        {
          *HotkeySlot = HotkeyBinding;
        }
        else
        {
          SoftError("Hotkey (%S) already bound! Not rebinding.", TSettingValue->Value);
          Result = False;
        }
      }
      else
      {
        if (HotkeySlot == 0)
        {
          SoftError("(%S) is an invalid hotkey name", TSettingName->Value);
          Result = False;
        }

        if (HotkeyBinding == 0)
        {
          SoftError("(%S) is an invalid hotkey value", TSettingValue->Value);
          Result = False;
        }
      }
    }
    else if (OptionalToken(Parser, CToken(CSz("chord"))))
    {
      c_token *TSettingName = RequireTokenPointer(Parser, CTokenType_Identifier);
      RequireToken(Parser, CTokenType_Equals);

      hotkey_chord *Chord = ResolveNameToChordPointer(TSettingName->Value, Hotkeys);
      if (Chord)
      {
        Chord->EventCount = CountTokensBeforeNext(Parser, CTokenType_Identifier, CTokenType_Newline);
        Chord->Events = Cast(input_event**, HeapAllocate(Heap, sizeof(input_event*)*Chord->EventCount));

        RangeIterator_t(u32, ValueIndex, Chord->EventCount)
        {
          c_token *TSettingValue = PopTokenPointer(Parser);
          if (TSettingValue->Type == CTokenType_Identifier)
          {
            input_event *Hotkey = ResolveNameToInputPointer(TSettingValue->Value, Input);
            if (Hotkey)
            {
              Chord->Events[ValueIndex] = Hotkey;
            }
            else
            {
              SoftError("(%S) is an invalid hotkey value", TSettingValue->Value);
              Chord->Events[ValueIndex] = &NullInputEvent;
              Result = False;
            }
          }
          else
          {
            SoftError("(%S) is an invalid hotkey value", TSettingValue->Value);
            Chord->Events[ValueIndex] = &NullInputEvent;
            Result = False;
          }
        }
      }
      else
      {
        SoftError("(%S) is an invalid chord name", TSettingName->Value);
        Result = False;
      }

    }
    else
    {
      SoftError("Unknown keyword (%S)", PopToken(Parser).Value);
      Result = False;
    }
  }

  poof(
    func (hotkey_settings tHotkeys) @code_fragment
    {
      tHotkeys.map_members(HotkeyMember)
      {
        HotkeyMember.is_type(input_event)?
        {
          if (Hotkeys->(HotkeyMember.name) == 0)
          {
            SoftError("Hotkey ((HotkeyMember.name)) remains unbound!");
            Hotkeys->(HotkeyMember.name) = &NullInputEvent;
            Result = False;
          }
        }

        HotkeyMember.is_type(hotkey_chord)?
        {
          if (Hotkeys->(HotkeyMember.name).EventCount == 0)
          {
            SoftError("Chord ((HotkeyMember.name)) remains unbound!");
            Result = False;
          }
        }
      }
    }
  )
#include <generated/anonymous_4HBcbhn0.h>

  return Result;
}

