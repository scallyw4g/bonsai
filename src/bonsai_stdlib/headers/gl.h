
// Wrapper so assertions give us file/line numbers
#define AssertNoGlErrors do {            \
  u32 glErrorNo = GL->GetError();       \
  DumpGlErrorEnum(glErrorNo);         \
  Assert(glErrorNo == GL_NO_ERROR); } while (0)

#define GL_NO_ERROR                       0

#define GL_VERSION                        0x1F02
#define GL_SHADING_LANGUAGE_VERSION       0x8B8C

#define GL_BYTE                           0x1400
#define GL_UNSIGNED_BYTE                  0x1401
#define GL_SHORT                          0x1402
#define GL_UNSIGNED_SHORT                 0x1403
#define GL_INT                            0x1404
#define GL_UNSIGNED_INT                   0x1405
#define GL_FLOAT                          0x1406

#define GL_DEPTH_BUFFER_BIT               0x00000100
#define GL_STENCIL_BUFFER_BIT             0x00000400
#define GL_COLOR_BUFFER_BIT               0x00004000

#define GL_TRUE                           1
#define GL_FALSE                          0

#define GL_INVALID_ENUM                   0x0500
#define GL_INVALID_VALUE                  0x0501
#define GL_INVALID_OPERATION              0x0502
#define GL_OUT_OF_MEMORY                  0x0505
#define GL_INVALID_FRAMEBUFFER_OPERATION  0x0506
#define GL_STACK_OVERFLOW                 0x0503
#define GL_STACK_UNDERFLOW                0x0504

#define GL_DEBUG_OUTPUT_SYNCHRONOUS       0x8242
#define GL_DEBUG_SEVERITY_NOTIFICATION    0x826B
#define GL_DEBUG_TYPE_ERROR               0x824C
#define GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR 0x824D
#define GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR  0x824E
#define GL_DEBUG_TYPE_PORTABILITY         0x824F
#define GL_DEBUG_TYPE_PERFORMANCE         0x8250
#define GL_DEBUG_TYPE_OTHER               0x8251

#define GL_COMPILE_STATUS                 0x8B81
#define GL_INFO_LOG_LENGTH                0x8B84
#define GL_VERTEX_SHADER                  0x8B31
#define GL_FRAGMENT_SHADER                0x8B30
#define GL_LINK_STATUS                    0x8B82

#define GL_COMPRESSED_RGBA_S3TC_DXT1_EXT  0x83F1
#define GL_COMPRESSED_RGBA_S3TC_DXT3_EXT  0x83F2
#define GL_COMPRESSED_RGBA_S3TC_DXT5_EXT  0x83F3

#define GL_TEXTURE_1D                     0x0DE0
#define GL_TEXTURE_2D                     0x0DE1
#define GL_TEXTURE_WIDTH                  0x1000
#define GL_TEXTURE_HEIGHT                 0x1001
#define GL_TEXTURE_BORDER_COLOR           0x1004

#define GL_FRAMEBUFFER                    0x8D40
#define GL_FRAMEBUFFER_COMPLETE           0x8CD5

#define GL_PACK_ALIGNMENT                 0x0D05
#define GL_UNPACK_ALIGNMENT               0x0CF5

#define GL_NEAREST_MIPMAP_NEAREST         0x2700
#define GL_LINEAR_MIPMAP_NEAREST          0x2701
#define GL_NEAREST_MIPMAP_LINEAR          0x2702
#define GL_LINEAR_MIPMAP_LINEAR           0x2703
#define GL_TEXTURE_MAG_FILTER             0x2800
#define GL_TEXTURE_MIN_FILTER             0x2801
#define GL_TEXTURE_WRAP_S                 0x2802
#define GL_TEXTURE_WRAP_T                 0x2803
#define GL_REPEAT                         0x2901
#define GL_CLAMP_TO_BORDER                0x812D

#define GL_NEAREST                        0x2600
#define GL_LINEAR                         0x2601
#define GL_NEVER                          0x0200
#define GL_LESS                           0x0201
#define GL_EQUAL                          0x0202
#define GL_LEQUAL                         0x0203
#define GL_GREATER                        0x0204
#define GL_NONE                           0

#define GL_TEXTURE_COMPARE_FUNC           0x884D
#define GL_TEXTURE_COMPARE_MODE           0x884C

