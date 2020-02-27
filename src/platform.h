
#define WORK_QUEUE_SIZE (65536*8)

struct platform;
struct network_connection;
struct game_state;
struct memory_arena;
struct heap_allocator;
struct hotkeys;
struct work_queue_entry;
struct os;
struct thread_local_state;


#define EXPAND(E) #E
#define TO_STRING(Text) EXPAND(Text)

#define BONSAI_API_MAIN_THREAD_CALLBACK_NAME          MainThreadCallback
#define BONSAI_API_MAIN_THREAD_INIT_CALLBACK_NAME     InitMainThreadCallback
#define BONSAI_API_WORKER_THREAD_CALLBACK_NAME        WorkerThreadCallback
#define BONSAI_API_WORKER_THREAD_INIT_CALLBACK_NAME   InitWorkerThreadCallback

#define BONSAI_API_MAIN_THREAD_CALLBACK_PARAMS         platform *Plat, game_state *GameState, hotkeys *Hotkeys
#define BONSAI_API_MAIN_THREAD_INIT_CALLBACK_PARAMS    platform *Plat, memory_arena *GameMemory, get_debug_state_proc GetDebugState_in
#define BONSAI_API_WORKER_THREAD_CALLBACK_PARAMS       work_queue_entry* Entry, thread_local_state* Thread
#define BONSAI_API_WORKER_THREAD_INIT_CALLBACK_PARAMS  thread_local_state* Thread, game_state* GameState


#define BONSAI_API_MAIN_THREAD_CALLBACK() \
  exported_function void BONSAI_API_MAIN_THREAD_CALLBACK_NAME(BONSAI_API_MAIN_THREAD_CALLBACK_PARAMS)

#define BONSAI_API_MAIN_THREAD_INIT_CALLBACK() \
  exported_function game_state* BONSAI_API_MAIN_THREAD_INIT_CALLBACK_NAME(BONSAI_API_MAIN_THREAD_INIT_CALLBACK_PARAMS)

#define BONSAI_API_WORKER_THREAD_CALLBACK() \
  exported_function void BONSAI_API_WORKER_THREAD_CALLBACK_NAME(BONSAI_API_WORKER_THREAD_CALLBACK_PARAMS)

#define BONSAI_API_WORKER_THREAD_INIT_CALLBACK() \
  exported_function void BONSAI_API_WORKER_THREAD_INIT_CALLBACK_NAME(BONSAI_API_WORKER_THREAD_INIT_CALLBACK_PARAMS)

typedef void (*GameCallback)(void*);


typedef void (*bonsai_main_thread_callback)        (BONSAI_API_MAIN_THREAD_CALLBACK_PARAMS);
typedef void (*bonsai_worker_thread_init_callback) (BONSAI_API_WORKER_THREAD_INIT_CALLBACK_PARAMS);

typedef void        (*bonsai_worker_thread_callback)    (BONSAI_API_WORKER_THREAD_CALLBACK_PARAMS);
typedef game_state* (*bonsai_main_thread_init_callback) (BONSAI_API_MAIN_THREAD_INIT_CALLBACK_PARAMS);

struct work_queue_entry_copy_buffer
{
  untextured_3d_geometry_buffer* Src;
  untextured_3d_geometry_buffer Dest;
  v3 Basis;
};

struct work_queue_entry_init_world_chunk
{
  void *Input;
};



meta(
  d_union( work_queue_entry,
  {
    work_queue_entry_init_world_chunk,
    work_queue_entry_copy_buffer,
  })
)
#include <metaprogramming/output/d_union_work_queue_entry>

struct thread
{
  u32 ThreadIndex;
  thread_id ID;
};

struct work_queue
{
  volatile u32 EnqueueIndex;
  volatile u32 DequeueIndex;
  work_queue_entry *Entries;
  semaphore* GlobalQueueSemaphore;
};

struct thread_startup_params
{
  bonsai_worker_thread_init_callback InitProc;
  game_state* GameState;
  work_queue* LowPriority;
  work_queue* HighPriority;
  thread Self;
};

