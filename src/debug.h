#ifndef BONSAI_DEBUG_H
#define BONSAI_DEBUG_H

#include <iostream>
#include <cstdarg>

using namespace std;

#if(MSVC)
#define Assert(condition) if (!(condition)) __debugbreak();
#else
#include <signal.h>
#define Assert(condition) if (!(condition)) raise(SIGTRAP);
// #define Assert(condition) if (!(condition)) __builtin_trap();
#endif

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

#endif