#define GL_CLAMP_TO_EDGE                  0x812F

#define GL_TEXTURE_1D                     0x0DE0
#define GL_TEXTURE_2D                     0x0DE1
#define GL_TEXTURE_3D                     0x806F

#define GL_RGB                            0x1907
#define GL_RGB32F                         0x8815

#define GL_RGBA                           0x1908
#define GL_RGBA8                          0x8058
#define GL_RGBA32F                        0x8814

#define GL_R32F                           0x822E
#define GL_RED                            0x1903
#define GL_VERTEX_ARRAY                   0x8074

#define GL_TEXTURE0                       0x84C0
#define GL_COLOR_ATTACHMENT0              0x8CE0
#define GL_DEPTH_COMPONENT                0x1902
#define GL_DEPTH_COMPONENT32F             0x8CAC
#define GL_DEPTH_ATTACHMENT               0x8D00
#define GL_DEPTH_TEST                     0x0B71

#define GL_ARRAY_BUFFER                   0x8892
#define GL_WRITE_ONLY                     0x88B9
#define GL_MAP_READ_BIT                   0x0001
#define GL_MAP_WRITE_BIT                  0x0002
#define GL_STATIC_DRAW                    0x88E4

#define GL_CULL_FACE                      0x0B44
#define GL_BACK                           0x0405
#define GL_CW                             0x0900
#define GL_CCW                            0x0901

#define GL_TRIANGLES                      0x0004

#define GL_BLEND                          0x0BE2
#define GL_SRC_ALPHA                      0x0302
#define GL_ONE_MINUS_SRC_ALPHA            0x0303

typedef void GLvoid;
typedef unsigned int GLenum;
typedef float GLfloat;
typedef int GLint;
typedef int GLsizei;
typedef unsigned int GLbitfield;
typedef double GLdouble;
typedef unsigned int GLuint;
typedef unsigned char GLboolean;
typedef unsigned char GLubyte;

typedef ptrdiff_t GLsizeiptr;
typedef ptrdiff_t GLintptr;

typedef char GLchar;
typedef short GLshort;
typedef signed char GLbyte;
typedef unsigned short GLushort;

typedef void (*GLDEBUGPROC)(GLenum source,GLenum type,GLuint id,GLenum severity,GLsizei length,const GLchar *message,const void *userParam);

#if 0
exported_function const GLubyte* glGetString (GLenum name);
exported_function GLenum glGetError (void);
exported_function void glDebugMessageCallback (GLDEBUGPROC callback, const void *userParam);

exported_function void glEnable (GLenum cap);
exported_function void glDisable (GLenum cap);
exported_function void glCullFace (GLenum mode);
exported_function void glViewport (GLint x, GLint y, GLsizei width, GLsizei height);
exported_function void glDepthFunc (GLenum func);
exported_function void glBlendFunc (GLenum sfactor, GLenum dfactor);

exported_function void glDrawArrays (GLenum mode, GLint first, GLsizei count);

