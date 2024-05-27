#include <bonsai_types.h>

#include <game_constants.h>
#include <game_types.h>

BONSAI_API_WORKER_THREAD_CALLBACK()
{
  b32 Result = False;
  switch (Entry->Type)
  {
    InvalidCase(type_work_queue_entry_noop);
    InvalidCase(type_work_queue_entry__align_to_cache_line_helper);
    InvalidCase(type_work_queue_entry__bonsai_render_command);
 
    case type_work_queue_entry_init_world_chunk:
    {
      Result = True;
    } break;

    case type_work_queue_entry_async_function_call:
    case type_work_queue_entry_update_world_region:
    case type_work_queue_entry_rebuild_mesh:
    case type_work_queue_entry_init_asset:
    case type_work_queue_entry_copy_buffer_ref:
    case type_work_queue_entry_copy_buffer_set:
    case type_work_queue_entry_sim_particle_system: {} break;
  }

  return Result;
}

BONSAI_API_MAIN_THREAD_INIT_CALLBACK()
{
  UNPACK_ENGINE_RESOURCES(Resources);

  AllocateWorld(World, {}, WORLD_CHUNK_DIM, g_VisibleRegion);

  GameState = Allocate(game_state, Resources->GameMemory, 1);
  return GameState;
}


link_internal b32
FilterByLoadableDLLs(file_traversal_node *Node)
{
  b32 Result = (EndsWith(Node->Name, CSz("_loadable" PLATFORM_RUNTIME_LIB_EXTENSION))) &&
               (StringsMatch(Node->Name, CSz("project_and_level_picker_loadable" PLATFORM_RUNTIME_LIB_EXTENSION)) == False);
  return Result;
}

BONSAI_API_MAIN_THREAD_CALLBACK()
{
  TIMED_FUNCTION();
  UNPACK_ENGINE_RESOURCES(Resources);

  Assert(ThreadLocal_ThreadIndex == 0);

  local_persist window_layout Window = WindowLayout("Project Loader Window");
  PushWindowStart(Ui, &Window);

  filtered_file_traversal_helper_params HelperParams = {&Window, FilterByLoadableDLLs};
    maybe_file_traversal_node MaybeNode = PlatformTraverseDirectoryTreeUnordered(CSz("bin/game_libs"), EngineDrawFileNodesFilteredHelper, u64(&HelperParams));
    if (MaybeNode.Tag)
    {
      RequestGameLibReload(Resources, MaybeNode.Value);
    }

  PushWindowEnd(Ui, &Window);
}
