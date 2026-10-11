# Async Work Queues

## Overview

The async threadpool in bonsai_stdlib is composed of several tiers of primitives.

- `work_queue` is a list of `work_queue_job` entries which worker threads consume from
- `work_queue_job` is a list of `work_queue_task` entries that a worker thread will process
- `work_queue_task` is a closure capture of the arguments to a function

Async functions are genrated by appending a `poof(@async)` tag to any function
definition.  This generates three additional ways of using the function.

For example:

```cpp
u32 poof(@async) ThreadsafePrintf(s32 Argument) { .. print the argument somehow .. }
```

Generates:

- `ThreadsafePrintf_Async(...)`
  Fire-and-forget path.  Creates and immediately submits an async job with a
  single task corresponding to `ThreadsafePrintf`

- `ThreadsafePrintf_Job(...)`
  Reserves a job and add a `ThreadsafePrintf` task.  Leaves the job unsubmitted
  so it can receive additional tasks, or be connected to a continuation.

- `ThreadsafePrintf_Task(...)`
  Captures the arguments for ThreadsafePrintf and wraps in a task.  Useful for
  chaining additional tasks onto an existing job.

### Prototypes

Generated functions generally share a set of arguments, inferred from the target function.

`work_queue *Queue, ... ThreadsafePrintf Args ..., Optional ThreadsafePrintf Return Value pointer, work_queue_job_reserve_flags Flags`

In more concrete terms, the generated functions for our example, `ThreadsafePrintf`, would be

`global_job_index ThreadsafePrintf_Async(work_queue *Queue, u32 Argument, u32 *ReturnDest = 0, work_queue_job_reserve_flags Flags = 0)`
`work_queue_job *   ThreadsafePrintf_Job(work_queue *Queue, u32 Argument, u32 *ReturnDest = 0, work_queue_job_reserve_flags Flags = 0)`
`work_queue_task   ThreadsafePrintf_Task(work_queue *Queue, u32 Argument, u32 *ReturnDest = 0);

Notice, arguments for the three are very similar; the major difference between
the prototypes is their return value.

A `global_job_index` can be used to retrieve a `work_queue_job` pointer using
`GetJobFromGlobal` `ThreadsafePrintf_Async` returns a valid index only if the Await
flag is passed in the Flags argument.  This is a safety precuation; it is always
a bug to submit a job without an Await flag and subsequently query for it's state.

A `work_queue_job` pointer returned from `_Job` or `GetJobFromGlobal` may be
used to manipulate the job directly.  `ThreadsafePrintf_Job` returns a job pointer
directly here because the job is left in a `Reserved` state.  The job has been
allocated, but not yet submitted; the intent of `_Job` is that the user will be
appending additional tasks to the job, or registering a continuation, detailed
below.

A `work_queue_task` may be appended to a `work_queue_job` by using the
`PushTask(Job, &Task)` API.  Note the task is copied inside PushTask; passing a
stack pointer to Task is valid.

The queue is selected at the level of the task. The worker thread executes the
task function with the captured arguments.  For a function with a return value,
pass a result destination pointer. Keep that destination alive until the
work has completed.



## Job Construction, Submission and Completion

### Fire and Forget

For jobs with no completion dependencies, simply call the `_Async` version of
the function.  Specify the queue you'd like to submit to, and you're done.

```cpp
ThreadsafePrintf_Async( Queue, 42 );
```


### Task chaining

To construct a job with multiple tasks, first Reserve a job with the `_Job`
variant, construct additional tasks with the `_Task` variant, push the tasks
onto the job, and submit.

Note it is not required that all tasks in a job request the same queue.  A job
will be submitted to the queue specified by it's next available task (in this case,
the first).  When consumed, the worker thread will consume all tasks that
request the same queue it was consumed from, and reschedule the job when it
finds a task requesting a different queue.

```cpp
auto Job   = ThreadsafePrintf_Job( Queue, 42 );
auto Task1 = ThreadsafePrintf_Task(Queue, 69);
auto Task2 = ThreadsafePrintf_Task(Queue, 420);

PushTask(Job, Task1);
PushTask(Job, Task2);

SubmitJob(Job);

```


### Observing Job Completion

If the caller needs to observe completion, pass `WorkQueueJobReserveFlag_Await`
and keep the returned `global_job_index`, then manually retire the job to
release the `global_job_index` slot back to the pool.

```cpp
global_job_index JobId = ThreadsafePrintf_Async( Queue, 42, WorkQueueJobReserveFlag_Await);

work_queue_job *Job = GetJobFromGlobal(Plat, JobId);
while (Job->State != WorkQueueJobState_Await);

// Job complete, do whatever you need to do with the result

