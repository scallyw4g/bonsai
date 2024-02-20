// src/engine/settings.h:38:0

link_internal counted_string
ToString(shader_language_setting Type)
{
  counted_string Result = {};
  switch (Type)
  {
    case ShaderLanguageSetting_330core: { Result = CSz("ShaderLanguageSetting_330core"); } break;
    case ShaderLanguageSetting_310es: { Result = CSz("ShaderLanguageSetting_310es"); } break;

    
  }
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


