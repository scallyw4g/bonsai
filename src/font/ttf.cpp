#include <bonsai_types.h>



global_variable v4 White = V4(1,1,1,0);
global_variable v4 Black = {};
global_variable v4 Red   = V4(1,0,0,0);
global_variable v4 Blue  = V4(0,0,1,0);
global_variable v4 Pink  = V4(1,0,1,0);
global_variable v4 Green = V4(0,1,0,0);

global_variable u32 PackedWhite = PackRGBALinearTo255(White );
global_variable u32 PackedBlack = PackRGBALinearTo255(Black );
global_variable u32 PackedRed   = PackRGBALinearTo255(Red   );
global_variable u32 PackedBlue  = PackRGBALinearTo255(Blue  );
global_variable u32 PackedPink  = PackRGBALinearTo255(Pink  );
global_variable u32 PackedGreen = PackRGBALinearTo255(Green );


inline u8
ReadU8(u8* Source)
{
  u8 Result = Source[0];
  return Result;
}

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

inline s64
ReadS64(u8* Source)
{
  s64 Result = (s64)( ((u64)Source[0]<<56) + ((u64)Source[1]<<48) + ((u64)Source[2]<<40) + ((u64)Source[3]<<32) + ((u64)Source[4]<<24) + ((u64)Source[5]<<16) + ((u64)Source[6]<<8) + ((u64)Source[7]) );
  return Result;
}

inline u32
ReadU32(u8* Source)
{
  u32 Result = (u32)( (Source[0]<<24) + (Source[1]<<16) + (Source[2]<<8) + Source[3] );
  return Result;
}

inline u8*
ReadU8Array(u8_stream *Source, u32 Count)
{
  u8 *Result = Source->At;
  Source->At += Count;
  Assert(Source->At <= Source->End);
  return Result;
}

inline u16*
ReadU16Array(u8_stream *Source, u32 Count)
{
  u16 *Result = (u16*)Source->At;
  Source->At += sizeof(u16)*Count;
  Assert(Source->At <= Source->End);
  return Result;
}

inline s16*
ReadS16Array(u8_stream *Source, u32 Count)
{
  s16 *Result = (s16*)Source->At;
  Source->At += sizeof(s16)*Count;
  Assert(Source->At <= Source->End);
  return Result;
}

inline u8
ReadU8(u8_stream *Source)
{
  u8 Result = ReadU8(Source->At);
  Source->At += sizeof(u8);
  Assert(Source->At <= Source->End);
  return Result;
}

inline s16
ReadS16(u8_stream *Source)
{
  s16 Result = ReadS16(Source->At);
  Source->At += sizeof(s16);
  Assert(Source->At <= Source->End);
  return Result;
}

inline u16
ReadU16(u8_stream *Source)
{
  u16 Result = ReadU16(Source->At);
  Source->At += sizeof(u16);
  Assert(Source->At <= Source->End);
  return Result;
}

inline s64
ReadS64(u8_stream *Source)
{
  s64 Result = ReadS64(Source->At);
  Source->At += sizeof(s64);
  Assert(Source->At <= Source->End);
  return Result;
}

inline u32
ReadU32(u8_stream *Source)
{
  u32 Result = ReadU32(Source->At);
  Source->At += sizeof(u32);
  Assert(Source->At <= Source->End);
  return Result;
}

struct head_table
{
  // Technically the spec says these are 32bit fixed point numbers, but IDC
  // because I never use them
  u32 Version;
  u32 FontRevision;

  u32 ChecksumAdjustment;

  u32 MagicNumber;
  u16 Flags;
  u16 UnitsPerEm;

  s64 Created;
  s64 Modified;

  s16 xMin;
  s16 yMin;
  s16 xMax;
  s16 yMax;

  u16 MacStyle;
  u16 LowestRecPPEM;
  u16 FontDirectionHint;
  u16 IndexToLocFormat;
  u16 GlyphDataFormat;
};

