#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII

// Note(Jesse): Must match shader define in header.glsl
#define DEBUG_TEXTURE_DIM 512

#if 0
texture
LoadDDS(const char * FilePath, memory_arena *Arena)
{
  texture Result = {};

  FILE *TextureFile = fopen(FilePath, "rb");

  if (!TextureFile){
    Error("Couldn't open shader - Shit!");
    Assert(False);
    return Result;
  }

  const u32 FileCodeLength = 4;
  char filecode[FileCodeLength];

  ReadBytes((u8*)filecode, FileCodeLength, TextureFile);
  if (!StringsMatch(CS(filecode), CS("DDS ")) )
  {
    Error("Invalid File format opening DDS file");
    Assert(False);
    fclose(TextureFile);
    return Result;
  }

  const u32 HeaderLength = 124;
  u32 Header[HeaderLength];
  ReadBytes((u8*)Header, HeaderLength, TextureFile);

  s32 height      = (s32)Header[2];
  s32 width       = (s32)Header[3];
  u32 linearSize  = Header[4];
  s32 mipMapCount = (s32)Header[6];
  u32 fourCC      = Header[20];

  Result.Dim.x = (s32)width;
  Result.Dim.y = (s32)height;

  u32 BufferSize = mipMapCount > 1 ? linearSize * 2 : linearSize;
  u8* Buffer = Allocate(u8, Arena, BufferSize);

  ReadBytes(Buffer, BufferSize, TextureFile);

  s32 Closed = fclose(TextureFile);
  Assert(Closed == 0);

  unsigned int format;

  switch(fourCC)
  {
  case FOURCC_DXT1:
    format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
    break;
  case FOURCC_DXT3:
    format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
    break;
  case FOURCC_DXT5:
    format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
    break;
  default:
    return Result;
  }

  s32 offset = 0;

  glGenTextures(1, &Result.ID);

  glBindTexture(GL_TEXTURE_2D, Result.ID);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);


  s32 blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
  for (s32 level = 0; level < mipMapCount && (width || height); ++level)
  {
    s32 size = ((width+3)/4)*((height+3)/4)*blockSize;
    glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height,
      0, size, Buffer + offset);

    offset += size;
    width  /= 2;
    height /= 2;

    // Deal with Non-Power-Of-Two textures. This code is not included in the webpage to reduce clutter.
    if(width < 1) width = 1;
    if(height < 1) height = 1;

  }

  return Result;
}
#endif

