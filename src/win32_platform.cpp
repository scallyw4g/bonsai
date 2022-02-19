#include <chrono> // Timer

#define BONSAI_MAIN() int CALLBACK WinMain( HINSTANCE AppHandle, HINSTANCE Ignored, LPSTR CmdLine, int CmdShow )

global_variable HPALETTE global_hPalette;

inline void
ThreadSleep( semaphore Semaphore )
{
  WaitForSingleObject( Semaphore, INFINITE );
  return;
}

inline void
WakeThread( semaphore Semaphore )
{
  ReleaseSemaphore( Semaphore, 1, 0 );
  return;
}

semaphore
CreateSemaphore(void)
{
  semaphore Result = CreateSemaphore( 0, 0, LONG_MAX, 0);
  return Result;
}

thread_id
PlatformCreateThread( thread_main_callback_type ThreadMain, thread_startup_params *Params)
{
  DWORD flags = 0;

  thread_id ThreadId = CreateThread(
      0,
      0,
      (LPTHREAD_START_ROUTINE)ThreadMain,
      (void *)Params,
      flags,
      0 // &Params->Self.ID
  );

  return ThreadId;
}

#define CompleteAllWrites _WriteBarrier(); _mm_sfence()

inline r64
GetHighPrecisionClock()
{
  global_variable auto FirstTime = std::chrono::high_resolution_clock::now();
  // cout << "FirstTime Time : " << chrono::time_point_cast<chrono::nanoseconds>(FirstTime).time_since_epoch().count() << " ns \n";

  r64 Result = (r64)(std::chrono::high_resolution_clock::now() - FirstTime).count();
  // cout << "Time/iter, clock: " << chrono::duration_cast<chrono::nanoseconds>(Result).count() << " ns \n";

  return Result;
}

inline r64
ComputeDtForFrame(r64 *LastTime)
{
  r64 CurrentTime = GetHighPrecisionClock();
  r64 Dt = (CurrentTime - *LastTime) / 1000000000.0;

  *LastTime = CurrentTime;
  return Dt;
}

void
Terminate(os *Os)
{
  if (Os->GlContext) // Cleanup Opengl context
  {
    wglMakeCurrent(NULL, NULL);
    wglDeleteContext(Os->GlContext);
  }

  if (global_hPalette)
  {
    DeleteObject(global_hPalette);
  }

  ReleaseDC(Os->Window, Os->Display);

  SuspendWorkerThreads();

  PostQuitMessage(0);

  return;
}

void
setupPixelFormat(HDC hDC)
{
    PIXELFORMATDESCRIPTOR pfd = {
        sizeof(PIXELFORMATDESCRIPTOR),                              // size
        1,                                                          // version
        PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER, // support double-buffering
        PFD_TYPE_RGBA,                                              // color type
        16,                                                         // prefered color depth
        0, 0, 0, 0, 0, 0,                                           // color bits (ignored)
        0,                                                          // no alpha buffer
        0,                                                          // alpha bits (ignored)
        0,                                                          // no accumulation buffer
        0, 0, 0, 0,                                                 // accum bits (ignored)
        16,                                                         // depth buffer
        0,                                                          // no stencil buffer
        0,                                                          // no auxiliary buffers
        PFD_MAIN_PLANE,                                             // main layer
        0,                                                          // reserved
        0, 0, 0,                                                    // no layer, visible, damage masks
    };

    int pixelFormat = ChoosePixelFormat(hDC, &pfd);

    if (pixelFormat == 0) {
        MessageBox(WindowFromDC(hDC), "ChoosePixelFormat failed.", "Error",
                MB_ICONERROR | MB_OK);
        exit(1);
    }

    if (SetPixelFormat(hDC, pixelFormat, &pfd) != TRUE) {
        MessageBox(WindowFromDC(hDC), "SetPixelFormat failed.", "Error",
                MB_ICONERROR | MB_OK);
        exit(1);
    }
}

