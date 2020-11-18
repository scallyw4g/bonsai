bonsai_function void
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
  s32 VsyncFrames = 0;
  // TODO(Jesse, id: 150, tags: platform, opengl): Should this be done elsewhere?
#if BONSAI_LINUX
  // TODO(Jesse, id: 151, tags: open_question, platform_linux): Not getting vsync on my arch laptop.

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
#elif EMCC

  NotImplemented;

#else
#error "Unknown Platform"
#endif

  return;
}
