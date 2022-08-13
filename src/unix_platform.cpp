#include <bonsai_net/network.h>


void *
OpenLibrary(const char *filename)
{
  void* Result = dlopen(filename, RTLD_NOW);

  if (!Result)
  {
    char *error = dlerror();
    Error("Loading library: %s", error);
    Assert(False);
  }
  else
  {
    Info("Library (%s) loaded!", filename);
  }

  return Result;
}

void
CloseLibrary(shared_lib Lib)
{
  s32 E = dlclose(Lib);
  if (E != 0)
  {
    Error("Closing Shared Library");
  }

  return;
}

void
HandleGlDebugMessage(GLenum Source, GLenum Type, GLuint Id, GLenum Severity,
                     GLsizei MessageLength, const GLchar* Message, const void* UserData)
{
  if (Severity != GL_DEBUG_SEVERITY_NOTIFICATION)
  {

    DebugLine("%s", Message);
    RuntimeBreak();
    const char* MessageTypeName = 0;
    switch(Type) {
      case(GL_DEBUG_TYPE_ERROR):
      {
        MessageTypeName = "ERROR";
      } break;
      case(GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR):
      {
        MessageTypeName = "DEPRECATED_BEHAVIOR";
      } break;
      case(GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR):
      {
        MessageTypeName = "UNDEFINED_BEHAVIOR";
      } break;
      case(GL_DEBUG_TYPE_PORTABILITY):
      {
        MessageTypeName = "PORTABILITY";
      } break;
      case(GL_DEBUG_TYPE_PERFORMANCE):
      {
        MessageTypeName = "PERFORMANCE";
      } break;
      case(GL_DEBUG_TYPE_OTHER):
      {
        MessageTypeName = "OTHER";
      } break;
      InvalidDefaultCase;
    }

    OpenGlDebugMessage("Source %u, Type: %s, Id %u - %.*s", Source, MessageTypeName, Id, MessageLength, Message);
    if (UserData)
    {
      OpenGlDebugMessage("User Data At %p", UserData);
    }
  }

  return;
}

#if PLATFORM_LIBRARY_AND_WINDOW_IMPLEMENTATIONS
// TODO(Jesse id: 267): Unnecessary .. I just added these as a hack get parsing to work
typedef Colormap x_colormap;
typedef XSetWindowAttributes x_set_window_attribs;

