#define BONSAI_DEBUG_SYSTEM_API 1
#define BONSAI_DEBUG_SYSTEM_LOADER_API 1

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

  memory_arena BootstrapArena = {};
  engine_resources Engine = {};
  Global_EngineResources = &Engine;
  Ensure( InitializeBonsaiStdlib( BonsaiInit_InitDebugSystem,
                                  {},
                                  &Engine.Stdlib,
                                  &BootstrapArena ) );

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

