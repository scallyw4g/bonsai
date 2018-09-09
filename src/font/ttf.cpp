#define BONSAI_NO_MUTEX_TRACKING
#define BONSAI_NO_TIMED_FUNCTIONS
#define BONSAI_NO_PUSH_METADATA
#define BONSAI_NO_DEBUG_MEMORY_ALLOCATOR

#include <bonsai_types.h>
#include <unix_platform.cpp>

inline u16
ReadU16(u8* Source)
{
  u32 Result = (Source[0]<<8) + Source[1];
  return Result;
}

inline u32
ReadU32(u8* Source)
{
  u32 Result = (Source[0]<<24) + (Source[1]<<16) + (Source[2]<<8) + Source[3];
  return Result;
}

inline u16
ReadU16(binary_stream *Source)
{
  u32 Result = ReadU16(Source->At);
  Source->At += sizeof(u16);
  return Result;
}

inline u32
ReadU32(binary_stream *Source)
{
  u32 Result = ReadU32(Source->At);
  Source->At += sizeof(u32);
  return Result;
}


struct simple_glyph
{
  s16 ContourCount;
  s16 xMin;
  s16 yMin;

  s16 xMax;
  s16 yMax;

  u16 EndOfContours;

  u16 InstructionLength;
  u8 *Instructions; // [InstructionLength]

  u8 *Flags;

  union
  {
    u8 xCoord8;
    u16 xCoord16;
  };
  union
  {
    u8 yCoord8;
    u16 yCoord16;
  };
};

struct font_table
{
  u32 Tag;
  char* HumanTag;

  u32 Checksum;
  u32 Offset;
  u32 Length;

  u8* Data;
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

#define PushArray(Type, Count, Arena) \


char *
HumanTag(u32 Tag, memory_arena *Memory)
{
  char* Result = Allocate(char, Memory, 5);
  char* Bin = (char*)&Tag;

  Result[0] = Bin[3];
  Result[1] = Bin[2];
  Result[2] = Bin[1];
  Result[3] = Bin[0];

  return Result;
}

#define TTF_TAG(s) \
  ((s[0]<<24) | (s[1]<<16) | (s[2]<<8) | s[3])

#define AssignTo(prop) \
  case TTF_TAG(#prop): { Assert(!Font->prop); Font->prop = Table; Info("Assigned Table : %s", Table->HumanTag);  } break;

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
      Warn("Unknown Table encountered : %s", Table->HumanTag);
    } break;
  }
}

inline font_table
ParseFontTable(binary_stream *Source, memory_arena *Arena)
{
  font_table Result = {};
  Result.Tag          = ReadU32(Source);
  Result.Checksum     = ReadU32(Source);
  Result.Offset       = ReadU32(Source);
  Result.Length       = ReadU32(Source);

  Result.Data     = Source->Start + Result.Offset;
  Result.HumanTag = HumanTag(Result.Tag, Arena);

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

  Result.NumTables     = ReadU16(Source);
  Result.SearchRange   = ReadU16(Source);
  Result.EntrySelector = ReadU16(Source);
  Result.RangeShift    = ReadU16(Source);

  return Result;
}

u32
CalculateTableChecksum(font_table *Table)
{
  u32 Sum = 0;
  u8* TableData = Table->Data;

  u32 FourByteChunks = (Table->Length + 3) / 4;
  while (FourByteChunks-- > 0)
  {
      Sum += ReadU32(TableData);
      TableData += 4;
  }

  return Sum;
}

ttf
InitTTF(const char* SourceFile, memory_arena *Arena)
{
  binary_stream Source = BinaryStreamFromFile(SourceFile, Arena);

  offset_subtable TableOffsets = ParseOffsetSubtable(&Source);

  ttf Result = {};
  for (u32 TableIndex = 0;
      TableIndex < TableOffsets.NumTables;
      ++TableIndex)
  {
    font_table CurrentTable = ParseFontTable(&Source, Arena);

    u32 Checksum = CalculateTableChecksum(&CurrentTable);
    if (!Checksum == CurrentTable.Checksum)
    {
      Warn("Invalid checksum encountered when reading table %s", CurrentTable.HumanTag);
    }
    else
    {
      AssignTable(&CurrentTable, &Result);
    }
  }


  return Result;
}

int
main()
{
  memory_arena Arena = {};
  ttf Font = InitTTF("roboto_for_powerline.ttf", &Arena);

  return 0;
}
