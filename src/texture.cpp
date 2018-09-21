#ifndef BONSAI_TEXTURE_H
#define BONSAI_TEXTURE_H

global_variable memory_arena *TranArena = {};

#include <stdio.h>
#include <string.h>
#include <texture.hpp>
#include <bitmap.cpp>

#if 0
GLuint loadBMP_custom(const char * FilePath){

  Log("Reading image %s\n", FilePath);

  // Data read from the header of the BMP file
  unsigned char header[54];
  unsigned int dataPos;
  unsigned int imageSize;
  unsigned int width, height;
  // Actual RGB data
  unsigned char * data;

  // Open the file
  FILE * file = fopen(FilePath,"rb");
  if (!file)							    {Log("%s could not be opened. Are you in the right directory ? Don't forget to read the FAQ !\n", FilePath); getchar(); return 0;}

  // Read the header, i.e. the 54 first bytes

  // If less than 54 bytes are read, problem
  if ( fread(header, 1, 54, file)!=54 ){
    Log("Not a correct BMP file\n");
    return 0;
  }
  // A BMP files always begins with "BM"
  if ( header[0]!='B' || header[1]!='M' ){
    Log("Not a correct BMP file\n");
    return 0;
  }
  // Make sure this is a 24bpp file
  if ( *(int*)&(header[0x1E])!=0  )         {Log("Not a correct BMP file\n");    return 0;}
  if ( *(int*)&(header[0x1C])!=24 )         {Log("Not a correct BMP file\n");    return 0;}

  // Read the information about the image
  dataPos    = *(int*)&(header[0x0A]);
  imageSize  = *(int*)&(header[0x22]);
  width      = *(int*)&(header[0x12]);
  height     = *(int*)&(header[0x16]);

  // Some BMP files are misformatted, guess missing information
  if (imageSize==0)    imageSize=width*height*3; // 3 : one byte for each Red, Green and Blue component
  if (dataPos==0)      dataPos=54; // The BMP header is done that way

  // Create a buffer
  data = new unsigned char [imageSize];

  // Read the actual data from the file into the buffer
  fread(data,1,imageSize,file);

  // Everything is in memory now, the file wan be closed
  fclose (file);

  // Create one OpenGL texture
  GLuint textureID;
  glGenTextures(1, &textureID);

  // "Bind" the newly created texture : all future texture functions will modify this texture
  glBindTexture(GL_TEXTURE_2D, textureID);

  // Give the image to OpenGL
  glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

  // OpenGL has now copied the data. Free our own version
  delete [] data;

  // Poor filtering, or ...
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

  // ... nice trilinear filtering.
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glGenerateMipmap(GL_TEXTURE_2D);

  // Return the ID of the texture we just created
  return textureID;
}
#endif

#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII



texture
LoadDDS(const char * FilePath)
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
  if (strncmp(filecode, "DDS ", FileCodeLength) != 0)
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
  u8* Buffer = Allocate(u8, TranArena, BufferSize);

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

// TODO(Jesse): Why are these allocated on the heap?  Seems unnecessary..
texture *
GenTexture(v2i Dim, memory_arena *Mem)
{
  texture *Texture = Allocate(texture, Mem, 1);
  Texture->Dim = Dim;

  glGenTextures(1, &Texture->ID);
  Assert(Texture->ID);

  glBindTexture(GL_TEXTURE_2D, Texture->ID);

  // Note(Jesse): This is required to be set if mipmapping is off.  The default behavior
  // is to lerp between the two closest mipmap levels, and when there is only one level
  // that fails, at least on my GL implementation.
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  //

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
  /* glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE); */
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);

  return Texture;
}

texture *
MakeTexture_RGBA(v2i Dim, u32* Data, memory_arena *Mem)
{
  texture *Texture = GenTexture(Dim, Mem);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8,
      Texture->Dim.x, Texture->Dim.y, 0,  GL_RGBA, GL_UNSIGNED_BYTE, Data);

  glBindTexture(GL_TEXTURE_2D, 0);

  return Texture;
}

texture *
MakeTexture_RGBA(v2i Dim, v4* Data, memory_arena *Mem)
{
  texture *Texture = GenTexture(Dim, Mem);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F,
      Texture->Dim.x, Texture->Dim.y, 0,  GL_RGBA, GL_FLOAT, Data);

  glBindTexture(GL_TEXTURE_2D, 0);

  return Texture;
}

texture *
MakeTexture_SingleChannel(v2i Dim, memory_arena *Mem)
{
  texture *Texture = GenTexture(Dim, Mem);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F,
      Texture->Dim.x, Texture->Dim.y, 0,  GL_RED, GL_FLOAT, 0);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glBindTexture(GL_TEXTURE_2D, 0);

  return Texture;
}

texture *
MakeTexture_RGB(v2i Dim, const v3* Data, memory_arena *Mem)
{
  texture *Texture = GenTexture(Dim, Mem);

  // TODO(Jesse): 32F is only necessary for reprojection of Position for
  // calculating AO.  Consider passing this in when creating a Texture?

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F,
      Texture->Dim.x, Texture->Dim.y, 0,  GL_RGB, GL_FLOAT, Data);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glBindTexture(GL_TEXTURE_2D, 0);

  return Texture;
}

texture *
MakeDepthTexture(v2i Dim, memory_arena *Mem)
{
  texture *Texture = GenTexture(Dim, Mem);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F,
    Texture->Dim.x, Texture->Dim.y, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

  r32 BorderColors[4] = {1};
  glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, &BorderColors[0]);

  glBindTexture(GL_TEXTURE_2D, 0);

  return Texture;
}

void
FramebufferDepthTexture(texture *Tex)
{
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
      GL_TEXTURE_2D, Tex->ID, 0);
  return;
}

void
FramebufferTexture(framebuffer *FBO, texture *Tex)
{
  u32 Attachment = FBO->Attachments++;
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + Attachment,
      GL_TEXTURE_2D, Tex->ID, 0);

  return;
}

texture*
LoadBitmap(const char* FilePath, memory_arena *Arena)
{
  bitmap TexBitmap = ReadBitmapFromDisk(FilePath, Arena);
  texture* Result = MakeTexture_RGBA(TexBitmap.Dim, TexBitmap.Pixels.Start, Arena);
  return Result;
}


#endif
