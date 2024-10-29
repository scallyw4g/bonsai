#define INVALID_GPU_TIMER_HANDLE (0xFFFFFFFF)

struct gpu_timer
{
  u32 Handle = INVALID_GPU_TIMER_HANDLE;
  u64 Ns;
};

poof(block_array_h(gpu_timer, {128}, {}))
#include <generated/block_array_h_gpu_timer_688735882_0.h>

global_variable b32 Global_ActiveGPUTimer;

link_internal gpu_timer
StartGpuTimer(void)
{
  Assert(Global_ActiveGPUTimer == False);
  Global_ActiveGPUTimer = True;

  gpu_timer Result = {};
  GL.GenQueries(1, &Result.Handle);
  Assert(Result.Handle != INVALID_GPU_TIMER_HANDLE);

  GL.BeginQuery(GL_TIME_ELAPSED, Result.Handle);
  return Result;
}

link_internal void
EndGpuTimer(gpu_timer *Timer)
{
  GL.EndQuery(GL_TIME_ELAPSED);
  Assert(Global_ActiveGPUTimer == True);
  Global_ActiveGPUTimer = False;
}

link_internal s32
QueryGpuTimer(gpu_timer *Timer)
{
  Assert(Timer->Handle != INVALID_GPU_TIMER_HANDLE);
  s32 Available = False;
  GL.GetQueryObjectiv(Timer->Handle, GL_QUERY_RESULT_AVAILABLE, &Available);

  if (Available)
  {
    GL.GetQueryObjectui64v(Timer->Handle, GL_QUERY_RESULT, &Timer->Ns);
    GL.DeleteQueries(1, &Timer->Handle);
    Timer->Handle = INVALID_GPU_TIMER_HANDLE;
  }

  return Available;
}
