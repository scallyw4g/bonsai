#define CACHE_LINE_SIZE (64)

#define BindToInput(Keysym, InputField, Boolean) \
  case Keysym: {                                 \
    Plat->Input.InputField.Clicked = Boolean; \
    Plat->Input.InputField.Pressed = Boolean;     \
  } break;

#define BindKeydownToInput(Keysym, InputField) \
    BindToInput(Keysym, InputField, True)

#define BindKeyupToInput(Keysym, InputField) \
    BindToInput(Keysym, InputField, False)


#if BONSAI_WIN32
#include <win32_platform.h>

#elif BONSAI_LINUX
#include <posix_platform.h>
#include <unix_platform.h>

#elif BONSAI_EMCC
#include <posix_platform.h>
#include <wasm_platform.h>

#else
#error "Unknown Platform!"
#endif
