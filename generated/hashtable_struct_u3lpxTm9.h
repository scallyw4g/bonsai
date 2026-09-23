// callsite
// external/bonsai_stdlib/src/work_queue.h:23:0

// def (hashtable_struct)
// external/bonsai_stdlib/src/poof_functions.h:804:0
struct work_queue_job_stats_linked_list_node
{
  b32 Tombstoned;
  work_queue_job_stats Element;
  work_queue_job_stats_linked_list_node *Next;
};

struct work_queue_job_stats_hashtable
poof(
  @collection
  
  
)
{
  u32 Size;
  u32 Pad;
  work_queue_job_stats_linked_list_node **Elements;
  /* OWNED_BY_THREAD_MEMBER() */
};

