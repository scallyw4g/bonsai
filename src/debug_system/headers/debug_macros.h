
#if BONSAI_DEBUG_SYSTEM_API

#define TIMED_FUNCTION() debug_timed_function FunctionTimer(__func__)
#define TIMED_NAMED_BLOCK(BlockName) debug_timed_function BlockTimer(BlockName)

#define TIMED_BLOCK(BlockName) { debug_timed_function BlockTimer(BlockName)
#define END_BLOCK(BlockName) } do {} while (0)

#define DEBUG_VALUE(Pointer) if (GetDebugState) {GetDebugState()->DebugValue(Pointer, #Pointer);}

#define DEBUG_FRAME_RECORD(...) DoDebugFrameRecord(__VA_ARGS__)
#define DEBUG_FRAME_END(Plat, ServerState) if (GetDebugState) {GetDebugState()->FrameEnd(Plat, ServerState);}
#define DEBUG_FRAME_BEGIN(Hotkeys) if (GetDebugState) {GetDebugState()->FrameBegin(Hotkeys);}

void DebugTimedMutexWaiting(mutex *Mut);
void DebugTimedMutexAquired(mutex *Mut);
void DebugTimedMutexReleased(mutex *Mut);
#define TIMED_MUTEX_WAITING(Mut)  if (GetDebugState) {GetDebugState()->MutexWait(Mut);}
#define TIMED_MUTEX_AQUIRED(Mut)  if (GetDebugState) {GetDebugState()->MutexAquired(Mut);}
#define TIMED_MUTEX_RELEASED(Mut) if (GetDebugState) {GetDebugState()->MutexReleased(Mut);}

#define MAIN_THREAD_ADVANCE_DEBUG_SYSTEM(dt)               if (GetDebugState) {GetDebugState()->MainThreadAdvanceDebugSystem(dt);}
#define WORKER_THREAD_ADVANCE_DEBUG_SYSTEM()               if (GetDebugState) {GetDebugState()->WorkerThreadAdvanceDebugSystem();}

#define DEBUG_CLEAR_META_RECORDS_FOR(Arena)                if (GetDebugState) {GetDebugState()->ClearMetaRecordsFor(Arena);}
#define DEBUG_TRACK_DRAW_CALL(CallingFunction, VertCount)  if (GetDebugState) {GetDebugState()->TrackDrawCall(CallingFunction, VertCount);}

#define DEBUG_REGISTER_VIEW_PROJECTION_MATRIX(ViewProjPtr) if (GetDebugState) {GetDebugState()->ViewProjection = ViewProjPtr;}
#define DEBUG_COMPUTE_PICK_RAY(Plat, ViewProjPtr)          if (GetDebugState) {GetDebugState()->ComputePickRay(Plat, ViewProjPtr);}
#define DEBUG_PICK_CHUNK(Chunk, ChunkAABB)                 if (GetDebugState) {GetDebugState()->PickChunk(Chunk, ChunkAABB);}

#else

#define TIMED_FUNCTION(...)
#define TIMED_BLOCK(...)
#define END_BLOCK(...)

#define TIMED_MUTEX_WAITING(...)
#define TIMED_MUTEX_AQUIRED(...)
#define TIMED_MUTEX_RELEASED(...)

#define DEBUG_FRAME_RECORD(...)
#define DEBUG_FRAME_END(...)
#define DEBUG_FRAME_BEGIN(...)

#define WORKER_THREAD_WAIT_FOR_DEBUG_SYSTEM(...)
#define MAIN_THREAD_ADVANCE_DEBUG_SYSTEM(...)
#define WORKER_THREAD_ADVANCE_DEBUG_SYSTEM()

#define DEBUG_CLEAR_META_RECORDS_FOR(...)
#define DEBUG_TRACK_DRAW_CALL(...)

#define DEBUG_REGISTER_VIEW_PROJECTION_MATRIX(...)

#define DEBUG_COMPUTE_PICK_RAY(...)
#define DEBUG_PICK_CHUNK(...)

#endif
