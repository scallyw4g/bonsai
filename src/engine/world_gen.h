
struct terrain_gen_job_texture_pack
{
};

struct terrain_gen_job_context
{
  work_queue_job *Job;
  gpu_readback_buffer ReadbackHandles;
  /* u32 *ReadbackBuffer; */
  /* terrain_gen_job_texture_pack Textures; */
};


link_internal void
FinalizeNoiseValues( work_queue_job *Job, u32 *BatchNoiseData, v3i  NoiseDim, octree_node_ptr_static_cursor_TERRAIN_BATCH_CHUNK_COUNT Batch );

link_internal void
CheckNoiseReadbackJob(           terrain_gen_job_context  Context,
                                                     v3i  NoiseDim,
 octree_node_ptr_static_cursor_TERRAIN_BATCH_CHUNK_COUNT  Batch );
