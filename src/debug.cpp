#ifndef BONSAI_DEBUG_CPP
#define BONSAI_DEBUG_CPP

GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path);

void
initText2D(const char *TexturePath, debug_text_render_group *RG)
{
  RG->Text2DTextureID = loadDDS(TexturePath);

  GL_Global->glGenBuffers(1, &RG->Text2DVertexBufferID);
  GL_Global->glGenBuffers(1, &RG->Text2DUVBufferID);

  RG->Text2DShaderID = LoadShaders("TextVertexShader.vertexshader", "TextVertexShader.fragmentshader");
  RG->Text2DUniformID = GL_Global->glGetUniformLocation(RG->Text2DShaderID, "myTextureSampler");

  return;
}

void
PrintDebugText( debug_text_render_group *RG, const char *Text, int x, int y, int size)
{
  unsigned int length = strlen(Text);
  unsigned int BufferIndex = 0;

  // Fill buffers
  v2 vertices[1024];
  v2 UVs[1024];

  for ( unsigned int i=0 ; i<length ; i++ )
  {
    v2 vertex_up_left    = V2( x+i*size     , y+size );
    v2 vertex_up_right   = V2( x+i*size+size, y+size );
    v2 vertex_down_right = V2( x+i*size+size, y      );
    v2 vertex_down_left  = V2( x+i*size     , y      );

    char character = Text[i];
    float uv_x = (character%16)/16.0f;
    float uv_y = (character/16)/16.0f;

    v2 uv_up_left    = V2( uv_x           , uv_y );
    v2 uv_up_right   = V2( uv_x+1.0f/16.0f, uv_y );
    v2 uv_down_right = V2( uv_x+1.0f/16.0f, (uv_y + 1.0f/16.0f) );
    v2 uv_down_left  = V2( uv_x           , (uv_y + 1.0f/16.0f) );


    vertices[BufferIndex] = vertex_up_left;
    UVs[BufferIndex++] = uv_up_left;

    vertices[BufferIndex] = vertex_down_left;
    UVs[BufferIndex++] = uv_down_left;

    vertices[BufferIndex] = vertex_up_right;
    UVs[BufferIndex++] = uv_up_right;


    vertices[BufferIndex] = vertex_down_right;
    UVs[BufferIndex++] = uv_down_right;

    vertices[BufferIndex] = vertex_up_right;
    UVs[BufferIndex++] = uv_up_right;

    vertices[BufferIndex] = vertex_down_left;
    UVs[BufferIndex++] = uv_down_left;

    continue;
  }

  GL_Global->glBindBuffer(GL_ARRAY_BUFFER, RG->Text2DVertexBufferID);
  AssertNoGlErrors;
  GL_Global->glBufferData(GL_ARRAY_BUFFER, (BufferIndex+1) * sizeof(v2), &vertices[0], GL_STATIC_DRAW);
  AssertNoGlErrors;

  GL_Global->glBindBuffer(GL_ARRAY_BUFFER, RG->Text2DUVBufferID);
  AssertNoGlErrors;
  GL_Global->glBufferData(GL_ARRAY_BUFFER, (BufferIndex+1) * sizeof(v2), &UVs[0], GL_STATIC_DRAW);
  AssertNoGlErrors;

  // Bind shader
  GL_Global->glUseProgram(RG->Text2DShaderID);
  AssertNoGlErrors;

  // Bind texture
  GL_Global->glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, RG->Text2DTextureID);
  // Set our "myTextureSampler" sampler to user Texture Unit 0
  GL_Global->glUniform1i(RG->Text2DUniformID, 0);

  AssertNoGlErrors;
  // 1rst attribute buffer : vertices
  GL_Global->glEnableVertexAttribArray(0);
  GL_Global->glBindBuffer(GL_ARRAY_BUFFER, RG->Text2DVertexBufferID);
  GL_Global->glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0 );

  AssertNoGlErrors;
  // 2nd attribute buffer : UVs
  GL_Global->glEnableVertexAttribArray(1);
  GL_Global->glBindBuffer(GL_ARRAY_BUFFER, RG->Text2DUVBufferID);
  GL_Global->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0 );

  AssertNoGlErrors;
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  AssertNoGlErrors;
  // Draw call
  glDrawArrays(GL_TRIANGLES, 0, BufferIndex );

  AssertNoGlErrors;
  glDisable(GL_BLEND);

  AssertNoGlErrors;
  GL_Global->glDisableVertexAttribArray(0);
  GL_Global->glDisableVertexAttribArray(1);

  AssertNoGlErrors;
  return;
}

void
CleanupText2D(debug_text_render_group *RG)
{
  // Delete buffers
  GL_Global->glDeleteBuffers(1, &RG->Text2DVertexBufferID);
  GL_Global->glDeleteBuffers(1, &RG->Text2DUVBufferID);

  // Delete texture
  glDeleteTextures(1, &RG->Text2DTextureID);

  // Delete shader
  GL_Global->glDeleteProgram(RG->Text2DShaderID);

  return;
}

#endif
