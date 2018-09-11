#define BONSAI_NO_MUTEX_TRACKING
#define BONSAI_NO_TIMED_FUNCTIONS
#define BONSAI_NO_PUSH_METADATA
#define BONSAI_NO_DEBUG_MEMORY_ALLOCATOR

#include <bonsai_types.h>
#include <unix_platform.cpp>

inline s16
ReadS16(s16* Source)
{
  s16 Result = (((u8*)Source)[0]*256) + ((u8*)Source)[1];
  return Result;
}

inline s16
ReadS16(u8* Source)
{
  s16 Result = (Source[0]*256) + Source[1];
  return Result;
}

inline u16
ReadU16(u16* Source)
{
  u16 Result = (((u8*)Source)[0]*256) + ((u8*)Source)[1];
  return Result;
}

inline u16
ReadU16(u8* Source)
{
  u16 Result = (Source[0]*256) + Source[1];
  return Result;
}

inline u32
ReadU32(u8* Source)
{
  u32 Result = (Source[0]<<24) + (Source[1]<<16) + (Source[2]<<8) + Source[3];
  return Result;
}

inline u8*
ReadU8Array(binary_stream *Source, u32 Count)
{
  u8 *Result = Source->At;
  Source->At += Count;
  return Result;
}

inline u16*
ReadU16Array(binary_stream *Source, u32 Count)
{
  u16 *Result = (u16*)Source->At;
  Source->At += sizeof(u16)*Count;
  return Result;
}

inline s16*
ReadS16Array(binary_stream *Source, u32 Count)
{
  s16 *Result = (s16*)Source->At;
  Source->At += sizeof(s16)*Count;
  return Result;
}

inline s16
ReadS16(binary_stream *Source)
{
  s16 Result = ReadS16(Source->At);
  Source->At += sizeof(s16);
  return Result;
}

inline u16
ReadU16(binary_stream *Source)
{
  u16 Result = ReadU16(Source->At);
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

  // FIXME(Jesse): Appears my accessors for s16 types are fucked?
  /* s16 xMin; */
  /* s16 yMin; */
  /* s16 xMax; */
  /* s16 yMax; */

  u16* EndPointsOfContours;

  u16 InstructionLength;
  u8* Instructions; // [InstructionLength]

  u8* Flags;

  u8* xCoords;
  u8* yCoords;
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

binary_stream
BinaryStream(font_table *Table)
{
  binary_stream Result = BinaryStream(Table->Data, Table->Data+Table->Length);
  return Result;
}

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

inline font_table*
ParseFontTable(binary_stream *Source, memory_arena *Arena)
{
  font_table *Result = Allocate(font_table, Arena, 1);
  Result->Tag          = ReadU32(Source);
  Result->Checksum     = ReadU32(Source);
  Result->Offset       = ReadU32(Source);
  Result->Length       = ReadU32(Source);

  Result->Data     = Source->Start + Result->Offset;
  Result->HumanTag = HumanTag(Result->Tag, Arena);

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
      Error("Unsupported ScalerType encountered in FontTable");
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
    font_table *CurrentTable = ParseFontTable(&Source, Arena);

    u32 Checksum = CalculateTableChecksum(CurrentTable);
    if (Checksum == CurrentTable->Checksum)
    { AssignTable(CurrentTable, &Result); }
    else
    { Error("Invalid checksum encountered when reading table %s", CurrentTable->HumanTag); }
  }


  return Result;
}

enum ttf_flag
{
  TTFFlag_OnCurve = 1 << 0,
  TTFFlag_ShortX  = 1 << 1,
  TTFFlag_ShortY  = 1 << 2,
  TTFFlag_Repeat  = 1 << 3,
  TTFFlag_DualX   = 1 << 4,
  TTFFlag_DualY   = 1 << 5,
};

struct ttf_vert
{
  s16 x;
  s16 y;

  u16 Flags;
};

/* struct encoding_subtable */
/* { */
/*   u16 PlatformId; */
/*   u16 PlatformSpecificId; */
/*   u16 Format; */

/*   u32 Offset; */
/*   u8 *Start; */
/* }; */

/* enum platform_id */
/* { */
/*   PlatformId_Unicode  = 0, */
/*   PlatformId_Mac      = 1, */
/*   PlatformId_Reserved = 2, */
/*   PlatformId_Win32    = 3, */
/* }; */

/* enum platform_specific_id_unicode */
/* { */
/*   UnicodePlatform_Default       = 0, */
/*   UnicodePlatform_V11           = 1, */
/*   UnicodePlatform_ISO10646      = 2, // Deprecated */
/*   UnicodePlatform_V2_BMP        = 3, */
/*   UnicodePlatform_V2_Full       = 4, */
/*   UnicodePlatform_Variation     = 5, */
/*   UnicodePlatform_Full_Coverage = 6, */
/* }; */

