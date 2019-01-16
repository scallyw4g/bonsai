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

enum work_queue_entry_type
{
  WorkEntryType_None            =      0,
  WorkEntryType_InitWorldChunk  = 1 << 1,
  WorkEntryType_CopyBuffer      = 1 << 2,
};
struct gpu_buffer_copy_params
{
  untextured_3d_geometry_buffer* Src;
  untextured_3d_geometry_buffer Dest;
  v3 Basis;
};
struct work_queue_entry
{
  work_queue_entry_type Type;
  game_state *GameState;

  union {
    void *Input;
    gpu_buffer_copy_params GpuCopyParams;
  };
};

struct thread
{
  int ThreadIndex;
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

struct gl_extensions
{
  PFNGLCREATESHADERPROC glCreateShader;
  PFNGLSHADERSOURCEPROC glShaderSource;
  PFNGLCOMPILESHADERPROC glCompileShader;
  PFNGLGETSHADERIVPROC glGetShaderiv;
  PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
  PFNGLATTACHSHADERPROC glAttachShader;
  PFNGLDETACHSHADERPROC glDetachShader;
  PFNGLDELETESHADERPROC glDeleteShader;
  PFNGLCREATEPROGRAMPROC glCreateProgram;
  PFNGLLINKPROGRAMPROC glLinkProgram;
  PFNGLGETPROGRAMIVPROC glGetProgramiv;
  PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;
  PFNGLUSEPROGRAMPROC glUseProgram;
  PFNGLDELETEPROGRAMPROC glDeleteProgram;
  PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
  PFNGLGENFRAMEBUFFERSPROC glGenFramebuffers;
  PFNGLBINDFRAMEBUFFERPROC glBindFramebuffer;
  /* PFNGLFRAMEBUFFERTEXTUREPROC glFramebufferTexture; */
  PFNGLFRAMEBUFFERTEXTURE2DPROC glFramebufferTexture2D;
  PFNGLCHECKFRAMEBUFFERSTATUSPROC glCheckFramebufferStatus;
  PFNGLCOMPRESSEDTEXIMAGE2DPROC glCompressedTexImage2D;
  PFNGLGENBUFFERSPROC glGenBuffers;
  PFNGLBINDBUFFERPROC glBindBuffer;
  PFNGLBUFFERDATAPROC glBufferData;
  PFNGLDRAWBUFFERSPROC glDrawBuffers;
  PFNGLDELETEBUFFERSPROC glDeleteBuffers;
  PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
  PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
  PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray;
  PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
  PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
  PFNGLUNIFORM3FVPROC glUniform3fv;
  PFNGLUNIFORM2FVPROC glUniform2fv;
  PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;
  PFNGLUNIFORM1IPROC glUniform1i;
  PFNGLACTIVETEXTUREPROC glActiveTexture;
  PFNGLUNIFORM1FPROC glUniform1f;
  PFNGLUNIFORM1UIPROC glUniform1ui;

  // Platform specific (wgl / glX)
  PFNSWAPINTERVALPROC glSwapInterval;
};

// XXX Make sure this ONLY contains input_event structs - they are cleared in a
// loop and the struct size is used to determine the ending index
struct input
{
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

  gl_extensions GL;

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

