# Async Work Queues

## Overview

The async threadpool in bonsai_stdlib is composed of several tiers of primitives.

- `work_queue` is a list of `work_queue_job`s which worker threads consume.
- `work_queue_job` is a list of `work_queue_task`s that a worker thread will process
- `work_queue_task` is a closure capture of the arguments to a function

Work-queue functions are declared by appending a `poof(@async)` tag to your function
definition.  This generates three additional ways to use the function.

The queue is selected when creating the task or job. The worker executes the
captured function arguments; for a function with a return value, pass a result
destination pointer. Keep that destination alive until the work has completed.

For example:

```cpp
u32 poof(@async) Function(s32 Argument) { ... whatever ... }
```

Generates:

- `Function_Async(...)`
  Fire-and-forget path.  Creates and immediately submits an async job that
  calls the single function.  The ResultDestination pointer is optional if you
  do not care about the return value of the function.

- `Function_Job(...)`
  Reserves a job and adds the Function task, but leaves the job unsubmitted so
  it can recieve additional tasks, or be connected to a continuation.

- `Function_Task(...)`
  Captures the arguments and creates a task for chaining additional tasks onto
  a single job.

## Fire and Forget

For fire-and-forget, simply call the `_Async` version of the function.
Specify the queue you'd like to submit to, and you're done.

```cpp
Function_Async( Queue, 42 );
```

## Task chaining

To construct a job with multiple tasks, first Reserve a job with the `_Job`
variant, construct additional tasks with the `_Task` variant, push the tasks
onto the job, and submit.

Note it is not required for all tasks request the same queue.  A job will be
submitted to the queue found in it's next available task (in this case, the first).
When consumed, the worker thread will consume all tasks that request the same queue
it was consumed from, and reschedule the task when it hits a different queue.

```cpp
auto Job   = Function_Job( Queue, 42 );
auto Task1 = Function_Task(Queue, 69);
auto Task2 = Function_Task(Queue, 420);

PushTask(Job, Task1);
PushTask(Job, Task2);

SubmitJob(Job);

```


## Observing Job Completion

If the caller needs to observe completion, pass `WorkQueueJobReserveFlag_Await`
and keep the returned `global_job_index`, then manually retire the job to
release the `global_job_index` slot back to the pool.

```cpp
global_job_index JobId = Function_Async( Queue, 42, WorkQueueJobReserveFlag_Await);

work_queue_job *Job = GetJobFromGlobal(Plat, JobId);
while (Job->State != WorkQueueJobState_Await);

// Job complete, do whatever you need to do with the result

UnawaitAndRetire(Plat, Job);
```

An awaited job transitions to `WorkQueueJobState_Await` when its work finishes,
and remains allocated while its waiter observes it. `UnawaitAndRetire` releases
the job and it is returned to the pool.

Without the await flag, a completed job is retired automatically.  The `_Async`
variant returns an invalid `global_job_index` for jobs started without the
await flag; there is no well-defined way to query for the state of a job that's
not been marked with await.  Launching an `_Async` job without the async flag,
then querying for the state, is always a bug.

## Join a set of Parent jobs with a Continuation

If you have a set of async jobs that must run and 'fan-in' to a job that
processes their results, you can use the `OnComplete` API.

Use `_Job` to assemble the Parents and Continuation jobs.  Before submitting
the jobs, register each Parent with the Continuation using `OnComplete`.  Then
submit the parents. Each completed parent decrements the continuation's join
count; the continuation is submitted automatically when the final parent
completes.

In the following example, we build a set of chunks using `BuildChunk`, then
the `RebuildWorld` continuation is fired automatically once they're all complete.

```cpp

link_internal void poof(@async)
BuildChunk(world_chunk *Chunk)
{
  // IDK .. do stuff here ..
}

link_internal void poof(@async)
RebuildWorld(world *World, world_chunk *Chunks, u32 ChunkCount)
{
  RangeIterator(JobIndex, ParentJobCount)
  {
    // Do stuff with Chunks that we passed individually to BuildChunk
  }
}

link_internal void
DispatchWorldRebuildJobs(platform *Plat, world *World, world_chunk *Chunks, u32 ChunkCount)
{
  work_queue *Queue = &Plat->LowPriority;
  work_queue_job_reserve_flags AwaitFlag = WorkQueueJobReserveFlag_Await;

  // Temp-allocate a list of Parent job pointers so we can retire them at the end
  //
  work_queue_job *Parents = Allocate(work_queue_job*, GetTranArena(), ChunkCount);

  // Construct the Continuation job that will fire once all Parents (BuildChunk)
  // are completed.
  //
  auto Continuation = RebuildWorld_Job(Queue, World, Chunks, ChunkCount, AwaitFlag);

  //
  // Connect all Parent jobs to the Continuation
  //
  // NOTE(Jesse): Constructing Parent jobs with the AwaitFlag is not necessary
  // for this example, but for completeness of the demonstration we will await
  // the Parent jobs and retire them manually at the end.
  //
  for (s32 Index = 0; Index < ChunkCount; ++Index)
  {
    Parents[Index] = BuildChunk_Job(Queue, Chunks + Index, AwaitFlag);
    OnComplete(Parents[Index], Continuation);
  }

  // Parents all registered with Continuation, it is safe to launch parent jobs
  //
  for (s32 Index = 0; Index < ChunkCount; ++Index)
  {
    SubmitJob(Parents[Index]);
  }

  // Wait for the continuation to complete
  //
  // The continuation job is fired automatically when the Parent jobs all
  // complete.  Note that this happens before the jobs are all retired, so you
  // can safely await only the continuation job and retire the parents when it completes.
  //
  while (Continuation->State != WorkQueueJobState_Await);

  // Continuation complete, safe to use the result if required.  Note that
  // after you call UnawaitAndRetire, the continuation becomes eligible for
  // reuse, so if you plan on reading through the job pointer you must wait to
  // retire the job until you've done so.
  //

  UnawaitAndRetire(Plat, Continuation);

  // Continuation poitner now invalid

  // Since we fired the parent jobs with the AwaitFlag, we must retire them here.
  // If you do not fire them with the Await flag, you can safely skip this step
  // as they will be retired automatically.
  //
  for (s32 Index = 0; Index < ChunkCount; ++Index)
  {
    while (Parents[Index]->State != WorkQueueJobState_Await) {}
    UnawaitAndRetire(Plat, Parents[Index]);
  }

  // :)

}

```

Register all `OnComplete` links before submitting any participating job.
`OnComplete` requires both jobs to still be `Reserved`; attaching after
submission can miss completion or allow the continuation to run too early.
The continuation itself is ordinary async work: reserve it with `Await` if
the caller needs to wait for it and retire it explicitly. Parent jobs can be
retired independently as each reaches `Await`; their completion has already
triggered the continuation's join bookkeeping.

The job pool is finite, so reserve only as much unsubmitted work as the
workflow needs. A retained job also occupies its slot until all its awaiters
release it.
