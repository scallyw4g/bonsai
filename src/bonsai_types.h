#if BONSAI_STDLIB_NO_THREADPOOL
#error "Bonsai engine unity builds cannot disable the threadpool"
#endif

// Engine async closures require engine types before the threadpool implementation.
#ifndef BONSAI_STDLIB_USE_CUSTOM_THREADPOOL
#define BONSAI_STDLIB_USE_CUSTOM_THREADPOOL 1
#elif !BONSAI_STDLIB_USE_CUSTOM_THREADPOOL
#error "Bonsai engine unity builds require the engine threadpool"
#endif

#include <bonsai_stdlib/bonsai_stdlib.h>
#include <bonsai_stdlib/bonsai_stdlib.cpp>

#include <engine/engine.h>
#include <engine/engine.cpp>
