#include <sys/mman.h>

#define registered_memory_arena(Arena) \
  memory_arena *Arena = PlatformAllocateArena(); \
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

template <typename T> inline void
Clear(T *Struct)
{
  for ( u32 Byte = 0;
      Byte < sizeof(T);
      ++Byte)
  {
    *(((u8*)Struct) + Byte) = 0;
  }
}

struct memory_arena
{
  u8* FirstFreeByte;
  umm Remaining;
  umm TotalSize;
  umm NextBlockSize;

  memory_arena *Prev;

#if BONSAI_INTERNAL
  umm Pushes;

#if MEMPROTECT
  b32 MemProtect = true;
#endif

#endif
};

#define PUSH_STRUCT_CHECKED(Type, Arena, Number) \
  (Type*)PushStructChecked_( Arena, sizeof(Type)*Number, #Type, __LINE__, __FILE__ );

#if BONSAI_INTERNAL
#define DEBUG_REGISTER_ARENA(Arena) \
  DebugRegisterArena(#Arena, Arena)
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


memory_arena*
PlatformAllocateArena(umm Bytes);

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
    Assert( (umm)Arena->FirstFreeByte % PageSize == 0);
  }
#endif

  b32 ArenaIsFull = RequestedSize > Arena->Remaining;
  if (ArenaIsFull) // Reallocate the arena
  {
    u64 AllocationSize = Arena->NextBlockSize;
    if (RequestedSize > AllocationSize)
      AllocationSize = RequestedSize;

    memory_arena *Allocated = PlatformAllocateArena(AllocationSize);

#if MEMPROTECT
    Allocated->MemProtect = Arena->MemProtect;
#endif

    // TODO(Jesse): Can these copies be avoided with some more clever pointer-swapping?
    memory_arena PrevArena = *Arena;
    memory_arena NewArena = *Allocated;

    // Swap
    *Allocated = PrevArena;
    *Arena = NewArena;

    // And point back
    Arena->Prev = Allocated;

  }

  u8* Result = Arena->FirstFreeByte;

#if MEMPROTECT_OVERFLOW
  if (Arena->MemProtect)
  {
    umm EndOfStruct = (umm)Arena->FirstFreeByte + SizeIn;
    umm EndToNextPage = PageSize - (EndOfStruct % PageSize);
    Assert( (EndOfStruct+EndToNextPage) % PageSize == 0);

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
  *Arena->FirstFreeByte = 0;

#if MEMPROTECT
  if (Arena->MemProtect)
  {
    Assert( ((umm)Result+SizeIn) % PageSize == 0);
    Assert( (umm)Arena->FirstFreeByte % PageSize == 0);
  }
#endif

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

