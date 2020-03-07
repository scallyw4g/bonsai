struct work_queue_entry_copy_buffer
{
  untextured_3d_geometry_buffer* Src;
  untextured_3d_geometry_buffer Dest;
  v3 Basis;
};

struct work_queue_entry_init_world_chunk
{
  void *Input;
};

meta(
  d_union( work_queue_entry,
  {
    work_queue_entry_init_world_chunk,
    work_queue_entry_copy_buffer,
  })
)
#include <metaprogramming/output/d_union_work_queue_entry>

