link_internal b32
InitializeOpengl(os* Os)
{
  Info("Initializing OpenGL Extensions");

#if 0
  const char* glxExtensionString = glXQueryExtensionsString(Os->Display, DefaultScreen(Os->Display));
  const char* glExtensionString = (const char*)glGetString(GL_EXTENSIONS);
  Debug(glExtensionString);
  Debug(glxExtensionString);
#endif

  GL.Initialized = True;

  GL.GetString                = (OpenglGetString)PlatformGetGlFunction("glGetString");
  GL.Initialized              &= GL.GetString != 0;

  if (GL.GetString)
  {
    Info(CS(GL.GetString(GL_VENDOR)));
    Info(CS(GL.GetString(GL_RENDERER)));
    Info(CS(GL.GetString(GL_VERSION)));
  }

  GL.GetError                 = (OpenglGetError)PlatformGetGlFunction("glGetError");
  GL.Initialized              &= GL.GetError != 0;

  GL.Enable                   = (OpenglEnable)PlatformGetGlFunction("glEnable");
  GL.Initialized              &= GL.Enable != 0;

  GL.Disable                  = (OpenglDisable)PlatformGetGlFunction("glDisable");
  GL.Initialized              &= GL.Disable != 0;

  GL.CullFace                 = (OpenglCullFace)PlatformGetGlFunction("glCullFace");
  GL.Initialized              &= GL.CullFace != 0;

  GL.Viewport                 = (OpenglViewport)PlatformGetGlFunction("glViewport");
  GL.Initialized              &= GL.Viewport != 0;

  GL.DepthFunc                = (OpenglDepthFunc)PlatformGetGlFunction("glDepthFunc");
  GL.Initialized              &= GL.DepthFunc != 0;

  GL.BlendFunc                = (OpenglBlendFunc)PlatformGetGlFunction("glBlendFunc");
  GL.Initialized              &= GL.BlendFunc != 0;

  GL.DrawArrays               = (OpenglDrawArrays)PlatformGetGlFunction("glDrawArrays");
  GL.Initialized              &= GL.DrawArrays != 0;

  GL.Clear                    = (OpenglClear)PlatformGetGlFunction("glClear");
  GL.Initialized              &= GL.Clear != 0;

  GL.ClearColor               = (OpenglClearColor)PlatformGetGlFunction("glClearColor");
  GL.Initialized              &= GL.ClearColor != 0;

  GL.ClearDepth               = (OpenglClearDepth)PlatformGetGlFunction("glClearDepth");
  GL.Initialized              &= GL.ClearDepth != 0;

  GL.GenTextures              = (OpenglGenTextures)PlatformGetGlFunction("glGenTextures");
  GL.Initialized              &= GL.GenTextures != 0;

  GL.BindTextures             = (OpenglBindTextures)PlatformGetGlFunction("glBindTextures");
  GL.Initialized              &= GL.BindTextures != 0;

  GL.BindTexture              = (OpenglBindTexture)PlatformGetGlFunction("glBindTexture");
  GL.Initialized              &= GL.BindTexture != 0;

  GL.DeleteTextures           = (OpenglDeleteTextures)PlatformGetGlFunction("glDeleteTextures");
  GL.Initialized              &= GL.DeleteTextures != 0;

  GL.ActiveTexture            = (OpenglActiveTexture)PlatformGetGlFunction("glActiveTexture");
  GL.Initialized              &= GL.ActiveTexture != 0;

  GL.TexStorage1D             = (OpenglTexStorage1D)PlatformGetGlFunction("glTexStorage1D");
  GL.Initialized              &= GL.TexStorage1D != 0;

  GL.TexStorage2D             = (OpenglTexStorage2D)PlatformGetGlFunction("glTexStorage2D");
  GL.Initialized              &= GL.TexStorage2D != 0;

  GL.TexStorage3D             = (OpenglTexStorage3D)PlatformGetGlFunction("glTexStorage3D");
  GL.Initialized              &= GL.TexStorage3D != 0;

  GL.TexImage1D               = (OpenglTexImage1D)PlatformGetGlFunction("glTexImage1D");
  GL.Initialized              &= GL.TexImage1D != 0;

  GL.TexImage2D               = (OpenglTexImage2D)PlatformGetGlFunction("glTexImage2D");
  GL.Initialized              &= GL.TexImage2D != 0;

  GL.TexImage3D               = (OpenglTexImage3D)PlatformGetGlFunction("glTexImage3D");
  GL.Initialized              &= GL.TexImage3D != 0;

  GL.TexSubImage1D            = (OpenglTexSubImage1D)PlatformGetGlFunction("glTexSubImage1D");
  GL.Initialized              &= GL.TexSubImage1D != 0;

  GL.TexSubImage2D            = (OpenglTexSubImage2D)PlatformGetGlFunction("glTexSubImage2D");
  GL.Initialized              &= GL.TexSubImage2D != 0;

  GL.TexSubImage3D            = (OpenglTexSubImage3D)PlatformGetGlFunction("glTexSubImage3D");
  GL.Initialized              &= GL.TexSubImage3D != 0;

  GL.TexParameterf            = (OpenglTexParameterf)PlatformGetGlFunction("glTexParameterf");
  GL.Initialized              &= GL.TexParameterf != 0;

  GL.TexParameterfv           = (OpenglTexParameterfv)PlatformGetGlFunction("glTexParameterfv");
  GL.Initialized              &= GL.TexParameterfv != 0;

  GL.TexParameteri            = (OpenglTexParameteri)PlatformGetGlFunction("glTexParameteri");
  GL.Initialized              &= GL.TexParameteri != 0;

  GL.TexParameteriv           = (OpenglTexParameteriv)PlatformGetGlFunction("glTexParameteriv");
  GL.Initialized              &= GL.TexParameteriv != 0;

  GL.CompressedTexImage3D     = (OpenglCompressedTexImage3D)PlatformGetGlFunction("glCompressedTexImage3D");
  GL.Initialized              &= GL.CompressedTexImage3D != 0;

  GL.CompressedTexImage2D     = (OpenglCompressedTexImage2D)PlatformGetGlFunction("glCompressedTexImage2D");
  GL.Initialized              &= GL.CompressedTexImage2D != 0;

  GL.CompressedTexImage1D     = (OpenglCompressedTexImage1D)PlatformGetGlFunction("glCompressedTexImage1D");
  GL.Initialized              &= GL.CompressedTexImage1D != 0;

  GL.PixelStoref              = (OpenglPixelStoref)PlatformGetGlFunction("glPixelStoref");
  GL.Initialized              &= GL.PixelStoref != 0;

  GL.PixelStorei              = (OpenglPixelStorei)PlatformGetGlFunction("glPixelStorei");
  GL.Initialized              &= GL.PixelStorei != 0;

  GL.EnableVertexAttribArray  = (OpenglEnableVertexAttribArray)PlatformGetGlFunction("glEnableVertexAttribArray");
  GL.Initialized              &= GL.EnableVertexAttribArray != 0;

  GL.DisableVertexAttribArray = (OpenglDisableVertexAttribArray)PlatformGetGlFunction("glDisableVertexAttribArray");
  GL.Initialized              &= GL.DisableVertexAttribArray != 0;

  GL.VertexAttribPointer      = (OpenglVertexAttribPointer)PlatformGetGlFunction("glVertexAttribPointer");
  GL.Initialized              &= GL.VertexAttribPointer != 0;

  GL.BindFramebuffer          = (OpenglBindFramebuffer)PlatformGetGlFunction("glBindFramebuffer");
  GL.Initialized              &= GL.BindFramebuffer != 0;

  GL.DeleteFramebuffers       = (OpenglDeleteFramebuffers)PlatformGetGlFunction("glDeleteFramebuffers");
  GL.Initialized              &= GL.DeleteFramebuffers != 0;

  GL.GenFramebuffers          = (OpenglGenFramebuffers)PlatformGetGlFunction("glGenFramebuffers");
  GL.Initialized              &= GL.GenFramebuffers != 0;

  GL.CheckFramebufferStatus   = (OpenglCheckFramebufferStatus)PlatformGetGlFunction("glCheckFramebufferStatus");
  GL.Initialized              &= GL.CheckFramebufferStatus != 0;

  GL.FramebufferTexture1D     = (OpenglFramebufferTexture1D)PlatformGetGlFunction("glFramebufferTexture1D");
  GL.Initialized              &= GL.FramebufferTexture1D != 0;

  GL.FramebufferTexture2D     = (OpenglFramebufferTexture2D)PlatformGetGlFunction("glFramebufferTexture2D");
  GL.Initialized              &= GL.FramebufferTexture2D != 0;

  GL.FramebufferTexture3D     = (OpenglFramebufferTexture3D)PlatformGetGlFunction("glFramebufferTexture3D");
  GL.Initialized              &= GL.FramebufferTexture3D != 0;

  GL.FramebufferTextureLayer  = (OpenglFramebufferTextureLayer)PlatformGetGlFunction("glFramebufferTextureLayer");
  GL.Initialized              &= GL.FramebufferTextureLayer != 0;

  GL.FramebufferRenderbuffer  = (OpenglFramebufferRenderbuffer)PlatformGetGlFunction("glFramebufferRenderbuffer");
  GL.Initialized              &= GL.FramebufferRenderbuffer != 0;

  GL.CompileShader            = (OpenglCompileShader)PlatformGetGlFunction("glCompileShader");
  GL.Initialized              &= GL.CompileShader != 0;

  GL.CreateProgram            = (OpenglCreateProgram)PlatformGetGlFunction("glCreateProgram");
  GL.Initialized              &= GL.CreateProgram != 0;

  GL.CreateShader             = (OpenglCreateShader)PlatformGetGlFunction("glCreateShader");
  GL.Initialized              &= GL.CreateShader != 0;

  GL.LinkProgram              = (OpenglLinkProgram)PlatformGetGlFunction("glLinkProgram");
  GL.Initialized              &= GL.LinkProgram != 0;

  GL.ShaderSource             = (OpenglShaderSource)PlatformGetGlFunction("glShaderSource");
  GL.Initialized              &= GL.ShaderSource != 0;

  GL.UseProgram               = (OpenglUseProgram)PlatformGetGlFunction("glUseProgram");
  GL.Initialized              &= GL.UseProgram != 0;

  GL.GetProgramInfoLog        = (OpenglGetProgramInfoLog)PlatformGetGlFunction("glGetProgramInfoLog");
  GL.Initialized              &= GL.GetProgramInfoLog != 0;

  GL.GetShaderInfoLog         = (OpenglGetShaderInfoLog)PlatformGetGlFunction("glGetShaderInfoLog");
  GL.Initialized              &= GL.GetShaderInfoLog != 0;

  GL.DeleteProgram            = (OpenglDeleteProgram)PlatformGetGlFunction("glDeleteProgram");
  GL.Initialized              &= GL.DeleteProgram != 0;

  GL.DeleteShader             = (OpenglDeleteShader)PlatformGetGlFunction("glDeleteShader");
  GL.Initialized              &= GL.DeleteShader != 0;

  GL.DetachShader             = (OpenglDetachShader)PlatformGetGlFunction("glDetachShader");
  GL.Initialized              &= GL.DetachShader != 0;

  GL.Uniform1f                = (OpenglUniform1f)PlatformGetGlFunction("glUniform1f");
  GL.Initialized              &= GL.Uniform1f != 0;

  GL.Uniform2f                = (OpenglUniform2f)PlatformGetGlFunction("glUniform2f");
  GL.Initialized              &= GL.Uniform2f != 0;

  GL.Uniform3f                = (OpenglUniform3f)PlatformGetGlFunction("glUniform3f");
  GL.Initialized              &= GL.Uniform3f != 0;

  GL.Uniform4f                = (OpenglUniform4f)PlatformGetGlFunction("glUniform4f");
  GL.Initialized              &= GL.Uniform4f != 0;

  GL.Uniform1i                = (OpenglUniform1i)PlatformGetGlFunction("glUniform1i");
  GL.Initialized              &= GL.Uniform1i != 0;

  GL.Uniform2i                = (OpenglUniform2i)PlatformGetGlFunction("glUniform2i");
  GL.Initialized              &= GL.Uniform2i != 0;

  GL.Uniform3i                = (OpenglUniform3i)PlatformGetGlFunction("glUniform3i");
  GL.Initialized              &= GL.Uniform3i != 0;

  GL.Uniform4i                = (OpenglUniform4i)PlatformGetGlFunction("glUniform4i");
  GL.Initialized              &= GL.Uniform4i != 0;

  GL.Uniform1fv               = (OpenglUniform1fv)PlatformGetGlFunction("glUniform1fv");
  GL.Initialized              &= GL.Uniform1fv != 0;

  GL.Uniform2fv               = (OpenglUniform2fv)PlatformGetGlFunction("glUniform2fv");
  GL.Initialized              &= GL.Uniform2fv != 0;

  GL.Uniform3fv               = (OpenglUniform3fv)PlatformGetGlFunction("glUniform3fv");
  GL.Initialized              &= GL.Uniform3fv != 0;

  GL.Uniform4fv               = (OpenglUniform4fv)PlatformGetGlFunction("glUniform4fv");
  GL.Initialized              &= GL.Uniform4fv != 0;

  GL.Uniform1iv               = (OpenglUniform1iv)PlatformGetGlFunction("glUniform1iv");
  GL.Initialized              &= GL.Uniform1iv != 0;

  GL.Uniform2iv               = (OpenglUniform2iv)PlatformGetGlFunction("glUniform2iv");
  GL.Initialized              &= GL.Uniform2iv != 0;

  GL.Uniform3iv               = (OpenglUniform3iv)PlatformGetGlFunction("glUniform3iv");
  GL.Initialized              &= GL.Uniform3iv != 0;

  GL.Uniform4iv               = (OpenglUniform4iv)PlatformGetGlFunction("glUniform4iv");
  GL.Initialized              &= GL.Uniform4iv != 0;

  GL.UniformMatrix2fv         = (OpenglUniformMatrix2fv)PlatformGetGlFunction("glUniformMatrix2fv");
  GL.Initialized              &= GL.UniformMatrix2fv != 0;

  GL.UniformMatrix3fv         = (OpenglUniformMatrix3fv)PlatformGetGlFunction("glUniformMatrix3fv");
  GL.Initialized              &= GL.UniformMatrix3fv != 0;

  GL.UniformMatrix4fv         = (OpenglUniformMatrix4fv)PlatformGetGlFunction("glUniformMatrix4fv");
  GL.Initialized              &= GL.UniformMatrix4fv != 0;

  GL.Uniform1ui               = (OpenglUniform1ui)PlatformGetGlFunction("glUniform1ui");
  GL.Initialized              &= GL.Uniform1ui != 0;

  GL.Uniform2ui               = (OpenglUniform2ui)PlatformGetGlFunction("glUniform2ui");
  GL.Initialized              &= GL.Uniform2ui != 0;

  GL.Uniform3ui               = (OpenglUniform3ui)PlatformGetGlFunction("glUniform3ui");
  GL.Initialized              &= GL.Uniform3ui != 0;

  GL.Uniform4ui               = (OpenglUniform4ui)PlatformGetGlFunction("glUniform4ui");
  GL.Initialized              &= GL.Uniform4ui != 0;

  GL.Uniform1uiv              = (OpenglUniform1uiv)PlatformGetGlFunction("glUniform1uiv");
  GL.Initialized              &= GL.Uniform1uiv != 0;

  GL.Uniform2uiv              = (OpenglUniform2uiv)PlatformGetGlFunction("glUniform2uiv");
  GL.Initialized              &= GL.Uniform2uiv != 0;

  GL.Uniform3uiv              = (OpenglUniform3uiv)PlatformGetGlFunction("glUniform3uiv");
  GL.Initialized              &= GL.Uniform3uiv != 0;

  GL.Uniform4uiv              = (OpenglUniform4uiv)PlatformGetGlFunction("glUniform4uiv");
  GL.Initialized              &= GL.Uniform4uiv != 0;

  GL.GetUniformLocation       = (OpenglGetUniformLocation)PlatformGetGlFunction("glGetUniformLocation");
  GL.Initialized              &= GL.GetUniformLocation != 0;

  GL.GetShaderiv              = (OpenglGetShaderiv)PlatformGetGlFunction("glGetShaderiv");
  GL.Initialized              &= GL.GetShaderiv != 0;

  GL.GetProgramiv             = (OpenglGetProgramiv)PlatformGetGlFunction("glGetProgramiv");
  GL.Initialized              &= GL.GetProgramiv != 0;

  GL.AttachShader             = (OpenglAttachShader)PlatformGetGlFunction("glAttachShader");
  GL.Initialized              &= GL.AttachShader != 0;

  GL.BindBuffer               = (OpenglBindBuffer)PlatformGetGlFunction("glBindBuffer");
  GL.Initialized              &= GL.BindBuffer != 0;

  GL.DeleteBuffers            = (OpenglDeleteBuffers)PlatformGetGlFunction("glDeleteBuffers");
  GL.Initialized              &= GL.DeleteBuffers != 0;

  GL.GenBuffers               = (OpenglGenBuffers)PlatformGetGlFunction("glGenBuffers");
  GL.Initialized              &= GL.GenBuffers != 0;

  GL.BufferData               = (OpenglBufferData)PlatformGetGlFunction("glBufferData");
  GL.Initialized              &= GL.BufferData != 0;

  GL.MapBuffer                = (OpenglMapBuffer)PlatformGetGlFunction("glMapBuffer");
  GL.Initialized              &= GL.MapBuffer != 0;

  GL.MapBufferRange           = (OpenglMapBufferRange)PlatformGetGlFunction("glMapBufferRange");
  GL.Initialized              &= GL.MapBufferRange != 0;

  GL.UnmapBuffer              = (OpenglUnmapBuffer)PlatformGetGlFunction("glUnmapBuffer");
  GL.Initialized              &= GL.UnmapBuffer != 0;

  GL.DrawBuffers              = (OpenglDrawBuffers)PlatformGetGlFunction("glDrawBuffers");
  GL.Initialized              &= GL.DrawBuffers != 0;

  GL.GetIntegerv              = (OpenglGetIntegerv)PlatformGetGlFunction("glGetIntegerv");
  GL.Initialized              &= GL.GetIntegerv != 0;

  GL.DebugMessageCallback     = (OpenglDebugMessageCallback)PlatformGetGlFunction("glDebugMessageCallback");
  GL.Initialized              &= GL.DebugMessageCallback != 0;


  if (GL.Initialized)
  {
    GL.DebugMessageCallback(HandleGlDebugMessage, 0);

    GL.Enable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    GL.Enable(GL_DEPTH_TEST);

    GL.DepthFunc(GL_LEQUAL);

    AssertNoGlErrors;

    s32 VsyncFrames = 0;
#if BONSAI_LINUX
    // TODO(Jesse, id: 151, tags: open_question, platform_linux): Not getting vsync on my arch laptop.
    PFNSWAPINTERVALPROC glSwapInterval = (PFNSWAPINTERVALPROC)PlatformGetGlFunction("glXSwapIntervalEXT");
    if ( glSwapInterval )
    { glSwapInterval(Os->Display, Os->Window, VsyncFrames); }
    else
    { Info("No Vsync"); }
#elif BONSAI_WIN32
    PFNSWAPINTERVALPROC glSwapInterval = (PFNSWAPINTERVALPROC)PlatformGetGlFunction("wglSwapIntervalEXT");
    if ( glSwapInterval )
    { glSwapInterval(VsyncFrames); }
    else
    { Info("No Vsync"); }
#elif EMCC
    // TODO(Jesse id: 368): How do we get vsync here?
    // @emcc_vsync
#endif

    AssertNoGlErrors;
  }

  b32 Result = GL.Initialized && CheckOpenglVersion();
  return Result;
}