HPALETTE
setupPalette(HDC hDC)
{
    int pixelFormat = GetPixelFormat(hDC);
    PIXELFORMATDESCRIPTOR pfd;
    LOGPALETTE* pPal;
    u64 paletteSize;

    DescribePixelFormat(hDC, pixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

    if (pfd.dwFlags & PFD_NEED_PALETTE) {
        paletteSize = 1 << pfd.cColorBits;
    } else {
        return 0;
    }

    pPal = (LOGPALETTE*)
        malloc(sizeof(LOGPALETTE) + paletteSize * sizeof(PALETTEENTRY));
    pPal->palVersion = 0x300;
    pPal->palNumEntries = (u16)paletteSize;

    /* build a simple RGB color palette */
    {
        u8 redMask = (u8)(1 << pfd.cRedBits) - 1;
        u8 greenMask = (u8)(1 << pfd.cGreenBits) - 1;
        u8 blueMask = (u8)(1 << pfd.cBlueBits) - 1;
        u64 i;

        for (i=0; i<paletteSize; ++i) {
            pPal->palPalEntry[i].peRed =
                    (((i >> pfd.cRedShift) & redMask) * 255) / redMask;
            pPal->palPalEntry[i].peGreen =
                    (((i >> pfd.cGreenShift) & greenMask) * 255) / greenMask;
            pPal->palPalEntry[i].peBlue =
                    (((i >> pfd.cBlueShift) & blueMask) * 255) / blueMask;
            pPal->palPalEntry[i].peFlags = 0;
        }
    }

    HPALETTE hPalette = CreatePalette(pPal);
    free(pPal);

    if (hPalette) {
        SelectPalette(hDC, hPalette, FALSE);
        RealizePalette(hDC);
    }

	return hPalette;
}

LRESULT APIENTRY
WindowMessageCallback(
    HWND hWnd,
    UINT message,
    WPARAM wParam,
    LPARAM lParam)
{
  platform *Plat = (platform*)GetWindowLongPtr(hWnd, PLATFORM_OFFSET);
  os *Os = (os*)GetWindowLongPtr(hWnd, 0);

  switch (message)
  {
    case WM_CREATE:
    {
    } return 0;


    case WM_DESTROY:
    {
      Os->ContinueRunning = false;

    } return 0;

    case WM_SIZE:
    {
      int WinWidth = LOWORD(lParam);
      int WinHeight = HIWORD(lParam);

      Plat->WindowWidth = WinWidth;
      Plat->WindowHeight = WinHeight;
    } return 0;

    case WM_PALETTECHANGED:
    {
      /* realize palette if this is *not* the current window */
      if (Os->GlContext && global_hPalette && (HWND) wParam != hWnd)
      {
        UnrealizeObject(global_hPalette);
        SelectPalette(Os->Display, global_hPalette, FALSE);
        RealizePalette(Os->Display);
        SwapBuffers(Os->Display);
      }

    } return 0;

    case WM_QUERYNEWPALETTE:
    {
      /* realize palette if this is the current window */
      if (Os->GlContext && global_hPalette)
      {
        UnrealizeObject(global_hPalette);
        SelectPalette(Os->Display, global_hPalette, FALSE);
        RealizePalette(Os->Display);
        SwapBuffers(Os->Display);
        return TRUE;
      }

    } return 0;

    case WM_PAINT:
    {
      PAINTSTRUCT ps;
      BeginPaint(hWnd, &ps);
      if (Os->GlContext)
      {
        SwapBuffers(Os->Display);
      }
      EndPaint(hWnd, &ps);

    } return 0;


    /*
     * User Input
     */
    case WM_LBUTTONDOWN:
    {
      Plat->Input.LMB.Clicked = True;
      Plat->Input.LMB.Pressed = True;
    } return 0;

    case WM_LBUTTONUP:
    {
      Plat->Input.LMB.Pressed = False;
    } return 0;

    case WM_RBUTTONDOWN:
    {
      Plat->Input.RMB.Clicked = True;
      Plat->Input.RMB.Pressed = True;
    } return 0;

    case WM_RBUTTONUP:
    {
      Plat->Input.RMB.Pressed = False;
    } return 0;

    case WM_MOUSEMOVE:
    {
      Plat->MouseP.x = (r32)GET_X_LPARAM(lParam);
      Plat->MouseP.y = (r32)GET_Y_LPARAM(lParam);

    } return 0;



    case WM_KEYUP:
    {
      switch ((int)wParam)
      {
        BindKeyupToInput(0x57, W);
        BindKeyupToInput(0x44, D);
        BindKeyupToInput(0x53, S);
        BindKeyupToInput(0x41, A);
        BindKeyupToInput(0x51, Q);
        BindKeyupToInput(0x45, E);

        BindKeyupToInput(VK_F12, F12);
        BindKeyupToInput(VK_F11, F11);
        BindKeyupToInput(VK_F10, F10);
        BindKeyupToInput(VK_F9, F9);
        BindKeyupToInput(VK_F8, F8);
        BindKeyupToInput(VK_F7, F7);
        BindKeyupToInput(VK_F6, F6);
        BindKeyupToInput(VK_F5, F5);
        BindKeyupToInput(VK_F4, F4);
        BindKeyupToInput(VK_F3, F3);
        BindKeyupToInput(VK_F2, F2);
        BindKeyupToInput(VK_F1, F1);

        BindKeyupToInput(VK_SHIFT, Shift);
        BindKeyupToInput(VK_MENU, Alt);
        BindKeyupToInput(VK_CONTROL, Ctrl);
        BindKeyupToInput(VK_SPACE, Space);
        default: { /* Ignore all other keypresses */ } break;
      }
    } break;



    case WM_KEYDOWN:
    {
      switch ((int)wParam)
      {
        case VK_ESCAPE:
        {
          DestroyWindow(hWnd);
        } return 0;

        BindKeydownToInput(0x57, W);
        BindKeydownToInput(0x44, D);
        BindKeydownToInput(0x53, S);
        BindKeydownToInput(0x41, A);
        BindKeydownToInput(0x51, Q);
        BindKeydownToInput(0x45, E);

        BindKeydownToInput(VK_F12, F12);
        BindKeydownToInput(VK_F11, F11);
        BindKeydownToInput(VK_F10, F10);
        BindKeydownToInput(VK_F9, F9);
        BindKeydownToInput(VK_F8, F8);
        BindKeydownToInput(VK_F7, F7);
        BindKeydownToInput(VK_F6, F6);
        BindKeydownToInput(VK_F5, F5);
        BindKeydownToInput(VK_F4, F4);
        BindKeydownToInput(VK_F3, F3);
        BindKeydownToInput(VK_F2, F2);
        BindKeydownToInput(VK_F1, F1);

        BindKeydownToInput(VK_SHIFT, Shift);
        BindKeydownToInput(VK_MENU, Alt);
        BindKeydownToInput(VK_CONTROL, Ctrl);
        BindKeydownToInput(VK_SPACE, Space);
        default: { /* Ignore all other keypresses */ } break;

      } break;

      default: { /* Ignore all other window messages */ } break;
    }
  }

   return DefWindowProc(hWnd, message, wParam, lParam);
}

b32
OpenAndInitializeWindow( os *Os, platform *Plat)
{
  WNDCLASS wndClass;

  HINSTANCE AppHandle = GetModuleHandle(0);

  LPCSTR className = "Bonsai";

  // Register window class
  wndClass.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
  wndClass.lpfnWndProc = WindowMessageCallback;
  wndClass.cbClsExtra = 0;
  wndClass.cbWndExtra = sizeof(Plat) + sizeof(Os);
  wndClass.hInstance = AppHandle;
  wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
  wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
  wndClass.hbrBackground = CreateSolidBrush(COLOR_BACKGROUND);
  wndClass.lpszMenuName = NULL;
  wndClass.lpszClassName = className;
  RegisterClass(&wndClass);

  Os->Window = CreateWindow(
      className, className,
      WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
      100, 100, 1920, 1080,
      NULL, NULL, AppHandle, NULL);

  Os->Display = GetDC(Os->Window);

  setupPixelFormat(Os->Display);

  global_hPalette = setupPalette(Os->Display);

  HGLRC TempCtx = wglCreateContext(Os->Display);
  wglMakeCurrent(Os->Display, TempCtx);

int attribs[] =
  {
    WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
    WGL_CONTEXT_MINOR_VERSION_ARB, 0,
    WGL_CONTEXT_FLAGS_ARB, 0,
    0
  };

  PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
  Os->GlContext = wglCreateContextAttribsARB(Os->Display, 0, attribs);

  wglMakeCurrent(NULL, NULL);
  wglDeleteContext(TempCtx);

  wglMakeCurrent(Os->Display, Os->GlContext);

  SetWindowLongPtr(Os->Window, 0, (LONG_PTR)Os);
  SetWindowLongPtr(Os->Window, sizeof(Os), (LONG_PTR)Plat);

  ShowWindow(Os->Window, SW_SHOW);
  UpdateWindow(Os->Window);

  return True;
}


inline void*
GetProcFromLib(shared_lib Lib, const char *Name)
{
  void* Result = (void*)GetProcAddress(Lib, Name);
  return Result;
}

inline shared_lib
OpenLibrary(const char *LibPath)
{
  shared_lib Result = LoadLibrary(LibPath);

  if (!Result)
  {
    Error("Error loading library: %s", LibPath);
  }
  else
  {
    Info("Library (%s) Loaded Successfully.", LibPath);
  }

  return Result;
}

inline b32
CloseLibrary(shared_lib Lib)
{
  b32 Result = (b32)FreeLibrary(Lib);
  return Result;
}

#define CwdBufferLen 2048
debug_global char CwdBuffer[CwdBufferLen];

inline char*
GetCwd()
{
  GetCurrentDirectory( CwdBufferLen, CwdBuffer );
  return CwdBuffer;
}

b32
ProcessOsMessages(os *Os, platform *Plat)
{
  b32 Result = False;

  MSG Message;
  if ( PeekMessage(&Message, Os->Window, 0, 0, 0) )
  {
    Result = true;
    BOOL bRet = GetMessage( &Message, Os->Window, 0, 0 );

    if (bRet == -1)
    {
      // Error retreiving message, panic ?
      Assert(False);
    }
    else
    {
      TranslateMessage(&Message);
      DispatchMessage(&Message);
    }
  }

  return Result;
}

inline b32
IsFilesystemRoot(const char *Filepath)
{
  b32 Result = ( strlen(Filepath) == 3 && Filepath[1] == ':' && Filepath[2] == '\\');
  return Result;
}

inline void
BonsaiSwapBuffers(os *Os)
{
  SwapBuffers(Os->Display);
}

bonsai_function u64
PlatformGetPageSize()
{
  SYSTEM_INFO sysInfo;
  GetSystemInfo(&sysInfo);
  local_persist u64 Result = sysInfo.dwPageSize;
  Assert(Result == 4096);
  return Result;
}

bonsai_function u8*
PlatformAllocateSize(umm AllocationSize)
{
  Assert(AllocationSize % PlatformGetPageSize() == 0);

  u32 AllocationType = MEM_COMMIT|MEM_RESERVE;
  u8 *Result = (u8*)VirtualAlloc(0, AllocationSize, AllocationType, PAGE_READWRITE);

  if (!Result)
  {
    Error("Allocating %d bytes.", AllocationSize);
  }

  return Result;
}

bonsai_function b32
PlatformDeallocate(u8 *Base, umm Size)
{
  Assert( (umm)Base % PlatformGetPageSize() == 0);
  Assert(Size % PlatformGetPageSize() == 0);
  b32 Result = (b32)VirtualFree(Base, 0, MEM_RELEASE);
  if (Result)
  {
  }
  else
  {
    char* Buffer;

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        0,
        GetLastError(),
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR)&Buffer,
        0,
        0 );

    Error("Deallocating ptr(%llu) size(%llu) message(%s)", Base, Size, Buffer);
  }
  return Result;
}