struct ttf_vert
{
  v2i P;
  u16 Flags;
};

struct ttf_contour
{
  u32 StartIndex;
  u32 EndIndex;
};

struct simple_glyph
{
  v2i MinP;
  v2i EmSpaceDim;

  s16 ContourCount;
  ttf_contour* Contours;

  s16 VertCount;
  ttf_vert* Verts;
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
  head_table* HeadTable;

  font_table* cmap; // Character Glyph mapping
  font_table* glyf; // Glyph data
  font_table* hhea; // Horizontal Header
  font_table* htmx; // Horizontal Metrics
  font_table* loca; // Index to Location
  font_table* maxp; // Maximum Profile
  font_table* name; // Naming
  font_table* post; // PostScript

  b32 Loaded;
};

struct offset_subtable
{
  u32 ScalerType;
  u16 NumTables;
  u16 SearchRange;
  u16 EntrySelector;
  u16 RangeShift;

  b32 Valid;
};

enum ttf_flag
{
  TTFFlag_OnCurve = 1 << 0,
  TTFFlag_ShortX  = 1 << 1,
  TTFFlag_ShortY  = 1 << 2,
  TTFFlag_Repeat  = 1 << 3,
  TTFFlag_DualX   = 1 << 4,
  TTFFlag_DualY   = 1 << 5,
};

