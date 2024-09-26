// examples/terrain_gen/game_types.h:28:0

link_internal counted_string
ToStringPrefixless(terrain_gen_type Type)
{
  counted_string Result = {};
  switch (Type)
  {
    case TerrainGenType_Flat: { Result = CSz("Flat"); } break;
    case TerrainGenType_Checkerboard: { Result = CSz("Checkerboard"); } break;
    case TerrainGenType_SinCos: { Result = CSz("SinCos"); } break;
    case TerrainGenType_Voronoi: { Result = CSz("Voronoi"); } break;
    case TerrainGenType_Perlin2D: { Result = CSz("Perlin2D"); } break;
    case TerrainGenType_Perlin3D: { Result = CSz("Perlin3D"); } break;
    case TerrainGenType_FBM2D: { Result = CSz("FBM2D"); } break;
    case TerrainGenType_TerracedTerrain: { Result = CSz("TerracedTerrain"); } break;
    case TerrainGenType_GrassyTerracedTerrain: { Result = CSz("GrassyTerracedTerrain"); } break;
    case TerrainGenType_GrassyLargeTerracedTerrain: { Result = CSz("GrassyLargeTerracedTerrain"); } break;
    case TerrainGenType_GrassyTerracedTerrain2: { Result = CSz("GrassyTerracedTerrain2"); } break;
    case TerrainGenType_GrassyTerracedTerrain3: { Result = CSz("GrassyTerracedTerrain3"); } break;
    case TerrainGenType_GrassyTerracedTerrain4: { Result = CSz("GrassyTerracedTerrain4"); } break;
    case TerrainGenType_GrassyIsland: { Result = CSz("GrassyIsland"); } break;
    case TerrainGenType_Hoodoo: { Result = CSz("Hoodoo"); } break;
    case TerrainGenType_Warped: { Result = CSz("Warped"); } break;
    case TerrainGenType_Debug: { Result = CSz("Debug"); } break;

    
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (EnumType.name)", Type); } */
  return Result;
}

link_internal counted_string
ToString(terrain_gen_type Type)
{
  counted_string Result = {};
  switch (Type)
  {
    case TerrainGenType_Flat: { Result = CSz("TerrainGenType_Flat"); } break;
    case TerrainGenType_Checkerboard: { Result = CSz("TerrainGenType_Checkerboard"); } break;
    case TerrainGenType_SinCos: { Result = CSz("TerrainGenType_SinCos"); } break;
    case TerrainGenType_Voronoi: { Result = CSz("TerrainGenType_Voronoi"); } break;
    case TerrainGenType_Perlin2D: { Result = CSz("TerrainGenType_Perlin2D"); } break;
    case TerrainGenType_Perlin3D: { Result = CSz("TerrainGenType_Perlin3D"); } break;
    case TerrainGenType_FBM2D: { Result = CSz("TerrainGenType_FBM2D"); } break;
    case TerrainGenType_TerracedTerrain: { Result = CSz("TerrainGenType_TerracedTerrain"); } break;
    case TerrainGenType_GrassyTerracedTerrain: { Result = CSz("TerrainGenType_GrassyTerracedTerrain"); } break;
    case TerrainGenType_GrassyLargeTerracedTerrain: { Result = CSz("TerrainGenType_GrassyLargeTerracedTerrain"); } break;
    case TerrainGenType_GrassyTerracedTerrain2: { Result = CSz("TerrainGenType_GrassyTerracedTerrain2"); } break;
    case TerrainGenType_GrassyTerracedTerrain3: { Result = CSz("TerrainGenType_GrassyTerracedTerrain3"); } break;
    case TerrainGenType_GrassyTerracedTerrain4: { Result = CSz("TerrainGenType_GrassyTerracedTerrain4"); } break;
    case TerrainGenType_GrassyIsland: { Result = CSz("TerrainGenType_GrassyIsland"); } break;
    case TerrainGenType_Hoodoo: { Result = CSz("TerrainGenType_Hoodoo"); } break;
    case TerrainGenType_Warped: { Result = CSz("TerrainGenType_Warped"); } break;
    case TerrainGenType_Debug: { Result = CSz("TerrainGenType_Debug"); } break;

    
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (EnumType.name)", Type); } */
  return Result;
}

link_internal terrain_gen_type
TerrainGenType(counted_string S)
{
  terrain_gen_type Result = {};

  if (StringsMatch(S, CSz("TerrainGenType_Flat"))) { return TerrainGenType_Flat; }
  if (StringsMatch(S, CSz("TerrainGenType_Checkerboard"))) { return TerrainGenType_Checkerboard; }
  if (StringsMatch(S, CSz("TerrainGenType_SinCos"))) { return TerrainGenType_SinCos; }
  if (StringsMatch(S, CSz("TerrainGenType_Voronoi"))) { return TerrainGenType_Voronoi; }
  if (StringsMatch(S, CSz("TerrainGenType_Perlin2D"))) { return TerrainGenType_Perlin2D; }
  if (StringsMatch(S, CSz("TerrainGenType_Perlin3D"))) { return TerrainGenType_Perlin3D; }
  if (StringsMatch(S, CSz("TerrainGenType_FBM2D"))) { return TerrainGenType_FBM2D; }
  if (StringsMatch(S, CSz("TerrainGenType_TerracedTerrain"))) { return TerrainGenType_TerracedTerrain; }
  if (StringsMatch(S, CSz("TerrainGenType_GrassyTerracedTerrain"))) { return TerrainGenType_GrassyTerracedTerrain; }
  if (StringsMatch(S, CSz("TerrainGenType_GrassyLargeTerracedTerrain"))) { return TerrainGenType_GrassyLargeTerracedTerrain; }
  if (StringsMatch(S, CSz("TerrainGenType_GrassyTerracedTerrain2"))) { return TerrainGenType_GrassyTerracedTerrain2; }
  if (StringsMatch(S, CSz("TerrainGenType_GrassyTerracedTerrain3"))) { return TerrainGenType_GrassyTerracedTerrain3; }
  if (StringsMatch(S, CSz("TerrainGenType_GrassyTerracedTerrain4"))) { return TerrainGenType_GrassyTerracedTerrain4; }
  if (StringsMatch(S, CSz("TerrainGenType_GrassyIsland"))) { return TerrainGenType_GrassyIsland; }
  if (StringsMatch(S, CSz("TerrainGenType_Hoodoo"))) { return TerrainGenType_Hoodoo; }
  if (StringsMatch(S, CSz("TerrainGenType_Warped"))) { return TerrainGenType_Warped; }
  if (StringsMatch(S, CSz("TerrainGenType_Debug"))) { return TerrainGenType_Debug; }

  return Result;
}


