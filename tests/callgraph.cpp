#define BONSAI_NO_PUSH_METADATA
#define PLATFORM_LIBRARY_AND_WINDOW_IMPLEMENTATIONS 1

#include <bonsai_types.h>
#include <unix_platform.cpp>

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

  // @bootstrap-debug-system
  shared_lib DebugLib = OpenLibrary(DEFAULT_DEBUG_LIB);
  if (!DebugLib) { Error("Loading DebugLib :( "); return False; }

  GetDebugState = (get_debug_state_proc)GetProcFromLib(DebugLib, "GetDebugState_Internal");
  if (!GetDebugState) { Error("Retreiving GetDebugState from Debug Lib :( "); return False; }

  debug_init_debug_system_proc InitDebugSystem = (debug_init_debug_system_proc)GetProcFromLib(DebugLib, "InitDebugSystem");
  if (!InitDebugSystem) { Error("Retreiving InitDebugSystem from Debug Lib :( "); return False; }

  InitDebugSystem(False);

  debug_state* DebugState = GetDebugState();
  DebugState->DebugDoScopeProfiling = True;

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