exported_function void glClear (GLbitfield mask);
exported_function void glClearColor (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
exported_function void glClearDepth (GLdouble depth);

exported_function void glGenTextures(GLsizei n, GLuint *textures);
exported_function void glBindTextures (GLuint first, GLsizei count, const GLuint *textures);
exported_function void glBindTexture (GLenum target, GLuint texture);
exported_function void glDeleteTextures (GLsizei n, const GLuint *textures);
exported_function void glActiveTexture (GLenum texture);

exported_function void glTexStorage1D (GLenum target, GLsizei levels, GLenum internalformat, GLsizei width);
exported_function void glTexStorage2D (GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height);
exported_function void glTexStorage3D (GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth);

exported_function void glTexImage1D (GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const void *pixels);
exported_function void glTexImage2D (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels);
exported_function void glTexImage3D (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void *pixels);

exported_function void glTexSubImage1D (GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const void *pixels);
exported_function void glTexSubImage2D (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels);
exported_function void glTexSubImage3D (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *pixels);

exported_function void glTexParameterf (GLenum target, GLenum pname, GLfloat param);
exported_function void glTexParameterfv (GLenum target, GLenum pname, const GLfloat *params);
exported_function void glTexParameteri (GLenum target, GLenum pname, GLint param);
exported_function void glTexParameteriv (GLenum target, GLenum pname, const GLint *params);

exported_function void glCompressedTexImage3D (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const void *data);
exported_function void glCompressedTexImage2D (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void *data);
exported_function void glCompressedTexImage1D (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const void *data);

exported_function void glPixelStoref (GLenum pname, GLfloat param);
exported_function void glPixelStorei (GLenum pname, GLint param);

exported_function void glEnableVertexAttribArray(GLuint index);
exported_function void glDisableVertexAttribArray(GLuint index);

exported_function void glVertexAttribPointer (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer);

exported_function void glBindFramebuffer (GLenum target, GLuint framebuffer);
exported_function void glDeleteFramebuffers (GLsizei n, const GLuint *framebuffers);
exported_function void glGenFramebuffers (GLsizei n, GLuint *framebuffers);
exported_function GLenum glCheckFramebufferStatus (GLenum target);
exported_function void glFramebufferTexture1D (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
exported_function void glFramebufferTexture2D (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
exported_function void glFramebufferTexture3D (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset);
exported_function void glFramebufferTextureLayer (GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer);
exported_function void glFramebufferRenderbuffer (GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);

exported_function void glCompileShader (GLuint shader);
exported_function GLuint glCreateProgram (void);
exported_function GLuint glCreateShader (GLenum type);
exported_function void glLinkProgram (GLuint program);
exported_function void glShaderSource (GLuint shader, GLsizei count, const GLchar *const*string, const GLint *length);
exported_function void glUseProgram (GLuint program);

exported_function void glGetProgramInfoLog (GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
exported_function void glGetShaderInfoLog (GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog);

exported_function void glDeleteProgram (GLuint program);
exported_function void glDeleteShader (GLuint shader);
exported_function void glDetachShader (GLuint program, GLuint shader);

exported_function void glUniform1f (GLint location, GLfloat v0);
exported_function void glUniform2f (GLint location, GLfloat v0, GLfloat v1);
exported_function void glUniform3f (GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
exported_function void glUniform4f (GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
exported_function void glUniform1i (GLint location, GLint v0);
exported_function void glUniform2i (GLint location, GLint v0, GLint v1);
exported_function void glUniform3i (GLint location, GLint v0, GLint v1, GLint v2);
exported_function void glUniform4i (GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
exported_function void glUniform1fv (GLint location, GLsizei count, const GLfloat *value);
exported_function void glUniform2fv (GLint location, GLsizei count, const GLfloat *value);
exported_function void glUniform3fv (GLint location, GLsizei count, const GLfloat *value);
exported_function void glUniform4fv (GLint location, GLsizei count, const GLfloat *value);
exported_function void glUniform1iv (GLint location, GLsizei count, const GLint *value);
exported_function void glUniform2iv (GLint location, GLsizei count, const GLint *value);
exported_function void glUniform3iv (GLint location, GLsizei count, const GLint *value);
exported_function void glUniform4iv (GLint location, GLsizei count, const GLint *value);
exported_function void glUniformMatrix2fv (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
exported_function void glUniformMatrix3fv (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
exported_function void glUniformMatrix4fv (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
exported_function void glUniform1ui (GLint location, GLuint v0);
exported_function void glUniform2ui (GLint location, GLuint v0, GLuint v1);
exported_function void glUniform3ui (GLint location, GLuint v0, GLuint v1, GLuint v2);
exported_function void glUniform4ui (GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3);
exported_function void glUniform1uiv (GLint location, GLsizei count, const GLuint *value);
exported_function void glUniform2uiv (GLint location, GLsizei count, const GLuint *value);
exported_function void glUniform3uiv (GLint location, GLsizei count, const GLuint *value);
exported_function void glUniform4uiv (GLint location, GLsizei count, const GLuint *value);

exported_function GLint glGetUniformLocation (GLuint program, const GLchar *name);

exported_function void glGetShaderiv (GLuint shader, GLenum pname, GLint *params);
exported_function void glGetProgramiv (GLuint program, GLenum pname, GLint *params);
exported_function void glAttachShader (GLuint program, GLuint shader);

exported_function void glBindBuffer (GLenum target, GLuint buffer);
exported_function void glDeleteBuffers (GLsizei n, const GLuint *buffers);
exported_function void glGenBuffers (GLsizei n, GLuint *buffers);
exported_function void glBufferData (GLenum target, GLsizeiptr size, const void *data, GLenum usage);
exported_function void* glMapBuffer (GLenum target, GLenum access);
exported_function void* glMapBufferRange (GLenum target, GLintptr offset, GLsizeiptr length,  GLenum access);
exported_function GLboolean glUnmapBuffer (GLenum target);

exported_function void glDrawBuffers (GLsizei n, const GLenum *bufs);
#endif

bonsai_function void
DumpGlErrorEnum(u32 ErrorNumber)
{
  if ( ErrorNumber != 0 )
  {
    Error("%d", ErrorNumber);
  }

  switch (ErrorNumber)
  {
    case GL_INVALID_ENUM:
    {
      Error(" GL_INVALID_ENUM: An unacceptable value is specified for an enumerated argument. The offending command is ignored and has no other side effect than to set the error flag.");
    } break;

    case GL_INVALID_VALUE:
    {
      Error(" GL_INVALID_VALUE: A numeric argument is out of range. The offending command is ignored and has no other side effect than to set the error flag.");
    } break;

    case GL_INVALID_OPERATION:
    {
      Error(" GL_INVALID_OPERATION: The specified operation is not allowed in the current state. The offending command is ignored and has no other side effect than to set the error flag.");
    } break;

    case GL_INVALID_FRAMEBUFFER_OPERATION:
    {
      Error(" GL_INVALID_FRAMEBUFFER_OPERATION: The framebuffer object is not complete. The offending command is ignored and has no other side effect than to set the error flag.");
    } break;

    case GL_OUT_OF_MEMORY:
    {
      Error(" GL_OUT_OF_MEMORY: There is not enough memory left to execute the command. The state of the GL is undefined, except for the state of the error flags, after this error is recorded.");
    } break;

    case GL_STACK_UNDERFLOW:
    {
      Error(" GL_STACK_UNDERFLOW: An attempt has been made to perform an operation that would cause an internal stack to underflow.");
    } break;

    case GL_STACK_OVERFLOW:
    {
      Error(" GL_STACK_OVERFLOW: An attempt has been made to perform an operation that would cause an internal stack to overflow.");
    } break;

    case GL_NO_ERROR:
    {
      // Happy days :D
    } break;

    default :
    {
      Error("Some weird OpenGL shit happened.");
    } break;
  }

  return;
}













typedef const GLubyte*  (*OpenglGetString)                 (GLenum name);
typedef GLenum          (*OpenglGetError)                  (void);
typedef void            (*OpenglDebugMessageCallback)      (GLDEBUGPROC callback, const void *userParam);
typedef void            (*OpenglEnable)                    (GLenum cap);
typedef void            (*OpenglDisable)                   (GLenum cap);
typedef void            (*OpenglCullFace)                  (GLenum mode);
typedef void            (*OpenglViewport)                  (GLint x, GLint y, GLsizei width, GLsizei height);
typedef void            (*OpenglDepthFunc)                 (GLenum func);
typedef void            (*OpenglBlendFunc)                 (GLenum sfactor, GLenum dfactor);
typedef void            (*OpenglDrawArrays)                (GLenum mode, GLint first, GLsizei count);
typedef void            (*OpenglClear)                     (GLbitfield mask);
typedef void            (*OpenglClearColor)                (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
typedef void            (*OpenglClearDepth)                (GLdouble depth);
typedef void            (*OpenglGenTextures)               (GLsizei n, GLuint *textures);
typedef void            (*OpenglBindTextures)              (GLuint first, GLsizei count, const GLuint *textures);
typedef void            (*OpenglBindTexture)               (GLenum target, GLuint texture);
typedef void            (*OpenglDeleteTextures)            (GLsizei n, const GLuint *textures);
typedef void            (*OpenglActiveTexture)             (GLenum texture);
typedef void            (*OpenglTexStorage1D)              (GLenum target, GLsizei levels, GLenum internalformat, GLsizei width);
typedef void            (*OpenglTexStorage2D)              (GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height);
typedef void            (*OpenglTexStorage3D)              (GLenum target, GLsizei levels, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth);
typedef void            (*OpenglTexImage1D)                (GLenum target, GLint level, GLint internalformat, GLsizei width, GLint border, GLenum format, GLenum type, const void *pixels);
typedef void            (*OpenglTexImage2D)                (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void *pixels);
typedef void            (*OpenglTexImage3D)                (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void *pixels);
typedef void            (*OpenglTexSubImage1D)             (GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const void *pixels);
typedef void            (*OpenglTexSubImage2D)             (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void *pixels);
typedef void            (*OpenglTexSubImage3D)             (GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void *pixels);
typedef void            (*OpenglTexParameterf)             (GLenum target, GLenum pname, GLfloat param);
typedef void            (*OpenglTexParameterfv)            (GLenum target, GLenum pname, const GLfloat *params);
typedef void            (*OpenglTexParameteri)             (GLenum target, GLenum pname, GLint param);
typedef void            (*OpenglTexParameteriv)            (GLenum target, GLenum pname, const GLint *params);
typedef void            (*OpenglCompressedTexImage3D)      (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const void *data);
typedef void            (*OpenglCompressedTexImage2D)      (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void *data);
typedef void            (*OpenglCompressedTexImage1D)      (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLint border, GLsizei imageSize, const void *data);
typedef void            (*OpenglPixelStoref)               (GLenum pname, GLfloat param);
typedef void            (*OpenglPixelStorei)               (GLenum pname, GLint param);
typedef void            (*OpenglEnableVertexAttribArray)   (GLuint index);
typedef void            (*OpenglDisableVertexAttribArray)  (GLuint index);
typedef void            (*OpenglVertexAttribPointer)       (GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer);
typedef void            (*OpenglBindFramebuffer)           (GLenum target, GLuint framebuffer);
typedef void            (*OpenglDeleteFramebuffers)        (GLsizei n, const GLuint *framebuffers);
typedef void            (*OpenglGenFramebuffers)           (GLsizei n, GLuint *framebuffers);
typedef GLenum          (*OpenglCheckFramebufferStatus)    (GLenum target);
typedef void            (*OpenglFramebufferTexture1D)      (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
typedef void            (*OpenglFramebufferTexture2D)      (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
typedef void            (*OpenglFramebufferTexture3D)      (GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset);
typedef void            (*OpenglFramebufferTextureLayer)   (GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer);
typedef void            (*OpenglFramebufferRenderbuffer)   (GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
typedef void            (*OpenglCompileShader)             (GLuint shader);
typedef GLuint          (*OpenglCreateProgram)             (void);
typedef GLuint          (*OpenglCreateShader)              (GLenum type);
typedef void            (*OpenglLinkProgram)               (GLuint program);
typedef void            (*OpenglShaderSource)              (GLuint shader, GLsizei count, const GLchar *const*string, const GLint *length);
typedef void            (*OpenglUseProgram)                (GLuint program);
typedef void            (*OpenglGetProgramInfoLog)         (GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
typedef void            (*OpenglGetShaderInfoLog)          (GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
typedef void            (*OpenglDeleteProgram)             (GLuint program);
typedef void            (*OpenglDeleteShader)              (GLuint shader);
typedef void            (*OpenglDetachShader)              (GLuint program, GLuint shader);
typedef void            (*OpenglUniform1f)                 (GLint location, GLfloat v0);
typedef void            (*OpenglUniform2f)                 (GLint location, GLfloat v0, GLfloat v1);
typedef void            (*OpenglUniform3f)                 (GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
typedef void            (*OpenglUniform4f)                 (GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
typedef void            (*OpenglUniform1i)                 (GLint location, GLint v0);
typedef void            (*OpenglUniform2i)                 (GLint location, GLint v0, GLint v1);
typedef void            (*OpenglUniform3i)                 (GLint location, GLint v0, GLint v1, GLint v2);
typedef void            (*OpenglUniform4i)                 (GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
typedef void            (*OpenglUniform1fv)                (GLint location, GLsizei count, const GLfloat *value);
typedef void            (*OpenglUniform2fv)                (GLint location, GLsizei count, const GLfloat *value);
typedef void            (*OpenglUniform3fv)                (GLint location, GLsizei count, const GLfloat *value);
typedef void            (*OpenglUniform4fv)                (GLint location, GLsizei count, const GLfloat *value);
typedef void            (*OpenglUniform1iv)                (GLint location, GLsizei count, const GLint *value);
typedef void            (*OpenglUniform2iv)                (GLint location, GLsizei count, const GLint *value);
typedef void            (*OpenglUniform3iv)                (GLint location, GLsizei count, const GLint *value);
typedef void            (*OpenglUniform4iv)                (GLint location, GLsizei count, const GLint *value);
typedef void            (*OpenglUniformMatrix2fv)          (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void            (*OpenglUniformMatrix3fv)          (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void            (*OpenglUniformMatrix4fv)          (GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
typedef void            (*OpenglUniform1ui)                (GLint location, GLuint v0);
typedef void            (*OpenglUniform2ui)                (GLint location, GLuint v0, GLuint v1);
typedef void            (*OpenglUniform3ui)                (GLint location, GLuint v0, GLuint v1, GLuint v2);
typedef void            (*OpenglUniform4ui)                (GLint location, GLuint v0, GLuint v1, GLuint v2, GLuint v3);
typedef void            (*OpenglUniform1uiv)               (GLint location, GLsizei count, const GLuint *value);
typedef void            (*OpenglUniform2uiv)               (GLint location, GLsizei count, const GLuint *value);
typedef void            (*OpenglUniform3uiv)               (GLint location, GLsizei count, const GLuint *value);
typedef void            (*OpenglUniform4uiv)               (GLint location, GLsizei count, const GLuint *value);
typedef GLint           (*OpenglGetUniformLocation)        (GLuint program, const GLchar *name);
typedef void            (*OpenglGetShaderiv)               (GLuint shader, GLenum pname, GLint *params);
typedef void            (*OpenglGetProgramiv)              (GLuint program, GLenum pname, GLint *params);
typedef void            (*OpenglAttachShader)              (GLuint program, GLuint shader);
typedef void            (*OpenglBindBuffer)                (GLenum target, GLuint buffer);
typedef void            (*OpenglDeleteBuffers)             (GLsizei n, const GLuint *buffers);
typedef void            (*OpenglGenBuffers)                (GLsizei n, GLuint *buffers);
typedef void            (*OpenglBufferData)                (GLenum target, GLsizeiptr size, const void *data, GLenum usage);
typedef void*           (*OpenglMapBuffer)                 (GLenum target, GLenum access);
typedef void*           (*OpenglMapBufferRange)            (GLenum target, GLintptr offset, GLsizeiptr length,  GLenum access);
typedef GLboolean       (*OpenglUnmapBuffer)               (GLenum target);
typedef void            (*OpenglDrawBuffers)               (GLsizei n, const GLenum *bufs);



struct gl_extensions
{
  OpenglGetString GetString;
  OpenglGetError GetError;
  OpenglDebugMessageCallback DebugMessageCallback;
  OpenglEnable Enable;
  OpenglDisable Disable;
  OpenglCullFace CullFace;
  OpenglViewport Viewport;
  OpenglDepthFunc DepthFunc;
  OpenglBlendFunc BlendFunc;
  OpenglDrawArrays DrawArrays;
  OpenglClear Clear;
  OpenglClearColor ClearColor;
  OpenglClearDepth ClearDepth;
  OpenglGenTextures GenTextures;
  OpenglBindTextures BindTextures;
  OpenglBindTexture BindTexture;
  OpenglDeleteTextures DeleteTextures;
  OpenglActiveTexture ActiveTexture;
  OpenglTexStorage1D TexStorage1D;
  OpenglTexStorage2D TexStorage2D;
  OpenglTexStorage3D TexStorage3D;
  OpenglTexImage1D TexImage1D;
  OpenglTexImage2D TexImage2D;
  OpenglTexImage3D TexImage3D;
  OpenglTexSubImage1D TexSubImage1D;
  OpenglTexSubImage2D TexSubImage2D;
  OpenglTexSubImage3D TexSubImage3D;
  OpenglTexParameterf TexParameterf;
  OpenglTexParameterfv TexParameterfv;
  OpenglTexParameteri TexParameteri;
  OpenglTexParameteriv TexParameteriv;
  OpenglCompressedTexImage3D CompressedTexImage3D;
  OpenglCompressedTexImage2D CompressedTexImage2D;
  OpenglCompressedTexImage1D CompressedTexImage1D;
  OpenglPixelStoref PixelStoref;
  OpenglPixelStorei PixelStorei;
  OpenglEnableVertexAttribArray EnableVertexAttribArray;
  OpenglDisableVertexAttribArray DisableVertexAttribArray;
  OpenglVertexAttribPointer VertexAttribPointer;
  OpenglBindFramebuffer BindFramebuffer;
  OpenglDeleteFramebuffers DeleteFramebuffers;
  OpenglGenFramebuffers GenFramebuffers;
  OpenglCheckFramebufferStatus CheckFramebufferStatus;
  OpenglFramebufferTexture1D FramebufferTexture1D;
  OpenglFramebufferTexture2D FramebufferTexture2D;
  OpenglFramebufferTexture3D FramebufferTexture3D;
  OpenglFramebufferTextureLayer FramebufferTextureLayer;
  OpenglFramebufferRenderbuffer FramebufferRenderbuffer;
  OpenglCompileShader CompileShader;
  OpenglCreateProgram CreateProgram;
  OpenglCreateShader CreateShader;
  OpenglLinkProgram LinkProgram;
  OpenglShaderSource ShaderSource;
  OpenglUseProgram UseProgram;
  OpenglGetProgramInfoLog GetProgramInfoLog;
  OpenglGetShaderInfoLog GetShaderInfoLog;
  OpenglDeleteProgram DeleteProgram;
  OpenglDeleteShader DeleteShader;
  OpenglDetachShader DetachShader;
  OpenglUniform1f Uniform1f;
  OpenglUniform2f Uniform2f;
  OpenglUniform3f Uniform3f;
  OpenglUniform4f Uniform4f;
  OpenglUniform1i Uniform1i;
  OpenglUniform2i Uniform2i;
  OpenglUniform3i Uniform3i;
  OpenglUniform4i Uniform4i;
  OpenglUniform1fv Uniform1fv;
  OpenglUniform2fv Uniform2fv;
  OpenglUniform3fv Uniform3fv;
  OpenglUniform4fv Uniform4fv;
  OpenglUniform1iv Uniform1iv;
  OpenglUniform2iv Uniform2iv;
  OpenglUniform3iv Uniform3iv;
  OpenglUniform4iv Uniform4iv;
  OpenglUniformMatrix2fv UniformMatrix2fv;
  OpenglUniformMatrix3fv UniformMatrix3fv;
  OpenglUniformMatrix4fv UniformMatrix4fv;
  OpenglUniform1ui Uniform1ui;
  OpenglUniform2ui Uniform2ui;
  OpenglUniform3ui Uniform3ui;
  OpenglUniform4ui Uniform4ui;
  OpenglUniform1uiv Uniform1uiv;
  OpenglUniform2uiv Uniform2uiv;
  OpenglUniform3uiv Uniform3uiv;
  OpenglUniform4uiv Uniform4uiv;
  OpenglGetUniformLocation GetUniformLocation;
  OpenglGetShaderiv GetShaderiv;
  OpenglGetProgramiv GetProgramiv;
  OpenglAttachShader AttachShader;
  OpenglBindBuffer BindBuffer;
  OpenglDeleteBuffers DeleteBuffers;
  OpenglGenBuffers GenBuffers;
  OpenglBufferData BufferData;
  OpenglMapBuffer MapBuffer;
  OpenglMapBufferRange MapBufferRange;
  OpenglUnmapBuffer UnmapBuffer;
  OpenglDrawBuffers DrawBuffers;
  // Platform specific (wgl / glX)
  /* OpenglSwapInterval SwapInterval; */
};

global_variable gl_extensions *GL;

bonsai_function b32
CheckShadingLanguageVersion()
{
  char *OpenGlVersion = (char*)GL->GetString(GL_VERSION);
  r32 ShadingLanguageVersion = (r32)atof((char*)GL->GetString(GL_SHADING_LANGUAGE_VERSION));

  Info("OpenGl Ver. (%s)", OpenGlVersion );
  Info("GLSL   Ver. (%f)", ShadingLanguageVersion );

  r32 RequiredShadingLanguageVersion = 3.3f;
  if (ShadingLanguageVersion < RequiredShadingLanguageVersion)
  {
    Error("Unsupported Version of GLSL :: Got %f, Needed: %f", ShadingLanguageVersion, RequiredShadingLanguageVersion);
  }

  b32 Result = (ShadingLanguageVersion > RequiredShadingLanguageVersion);
  return Result;
}

