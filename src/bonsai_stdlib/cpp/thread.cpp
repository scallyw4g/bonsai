u32
GetWorkerThreadCount()
{
  u32 LogicalCoreCount = PlatformGetLogicalCoreCount();
  u32 Bias = 1 + DEBUG_THREAD_COUNT_BIAS; // +1 because we already have a main thread

  if (Bias >= LogicalCoreCount)
  {
    Bias = LogicalCoreCount - 1;
  }

  u32 ThreadCount = LogicalCoreCount - Bias;
  return ThreadCount;
}

u32
GetTotalThreadCount()
{
  u32 Result = GetWorkerThreadCount() + 1;
  return Result;
}

