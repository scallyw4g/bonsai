#if BONSAI_WIN32
#include <win32_platform.h>

#elif BONSAI_LINUX
#include <posix_platform.h>
#include <unix_platform.h>

#elif EMCC
#include <posix_platform.h>
#include <wasm_platform.h>

#else
#error "Unknown Platform!"
#endif
