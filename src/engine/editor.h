#define EDITOR_UI_VALUE_RANGE_PROTO_DEFAULTS r32 MinValue = 0.f, r32 MaxValue = 1.f
#define EDITOR_UI_VALUE_RANGE_INSTANCE_NAMES MinValue, MaxValue

poof(
  func do_editor_ui_for_vector_type(type_poof_symbol type_list)
  {
    type_list.map(type)
    {
      link_internal void
      DoEditorUi(renderer_2d *Ui, type.name *Value, cs Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS, EDITOR_UI_VALUE_RANGE_PROTO_DEFAULTS)
      {
        type.member(0, (E) 
        {
          /* PushTableStart(Ui); */
            if (Name) { PushColumn(Ui, CS(Name), EDITOR_UI_FUNCTION_INSTANCE_NAMES); }

            if (Value)
            {
              u32 Start = StartColumn(Ui, EDITOR_UI_FUNCTION_INSTANCE_NAMES);
                PushTableStart(Ui);
                  E.map_array(e_index)
                  {
                    DoEditorUi(Ui, &Value->(E.name)[e_index], {}, EDITOR_UI_FUNCTION_INSTANCE_NAMES, EDITOR_UI_VALUE_RANGE_INSTANCE_NAMES );
                  }
                PushTableEnd(Ui);
                /* PushNewRow(Ui); */
              EndColumn(Ui, Start);
            }
          /* PushTableEnd(Ui); */
        })
        PushNewRow(Ui);
      }
    }
  }
)

