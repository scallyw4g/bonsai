#define DEBUG_SYSTEM_API 1
#define DEBUG_SYSTEM_LOADER_API 1

#include <bonsai_types.h>
#include <bonsai_stdlib/test/utils.h>

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
main(s32 ArgCount, const char** Args)
{
  TestSuiteBegin("callgraph", ArgCount, Args);

  bonsai_debug_system DebugSystem = InitializeBonsaiDebug("./bin/lib_debug_system_loadable" PLATFORM_RUNTIME_LIB_EXTENSION, 0);

  TIMED_FUNCTION();

  FunctionOne();

  debug_scope_tree *Tree = GetDebugState()->GetWriteScopeTree();
  TestThat(Tree);
  TestThat( Contains(Tree->Root->Name, "main") );
  TestThat( Contains(Tree->Root->Child->Name, "FunctionOne") );
  TestThat( Contains(Tree->Root->Child->Child->Name, "FunctionTwo") );
  TestThat( Contains(Tree->Root->Child->Child->Child->Name, "FunctionThree") );

  TestSuiteEnd();
}

