#define BONSAI_NO_MUTEX_TRACKING
#define BONSAI_NO_TIMED_FUNCTIONS
#define BONSAI_NO_PUSH_METADATA
#define BONSAI_NO_DEBUG_MEMORY_ALLOCATOR

#include <bonsai_types.h>
#include <unix_platform.cpp>

inline u16
ReadU16(binary_stream *Source)
{
  u32 Result = (Source->At[0]<<8) + Source->At[1];
  Source->At += sizeof(u16);
  return Result;
}

inline u32
ReadU32(binary_stream *Source)
{
  u32 Result = (Source->At[0]<<24) + (Source->At[1]<<16) + (Source->At[2]<<8) + Source->At[3];
  Source->At += sizeof(u32);
  return Result;
}

struct font_table
{
  u32 Tag;
  u32 Checksum;
  u32 Offset;
  u32 Length;
};

struct ttf
{
  font_table* head; // Font Header
  font_table* cmap; // Character Glyph mapping
  font_table* glyf; // Glyph data
  font_table* hhea; // Horizontal Header
  font_table* htmx; // Horizontal Metrics
  font_table* loca; // Index to Location
  font_table* maxp; // Maximum Profile
  font_table* name; // Naming
  font_table* post; // PostScript
};

struct offset_subtable
{
  u32 ScalerType;
  u16 NumTables;
  u16 SearchRange;
  u16 EntrySelector;
  u16 RangeShift;
};

#define TTF_TAG(s) (s[0]<<24) | (s[1]<<16) | (s[2]<<8) | s[3]

#define AssignTo(prop) \
  case TTF_TAG(#prop): { Assert(!Font->prop); Font->prop = Table; } break;

inline void
AssignTable(font_table *Table, ttf *Font)
{
  switch (Table->Tag)
  {
    AssignTo(head);
    AssignTo(cmap);
    AssignTo(glyf);
    AssignTo(hhea);
    AssignTo(htmx);
    AssignTo(loca);
    AssignTo(maxp);
    AssignTo(name);
    AssignTo(post);

    default:
    {
      Info("Unknown Table encountered");
    } break;
  }
}

inline font_table
ParseFontTable(binary_stream *Source)
{
  font_table Result = {};
  Result.Tag          = ReadU32(Source);
  Result.Checksum     = ReadU32(Source);
  Result.Offset       = ReadU32(Source);
  Result.Length       = ReadU32(Source);

  return Result;
}

offset_subtable
ParseOffsetSubtable(binary_stream *Source)
{
  Assert(Source->At == Source->Start);

  offset_subtable Result = {};

  Result.ScalerType = ReadU32(Source);

  switch (Result.ScalerType)
  {
    case 0x00010000:
    {
    } break;

    // TODO(Jesse): Can we support these?
    case 'true': // Apple encoding
    case 'typ1': // Apple encoding
    case 'OTTO': // OTF 1/2 - Has a CFF Table .. whatever that means
    default:
    {
      Error("Unsupported ? ScalerType encountered in FontTable");
    } break;
  }

  Result.NumTables    = ReadU16(Source);
  Result.SearchRange  = ReadU16(Source);
  Result.EntrySelector= ReadU16(Source);
  Result.RangeShift   = ReadU16(Source);

  return Result;
}

ttf
InitTtf(const char* SourceFile, memory_arena *Arena)
{
  binary_stream Source = BinaryStreamFromFile(SourceFile, Arena);

  offset_subtable TableOffsets = ParseOffsetSubtable(&Source);

  ttf Result = {};
  for (u32 TableIndex = 0;
      TableIndex < TableOffsets.NumTables;
      ++TableIndex)
  {
    font_table CurrentTable = ParseFontTable(&Source);
    AssignTable(&CurrentTable, &Result);
  }


  return Result;
}

int
main()
{
  memory_arena Arena = {};
  ttf Font = InitTtf("roboto_for_powerline.ttf", &Arena);

  return 0;
}
