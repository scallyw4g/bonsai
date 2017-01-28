#ifndef DEBUG_H
#define DEBUG_H

#define Print(Pos) \
  Print_P( Pos, #Pos )

void
Print_Binary( unsigned int input )
{
  printf("High bit --> ");
  for (int i = (sizeof(int)*8)-1; i >= 0; --i)
  {
    if ( ((input >> i) & 1) == 1 )
    {
      printf("1");
    }
    else
    {
      printf("0");
    }
    if ( (i % 8) == 0 ) { printf(" "); }
  }

  printf("<-- Low bit \n");
}

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
