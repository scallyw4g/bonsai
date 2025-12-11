#define BONSAI_DEBUG_SYSTEM_API 1

#include <bonsai_types.h>

#include "game.h"


BONSAI_API_WORKER_THREAD_INIT_CALLBACK()
{
  SetThreadLocal_ThreadIndex(Thread->Index);
}

BONSAI_API_MAIN_THREAD_INIT_CALLBACK()
{
  UNPACK_ENGINE_RESOURCES(Resources);

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

#if 0
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

  return GameState;
}


poof(do_editor_ui_for_compound_type(game_state))
#include <generated/do_editor_ui_for_compound_type_game_state.h>

BONSAI_API_MAIN_THREAD_CALLBACK()
{
  Assert(ThreadLocal_ThreadIndex == 0);

  TIMED_FUNCTION();
  UNPACK_ENGINE_RESOURCES(Resources);

  f32 dt = Plat->dt;
  f32 Speed = 80.f;

#if 1
  {
    window_layout *Window = GetOrCreateWindow(Ui, "GameState");
    PushWindowStart(Ui, Window);
      DoEditorUi(Ui, Window, GameState, {}, u32(Hash(Window)));
    PushWindowEnd(Ui, Window);
  }

  /* { */
  /*   window_layout *Window = GetOrCreateWindow(Ui, "Easing Window"); */
  /*   PushWindowStart(Ui, Window); */
  /*   PushUntexturedQuad(Ui, V2(0), V2(256), zDepth_Text, 0, {}, UiElementLayoutFlag_Default, UseRenderPass_easing_function_visualizer_render_pass, &GameState->EasingFunctionVisRP); */
  /*   PushWindowEnd(Ui, Window); */
  /* } */

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

#if 1
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

#if 1
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


#if 1
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

}
