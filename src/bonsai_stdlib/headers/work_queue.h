// TODO(Jesse): This seems like _way_ overkill..
#define WORK_QUEUE_SIZE (65536*8)

// Note(Jesse): The userland code must define work_queue_entry
struct work_queue_entry;
struct work_queue
{
  volatile u32 EnqueueIndex;
  volatile u32 DequeueIndex;
  work_queue_entry *Entries;
  semaphore* GlobalQueueSemaphore;
};

b32
QueueIsEmpty(work_queue* Queue)
{
  b32 Result = Queue->DequeueIndex == Queue->EnqueueIndex;
  return Result;
}

