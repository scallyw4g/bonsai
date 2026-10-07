#define PLATFORM_WINDOW_IMPLEMENTATIONS 1

#define BONSAI_DEBUG_SYSTEM_API 1
#define BONSAI_DEBUG_SYSTEM_LOADER_API 1

#define BONSAI_STDLIB_USE_CUSTOM_THREADPOOL 1

#include <bonsai_stdlib/bonsai_stdlib.h>
#include <bonsai_stdlib/bonsai_stdlib.cpp>

link_internal void CounterTest();

#include <bonsai_stdlib/src/threadpool.cpp>

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
    auto Job = CounterTest_Job(&Plat->HighPriority);
    TestThat(Job->State == WorkQueueJobState_Reserved);
    FirstJobIndex = Job->Index;
    SubmitJob(&Plat->HighPriority, Job);
    while (Job->State != WorkQueueJobState_Free); // Wait for the job to flush
  }
  TestThat(GlobalCounter == 1);

  {
    auto CounterJobIndex = CounterTest_Async(&Plat->HighPriority);
    TestThat(CounterJobIndex.Index == FirstJobIndex.Index);

    work_queue_job *Job = GetJobFromGlobal(Plat, CounterJobIndex);
    // Wait for the job to flush
    while (Job->State != WorkQueueJobState_Free);
  }
  TestThat(GlobalCounter == 2);

  {
    auto CounterJobIndex = CounterTest_Async(&Plat->HighPriority);
    TestThat(CounterJobIndex.Index == FirstJobIndex.Index);

    work_queue_job *Job = GetJobFromGlobal(Plat, CounterJobIndex);
    // Wait for the job to flush
    while (Job->State != WorkQueueJobState_Free);
  }
  TestThat(GlobalCounter == 3);

}

link_internal void
ResetJobIndexGenerations()
{
  platform *Plat = GetPlatform();

  RangeIterator_t(u32, JobIndex, Plat->JobCount)
  {
    auto Job = Plat->Jobs+JobIndex;
    Job->Index.Generation = 0;
  }
}

// Test case using Await mechanism.  This is how you're actually supposed to
// use the API.
//
link_internal void
TestMultipleJobs()
{
  platform *Plat = GetPlatform();

  const s32 JobCount = 64;
  global_job_index JobIds[JobCount] = {};

  GlobalCounter = 0;

  work_queue_job_reserve_flags Flags = WorkQueueJobReserveFlag_Await;
  RangeIterator(JobIndex, JobCount)
  {
    JobIds[JobIndex] = CounterTest_Async(&Plat->HighPriority, Flags);
    work_queue_job *Job = GetJobFromGlobal(Plat, JobIds[JobIndex]);
    TestThat( Job->AwaitCount == 1);
  }

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

    Unawait(Plat, Job);
  }

  TestThat(GlobalCounter == JobCount);
}

s32
main(s32 ArgCount, const char** Args)
{
  TestSuiteBegin("Work Queue", ArgCount, Args);

  TestSerialJobs();


  ResetJobIndexGenerations();
  TestMultipleJobs();

  TestSuiteEnd();
  exit(TestsFailed);
}


