// callsite
// src/engine/settings.h:32:0

// def (string_and_value_tables)
// external/bonsai_stdlib/src/poof_functions.h:2131:0
link_internal b32
IsValid(shader_language_setting Value)
{
  b32 Result = False;
  switch (Value)
  {
        case ShaderLanguageSetting_330core:
    case ShaderLanguageSetting_310es:

    {
      Result = True;
    }
  }
  return Result;
}



link_internal counted_string
ToStringPrefixless(shader_language_setting Type)
{
  cs Result = {};
  if (IsValid(Type))
  {
    switch (Type)
    {
            case ShaderLanguageSetting_330core: { Result = CSz("330core"); } break;
      case ShaderLanguageSetting_310es: { Result = CSz("310es"); } break;


      
    }
  }
  else
  {
    Result = CSz("(CORRUPT ENUM VALUE)");
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (enum_t.name)", Type); } */
  return Result;
}

link_internal counted_string
ToString(shader_language_setting Type)
{
  Assert(IsValid(Type));

  counted_string Result = {};
  switch (Type)
  {
        case ShaderLanguageSetting_330core: { Result = CSz("ShaderLanguageSetting_330core"); } break;
    case ShaderLanguageSetting_310es: { Result = CSz("ShaderLanguageSetting_310es"); } break;


    
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (enum_t.name)", Type); } */
  return Result;
}

link_internal shader_language_setting
ShaderLanguageSetting(counted_string S)
{
  shader_language_setting Result = {};

    if (StringsMatch(S, CSz("ShaderLanguageSetting_330core"))) { return ShaderLanguageSetting_330core; }
  if (StringsMatch(S, CSz("ShaderLanguageSetting_310es"))) { return ShaderLanguageSetting_310es; }


  return Result;
}