UnawaitAndRetire(Plat, Job);
```

An awaited job transitions to `WorkQueueJobState_Await` when its work finishes,
and remains allocated while its waiter observes it. `UnawaitAndRetire` releases
the job and it is returned to the pool, rendering pointers to it returned by
`_Job` or `GetJobFromGlobal` invalid.

Without the await flag, a completed job is retired automatically.  The `_Async`
variant returns an invalid `global_job_index` for jobs started without the
await flag; there is no well-defined way to query for the state of a job that's
not been marked with await.  Launching an `_Async` job without the Await flag
and then querying for the state is always a bug, so this operation is disallowed
at the API level.



## Continuations

If you have a set of async jobs that must run and 'fan-in' to a job that
processes their results, you can use the `OnComplete` API.

Use `_Job` to assemble a list of Parents and a Continuation job.  Before submitting
the Parent jobs, register each Parent with the Continuation using `OnComplete`.  Then
submit the parents. Each completed parent decrements the continuation's join
count and the continuation is submitted automatically when the last parent
completes.

// TODO(Jesse): Modify this note; this is not strictly true
It is important to note that using the `OnComplete` API requires both jobs to
still be `Reserved`; attaching after submission can miss completion or allow
the continuation to run too early.

The continuation itself is ordinary async work: reserve it with `Await` if
the caller needs to wait for it and retire it explicitly. Parent jobs can be
retired independently as each reaches `Await`; their completion has already
triggered the continuation's join bookkeeping.


## Complete Example

In the following example, we build a set of chunks using `BuildChunk`, then
the `RebuildWorld` continuation is fired automatically once they're all complete.

Note that this example is written with the purpose of clarity being the
foremost priority.  There are more efficient ways of dispatching work, but this
illustrates the basics clearly without introducing potential for subtle bugs.

```cpp

link_internal void poof(@async)
BuildChunk(world_chunk *Chunk)
{
  // IDK .. do stuff here ..
  //
  // It is important to note the runtime does not do any locking of resources
  // ensuring it is safe to access Chunk in a multithreaded context.  That is
  // left as a responsibility of the user.
}

// Runs once all BuildChunk jobs have completed, detailed below
//
link_internal void poof(@async)
RebuildWorld(world *World, world_chunk *Chunks, u32 ChunkCount)
{
  RangeIterator(ChunkIndex, ChunkCount)
  {
    world_chunk *Chunk = Chunks + ChunkIndex;

    // Do stuff with Chunks that we passed individually to BuildChunk ..
  }
}

link_internal void
DispatchWorldRebuildJobs(platform *Plat, work_queue *Queue, world *World)
{
          u32  ChunkCount = World->ChunkCount;
  world_chunk *Chunks     = World->Chunks;

  // Temp-allocate a list of Parent job pointers so we can retire them at the end
  //
  work_queue_job **Parents = Allocate(work_queue_job*, GetTranArena(), ChunkCount);

  // Construct the Continuation job that will fire once all Parent
  // (BuildChunk) jobs are completed.  We pass the AwaitFlag such that we can
  // spinlock at the end of the function, polling for completion.
  //
  // NOTE(Jesse): The Await flag is not strictly necessary for this example,
  // but is included for completeness of the demonstration.
  //
  work_queue_job_reserve_flags AwaitFlag = WorkQueueJobReserveFlag_Await;
  auto Continuation = RebuildWorld_Job(Queue, World, Chunks, ChunkCount, AwaitFlag);

  //
  // Connect all Parent jobs to the Continuation
  //
  // NOTE(Jesse): Again, constructing Parent jobs with the AwaitFlag is not
  // necessary for this example, but for completeness of the demonstration we
  // will await the Parent jobs and retire them manually at the end.
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

  //
  // Wait for the continuation to complete
  //
  // The continuation job is fired automatically after the Parent jobs all complete.
  // The continuation is fired before the parent jobs are retired, so you can safely
  // await only the continuation and retire the parents when it completes.
  //
  // If you do not construct the Continuation with the Await flag, it is retired
  // automatically and will never reach the Await state.
  //
  while (Continuation->State != WorkQueueJobState_Await);

  //
  // All parents and continuation complete, it is now safe to use the results.
  //
  // Note that after you call UnawaitAndRetire, the continuation becomes
  // eligible for reuse, so if you plan on reading through the job pointer you
  // must wait to retire the job until you've done so.
  //

  UnawaitAndRetire(Plat, Continuation);

  // Continuation poitner now invalid

  // Since we fired the parent jobs with the AwaitFlag, we must retire them here.
  //
  // If you do not fire the parent jobs with the Await flag, you can safely
  // skip this step; they will be retired automatically.
  //
  for (s32 Index = 0; Index < ChunkCount; ++Index)
  {
    while (Parents[Index]->State != WorkQueueJobState_Await) {}
    UnawaitAndRetire(Plat, Parents[Index]);

    // Parents[Index] pointer now invalid
    //
  }

  // :)

}

```

## Additional Examples

Examples resembling those found in this documentation that compile and run may
be found in [src/tests/work_queue.cpp](https://github.com/scallyw4g/bonsai/blob/master/src/tests/work_queue.cpp)

Specifically, the analogue to the Await/Continuation example is called TestAwaitContinuation.

