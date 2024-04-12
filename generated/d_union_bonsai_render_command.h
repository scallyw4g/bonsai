// src/engine/render_command.h:24:0

enum work_queue_entry__bonsai_render_command_type
{
  type_work_queue_entry__bonsai_render_command_noop,
  type_bonsai_render_command_allocate_buffers,
  type_bonsai_render_command_realloc_buffers,
  type_bonsai_render_command_delete_buffers,
};

struct work_queue_entry__bonsai_render_command
{
  enum work_queue_entry__bonsai_render_command_type Type;

  union
  {
    struct bonsai_render_command_allocate_buffers bonsai_render_command_allocate_buffers;
    struct bonsai_render_command_realloc_buffers bonsai_render_command_realloc_buffers;
    struct bonsai_render_command_delete_buffers bonsai_render_command_delete_buffers;
  };
};


