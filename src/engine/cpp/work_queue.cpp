void
PushWorkQueueEntry(work_queue *Queue, work_queue_entry *Entry)
{
  TIMED_FUNCTION();
  work_queue_entry* Dest = Queue->Entries + Queue->EnqueueIndex;
  Assert(Dest->Type == type_work_queue_entry_noop);

  *Dest = *Entry;

  FullBarrier;

  Queue->EnqueueIndex = (Queue->EnqueueIndex+1) % WORK_QUEUE_SIZE;

  WakeThread( Queue->GlobalQueueSemaphore );

  return;
}

global_variable memory_arena Global_PermMemory = {};

inline void
QueueChunkForInit(work_queue *Queue, world_chunk *Chunk, heap_allocator *Heap)
{
  TIMED_FUNCTION();
  Assert( Chunk->Data->Flags == Chunk_Uninitialized);

  work_queue_entry Entry = {};

  counted_string AssetFilename = GetAssetFilenameFor(Global_AssetPrefixPath, Chunk->WorldP, TranArena);

  native_file AssetFile = OpenFile(AssetFilename, "r");
  if (AssetFile.Handle)
  {
    world_chunk_file_header Header = ReadWorldChunkFileHeader(&AssetFile);
    CloseFile(&AssetFile);

    if (Header.MeshElementCount)
    {
      Assert(Chunk->Mesh == 0);
      Chunk->Mesh = AllocateMesh(&Global_PermMemory, (u32)Header.MeshElementCount);
    }

    Entry.Type = type_work_queue_entry_init_asset;
    work_queue_entry_init_asset *Job = SafeAccess(work_queue_entry_init_asset, &Entry);

    Job->Chunk = Chunk;
    /* Job->File = File; */
  }
  else
  {
    Entry.Type = type_work_queue_entry_init_world_chunk;
    work_queue_entry_init_world_chunk *Job = SafeAccess(work_queue_entry_init_world_chunk, &Entry);
    Job->Chunk = Chunk;
  }

  Chunk->Data->Flags = Chunk_Queued;
  PushWorkQueueEntry(Queue, &Entry);

  return;
}

link_internal void
DoCopyJob(work_queue_entry_copy_buffer *Job)
{
  untextured_3d_geometry_buffer* Src = Job->Src;
  untextured_3d_geometry_buffer* Dest = &Job->Dest;
  Assert(Src->At <= Dest->End);

  v3 Basis = Job->Basis;
  BufferVertsChecked(Src, Dest, Basis, V3(1.0f));
}


