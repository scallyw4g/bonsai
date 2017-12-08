
#define registered_memory_arena(Arena) \
  memory_arena Arena = {};             \
  DEBUG_REGISTER_ARENA(Arena);


struct memory_arena
{
  u8* FirstFreeByte;
  umm Remaining;
  umm TotalSize;
  umm Allocations;

#if BONSAI_INTERNAL
  umm Pushes;
#endif
};

#define PUSH_STRUCT_CHECKED(Type, Arena, Number) \
  (Type*)PushStructChecked_( Arena, sizeof(Type)*Number, #Type, __LINE__, __FILE__ );

inline u64
Kilobytes(u32 Bytes)
{
  u64 Result = Bytes * 1024;
  return Result;
}

inline u64
Megabytes(u32 Number)
{
  u64 Result = Number*Kilobytes(1024);
  return Result;
}

inline u64
Gigabytes(u32 Number)
{
  u64 Result = Number*Megabytes(1024);
  return Result;
}

inline u64
Terabytes(u32 Number)
{
  u64 Result = Number*Gigabytes(1024);
  return Result;
}

u8*
PlatformAllocateAligned(umm Bytes, u32 Alignment);

u8*
PlatformProtectPage(u8* Mem);

s64
PlatformGetPageSize();

#define ARENA_BLOCK_SIZE (Megabytes(4))

inline u8*
AllocatePages(umm Bytes)
{
  s64 PageSize = PlatformGetPageSize();
  u8* Result = PlatformAllocateAligned(Bytes, PageSize);

  Assert(Result);
  Assert((s64)Result % PageSize == 0);

  return Result;
}

u8*
PushSize(memory_arena *Arena, umm Size)
{
  u8* Result = 0;

#if BONSAI_INTERNAL
  ++Arena->Pushes;
#endif

  b32 ArenaIsFull = Size > Arena->Remaining;
  if (ArenaIsFull)
  {
    ++Arena->Allocations;

    u64 SizeToAllocate = ARENA_BLOCK_SIZE * Arena->Allocations * Arena->Allocations;
    if (Size > SizeToAllocate)
      SizeToAllocate = Size;

    Arena->FirstFreeByte = AllocatePages(SizeToAllocate);
    Assert(Arena->FirstFreeByte);

    Arena->Remaining = SizeToAllocate;
    Arena->TotalSize = SizeToAllocate;
  }

  Result = Arena->FirstFreeByte;
  Arena->FirstFreeByte += Size;
  Arena->Remaining -= Size;

  return Result;
}

void*
PushStruct(memory_arena *Memory, umm sizeofStruct)
{
  void* Result = PushSize(Memory, sizeofStruct);
  return Result;
}

inline void*
PushStructChecked_(memory_arena *Arena, umm Size, const char* StructType, s32 Line, const char* File)
{
  void* Result = PushStruct( Arena, Size );

  if (!(Result)) {
    Error("Pushing %s on Line: %d, in file %s", StructType, Line, File); return False;
  }

  return Result;
}

#if BONSAI_INTERNAL
#define DEBUG_REGISTER_ARENA(Arena) \
  DebugRegisterArena(#Arena, &Arena)
#else
#define DEBUG_REGISTER_ARENA(...)
#endif

#if 0
#define SubArena(Src, Dest, Size) \
  SubArena_(Src, Dest, Size); \
  DEBUG_REGISTER_ARENA(#Dest, Dest)

#define AllocateAndInitializeArena(Arena, Size) \
  AllocateAndInitializeArena_(Arena, Size); \
  DEBUG_REGISTER_ARENA(#Arena, Arena)

inline void
SubArena_( memory_arena *Src, memory_arena *Dest, umm Size)
{
  Dest->FirstFreeByte = (u8*)PushSize(Src, Size);
  Dest->Remaining = Size;
  Dest->TotalSize = Size;

  Assert(Dest->FirstFreeByte);

  return;
}

inline void
AllocateAndInitializeArena_(memory_arena *Arena, umm Size)
{
  Arena->Remaining = Size;
  Arena->TotalSize = Size;

  Arena->FirstFreeByte = Allocate(Arena->Remaining);
  Assert(Arena->FirstFreeByte);
  return;
}
#endif

inline void
Rewind(memory_arena *Memory)
{
  Memory->FirstFreeByte = Memory->FirstFreeByte - (Memory->TotalSize - Memory->Remaining);
  Memory->Remaining = Memory->TotalSize;

  return;
}

inline void
MemCopy(u8 *Src, u8 *Dest, umm Size)
{
  // TODO(Jesse): Vectorize for speed boost!
  for( umm BytesCopied = 0;
       BytesCopied < Size;
       ++BytesCopied )
  {
     Dest[BytesCopied] = Src[BytesCopied];
  }

}

inline void
CopyArena(memory_arena *Src, memory_arena *Dest)
{
  Rewind(Dest);
  Assert(Dest->Remaining >= Src->TotalSize);

  u8 *FirstSrcByte = Src->FirstFreeByte - (Src->TotalSize - Src->Remaining);

  u8 *FirstDestByte = (u8*)PushSize(Dest, Src->TotalSize);

  MemCopy( FirstSrcByte, FirstDestByte, Src->TotalSize);

  return;
}


