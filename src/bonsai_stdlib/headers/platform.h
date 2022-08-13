//
// Required for FILE* type .. might want to rebuild the file API to use
// platform functions instead, but for now the CRT ones are good enough.
//
#include <stdio.h>


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
#include <bonsai_stdlib/platform/win32_platform.h>

#elif BONSAI_LINUX
#include <bonsai_stdlib/platform/posix_platform.h>
#include <bonsai_stdlib/platform/unix_platform.h>

#elif BONSAI_EMCC
#include <bonsai_stdlib/platform/posix_platform.h>
#include <bonsai_stdlib/platform/wasm_platform.h>

#else
#error "Unknown Platform!"
#endif