/* struct gl_extensions */
/* { */
/*   PFNGLCREATESHADERPROC glCreateShader; */
/*   PFNGLSHADERSOURCEPROC glShaderSource; */
/*   PFNGLCOMPILESHADERPROC glCompileShader; */
/*   PFNGLGETSHADERIVPROC glGetShaderiv; */
/*   PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog; */
/*   PFNGLATTACHSHADERPROC glAttachShader; */
/*   PFNGLDETACHSHADERPROC glDetachShader; */
/*   PFNGLDELETESHADERPROC glDeleteShader; */
/*   PFNGLCREATEPROGRAMPROC glCreateProgram; */
/*   PFNGLLINKPROGRAMPROC glLinkProgram; */
/*   PFNGLGETPROGRAMIVPROC glGetProgramiv; */
/*   PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog; */
/*   PFNGLUSEPROGRAMPROC glUseProgram; */
/*   PFNGLDELETEPROGRAMPROC glDeleteProgram; */
/*   PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation; */
/*   PFNGLGENFRAMEBUFFERSPROC glGenFramebuffers; */
/*   PFNGLBINDFRAMEBUFFERPROC glBindFramebuffer; */
/*   /1* PFNGLFRAMEBUFFERTEXTUREPROC glFramebufferTexture; *1/ */
/*   PFNGLFRAMEBUFFERTEXTURE2DPROC glFramebufferTexture2D; */
/*   PFNGLCHECKFRAMEBUFFERSTATUSPROC glCheckFramebufferStatus; */
/*   PFNGLCOMPRESSEDTEXIMAGE2DPROC glCompressedTexImage2D; */
/*   PFNGLGENBUFFERSPROC glGenBuffers; */
/*   PFNGLBINDBUFFERPROC glBindBuffer; */
/*   PFNGLBUFFERDATAPROC glBufferData; */
/*   PFNGLDRAWBUFFERSPROC glDrawBuffers; */
/*   PFNGLDELETEBUFFERSPROC glDeleteBuffers; */
/*   PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer; */
/*   PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray; */
/*   PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray; */
/*   PFNGLGENVERTEXARRAYSPROC glGenVertexArrays; */
/*   PFNGLBINDVERTEXARRAYPROC glBindVertexArray; */
/*   PFNGLUNIFORM3FVPROC glUniform3fv; */
/*   PFNGLUNIFORM2FVPROC glUniform2fv; */
/*   PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv; */
/*   PFNGLUNIFORM1IPROC glUniform1i; */
/*   PFNGLACTIVETEXTUREPROC glActiveTexture; */
/*   PFNGLUNIFORM1FPROC glUniform1f; */
/*   PFNGLUNIFORM1UIPROC glUniform1ui; */

  // Platform specific (wgl / glX)
  /* PFNSWAPINTERVALPROC glSwapInterval; */
/* }; */

struct input
{
  input_event Escape;

  input_event Space;
  input_event Shift;
  input_event Ctrl;
  input_event Alt;

  input_event F12;
  input_event F11;
  input_event F10;
  input_event F9;
  input_event F8;
  input_event F7;
  input_event F6;
  input_event F5;
  input_event F4;
  input_event F3;
  input_event F2;
  input_event F1;

  input_event RMB;
  input_event LMB;
  input_event MMB;
  input_event W;
  input_event A;
  input_event S;
  input_event D;

  input_event Q;
  input_event E;
};

struct platform
{
  work_queue LowPriority;
  work_queue HighPriority;
  semaphore QueueSemaphore;

  thread_startup_params *Threads;

  network_connection Network = { Socket_NonBlocking, SERVER_IP };

  v2 MouseP;
  v2 MouseDP;

  memory_arena *Memory;

  /* gl_extensions GL; */

  r32 dt;
  s32 WindowWidth = SCR_WIDTH;
  s32 WindowHeight = SCR_HEIGHT;

  input Input;
};

struct os
{
  window Window;
  display Display;
  gl_context GlContext;

  b32 ContinueRunning = True;
};

