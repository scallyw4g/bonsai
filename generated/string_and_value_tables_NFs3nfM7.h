// callsite
// src/engine/editor.h:1851:0

// def (string_and_value_tables)
// external/bonsai_stdlib/src/poof_functions.h:2131:0
link_internal b32
IsValid(prefab_spawn_callback Value)
{
  b32 Result = False;
  switch (Value)
  {
        case PrefabSpawnCallback_None:
    case PrefabSpawnCallback_DefaultPrefabSpawnCallback:

    {
      Result = True;
    }
  }
  return Result;
}



link_internal counted_string
ToStringPrefixless(prefab_spawn_callback Type)
{
  cs Result = {};
  if (IsValid(Type))
  {
    switch (Type)
    {
            case PrefabSpawnCallback_None: { Result = CSz("None"); } break;
      case PrefabSpawnCallback_DefaultPrefabSpawnCallback: { Result = CSz("DefaultPrefabSpawnCallback"); } break;


      
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
ToString(prefab_spawn_callback Type)
{
  Assert(IsValid(Type));

  counted_string Result = {};
  switch (Type)
  {
        case PrefabSpawnCallback_None: { Result = CSz("PrefabSpawnCallback_None"); } break;
    case PrefabSpawnCallback_DefaultPrefabSpawnCallback: { Result = CSz("PrefabSpawnCallback_DefaultPrefabSpawnCallback"); } break;


    
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (enum_t.name)", Type); } */
  return Result;
}

link_internal prefab_spawn_callback
PrefabSpawnCallback(counted_string S)
{
  prefab_spawn_callback Result = {};

    if (StringsMatch(S, CSz("PrefabSpawnCallback_None"))) { return PrefabSpawnCallback_None; }
  if (StringsMatch(S, CSz("PrefabSpawnCallback_DefaultPrefabSpawnCallback"))) { return PrefabSpawnCallback_DefaultPrefabSpawnCallback; }


  return Result;
}


