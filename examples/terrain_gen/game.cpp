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
  /* auto VisibleRegionSize = VisibleRegionSize_1024; */
  /* auto VisibleRegionSize = VisibleRegionSize_8192; */
  auto VisibleRegionSize = VisibleRegionSize_16k;
  /* auto VisibleRegionSize = VisibleRegionSize_64k; */
  /* auto VisibleRegionSize = VisibleRegionSize_64k; */
  v3i VisibleRegion = V3i(VisibleRegionSize);
  f32 DistanceFromTarget = 5000.f;
  f32 FarClip = 5000000.f;
  StandardCamera(Graphics->Camera, FarClip, DistanceFromTarget);

  AllocateWorld(World, WorldCenter, WORLD_CHUNK_DIM, VisibleRegionSize);

  GameState = Allocate(game_state, Resources->GameMemory, 1);
  *GameState = {};

  Camera->GhostId = GetFreeEntity(EntityTable);
  entity *CameraGhost = GetEntity(EntityTable, Camera->GhostId);
  /* CameraGhost->P.WorldP = V3i(-53, -93, 2); */ 
  /* CameraGhost->P.WorldP = V3i(-25, -75, 2); */ 
  /* CameraGhost->P.WorldP = V3i(-5, -121, 2); */ 

  /* CameraGhost->P.WorldP = V3i(330, -87, 2); */ 
  /* CameraGhost->P.WorldP = V3i(33, -87, 2); */ 
  /* CameraGhost->P.WorldP = V3i(5, -73, 2); */ 
  /* CameraGhost->P.WorldP = V3i(1000, 1000, 100); */ 
  /* CameraGhost->P.WorldP = WorldCenter; */
  /* CameraGhost->P.WorldP = WorldCenter + VisibleRegion/2; */
  /* CameraGhost->P.WorldP = VisibleRegion/2; */
  CameraGhost->P.WorldP.z = 1000;
  CameraGhost->Behavior = entity_behavior_flags(CameraGhost->Behavior|EntityBehaviorFlags_DefatulCameraGhostBehavior|EntityBehaviorFlags_WorldCenter);


  GameState->VisibleRegionSize = &World->VisibleRegionSize;

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
       0);

  SpawnEntity(CameraGhost);
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
            // Force engine to reload new shader
            Resources->Graphics.TerrainShapingRC.Program.FragSourceFilename = Concat(FileNode->Dir, CSz("/"), FileNode->Name, GetTranArena());
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
            // Force engine to reload new shader
            Resources->Graphics.TerrainDecorationRC.Program.FragSourceFilename = Concat(FileNode->Dir, CSz("/"), FileNode->Name, GetTranArena());
            Resources->Graphics.TerrainDecorationRC.Program.FragmentTimeModifiedWhenLoaded = 0;
          }
          PushNewRow(Ui);
        }

      }
    PushWindowEnd(Ui, &Window);
  }


}
