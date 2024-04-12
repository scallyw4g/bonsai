// src/engine/render_command.h:84:0


link_internal work_queue_entry__bonsai_render_command
WorkQueueEntryBonsaiRenderCommand( bonsai_render_command_clear_all_framebuffers A  )
{
  work_queue_entry__bonsai_render_command Result = {
    .Type = type_bonsai_render_command_clear_all_framebuffers,
    .bonsai_render_command_clear_all_framebuffers = A,

    

  };
  return Result;
}

link_internal work_queue_entry__bonsai_render_command
WorkQueueEntryBonsaiRenderCommand( bonsai_render_command_allocate_buffers A  )
{
  work_queue_entry__bonsai_render_command Result = {
    .Type = type_bonsai_render_command_allocate_buffers,
    .bonsai_render_command_allocate_buffers = A,

    

  };
  return Result;
}

link_internal work_queue_entry__bonsai_render_command
WorkQueueEntryBonsaiRenderCommand( bonsai_render_command_realloc_buffers A  )
{
  work_queue_entry__bonsai_render_command Result = {
    .Type = type_bonsai_render_command_realloc_buffers,
    .bonsai_render_command_realloc_buffers = A,

    

  };
  return Result;
}

link_internal work_queue_entry__bonsai_render_command
WorkQueueEntryBonsaiRenderCommand( bonsai_render_command_delete_buffers A  )
{
  work_queue_entry__bonsai_render_command Result = {
    .Type = type_bonsai_render_command_delete_buffers,
    .bonsai_render_command_delete_buffers = A,

    

  };
  return Result;
}

link_internal work_queue_entry__bonsai_render_command
WorkQueueEntryBonsaiRenderCommand( bonsai_render_command_do_stuff A  )
{
  work_queue_entry__bonsai_render_command Result = {
    .Type = type_bonsai_render_command_do_stuff,
    .bonsai_render_command_do_stuff = A,

    

  };
  return Result;
}

link_internal work_queue_entry__bonsai_render_command
WorkQueueEntryBonsaiRenderCommand( bonsai_render_command_setup_shader A  )
{
  work_queue_entry__bonsai_render_command Result = {
    .Type = type_bonsai_render_command_setup_shader,
    .bonsai_render_command_setup_shader = A,

    

  };
  return Result;
}

link_internal work_queue_entry__bonsai_render_command
WorkQueueEntryBonsaiRenderCommand( bonsai_render_command_teardown_shader A  )
{
  work_queue_entry__bonsai_render_command Result = {
    .Type = type_bonsai_render_command_teardown_shader,
    .bonsai_render_command_teardown_shader = A,

    

  };
  return Result;
}

link_internal work_queue_entry__bonsai_render_command
WorkQueueEntryBonsaiRenderCommand( bonsai_render_command_draw_world_chunk_draw_list A  )
{
  work_queue_entry__bonsai_render_command Result = {
    .Type = type_bonsai_render_command_draw_world_chunk_draw_list,
    .bonsai_render_command_draw_world_chunk_draw_list = A,

    

  };
  return Result;
}

link_internal work_queue_entry__bonsai_render_command
WorkQueueEntryBonsaiRenderCommand( bonsai_render_command_draw_all_entities A  )
{
  work_queue_entry__bonsai_render_command Result = {
    .Type = type_bonsai_render_command_draw_all_entities,
    .bonsai_render_command_draw_all_entities = A,

    

  };
  return Result;
}


link_internal bonsai_render_command_clear_all_framebuffers
BonsaiRenderCommandClearAllFramebuffers( u32  Ignored  )
{
  bonsai_render_command_clear_all_framebuffers Reuslt = {
    .Ignored = Ignored
  };
  return Reuslt;
}

link_internal bonsai_render_command_allocate_buffers
BonsaiRenderCommandAllocateBuffers( u32* Buffers , s32  Count  )
{
  bonsai_render_command_allocate_buffers Reuslt = {
    .Buffers = Buffers,
    .Count = Count
  };
  return Reuslt;
}

link_internal bonsai_render_command_realloc_buffers
BonsaiRenderCommandReallocBuffers( gpu_element_buffer_handles* Handles , untextured_3d_geometry_buffer* Mesh  )
{
  bonsai_render_command_realloc_buffers Reuslt = {
    .Handles = Handles,
    .Mesh = Mesh
  };
  return Reuslt;
}

link_internal bonsai_render_command_delete_buffers
BonsaiRenderCommandDeleteBuffers( u32* Buffers , s32  Count  )
{
  bonsai_render_command_delete_buffers Reuslt = {
    .Buffers = Buffers,
    .Count = Count
  };
  return Reuslt;
}

link_internal bonsai_render_command_do_stuff
BonsaiRenderCommandDoStuff( u32  Ignorerd  )
{
  bonsai_render_command_do_stuff Reuslt = {
    .Ignorerd = Ignorerd
  };
  return Reuslt;
}

link_internal bonsai_render_command_setup_shader
BonsaiRenderCommandSetupShader( bonsai_render_command_shader_id  ShaderId  )
{
  bonsai_render_command_setup_shader Reuslt = {
    .ShaderId = ShaderId
  };
  return Reuslt;
}

link_internal bonsai_render_command_teardown_shader
BonsaiRenderCommandTeardownShader( bonsai_render_command_shader_id  ShaderId  )
{
  bonsai_render_command_teardown_shader Reuslt = {
    .ShaderId = ShaderId
  };
  return Reuslt;
}

link_internal bonsai_render_command_draw_world_chunk_draw_list
BonsaiRenderCommandDrawWorldChunkDrawList( world_chunk_ptr_paged_list* DrawList , shader* Shader  )
{
  bonsai_render_command_draw_world_chunk_draw_list Reuslt = {
    .DrawList = DrawList,
    .Shader = Shader
  };
  return Reuslt;
}

link_internal bonsai_render_command_draw_all_entities
BonsaiRenderCommandDrawAllEntities(  )
{
  bonsai_render_command_draw_all_entities Reuslt = {
    
  };
  return Reuslt;
}

