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
RightFaceVertexData( v3 Center, v3 Radius, v3 *Result)
{
  /* TIMED_FUNCTION(); */
  v3 Max = Center + Radius;
  v3 Min = Center - Radius;

  //  0(3) -- 1(7)
  //
  //   |        |
  //
  //  2(2) -- 3(6)

  v3 P0 = {{ Max.x , Min.y , Max.z }};
  v3 P1 = {{ Max.x , Max.y , Max.z }};
  v3 P2 = {{ Max.x , Min.y , Min.z }};
  v3 P3 = {{ Max.x , Max.y , Min.z }};

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
LeftFaceVertexData( v3 Center, v3 Radius, v3 *Result)
{
  /* TIMED_FUNCTION(); */

  //  0(4) -- 1(0)
  //
  //   |        |
  //
  //  2(5) -- 3(1)

  v3 Min = Center - Radius;
  v3 Max = Center + Radius;


  v3 P0 = {{Min.x, Max.y, Max.z}};
  v3 P1 = {{Min.x, Min.y, Max.z}};
  v3 P2 = {{Min.x, Max.y, Min.z}};
  v3 P3 = {{Min.x, Min.y, Min.z}};

#if WIND_CCW
  v3 Temp[] = { P2, P1, P0, P2, P3, P1 };
#else
  v3 Temp[] = { P0, P1, P2, P1, P3, P2 };
#endif


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
BackFaceVertexData( v3 Center, v3 Radius, v3 *Result)
{
  /* TIMED_FUNCTION(); */
  v3 Min = Center - Radius;
  v3 Max = Center + Radius;

  //  0(0) -- 1(3)
  //
  //   |        |
  //
  //  2(1) -- 3(2)

  v3 P0 = {{ Min.x, Min.y, Max.z }};
  v3 P1 = {{ Max.x, Min.y, Max.z }};
  v3 P2 = {{ Min.x, Min.y, Min.z }};
  v3 P3 = {{ Max.x, Min.y, Min.z }};

#if WIND_CCW
  v3 Temp[] = { P2, P1, P0, P2, P3, P1 };
#else
  v3 Temp[] = { P0, P1, P2, P1, P3, P2 };
#endif

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
FrontFaceVertexData( v3 Center, v3 Radius, v3 *Result)
{
  /* TIMED_FUNCTION(); */
  v3 Min = Center - Radius;
  v3 Max = Center + Radius;

  //  0(7) -- 1(4)
  //
  //   |        |
  //
  //  2(6) -- 3(5)

  v3 P0 = {{Min.x , Max.y, Max.z}};
  v3 P1 = {{Max.x , Max.y, Max.z}};
  v3 P2 = {{Min.x , Max.y, Min.z}};
  v3 P3 = {{Max.x , Max.y, Min.z}};

#if WIND_CCW
  v3 Temp[] = { P2, P1, P0, P2, P3, P1 };
#else
  v3 Temp[] = { P0, P1, P2, P1, P3, P2 };
#endif

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
TopFaceVertexData( v3 Center, v3 Radius, v3 *Result)
{
  /* TIMED_FUNCTION(); */
  v3 Min = Center - Radius;
  v3 Max = Center + Radius;

  //  0(4) -- 1(7)
  //
  //   |        |
  //
  //  2(0) -- 3(3)

  v3 P0 = {{ Min.x, Max.y, Max.z }};
  v3 P1 = {{ Max.x, Max.y, Max.z }};
  v3 P2 = {{ Min.x, Min.y, Max.z }};
  v3 P3 = {{ Max.x, Min.y, Max.z }};

#if WIND_CCW
  v3 Temp[] = { P2, P1, P0, P2, P3, P1 };
#else
  v3 Temp[] = { P0, P1, P2, P1, P3, P2 };
#endif

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
BottomFaceVertexData( v3 Center, v3 Radius, v3 *Result)
{
  /* TIMED_FUNCTION(); */
  v3 Min = Center - Radius;
  v3 Max = Center + Radius;

  //  0(1) -- 1(2)
  //
  //   |        |
  //
  //  2(5) -- 3(6)

  v3 P0 = {{ Min.x, Min.y, Min.z }};
  v3 P1 = {{ Max.x, Min.y, Min.z }};
  v3 P2 = {{ Min.x, Max.y, Min.z }};
  v3 P3 = {{ Max.x, Max.y, Min.z }};

#if WIND_CCW
  v3 Temp[] = { P2, P1, P0, P2, P3, P1 };
#else
  v3 Temp[] = { P0, P1, P2, P1, P3, P2 };
#endif

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
