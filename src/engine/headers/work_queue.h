struct work_queue_entry_copy_buffer
{
  untextured_3d_geometry_buffer* Src;
  untextured_3d_geometry_buffer Dest;
  v3 Basis;
};


#define WORK_QUEUE_MAX_COPY_TARGETS 16
struct work_queue_entry_copy_buffer_set
{
  u32 Count;
  work_queue_entry_copy_buffer CopyTargets[WORK_QUEUE_MAX_COPY_TARGETS];
};

struct work_queue_entry_init_world_chunk
{
  void *Input;
};

poof(
  d_union work_queue_entry
  {
    work_queue_entry_init_world_chunk
    work_queue_entry_copy_buffer
    work_queue_entry_copy_buffer_set
  }
)
#include <generated/d_union_work_queue_entry.h>

