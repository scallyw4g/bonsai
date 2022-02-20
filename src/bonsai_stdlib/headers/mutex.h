#define MUTEX_OPS_PER_FRAME (64)

enum mutex_op
{
  MutexOp_Waiting,
  MutexOp_Aquired,
  MutexOp_Released
};

struct mutex_op_record;
struct mutex
{
  native_mutex M;

#if BONSAI_INTERNAL
  mutex_op_record *DebugRecord;
#endif
};

struct mutex_op_record
{
  mutex *Mutex;
  u64 Cycle;
  mutex_op Op;
};

struct mutex_op_array
{
  u32 NextRecord;
  mutex_op_record Records[MUTEX_OPS_PER_FRAME];
};

