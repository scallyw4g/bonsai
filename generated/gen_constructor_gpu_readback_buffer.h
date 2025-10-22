// src/engine/graphics.h:160:0

link_internal gpu_readback_buffer
GpuReadbackBuffer(  u32   PBO , GLsync   Fence  )
{
  gpu_readback_buffer Reuslt = {
            .PBO = PBO

,        .Fence = Fence
  };

  
  return Reuslt;
}

