link_internal void
FramebufferTextureLayer(framebuffer *FBO, texture *Tex, debug_texture_array_slice Layer)
{
  u32 Attachment = FBO->Attachments++;
  GL.FramebufferTextureLayer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + Attachment, Tex->ID, 0, Layer);
  return;
}

link_internal void
AllocateAndInitGeoBuffer(textured_2d_geometry_buffer *Geo, u32 ElementCount, memory_arena *DebugArena)
{
  Geo->Verts  = Allocate(v3, DebugArena, ElementCount);
  Geo->Colors = Allocate(v3, DebugArena, ElementCount);
  Geo->UVs    = Allocate(v3, DebugArena, ElementCount);

  Geo->End = ElementCount;
  Geo->At = 0;
}

link_internal void
AllocateAndInitGeoBuffer(untextured_2d_geometry_buffer *Geo, u32 ElementCount, memory_arena *DebugArena)
{
  Geo->Verts = Allocate(v3, DebugArena, ElementCount);
  Geo->Colors = Allocate(v3, DebugArena, ElementCount);

  Geo->End = ElementCount;
  Geo->At = 0;
  return;
}

link_internal shader
MakeRenderToTextureShader(memory_arena *Memory, m4 *ViewProjection)
{
  shader Shader = LoadShaders( CSz("RenderToTexture.vertexshader"), CSz("RenderToTexture.fragmentshader") );

  shader_uniform **Current = &Shader.FirstUniform;

  *Current = GetUniform(Memory, &Shader, ViewProjection, "ViewProjection");
  Current = &(*Current)->Next;

  return Shader;
}

void
InitRenderToTextureGroup(texture *DebugTextureArray, render_entity_to_texture_group *Group, memory_arena *PermMemory)
{
  AllocateGpuElementBuffer(&Group->GameGeo, (u32)Megabytes(4));

  Group->GameGeoFBO = GenFramebuffer();
  GL.BindFramebuffer(GL_FRAMEBUFFER, Group->GameGeoFBO.ID);

  FramebufferTextureLayer(&Group->GameGeoFBO, DebugTextureArray, DebugTextureArraySlice_Viewport);
  SetDrawBuffers(&Group->GameGeoFBO);
  Group->GameGeoShader = MakeRenderToTextureShader(PermMemory, &Group->ViewProjection);
  Group->Camera = Allocate(camera, PermMemory, 1);
  StandardCamera(Group->Camera, 10000.0f, 100.0f, {});

  GL.ClearColor(f32_MAX, f32_MAX, f32_MAX, f32_MAX);
  GL.ClearDepth(f32_MAX);
}

link_internal b32
InitRenderer2D(renderer_2d *UiGroup, heap_allocator *Heap, memory_arena *PermMemory)
{
  UiGroup->TextGroup     = Allocate(render_buffers_2d, PermMemory, 1);
  UiGroup->CommandBuffer = Allocate(ui_render_command_buffer, PermMemory, 1);

  // TODO(Jesse, memory): Instead of allocate insanely massive buffers (these are ~400x overkill)
  // we should have a system that streams blocks of memory in as-necessary
  // @streaming_ui_render_memory
  u32 ElementCount = (u32)Megabytes(2);
  AllocateAndInitGeoBuffer(&UiGroup->TextGroup->Geo, ElementCount, PermMemory);
  AllocateAndInitGeoBuffer(&UiGroup->Geo, ElementCount, PermMemory);


  auto TextGroup = UiGroup->TextGroup;
  TextGroup->DebugTextureArray = LoadBitmap("texture_atlas_0.bmp", PermMemory, DebugTextureArraySlice_Count);
  GL.GenBuffers(1, &TextGroup->SolidUIVertexBuffer);
  GL.GenBuffers(1, &TextGroup->SolidUIColorBuffer);
  GL.GenBuffers(1, &TextGroup->SolidUIUVBuffer);
  TextGroup->Text2DShader = LoadShaders( CSz("TextVertexShader.vertexshader"), CSz("TextVertexShader.fragmentshader") );
  TextGroup->TextTextureUniform = GL.GetUniformLocation(TextGroup->Text2DShader.ID, "TextTextureSampler");
  UiGroup->TextGroup->SolidUIShader = LoadShaders( CSz("SimpleColor.vertexshader"), CSz("SimpleColor.fragmentshader") );

  v2i TextureDim = V2i(DEBUG_TEXTURE_DIM, DEBUG_TEXTURE_DIM);
  texture *DepthTexture = MakeDepthTexture( TextureDim, PermMemory );
  FramebufferDepthTexture(DepthTexture);

  b32 Result = CheckAndClearFramebuffer();
  Assert(Result);

  GL.BindFramebuffer(GL_FRAMEBUFFER, 0);
  GL.Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  AssertNoGlErrors;

  random_series Entropy = {54623153};
  for (u32 ColorIndex = 0; ColorIndex < RANDOM_COLOR_COUNT; ++ColorIndex)
  {
    UiGroup->DebugColors[ColorIndex] = RandomV3(&Entropy);
  }

  return Result;
}

