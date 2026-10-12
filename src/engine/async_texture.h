link_internal void
MakeTexture_RGB_Async(  work_queue *RenderQ,
                           texture *Texture,
                               v2i  Dim,
                          const v3 *Data,
                                cs  DebugName,
            texture_storage_format  StorageFormat = TextureStorageFormat_RGB32F);



link_internal void
MakeTexture_RGBA_Async( work_queue *RenderQ,
                           texture *Texture,
                               v2i  Dim,
                               u32 *Data,
                                cs  DebugName,
            texture_storage_format  StorageFormat = TextureStorageFormat_RGB32F);

link_internal void
AllocateTexture(texture *Texture, void *Data);
