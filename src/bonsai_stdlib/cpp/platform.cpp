
#if BONSAI_LINUX || BONSAI_EMCC
#include <posix_platform.cpp>
#endif

#if BONSAI_WIN32
#include <win32_platform.cpp>
#elif BONSAI_LINUX
#include <unix_platform.cpp>
#elif BONSAI_EMCC
#include <wasm_platform.cpp>
#else
#error "Unsupported Platform"
#endif
