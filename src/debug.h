debug_timed_function
DebugTimedFunction(u32 FunctionIndexIn, const char* FuncNameIn)
{
  Assert(FunctionIndexIn < DEBUG_STATE_ENTRY_COUNT);

  debug_timed_function Result = {};
  Result.FunctionIndex = FunctionIndexIn;
  Result.FuncName = FuncNameIn;

  // Record cycle count at last moment
  Result.StartingCycleCount = GetDebugState()->GetCycleCount();
  return Result;
}

#if DEBUG

#define TIMED_FUNCTION() debug_timed_function FunctionTimer = DebugTimedFunction(__COUNTER__, __FUNCTION_NAME__)
#define TIMED_BLOCK(BlockName) { debug_timed_function BlockTimer = DebugTimedFunction(__COUNTER__, BlockName)
#define END_BLOCK(BlockName) }
#define DEBUG_FRAME_END(...) DebugFrameEnd(__VA_ARGS__)

#else

#define TIMED_FUNCTION(...)
#define TIMED_BLOCK(...)
#define END_BLOCK(...)
#define DEBUG_FRAME_END(...)

#endif


