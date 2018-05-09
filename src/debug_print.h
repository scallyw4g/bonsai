#include <iostream>
#include <cstdarg>


void
Log(const char* fmt...)
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
        std::cout << va_arg(args, s32);
      }
      else if (*fmt == 'l')
      {
        ++fmt;
        if (*fmt == 'u')
        {
          std::cout << va_arg(args, u64);
        }
        else if (*fmt == 'd')
        {
          std::cout << va_arg(args, s64);
        }
      }
      else if (*fmt == 'x')
      {
        std::cout << va_arg(args, void*);
      }
      else if (*fmt == 'u')
      {
        std::cout << va_arg(args, u32);
      }
      else if (*fmt == 's')
      {
        std::cout << va_arg(args, char*);
      }
      else if (*fmt == 'f')
      {
        std::cout << va_arg(args, r64);
      }
      else if (*fmt == 'b')
      {
        b32 BoolVal = va_arg(args, int);
        const char *Output = 0;

        if (BoolVal)
          Output = "True";
        else
          Output = "False";

        std::cout << Output;
      }
      else
      {
        va_arg(args, void*);
        std::cout << "Invalid Format String";
      }

    }
    else
    {
      std::cout << (char)(*fmt);
    }

    ++fmt;
  }

  std::cout << std::endl;

  va_end(args);
}

#define Print(Var) \
  Print_P( Var, #Var )

void
Print_Binary( unsigned int input )
{
  Log("High bit --> ");
  for (int i = (sizeof(int)*8)-1; i >= 0; --i)
  {
    if ( ((input >> i) & 1) == 1 )
      Log("1");
    else
      Log("0");

    if ( (i % 8) == 0 ) { Log(" "); }
  }

  Log("<-- Low bit \n");
}

/* inline void */
/* Print_P( debug_profile_scope *E, const char* name) */
/* { */
/*   Log(" -- %s -> %s \n", name, E->Name); */
/*   Log(" -- Parent %x \n", name, E->Parent); */
/*   Log(" -- Sibling %x \n", name, E->Sibling); */
/*   Log(" -- Child %x \n", name, E->Child); */
/* } */

inline void
Print_P( const char *C, const char* name)
{
  Log(" -- %s \n", C);
}

inline void
Print_P( input_event E, const char* name)
{
  Log(" -- %s == IsDown %d WasPressed %d \n", name, E.IsDown, E.WasPressed );
}

inline void
Print_P( u64 N, const char* name)
{
  Log(" -- %s == %lu \n", name, N);
}

inline void
Print_P( u32 N, const char* name)
{
  Log(" -- %s == %u \n", name, N);
}

inline void
Print_P( s32 N, const char* name)
{
  Log(" -- %s == %d \n", name, N);
}

inline void
Print_P( r64 N, const char* name)
{
  Log(" -- %s == %f \n", name, N);
}

inline void
Print_P( r32 N, const char* name)
{
  Log(" -- %s == %f \n", name, N);
}

inline void
Print_P( aabb P, const char* name)
{
  Log(" -- %s", name);
  Log(" Center: %f %f %f ", P.Center.x, P.Center.y, P.Center.z );
  Log(" Radius: %f %f %f \n", P.Radius.x, P.Radius.y, P.Radius.z );
}

inline void
Print_P( canonical_position P, const char* name)
{
  Log(" -- %s", name);
  Log(" Offset: %f %f %f ", P.Offset.x, P.Offset.y, P.Offset.z );
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
#if 0
  if ( Abs(P.x) < 0.000001f )
    P.x = 0;

  if ( Abs(P.y) < 0.000001f )
    P.y = 0;

  if ( Abs(P.z) < 0.000001f )
    P.z = 0;
#endif

  Log(" %s %f %f %f \n", name, P.x, P.y, P.z );
}

inline void
Print_P( v2 P, const char* name)
{
#if 0
  if ( Abs(P.x) < 0.000001f )
    P.x = 0;

  if ( Abs(P.y) < 0.000001f )
    P.y = 0;
#endif

  Log(" %s %f %f \n", name, P.x, P.y );
}

inline void
Print_P( m4 *Mat, const char* name)
{
  Log("\n %s ", name );
  Log(" %f %f %f %f ", Mat->E[0].E[0], Mat->E[0].E[1], Mat->E[0].E[2], Mat->E[0].E[3] );
  Log(" %f %f %f %f ", Mat->E[1].E[0], Mat->E[1].E[1], Mat->E[1].E[2], Mat->E[1].E[3] );
  Log(" %f %f %f %f ", Mat->E[2].E[0], Mat->E[2].E[1], Mat->E[2].E[2], Mat->E[2].E[3] );
  Log(" %f %f %f %f ", Mat->E[3].E[0], Mat->E[3].E[1], Mat->E[3].E[2], Mat->E[3].E[3] );
  Log("");
}
