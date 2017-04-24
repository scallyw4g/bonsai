#ifndef WIN32_PLATFORM
#define WIN32_PLATFORM

#include <win32_platform.h>
#include <platform.h>

#include <GL/wglext.h>


#define BONSAI_MAIN(void) int CALLBACK WinMain( HINSTANCE AppHandle, HINSTANCE Ignored, LPSTR CmdLine, int CmdShow )

inline unsigned int
AtomicCompareExchange( volatile unsigned int *Source, unsigned int Exchange, unsigned int Comparator )
{
  unsigned int Result = InterlockedCompareExchange( (LONG volatile *)Source, Exchange, Comparator);
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
  Assert(false);
  return 0;
}

void
SwapBuffers()
{
  Assert(false);
  return;
}

void
Terminate()
{
  Assert(false);
  return;
}

void
InitializeOpenGlExtensions(gl_extensions *GL)
{

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
  static HDC hDC;
  static HGLRC hGLRC;
  static HPALETTE hPalette;

  switch (message) {
     case WM_CREATE:
         /* initialize OpenGL rendering */
         hDC = GetDC(hWnd);
         setupPixelFormat(hDC);
         hPalette = setupPalette(hDC);
         hGLRC = wglCreateContext(hDC);
         wglMakeCurrent(hDC, hGLRC);
         return 0;
     case WM_DESTROY:
         /* finish OpenGL rendering */
         if (hGLRC) {
             wglMakeCurrent(NULL, NULL);
             wglDeleteContext(hGLRC);
         }
         if (hPalette) {
             DeleteObject(hPalette);
         }
         ReleaseDC(hWnd, hDC);
         PostQuitMessage(0);
         return 0;
     case WM_SIZE:
         /* track window size changes */
         if (hGLRC) {
             return 0;
         }
     case WM_PALETTECHANGED:
         /* realize palette if this is *not* the current window */
         if (hGLRC && hPalette && (HWND) wParam != hWnd) {
             UnrealizeObject(hPalette);
             SelectPalette(hDC, hPalette, FALSE);
             RealizePalette(hDC);
             SwapBuffers(hDC);
             break;
         }
         break;
     case WM_QUERYNEWPALETTE:
         /* realize palette if this is the current window */
         if (hGLRC && hPalette) {
             UnrealizeObject(hPalette);
             SelectPalette(hDC, hPalette, FALSE);
             RealizePalette(hDC);
             SwapBuffers(hDC);
             return TRUE;
         }
         break;
     case WM_PAINT:
         {
             PAINTSTRUCT ps;
             BeginPaint(hWnd, &ps);
             if (hGLRC) {
                 SwapBuffers(hDC);
             }
             EndPaint(hWnd, &ps);
             return 0;
         }
         break;
     case WM_CHAR:
         /* handle keyboard input */
         switch ((int)wParam) {
         case VK_ESCAPE:
             DestroyWindow(hWnd);
             return 0;
         default:
             break;
         }
         break;
     default:
         break;
     }

   return DefWindowProc(hWnd, message, wParam, lParam);
}

window
OpenAndInitializeWindow( int WindowWidth, int WindowHeight )
{
  WNDCLASS wndClass;
  window Window;

  HINSTANCE AppHandle = GetModuleHandle(0);

  LPCSTR className = "Bonsai";

  // Register window class
  wndClass.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
  wndClass.lpfnWndProc = WindowMessageCallback;
  wndClass.cbClsExtra = 0;
  wndClass.cbWndExtra = 0;
  wndClass.hInstance = AppHandle;
  wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
  wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
  wndClass.hbrBackground = CreateSolidBrush(COLOR_BACKGROUND);
  wndClass.lpszMenuName = NULL;
  wndClass.lpszClassName = className;
  RegisterClass(&wndClass);

  int e = GetLastError();

  Window = CreateWindow(
      className, className,
      WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
      0, 0, WindowWidth, WindowHeight,
      NULL, NULL, AppHandle, NULL);

  e = GetLastError();

  ShowWindow(Window, SW_SHOW);
  UpdateWindow(Window);

  return Window;

  /* int error = glGetError(); */
  /* if ( error == GL_INVALID_ENUM  || error == GL_NO_ERROR ) */
  /* { */
  /*   // Everythings fine, this is a design flaw: */
  /*   // http://stackoverflow.com/questions/20034615/why-does-glewinit-result-in-gl-invalid-enum-after-making-some-calls-to-glfwwin */
  /* } */
  /* else */
  /* { */
  /*   Assert(false); // We hit a real error */
  /* } */

  // glClearColor(0.25f, 0.25f, 0.25f, 0.25f);

  // Enable depth test
  // glEnable(GL_DEPTH_TEST);

  // Accept fragment if it closer to the camera than the former one
  // glDepthFunc(GL_LESS);
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


#endif
