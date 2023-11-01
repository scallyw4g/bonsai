

//                 Not sure why I drew this, but it was kinda fun..
//
//                        top
//                    4 --------- 7
//                   /|          /|     Z
//                  / |         / |     ^
//                 0 --------- 3  |     |
//                 |  |        |  |     |   Y
//                 |  5 -------|- 6     |  /
//          left   | /         | /      | /
//                 |/          |/       |/
//                 1 --------- 2        o-----------> X
//
//                   back

/* global_variable v3 ShimmerFix = {}; */
/* global_variable f32 ShimmerStrength = 0.0005f; */
/* global_variable f32 ShimmerStrength = -0.0005f; */
// TODO(Jesse): Remove this
global_variable f32 ShimmerStrength = 0.f;
global_variable v3 ShimmerFix = {{ShimmerStrength, ShimmerStrength, ShimmerStrength}};

global_variable v3 TopPlane  = {{ 1.f, 1.f, 0.f }};
global_variable v3 BotPlane  = {{ 1.f, 1.f, 0.f }};

global_variable v3 LeftPlane  = {{ 0.f, 1.f, 1.f }};
global_variable v3 RightPlane = {{ 0.f, 1.f, 1.f }};

global_variable v3 FrontPlane = {{ 1.f, 0.f, 1.f }};
global_variable v3 BackPlane  = {{ 1.f, 0.f, 1.f }};


// NOTE(Jesse): This is pretty similar to voxel_rule_direction @duplicate_face_index_enum
enum face_index
{
  FaceIndex_None,

  FaceIndex_Left,
  FaceIndex_Right,
  FaceIndex_Front,
  FaceIndex_Back,
  FaceIndex_Top,
  FaceIndex_Bot,
};



inline void
RightFaceVertexData( v3 MinP, v3 Diameter, v3 *Result)
{
  v3 MaxP = MinP + Diameter + (ShimmerFix*RightPlane);
  MinP -= ShimmerFix;

  /* Round(MaxP); */
  /* Round(MinP); */

  //  0    1
  //
  //  2    3

  v3 P0 = {{ MaxP.x, MinP.y, MaxP.z }};
  v3 P1 = {{ MaxP.x, MaxP.y, MaxP.z }};
  v3 P2 = {{ MaxP.x, MinP.y, MinP.z }};
  v3 P3 = {{ MaxP.x, MaxP.y, MinP.z }};

  v3 Temp[] = { P2, P1, P0, P2, P3, P1 };
  MemCopy((u8*)Temp, (u8*)Result, sizeof(Temp));

  return;
}

global_variable v3 RightFaceNormalData[] =
{
  {{1, 0, 0}},
  {{1, 0, 0}},
  {{1, 0, 0}},
  {{1, 0, 0}},
  {{1, 0, 0}},
  {{1, 0, 0}}
};

inline void
LeftFaceVertexData( v3 MinP, v3 Diameter, v3 *Result)
{
  v3 MaxP = MinP + Diameter + (ShimmerFix*LeftPlane);
  MinP -= ShimmerFix;

  /* Round(MaxP); */
  /* Round(MinP); */

  //  0    1
  //
  //  2    3

  v3 P0 = {{ MinP.x, MaxP.y, MaxP.z }};
  v3 P1 = {{ MinP.x, MinP.y, MaxP.z }};
  v3 P2 = {{ MinP.x, MaxP.y, MinP.z }};
  v3 P3 = {{ MinP.x, MinP.y, MinP.z }};

  v3 Temp[] = { P2, P1, P0, P2, P3, P1 };
  MemCopy((u8*)Temp, (u8*)Result, sizeof(Temp));

  return;
}

global_variable v3 LeftFaceNormalData[] =
{
  {{-1, 0, 0}},
  {{-1, 0, 0}},
  {{-1, 0, 0}},
  {{-1, 0, 0}},
  {{-1, 0, 0}},
  {{-1, 0, 0}},
};

inline void
BackFaceVertexData( v3 MinP, v3 Diameter, v3 *Result)
{
  v3 MaxP = MinP + Diameter + (ShimmerFix*BackPlane);
  MinP -= ShimmerFix;

  /* Round(MaxP); */
  /* Round(MinP); */

  //  0    3
  //
  //  1    2

  v3 P0 = {{ MinP.x, MinP.y, MaxP.z }};
  v3 P1 = {{ MaxP.x, MinP.y, MaxP.z }};
  v3 P2 = {{ MinP.x, MinP.y, MinP.z }};
  v3 P3 = {{ MaxP.x, MinP.y, MinP.z }};

  v3 Temp[] = { P0, P2, P1, P1, P2, P3 };
  MemCopy((u8*)Temp, (u8*)Result, sizeof(Temp));

  return;
}

