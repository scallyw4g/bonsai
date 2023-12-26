link_internal counted_string
ToString(terrain_gen_type Type)
{
  counted_string Result = {};
  switch (Type)
  {
    case TerrainGenType_Flat: { Result = CSz("TerrainGenType_Flat"); } break;
    case TerrainGenType_Checkerboard: { Result = CSz("TerrainGenType_Checkerboard"); } break;
    case TerrainGenType_Perlin2D: { Result = CSz("TerrainGenType_Perlin2D"); } break;
    case TerrainGenType_Perlin3D: { Result = CSz("TerrainGenType_Perlin3D"); } break;
    case TerrainGenType_FBM2D: { Result = CSz("TerrainGenType_FBM2D"); } break;
    case TerrainGenType_TerracedTerrain: { Result = CSz("TerrainGenType_TerracedTerrain"); } break;
    case TerrainGenType_GrassyIsland: { Result = CSz("TerrainGenType_GrassyIsland"); } break;
    case TerrainGenType_Warped: { Result = CSz("TerrainGenType_Warped"); } break;
  }
  return Result;
}

link_internal terrain_gen_type
TerrainGenType(counted_string S)
{
  terrain_gen_type Result = {};

  if (StringsMatch(S, CSz("TerrainGenType_Flat"))) { return TerrainGenType_Flat; }
  if (StringsMatch(S, CSz("TerrainGenType_Checkerboard"))) { return TerrainGenType_Checkerboard; }
  if (StringsMatch(S, CSz("TerrainGenType_Perlin2D"))) { return TerrainGenType_Perlin2D; }
  if (StringsMatch(S, CSz("TerrainGenType_Perlin3D"))) { return TerrainGenType_Perlin3D; }
  if (StringsMatch(S, CSz("TerrainGenType_FBM2D"))) { return TerrainGenType_FBM2D; }
  if (StringsMatch(S, CSz("TerrainGenType_TerracedTerrain"))) { return TerrainGenType_TerracedTerrain; }
  if (StringsMatch(S, CSz("TerrainGenType_GrassyIsland"))) { return TerrainGenType_GrassyIsland; }
  if (StringsMatch(S, CSz("TerrainGenType_Warped"))) { return TerrainGenType_Warped; }

  return Result;
}


