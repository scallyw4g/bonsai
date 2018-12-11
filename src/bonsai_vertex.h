#ifndef BONSAI_VERTEX_H
#define BONSAI_VERTEX_H




//    Not sure why I drew this, but it was kinda fun..
//
//     4              7
//       +-----------+
//      /|          /|      Z
//     / |         / |      ^
//  0 +-----------3  |      |
//    |  |        |  |      |   Y
//    |  +--------|--+      |  /
//    | / 5       | /   6   | /
//    |/          |/        |/
//    +-----------+         +-----------> X
//  1              2
//


#define WIND_CCW 0

inline void
RightFaceVertexData( v3 MinP, v3 Diameter, v3 *Result)
{
  /* TIMED_FUNCTION(); */
  v3 MaxP = MinP + Diameter;

  //  0    1
  //
  //  2    3

  v3 P0 = {{ MaxP.x, MinP.y, MaxP.z }};
  v3 P1 = {{ MaxP.x, MaxP.y, MaxP.z }};
  v3 P2 = {{ MaxP.x, MinP.y, MinP.z }};
  v3 P3 = {{ MaxP.x, MaxP.y, MinP.z }};

#if WIND_CCW
  v3 Temp[] = { P2, P1, P0, P2, P3, P1 };
#else
  v3 Temp[] = { P0, P1, P2, P1, P3, P2 };
#endif

  memcpy(Result, Temp, sizeof(Temp));

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
  /* TIMED_FUNCTION(); */
  r32 Temp[] = {
    MinP.x , MinP.y +  Diameter.y , MinP.z +  Diameter.z ,
    MinP.x , MinP.y               , MinP.z               ,
    MinP.x , MinP.y +  Diameter.y , MinP.z               ,
    MinP.x , MinP.y               , MinP.z               ,
    MinP.x , MinP.y +  Diameter.y , MinP.z +  Diameter.z ,
    MinP.x , MinP.y               , MinP.z +  Diameter.z ,
  };

  memcpy(Result, Temp, sizeof(Temp));

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
  /* TIMED_FUNCTION(); */
  v3 MaxP = MinP + Diameter;

  //  0    1
  //
  //  2    3

  v3 P0 = {{ MinP.x, MinP.y, MaxP.z }};
  v3 P1 = {{ MaxP.x, MinP.y, MaxP.z }};
  v3 P2 = {{ MinP.x, MinP.y, MinP.z }};
  v3 P3 = {{ MaxP.x, MinP.y, MinP.z }};

  v3 Temp[] = { P0, P1, P2, P1, P3, P2 };

  memcpy(Result, Temp, sizeof(Temp));

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
  /* TIMED_FUNCTION(); */
  r32 Temp[] = {
    MinP.x + Diameter.x , MinP.y + Diameter.y , MinP.z + Diameter.z ,
    MinP.x              , MinP.y + Diameter.y , MinP.z              ,
    MinP.x + Diameter.x , MinP.y + Diameter.y , MinP.z              ,
    MinP.x              , MinP.y + Diameter.y , MinP.z              ,
    MinP.x + Diameter.x , MinP.y + Diameter.y , MinP.z + Diameter.z ,
    MinP.x              , MinP.y + Diameter.y , MinP.z + Diameter.z ,
  };

  memcpy(Result, Temp, sizeof(Temp));

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
  /* TIMED_FUNCTION(); */
  v3 MaxP = MinP + Diameter;

  //  0    1
  //
  //  2    3

  v3 P0 = {{ MinP.x, MaxP.y, MaxP.z }};
  v3 P1 = {{ MaxP.x, MaxP.y, MaxP.z }};
  v3 P2 = {{ MinP.x, MinP.y, MaxP.z }};
  v3 P3 = {{ MaxP.x, MinP.y, MaxP.z }};

  v3 Temp[] = { P0, P1, P2, P1, P3, P2 };

  memcpy(Result, Temp, sizeof(Temp));

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
  r32 Temp[] = {
    MinP.x + Diameter.x , MinP.y + Diameter.y , MinP.z ,
    MinP.x              , MinP.y              , MinP.z ,
    MinP.x + Diameter.x , MinP.y              , MinP.z ,
    MinP.x              , MinP.y              , MinP.z ,
    MinP.x + Diameter.x , MinP.y + Diameter.y , MinP.z ,
    MinP.x              , MinP.y + Diameter.y , MinP.z ,
  };

  memcpy(Result, Temp, sizeof(Temp));

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


#endif
