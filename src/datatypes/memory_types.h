#include <sys/mman.h>


inline u32
Kilobytes(u32 Bytes)
{
  u32 Result = Bytes * 1024;
  return Result;
}

inline u32
Megabytes(u32 Number)
{
  u32 Result = Number*Kilobytes(1024);
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
  u8* Start;
  u8* At;
  u8* End;
  umm NextBlockSize;

  memory_arena *Prev;

#if BONSAI_INTERNAL
  umm Pushes;

#if MEMPROTECT
  b32 MemProtect = true;
#endif

#endif
  u8 Pad[4];
};

#if BONSAI_INTERNAL

void*
PushStructChecked_(memory_arena *Arena, umm StructCount, umm StructSize, const char* Name, s32 Line, const char* File);

#define PUSH_STRUCT_CHECKED(Type, Arena, Number) \
  (Type*)PushStructChecked_( Arena, sizeof(Type), (umm)Number, #Type, __LINE__, __FILE__ );

#else

#define PUSH_STRUCT_CHECKED(Type, Arena, Number) \
  (Type*)PushStruct( Arena, sizeof(Type)*Number );

#endif

#if BONSAI_INTERNAL
#define DEBUG_REGISTER_ARENA(Arena, DebugState) \
  DebugRegisterArena(#Arena, Arena, DebugState)
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
  Dest->At = (u8*)PushSize(Src, Size);
  Dest->Remaining = Size;
  Dest->TotalSize = Size;

  Assert(Dest->At);

  return;
}

inline void
AllocateAndInitializeArena_(memory_arena *Arena, umm Size)
{
  Arena->Remaining = Size;
  Arena->TotalSize = Size;

  Arena->At = Allocate(Arena->Remaining);
  Assert(Arena->At);
  return;
}
#endif


memory_arena*
PlatformAllocateArena(umm Bytes);

u8*
PlatformProtectPage(u8* Mem);

u64
PlatformGetPageSize();


void
ReallocateArena(memory_arena *Arena, umm MinSize)
{
  u64 AllocationSize = Arena->NextBlockSize;
  if (MinSize > AllocationSize)
    AllocationSize = MinSize;

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

  Assert( (umm)(Arena->End - Arena->At) >= MinSize);
  return;
}

b32
OnPageBoundary(memory_arena *Arena, umm PageSize)
{
  b32 Result = (umm)Arena->At % PageSize == 0;
  return Result;
}

inline void
SetToPageBoundary(memory_arena *Arena)
{
  u64 PageSize = PlatformGetPageSize();
  umm At = (umm)Arena->At;
  umm ToNextPage = PageSize - (At % PageSize);
  Assert( (At+ToNextPage) % PageSize == 0);

  if (ToNextPage != PageSize) // We're on a page boundary
  {
    Arena->At += ToNextPage;
    Assert(Arena->At <= Arena->End);
    Assert((umm)Arena->At % PageSize == 0);
  }

  return;
}

void
AdvanceToBytesBeforeNextPage(umm Bytes, memory_arena *Arena)
{
  umm PageSize = PlatformGetPageSize();

  umm At = (umm)Arena->At;
  umm AtInPage = (At % PageSize);
  umm AtToPageBoundary = PageSize - AtInPage;

  if (AtToPageBoundary < Bytes)
  {
    SetToPageBoundary(Arena);
    AdvanceToBytesBeforeNextPage(Bytes, Arena);
  }
  else
  {
    umm EndOfBytes = (umm)Arena->At + Bytes;
    umm EndToNextPage = PageSize - (EndOfBytes % PageSize);
    Arena->At += EndToNextPage;

    Assert( (EndOfBytes+EndToNextPage) % PageSize == 0);
    Assert( ((umm)Arena->At + Bytes) % PageSize == 0);
  }

#if 0
  Assert(Bytes < PageSize); // TODO(Jesse): Prove this is unnecessary

  umm EndOfBytes = (umm)Arena->At + Bytes;
  umm StartingPage = (umm)Arena->At - ((umm)Arena->At % PageSize);
  umm StartingPageToEndOfBytes = EndOfBytes - StartingPage;

  umm EndToNextPage = PageSize - StartingPageToEndOfBytes;
  Assert( (EndOfBytes+EndToNextPage) % PageSize == 0);

  Arena->At += EndToNextPage;
  Assert(Arena->At <= Arena->End);
#endif

  return;
}

umm
TotalSize(memory_arena *Arena)
{
  Assert(Arena->At <= Arena->End);
  umm Result = (umm)(Arena->End - Arena->Start);
  return Result;
}

umm
Remaining(memory_arena *Arena)
{
  Assert(Arena->At <= Arena->End);
  umm Result = (umm)(Arena->End - Arena->At);
  return Result;
}

u8*
PushSize(memory_arena *Arena, umm SizeIn)
{
  umm RequestedSize = SizeIn;
  Assert(Arena->At <= Arena->End);
  Assert(Remaining(Arena) <= TotalSize(Arena));

#if MEMPROTECT
  u64 PageSize = PlatformGetPageSize();
  if (Arena->MemProtect)
  {
    u32 Pages = (u32)((SizeIn/PageSize) + 1);
    RequestedSize = (Pages*PageSize) + PageSize;
    SetToPageBoundary(Arena);
    Assert( RequestedSize % PageSize == 0 );
    Assert( (umm)Arena->At % PageSize == 0);
  }
#endif
  Assert(Arena->At <= Arena->End);

  umm RemainingInArena = Remaining(Arena);
  b32 ArenaIsFull = RequestedSize > RemainingInArena;
  if (ArenaIsFull) // Reallocate the arena
  {
    ReallocateArena(Arena, RequestedSize);
  }

  u8* Result = Arena->At;
  Assert(Arena->At <= Arena->End);

#if MEMPROTECT_OVERFLOW
  if (Arena->MemProtect)
  {
    umm EndOfStruct = (umm)Arena->At + SizeIn;
    umm EndToNextPage = PageSize - (EndOfStruct % PageSize);
    Assert( (EndOfStruct+EndToNextPage) % PageSize == 0);

    Result = Arena->At + EndToNextPage;
    u8* LastPage = Result + SizeIn;
    Assert( (u64)LastPage % PageSize == 0)

    mprotect(LastPage, PageSize, PROT_NONE);
  }
#endif
  Assert(Arena->At <= Arena->End);

#if MEMPROTECT_UNDERFLOW
  if (Arena->MemProtect)
  {
    umm At = (umm)Arena->At;
    umm NextPageOffset = PageSize - (At % PageSize);
    Assert( (At+NextPageOffset) % PageSize == 0)

    u8* NextPage = Arena->At + NextPageOffset;
    mprotect(NextPage, PageSize, PROT_NONE);

    Result = NextPage + PageSize;
  }
#endif

  Assert(Arena->At <= Arena->End);
  Arena->At += RequestedSize;
  Assert(Arena->At <= Arena->End);

#if MEMPROTECT
  ++Arena->Pushes;

  if (Arena->MemProtect)
  {
    Assert( ((umm)Result+SizeIn) % PageSize == 0);
    Assert( (umm)Arena->At % PageSize == 0);
  }
#endif

  Assert(Arena->At <= Arena->End);

  return Result;
}

void*
PushStruct(memory_arena *Memory, umm sizeofStruct)
{
  void* Result = PushSize(Memory, sizeofStruct);
  return Result;
}

struct push_metadata
{
  const char* Name;
  memory_arena *Arena;
  umm StructSize;
  umm StructCount;

  u32 PushCount;
};

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