void
DumpGlErrorEnum(u32 Error)
{
  if ( Error != 0 )
  {
    Error("%d", Error);
  }

  switch (Error)
  {
    case GL_INVALID_ENUM:
    {
      Error(" GL_INVALID_ENUM: An unacceptable value is specified for an enumerated argument. The offending command is ignored and has no other side effect than to set the error flag.\n");
    } break;

    case GL_INVALID_VALUE:
    {
      Error(" GL_INVALID_VALUE: A numeric argument is out of range. The offending command is ignored and has no other side effect than to set the error flag.\n");
    } break;

    case GL_INVALID_OPERATION:
    {
      Error(" GL_INVALID_OPERATION: The specified operation is not allowed in the current state. The offending command is ignored and has no other side effect than to set the error flag.\n");
    } break;

    case GL_INVALID_FRAMEBUFFER_OPERATION:
    {
      Error(" GL_INVALID_FRAMEBUFFER_OPERATION: The framebuffer object is not complete. The offending command is ignored and has no other side effect than to set the error flag.\n");
    } break;

    case GL_OUT_OF_MEMORY:
    {
      Error(" GL_OUT_OF_MEMORY: There is not enough memory left to execute the command. The state of the GL is undefined, except for the state of the error flags, after this error is recorded.\n");
    } break;

    case GL_STACK_UNDERFLOW:
    {
      Error(" GL_STACK_UNDERFLOW: An attempt has been made to perform an operation that would cause an internal stack to underflow.\n");
    } break;

    case GL_STACK_OVERFLOW:
    {
      Error(" GL_STACK_OVERFLOW: An attempt has been made to perform an operation that would cause an internal stack to overflow.\n");
    } break;

    case GL_NO_ERROR:
    {
      // Happy days :D
    } break;

    default :
    {
      Error("Some weird OpenGL shit happened\n");
    } break;
  }

  return;
}