u8_stream
U8_Stream(font_table *Table)
{
  u8_stream Result = U8_Stream(Table->Data, Table->Data+Table->Length);
  return Result;
}

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
ParseFontTable(u8_stream *Source, memory_arena *Arena)
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
ParseOffsetSubtable(u8_stream *Source, memory_arena* Arena)
{
  Assert(Source->At == Source->Start);

  offset_subtable Result = {};

  Result.ScalerType = ReadU32(Source);

  switch (Result.ScalerType)
  {
    case 0x00010000:
    {
      Result.Valid = True;
    } break;

    // TODO(Jesse, id: 140, tags: font, ttf_rasterizer, completeness): Can we support these?
    case 'true': // Apple encoding
    case 'typ1': // Apple encoding
    case 'OTTO': // OTF 1/2 - Has a CFF Table .. whatever that means
    {
      Error("Unsupported ScalerType encountered in FontTable : %s", HumanTag(Result.ScalerType, Arena) );
    } break;

    InvalidDefaultCase;
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
  ttf Result = {};
  u8_stream Source = U8_StreamFromFile(SourceFile, Arena);

  if (Source.Start)
  {
    offset_subtable TableOffsets = ParseOffsetSubtable(&Source, Arena);

    if (TableOffsets.Valid)
    {
      for (u32 TableIndex = 0;
          TableIndex < TableOffsets.NumTables;
          ++TableIndex)
      {
        font_table *CurrentTable = ParseFontTable(&Source, Arena);

        u32 Checksum = CalculateTableChecksum(CurrentTable);
        if (Checksum != CurrentTable->Checksum) { Error("Invalid checksum encountered when reading table %s", CurrentTable->HumanTag); }

        AssignTable(CurrentTable, &Result);
      }

      Result.Loaded = True;
    }
  }

  return Result;
}

simple_glyph
ParseGlyph(u8_stream *Stream, memory_arena *Arena)
{
  simple_glyph Glyph = {};

  Glyph.ContourCount = ReadS16(Stream);
  if (Glyph.ContourCount > 0) // We don't support compound glyphs, yet
  {
    Glyph.Contours = Allocate(ttf_contour, Arena, Glyph.ContourCount);
    s16 xMin = ReadS16(Stream);
    s16 yMin = ReadS16(Stream);
    s16 xMax = ReadS16(Stream);
    s16 yMax = ReadS16(Stream);

    Glyph.MinP = {{ xMin, yMin }};

    Glyph.EmSpaceDim.x = xMax - xMin + 1; // Add one to put from 0-based to 1-based
    Glyph.EmSpaceDim.y = yMax - yMin + 1; // coordinate system

    u16 *EndPointsOfContours = ReadU16Array(Stream, (u32)Glyph.ContourCount);

    u16 NextStart = 0;
    for (u16 ContourIndex = 0;
        ContourIndex < Glyph.ContourCount;
        ++ContourIndex)
    {
      Glyph.Contours[ContourIndex].StartIndex = NextStart;
      Glyph.Contours[ContourIndex].EndIndex = ReadU16(EndPointsOfContours + ContourIndex);
      NextStart = SafeTruncateToU16(Glyph.Contours[ContourIndex].EndIndex + 1);
    }

    u16 InstructionLength = ReadU16(Stream);

    /* u8* Instructions = */ ReadU8Array(Stream, InstructionLength);

    u8* Flags = Stream->At;
    u8* FlagsAt = Flags;

    Glyph.VertCount = (s16)(1 + ReadU16(EndPointsOfContours+Glyph.ContourCount-1));
    Glyph.Verts = Allocate(ttf_vert, Arena, Glyph.VertCount);

    s16 RepeatCount = 0;
    u8 Flag = 0;

    for (s16 PointIndex = 0;
        PointIndex < Glyph.VertCount;
        ++PointIndex)
    {
      if (RepeatCount)
      {
        --RepeatCount;
      }
      else
      {
        Flag = *FlagsAt++;
        Assert((Flag & 64) == 0);
        Assert((Flag & 128) == 0);
        if (Flag & TTFFlag_Repeat)
        {
          RepeatCount = *FlagsAt++;
          Assert(PointIndex + RepeatCount < Glyph.VertCount);
        }
      }

      Glyph.Verts[PointIndex].Flags = Flag;
    }

    u8_stream VertStream = U8_Stream(FlagsAt, (u8*)0xFFFFFFFFFFFFFFFF);
    s16 X = 0;
    for (s16 PointIndex = 0;
        PointIndex < Glyph.VertCount;
        ++PointIndex)
    {
      ttf_vert *Vert = Glyph.Verts + PointIndex;
      if (Vert->Flags & TTFFlag_ShortX)
      {
        u16 Delta = ReadU8(&VertStream);
        X += (Vert->Flags & TTFFlag_DualX) ? Delta : -Delta;
      }
      else
      {
        if (!(Vert->Flags & TTFFlag_DualX))
        {
          X += ReadU16(&VertStream);
        }
      }

      Vert->P.x = X - xMin;
      Assert(Vert->P.x >= 0);
      Assert(Vert->P.x <= Glyph.EmSpaceDim.x);
    }

    s16 Y = 0;
    for (s16 PointIndex = 0;
        PointIndex < Glyph.VertCount;
        ++PointIndex)
    {
      ttf_vert *Vert = Glyph.Verts + PointIndex;
      if (Vert->Flags & TTFFlag_ShortY)
      {
        u16 Delta = ReadU8(&VertStream);
        Y += (Vert->Flags & TTFFlag_DualY) ? Delta : -Delta;
      }
      else
      {
        if (!(Vert->Flags & TTFFlag_DualY))
        {
          Y += ReadU16(&VertStream);
        }
      }

      Vert->P.y = Y - yMin;
      Assert(Vert->P.y >= 0);
      Assert(Vert->P.y <= Glyph.EmSpaceDim.y);
    }
  }

  return Glyph;
}

/* #define DebugCase(platform_id) \ */
/*   case platform_id: { Info("Platform Id : %s", #platform_id); */

/* #define UnsupportedPlatform(platform_id) \ */
/*   case platform_id: { Error("Unsupported Platform %s", #platform_id); } break; */

u16
GetGlyphIdForCharacterCode(u32 UnicodeCodepoint, ttf *Font)
{
  font_table *Cmap = Font->cmap;
  u32 Checksum = CalculateTableChecksum(Cmap);
  Assert(Checksum == Cmap->Checksum);

  u8_stream CmapStream = U8_Stream(Cmap);
  u16 TableVersion = ReadU16(&CmapStream);
  Assert(TableVersion == 0);

  u16 NumSubtables = ReadU16(&CmapStream);

  for (u32 SubtableIndex = 0;
      SubtableIndex < NumSubtables;
      ++SubtableIndex)
  {
    /* u16 PlatformId         = */ ReadU16(&CmapStream);
    /* u16 PlatformSpecificId = */ ReadU16(&CmapStream);
    u32 Offset             = ReadU32(&CmapStream);
    u8* Start              = CmapStream.Start + Offset;

    u8_stream TableStream = {};
    TableStream.Start = Start;
    TableStream.At = Start;
    TableStream.End = Start+4;

    u16 Format = ReadU16(&TableStream);
    u16 Length = ReadU16(&TableStream);

    TableStream.End = Start+Length;
    if (Format == 4)
    {
      /* u16 Lang          = */ ReadU16(&TableStream);
      u16 SegCountX2    = ReadU16(&TableStream);
      u16 SegCount      = SegCountX2/2;
      /* u16 SearchRange   = */ ReadU16(&TableStream);
      /* u16 EntrySelector = */ ReadU16(&TableStream);
      /* u16 RangeShift    = */ ReadU16(&TableStream);

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
        u16 StartCode = ReadU16(StartCodes+SegIdx);
        u16 End = ReadU16(EndCodes+SegIdx);
        u16 Delta = ReadU16(IdDelta+SegIdx);
        u16 RangeOffset = ReadU16(IdRangeOffset+SegIdx);

        if (UnicodeCodepoint >= StartCode && UnicodeCodepoint <= End)
        {
          if (RangeOffset)
          {
            u16 GlyphIndex = ReadU16( &IdRangeOffset[SegIdx] + RangeOffset / 2 + (UnicodeCodepoint - StartCode) );
            u16 Result = GlyphIndex ? GlyphIndex + Delta : GlyphIndex;
            return Result;
          }
          else
          {
            u32 GlyphIndex = Delta + UnicodeCodepoint;
            return GlyphIndex & 0xFFFF;
          }

        }

      }
    }
  }

  return 0;
}

