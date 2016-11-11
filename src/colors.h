#define FACE_COLOR_SIZE sizeof(YELLOW)

const float YELLOW[] =
{
  1.0f,  0.0f,  1.0f,
  1.0f,  0.0f,  1.0f,
  1.0f,  0.0f,  1.0f,

  1.0f,  0.0f,  1.0f,
  1.0f,  0.0f,  1.0f,
  1.0f,  0.0f,  1.0f
};

const float RED[] =
{
  1.0f,  0.0f,  0.0f,
  1.0f,  0.0f,  0.0f,
  1.0f,  0.0f,  0.0f,

  1.0f,  0.0f,  0.0f,
  1.0f,  0.0f,  0.0f,
  1.0f,  0.0f,  0.0f
};

const float TEAL[] =
{
  0.0f,  1.0f,  1.0f,
  0.0f,  1.0f,  1.0f,
  0.0f,  1.0f,  1.0f,

  0.0f,  1.0f,  1.0f,
  0.0f,  1.0f,  1.0f,
  0.0f,  1.0f,  1.0f
};

const float GREEN[] =
{
  0.0f,  1.0f,  0.0f,
  0.0f,  1.0f,  0.0f,
  0.0f,  1.0f,  0.0f,

  0.0f,  1.0f,  0.0f,
  0.0f,  1.0f,  0.0f,
  0.0f,  1.0f,  0.0f
};

const float WHITE[] =
{
  1.0f,  1.0f,  1.0f,
  1.0f,  1.0f,  1.0f,
  1.0f,  1.0f,  1.0f,

  1.0f,  1.0f,  1.0f,
  1.0f,  1.0f,  1.0f,
  1.0f,  1.0f,  1.0f
};

const float PURPLE[] =
{
  1.0f,  1.0f,  0.0f,
  1.0f,  1.0f,  0.0f,
  1.0f,  1.0f,  0.0f,

  1.0f,  1.0f,  0.0f,
  1.0f,  1.0f,  0.0f,
  1.0f,  1.0f,  0.0f
};

const float*
GetColorData( int VoxelFlags )
{
  if ( IsSet(VoxelFlags, Voxel_Purple) )
  {
    return PURPLE;
  }

  if ( IsSet(VoxelFlags, Voxel_Yellow) )
  {
    return YELLOW;
  }

  if ( IsSet(VoxelFlags, Voxel_Red) )
  {
    return RED;
  }

  if ( IsSet(VoxelFlags, Voxel_Green) )
  {
    return GREEN;
  }

  if ( IsSet(VoxelFlags, Voxel_Teal) )
  {
    return TEAL;
  }

  if ( IsSet(VoxelFlags, Voxel_White) )
  {
    return WHITE;
  }

  assert(false);
  return 0;
}

