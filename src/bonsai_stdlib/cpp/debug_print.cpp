
// TODO(Jesse id: 265): Unnecessary .. I just added these as a hack get parsing to work
typedef va_list bonsai_va_list;

bonsai_function counted_string FormatCountedString_(char* Buffer, umm BufferSize, counted_string FS, va_list Args);

#define Print(Var) \
  Print_P( Var, #Var )

void
PrintBinary( unsigned int Input )
{
  DebugChars("High bit --> ");
  for (int i = (sizeof(int)*8)-1; i >= 0; --i)
  {
    if ( ((Input >> i) & 1) == 1 )
    {
      DebugChars("1");
    }
    else
    {
      DebugChars("0");
    }

    if ( (i % 8) == 0 ) { DebugChars(" "); }
  }

  DebugLine("<-- Low bit \n");
}

inline void
Print_P( void *P, const char* name)
{
  DebugLine(" %s -- %u \n", name, P);
}

inline void
Print_P( const char *C, const char* name)
{
  DebugLine(" %s -- %s \n", name, C);
}

inline void
Print_P( input_event E, const char* name)
{
  DebugLine(" -- %s == Pressed %d Clicked %d \n", name, E.Pressed, E.Clicked );
}

inline void
Print_P( u64 N, const char* name)
{
  DebugLine(" -- %s == %lu \n", name, N);
}

inline void
Print_P( u16 N, const char* name)
{
  DebugLine(" -- %s == %u \n", name, N);
}

inline void
Print_P( u32 N, const char* name)
{
  DebugLine(" -- %s == %u \n", name, N);
}

inline void
Print_P( s32 N, const char* name)
{
  DebugLine(" -- %s == %d \n", name, N);
}

inline void
Print_P( r64 N, const char* name)
{
  DebugLine(" -- %s == %f \n", name, N);
}

inline void
Print_P( r32 N, const char* name)
{
  DebugLine(" -- %s == %f \n", name, N);
}

inline void
Print_P( aabb P, const char* name)
{
  DebugLine(" -- %s", name);
  DebugLine(" Center: %f %f %f ", P.Center.x, P.Center.y, P.Center.z );
  DebugLine(" Radius: %f %f %f \n", P.Radius.x, P.Radius.y, P.Radius.z );
}

inline void
Print_P( canonical_position P, const char* name)
{
  DebugLine(" -- %s", name);
  DebugLine(" Offset: %f %f %f ", P.Offset.x, P.Offset.y, P.Offset.z );
  DebugLine(" WorldP: %d %d %d \n", P.WorldP.x, P.WorldP.y, P.WorldP.z );
}

inline void
Print_P( voxel_position P, const char* name)
{
  DebugLine(" %s %d %d %d \n", name, P.x, P.y, P.z );
}

inline void
Print_P( v4 P, const char* name)
{
#if 0
  if ( Abs(P.x) < 0.000001f )
    P.x = 0;

  if ( Abs(P.y) < 0.000001f )
    P.y = 0;

  if ( Abs(P.z) < 0.000001f )
    P.z = 0;
#endif

  DebugLine(" %s %f %f %f %f \n", name, P.x, P.y, P.z, P.w );
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

  DebugLine(" %s %f %f %f \n", name, P.x, P.y, P.z );
}

inline void
Print_P( v2i P, const char* name)
{
#if 0
  if ( Abs(P.x) < 0.000001f )
    P.x = 0;

  if ( Abs(P.y) < 0.000001f )
    P.y = 0;
#endif

  DebugLine(" %s %d %d \n", name, P.x, P.y );
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

  DebugLine(" %s %f %f \n", name, P.x, P.y );
}

inline void
Print_P( rect2 Rect, const char* name)
{
  DebugLine(" %s.Min %f %f \n", name, Rect.Min.x, Rect.Min.y );
  DebugLine(" %s.Max %f %f \n", name, Rect.Max.x, Rect.Max.y );
}

inline void
Print_P( m4 *Mat, const char* name)
{
  DebugLine("\n %s ", name );
  DebugLine(" %f %f %f %f ", Mat->E[0].E[0], Mat->E[0].E[1], Mat->E[0].E[2], Mat->E[0].E[3] );
  DebugLine(" %f %f %f %f ", Mat->E[1].E[0], Mat->E[1].E[1], Mat->E[1].E[2], Mat->E[1].E[3] );
  DebugLine(" %f %f %f %f ", Mat->E[2].E[0], Mat->E[2].E[1], Mat->E[2].E[2], Mat->E[2].E[3] );
  DebugLine(" %f %f %f %f ", Mat->E[3].E[0], Mat->E[3].E[1], Mat->E[3].E[2], Mat->E[3].E[3] );
  DebugLine("");
}

inline void
Print_P(counted_string String, const char* Name)
{
  DebugLine("%s %.*s", Name, String.Count, String.Start);
}

inline void
Print_P( xml_token *Token, const char* Name)
{
  DebugLine("Name: %s", Name);
  switch (Token->Type)
  {
    case XmlTokenType_Boolean:
    {
      DebugLine("Boolean %.*s", Token->Property.Name.Count, Token->Property.Name.Start);
    } break;

    case XmlTokenType_Float:
    {
      DebugLine("Float %.*s = %.*s", Token->Property.Name.Count, Token->Property.Name.Start, Token->Property.Value.Count, Token->Property.Value.Start);
    } break;

    case XmlTokenType_Int:
    {
      DebugLine("Int %.*s = %.*s", Token->Property.Name.Count, Token->Property.Name.Start, Token->Property.Value.Count, Token->Property.Value.Start);
    } break;

    case XmlTokenType_Property:
    {
      DebugLine("String %.*s = \"%.*s\"", Token->Property.Name.Count, Token->Property.Name.Start, Token->Property.Value.Count, Token->Property.Value.Start);
    } break;

    case XmlTokenType_Open:
    {
      DebugLine("OpenToken %.*s", Token->Property.Name.Count, Token->Property.Name.Start);
    } break;

    case XmlTokenType_Close:
    {
      DebugLine("CloseToken %.*s", Token->Property.Name.Count, Token->Property.Name.Start);
    } break;

    InvalidDefaultCase;
  }

  return;
}

inline void
Print_P( xml_tag *Tag, const char* Name)
{
  xml_token* Token = Tag->Open;
  Print_P(Token, Name);

  xml_property* Current = Tag->Properties;
  while (Current)
  {
    Print_P(Current->Name, Name);
    Print_P(Current->Value, Name);
    Current = Current->Next;
  }

  return;
}

inline void
Print_P( camera *Camera, const char* name)
{
  DebugLine("\n %s ", name );
  Print(Camera->TargetP);
  Print(Camera->CurrentP);
  Print(Camera->ViewingTarget);
  DebugLine("");
}

inline void
Print_P( interactable *Interactable, const char* name)
{
  DebugLine("\n %s ", name );
  Print(Interactable->ID);
  Print(Interactable->MinP);
  Print(Interactable->MaxP);
  DebugLine("");
}

inline void
Print_P( interactable Interactable, const char* name)
{
  Print_P(&Interactable, name);
}

inline void
Print_P( window_layout *Window, const char* name)
{
  DebugLine("%s", name);
  Print(Window->Title);
  Print(Window->MaxClip);
  Print(Window->Basis);
}