// TODO(Jesse, id: 136, tags: allocation, speed): Why are these allocated on the heap?  Seems unnecessary..
texture *
GenTexture(v2i Dim, memory_arena *Mem, u32 TextureDimensionality = GL_TEXTURE_2D)
{
  texture *Texture = Allocate(texture, Mem, 1);
  Texture->Dim = Dim;

  GL.GenTextures(1, &Texture->ID);
  GL.BindTexture(TextureDimensionality, Texture->ID);

  // Note(Jesse): This is required to be set if mipmapping is off.  The default
  // behavior is to lerp between the two closest mipmap levels, and when there
  // is only one level that fails, at least on my GL implementation.
  GL.TexParameteri(TextureDimensionality, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  //

  GL.TexParameteri(TextureDimensionality, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  GL.TexParameteri(TextureDimensionality, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  GL.TexParameteri(TextureDimensionality, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  GL.TexParameteri(TextureDimensionality, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
  /* glTexParameteri(TextureDimensionality, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE); */
  GL.TexParameteri(TextureDimensionality, GL_TEXTURE_COMPARE_MODE, GL_NONE);

  return Texture;
}

texture *
MakeTexture_RGBA(v2i Dim, u32* Data, memory_arena *Mem, u32 MaxTextureSlices = 1)
{
  Assert(MaxTextureSlices);

  b32 Multidimensional = MaxTextureSlices > 1;

  u32 TextureDimensionality = Multidimensional ? GL_TEXTURE_2D_ARRAY : GL_TEXTURE_2D;
  texture *Texture = GenTexture(Dim, Mem, TextureDimensionality);

  u32 InternalFormat = GL_RGBA8;
  u32 TextureFormat = GL_RGBA;
  u32 ElementType = GL_UNSIGNED_BYTE;
  if (MaxTextureSlices == 1)
  {
    GL.TexImage2D(GL_TEXTURE_2D, 0, (s32)InternalFormat,
        Texture->Dim.x, Texture->Dim.y, 0, TextureFormat, ElementType, Data);
  }
  else
  {
    /*
     */

#if 0
    glTexImage3D(GL_TEXTURE_2D_ARRAY, MaxTextureSlices, InternalFormat,
        Texture->Dim.x, Texture->Dim.y, MaxTextureSlices,
        0, TextureFormat, ElementType,
        0);
#else

    /* TODO(Jesse, id: 137, tags: robustness, open_question): This _should_ be
     * able to be glTexImage3D, but the driver is throwing an error .. why?!
     */

    s32 Mips = (s32)MaxTextureSlices;
    GL.TexStorage3D(GL_TEXTURE_2D_ARRAY, Mips, InternalFormat,
                    DEBUG_TEXTURE_DIM, DEBUG_TEXTURE_DIM, (s32)MaxTextureSlices);
#endif

    s32 xOffset = 0;
    s32 yOffset = 0;
    s32 zOffset = 0;

    s32 TextureDepth = 1;
    GL.TexSubImage3D( GL_TEXTURE_2D_ARRAY, 0,
                      xOffset, yOffset, zOffset,
                      Texture->Dim.x, Texture->Dim.y, TextureDepth,
                      TextureFormat, ElementType, Data );
  }

  GL.BindTexture(TextureDimensionality, 0);

  return Texture;
}

texture *
MakeTexture_RGBA(v2i Dim, v4* Data, memory_arena *Mem)
{
  texture *Texture = GenTexture(Dim, Mem);

  u32 TextureFormat = GL_RGBA;
  s32 InternalFormat = GL_RGBA32F;
  u32 ElementType = GL_FLOAT;
  GL.TexImage2D(GL_TEXTURE_2D, 0, InternalFormat,
      Texture->Dim.x, Texture->Dim.y, 0,  TextureFormat, ElementType, Data);

  GL.BindTexture(GL_TEXTURE_2D, 0);

  return Texture;
}

texture *
MakeTexture_SingleChannel(v2i Dim, memory_arena *Mem)
{
  texture *Texture = GenTexture(Dim, Mem);

  GL.TexImage2D(GL_TEXTURE_2D, 0, GL_R32F,
      Texture->Dim.x, Texture->Dim.y, 0,  GL_RED, GL_FLOAT, 0);

  GL.TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  GL.TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  GL.BindTexture(GL_TEXTURE_2D, 0);

  return Texture;
}

texture *
MakeTexture_RGB(v2i Dim, const v3* Data, memory_arena *Mem)
{
  texture *Texture = GenTexture(Dim, Mem);

  /* TODO(Jesse, id: 138, tags: opengl, memory_consumption): 32F is only
   * necessary for reprojection of Position for calculating AO.  Consider
   * passing this in when creating a Texture?
   */

  GL.TexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F,
      Texture->Dim.x, Texture->Dim.y, 0,  GL_RGB, GL_FLOAT, Data);

  GL.TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  GL.TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  GL.BindTexture(GL_TEXTURE_2D, 0);

  return Texture;
}

texture *
MakeDepthTexture(v2i Dim, memory_arena *Mem)
{
  texture *Texture = GenTexture(Dim, Mem);
  GL.TexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F,
    Texture->Dim.x, Texture->Dim.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);

  GL.TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  GL.TexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

  r32 BorderColors[4] = {1};
  GL.TexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, &BorderColors[0]);

  GL.BindTexture(GL_TEXTURE_2D, 0);

  return Texture;
}

link_internal void
FramebufferDepthTexture(texture *Tex)
{
  GL.FramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, Tex->ID, 0);
}

void
FramebufferTexture(framebuffer *FBO, texture *Tex)
{
  u32 Attachment = FBO->Attachments++;
  GL.FramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + Attachment, GL_TEXTURE_2D, Tex->ID, 0);
}

texture*
LoadBitmap(const char* FilePath, memory_arena *Arena, u32 SliceCount)
{
  bitmap TexBitmap = ReadBitmapFromDisk(FilePath, Arena);
  texture* Result = MakeTexture_RGBA(TexBitmap.Dim, TexBitmap.Pixels.Start, Arena, SliceCount);
  return Result;
}

texture*
LoadBitmap(const char* FilePath, memory_arena *Arena)
{
  bitmap TexBitmap = ReadBitmapFromDisk(FilePath, TranArena);
  texture* Result = MakeTexture_RGBA(TexBitmap.Dim, TexBitmap.Pixels.Start, Arena);
  return Result;
}