function void
InitializeOpenGlExtensions(os* Os)
{
  Info("Initializing OpenGL Extensions");

#if 0
  const char* glxExtensionString = glXQueryExtensionsString(Os->Display, DefaultScreen(Os->Display));
  const char* glExtensionString = (const char*)glGetString(GL_EXTENSIONS);
  Debug(glExtensionString);
  Debug(glxExtensionString);
#endif

  // Somehow on linux this is irrelevant..
#if 1

#if 0

#define DefGlProc(ProcType, ProcName) \
  ProcType ProcName = (ProcType)bonsaiGlGetProcAddress(#ProcName); Assert(ProcName)

  /*
   * 1.3
   */
  DefGlProc(PFNGLCOMPRESSEDTEXIMAGE2DPROC, glCompressedTexImage2D);
  DefGlProc(PFNGLACTIVETEXTUREPROC, glActiveTexture);

  /*
   * 1.5
   */
  DefGlProc(PFNGLGENBUFFERSPROC, glGenBuffers);
  DefGlProc(PFNGLBINDBUFFERPROC, glBindBuffer);
  DefGlProc(PFNGLBUFFERDATAPROC, glBufferData);
  DefGlProc(PFNGLDELETEBUFFERSPROC, glDeleteBuffers);

  /*
   * 2.0
   */
  DefGlProc(PFNGLCREATESHADERPROC, glCreateShader);
  DefGlProc(PFNGLSHADERSOURCEPROC, glShaderSource);
  DefGlProc(PFNGLCOMPILESHADERPROC, glCompileShader);
  DefGlProc(PFNGLGETSHADERIVPROC, glGetShaderiv);
  DefGlProc(PFNGLGETSHADERINFOLOGPROC, glGetShaderInfoLog);
  DefGlProc(PFNGLATTACHSHADERPROC, glAttachShader);
  DefGlProc(PFNGLDETACHSHADERPROC, glDetachShader);
  DefGlProc(PFNGLDELETESHADERPROC, glDeleteShader);
  DefGlProc(PFNGLCREATEPROGRAMPROC, glCreateProgram);
  DefGlProc(PFNGLLINKPROGRAMPROC, glLinkProgram);
  DefGlProc(PFNGLGETPROGRAMIVPROC, glGetProgramiv);
  DefGlProc(PFNGLGETPROGRAMINFOLOGPROC, glGetProgramInfoLog);
  DefGlProc(PFNGLUSEPROGRAMPROC, glUseProgram);
  DefGlProc(PFNGLDELETEPROGRAMPROC, glDeleteProgram);
  DefGlProc(PFNGLGETUNIFORMLOCATIONPROC, glGetUniformLocation);
  DefGlProc(PFNGLDRAWBUFFERSPROC, glDrawBuffers);
  DefGlProc(PFNGLVERTEXATTRIBPOINTERPROC, glVertexAttribPointer);
  DefGlProc(PFNGLENABLEVERTEXATTRIBARRAYPROC, glEnableVertexAttribArray);
  DefGlProc(PFNGLDISABLEVERTEXATTRIBARRAYPROC, glDisableVertexAttribArray);
  DefGlProc(PFNGLUNIFORM3FVPROC, glUniform3fv);
  DefGlProc(PFNGLUNIFORM2FVPROC, glUniform2fv);
  DefGlProc(PFNGLUNIFORMMATRIX4FVPROC, glUniformMatrix4fv);
  DefGlProc(PFNGLUNIFORM1IPROC, glUniform1i);
  DefGlProc(PFNGLUNIFORM1FPROC, glUniform1f);
  DefGlProc(PFNGLUNIFORM1UIPROC, glUniform1ui);

  /*
   * 3.0
   */
  DefGlProc(PFNGLGENFRAMEBUFFERSPROC, glGenFramebuffers);
  DefGlProc(PFNGLBINDFRAMEBUFFERPROC, glBindFramebuffer);
  DefGlProc(PFNGLFRAMEBUFFERTEXTURE2DPROC, glFramebufferTexture2D);
  DefGlProc(PFNGLCHECKFRAMEBUFFERSTATUSPROC, glCheckFramebufferStatus);
  DefGlProc(PFNGLGENVERTEXARRAYSPROC, glGenVertexArrays);
  DefGlProc(PFNGLBINDVERTEXARRAYPROC, glBindVertexArray);


  /*
   * 3.2
   */
  // My laptop is running 3.1ES, but this worked with GLEW, so presumably
  // it can be loaded somehow..  Maybe the _EXT or _ARB one?
  // DefGlProc(PFNGLFRAMEBUFFERTEXTUREPROC, glFramebufferTexture);

  /*
   * 4.3
   */
  DefGlProc(PFNGLXCREATECONTEXTATTRIBSARBPROC, glXCreateContextAttribsARB);

#endif


#endif

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);

  AssertNoGlErrors;

  // Platform specific (wgl / glX)
  s32 VsyncFrames = 1;
#if BONSAI_LINUX
  // TODO(Jesse): Not getting vsync on my arch laptop...
  PFNSWAPINTERVALPROC glSwapInterval = (PFNSWAPINTERVALPROC)bonsaiGlGetProcAddress("glXSwapIntervalEXT");
  if ( glSwapInterval )
  {
    glSwapInterval(Os->Display, Os->Window, VsyncFrames);
  }
  else
  {
    Info("No Vsync");
  }
#elif BONSAI_WIN32
  PFNSWAPINTERVALPROC glSwapInterval = (PFNSWAPINTERVALPROC)bonsaiGlGetProcAddress("wglSwapIntervalEXT");
  Assert( glSwapInterval );
  glSwapInterval(VsyncFrames);
#else
#error "Unknown Platform"
#endif

  return;
}

function b32
CheckShadingLanguageVersion()
{
  char *OpenGlVersion = (char*)glGetString ( GL_VERSION );
  r32 ShadingLanguageVersion = (r32)atof((char*)glGetString ( GL_SHADING_LANGUAGE_VERSION ));

  Info("OpenGl Verison : %s", OpenGlVersion );
  Info("Shading Language Verison : %f", ShadingLanguageVersion );

  r32 RequiredShadingLanguageVersion = 3.3f;
  if (ShadingLanguageVersion < RequiredShadingLanguageVersion)
  {
    Error("Unsupported Version of GLSL :: Got %f, Needed: %f", ShadingLanguageVersion, RequiredShadingLanguageVersion);
  }

  b32 Result = (ShadingLanguageVersion > RequiredShadingLanguageVersion);
  return Result;
}