inline head_table*
ParseHeadTable(u8_stream *Stream, memory_arena *Arena)
{
  head_table *Result = Allocate(head_table, Arena, 1);

  Result->Version = ReadU32(Stream);
  Assert(Result->Version == 0x00010000);

  Result->FontRevision       = ReadU32(Stream);

  Result->ChecksumAdjustment = ReadU32(Stream);
  Result->MagicNumber        = ReadU32(Stream);
  Assert(Result->MagicNumber == 0x5F0F3CF5);

  Result->Flags             = ReadU16(Stream);
  Result->UnitsPerEm        = ReadU16(Stream);
  Result->Created           = ReadS64(Stream);
  Result->Modified          = ReadS64(Stream);
  Result->xMin              = ReadS16(Stream);
  Result->yMin              = ReadS16(Stream);
  Result->xMax              = ReadS16(Stream);
  Result->yMax              = ReadS16(Stream);
  Result->MacStyle          = ReadU16(Stream);
  Result->LowestRecPPEM     = ReadU16(Stream);
  Result->FontDirectionHint = ReadU16(Stream);
  Result->IndexToLocFormat  = ReadU16(Stream);
  Result->GlyphDataFormat   = ReadU16(Stream);

  Assert(Stream->At == Stream->End);

  return Result;
}

#define SHORT_INDEX_LOCATION_FORMAT 0
#define LONG_INDEX_LOCATION_FORMAT 1