global_variable v3 BackFaceNormalData[] =
{
  {{0,-1, 0}},
  {{0,-1, 0}},
  {{0,-1, 0}},
  {{0,-1, 0}},
  {{0,-1, 0}},
  {{0,-1, 0}},
};

inline void
FrontFaceVertexData( v3 MinP, v3 Diameter, v3 *Result)
{
  v3 MaxP = MinP + Diameter + (ShimmerFix*FrontPlane);
  MinP -= ShimmerFix;

  /* Round(MaxP); */
  /* Round(MinP); */

  //  7    4
  //
  //  6    5

  v3 P7 = {{ MaxP.x, MaxP.y, MaxP.z }};
  v3 P4 = {{ MinP.x, MaxP.y, MaxP.z }};
  v3 P6 = {{ MaxP.x, MaxP.y, MinP.z }};
  v3 P5 = {{ MinP.x, MaxP.y, MinP.z }};

  v3 Temp[] = { P7, P6, P4, P4, P6, P5 };
  MemCopy((u8*)Temp, (u8*)Result, sizeof(Temp));


  return;
}

global_variable v3 FrontFaceNormalData[] =
{
  {{0, 1, 0}},
  {{0, 1, 0}},
  {{0, 1, 0}},
  {{0, 1, 0}},
  {{0, 1, 0}},
  {{0, 1, 0}},
};

inline void
TopFaceVertexData( v3 MinP, v3 Diameter, v3 *Result)
{
  v3 MaxP = MinP + Diameter + (ShimmerFix*TopPlane);
  MinP -= ShimmerFix;

  /* Round(MaxP); */
  /* Round(MinP); */

  //  4    7
  //
  //  0    3

  v3 P4 = {{ MinP.x, MaxP.y, MaxP.z }};
  v3 P7 = {{ MaxP.x, MaxP.y, MaxP.z }};
  v3 P0 = {{ MinP.x, MinP.y, MaxP.z }};
  v3 P3 = {{ MaxP.x, MinP.y, MaxP.z }};

  v3 Temp[] = { P4, P0, P7, P7, P0, P3 };
  MemCopy((u8*)Temp, (u8*)Result, sizeof(Temp));

  return;
}

global_variable v3 TopFaceNormalData[] =
{
  {{0, 0, 1}},
  {{0, 0, 1}},
  {{0, 0, 1}},

  {{0, 0, 1}},
  {{0, 0, 1}},
  {{0, 0, 1}},
};

inline void
BottomFaceVertexData( v3 MinP, v3 Diameter, v3 *Result)
{
  /* TIMED_FUNCTION(); */
  v3 MaxP = MinP + Diameter + (ShimmerFix*BotPlane);
  MinP -= ShimmerFix;

  /* Round(MaxP); */
  /* Round(MinP); */

  r32 Temp[] = {
    MaxP.x , MinP.y , MinP.z ,
    MinP.x , MinP.y , MinP.z ,
    MaxP.x , MaxP.y , MinP.z ,
    MinP.x , MaxP.y , MinP.z ,
    MaxP.x , MaxP.y , MinP.z ,
    MinP.x , MinP.y , MinP.z ,
  };

  MemCopy((u8*)Temp, (u8*)Result, sizeof(Temp));

  return;
}

global_variable v3 BottomFaceNormalData[] =
{
  {{0, 0, -1}},
  {{0, 0, -1}},
  {{0, 0, -1}},

  {{0, 0, -1}},
  {{0, 0, -1}},
  {{0, 0, -1}},
};




link_internal v3
NormalForFace(face_index Face)
{
  v3 Result;
  switch (Face)
  {
    InvalidCase(FaceIndex_None);

    case FaceIndex_Top:
    {
      Result = V3(0, 0, 1);
    } break;

    case FaceIndex_Bot:
    {
      Result = V3(0, 0, -1);
    } break;

    case FaceIndex_Left:
    {
      Result = V3(-1, 0, 0);
    } break;

    case FaceIndex_Right:
    {
      Result = V3(1, 0, 0);
    } break;

    case FaceIndex_Front:
    {
      Result = V3(0, 1, 0);
    } break;

    case FaceIndex_Back:
    {
      Result = V3(0, -1, 0);
    } break;
  }

  return Result;
}