poof(
  func do_editor_ui_for_primitive_type(type_poof_symbol type_list)
  {
    type_list.map(type)
    {
      link_internal void
      DoEditorUi(renderer_2d *Ui, type.name *Value, cs Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS, EDITOR_UI_VALUE_RANGE_PROTO_DEFAULTS)
      {
        if (Name) { PushColumn(Ui,    Name, EDITOR_UI_FUNCTION_INSTANCE_NAMES); }

        if (Value)
        {
          u32 Start = StartColumn(Ui, EDITOR_UI_FUNCTION_INSTANCE_NAMES);
            PushTableStart(Ui);
              if (Button(Ui, CSz("-"), UiId(Value, "decrement"), EDITOR_UI_FUNCTION_INSTANCE_NAMES)) { *Value = *Value - 1; }
                  PushColumn(Ui, CS(*Value), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
              if (Button(Ui, CSz("+"), UiId(Value, "increment"), EDITOR_UI_FUNCTION_INSTANCE_NAMES)) { *Value = *Value + 1; }
            PushTableEnd(Ui);
          EndColumn(Ui, Start);
        }
        else
        {
          PushColumn(Ui, CSz("(null)"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
          PushNewRow(Ui);
        }

      }

      link_internal void
      DoEditorUi(renderer_2d *Ui, volatile type.name *Value, cs Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
      {
        DoEditorUi(Ui, ((type.name)*) Value, Name, EDITOR_UI_FUNCTION_INSTANCE_NAMES);
      }

    }
  }
)

poof(
  func do_editor_ui_for_compound_type(type)
  {
    link_internal void
    DoEditorUi(renderer_2d *Ui, type.name *Element, cs Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
    {
      if (Element)
      {
        if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Element, Name), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
        {
          PushNewRow(Ui);

          PushTableStart(Ui);
          PushForceUpdateBasis(Ui, V2(20.f, 0.f));
            type.map(member)
            {
              member.has_tag(ui_skip)?
              {
              }
              {
                member.is_array?
                {
                  RangeIterator(ArrayIndex, member.array)
                  {
                    DoEditorUi(Ui, Element->(member.name)+ArrayIndex, CSz("member.type member.name"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
                    member.is_primitive?  { PushNewRow(Ui); }
                  }
                }
                {
                  member.is_pointer?
                  {
                    DoEditorUi(Ui, Element->(member.name), CSz("member.type member.name"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
                  }
                  {
                    member.has_tag(ui_value_range)?
                    {
                      DoEditorUi(Ui, &Element->(member.name), CSz("member.type member.name"), EDITOR_UI_FUNCTION_INSTANCE_NAMES, member.tag_value(ui_value_range));
                    }
                    {
                      member.is_type(b32)?
                      {
                        DoEditorUi(Ui, Cast(b8*, &Element->(member.name)), CSz("member.type member.name"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
                      }
                      {
                        DoEditorUi(Ui, &Element->(member.name), CSz("member.type member.name"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
                      }
                    }
                  }
                }
              }

              member.is_primitive?
              {
                PushNewRow(Ui);
              }
            }
          PushForceUpdateBasis(Ui, V2(-20.f, 0.f));
          PushTableEnd(Ui);
        }

        PushNewRow(Ui);
      }
      else
      {
        PushColumn(Ui, Name, EDITOR_UI_FUNCTION_INSTANCE_NAMES);
        PushColumn(Ui, CSz("(null)"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
        PushNewRow(Ui);
      }

    }
  }
)

poof(
  func do_editor_ui_for_enum(enum_t)
  {
    link_internal void
    DoEditorUi(renderer_2d *Ui, enum_t.name *Element, cs Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
    {
      if (Name) { PushColumn(Ui, CS(Name), EDITOR_UI_FUNCTION_INSTANCE_NAMES); }

      cs ElementName = ToString(*Element);
      if (ToggleButton(Ui, ElementName, ElementName, UiId(Element, "enum value.type value.name"), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
      {
        PushNewRow(Ui);
        enum_t.map(value)
        {
          PushColumn(Ui, CSz("")); // Skip the first Name column
          if (Button(Ui, CSz("value.name"), UiId(Element, "enum value.type value.name"), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
          {
            *Element = value.name;
          }
          PushNewRow(Ui);
        }
      }
      else
      {
        PushNewRow(Ui);
      }
    }
  }
)

poof(
  func do_editor_ui_for_container(type)
  {
    link_internal void
    DoEditorUi(renderer_2d *Ui, type.name *Container, cs Name, EDITOR_UI_FUNCTION_PROTO_DEFAULTS)
    {
      if (Container)
      {
        if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Container, Name), EDITOR_UI_FUNCTION_INSTANCE_NAMES))
        {
          PushNewRow(Ui);
          IterateOver(Container, Element, ElementIndex)
          {
            DoEditorUi(Ui, Element, CS(ElementIndex), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
            PushNewRow(Ui);
          }
        }
        PushNewRow(Ui);
      }
      else
      {
        PushColumn(Ui, FSz("%S", Name), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
        PushColumn(Ui, CSz("(null)"), EDITOR_UI_FUNCTION_INSTANCE_NAMES);
      }
    }
  }
)

link_internal void
DoEditorUi(renderer_2d *Ui, aabb *Element, cs Name, EDITOR_UI_FUNCTION_PROTO_ARGUMENTS);

/* link_internal void */
/* DoEditorUi(renderer_2d *Ui, v3i *Element, cs Name, EDITOR_UI_FUNCTION_PROTO_ARGUMENTS); */

struct asset_thumbnail
{
  texture *Texture;
  camera   Camera;
};

poof(block_array_h(asset_thumbnail, {8}))
#include <generated/block_array_h_asset_thumbnail_688856411.h>

struct selection_modification_state
{
  face_index ClickedFace;
  v3 ClickedP[2];
};

enum level_editor_flags
{
  LevelEditorFlags_Noop                              = (1 << 0),
  /* LevelEditorFlags_RecomputeStandingSpotsOnLevelLoad = (1 << 1), */
};

poof(radio_button_group_for_bitfield_enum(level_editor_flags));
#include <generated/radio_button_group_for_bitfield_enum_level_editor_flags.h>

struct level_editor
{
  u16 SelectedColorIndex;
  u16 HoverColorIndex;

  u32 SelectionClicks;
  cp SelectionBase;

  rect3cp SelectionRegion;
  rect3cp CopyRegion;

  // Recorded when accel-clicking on the selection to manipulate it
  selection_modification_state Selection;
  selection_modification_state Entity;

  asset_thumbnail_block_array AssetThumbnails;
};

link_internal b32
SelectionIncomplete(u32 SelectionClicks)
{
  return SelectionClicks == 0 || SelectionClicks == 1;
}

link_internal void
ResetSelection(level_editor *Editor)
{
  Editor->SelectionClicks = 0;
  Editor->SelectionBase = {};
  Editor->SelectionRegion = {};
}

link_internal void
ResetSelectionIfIncomplete(level_editor *Editor)
{
  if (SelectionIncomplete(Editor->SelectionClicks)) { ResetSelection(Editor); }
}

struct maybe_v3
{
  maybe_tag Tag;
  v3 V3;
};

enum selection_mode
{
  SelectionMode_Noop,

  SelectionMode_Resize,
  SelectionMode_TranslateLinear,
  SelectionMode_TranslatePlanar,
};

enum world_edit_mode
{
  WorldEditMode_Select          = (1 << 0),

  WorldEditMode_FillSelection   = (1 << 1),
  WorldEditMode_PaintSelection  = (1 << 2),
  WorldEditMode_DeleteSelection = (1 << 3),

  WorldEditMode_Eyedropper      = (1 << 4),

  WorldEditMode_AddSingle       = (1 << 5),
  WorldEditMode_RemoveSingle    = (1 << 6),
  WorldEditMode_PaintSingle     = (1 << 7),

  WorldEditMode_BlitEntity      = (1 << 8),

  WorldEditMode_RecomputeStandingSpots = (1 << 9),
};

poof(string_and_value_tables(world_edit_mode))
#include <generated/string_and_value_tables_world_edit_mode.h>

poof(radio_button_group_for_bitfield_enum(world_edit_mode));
#include <generated/radio_button_group_for_bitfield_enum_world_edit_mode.h>

// TODO(Jesse): rect3cp should probably be a pointer..?
//
link_internal world_chunk_ptr_buffer
GatherChunksOverlappingArea(world *World, rect3cp Region, memory_arena *Memory)
{
  auto MinP = Region.Min;
  auto MaxP = Region.Max;

  world_position Delta = MaxP.WorldP - MinP.WorldP + 1;
  u32 TotalChunkCount = Abs(Volume(Delta));

  world_chunk_ptr_buffer Result = {};

  // NOTE(Jesse): These need to be aligned to the cache line size, so don't use the constructor fn
  Result.Start = AllocateAligned(world_chunk*, Memory, TotalChunkCount, CACHE_LINE_SIZE);

  u32 ChunkIndex = 0;
  for (s32 zChunk = MinP.WorldP.z; zChunk <= MaxP.WorldP.z; ++zChunk)
  {
    for (s32 yChunk = MinP.WorldP.y; yChunk <= MaxP.WorldP.y; ++yChunk)
    {
      for (s32 xChunk = MinP.WorldP.x; xChunk <= MaxP.WorldP.x; ++xChunk)
      {
        world_position ChunkP = World_Position(xChunk, yChunk, zChunk);
        world_chunk *Chunk = GetWorldChunkFromHashtable(World, ChunkP);
        if (Chunk)
        {
          Assert(ChunkIndex < TotalChunkCount);
          Result.Start[ChunkIndex++] = Chunk;
        }
      }
    }
  }

  Result.Count = ChunkIndex;
  return Result;
}

link_internal void
GatherVoxelsOverlappingArea(world *World, rect3i SimSpaceAABB, world_chunk_ptr_buffer *ChunkBuffer, voxel *Voxels, s32 VoxelCount)
{
  Assert(Volume(SimSpaceAABB) == VoxelCount);

  v3i SimSpaceQueryDim = GetDim(SimSpaceAABB);

  s32 TotalVoxels_signed = Volume(SimSpaceAABB);
  Assert(TotalVoxels_signed > 0);

  u32 TotalVoxels = (u32)TotalVoxels_signed;

  // TODO(Jesse): Put this behind a debug/internal flag ?
  voxel UnsetVoxel = { 0xff, 0xff, 0xffff };
  /* voxel UnsetVoxel = { 0xff, 0xff, 0xffff, {}, {}, }; */
  for (u32 VoxelIndex = 0; VoxelIndex < TotalVoxels; ++VoxelIndex) { Voxels[VoxelIndex] = UnsetVoxel; }

  v3i SimSpaceQueryMinP = SimSpaceAABB.Min;

  for (u32 ChunkIndex = 0; ChunkIndex < ChunkBuffer->Count; ++ChunkIndex)
  {
    world_chunk *Chunk = ChunkBuffer->Start[ChunkIndex];
    auto SimSpaceChunkRect = GetSimSpaceAABBi(World, Chunk);
    auto SimSpaceIntersectionRect = Union(&SimSpaceChunkRect, &SimSpaceAABB);

    auto SimSpaceIntersectionMin = SimSpaceIntersectionRect.Min;
    auto SimSpaceIntersectionMax = SimSpaceIntersectionRect.Max;

    auto SimSpaceChunkMin = SimSpaceChunkRect.Min;
    auto SimSpaceChunkMax = SimSpaceChunkRect.Max;

    auto ChunkRelRectMin = SimSpaceIntersectionMin - SimSpaceChunkMin;
    auto ChunkRelRectMax = SimSpaceIntersectionMax - SimSpaceChunkMin;

    for (s32 zVoxel = s32(ChunkRelRectMin.z); zVoxel < s32(ChunkRelRectMax.z); zVoxel += 1)
    {
      for (s32 yVoxel = s32(ChunkRelRectMin.y); yVoxel < s32(ChunkRelRectMax.y); yVoxel += 1)
      {
        for (s32 xVoxel = s32(ChunkRelRectMin.x); xVoxel < s32(ChunkRelRectMax.x); xVoxel += 1)
        {
          voxel_position RelVoxP = Voxel_Position(s32(xVoxel), s32(yVoxel), s32(zVoxel));
          voxel *V = GetVoxel(Chunk, RelVoxP);
          Assert( (V->Flags & Voxel_MarkBit) == 0);

          v3i SimSpaceVoxPExact = V3i(xVoxel, yVoxel, zVoxel) + SimSpaceChunkMin;

          Assert(SimSpaceQueryMinP <= SimSpaceVoxPExact);
          u32 Index = MapIntoQueryBox(SimSpaceVoxPExact, SimSpaceQueryMinP, SimSpaceQueryDim);
          Assert(Index < TotalVoxels);
          Assert(Voxels[Index] == UnsetVoxel);
          Voxels[Index] = *V;
        }
      }
    }
  }
}

