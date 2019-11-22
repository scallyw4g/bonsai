#ifndef BONSAI_VERTEX_H
#define BONSAI_VERTEX_H




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


inline void
RightFaceVertexData( v3 MinP, v3 Diameter, v3 *Result)
{
  v3 MaxP = MinP + Diameter;

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
  v3 MaxP = MinP + Diameter;

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
  v3 MaxP = MinP + Diameter;

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
  v3 MaxP = MinP + Diameter;

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
  v3 MaxP = MinP + Diameter;

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
  r32 Temp[] = {
    MinP.x + Diameter.x , MinP.y + Diameter.y , MinP.z ,
    MinP.x              , MinP.y              , MinP.z ,
    MinP.x + Diameter.x , MinP.y              , MinP.z ,
    MinP.x              , MinP.y              , MinP.z ,
    MinP.x + Diameter.x , MinP.y + Diameter.y , MinP.z ,
    MinP.x              , MinP.y + Diameter.y , MinP.z ,
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


#endif
