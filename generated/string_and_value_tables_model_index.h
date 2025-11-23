// callsite
// src/engine/model.h:55:0

// def (string_and_value_tables)
// external/bonsai_stdlib/src/poof_functions.h:2131:0
link_internal b32
IsValid(model_index Value)
{
  b32 Result = False;
  switch (Value)
  {
        case ModelIndex_None:
    case ModelIndex_Enemy_Skeleton_Axe:
    case ModelIndex_Enemy_Skeleton_Sword:
    case ModelIndex_Enemy_Skeleton_Lasher:
    case ModelIndex_Enemy_Skeleton_Archer:
    case ModelIndex_Enemy_Skeleton_Spear:
    case ModelIndex_Enemy_Skeleton_AxeArmor:
    case ModelIndex_Enemy_Skeleton_Hounds:
    case ModelIndex_Enemy_Skeleton_Horserider:
    case ModelIndex_Enemy_Skeleton_Horsebanner:
    case ModelIndex_Enemy_Skeleton_Shaman:
    case ModelIndex_Enemy_Skeleton_Champion:
    case ModelIndex_Enemy_Skeleton_ChampionChampion:
    case ModelIndex_Enemy_Skeleton_Concubiner:
    case ModelIndex_Enemy_Skeleton_King:
    case ModelIndex_Player_jp:
    case ModelIndex_Player_bow:
    case ModelIndex_Player_cat:
    case ModelIndex_Player_fox:
    case ModelIndex_Player_gumi:
    case ModelIndex_Player_knight:
    case ModelIndex_Player_man:
    case ModelIndex_Player_mom:
    case ModelIndex_Player_old:
    case ModelIndex_Player_poem:
    case ModelIndex_Player_rain:
    case ModelIndex_Player_sasami:
    case ModelIndex_Player_sol:
    case ModelIndex_Player_sword:
    case ModelIndex_Player_tale:
    case ModelIndex_Player_tama:
    case ModelIndex_Player_tsurugi:
    case ModelIndex_Loot:
    case ModelIndex_Projectile:
    case ModelIndex_Proton:
    case ModelIndex_Bitty0:
    case ModelIndex_Bitty1:
    case ModelIndex_Level:
    case ModelIndex_Count:

    {
      Result = True;
    }
  }
  return Result;
}



