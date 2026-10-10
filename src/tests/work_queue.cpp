#define PLATFORM_WINDOW_IMPLEMENTATIONS 1

#define BONSAI_DEBUG_SYSTEM_API 1
#define BONSAI_DEBUG_SYSTEM_LOADER_API 1

#define BONSAI_STDLIB_USE_CUSTOM_THREADPOOL 1

#include <bonsai_stdlib/bonsai_stdlib.h>
#include <bonsai_stdlib/bonsai_stdlib.cpp>

link_internal void CounterTest();

#include <bonsai_stdlib/src/work_queue.cpp>

#include <bonsai_stdlib/test/utils.h>

debug_global volatile u32 GlobalCounter;

link_internal void
poof(@async)
CounterTest()
{
  AtomicIncrement(&GlobalCounter);
}

// NOTE(Jesse) This test excercises taking the same JobId multiple times
// This is an implementation detail, but it is assumed to work.
//
// This test also leans on the assumption that a job without waiters will
// always end up back on the freelist and polls the state.  You should
// never do this in practice, instead adding yourself as a waiter and polling the
// Wait state, but this is safe in this context (single job, nobody ever does
// anything weird).
//
// This also ensures the non-waiter path works as intended.
//
link_internal void
TestSerialJobs()
{
  platform *Plat = GetPlatform();

  global_job_index FirstJobIndex = {};

  {
    auto Job = CounterTest_Job(&Plat->LowPriority);
    TestThat(Job->State == WorkQueueJobState_Reserved);
    FirstJobIndex = Job->Index;
    SubmitJob(&Plat->LowPriority, Job);
    while (Job->State != WorkQueueJobState_Free); // Wait for the job to flush

    // NOTE(Jesse): This is hella janky and only necessary for the test ..
    // After the job sets itself to free in RetireWorkQueueJob it links it onto
    // the freelist.  This test is checking we always get the same ID for jobs
    // that have already flushed, so we need to make sure that link has had time
    // to complete.  It's not defined behavior that this works, but for this test
    // I want to make sure that it does.
    //
    // @janky_ass_sleep_waiting_for_LinkTS
    SleepMs(1);
  }
  TestThat(GlobalCounter == 1);

  {
    auto CounterJobIndex = CounterTest_Async(&Plat->LowPriority);
    TestThat(CounterJobIndex.Index == FirstJobIndex.Index);

    work_queue_job *Job = GetJobFromGlobal(Plat, CounterJobIndex);
    while (Job->State != WorkQueueJobState_Free); // Wait for the job to flush

    // @janky_ass_sleep_waiting_for_LinkTS
    SleepMs(1);
  }
  TestThat(GlobalCounter == 2);

  {
    auto CounterJobIndex = CounterTest_Async(&Plat->LowPriority);
    TestThat(CounterJobIndex.Index == FirstJobIndex.Index);

    work_queue_job *Job = GetJobFromGlobal(Plat, CounterJobIndex);
    while (Job->State != WorkQueueJobState_Free); // Wait for the job to flush

    // @janky_ass_sleep_waiting_for_LinkTS
    SleepMs(1);
  }
  TestThat(GlobalCounter == 3);

}

link_internal void
ResetJobIndexGenerations()
{
  platform *Plat = GetPlatform();

  Plat->JobsFreelist = 0;

  auto Freelist = Cast(volatile freelist_entry **, &Plat->JobsFreelist);
  RangeIterator_t(u32, JobIndex, Plat->JobCount)
  {
    work_queue_job *Job = StripVolatile(work_queue_job *, Plat->Jobs+JobIndex);

    /* Assert( Job->OwningThreadId == INVALID_THREAD_LOCAL_THREAD_INDEX ); */

    Job->Index.Generation = 0;
    Job->JoinContinuationJobId = {};;
    Job->Stats = 0;
    Link_TS(Freelist, Cast(freelist_entry *, Job));
  }
}

