
#define BONSAI_NO_PUSH_METADATA

#include <bonsai_types.h>
#include <unix_platform.cpp>

#include <debug_data_system.cpp>

#include <test_utils.cpp>


void
FunctionThree()
{
  TIMED_FUNCTION();
}

void
FunctionTwo()
{
  TIMED_FUNCTION();
  FunctionThree();
}

void
FunctionOne()
{
  TIMED_FUNCTION();
  FunctionTwo();
}

s32
main()
{
  TestSuiteBegin("callgraph");

  u32 TotalThreadCount = GetTotalThreadCount();

  debug_state DebugState = {};
  GlobalDebugState = &DebugState;
  GlobalDebugState->Initialized = True;

  InitDebugMemoryAllocationSystem(&DebugState);
  InitScopeTrees(ThreadsafeDebugMemoryAllocator(), TotalThreadCount);

  DebugState.Initialized = True;

  TIMED_FUNCTION();

  debug_scope_tree *Tree = DebugState.GetWriteScopeTree();

  FunctionOne();

  TestThat(Tree);
  TestThat( Contains(Tree->Root->Name, "main") );
  TestThat( Contains(Tree->Root->Child->Name, "FunctionOne") );
  TestThat( Contains(Tree->Root->Child->Child->Name, "FunctionTwo") );
  TestThat( Contains(Tree->Root->Child->Child->Child->Name, "FunctionThree") );

  TestSuiteEnd();
}

