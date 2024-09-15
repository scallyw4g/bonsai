
#define MIN_TERRAIN_NOISE_WIDTH (8)


#define COMPUTE_NOISE_INPUT(channel_name, offset, chunk) (chunk->DimInChunks.channel_name/2) + ((f32(offset)+channel_name)*chunk->DimInChunks.channel_name) + (chunk->WorldP.channel_name*WorldChunkDim.channel_name) + SrcToDest.channel_name 


poof(
  func terrain_iteration_pattern( type_poof_symbol noise_input_name,
                                  type_poof_symbol noise_value_name,
                                  type_poof_symbol packed_HSV_color_value_name,
                                  type_poof_symbol user_code)
  @code_fragment
  {
    Period = Max(Period, V3(1.f));

    for ( s32 z = 0; z < Dim.z; ++ z)
    {
      s64 WorldZ = s64(COMPUTE_NOISE_INPUT(z, 0, Chunk));
      s64 WorldZBiased = WorldZ - zMin;
      for ( s32 y = 0; y < Dim.y; ++ y)
      {
        for ( s32 x = 0; x < Dim.x; ++ x)
        {
          s32 VoxIndex = GetIndex(Voxel_Position(x,y,z), Dim);
          Chunk->Voxels[VoxIndex].Flags = Voxel_Empty;

          v3 noise_input_name = V3(
              COMPUTE_NOISE_INPUT(x, 0, Chunk),
              COMPUTE_NOISE_INPUT(y, 0, Chunk),
              COMPUTE_NOISE_INPUT(z, 0, Chunk)
          );


          r32 noise_value_name = 0.f;

          user_code

          b32 NoiseChoice = r64((noise_value_name)) > r64(WorldZBiased);

          SetFlag(&Chunk->Voxels[VoxIndex], (voxel_flag)(Voxel_Filled*NoiseChoice));
          Chunk->Voxels[VoxIndex].Color = packed_HSV_color_value_name*u16(NoiseChoice);
          ChunkSum += NoiseChoice;

          Assert( (Chunk->Voxels[VoxIndex].Flags&VoxelFaceMask) == 0);

          if (NoiseChoice)
          {
            Assert( IsSet(&Chunk->Voxels[VoxIndex], Voxel_Filled) );
          }
          else
          {
            Assert( NotSet(&Chunk->Voxels[VoxIndex], Voxel_Filled) );
          }
        }
      }
    }
  }
)



poof(
  func terrain_iteration_pattern_8x( type_poof_symbol noise_input_name,
                                     type_poof_symbol noise_value_name,
                                     type_poof_symbol packed_HSV_color_value_name,
                                     type_poof_symbol user_code)
  @code_fragment
  {
    // NOTE(Jesse): This must hold true for using any Noise_8x func
    Assert(Chunk->Dim % V3i(MIN_TERRAIN_NOISE_WIDTH) == V3i(0));

    Period = Max(Period, V3(1.f));

    for ( s32 z = 0; z < Dim.z; ++ z)
    {
      f32 zCoord = COMPUTE_NOISE_INPUT(z, 0, Chunk);
      f32 WorldZBiased = zCoord - zMin;
      for ( s32 y = 0; y < Dim.y; ++ y)
      {
        f32 yCoord = COMPUTE_NOISE_INPUT(y, 0, Chunk);
        for ( s32 x = 0; x < Dim.x; x += MIN_TERRAIN_NOISE_WIDTH)
        {
          s32 VoxIndex = GetIndex(Voxel_Position(x,y,z), Dim);
          Chunk->Voxels[VoxIndex].Flags = Voxel_Empty;

          f32 xCoords[MIN_TERRAIN_NOISE_WIDTH] =
          {
            (COMPUTE_NOISE_INPUT(x, 0, Chunk)),
            (COMPUTE_NOISE_INPUT(x, 1, Chunk)),
            (COMPUTE_NOISE_INPUT(x, 2, Chunk)),
            (COMPUTE_NOISE_INPUT(x, 3, Chunk)),
            (COMPUTE_NOISE_INPUT(x, 4, Chunk)),
            (COMPUTE_NOISE_INPUT(x, 5, Chunk)),
            (COMPUTE_NOISE_INPUT(x, 6, Chunk)),
            (COMPUTE_NOISE_INPUT(x, 7, Chunk)),
          };

          user_code

          RangeIterator(ValueIndex, MIN_TERRAIN_NOISE_WIDTH)
          {
            s32 ThisIndex = VoxIndex+ValueIndex;
            b32 NoiseChoice = NoiseValues[ThisIndex] > WorldZBiased;
            ChunkSum += NoiseChoice;

            SetFlag(&Chunk->Voxels[ThisIndex], (voxel_flag)(Voxel_Filled*NoiseChoice));
            Chunk->Voxels[ThisIndex].Color = packed_HSV_color_value_name*u16(NoiseChoice);

            Assert( (Chunk->Voxels[ThisIndex].Flags&VoxelFaceMask) == 0);
            if (NoiseChoice)
            {
              Assert( IsSet(&Chunk->Voxels[ThisIndex], Voxel_Filled) );
            }
            else
            {
              Assert( NotSet(&Chunk->Voxels[ThisIndex], Voxel_Filled) );
            }
          }

        }
      }
    }
  }
)




struct noise_value_to_material_index
{
  f32 NoiseValue;
  v3 Color;
};

link_internal v3
MapNoiseValueToMaterial(f32 Value)
{
  noise_value_to_material_index Points[] =
  {
    {1.0f,  RGB_SNOW },
    {0.5f,  RGB_STONE },
    {0.35f, RGB_GRASS_GREEN },
    {0.3f,  RGB_GRASS_GREEN },
    {0.2f,  RGB_GRASS_GREEN },
    {0.0f,  RGB_GRASS_GREEN },
  };


  b32 Hit = False;
  v3 Result = {};
  s32 PointCount = s32(ArrayCount(Points));
  RangeIterator(PointIndex, PointCount)
  {
    noise_value_to_material_index *P = Points + PointIndex;

    // This is sort of a hack to fix the degenerate case when the value is 0 or 1.
    // TODO(Jesse): Make this branchless
    if (Value == P->NoiseValue) return P->Color;

    if (Value > P->NoiseValue)
    {
      return P->Color;
    }
  }
  return {};
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
