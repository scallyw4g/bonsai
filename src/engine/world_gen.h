link_internal void
FinalizeNoiseValues( work_queue_job *Job, gpu_readback_buffer  PBOBuf, u32 *NoiseData, v3i  NoiseDim, octree_node *DestNode );

link_internal void
CheckNoiseReadbackJob( work_queue_job *Job, gpu_readback_buffer PBOBuf, v3i NoiseDim, octree_node *DestNode );
