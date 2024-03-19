
struct noise_value_to_material_index
{
  f32 NoiseValue;
  u16 MaterialIndex;
};

link_internal u16
MapNoiseValueToMaterial(f32 Value)
{
  noise_value_to_material_index Points[] =
  {
    {1.0f,  SNOW },
    {0.5f,  STONE },
    {0.35f, GRASS_GREEN },
    {0.3f,  GRASS_GREEN },
    {0.2f,  GRASS_GREEN },
    {0.0f,  GRASS_GREEN },
  };


  b32 Hit = False;;
  r32 Result = Value;
  s32 PointCount = s32(ArrayCount(Points));
  RangeIterator(PointIndex, PointCount)
  {
    noise_value_to_material_index *P = Points + PointIndex;

    // This is sort of a hack to fix the degenerate case when the value is 0 or 1.
    // TODO(Jesse): Make this branchless
    if (Value == P->NoiseValue) return P->MaterialIndex;

    if (Value > P->NoiseValue)
    {
      return P->MaterialIndex;
    }
  }
  return 0;
}

link_internal f32
MapNoiseValueToFinal(f32 Value)
{
  /* Assert(Value <= 1.f); */
  /* Assert(Value >= 0.f); */

  // NOTE(Jesse): Descending order so we can scan from the front and find the interval we care about
  /* v2 Points[] = */
  /* { */
  /*   {{1.0f, 1.0f }}, */
  /*   {{0.7f, 0.85f }}, */
  /*   {{0.6f, 0.5f }}, */
  /*   {{0.3f, 0.4f }}, */
  /*   {{0.2f, 0.1f }}, */
  /*   {{0.0f, 0.0f }}, */
  /* }; */

  /* v2 Points[] = */
  /* { */
  /*   {{1.0f, 1.0f }}, */
  /*   {{0.5f, 0.7f }}, */
  /*   {{0.35f, 0.43f }}, */
  /*   {{0.3f, 0.4f }}, */
  /*   {{0.2f, 0.1f }}, */
  /*   {{0.0f, 0.0f }}, */
  /* }; */

  // Pretty nice
/*   v2 Points[] = */
/*   { */
/*     {{1.0f, 1.0f }}, */
/*     {{0.7f, 0.7f }}, */
/*     {{0.65f, 0.43f }}, */
/*     {{0.6f, 0.4f }}, */
/*     {{0.5f, 0.1f }}, */
/*     {{0.0f, 0.0f }}, */
/*   }; */

  // Pillar-y
  v2 Points[] =
  {
    {{1.0f, 0.5f }},
    {{0.6f, 0.3f }},
    {{0.1f, 0.2f }},
    {{0.05f, 0.45f }},
    {{0.0f , 0.5f }},
  };

/*   v2 Points[] = */
/*   { */
/*     {{1.0f, 0.0f }}, */
/*     {{0.9f, 1.0f }}, */
/*     {{0.6f, 0.3f }}, */
/*     {{0.1f, 0.1f }}, */
/*     {{0.05f, 0.45f }}, */
/*     {{0.0f , 0.5f }}, */
/*   }; */



  b32 Hit = False;;
  r32 Result = Value;
  s32 PointCount = s32(ArrayCount(Points));
  RangeIterator(PointIndex, PointCount)
  {
    v2 *P = Points + PointIndex;

    // This is sort of a hack to fix the degenerate case when the value is 0 or 1.
    // TODO(Jesse): Make this branchless
    if (Value == P->x) return P->y;

    if (Value > P->x)
    {
      Hit = True;
      /* Assert(PointIndex > 0); */

      v2 *PNext = Points + PointIndex - 1;

      r32 Range = PNext->x - P->x;
      /* Assert(Range > 0.f); */

      r32 t = (Value-P->x) / Range;
      /* Result = Lerp(t, P->y, PNext->y); */
      Result = CosineInterpolate(t, P->y, PNext->y);
      /* Assert(Result >= 0.f); */
      /* Assert(Result <= 1.f); */

      break;
    }
  }

  /* Assert(Hit); */
  /* Assert(Result <= 1.f); */
  /* Assert(Result >= 0.f); */

  return Result;
}


link_internal void
ComputeNormalsForChunkFromNoiseValues( r32 ChunkWorldZ, r32 *NoiseValues, v3i NoiseDim, v3 *Normals, v3i NormalsDim);


/* link_internal u32 */
/* Terrain_Voronoi2D( world_chunk *, v3i, v3i, v3i, u16, s32, s32, s64, v3i, void *); */

link_internal u32
Terrain_Voronoi3D( world_chunk *, v3i, void *, void * );