link_internal counted_string
ToStringPrefixless(model_index Type)
{
  cs Result = {};
  if (IsValid(Type))
  {
    switch (Type)
    {
            case ModelIndex_None: { Result = CSz("None"); } break;
      case ModelIndex_Enemy_Skeleton_Axe: { Result = CSz("Axe"); } break;
      case ModelIndex_Enemy_Skeleton_Sword: { Result = CSz("Sword"); } break;
      case ModelIndex_Enemy_Skeleton_Lasher: { Result = CSz("Lasher"); } break;
      case ModelIndex_Enemy_Skeleton_Archer: { Result = CSz("Archer"); } break;
      case ModelIndex_Enemy_Skeleton_Spear: { Result = CSz("Spear"); } break;
      case ModelIndex_Enemy_Skeleton_AxeArmor: { Result = CSz("AxeArmor"); } break;
      case ModelIndex_Enemy_Skeleton_Hounds: { Result = CSz("Hounds"); } break;
      case ModelIndex_Enemy_Skeleton_Horserider: { Result = CSz("Horserider"); } break;
      case ModelIndex_Enemy_Skeleton_Horsebanner: { Result = CSz("Horsebanner"); } break;
      case ModelIndex_Enemy_Skeleton_Shaman: { Result = CSz("Shaman"); } break;
      case ModelIndex_Enemy_Skeleton_Champion: { Result = CSz("Champion"); } break;
      case ModelIndex_Enemy_Skeleton_ChampionChampion: { Result = CSz("ChampionChampion"); } break;
      case ModelIndex_Enemy_Skeleton_Concubiner: { Result = CSz("Concubiner"); } break;
      case ModelIndex_Enemy_Skeleton_King: { Result = CSz("King"); } break;
      case ModelIndex_Player_jp: { Result = CSz("jp"); } break;
      case ModelIndex_Player_bow: { Result = CSz("bow"); } break;
      case ModelIndex_Player_cat: { Result = CSz("cat"); } break;
      case ModelIndex_Player_fox: { Result = CSz("fox"); } break;
      case ModelIndex_Player_gumi: { Result = CSz("gumi"); } break;
      case ModelIndex_Player_knight: { Result = CSz("knight"); } break;
      case ModelIndex_Player_man: { Result = CSz("man"); } break;
      case ModelIndex_Player_mom: { Result = CSz("mom"); } break;
      case ModelIndex_Player_old: { Result = CSz("old"); } break;
      case ModelIndex_Player_poem: { Result = CSz("poem"); } break;
      case ModelIndex_Player_rain: { Result = CSz("rain"); } break;
      case ModelIndex_Player_sasami: { Result = CSz("sasami"); } break;
      case ModelIndex_Player_sol: { Result = CSz("sol"); } break;
      case ModelIndex_Player_sword: { Result = CSz("sword"); } break;
      case ModelIndex_Player_tale: { Result = CSz("tale"); } break;
      case ModelIndex_Player_tama: { Result = CSz("tama"); } break;
      case ModelIndex_Player_tsurugi: { Result = CSz("tsurugi"); } break;
      case ModelIndex_Loot: { Result = CSz("Loot"); } break;
      case ModelIndex_Projectile: { Result = CSz("Projectile"); } break;
      case ModelIndex_Proton: { Result = CSz("Proton"); } break;
      case ModelIndex_Bitty0: { Result = CSz("Bitty0"); } break;
      case ModelIndex_Bitty1: { Result = CSz("Bitty1"); } break;
      case ModelIndex_Level: { Result = CSz("Level"); } break;
      case ModelIndex_Count: { Result = CSz("Count"); } break;


      
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
ToString(model_index Type)
{
  Assert(IsValid(Type));

  counted_string Result = {};
  switch (Type)
  {
        case ModelIndex_None: { Result = CSz("ModelIndex_None"); } break;
    case ModelIndex_Enemy_Skeleton_Axe: { Result = CSz("ModelIndex_Enemy_Skeleton_Axe"); } break;
    case ModelIndex_Enemy_Skeleton_Sword: { Result = CSz("ModelIndex_Enemy_Skeleton_Sword"); } break;
    case ModelIndex_Enemy_Skeleton_Lasher: { Result = CSz("ModelIndex_Enemy_Skeleton_Lasher"); } break;
    case ModelIndex_Enemy_Skeleton_Archer: { Result = CSz("ModelIndex_Enemy_Skeleton_Archer"); } break;
    case ModelIndex_Enemy_Skeleton_Spear: { Result = CSz("ModelIndex_Enemy_Skeleton_Spear"); } break;
    case ModelIndex_Enemy_Skeleton_AxeArmor: { Result = CSz("ModelIndex_Enemy_Skeleton_AxeArmor"); } break;
    case ModelIndex_Enemy_Skeleton_Hounds: { Result = CSz("ModelIndex_Enemy_Skeleton_Hounds"); } break;
    case ModelIndex_Enemy_Skeleton_Horserider: { Result = CSz("ModelIndex_Enemy_Skeleton_Horserider"); } break;
    case ModelIndex_Enemy_Skeleton_Horsebanner: { Result = CSz("ModelIndex_Enemy_Skeleton_Horsebanner"); } break;
    case ModelIndex_Enemy_Skeleton_Shaman: { Result = CSz("ModelIndex_Enemy_Skeleton_Shaman"); } break;
    case ModelIndex_Enemy_Skeleton_Champion: { Result = CSz("ModelIndex_Enemy_Skeleton_Champion"); } break;
    case ModelIndex_Enemy_Skeleton_ChampionChampion: { Result = CSz("ModelIndex_Enemy_Skeleton_ChampionChampion"); } break;
    case ModelIndex_Enemy_Skeleton_Concubiner: { Result = CSz("ModelIndex_Enemy_Skeleton_Concubiner"); } break;
    case ModelIndex_Enemy_Skeleton_King: { Result = CSz("ModelIndex_Enemy_Skeleton_King"); } break;
    case ModelIndex_Player_jp: { Result = CSz("ModelIndex_Player_jp"); } break;
    case ModelIndex_Player_bow: { Result = CSz("ModelIndex_Player_bow"); } break;
    case ModelIndex_Player_cat: { Result = CSz("ModelIndex_Player_cat"); } break;
    case ModelIndex_Player_fox: { Result = CSz("ModelIndex_Player_fox"); } break;
    case ModelIndex_Player_gumi: { Result = CSz("ModelIndex_Player_gumi"); } break;
    case ModelIndex_Player_knight: { Result = CSz("ModelIndex_Player_knight"); } break;
    case ModelIndex_Player_man: { Result = CSz("ModelIndex_Player_man"); } break;
    case ModelIndex_Player_mom: { Result = CSz("ModelIndex_Player_mom"); } break;
    case ModelIndex_Player_old: { Result = CSz("ModelIndex_Player_old"); } break;
    case ModelIndex_Player_poem: { Result = CSz("ModelIndex_Player_poem"); } break;
    case ModelIndex_Player_rain: { Result = CSz("ModelIndex_Player_rain"); } break;
    case ModelIndex_Player_sasami: { Result = CSz("ModelIndex_Player_sasami"); } break;
    case ModelIndex_Player_sol: { Result = CSz("ModelIndex_Player_sol"); } break;
    case ModelIndex_Player_sword: { Result = CSz("ModelIndex_Player_sword"); } break;
    case ModelIndex_Player_tale: { Result = CSz("ModelIndex_Player_tale"); } break;
    case ModelIndex_Player_tama: { Result = CSz("ModelIndex_Player_tama"); } break;
    case ModelIndex_Player_tsurugi: { Result = CSz("ModelIndex_Player_tsurugi"); } break;
    case ModelIndex_Loot: { Result = CSz("ModelIndex_Loot"); } break;
    case ModelIndex_Projectile: { Result = CSz("ModelIndex_Projectile"); } break;
    case ModelIndex_Proton: { Result = CSz("ModelIndex_Proton"); } break;
    case ModelIndex_Bitty0: { Result = CSz("ModelIndex_Bitty0"); } break;
    case ModelIndex_Bitty1: { Result = CSz("ModelIndex_Bitty1"); } break;
    case ModelIndex_Level: { Result = CSz("ModelIndex_Level"); } break;
    case ModelIndex_Count: { Result = CSz("ModelIndex_Count"); } break;


    
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (enum_t.name)", Type); } */
  return Result;
}

link_internal model_index
ModelIndex(counted_string S)
{
  model_index Result = {};

    if (StringsMatch(S, CSz("ModelIndex_None"))) { return ModelIndex_None; }
  if (StringsMatch(S, CSz("ModelIndex_Enemy_Skeleton_Axe"))) { return ModelIndex_Enemy_Skeleton_Axe; }
  if (StringsMatch(S, CSz("ModelIndex_Enemy_Skeleton_Sword"))) { return ModelIndex_Enemy_Skeleton_Sword; }
  if (StringsMatch(S, CSz("ModelIndex_Enemy_Skeleton_Lasher"))) { return ModelIndex_Enemy_Skeleton_Lasher; }
  if (StringsMatch(S, CSz("ModelIndex_Enemy_Skeleton_Archer"))) { return ModelIndex_Enemy_Skeleton_Archer; }
  if (StringsMatch(S, CSz("ModelIndex_Enemy_Skeleton_Spear"))) { return ModelIndex_Enemy_Skeleton_Spear; }
  if (StringsMatch(S, CSz("ModelIndex_Enemy_Skeleton_AxeArmor"))) { return ModelIndex_Enemy_Skeleton_AxeArmor; }
  if (StringsMatch(S, CSz("ModelIndex_Enemy_Skeleton_Hounds"))) { return ModelIndex_Enemy_Skeleton_Hounds; }
  if (StringsMatch(S, CSz("ModelIndex_Enemy_Skeleton_Horserider"))) { return ModelIndex_Enemy_Skeleton_Horserider; }
  if (StringsMatch(S, CSz("ModelIndex_Enemy_Skeleton_Horsebanner"))) { return ModelIndex_Enemy_Skeleton_Horsebanner; }
  if (StringsMatch(S, CSz("ModelIndex_Enemy_Skeleton_Shaman"))) { return ModelIndex_Enemy_Skeleton_Shaman; }
  if (StringsMatch(S, CSz("ModelIndex_Enemy_Skeleton_Champion"))) { return ModelIndex_Enemy_Skeleton_Champion; }
  if (StringsMatch(S, CSz("ModelIndex_Enemy_Skeleton_ChampionChampion"))) { return ModelIndex_Enemy_Skeleton_ChampionChampion; }
  if (StringsMatch(S, CSz("ModelIndex_Enemy_Skeleton_Concubiner"))) { return ModelIndex_Enemy_Skeleton_Concubiner; }
  if (StringsMatch(S, CSz("ModelIndex_Enemy_Skeleton_King"))) { return ModelIndex_Enemy_Skeleton_King; }
  if (StringsMatch(S, CSz("ModelIndex_Player_jp"))) { return ModelIndex_Player_jp; }
  if (StringsMatch(S, CSz("ModelIndex_Player_bow"))) { return ModelIndex_Player_bow; }
  if (StringsMatch(S, CSz("ModelIndex_Player_cat"))) { return ModelIndex_Player_cat; }
  if (StringsMatch(S, CSz("ModelIndex_Player_fox"))) { return ModelIndex_Player_fox; }
  if (StringsMatch(S, CSz("ModelIndex_Player_gumi"))) { return ModelIndex_Player_gumi; }
  if (StringsMatch(S, CSz("ModelIndex_Player_knight"))) { return ModelIndex_Player_knight; }
  if (StringsMatch(S, CSz("ModelIndex_Player_man"))) { return ModelIndex_Player_man; }
  if (StringsMatch(S, CSz("ModelIndex_Player_mom"))) { return ModelIndex_Player_mom; }
  if (StringsMatch(S, CSz("ModelIndex_Player_old"))) { return ModelIndex_Player_old; }
  if (StringsMatch(S, CSz("ModelIndex_Player_poem"))) { return ModelIndex_Player_poem; }
  if (StringsMatch(S, CSz("ModelIndex_Player_rain"))) { return ModelIndex_Player_rain; }
  if (StringsMatch(S, CSz("ModelIndex_Player_sasami"))) { return ModelIndex_Player_sasami; }
  if (StringsMatch(S, CSz("ModelIndex_Player_sol"))) { return ModelIndex_Player_sol; }
  if (StringsMatch(S, CSz("ModelIndex_Player_sword"))) { return ModelIndex_Player_sword; }
  if (StringsMatch(S, CSz("ModelIndex_Player_tale"))) { return ModelIndex_Player_tale; }
  if (StringsMatch(S, CSz("ModelIndex_Player_tama"))) { return ModelIndex_Player_tama; }
  if (StringsMatch(S, CSz("ModelIndex_Player_tsurugi"))) { return ModelIndex_Player_tsurugi; }
  if (StringsMatch(S, CSz("ModelIndex_Loot"))) { return ModelIndex_Loot; }
  if (StringsMatch(S, CSz("ModelIndex_Projectile"))) { return ModelIndex_Projectile; }
  if (StringsMatch(S, CSz("ModelIndex_Proton"))) { return ModelIndex_Proton; }
  if (StringsMatch(S, CSz("ModelIndex_Bitty0"))) { return ModelIndex_Bitty0; }
  if (StringsMatch(S, CSz("ModelIndex_Bitty1"))) { return ModelIndex_Bitty1; }
  if (StringsMatch(S, CSz("ModelIndex_Level"))) { return ModelIndex_Level; }
  if (StringsMatch(S, CSz("ModelIndex_Count"))) { return ModelIndex_Count; }


  return Result;
}