simple_glyph
ParseGlyph(binary_stream *Stream, memory_arena *Arena)
{
  simple_glyph Glyph = {};
  Glyph.ContourCount = ReadS16(Stream);

#if 0
  // FIXME(Jesse): Appears my accessors for s16 types are fucked?
  Glyph.xMin                = ReadS16(Stream);
  Glyph.yMin                = ReadS16(Stream);
  Glyph.xMax                = ReadS16(Stream);
  Glyph.yMax                = ReadS16(Stream);
#else
  /* ReadS16(Stream); */
  /* ReadS16(Stream); */
  /* ReadS16(Stream); */
  /* ReadS16(Stream); */
  Stream->At += 4*sizeof(u16);
#endif

  Glyph.EndPointsOfContours = ReadU16Array(Stream, Glyph.ContourCount);

  for (u32 PointIndex = 0;
      PointIndex < Glyph.ContourCount;
      ++PointIndex)
  {
    u16 EndOfContour = ReadU16(Glyph.EndPointsOfContours + PointIndex);
    Print(EndOfContour);
  }

  Glyph.InstructionLength = ReadU16(Stream);
  Glyph.Instructions = ReadU8Array(Stream, Glyph.InstructionLength);

  Glyph.Flags = Stream->At;

  u16 NumContourPoints = ReadU16(Glyph.EndPointsOfContours+Glyph.ContourCount-1);
  ttf_vert *ContourVerts = Allocate(ttf_vert, Arena, NumContourPoints);


  u8* FlagsAt = Glyph.Flags;
  u8* xAt = Glyph.xCoords;
  u8* yAt = Glyph.yCoords;

  b32 RepeatCount = 0;
  u8 Flag = 0;

  for (u32 PointIndex = 0;
      PointIndex < NumContourPoints;
      ++PointIndex)
  {
    if (RepeatCount)
    {
      --RepeatCount;
    }
    else
    {
      Flag = *FlagsAt++;
      if (Flag & TTFFlag_Repeat)
      {
        RepeatCount = *FlagsAt++;
        Assert(PointIndex + RepeatCount < NumContourPoints);
      }
    }

    ContourVerts[PointIndex].Flags = Flag;
  }

  return Glyph;
}

#define DebugCase(platform_id) \
  case platform_id: { Info("Platform Id : %s", #platform_id);

#define UnsupportedPlatform(platform_id) \
  case platform_id: { Error("Unsupported Platform %s", #platform_id); } break;

binary_stream
GetStreamForGlyph(u32 GlyphQueryIndex, ttf *Font)
{
  font_table *Cmap = Font->cmap;
  u32 Checksum = CalculateTableChecksum(Cmap);
  Assert(Checksum == Cmap->Checksum);

  binary_stream CmapStream = BinaryStream(Cmap);
  u16 TableVersion = ReadU16(&CmapStream);
  Assert(TableVersion == 0);

  u16 NumSubtables = ReadU16(&CmapStream);

  for (u32 SubtableIndex = 0;
      SubtableIndex < NumSubtables;
      ++SubtableIndex)
  {
    u16 PlatformId         = ReadU16(&CmapStream);
    u16 PlatformSpecificId = ReadU16(&CmapStream);
    u32 Offset             = ReadU32(&CmapStream);
    u8* Start              = CmapStream.Start + Offset;

    binary_stream TableStream = {};
    TableStream.Start = Start;
    TableStream.At = Start;

    u16 Format = ReadU16(&TableStream);
    if (Format == 4)
    {
      /* RuntimeBreak(); */
      u16 Length        = ReadU16(&TableStream);
      u16 Lang          = ReadU16(&TableStream);
      u16 SegCountX2    = ReadU16(&TableStream);
      u16 SegCount      = SegCountX2/2;
      u16 SearchRange   = ReadU16(&TableStream);
      u16 EntrySelector = ReadU16(&TableStream);
      u16 RangeShift    = ReadU16(&TableStream);

      u16* EndCodes      = ReadU16Array(&TableStream, SegCount);
      u16 Pad            = ReadU16(&TableStream);
      Assert(Pad==0);
      u16* StartCodes    = ReadU16Array(&TableStream, SegCount);
      u16* IdDelta       = ReadU16Array(&TableStream, SegCount);
      u16* IdRangeOffset = ReadU16Array(&TableStream, SegCount);

      for (u32 SegIdx = 0;
          SegIdx < SegCount;
          ++SegIdx)
      {
        u16 Start = ReadU16(StartCodes+SegIdx);
        u16 End = ReadU16(EndCodes+SegIdx);
        u16 Delta = ReadU16(IdDelta+SegIdx);
        u16 RangeOffset = ReadU16(IdRangeOffset+SegIdx);

        /* u16 TestDelta = End - Start; */
        /* Assert(TestDelta == Delta); */
        /* Assert(Start <= End); */

        if (GlyphQueryIndex >= Start && GlyphQueryIndex <= End)
        {
          if (RangeOffset)
          {
            // TODO(Jesse): Does this actually work?
            NotImplemented;

            u32 G = Start + RangeOffset;
            u16 GlyphIndex = ReadU16(IdRangeOffset + SegIdx + G) + Delta;
          }
          else
          {
            // TODO(Jesse): Does overflowing _actually_ wrap correctly here?
            u16 GlyphIndex = Delta + GlyphQueryIndex;
          }

        }

      }

      u16* glyphIdArray = 0;
    }
  }

  u32 Offset = 0;
  u32 Length = 0;

  u8* At = Font->glyf->Data + Offset;

  binary_stream GlyphStream = BinaryStream(At, At+Length );
  return GlyphStream;
}

inline void
DumpGlyphTable(ttf* Font, memory_arena* Arena)
{
  binary_stream GlyphStream = GetStreamForGlyph(42, Font);
  simple_glyph Glyph = ParseGlyph(&GlyphStream, Arena);

  return;
}

int
main()
{
  memory_arena Arena = {};
  ttf Font = InitTTF("roboto_for_powerline.ttf", &Arena);

  DumpGlyphTable(&Font, &Arena);

  return 0;
}