inline u8_stream
GetStreamForGlyphIndex(u32 GlyphIndex, ttf *Font)
{
  head_table *HeadTable = Font->HeadTable;

  u8_stream Result = {};
  if (HeadTable->IndexToLocFormat == SHORT_INDEX_LOCATION_FORMAT)
  {
    u32 GlyphIndexOffset = GlyphIndex * sizeof(u16);
    u32 StartOffset = ReadU16(Font->loca->Data + GlyphIndexOffset) *2;
    u32 EndOffset = ReadU16(Font->loca->Data + GlyphIndexOffset + sizeof(u16)) *2;
    Assert(StartOffset <= EndOffset);

    u8* Start = Font->glyf->Data + StartOffset;
    u8* End = Font->glyf->Data + EndOffset;
    Result = U8_Stream(Start, End);
  }
  else if (HeadTable->IndexToLocFormat == LONG_INDEX_LOCATION_FORMAT)
  {
    /* u32 FirstOffset = */ ReadU32(Font->loca->Data);
    /* u32 FirstEndOffset = */ ReadU32(Font->loca->Data+1);


    u32 StartOffset = ReadU32(Font->loca->Data+(GlyphIndex*4));
    u32 EndOffset = ReadU32(Font->loca->Data+(GlyphIndex*4)+4);

    u8* Start = Font->glyf->Data + StartOffset;
    u8* End = Font->glyf->Data + EndOffset;
    Result = U8_Stream(Start, End);
  }
  else
  {
    InvalidCodePath();
  }

  return Result;
}

inline u32
GetPixelIndex(v2i PixelP, bitmap* Bitmap)
{
  Assert(PixelP.x < Bitmap->Dim.x);
  Assert(PixelP.y < Bitmap->Dim.y);
  u32 Result = (u32)(PixelP.x + (s32)(PixelP.y*Bitmap->Dim.x));
  Assert(Result < (u32)PixelCount(Bitmap));
  return Result;
}

u32
WrapToCurveIndex(u32 IndexWanted, u32 CurveStart, u32 CurveEnd)
{
  u32 Result = IndexWanted;
  if (Result > CurveEnd)
  {
    u32 Remaining = Result - CurveEnd;
    Result = CurveStart + Remaining - 1;
  }
  return Result;
}