b32
OpenAndInitializeWindow(os *Os, platform *Plat)
{
  GLint GlAttribs[] = {
    GLX_RGBA,
    GLX_DEPTH_SIZE, 24,
    GLX_DOUBLEBUFFER,
    None };

  Os->Display = XOpenDisplay(0);
  if (!Os->Display) { Error("Cannot connect to X Server"); return False; }

  window RootWindow = DefaultRootWindow(Os->Display);
  if (!RootWindow) { Error("Unable to get RootWindow"); return False; }

  s32 Screen = DefaultScreen(Os->Display);

  XVisualInfo *VisualInfo = glXChooseVisual(Os->Display, Screen, GlAttribs);
  if (!VisualInfo) { Error("Unable to get Visual Info"); return False; }

  Assert(VisualInfo->screen == Screen);

  x_colormap ColorInfo = XCreateColormap(Os->Display, RootWindow, VisualInfo->visual, AllocNone);

  Plat->WindowWidth = SCR_WIDTH;
  Plat->WindowHeight = SCR_HEIGHT;

  Assert(Plat->WindowWidth && Plat->WindowHeight);

  x_set_window_attribs WindowAttribs;
  WindowAttribs.colormap = ColorInfo;
  WindowAttribs.event_mask = WindowEventMasks;

  window xWindow = XCreateWindow( Os->Display, RootWindow,
                                  0, 0,
                                  (u32)Plat->WindowWidth, (u32)Plat->WindowHeight,
                                  0, VisualInfo->depth, InputOutput, VisualInfo->visual,
                                  CWColormap | CWEventMask, &WindowAttribs);

  if (!xWindow) { Error("Unable to Create Window"); return False; }

  XMapWindow(Os->Display, xWindow);
  XStoreName(Os->Display, xWindow, "Bonsai");

  const s32 FramebufferAttributes[] = {None};

  s32 ValidConfigCount = 0;
  GLXFBConfig* ValidConfigs =
    glXChooseFBConfig( Os->Display,  VisualInfo->screen, FramebufferAttributes, &ValidConfigCount);

  Assert(ValidConfigCount);
  GLXFBConfig FramebufferConfig = ValidConfigs[0];

  const s32 OpenGlContextAttribs[] = {
    GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
    GLX_CONTEXT_MINOR_VERSION_ARB, 0,
    GLX_CONTEXT_FLAGS_ARB, GLX_CONTEXT_DEBUG_BIT_ARB,
    GLX_CONTEXT_PROFILE_MASK_ARB, GLX_CONTEXT_CORE_PROFILE_BIT_ARB,
    None
  };

  GLXContext ShareContext = 0;
  PFNGLXCREATECONTEXTATTRIBSARBPROC glXCreateContextAttribsARB =
    (PFNGLXCREATECONTEXTATTRIBSARBPROC)PlatformGetGlFunction("glXCreateContextAttribsARB");
  Assert(glXCreateContextAttribsARB);

  Os->GlContext = glXCreateContextAttribsARB(Os->Display, FramebufferConfig, ShareContext, GL_TRUE, OpenGlContextAttribs);
  if (!Os->GlContext) { Error("Unable to Create GLXContext"); return False; }

  Assert(Os->Display != None && Os->GlContext);

  glXMakeCurrent(Os->Display, xWindow, Os->GlContext);

  Os->Window = xWindow;
  return True;
}

inline void*
GetProcFromLib(shared_lib Lib, const char *Name)
{
  void* Result = dlsym(Lib, Name);
  return Result;
}

inline void
Terminate(os *Os)
{
  XDestroyWindow(Os->Display, Os->Window);
  XCloseDisplay(Os->Display);

  SuspendWorkerThreads();

  return;
}

// TODO(Jesse id: 268): Unnecessary .. I just added these as a hack get parsing to work
typedef XEvent x_event;

