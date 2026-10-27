// callsite
// external/bonsai_stdlib/src/work_queue.cpp:133:0

// def (string_and_value_tables)
// external/bonsai_stdlib/src/poof_functions.h:2233:0
link_internal b32
IsValid(work_queue_job_state Value)
{
  b32 Result = False;
  switch (Value)
  {
            case WorkQueueJobState_Undefined:

        case WorkQueueJobState_Free:

        case WorkQueueJobState_Reserved:

        case WorkQueueJobState_Submitted:

        case WorkQueueJobState_Complete:

        case WorkQueueJobState_Await:

        case WorkQueueJobState_AwaitComplete:


    {
      Result = True;
    }
  }
  return Result;
}



link_internal counted_string
ToStringPrefixless(work_queue_job_state Type)
{
  cs Result = {};
  if (IsValid(Type))
  {
    switch (Type)
    {
                  case WorkQueueJobState_Undefined: { Result = CSz("Undefined"); } break;

            case WorkQueueJobState_Free: { Result = CSz("Free"); } break;

            case WorkQueueJobState_Reserved: { Result = CSz("Reserved"); } break;

            case WorkQueueJobState_Submitted: { Result = CSz("Submitted"); } break;

            case WorkQueueJobState_Complete: { Result = CSz("Complete"); } break;

            case WorkQueueJobState_Await: { Result = CSz("Await"); } break;

            case WorkQueueJobState_AwaitComplete: { Result = CSz("AwaitComplete"); } break;



      
    }
  }
  else
  {
    Result = CSz("(CORRUPT ENUM VALUE)");
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (enum_t.name)", Type); } */
  return Result;
}

link_internal counted_string
ToString(work_queue_job_state Type)
{
  Assert(IsValid(Type));

  counted_string Result = {};
  switch (Type)
  {
            case WorkQueueJobState_Undefined: { Result = CSz("WorkQueueJobState_Undefined"); } break;

        case WorkQueueJobState_Free: { Result = CSz("WorkQueueJobState_Free"); } break;

        case WorkQueueJobState_Reserved: { Result = CSz("WorkQueueJobState_Reserved"); } break;

        case WorkQueueJobState_Submitted: { Result = CSz("WorkQueueJobState_Submitted"); } break;

        case WorkQueueJobState_Complete: { Result = CSz("WorkQueueJobState_Complete"); } break;

        case WorkQueueJobState_Await: { Result = CSz("WorkQueueJobState_Await"); } break;

        case WorkQueueJobState_AwaitComplete: { Result = CSz("WorkQueueJobState_AwaitComplete"); } break;



    
  }
  /* if (Result.Start == 0) { Info("Could not convert value(%d) to (enum_t.name)", Type); } */
  return Result;
}

link_internal work_queue_job_state
WorkQueueJobState(counted_string S)
{
  work_queue_job_state Result = {};

    if (StringsMatch(S, CSz("WorkQueueJobState_Undefined"))) { return WorkQueueJobState_Undefined; }
  if (StringsMatch(S, CSz("WorkQueueJobState_Free"))) { return WorkQueueJobState_Free; }
  if (StringsMatch(S, CSz("WorkQueueJobState_Reserved"))) { return WorkQueueJobState_Reserved; }
  if (StringsMatch(S, CSz("WorkQueueJobState_Submitted"))) { return WorkQueueJobState_Submitted; }
  if (StringsMatch(S, CSz("WorkQueueJobState_Complete"))) { return WorkQueueJobState_Complete; }
  if (StringsMatch(S, CSz("WorkQueueJobState_Await"))) { return WorkQueueJobState_Await; }
  if (StringsMatch(S, CSz("WorkQueueJobState_AwaitComplete"))) { return WorkQueueJobState_AwaitComplete; }


  return Result;
}