bonsai_function b32
PlatformSetProtection(u8 *Base, u64 Size, memory_protection_type Protection)
{
  Assert(Size);
  b32 Result = False;
  u64 PageSize = PlatformGetPageSize();
  if ( (umm)Base % PageSize == 0 &&
            Size % PageSize == 0 )
  {
    u32 NativeProt = 0;
    switch (Protection)
    {
      case MemoryProtection_RW:
      {
        NativeProt = PAGE_READWRITE;
      } break;

      case MemoryProtection_Protected:
      {
        NativeProt = PAGE_NOACCESS;
      } break;
    }

    u32 OldProtect = 0;
    CAssert(sizeof(u32) == sizeof(DWORD));
    if (VirtualProtect( (void*)Base, Size, NativeProt, (PDWORD)&OldProtect))
    {
      Result = True;
    }
  }
  else
  {
    InvalidCodePath();
  }

  Assert(Result);
  return Result;
}

bonsai_function u32
PlatformGetLogicalCoreCount()
{
  SYSTEM_INFO sysinfo;
  GetSystemInfo(&sysinfo);
  u32 Result = (u32)sysinfo.dwNumberOfProcessors;
  return Result;
}

bonsai_function void
PlatformDebugStacktrace()
{
  // TODO(Jesse): Implement this.
  //
  // Helpful answer on how to get started
  // https://stackoverflow.com/questions/26398064/counterpart-to-glibcs-backtrace-and-backtrace-symbols-on-windows
  //
  // using these APIs
  //
  // https://docs.microsoft.com/en-us/previous-versions/windows/desktop/legacy/bb204633(v=vs.85)?redirectedfrom=MSDN
  // https://docs.microsoft.com/en-us/windows/win32/api/dbghelp/nf-dbghelp-symfromaddr?redirectedfrom=MSDN
  //

  Warn("Stack traces unavailable on windows.");
}