bitmap
RasterizeGlyph(v2i OutputSize, v2i FontMaxEmDim, v2i FontMinGlyphP, u8_stream *GlyphStream, memory_arena* Arena)
{
#define DO_RASTERIZE        1
#define DO_AA               1
#define WRITE_DEBUG_BITMAPS 0

  s32 SamplesPerPixel = 4;
  v2i SamplingBitmapDim = OutputSize*SamplesPerPixel;
  bitmap SamplingBitmap = AllocateBitmap(SamplingBitmapDim, Arena);

  bitmap OutputBitmap = {};
  if (Remaining(GlyphStream) > 0) // A glyph stream with 0 length means there's no glyph
  {

    simple_glyph Glyph = ParseGlyph(GlyphStream, Arena);
    if (Glyph.ContourCount > 0) // We don't support compound glyphs, yet
    {
      FillBitmap(PackRGBALinearTo255(White), &SamplingBitmap);

      OutputBitmap = AllocateBitmap(OutputSize, Arena);
      FillBitmap(PackRGBALinearTo255(White), &OutputBitmap);


      for (u16 ContourIndex = 0;
          ContourIndex < Glyph.ContourCount;
          ++ContourIndex)
      {
        ttf_contour* Contour = Glyph.Contours + ContourIndex;

        u32 ContourVertCount = Contour->EndIndex - Contour->StartIndex;
        u32 AtIndex = Contour->StartIndex;

        u32 VertsProcessed = 0;
        while (VertsProcessed <= ContourVertCount)
        {
          Assert(Glyph.Verts[AtIndex].Flags & TTFFlag_OnCurve);

          u32 VertCount = 1;
          u32 CurveEndIndex = WrapToCurveIndex(AtIndex+1, Contour->StartIndex, Contour->EndIndex);
          while ( !(Glyph.Verts[CurveEndIndex].Flags & TTFFlag_OnCurve) )
          {
            CurveEndIndex = WrapToCurveIndex(CurveEndIndex+1, Contour->StartIndex, Contour->EndIndex);
            ++VertCount;
          }
          ++VertCount;



          v2* TempVerts = Allocate(v2, Arena, VertCount); // TODO(Jesse, id: 141, tags: transient_memory, ttf_rasterizer): Temp-memory?

          v2i BaselineOffset = Glyph.MinP - FontMinGlyphP;

          // Here we have to subtract one from everything because we're going
          // from a dimension [1, Dim] to an indexable range [0, Dim-1]
          //
          // TODO(Jesse, id: 142, tags: open_question): Is there a better way of making that adjustment?
          v2i One = V2i(1,1);
          v2 EmScale = V2(Glyph.EmSpaceDim-One) / V2(FontMaxEmDim-One);
          v2 EmSpaceToPixelSpace = EmScale*( (SamplingBitmapDim-One) / (Glyph.EmSpaceDim-One) );

          v4 LastColor = Red;
          u32 LastPixelIndex = 0;
          for (r32 t = 0.0f;
              t < 1.0f;
              t += 0.0001f)
          {

            for (u32 VertIndex = 0;
                VertIndex < VertCount;
                ++VertIndex)
            {
              u32 CurveVertIndex = WrapToCurveIndex(AtIndex + VertIndex, Contour->StartIndex, Contour->EndIndex);
              TempVerts[VertIndex] = V2(Glyph.Verts[CurveVertIndex].P + BaselineOffset) * EmSpaceToPixelSpace;
            }

            v2 TangentMax = {};
            for (u32 Outer = VertCount;
                Outer > 1;
                --Outer)
            {
              for (u32 Inner = 0;
                  Inner < Outer-1;
                  ++Inner)
              {
                v2 tVec01 = (TempVerts[Inner+1]-TempVerts[Inner]) * t;
                TempVerts[Inner] = TempVerts[Inner+1] - tVec01;
                if (Inner == Outer-2)
                {
                  TangentMax = TempVerts[Inner+1];
                }
              }
            }

            v2 PointOnCurve = TempVerts[0];
            v4 Color = Red;

            // On-curve transition
            if ( (TangentMax.x >= PointOnCurve.x && TangentMax.y > PointOnCurve.y) ||
                 (TangentMax.x <= PointOnCurve.x && TangentMax.y > PointOnCurve.y) )
            {
              Color = Green;
            }
            else
            if ( (TangentMax.x >= PointOnCurve.x && TangentMax.y <= PointOnCurve.y) ||
                 (TangentMax.x <= PointOnCurve.x && TangentMax.y <= PointOnCurve.y) )
            {
              Color = Blue;
            }

            u32 PixelIndex = GetPixelIndex(V2i(PointOnCurve), &SamplingBitmap);
            SamplingBitmap.Pixels.Start[PixelIndex] = PackRGBALinearTo255(Color);

            if (LastColor == Green && Color == Blue)
            {
              SamplingBitmap.Pixels.Start[PixelIndex] = PackRGBALinearTo255(Green);
            }

            if (LastColor == Blue && Color == Green)
            {
              SamplingBitmap.Pixels.Start[LastPixelIndex] = PackRGBALinearTo255(Green);
            }

            LastColor = Color;
            LastPixelIndex = PixelIndex;
          }

          VertsProcessed += VertCount -1;
          AtIndex = CurveEndIndex;
        }
      }

#if DO_RASTERIZE
      for (s32 yIndex = 0;
          yIndex < SamplingBitmap.Dim.y;
          ++yIndex)
      {
        s32 TransitionCount = 0;
        for (s32 xIndex = 0;
            xIndex < SamplingBitmap.Dim.x;
            ++xIndex)
        {
          u32 PixelIndex = GetPixelIndex(V2i(xIndex, yIndex), &SamplingBitmap);
          u32 PixelColor = SamplingBitmap.Pixels.Start[PixelIndex];
          if (PixelColor == PackedGreen)
          {
            TransitionCount = 1;
            continue;
          }
          else if (PixelColor == PackedBlue)
          {
            TransitionCount = 0;
            continue;
          }

          if (TransitionCount > 0)
          {
            SamplingBitmap.Pixels.Start[PixelIndex] = PackedBlack;
          }
          else
          {
            SamplingBitmap.Pixels.Start[PixelIndex] = PackedWhite;
          }
        }
      }
#endif

#if DO_RASTERIZE && DO_AA
      r32 SampleContrib = 1.0f/((r32)SamplesPerPixel*(r32)SamplesPerPixel);
      for (s32 yPixelIndex = 0;
          yPixelIndex < OutputBitmap.Dim.y;
          ++yPixelIndex)
      {
        for (s32 xPixelIndex = 0;
            xPixelIndex < OutputBitmap.Dim.x;
            ++xPixelIndex)
        {
          r32 OneMinusAlpha = 0.0f;

          s32 yStart = yPixelIndex*SamplesPerPixel;
          for (s32 ySampleIndex = yStart;
              ySampleIndex < yStart+SamplesPerPixel;
              ++ySampleIndex)
          {
            s32 xStart = xPixelIndex*SamplesPerPixel;
            for (s32 xSampleIndex = xStart;
                xSampleIndex < xStart+SamplesPerPixel;
                ++xSampleIndex)
            {
              if (xSampleIndex < SamplingBitmap.Dim.x && ySampleIndex < SamplingBitmap.Dim.y)
              {
                u32 SampleIndex = GetPixelIndex(V2i(xSampleIndex, ySampleIndex), &SamplingBitmap);
                u32 PixelColor = SamplingBitmap.Pixels.Start[SampleIndex];
                OneMinusAlpha += PixelColor == PackedWhite ? SampleContrib : 0.0f;
              }
            }
          }

          u32 PixelIndex = GetPixelIndex(V2i(xPixelIndex, yPixelIndex), &OutputBitmap);
          r32 Alpha = 1.0f - OneMinusAlpha;

          OutputBitmap.Pixels.Start[PixelIndex] = PackRGBALinearTo255(V4(1.0f, 1.0f, 1.0f, Alpha));

          if (xPixelIndex == (OutputBitmap.Dim.x-1) || yPixelIndex == (OutputBitmap.Dim.y-1))
          {
            OutputBitmap.Pixels.Start[PixelIndex] = PackedRed;
          }
        }
      }
#endif

#if 0
      v2 At = V2(Glyph.Verts->P);
      v2 LastVertP = At;

      v4 CurrentColor = (Glyph.Verts->Flags&TTFFlag_OnCurve) ?  Black : Red;
      v4 TargetColor = Black;

      for (u32 VertIndex = 0;
          VertIndex < Glyph.VertCount;
          ++VertIndex)
      {
        ttf_vert *Vert = Glyph.Verts + VertIndex;
        v2 VertP = V2(Vert->P);

        if (Vert->Flags & TTFFlag_OnCurve)
        {
          TargetColor = Black;
          Debug("On  Curve %d %d", Vert->P.x, Vert->P.y);
        }
        else
        {
          TargetColor = Red;
          Debug("Off Curve %d %d", Vert->P.x, Vert->P.y);
        }


        v2 CurrentToVert = Normalize(VertP-At) * 0.5f;
        while(Abs(Length(VertP - At)) > 0.5f)
        {
          r32 t = SafeDivide0(LengthSq(LastVertP-At), LengthSq(LastVertP-VertP));
          u32 PixelColor = PackRGBALinearTo255(Lerp(t, CurrentColor, TargetColor));
          u32 PixelIndex = GetPixelIndex(V2i(At), &Bitmap);
          *(Bitmap.Pixels.Start + PixelIndex) = PixelColor;
          At += CurrentToVert;
        }

        LastVertP = VertP;
        CurrentColor = TargetColor;
      }
#endif
    }

#if WRITE_DEBUG_BITMAPS
    WriteBitmapToDisk(&SamplingBitmap, "sample_glyph.bmp");
    WriteBitmapToDisk(&OutputBitmap, "output_glyph.bmp");
#endif
  }

  return OutputBitmap;
}

