

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
/* global_variable f32 ShimmerStrength = 0.f; */
/* global_variable v3 ShimmerFix = {{ShimmerStrength, ShimmerStrength, ShimmerStrength}}; */

global_variable v3 TopPlane  = {{ 1.f, 1.f, 0.f }};
global_variable v3 BotPlane  = {{ 1.f, 1.f, 0.f }};

global_variable v3 LeftPlane  = {{ 0.f, 1.f, 1.f }};
global_variable v3 RightPlane = {{ 0.f, 1.f, 1.f }};

global_variable v3 FrontPlane = {{ 1.f, 0.f, 1.f }};
global_variable v3 BackPlane  = {{ 1.f, 0.f, 1.f }};


global_variable v3 RightFaceNormalData[]  = { {{1, 0, 0}}, {{1, 0, 0}}, {{1, 0, 0}}, {{1, 0, 0}}, {{1, 0, 0}}, {{1, 0, 0}} };
global_variable v3 LeftFaceNormalData[]   = { {{-1, 0, 0}}, {{-1, 0, 0}}, {{-1, 0, 0}}, {{-1, 0, 0}}, {{-1, 0, 0}}, {{-1, 0, 0}}, };
global_variable v3 BackFaceNormalData[]   = { {{0,-1, 0}}, {{0,-1, 0}}, {{0,-1, 0}}, {{0,-1, 0}}, {{0,-1, 0}}, {{0,-1, 0}}, };
global_variable v3 FrontFaceNormalData[]  = { {{0, 1, 0}}, {{0, 1, 0}}, {{0, 1, 0}}, {{0, 1, 0}}, {{0, 1, 0}}, {{0, 1, 0}}, };
global_variable v3 TopFaceNormalData[]    = { {{0, 0, 1}}, {{0, 0, 1}}, {{0, 0, 1}}, {{0, 0, 1}}, {{0, 0, 1}}, {{0, 0, 1}}, };
global_variable v3 BottomFaceNormalData[] = { {{0, 0, -1}}, {{0, 0, -1}}, {{0, 0, -1}}, {{0, 0, -1}}, {{0, 0, -1}}, {{0, 0, -1}}, };





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



poof(
  func vertex_data_functions(vector_t)
  {
    inline void
    RightFaceVertexData( vector_t.name MinP, v3 Diameter, vector_t.name *Result)
    {
      vector_t.name MaxP = MinP + Diameter;

      /* Round(MaxP); */
      /* Round(MinP); */

      //  0    1
      //
      //  2    3

      vector_t.name P0 = {{ MaxP.x, MinP.y, MaxP.z }};
      vector_t.name P1 = {{ MaxP.x, MaxP.y, MaxP.z }};
      vector_t.name P2 = {{ MaxP.x, MinP.y, MinP.z }};
      vector_t.name P3 = {{ MaxP.x, MaxP.y, MinP.z }};

      vector_t.name Temp[] = { P2, P1, P0, P2, P3, P1 };
      MemCopy((u8*)Temp, (u8*)Result, sizeof(Temp));

      return;
    }

    inline void
    LeftFaceVertexData( vector_t.name MinP, v3 Diameter, vector_t.name *Result)
    {
      vector_t.name MaxP = MinP + Diameter;

      /* Round(MaxP); */
      /* Round(MinP); */

      //  0    1
      //
      //  2    3

      vector_t.name P0 = {{ MinP.x, MaxP.y, MaxP.z }};
      vector_t.name P1 = {{ MinP.x, MinP.y, MaxP.z }};
      vector_t.name P2 = {{ MinP.x, MaxP.y, MinP.z }};
      vector_t.name P3 = {{ MinP.x, MinP.y, MinP.z }};

      vector_t.name Temp[] = { P2, P1, P0, P2, P3, P1 };
      MemCopy((u8*)Temp, (u8*)Result, sizeof(Temp));

      return;
    }

    inline void
    BackFaceVertexData( vector_t.name MinP, v3 Diameter, vector_t.name *Result)
    {
      vector_t.name MaxP = MinP + Diameter;

      /* Round(MaxP); */
      /* Round(MinP); */

      //  0    3
      //
      //  1    2

      vector_t.name P0 = {{ MinP.x, MinP.y, MaxP.z }};
      vector_t.name P1 = {{ MaxP.x, MinP.y, MaxP.z }};
      vector_t.name P2 = {{ MinP.x, MinP.y, MinP.z }};
      vector_t.name P3 = {{ MaxP.x, MinP.y, MinP.z }};

      vector_t.name Temp[] = { P0, P2, P1, P1, P2, P3 };
      MemCopy((u8*)Temp, (u8*)Result, sizeof(Temp));

      return;
    }

    inline void
    FrontFaceVertexData( vector_t.name MinP, v3 Diameter, vector_t.name *Result)
    {
      vector_t.name MaxP = MinP + Diameter;

      /* Round(MaxP); */
      /* Round(MinP); */

      //  7    4
      //
      //  6    5

      vector_t.name P7 = {{ MaxP.x, MaxP.y, MaxP.z }};
      vector_t.name P4 = {{ MinP.x, MaxP.y, MaxP.z }};
      vector_t.name P6 = {{ MaxP.x, MaxP.y, MinP.z }};
      vector_t.name P5 = {{ MinP.x, MaxP.y, MinP.z }};

      vector_t.name Temp[] = { P7, P6, P4, P4, P6, P5 };
      MemCopy((u8*)Temp, (u8*)Result, sizeof(Temp));


      return;
    }

    inline void
    TopFaceVertexData( vector_t.name MinP, v3 Diameter, vector_t.name *Result)
    {
      vector_t.name MaxP = MinP + Diameter;

      /* Round(MaxP); */
      /* Round(MinP); */

      //  4    7
      //
      //  0    3

      vector_t.name P4 = {{ MinP.x, MaxP.y, MaxP.z }};
      vector_t.name P7 = {{ MaxP.x, MaxP.y, MaxP.z }};
      vector_t.name P0 = {{ MinP.x, MinP.y, MaxP.z }};
      vector_t.name P3 = {{ MaxP.x, MinP.y, MaxP.z }};

      vector_t.name Temp[] = { P4, P0, P7, P7, P0, P3 };
      MemCopy((u8*)Temp, (u8*)Result, sizeof(Temp));

      return;
    }

    inline void
    BottomFaceVertexData( vector_t.name MinP, v3 Diameter, vector_t.name *Result)
    {
      /* TIMED_FUNCTION(); */
      vector_t.name MaxP = MinP + Diameter;

      /* Round(MaxP); */
      /* Round(MinP); */

      vector_t.name Temp[] = {
        {{MaxP.x , MinP.y , MinP.z}},
        {{MinP.x , MinP.y , MinP.z}},
        {{MaxP.x , MaxP.y , MinP.z}},
        {{MinP.x , MaxP.y , MinP.z}},
        {{MaxP.x , MaxP.y , MinP.z}},
        {{MinP.x , MinP.y , MinP.z}},
      };

      MemCopy((u8*)Temp, (u8*)Result, sizeof(Temp));

      return;
    }
  }
)

poof(vertex_data_functions(v3))
#include <generated/vertex_data_functions_v3.h>

poof(vertex_data_functions(v3_u8))
#include <generated/vertex_data_functions_v3_u8.h>

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

