
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
  PushBonsaiRenderCommandAllocateTexture(RenderQ, Texture, Cast(void*, Data));
}
