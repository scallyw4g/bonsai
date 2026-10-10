// callsite
// external/bonsai_stdlib/src/work_queue.cpp:548:0

// def (poof_builtin.for_datatypes)
// external/bonsai_stdlib/src/work_queue.cpp:548:0




































struct await_continuation_async_params;
link_internal work_queue_task
WorkQueueEntryAsyncFunction( work_queue *Queue, await_continuation_async_params *Params )
{
  work_queue_task Result = {};
  Result.Queue = Queue;
  Result.Type = type_work_queue_task_async_function_call;
  Result.work_queue_task_async_function_call.Type = type_await_continuation_async_params;
  Result.work_queue_task_async_function_call.await_continuation_async_params = *Params;
  return Result;
}



























































































































































struct make_texture__r_g_b_a_async_params;
link_internal work_queue_task
WorkQueueEntryAsyncFunction( work_queue *Queue, make_texture__r_g_b_a_async_params *Params )
{
  work_queue_task Result = {};
  Result.Queue = Queue;
  Result.Type = type_work_queue_task_async_function_call;
  Result.work_queue_task_async_function_call.Type = type_make_texture__r_g_b_a_async_params;
  Result.work_queue_task_async_function_call.make_texture__r_g_b_a_async_params = *Params;
  return Result;
}


struct make_texture__r_g_b_async_params;
link_internal work_queue_task
WorkQueueEntryAsyncFunction( work_queue *Queue, make_texture__r_g_b_async_params *Params )
{
  work_queue_task Result = {};
  Result.Queue = Queue;
  Result.Type = type_work_queue_task_async_function_call;
  Result.work_queue_task_async_function_call.Type = type_make_texture__r_g_b_async_params;
  Result.work_queue_task_async_function_call.make_texture__r_g_b_async_params = *Params;
  return Result;
}




































































































struct compile_shader_pair_async_params;
link_internal work_queue_task
WorkQueueEntryAsyncFunction( work_queue *Queue, compile_shader_pair_async_params *Params )
{
  work_queue_task Result = {};
  Result.Queue = Queue;
  Result.Type = type_work_queue_task_async_function_call;
  Result.work_queue_task_async_function_call.Type = type_compile_shader_pair_async_params;
  Result.work_queue_task_async_function_call.compile_shader_pair_async_params = *Params;
  return Result;
}






struct counter_test_async_params;
link_internal work_queue_task
WorkQueueEntryAsyncFunction( work_queue *Queue, counter_test_async_params *Params )
{
  work_queue_task Result = {};
  Result.Queue = Queue;
  Result.Type = type_work_queue_task_async_function_call;
  Result.work_queue_task_async_function_call.Type = type_counter_test_async_params;
  Result.work_queue_task_async_function_call.counter_test_async_params = *Params;
  return Result;
}

























