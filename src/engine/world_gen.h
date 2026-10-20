link_internal void
FinalizeNoiseValues( work_queue_job *Job, u32 *BatchNoiseData, v3i  NoiseDim, octree_node_ptr_static_cursor_TERRAIN_BATCH_CHUNK_COUNT Batch );

link_internal void
CheckNoiseReadbackJob( work_queue_job *Job,
                  gpu_readback_buffer  PBOBuf,
                                  v3i  NoiseDim,
                          octree_node_ptr_static_cursor_TERRAIN_BATCH_CHUNK_COUNT Batch );
