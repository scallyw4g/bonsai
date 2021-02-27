#define PLATFORM_LIBRARY_AND_WINDOW_IMPLEMENTATIONS 1

#include <bonsai_types.h>
#include <tests/test_utils.cpp>

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

  init_debug_system_proc InitDebugSystem = (init_debug_system_proc)GetProcFromLib(DebugLib, DebugLibName_InitDebugSystem);
  if (!InitDebugSystem) { Error("Retreiving InitDebugSystem from Debug Lib :( "); return False; }

  GetDebugState = InitDebugSystem(0);

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

