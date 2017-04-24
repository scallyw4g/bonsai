#ifndef UNIX_PLATFORM_CPP
#define UNIX_PLATFORM_CPP

#include <unistd.h>
#include <stdio.h>

#include <unix_platform.h>
#include <platform.h>

#include <GL/glext.h>

static Display *dpy;
static GLXContext glc;

inline bool
AtomicCompareExchange( volatile unsigned int *Source, unsigned int Exchange, unsigned int Comparator )
{
  bool Result = __sync_bool_compare_and_swap ( Source, Comparator, Exchange );
  return Result;
}

inline void
ThreadSleep( semaphore Semaphore )
{
  /* WaitForSingleObject( Semaphore, INFINITE ); */
  return;
}

inline void
WakeThread( semaphore Semaphore )
{
  /* ReleaseSemaphore( Semaphore, 1, 0 ); */
  return;
}

semaphore
CreateSemaphore( int ThreadCount )
{
  /* semaphore Result = CreateSemaphore( 0, 0, ThreadCount, 0); */
  return 0;
}

int
GetLogicalCoreCount()
{
  int Result = sysconf(_SC_NPROCESSORS_ONLN);
  return Result;
}

thread_id
CreateThread( void* (*ThreadMain)(void*), thread_startup_params *Params)
{
  pthread_t Thread;

  pthread_attr_t Attribs;
  pthread_attr_init(&Attribs);

  Params->Self.ID = pthread_create(&Thread, &Attribs, ThreadMain, Params);

  return Params->Self.ID;
}

#define CompleteAllWrites  asm volatile("" ::: "memory"); _mm_sfence()

__inline__ unsigned long long
GetCycleCount()
{
  unsigned hi, lo;
  __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));

  unsigned long long Result = ( (unsigned long long)lo)|( ((unsigned long long)hi)<<32 );

  return Result;
}

void
CloseLibrary(shared_lib Lib)
{
  int e = dlclose(Lib);
  Assert(!e);

  return;
}

inline void*
OpenLibrary(const char *filename)
{
  void* Result = dlopen(filename, RTLD_NOW);

  if (!Result)
  {
    char *error = dlerror();
    printf("Error loading library: %s \n", error);
    Assert(False);
  }
  else
  {
    printf("Game Lib loaded! \n");
  }

  return Result;
}

window
OpenAndInitializeWindow( int WindowWidth, int WindowHeight )
{
  GLint GlAttribs[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };

  dpy = XOpenDisplay(0);
  if (!dpy) { printf(" Cannot connect to X Server \n"); return False; }

  window RootWindow = DefaultRootWindow(dpy);
  if (!RootWindow) { printf(" Unable to get RootWindow \n"); return False; }

  XVisualInfo *VisualInfo = glXChooseVisual(dpy, 0, GlAttribs);
  if (!VisualInfo) { printf(" Unable to get Visual Info \n"); return False; }

  Colormap ColorInfo = XCreateColormap(dpy, RootWindow, VisualInfo->visual, AllocNone);

  XSetWindowAttributes WindowAttribs;
  WindowAttribs.colormap = ColorInfo;
  WindowAttribs.event_mask = ExposureMask | KeyPressMask;

  Window win = XCreateWindow(dpy, RootWindow, 0, 0, 600, 600, 0, VisualInfo->depth, InputOutput, VisualInfo->visual, CWColormap | CWEventMask, &WindowAttribs);
  if (!win) { printf(" Unable to Create Window \n"); return False; }

  XMapWindow(dpy, win);
  XStoreName(dpy, win, "Bonsai");

  glc = glXCreateContext(dpy, VisualInfo, NULL, GL_TRUE);
  if (!glc) { printf(" Unable to Create GLXContext \n"); return False; }

  glXMakeCurrent(dpy, win, glc);

  return win;
}

inline GameCallback
GetProcFromLib(shared_lib Lib, const char *Name)
{
  GameCallback Result = (GameCallback)dlsym(Lib, Name);
  return Result;
}

/*
 *  Returns nanoseconds
 */
inline real64
GetHighPrecisionClock()
{
  struct timespec Time;
  clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &Time);
  return Time.tv_nsec;
}

