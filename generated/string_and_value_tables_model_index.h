link_internal counted_string
ToString(model_index Type)
{
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


