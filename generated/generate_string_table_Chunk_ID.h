// callsite
// src/engine/loaders/vox.cpp:27:0

// def (generate_string_table)
// external/bonsai_stdlib/src/poof_functions.h:1821:0
link_internal b32
IsValid(Chunk_ID Value)
{
  b32 Result = False;
  switch (Value)
  {
        case ID_NONE:
    case ID_VOX:
    case ID_MAIN:
    case ID_PACK:
    case ID_SIZE:
    case ID_XYZI:
    case ID_RGBA:
    case ID_nTRN:
    case ID_nGRP:
    case ID_nSHP:
    case ID_MATL:
    case ID_LAYR:
    case ID_rOBJ:
    case ID_rCAM:
    case ID_NOTE:
    case ID_IMAP:

    {
      Result = True;
    }
  }
  return Result;
}



link_internal counted_string
ToStringPrefixless(Chunk_ID Type)
{
  cs Result = {};
  if (IsValid(Type))
  {
    switch (Type)
    {
            case ID_NONE: { Result = CSz("NONE"); } break;
      case ID_VOX: { Result = CSz("VOX"); } break;
      case ID_MAIN: { Result = CSz("MAIN"); } break;
      case ID_PACK: { Result = CSz("PACK"); } break;
      case ID_SIZE: { Result = CSz("SIZE"); } break;
      case ID_XYZI: { Result = CSz("XYZI"); } break;
      case ID_RGBA: { Result = CSz("RGBA"); } break;
      case ID_nTRN: { Result = CSz("nTRN"); } break;
      case ID_nGRP: { Result = CSz("nGRP"); } break;
      case ID_nSHP: { Result = CSz("nSHP"); } break;
      case ID_MATL: { Result = CSz("MATL"); } break;
      case ID_LAYR: { Result = CSz("LAYR"); } break;
      case ID_rOBJ: { Result = CSz("rOBJ"); } break;
      case ID_rCAM: { Result = CSz("rCAM"); } break;
      case ID_NOTE: { Result = CSz("NOTE"); } break;
      case ID_IMAP: { Result = CSz("IMAP"); } break;


      
    }
  }
  else
  {
    Result = CSz("(CORRUPT ENUM VALUE)");
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (enum_t.name)", Type); } */
  return Result;
}

link_internal counted_string
ToString(Chunk_ID Type)
{
  Assert(IsValid(Type));

  counted_string Result = {};
  switch (Type)
  {
        case ID_NONE: { Result = CSz("ID_NONE"); } break;
    case ID_VOX: { Result = CSz("ID_VOX"); } break;
    case ID_MAIN: { Result = CSz("ID_MAIN"); } break;
    case ID_PACK: { Result = CSz("ID_PACK"); } break;
    case ID_SIZE: { Result = CSz("ID_SIZE"); } break;
    case ID_XYZI: { Result = CSz("ID_XYZI"); } break;
    case ID_RGBA: { Result = CSz("ID_RGBA"); } break;
    case ID_nTRN: { Result = CSz("ID_nTRN"); } break;
    case ID_nGRP: { Result = CSz("ID_nGRP"); } break;
    case ID_nSHP: { Result = CSz("ID_nSHP"); } break;
    case ID_MATL: { Result = CSz("ID_MATL"); } break;
    case ID_LAYR: { Result = CSz("ID_LAYR"); } break;
    case ID_rOBJ: { Result = CSz("ID_rOBJ"); } break;
    case ID_rCAM: { Result = CSz("ID_rCAM"); } break;
    case ID_NOTE: { Result = CSz("ID_NOTE"); } break;
    case ID_IMAP: { Result = CSz("ID_IMAP"); } break;


    
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (enum_t.name)", Type); } */
  return Result;
}

