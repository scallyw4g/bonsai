struct work_queue_entry_copy_buffer
{
  untextured_3d_geometry_buffer *Src;
  untextured_3d_geometry_buffer Dest;
  v3 Basis;
};


#define WORK_QUEUE_MAX_COPY_TARGETS 8
struct work_queue_entry_copy_buffer_set
{
  u32 Count;
  work_queue_entry_copy_buffer CopyTargets[WORK_QUEUE_MAX_COPY_TARGETS];
};

struct world_chunk;
struct work_queue_entry_init_world_chunk
{
  world_chunk *Chunk;
};

struct work_queue_entry_init_asset
{
  world_chunk *Chunk;
};


poof(
  d_union work_queue_entry
  {
    work_queue_entry_init_world_chunk
    work_queue_entry_copy_buffer
    work_queue_entry_copy_buffer_set
    work_queue_entry_init_asset
  }
)
#include <generated/d_union_work_queue_entry.h>

