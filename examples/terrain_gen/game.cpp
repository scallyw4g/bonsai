#define BONSAI_DEBUG_SYSTEM_API 1

#include <bonsai_types.h>

#include "game.h"

/* static entity *TestEntity; */
/* static gen_chunk *Gen; */

BONSAI_API_WORKER_THREAD_INIT_CALLBACK()
{
  SetThreadLocal_ThreadIndex(Thread->Index);
}

BONSAI_API_MAIN_THREAD_INIT_CALLBACK()
{
  UNPACK_ENGINE_RESOURCES(Resources);

  auto Engine = Resources;

  Global_AssetPrefixPath = CSz("examples/terrain_gen/assets");

  world_position WorldCenter = V3i(100, 100, 0);

  canonical_position CameraTargetP = {};

  /* auto VisibleRegionSize = VisibleRegionSize_128; */
  auto VisibleRegionSize = VisibleRegionSize_1k;
  /* auto VisibleRegionSize = VisibleRegionSize_8192; */
  /* auto VisibleRegionSize = VisibleRegionSize_16k; */
  /* auto VisibleRegionSize = VisibleRegionSize_64k; */
  /* auto VisibleRegionSize = VisibleRegionSize_64k; */
  AllocateWorld(World, WorldCenter, VisibleRegionSize);
  SnapCameraToCenterOfWorld(Resources, VisibleRegionSize);

  GameState = Allocate(game_state, Resources->GameMemory, 1);
  *GameState = {}; GameState->VisibleRegionSize = &World->VisibleRegionSize;

#if 1
  easing_function *EasingFunction = &Graphics->TerrainShapingRC.ReshapeFunc;
  GameState->EasingFunction = EasingFunction;

  Push(&EasingFunction->Points, V2(0,0));
  Push(&EasingFunction->Points, V2(0.47f, 0.10f));
  Push(&EasingFunction->Points, V2(0.67f,0.78f));
  Push(&EasingFunction->Points, V2(0.87f,0.70f));
  Push(&EasingFunction->Points, V2(1,1));

  InitializeEasingFunctionVisualizerRenderPass_Async(
     LoRenderQ,
    &GameState->EasingFunctionVisRP,
     EasingFunction,
     0 );

#endif


#if 0
    entity *Entity = GetEntity(EntityTable, GetFreeEntity(EntityTable));
    TestEntity = Entity;

    Entity->P = GetCameraGhost(Engine)->P;

    file_traversal_node Dummy = 
    {
      FileTraversalType_File,
      CSz("dir"),
      CSz("dummy_name"),
    };

    maybe_asset_ptr MaybeAsset = GetOrAllocateAsset(Engine, &Dummy);

    if (MaybeAsset.Tag)
    {
      asset *Asset = MaybeAsset.Value;
      Entity->AssetId = Asset->Id;

      // TODO(Jesse): Free this somehow? @unclear_if_model_buffer_memory_is_released
      Asset->Models = ModelBuffer(1, Engine->GameMemory);

      Gen = GetOrAllocate(&Engine->GenChunkFreelist, {}, V3i(64) + V3i(0, 2, 2), V3i(1), Engine->GameMemory);


#if 1
    v3 rayPos = {};
    v3 rayDir = Normalize(V3(0.5f,1.f,1.f));

    // https://www.shadertoy.com/view/4dX3zl
    v3i mapPos = V3i(Floor(rayPos + 0.));

    v3 deltaDist = Abs(V3(Length(rayDir)) / rayDir);
    v3i rayStep = V3i(GetSign(rayDir));
    v3 sideDist = (GetSign(rayDir) * (V3(mapPos) - rayPos) + (GetSign(rayDir) * 0.5) + 0.5) * deltaDist;
    v3i mask = {};
    while (true)
    {
      v3 yzx = V3(sideDist.y, sideDist.z, sideDist.x);
      v3 zxy = V3(sideDist.z, sideDist.x, sideDist.y);

      mask.x = sideDist.x <= Min(yzx.x, zxy.x);
      mask.y = sideDist.y <= Min(yzx.y, zxy.y);
      mask.z = sideDist.z <= Min(yzx.z, zxy.z);

      //All components of mask are false except for the corresponding largest component
      //of sideDist, which is the axis along which the ray should be incremented.

      sideDist += V3(mask) * deltaDist;
      mapPos += V3i(V3(mask)) * rayStep;

      if (mapPos.x >= 64) break;
      if (mapPos.y >= 64) break;
      if (mapPos.z >= 64) break;

      s32 Index = GetIndex(mapPos, V3i(64));
      SetOccupancyBit(&Gen->Chunk, Index, 1);
      ++Gen->FilledCount;
    }
#else
      random_series Entropy = {5432643754854};
      RangeIterator(Index, 64*64)
      {
        u64 Mask = u64(RandomU32(&Entropy)) | (u64(RandomU32(&Entropy)) << 31);
        SetOccupancyMask(&Gen->Chunk, Index, Mask);
        Gen->FilledCount += CountBitsSet_Kernighan(Mask);
      }
#endif

      model *Model = Get(&Asset->Models, 0);
      Model->Gen = Gen;
      AllocateAndBuildMesh(&Engine->Stdlib.Plat, Gen, Model, GetTranArena(), Engine->GameMemory );

      SpawnEntity(Entity);
    }
    else
    {
      SoftError("Spawning procedural entity");
    }
#endif


  return GameState;
}