void
InitializeOpenGlExtensions(gl *GL)
{
  GL->glCreateShader = (PFNGLCREATESHADERPROC)glXGetProcAddress((GLubyte*)"glCreateShader");;
  GL->glShaderSource = (PFNGLSHADERSOURCEPROC)glXGetProcAddress((GLubyte*)"glShaderSource");
  GL->glCompileShader = (PFNGLCOMPILESHADERPROC)glXGetProcAddress((GLubyte*)"glCompileShader");
  GL->glGetShaderiv = (PFNGLGETSHADERIVPROC)glXGetProcAddress((GLubyte*)"glGetShaderiv");
  GL->glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)glXGetProcAddress((GLubyte*)"glGetShaderInfoLog");
  GL->glAttachShader = (PFNGLATTACHSHADERPROC)glXGetProcAddress((GLubyte*)"glAttachShader");
  GL->glDetachShader = (PFNGLDETACHSHADERPROC)glXGetProcAddress((GLubyte*)"glDetachShader");
  GL->glDeleteShader = (PFNGLDELETESHADERPROC)glXGetProcAddress((GLubyte*)"glDeleteShader");
  GL->glCreateProgram = (PFNGLCREATEPROGRAMPROC)glXGetProcAddress((GLubyte*)"glCreateProgram");
  GL->glLinkProgram = (PFNGLLINKPROGRAMPROC)glXGetProcAddress((GLubyte*)"glLinkProgram");
  GL->glGetProgramiv = (PFNGLGETPROGRAMIVPROC)glXGetProcAddress((GLubyte*)"glGetProgramiv");
  GL->glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)glXGetProcAddress((GLubyte*)"glGetProgramInfoLog");
  GL->glUseProgram = (PFNGLUSEPROGRAMPROC)glXGetProcAddress((GLubyte*)"glUseProgram");
  GL->glDeleteProgram = (PFNGLDELETEPROGRAMPROC)glXGetProcAddress((GLubyte*)"glDeleteProgram");
  GL->glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)glXGetProcAddress((GLubyte*)"glGetUniformLocation");
  GL->glGenFramebuffers = (PFNGLGENFRAMEBUFFERSPROC)glXGetProcAddress((GLubyte*)"glGenFramebuffers");
  GL->glBindFramebuffer = (PFNGLBINDFRAMEBUFFERPROC)glXGetProcAddress((GLubyte*)"glBindFramebuffer");

  // TODO(Jesse): This function appears to not work on ES 3.1 ..??
  // GL->glFramebufferTexture = (PFNGLFRAMEBUFFERTEXTUREPROC)glXGetProcAddress((GLubyte*)"glFramebufferTexture");

  GL->glFramebufferTexture2D = (PFNGLFRAMEBUFFERTEXTURE2DPROC)glXGetProcAddress((GLubyte*)"glFramebufferTexture2D");
  GL->glCheckFramebufferStatus = (PFNGLCHECKFRAMEBUFFERSTATUSPROC)glXGetProcAddress((GLubyte*)"glCheckFramebufferStatus");
  GL->glCompressedTexImage2D = (PFNGLCOMPRESSEDTEXIMAGE2DPROC)glXGetProcAddress((GLubyte*)"glCompressedTexImage2D");
  GL->glGenBuffers = (PFNGLGENBUFFERSPROC)glXGetProcAddress((GLubyte*)"glGenBuffers");
  GL->glBindBuffer = (PFNGLBINDBUFFERPROC)glXGetProcAddress((GLubyte*)"glBindBuffer");
  GL->glBufferData = (PFNGLBUFFERDATAPROC)glXGetProcAddress((GLubyte*)"glBufferData");
  GL->glDrawBuffers = (PFNGLDRAWBUFFERSPROC)glXGetProcAddress((GLubyte*)"glDrawBuffers");
  GL->glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)glXGetProcAddress((GLubyte*)"glDeleteBuffers");
  GL->glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)glXGetProcAddress((GLubyte*)"glVertexAttribPointer");
  GL->glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)glXGetProcAddress((GLubyte*)"glEnableVertexAttribArray");
  GL->glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC)glXGetProcAddress((GLubyte*)"glDisableVertexAttribArray");
  GL->glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)glXGetProcAddress((GLubyte*)"glGenVertexArrays");
  GL->glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)glXGetProcAddress((GLubyte*)"glBindVertexArray");
  GL->glUniform3fv = (PFNGLUNIFORM3FVPROC)glXGetProcAddress((GLubyte*)"glUniform3fv");
  GL->glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)glXGetProcAddress((GLubyte*)"glUniformMatrix4fv");
  GL->glUniform1i = (PFNGLUNIFORM1IPROC)glXGetProcAddress((GLubyte*)"glUniform1i");
  GL->glActiveTexture = (PFNGLACTIVETEXTUREPROC)glXGetProcAddress((GLubyte*)"glActiveTexture");

  return;
}


#endif