b32
ProcessOsMessages(os *Os, platform *Plat)
{
  TIMED_FUNCTION();

  x_event Event;
  b32 EventFound = (b32)XCheckWindowEvent( Os->Display, Os->Window, WindowEventMasks, &Event);

  if (EventFound)
  {

    switch (Event.type)
    {

      case ConfigureNotify:
      {
        Plat->WindowWidth = Event.xconfigure.width;
        Plat->WindowHeight = Event.xconfigure.height;
      } break;

      case MotionNotify:
      {
        Plat->MouseP.x = Event.xmotion.x;
        Plat->MouseP.y = Event.xmotion.y;
      } break;

      case ButtonPress:
      {
        if (Event.xbutton.button == Button1)
        {
          Plat->Input.LMB.Clicked = True;
          Plat->Input.LMB.Pressed = True;
        }
        if (Event.xbutton.button == Button2)
        {
          Plat->Input.MMB.Clicked = True;
          Plat->Input.MMB.Pressed = True;
        }
        if (Event.xbutton.button == Button3)
        {
          Plat->Input.RMB.Clicked = True;
          Plat->Input.RMB.Pressed = True;
        }
      } break;

      case ButtonRelease:
      {
        if (Event.xbutton.button == Button1)
        {
          Plat->Input.LMB.Pressed = False;
        }
        if (Event.xbutton.button == Button2)
        {
          Plat->Input.MMB.Pressed = False;
        }
        if (Event.xbutton.button == Button3)
        {
          Plat->Input.RMB.Pressed = False;
        }
      } break;

      case KeyRelease:
      {
        u64 KeySym = XLookupKeysym(&Event.xkey, 0);
        switch (KeySym)
        {

          BindKeyupToInput(XK_w, W);
          BindKeyupToInput(XK_s, S);
          BindKeyupToInput(XK_a, A);
          BindKeyupToInput(XK_d, D);
          BindKeyupToInput(XK_q, Q);
          BindKeyupToInput(XK_e, E);

          BindKeyupToInput(XK_F1, F1);
          BindKeyupToInput(XK_F2, F2);
          BindKeyupToInput(XK_F3, F3);
          BindKeyupToInput(XK_F4, F4);
          BindKeyupToInput(XK_F5, F5);
          BindKeyupToInput(XK_F6, F6);
          BindKeyupToInput(XK_F7, F7);
          BindKeyupToInput(XK_F8, F8);
          BindKeyupToInput(XK_F9, F9);
          BindKeyupToInput(XK_F10, F10);
          BindKeyupToInput(XK_F11, F11);
          BindKeyupToInput(XK_F12, F12);

          BindKeyupToInput(XK_space, Space);

          default:
          {
          } break;
        }
      } break;

      case KeyPress:
      {
        u64 KeySym = XLookupKeysym(&Event.xkey, 0);
        switch (KeySym)
        {

          BindKeydownToInput(XK_w, W);
          BindKeydownToInput(XK_s, S);
          BindKeydownToInput(XK_a, A);
          BindKeydownToInput(XK_d, D);
          BindKeydownToInput(XK_q, Q);
          BindKeydownToInput(XK_e, E);

          BindKeydownToInput(XK_F1, F1);
          BindKeydownToInput(XK_F2, F2);
          BindKeydownToInput(XK_F3, F3);
          BindKeydownToInput(XK_F4, F4);
          BindKeydownToInput(XK_F5, F5);
          BindKeydownToInput(XK_F6, F6);
          BindKeydownToInput(XK_F7, F7);
          BindKeydownToInput(XK_F8, F8);
          BindKeydownToInput(XK_F9, F9);
          BindKeydownToInput(XK_F10, F10);
          BindKeydownToInput(XK_F11, F11);
          BindKeydownToInput(XK_F12, F12);

          BindKeydownToInput(XK_space, Space);

          case XK_Escape:
          {
            Os->ContinueRunning = False;
          } break;

          default:
          {
          } break;
        }

      } break;
    }

  }

  return EventFound;
}
#endif // PLATFORM_LIBRARY_AND_WINDOW_IMPLEMENTATIONS

#if PLATFORM_GL_IMPLEMENTATIONS

inline void
BonsaiSwapBuffers(os *Os)
{
  TIMED_FUNCTION();
  glXSwapBuffers(Os->Display, Os->Window);
}

#endif // PLATFORM_GL_IMPLEMENTATIONS

inline void
ConnectToServer(network_connection *Connection)
{
  if (!Connection->Socket.Id)
  {
    Connection->Socket = CreateSocket(Socket_NonBlocking);
  }

  errno = 0;
  s32 ConnectStatus = connect(Connection->Socket.Id,
                              (sockaddr *)&Connection->Address,
                              sizeof(sockaddr_in));

  if (ConnectStatus == 0)
  {
      DebugLine("Connected");
      Connection->State = ConnectionState_AwaitingHandshake;
  }
  else if (ConnectStatus == -1)
  {
    switch (errno)
    {
      case 0:
      {
      } break;

      case EINPROGRESS:
      case EALREADY:
      {
        // Connection in progress
      } break;

      case ECONNREFUSED:
      {
        // Host is down
      } break;

      case EISCONN:
      {
        // Not sure if we should ever call connect on an already-connected connection
        Assert(False);
      } break;

      default :
      {
        Error("Connecting to remote host encountered an unexpected error : %d", errno);
        Assert(False);
      } break;

    }
  }
  else
  {
    InvalidCodePath();
  }

  return;
}
