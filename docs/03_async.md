# Async Work Queues

The work queue API is designed to compose async functions into jobs and then
connect those jobs with completion dependencies. The generated wrappers handle
capturing arguments and building tasks; callers choose whether to submit work
immediately, retain its result for a waiter, or make it part of a larger join.

## Submit Independent Work

Mark any function, with `@async` to generate `Function_Task`, `Function_Job`, and
`Function_Async` helpers:

```cpp
link_internal void
poof(@async)
CounterTest()
{
	AtomicIncrement(&GlobalCounter);
}
```

`CounterTest_Async` is the compact fire-and-forget path. It creates a one-task
job and submits it to the selected queue:

```cpp
CounterTest_Async(&Plat->HighPriority);
```

Function arguments are captured into the task when it is created. For async
functions that return a value, optionally pass a destination pointer; the
worker writes the result there, so that storage must remain valid until the job
executes.

## Keep a Job Until Completion

If the caller must inspect completion, reserve the job with the `Await` flag
before it is submitted. The generated `_Async` helper accepts the flags after
the function arguments and returns a generation-tagged job ID:

```cpp
work_queue_job_reserve_flags Flags = WorkQueueJobReserveFlag_Await;
global_job_index Id = CounterTest_Async(&Plat->HighPriority, Flags);
work_queue_job *Job = GetJobFromGlobal(Plat, Id);

// Spinlock until Job completes.
while (Job->State != WorkQueueJobState_Await)
{
  // Job is complete
}

UnawaitAndRetire(Plat, Job);
```

The `Await` flag keeps the job slot alive after its tasks finish. Completion
moves the job to `WorkQueueJobState_Await`; `UnawaitAndRetire` releases the
waiter and allows the slot to be reused. Do not retain or poll a job without an
awaiter: completed jobs are retired and their slots can be reused immediately.

// TODO(Jesse): Add an assertion to GetJobFromGlobal that the job we're getting
// isn't yet submitted, or has waiters.  I think it's always a bug to try and
// fetch a pointer to a job that's not in one of those two states.

## Join Jobs With a Continuation

For launching a continuation Job after a set of Jobs have completed, create the
continuation as a reserved job, attach it to each prerequisite with
`OnComplete`, then submit it after all dependencies have been registered. The
continuation's initial await is a hold that prevents it from running before
registration is complete. Each prerequisite adds a waiter to the continuation;
when the last prerequisite completes, the queue submits the continuation
automatically.

// TODO(Jesse): Add a @continuation tag that automatically adds the await tag

```cpp
work_queue_job_reserve_flags AwaitFlag = WorkQueueJobReserveFlag_Await;
global_job_index_block_array AwaitJobIds = {};
work_queue_job *Continuation =
	AwaitContinuation_Job(Queue, AwaitJobIds, AwaitFlag);

RangeIterator(Index, JobCount)
{
	global_job_index Id = CounterTest_Async(Queue, AwaitFlag);
	work_queue_job *Job = GetJobFromGlobal(Plat, Id);
	OnComplete(Job, Continuation);
}

SubmitJob(Continuation);
```

Each prerequisite is also awaited here so the submitting thread can retrieve
its job by ID and release that waiter after completion. The continuation can
run as soon as all its prerequisite links are satisfied; it does not wait for
the submitting thread to retire the prerequisite jobs. If a continuation needs
its own inputs, pass them when creating its `_Job`, just as with any other
generated async wrapper.

## Choose the Wrapper That Matches the Composition

Use `_Async` when one function call should become one submitted job. Use
`_Job` when you need a reserved, unsubmitted job so you can attach completion
links before starting it. `_Task` is the lower-level form for assembling tasks
into a job yourself. A job can contain multiple tasks; after each task runs,
remaining tasks are resubmitted in order, using the queue selected by each
task.
