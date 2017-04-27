#ifndef BONSAI_DEBUG_H
#define BONSAI_DEBUG_H

#include <iostream>
#include <cstdarg>

using namespace std;

// #define Assert(condition) if (!(condition)) __builtin_trap();

struct debug_text_render_group
{
  u32 Text2DTextureID;
  u32 Text2DVertexBufferID;
  u32 Text2DUVBufferID;

  u32 Text2DShaderID;
  u32 Text2DUniformID;
};

void Log(const char* fmt...)
{
  va_list args;
  va_start(args, fmt);

  while (*fmt != '\0')
  {

    if ( *fmt == '%' )
    {
      ++fmt;

      if (*fmt == 'd')
      {
        cout << va_arg(args, int);
      }
      else if (*fmt == 's')
      {
        cout << va_arg(args, char*);
      }
      else if (*fmt == 'f')
      {
        cout << va_arg(args, double);
      }
    }
    else
    {
      cout << (char)(*fmt);
    }

    ++fmt;
  }

  cout << endl;

  va_end(args);
}

#define Print(Pos) \
  Print_P( Pos, #Pos )

void
Print_Binary( unsigned int input )
{
  Log("High bit --> ");
  for (int i = (sizeof(int)*8)-1; i >= 0; --i)
  {
    if ( ((input >> i) & 1) == 1 )
    {
      Log("1");
    }
    else
    {
      Log("0");
    }
    if ( (i % 8) == 0 ) { Log(" "); }
  }

  Log("<-- Low bit \n");
}

inline void
Print_P( canonical_position P, const char* name)
{
  Log(" -- %s\n", name);
  Log(" Offset: %f %f %f \n", P.Offset.x, P.Offset.y, P.Offset.z );
  Log(" WorldP: %d %d %d \n", P.WorldP.x, P.WorldP.y, P.WorldP.z );
}

inline void
Print_P( voxel_position P, const char* name)
{
  Log(" %s %d %d %d \n", name, P.x, P.y, P.z );
}

inline void
Print_P( v3 P, const char* name)
{
  Log(" %s %f %f %f \n", name, P.x, P.y, P.z );
}

inline void
Print_P( glm::vec3 P, const char* name)
{
  Log(" %s %f %f %f \n", name, P.x, P.y, P.z );
}

inline void
Print_P( glm::mat4 &Mat, const char* name)
{
  Log("\n %s ", name );
  Log(" %f %f %f %f ", Mat[0][0], Mat[0][1], Mat[0][2], Mat[0][3] );
  Log(" %f %f %f %f ", Mat[1][0], Mat[1][1], Mat[1][2], Mat[1][3] );
  Log(" %f %f %f %f ", Mat[2][0], Mat[2][1], Mat[2][2], Mat[2][3] );
  Log(" %f %f %f %f ", Mat[3][0], Mat[3][1], Mat[3][2], Mat[3][3] );
}


#endif
