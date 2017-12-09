#include <sys/mman.h>

#define registered_memory_arena(Arena) \
  memory_arena Arena = {};             \
  DEBUG_REGISTER_ARENA(Arena);

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


struct memory_arena
{
  u8* FirstFreeByte;
  umm Remaining;
  umm TotalSize;
  umm Allocations;
  umm BlockSize = Megabytes(1);

  memory_arena *Next;

#if BONSAI_INTERNAL
  umm Pushes;

#if MEMPROTECT
  b32 MemProtect = true;
#endif

#endif
};

#define PUSH_STRUCT_CHECKED(Type, Arena, Number) \
  (Type*)PushStructChecked_( Arena, sizeof(Type)*Number, #Type, __LINE__, __FILE__ );

u8*
PlatformAllocatePages(umm Bytes);

u8*
PlatformProtectPage(u8* Mem);

u64
PlatformGetPageSize();


u8*
PushSize(memory_arena *Arena, umm SizeIn)
{

#if BONSAI_INTERNAL
  ++Arena->Pushes;
#endif

  umm RequestedSize = SizeIn;

#if MEMPROTECT
  u64 PageSize = PlatformGetPageSize();
  if (Arena->MemProtect)
  {
    u32 Pages = (SizeIn/PageSize) + 1;
    RequestedSize = (Pages*PageSize) + PageSize;
    Assert( RequestedSize % PageSize == 0 );
  }
#endif

  // Reallocate arena if there's not enough space left in it
  b32 ArenaIsFull = RequestedSize > Arena->Remaining;
  if (ArenaIsFull)
  {
    ++Arena->Allocations;

    u64 AllocationSize = Arena->BlockSize * 2;
    Arena->BlockSize = AllocationSize;
    if (RequestedSize > AllocationSize)
      AllocationSize = RequestedSize;

    Arena->FirstFreeByte = PlatformAllocatePages(AllocationSize);
    Assert(Arena->FirstFreeByte);

    Arena->Remaining = AllocationSize;
    Arena->TotalSize = AllocationSize;
  }

  u8* Result = Arena->FirstFreeByte;

#if MEMPROTECT_OVERFLOW
  if (Arena->MemProtect)
  {
    umm End = (umm)Arena->FirstFreeByte + SizeIn;
    umm EndToNextPage = PageSize - (End % PageSize);
    Assert( (End+EndToNextPage) % PageSize == 0);

    Result = Arena->FirstFreeByte + EndToNextPage;
    u8* LastPage = Result + SizeIn;
    Assert( (u64)LastPage % PageSize == 0)

    mprotect(LastPage, PageSize, PROT_NONE);
  }
#endif

#if MEMPROTECT_UNDERFLOW
  if (Arena->MemProtect)
  {
    umm At = (umm)Arena->FirstFreeByte;
    umm NextPageOffset = PageSize - (At % PageSize);
    Assert( (At+NextPageOffset) % PageSize == 0)

    u8* NextPage = Arena->FirstFreeByte + NextPageOffset;
    mprotect(NextPage, PageSize, PROT_NONE);

    Result = NextPage + PageSize;
  }
#endif

  Arena->FirstFreeByte += RequestedSize;
  Arena->Remaining -= RequestedSize;

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

  if (!Result)
  {
    Error("Pushing %s on Line: %d, in file %s", StructType, Line, File);
    Assert(False);
    return False;
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


