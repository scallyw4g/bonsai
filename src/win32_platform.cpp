#ifndef WIN32_PLATFORM
#define WIN32_PLATFORM

#include <win32_platform.h>
#include <platform.h>

#include <chrono> // Timer

#include <GL/wglext.h>

#define BONSAI_MAIN(void) int CALLBACK WinMain( HINSTANCE AppHandle, HINSTANCE Ignored, LPSTR CmdLine, int CmdShow )

static HDC hDC;
static HGLRC hGLRC;
static HPALETTE hPalette;

inline b32
AtomicCompareExchange( volatile unsigned int *Source, unsigned int Exchange, unsigned int Comparator )
{
  u32 Val = InterlockedCompareExchange( (LONG volatile *)Source, Exchange, Comparator);

  b32 Result = (Val == Comparator);
  return Result;
}

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
CreateSemaphore( int ThreadCount )
{
  semaphore Result = CreateSemaphore( 0, 0, ThreadCount, 0);
  return Result;
}

int
GetLogicalCoreCount()
{
  SYSTEM_INFO sysinfo;
  GetSystemInfo(&sysinfo);
  int Result = sysinfo.dwNumberOfProcessors;
  return Result;
}

thread_id
CreateThread( LPTHREAD_START_ROUTINE ThreadMain, thread_startup_params *Params)
{
  DWORD flags = 0;
  int StackSize = 0;

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

unsigned long long
GetCycleCount()
{
  unsigned long long Result = __rdtsc();
  return Result;
}

u64
GetHighPrecisionClock()
{
  u64 Result = std::chrono::high_resolution_clock::now().time_since_epoch().count();
  return Result;
}

void
Terminate()
{
  Assert(false);
  return;
}

void
setupPixelFormat(HDC hDC)
{
    PIXELFORMATDESCRIPTOR pfd = {
        sizeof(PIXELFORMATDESCRIPTOR),  /* size */
        1,                              /* version */
        PFD_SUPPORT_OPENGL |
        PFD_DRAW_TO_WINDOW |
        PFD_DOUBLEBUFFER,               /* support double-buffering */
        PFD_TYPE_RGBA,                  /* color type */
        16,                             /* prefered color depth */
        0, 0, 0, 0, 0, 0,               /* color bits (ignored) */
        0,                              /* no alpha buffer */
        0,                              /* alpha bits (ignored) */
        0,                              /* no accumulation buffer */
        0, 0, 0, 0,                     /* accum bits (ignored) */
        16,                             /* depth buffer */
        0,                              /* no stencil buffer */
        0,                              /* no auxiliary buffers */
        PFD_MAIN_PLANE,                 /* main layer */
        0,                              /* reserved */
        0, 0, 0,                        /* no layer, visible, damage masks */
    };
    int pixelFormat;

    pixelFormat = ChoosePixelFormat(hDC, &pfd);
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
    int paletteSize;

    DescribePixelFormat(hDC, pixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

    if (pfd.dwFlags & PFD_NEED_PALETTE) {
        paletteSize = 1 << pfd.cColorBits;
    } else {
        return 0;
    }

    pPal = (LOGPALETTE*)
        malloc(sizeof(LOGPALETTE) + paletteSize * sizeof(PALETTEENTRY));
    pPal->palVersion = 0x300;
    pPal->palNumEntries = paletteSize;

    /* build a simple RGB color palette */
    {
        int redMask = (1 << pfd.cRedBits) - 1;
        int greenMask = (1 << pfd.cGreenBits) - 1;
        int blueMask = (1 << pfd.cBlueBits) - 1;
        int i;

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

  switch (message)
  {
    case WM_CREATE:
    {
      hDC = GetDC(hWnd);
      setupPixelFormat(hDC);
      hPalette = setupPalette(hDC);
      hGLRC = wglCreateContext(hDC);
      wglMakeCurrent(hDC, hGLRC);
      return 0;

    } break;


    case WM_DESTROY:
    {
      if (hGLRC) // Cleanup Opengl context
      {
        wglMakeCurrent(NULL, NULL);
        wglDeleteContext(hGLRC);
      }

      if (hPalette)
      {
        DeleteObject(hPalette);
      }

      ReleaseDC(hWnd, hDC);
      PostQuitMessage(0);

      return 0;

    } break;

    case WM_SIZE:
    {
      // Handle resize ?
      return 0;

    } break;

    case WM_PALETTECHANGED:
    {
      /* realize palette if this is *not* the current window */
      if (hGLRC && hPalette && (HWND) wParam != hWnd)
      {
        UnrealizeObject(hPalette);
        SelectPalette(hDC, hPalette, FALSE);
        RealizePalette(hDC);
        SwapBuffers(hDC);
        break;
      }

    } break;

    case WM_QUERYNEWPALETTE:
    {
      /* realize palette if this is the current window */
      if (hGLRC && hPalette)
      {
        UnrealizeObject(hPalette);
        SelectPalette(hDC, hPalette, FALSE);
        RealizePalette(hDC);
        SwapBuffers(hDC);
        return TRUE;
      }

    } break;

    case WM_PAINT:
    {
      PAINTSTRUCT ps;
      BeginPaint(hWnd, &ps);
      if (hGLRC)
      {
        SwapBuffers(hDC);
      }
      EndPaint(hWnd, &ps);
      return 0;

    } break;


    /*
     * User Input
     */
    case WM_LBUTTONDOWN:
    {

      platform *Plat = (platform*)GetWindowLongPtr(hWnd, 0);
      Plat->Input.LMB = True;
      break;
    }

    case WM_RBUTTONDOWN:
    {
      platform *Plat = (platform*)GetWindowLongPtr(hWnd, 0);
      Plat->Input.RMB = True;
      break;
    }

    case WM_KEYDOWN:
    {
      switch ((int)wParam)
      {
        case VK_ESCAPE:
        {
          DestroyWindow(hWnd);
          return 0;
        } break;

        case 0x57:
        {
          platform *Plat = (platform*)GetWindowLongPtr(hWnd, 0);
          Plat->Input.W = True;
        } break;

        case 0x44:
        {
          platform *Plat = (platform*)GetWindowLongPtr(hWnd, 0);
          Plat->Input.D = True;
        } break;

        case 0x53:
        {
          platform *Plat = (platform*)GetWindowLongPtr(hWnd, 0);
          Plat->Input.S = True;
        } break;

        case 0x41:
        {
          platform *Plat = (platform*)GetWindowLongPtr(hWnd, 0);
          Plat->Input.A = True;
        } break;

        case 0x51:
        {
          platform *Plat = (platform*)GetWindowLongPtr(hWnd, 0);
          Plat->Input.Q = True;
        } break;

        case 0x45:
        {
          platform *Plat = (platform*)GetWindowLongPtr(hWnd, 0);
          Plat->Input.E = True;
        } break;

        case VK_F11:
        {
          platform *Plat = (platform*)GetWindowLongPtr(hWnd, 0);
          Plat->Input.F11 = True;
        } break;

        default:
        {
          // Ignore all other keypresses
        } break;
      }

    }

    default:
    {
      // Ignore all other window messages
    } break;

  }

   return DefWindowProc(hWnd, message, wParam, lParam);
}

void
OpenAndInitializeWindow( os *Os, platform *Plat, int WindowWidth, int WindowHeight )
{
  WNDCLASS wndClass;

  HINSTANCE AppHandle = GetModuleHandle(0);

  LPCSTR className = "Bonsai";

  // Register window class
  wndClass.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
  wndClass.lpfnWndProc = WindowMessageCallback;
  wndClass.cbClsExtra = 0;
  wndClass.cbWndExtra = sizeof(Plat);
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
      100, 800, WindowWidth, WindowHeight,
      NULL, NULL, AppHandle, NULL);

  SetWindowLongPtr(Os->Window, 0, (LONG_PTR)Plat);

  ShowWindow(Os->Window, SW_SHOW);
  UpdateWindow(Os->Window);

  // Enable depth test
  // glEnable(GL_DEPTH_TEST);

  // Accept fragment if it closer to the camera than the former one
  // glDepthFunc(GL_LESS);

  return;
}


inline FARPROC
GetProcFromLib(shared_lib Lib, const char *Name)
{
  FARPROC Result = (FARPROC)GetProcAddress(Lib, Name);
  return Result;
}

inline shared_lib
OpenLibrary(const char *LibPath)
{
  shared_lib Result = LoadLibrary(LibPath);

  if (!Result)
  {
    printf("Error loading library: %s \n", LibPath);
    Assert(False);
  }
  else
  {
    printf("Game Lib loaded! \n");
  }

  return Result;
}

inline b32
CloseLibrary(shared_lib Lib)
{
  b32 Result = FreeLibrary(Lib);
  return Result;
}

#define CwdBufferLen 2048
DEBUG_GLOBAL char CwdBuffer[CwdBufferLen];

inline char*
GetCwd()
{
  GetCurrentDirectory( CwdBufferLen, CwdBuffer );
  return &CwdBuffer[0];
}

b32
ProcessOsMessages(os *Os)
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
#endif
