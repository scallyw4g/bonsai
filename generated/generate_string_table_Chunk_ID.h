link_internal counted_string
ToString(Chunk_ID Type)
{
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
  return Result;
}

