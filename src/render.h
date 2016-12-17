#ifndef RENDER_H
#define RENDER_H

#include <GL/glew.h>

struct VertexBlock
{
  GLfloat *Data;
  int bytesAllocd;
  int filled;
};

#endif
