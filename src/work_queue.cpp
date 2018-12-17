b32
QueueIsEmpty(work_queue* Queue)
{
  b32 Result = Queue->DequeueIndex == Queue->EnqueueIndex;
  return Result;
}

