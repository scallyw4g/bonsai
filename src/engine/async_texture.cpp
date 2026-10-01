
#if 1
link_internal void
MakeTexture_RGB_Async(  work_queue *RenderQ,
                           texture *Texture,
                               v2i  Dim,
                          const v3 *Data,
                                cs  DebugName,
            texture_storage_format  StorageFormat = TextureStorageFormat_RGB32F)
{
  u32 Channels = 3;
  u32 Slices = 1;
  b32 IsDepthTexture = False;

  *Texture = InitTexture(Dim, DebugName, StorageFormat, Channels, Slices, False);
  
  AllocateTexture_Async(RenderQ, Texture, Cast(void*, Data));
  /* PushBonsaiRenderCommandAllocateTexture(RenderQ, Texture, Cast(void*, Data)); */
}



link_internal void
MakeTexture_RGBA_Async( work_queue *RenderQ,
                           texture *Texture,
                               v2i  Dim,
                               u32 *Data,
                                cs  DebugName,
            texture_storage_format  StorageFormat = TextureStorageFormat_RGB32F)
{
  u32 Channels = 4;
  u32 Slices = 1;
  b32 IsDepthTexture = False;

  *Texture = InitTexture(Dim, DebugName, StorageFormat, Channels, Slices, False);

  AllocateTexture_Async(RenderQ, Texture, Cast(void*, Data));
  /* PushBonsaiRenderCommandAllocateTexture(RenderQ, Texture, Cast(void*, Data)); */
}
#endif

link_internal void
poof(@async @render)
AllocateTexture(texture *Texture, void *Data)
{
  switch (Texture->Channels)
  {
    case 3:
    {
      *Texture = MakeTexture_RGB(Texture->Dim, Cast(const v3*, Data), Texture->DebugName, Texture->Slices, Texture->Format);
    } break;

    case 4:
    {
      *Texture = MakeTexture_RGBA(Texture->Dim, Cast(u32*, Data), Texture->DebugName, Texture->Slices, Texture->Format);
    } break;

    InvalidDefaultCase;
  }

}
