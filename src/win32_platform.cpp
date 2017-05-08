#ifndef WIN32_PLATFORM
#define WIN32_PLATFORM

#include <win32_platform.h>
#include <platform.h>

#include <chrono> // Timer

#define BONSAI_MAIN(void) int CALLBACK WinMain( HINSTANCE AppHandle, HINSTANCE Ignored, LPSTR CmdLine, int CmdShow )

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

inline r64
GetHighPrecisionClock()
{
  GLOBAL_VARIABLE auto FirstTime = std::chrono::high_resolution_clock::now();
  // cout << "FirstTime Time : " << chrono::time_point_cast<chrono::nanoseconds>(FirstTime).time_since_epoch().count() << " ns \n";

  r64 Result = (r64)(std::chrono::high_resolution_clock::now() - FirstTime).count();
  // cout << "Time/iter, clock: " << chrono::duration_cast<chrono::nanoseconds>(Result).count() << " ns \n";

  return Result;
}

inline r64
ComputeDtForFrame(r64 *LastTime)
{
  r64 CurrentTime = GetHighPrecisionClock();
  r64 Dt = (CurrentTime - *LastTime) / 1000000000;

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

  if (hPalette)
  {
    DeleteObject(hPalette);
  }

  ReleaseDC(Os->Window, Os->Display);
  PostQuitMessage(0);

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
    } return 0;


    case WM_DESTROY:
    {
      os *Os = (os*)GetWindowLongPtr(hWnd, 0);
      Os->ContinueRunning = false;

    } return 0;

    case WM_SIZE:
    {
      platform *Plat = (platform*)GetWindowLongPtr(hWnd, PLATFORM_OFFSET);

      int WinWidth = LOWORD(lParam);
      int WinHeight = HIWORD(lParam);

      Plat->WindowWidth = WinWidth;
      Plat->WindowHeight = WinHeight;
    } return 0;

    case WM_PALETTECHANGED:
    {
      os *Os = (os*)GetWindowLongPtr(hWnd, 0);

      /* realize palette if this is *not* the current window */
      if (Os->GlContext && hPalette && (HWND) wParam != hWnd)
      {
        UnrealizeObject(hPalette);
        SelectPalette(Os->Display, hPalette, FALSE);
        RealizePalette(Os->Display);
        SwapBuffers(Os->Display);
      }

    } return 0;

    case WM_QUERYNEWPALETTE:
    {
      os *Os = (os*)GetWindowLongPtr(hWnd, 0);

      /* realize palette if this is the current window */
      if (Os->GlContext && hPalette)
      {
        UnrealizeObject(hPalette);
        SelectPalette(Os->Display, hPalette, FALSE);
        RealizePalette(Os->Display);
        SwapBuffers(Os->Display);
        return TRUE;
      }

    } return 0;

    case WM_PAINT:
    {
      os *Os = (os*)GetWindowLongPtr(hWnd, 0);

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
      platform *Plat = (platform*)GetWindowLongPtr(hWnd, PLATFORM_OFFSET);
      Plat->Input.LMB = True;

      if (Plat->MouseClickP == InvalidMouseP)
      {
        Plat->MouseClickP = Plat->GetMouseP();

        // I'm not 100% sure -1,-1 is always invalid..
        Assert(Plat->MouseClickP != InvalidMouseP);
      }

    } return 0;

    case WM_LBUTTONUP:
    {
      platform *Plat = (platform*)GetWindowLongPtr(hWnd, PLATFORM_OFFSET);
	  Plat->Input.LMB = FALSE;
	  Plat->MouseClickP = InvalidMouseP;
    } return 0;

    case WM_RBUTTONDOWN:
    {
      platform *Plat = (platform*)GetWindowLongPtr(hWnd, PLATFORM_OFFSET);
      Plat->Input.RMB = True;
    } return 0;

    case WM_MOUSEMOVE:
    {
      /* s32 xPos = GET_X_LPARAM(lParam); */
      /* s32 yPos = GET_Y_LPARAM(lParam); */
      /* Info(" X: %d, Y: %d", xPos, yPos); */

    } return 0;

    case WM_KEYUP:
    {
      switch ((int)wParam)
      {

        case 0x57:
        {
          platform *Plat = (platform*)GetWindowLongPtr(hWnd, PLATFORM_OFFSET);
          Plat->Input.W = False;
        } return 0;

        case 0x44:
        {
          platform *Plat = (platform*)GetWindowLongPtr(hWnd, PLATFORM_OFFSET);
          Plat->Input.D = False;
        } return 0;

        case 0x53:
        {
          platform *Plat = (platform*)GetWindowLongPtr(hWnd, PLATFORM_OFFSET);
          Plat->Input.S = False;
        } return 0;

        case 0x41:
        {
          platform *Plat = (platform*)GetWindowLongPtr(hWnd, PLATFORM_OFFSET);
          Plat->Input.A = False;
        } return 0;

        case 0x51:
        {
          platform *Plat = (platform*)GetWindowLongPtr(hWnd, PLATFORM_OFFSET);
          Plat->Input.Q = False;
        } return 0;

        case 0x45:
        {
          platform *Plat = (platform*)GetWindowLongPtr(hWnd, PLATFORM_OFFSET);
          Plat->Input.E = False;
        } return 0;

        case VK_F11:
        {
          platform *Plat = (platform*)GetWindowLongPtr(hWnd, PLATFORM_OFFSET);
          Plat->Input.F11 = False;
        } return 0;

        default:
        {
          // Ignore all other keypresses
        } break;
      }

    }
    case WM_KEYDOWN:
    {
      switch ((int)wParam)
      {
        case VK_ESCAPE:
        {
          DestroyWindow(hWnd);
          return 0;
        } return 0;

        case 0x57:
        {
          platform *Plat = (platform*)GetWindowLongPtr(hWnd, PLATFORM_OFFSET);
          Plat->Input.W = True;
        } return 0;

        case 0x44:
        {
          platform *Plat = (platform*)GetWindowLongPtr(hWnd, PLATFORM_OFFSET);
          Plat->Input.D = True;
        } return 0;

        case 0x53:
        {
          platform *Plat = (platform*)GetWindowLongPtr(hWnd, PLATFORM_OFFSET);
          Plat->Input.S = True;
        } return 0;

        case 0x41:
        {
          platform *Plat = (platform*)GetWindowLongPtr(hWnd, PLATFORM_OFFSET);
          Plat->Input.A = True;
        } return 0;

        case 0x51:
        {
          platform *Plat = (platform*)GetWindowLongPtr(hWnd, PLATFORM_OFFSET);
          Plat->Input.Q = True;
        } return 0;

        case 0x45:
        {
          platform *Plat = (platform*)GetWindowLongPtr(hWnd, PLATFORM_OFFSET);
          Plat->Input.E = True;
        } return 0;

        case VK_F11:
        {
          platform *Plat = (platform*)GetWindowLongPtr(hWnd, PLATFORM_OFFSET);
          Plat->Input.F11 = True;
        } return 0;

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
      100, 800, Plat->WindowWidth, Plat->WindowHeight,
      NULL, NULL, AppHandle, NULL);

  Os->Display = GetDC(Os->Window);
  setupPixelFormat(Os->Display);
  hPalette = setupPalette(Os->Display);
  Os->GlContext = wglCreateContext(Os->Display);
  wglMakeCurrent(Os->Display, Os->GlContext);

  {
    SetWindowLongPtr(Os->Window, 0, (LONG_PTR)Os);
    int e = GetLastError();
    Assert(!e);
  }

  {
    SetWindowLongPtr(Os->Window, sizeof(Os), (LONG_PTR)Plat);
    int e = GetLastError();
    Assert(!e);
  }


  ShowWindow(Os->Window, SW_SHOW);
  UpdateWindow(Os->Window);

  return True;
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
    Error("Error loading library: %s", LibPath);
  }
  else
  {
    Info("Game Lib loaded!");
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

inline void
SetMouseP(v2 P)
{
  s32 e = SetCursorPos(P.x, P.y);
  Assert( e != 0 );
  return;
}

inline v2
GetMouseP()
{
  POINT P;
  GetCursorPos(&P);
  v2 Result { P.x, P.y };
  return Result;
}

#endif
