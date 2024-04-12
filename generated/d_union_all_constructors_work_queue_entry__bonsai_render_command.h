// src/engine/render_command.h:47:0


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
WorkQueueEntryBonsaiRenderCommand( bonsai_render_command_clear_all_framebuffers A  )
{
  work_queue_entry__bonsai_render_command Result = {
    .Type = type_bonsai_render_command_clear_all_framebuffers,
    .bonsai_render_command_clear_all_framebuffers = A,

    

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

link_internal bonsai_render_command_clear_all_framebuffers
BonsaiRenderCommandClearAllFramebuffers( u32  Ignored  )
{
  bonsai_render_command_clear_all_framebuffers Reuslt = {
    .Ignored = Ignored
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