// Test case using Await mechanism.
//
// This is how you're actually supposed to use the API.
//
link_internal void
TestMultipleJobs()
{
  platform *Plat = GetPlatform();

  const s32 JobCount = 64;
  global_job_index JobIds[JobCount] = {};

  GlobalCounter = 0;

  work_queue_job_reserve_flags AwaitFlag = WorkQueueJobReserveFlag_Await;
  RangeIterator(JobIndex, JobCount)
  {
    JobIds[JobIndex] = CounterTest_Async(&Plat->LowPriority, AwaitFlag);
    work_queue_job *Job = GetJobFromGlobal(Plat, JobIds[JobIndex]);
    TestThat( Job->AwaitCount == 1);
  }

  // Sanity check we got contiguous IDs.  The API is does not guarantee this
  // during normal use, but in virgin conditions this invariant should hold.
  RangeIterator(JobIndex, JobCount-1)
  {
    work_queue_job *Job = GetJobFromGlobal(Plat, JobIds[JobIndex]);
    work_queue_job *Next = GetJobFromGlobal(Plat, JobIds[JobIndex+1]);
    TestThat(Next->Index.Generation == 1);
    TestThat(Job->Index.Generation == 1);
    TestThat(Job->Index.Index-1 == Next->Index.Index);
  }

  RangeIterator(JobIndex, JobCount)
  {
    work_queue_job *Job = GetJobFromGlobal(Plat, JobIds[JobIndex]);
    while (Job->State != WorkQueueJobState_Await);

    UnawaitAndRetire(Plat, Job);
  }

  TestThat(GlobalCounter == JobCount);
}

link_internal void
TestAwaitContinuation()
{
  platform *Plat = GetPlatform();
  work_queue *Queue = &Plat->LowPriority;

  GlobalCounter = 0;
  const s32 JobCount = u16_MAX-1;
  CAssert(JobCount <= u16_MAX);

  debug_global global_job_index *JobIds = Allocate(global_job_index, GetTranArena(), JobCount);

  work_queue_job_reserve_flags AwaitFlag = WorkQueueJobReserveFlag_Await;

  auto           ParentJobs = GlobalJobIndexBlockArray(Plat->Memory);
  auto AwaitContinuationJob = AwaitContinuation_Job(Queue, ParentJobs, AwaitFlag);

  RangeIterator(JobIndex, JobCount)
  {
    JobIds[JobIndex] = CounterTest_Job(&Plat->LowPriority, AwaitFlag)->Index;
    work_queue_job *Job = GetJobFromGlobal(Plat, JobIds[JobIndex]);

    OnComplete(Job, AwaitContinuationJob);

    TestThat( Job->AwaitCount == 1);
    TestThat( s32(AwaitContinuationJob->JoinCount) == JobIndex+1);
  }

  // All OnComplete jobs registered, we can now fire them all off, and when they
  // all finish, the AwaitContinuationJob will fire
  //

  RangeIterator(JobIndex, JobCount)
  {
    work_queue_job *Job = GetJobFromGlobal(Plat, JobIds[JobIndex]);
    SubmitJob(Job);
  }

  // Block while waiting for jobs to complete, retiring in order as they do.
  // Retiring in order is not necessary.

  RangeIterator(JobIndex, JobCount)
  {
    work_queue_job *Job = GetJobFromGlobal(Plat, JobIds[JobIndex]);
    while (Job->State != WorkQueueJobState_Await);

    UnawaitAndRetire(Plat, Job);
  }

  // Indirectly assert that all the jobs fired and completed
  //
  TestThat(GlobalCounter == JobCount);

  // Block while the continuation job completes
  //
  while (AwaitContinuationJob->State != WorkQueueJobState_Await);

  // Retire Continuation
  //
  UnawaitAndRetire(Plat, AwaitContinuationJob);

  // Retire should put it back on the freelist
  //
  TestThat(AwaitContinuationJob->State == WorkQueueJobState_Free);

  // This is not necessary .. it's just a sanity check
  //
  SignalAndWaitForWorkers(&Plat->WorkerThreadsSuspendFutex);
  Assert(QueueIsEmpty(&Plat->LowPriority));
  UnsignalFutex(&Plat->WorkerThreadsSuspendFutex);
}

s32
main(s32 ArgCount, const char** Args)
{
  TestSuiteBegin("Work Queue", ArgCount, Args);

  TestSerialJobs();

  ResetJobIndexGenerations();
  TestMultipleJobs();

  // NOTE(Jesse): Do it 64 times, for fun and profit.  This is how I caught
  // some improbable bugs, so I'll leave it here for now.  For the least
  // frequently occuring bugs I had this infinite loop and just let it run.
  RangeIterator(LoopIndex, 64)
  {
    ResetJobIndexGenerations();
    TestAwaitContinuation();
  }

  /* TestSuiteEnd(); */
  /* exit(TestsFailed); */
}