void
CopyBitmapOffset(bitmap *Source, bitmap *Dest, v2i Offset)
{
  for (s32 ySourcePixel = 0;
      ySourcePixel < Source->Dim.y;
      ++ySourcePixel)
  {
    for (s32 xSourcePixel = 0;
        xSourcePixel < Source->Dim.x;
        ++xSourcePixel)
    {
      u32 SourcePixelIndex = GetPixelIndex(V2i(xSourcePixel, ySourcePixel), Source);
      u32 DestPixelIndex = GetPixelIndex( V2i(xSourcePixel, ySourcePixel) + Offset, Dest);

      Dest->Pixels.Start[DestPixelIndex] = Source->Pixels.Start[SourcePixelIndex] ;
    }
  }

  return;
}

int
main()
{
  const char* FontName = "fonts/Anonymice/Anonymice Nerd Font Complete Mono Windows Compatible.ttf";

  memory_arena* PermArena = AllocateArena();
  memory_arena* TempArena = AllocateArena();

  ttf Font = InitTTF(FontName, PermArena);

  if (Font.Loaded)
  {
    u8_stream HeadStream = U8_Stream(Font.head);
    Font.HeadTable = ParseHeadTable(&HeadStream, PermArena);
    v2i FontMaxEmDim = {{ Font.HeadTable->xMax - Font.HeadTable->xMin, Font.HeadTable->yMax - Font.HeadTable->yMin }};
    v2i FontMinGlyphP = V2i(Font.HeadTable->xMin, Font.HeadTable->yMin);

    v2i GlyphSize = V2i(32, 32);

    bitmap TextureAtlasBitmap = AllocateBitmap(16*GlyphSize, PermArena);

    u32 AtlasCount = 65536/256;
    for (u32 AtlasIndex = 0;
    AtlasIndex < AtlasCount;
    ++AtlasIndex)
    {
      FillBitmap(PackedPink, &TextureAtlasBitmap);

      u32 GlyphsRasterized = 0;
      for (u32 CharCode = AtlasIndex*256;
          CharCode < (AtlasIndex*256)+256;
          ++CharCode)
      {
        u32 GlyphIndex = GetGlyphIdForCharacterCode(CharCode, &Font);
        if (!GlyphIndex) continue;
        u8_stream GlyphStream = GetStreamForGlyphIndex(GlyphIndex, &Font);
        bitmap GlyphBitmap = RasterizeGlyph(GlyphSize, FontMaxEmDim, FontMinGlyphP, &GlyphStream, TempArena);

        if ( PixelCount(&GlyphBitmap) )
        {
          DebugLine("Rasterized Glyph %d (%d)", CharCode, GlyphsRasterized);
          ++GlyphsRasterized;

#if 1
          v2 UV = GetUVForCharCode((u8)(CharCode % 256));
          CopyBitmapOffset(&GlyphBitmap, &TextureAtlasBitmap, V2i(UV*V2(TextureAtlasBitmap.Dim)) );
#else
          char Name[128] = {};
          FormatCountedString_(Name, 128, "Glyph_%d.bmp", CharCode);
          WriteBitmapToDisk(&GlyphBitmap, Name);
#endif
        }

        RewindArena(TempArena);
      }

      if (GlyphsRasterized)
      {
        counted_string AtlasName = FormatCountedString(TempArena, CSz("texture_atlas_%d.bmp"), AtlasIndex);
        // TODO(Jesse, id: 143, tags: robustness, open_question, format_counted_string_api): This could probably be made better by writing to a statically allocated buffer ..?
        WriteBitmapToDisk(&TextureAtlasBitmap, GetNullTerminated(AtlasName));
      }

      GlyphsRasterized = 0;
    }

  }
  else
  {
    Error("Loading Font %s", FontName);
  }

  return 0;
}
