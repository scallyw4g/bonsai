#if PLATFORM_GL_IMPLEMENTATIONS

inline void
BonsaiSwapBuffers(os *Os)
{
  NotImplemented;
}

#endif // PLATFORM_GL_IMPLEMENTATIONS

void
PlatformDebugStacktrace()
{
  Warn("Couldn't get a stacktrace on this platform :((((");
  return;
}

b32
OpenAndInitializeWindow( os *Os, platform *Plat, s32 Ignored)
{
  Info("Creating Context");

  // Context configurations
  EmscriptenWebGLContextAttributes attr;
  emscripten_webgl_init_context_attributes(&attr);

  attr.depth = 1;
  attr.alpha = 0;
  attr.stencil = 0;
  attr.antialias = 0;
  attr.premultipliedAlpha = 0;
  attr.preserveDrawingBuffer = 0;
  attr.powerPreference = EM_WEBGL_POWER_PREFERENCE_HIGH_PERFORMANCE;
  attr.failIfMajorPerformanceCaveat = 1;

  attr.majorVersion = 2;
  attr.minorVersion = 0;

  // TODO(Jesse id: 367): How do we turn this on?  Currently on my machine it causes
  // context creation to fail.
  // @emcc_vsync
  attr.explicitSwapControl = 0;

  attr.enableExtensionsByDefault = 1;
  attr.proxyContextToMainThread = EMSCRIPTEN_WEBGL_CONTEXT_PROXY_DISALLOW;
  attr.renderViaOffscreenBackBuffer = 1;

  EMSCRIPTEN_WEBGL_CONTEXT_HANDLE ctx = emscripten_webgl_create_context("#canvas", &attr);
  emscripten_webgl_make_context_current(ctx);

  Os->GlContext = ctx;

  b32 Result = ctx ? True : False;
  return Result;
}