poof(do_editor_ui_for_compound_type(game_state))
#include <generated/do_editor_ui_for_compound_type_game_state.h>

BONSAI_API_MAIN_THREAD_CALLBACK()
{
  engine_resources *Engine = Resources;

  Assert(ThreadLocal_ThreadIndex == 0);

  TIMED_FUNCTION();
  UNPACK_ENGINE_RESOURCES(Resources);

  f32 dt = Plat->dt;
  f32 Speed = 80.f;


#if 0
  {

    local_persist b32 FirstFrame = True;

    if (FirstFrame)
    {
      FirstFrame=False;
      SleepMs(1000);
    }
    {
      Assert(HasGpuMesh(&Gen->Mesh));
      Assert(Gen->Mesh.Handles.Mapped == False);

      asset *Asset = GetAssetPtr(Engine, &TestEntity->AssetId, Engine->FrameIndex).Value;
      Assert(Asset);

      Asset->LoadState = AssetLoadState_Loaded;
      Asset->Type = AssetType_Models;
    }
  }
#endif

#if 1
  {
    window_layout *Window = GetOrCreateWindow(Ui, "GameState");
    PushWindowStart(Ui, Window);
      DoEditorUi(Ui, Window, GameState, {}, u32(Hash(Window)));
    PushWindowEnd(Ui, Window);
  }

  {
    window_layout *Window = GetOrCreateWindow(Ui, "Easing Window");
    PushWindowStart(Ui, Window);
    PushUntexturedQuad(Ui, V2(0), V2(256), zDepth_Text, 0, {}, UiElementLayoutFlag_Default, UseRenderPass_easing_function_visualizer_render_pass, &GameState->EasingFunctionVisRP);
    PushWindowEnd(Ui, Window);
  }

  {
    global_variable window_layout Window = WindowLayout("Terrain Shaping Shader", WindowLayoutFlag_Align_Right);
    PushWindowStart(Ui, &Window);

      file_traversal_node_block_array Files =
        GetLexicographicallySortedListOfFilesInDirectory(
            CSz("shaders/terrain/shaping"), GetTranArena());

      u32 I = 0;
      IterateOver(&Files, FileNode, FileNodeIndex)
      {
        if (FileNode->Type == FileTraversalType_File)
        {
          b32 Selected = Contains(Resources->Graphics.TerrainShapingRC.Program.FragSourceFilename, FileNode->Name);
          if (Button(Ui, FileNode->Name, UiId(&Window, "shader file name", I++), Selected))
          {
            cs *ShaderName = &Resources->Graphics.TerrainShapingRC.Program.FragSourceFilename;
            if (IsHeapAllocated(Heap, (void*)ShaderName->Start))
            {
              HeapDeallocate(Heap, (void*)ShaderName->Start);
            }
            *ShaderName = Concat(FileNode->Dir, CSz("/"), FileNode->Name, Heap);

            // Force engine to reload new shader
            Resources->Graphics.TerrainShapingRC.Program.FragmentTimeModifiedWhenLoaded = 0;
          }
          PushNewRow(Ui);
        }
      }
    PushWindowEnd(Ui, &Window);
  }

  {
    global_variable window_layout Window = WindowLayout("Terrain Decoration Shader", WindowLayoutFlag_Align_BottomRight);
    PushWindowStart(Ui, &Window);
      file_traversal_node_block_array Files = GetLexicographicallySortedListOfFilesInDirectory(CSz("shaders/terrain/decoration"), GetTranArena());

      u32 I = 0;
      IterateOver(&Files, FileNode, FileNodeIndex)
      {
        if (FileNode->Type == FileTraversalType_File)
        {
          b32 Selected = Contains(Resources->Graphics.TerrainDecorationRC.Program.FragSourceFilename, FileNode->Name);
          if (Button(Ui, FileNode->Name, UiId(&Window, "shader file name", I++), Selected))
          {
            cs *ShaderName = &Resources->Graphics.TerrainDecorationRC.Program.FragSourceFilename;
            if (IsHeapAllocated(Heap, (void*)ShaderName->Start))
            {
              HeapDeallocate(Heap, (void*)ShaderName->Start);
            }
            *ShaderName = Concat(FileNode->Dir, CSz("/"), FileNode->Name, Heap);

            // Force engine to reload new shader
            /* Resources->Graphics.TerrainDecorationRC.Program.FragSourceFilename = Concat(FileNode->Dir, CSz("/"), FileNode->Name, GetTranArena()); */
            Resources->Graphics.TerrainDecorationRC.Program.FragmentTimeModifiedWhenLoaded = 0;
          }
          PushNewRow(Ui);
        }

      }
    PushWindowEnd(Ui, &Window);
  }

#endif

#if 0
  IterateOver(&Editor->LoadedBrushes, Brush, BrushIndex)
  {
    if (StringsMatch(CS(Brush->NameBuf), CSz("rotate.brush")))
    {
      f32 BaseDiameter = 36.f;

      auto Shape = &Brush->Layers[0].Settings.Shape;
      auto Torus = &Brush->Layers[0].Settings.Shape.Torus;

      Torus->MajorRadius = BaseDiameter + Sin(Plat->GameTime)*4.f;
      if (Shape->Advanced.Rotation.x > 360.f)  { Shape->Advanced.Rotation.x -= 360.f; }
      if (Shape->Advanced.Rotation.y > 360.f)  { Shape->Advanced.Rotation.y -= 360.f; }
      if (Shape->Advanced.Rotation.z > 360.f)  { Shape->Advanced.Rotation.z -= 360.f; }

      Brush->Layers[0].Settings.Shape.Advanced.Rotation.x += Plat->dt*10.f;
      /* Brush->Layers[0].Settings.Shape.Advanced.Rotation.y += Plat->dt*30.f; */
      Brush->Layers[0].Settings.Shape.Advanced.Rotation.z += Plat->dt*30.f;
    }
  }
#endif

#if 0
  {
    IterateOver(&Editor->LoadedBrushes, Brush, BrushIndex)
    {
      if (StringsMatch(CS(Brush->NameBuf), CSz("portal.brush")))
      {
        f32 BaseDiameter = 36.f;
        f32 MinorRad = 3.f;

        auto Shape = &Brush->Layers[0].Settings.Shape;
        auto Torus = &Brush->Layers[0].Settings.Shape.Torus;

        f32 t = Mod(Plat->GameTime, PI32*4.f);
        Torus->MajorRadius = BaseDiameter + Sin(t)*0.5f;
        Torus->MinorRadius = MinorRad + Sin(t/2.f)*0.5f;
        /* if (Shape->Advanced.Rotation.x > 360.f)  { Shape->Advanced.Rotation.x -= 360.f; } */
        /* if (Shape->Advanced.Rotation.y > 360.f)  { Shape->Advanced.Rotation.y -= 360.f; } */
        if (Shape->Advanced.Rotation.z > 360.f)  { Shape->Advanced.Rotation.z -= 360.f; }

        /* Brush->Layers[0].Settings.Shape.Advanced.Rotation.x += Plat->dt*10.f; */
        Brush->Layers[0].Settings.Shape.Advanced.Rotation.z += Plat->dt*30.f;
      }
    }
  }
#endif

#if 0
  {
    IterateOver(&Editor->LoadedBrushes, Brush, BrushIndex)
    {
      if (StringsMatch(CS(Brush->NameBuf), CSz("portal.1.brush")))
      {
        f32 BaseDiameter = 36.f;
        f32 MinorRad = 3.f;

        auto Shape = &Brush->Layers[0].Settings.Shape;
        auto Torus = &Brush->Layers[0].Settings.Shape.Torus;

        f32 t = Mod(Plat->GameTime, PI32*4.f);
        Torus->MajorRadius = BaseDiameter + Sin(t);
        Torus->MinorRadius = MinorRad + Sin(t/2.f);
        /* if (Shape->Advanced.Rotation.x > 360.f)  { Shape->Advanced.Rotation.x -= 360.f; } */
        /* if (Shape->Advanced.Rotation.y > 360.f)  { Shape->Advanced.Rotation.y -= 360.f; } */
        /* if (Shape->Advanced.Rotation.z > 360.f)  { Shape->Advanced.Rotation.z -= 360.f; } */

        /* Brush->Layers[0].Settings.Shape.Advanced.Rotation.x += Plat->dt*10.f; */
        /* Brush->Layers[0].Settings.Shape.Advanced.Rotation.z += Plat->dt*30.f; */

        {
          auto Layer = &Brush->Layers[4].Settings;
          Layer->Offset.x += Plat->dt*10.f;
        }
      }
    }
  }
#endif


#if 0
  {
    IterateOver(&Editor->LoadedBrushes, Brush, BrushIndex)
    {
      if (StringsMatch(CS(Brush->NameBuf), CSz("portal.inner.brush")))
      {
        /* f32 BaseDiameter = 36.f; */
        /* f32 MinorRad = 3.f; */

        /* { */
        /*   auto Layer = &Brush->Layers[4].Settings; */
        /*   Layer->Offset.x += Plat->dt*10.f; */
        /* } */

        {
          auto Layer = &Brush->Layers[5].Settings;
          /* Layer->Offset.x += Plat->dt*5.f; */
          Layer->Offset.y += Plat->dt*5.f;
        }
      }
    }
  }
#endif

#if 1
  {
    IterateOver(&Editor->LoadedBrushes, Brush, BrushIndex)
    {
      if (Contains(CS(Brush->NameBuf), CSz("blood.inner")))
      {
        f32 ySpeed = Plat->dt*0.5f;
        f32 zSpeed = Plat->dt*5.f;
        {
          auto Layer = &Brush->Layers[0].Settings;
          /* Layer->Offset.x += Plat->dt*5.f; */
          Layer->Offset.y += ySpeed;
          Layer->Offset.z += zSpeed;
        }

        {
          auto Layer = &Brush->Layers[1].Settings;
          /* Layer->Offset.x += Plat->dt*5.f; */
          Layer->Offset.y += ySpeed;
          Layer->Offset.z += zSpeed;
        }
        {
          auto Layer = &Brush->Layers[2].Settings;
          Layer->Offset.y += ySpeed;
          Layer->Offset.z += zSpeed;
        }
      }
    }
  }
#endif

}
