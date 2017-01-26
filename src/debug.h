#ifndef DEBUG_H
#define DEBUG_H

#define Print(Pos) \
  Print_P( Pos, #Pos )

inline void
Print_P( canonical_position P, const char* name)
{
  printf(" -- %s\n", name);
  printf(" Offset: %f %f %f \n", P.Offset.x, P.Offset.y, P.Offset.z );
  printf(" WorldP: %d %d %d \n", P.WorldP.x, P.WorldP.y, P.WorldP.z );
}

inline void
Print_P( voxel_position P, const char* name)
{
  printf(" %s %d %d %d \n", name, P.x, P.y, P.z );
}

inline void
Print_P( v3 P, const char* name)
{
  printf(" %s %f %f %f \n", name, P.x, P.y, P.z );
}

inline void
Print_P( glm::vec3 P, const char* name)
{
  printf(" %s %f %f %f \n", name, P.x, P.y, P.z );
}

#endif
