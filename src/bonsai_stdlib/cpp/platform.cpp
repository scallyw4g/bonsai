
#if BONSAI_LINUX || EMCC
#include <posix_platform.cpp>
#endif

#if _WIN32
#include <win32_platform.cpp>
#elif BONSAI_LINUX
#include <unix_platform.cpp>
#elif EMCC
#include <wasm_platform.cpp>
#else
#error "Unsupported Platform"
#endif
