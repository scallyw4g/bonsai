#define MEMPROTECT (MEMPROTECT_OVERFLOW || MEMPROTECT_UNDERFLOW)
#define MEMPROTECT_UNDERFLOW (0)
#define MEMPROTECT_OVERFLOW (1)

#if MEMPROTECT_UNDERFLOW && MEMPROTECT_OVERFLOW
#error "Unfortunately, Underflow and Overflow protection at the same time is impossible"
#endif

enum memory_protection_type
{
  MemoryProtection_Protected,
  MemoryProtection_RW,
};

inline umm
Kilobytes(umm Bytes)
{
  umm Result = Bytes * 1024;
  return Result;
}

inline umm
Megabytes(umm Number)
{
  umm Result = Number*Kilobytes(1024);
  return Result;
}

inline u64
Gigabytes(umm Number)
{
  u64 Result = Number*Megabytes(1024);
  return Result;
}

inline u64
Terabytes(umm Number)
{
  u64 Result = Number*Gigabytes(1024);
  return Result;
}

template <typename T> inline void
Fill(T *Struct, u8 ByteValue)
{
  for ( umm Byte = 0;
      Byte < sizeof(T);
      ++Byte)
  {
    *(((u8*)Struct) + Byte) = ByteValue;
  }
}

template <typename T> inline void
Clear(T *Struct)
{
  Fill(Struct, 0);
}

#define INVALID_THREAD_HANDLE (u64_MAX)

struct memory_arena
{
  u8* Start;
  u8* At;
  u8* End;

  umm NextBlockSize;
  memory_arena *Prev;

#if BONSAI_INTERNAL
  umm Pushes;
#endif
};

#define AllocateProtection(Type, Arena, Number, Protection)                                                                 \
  ( GetDebugState && GetDebugState() ?                                                                                      \
      (Type*)GetDebugState()->Debug_Allocate(Arena, sizeof(Type), (umm)Number, #Type, __LINE__, __FILE__, 1, Protection ) : \
      (Type*)PushSize( Arena, sizeof(Type)*(umm)Number, 1, Protection)                                                      \
  )

#define AllocateAlignedProtection(Type, Arena, Number, Alignment, Protection)                                                      \
  ( GetDebugState && GetDebugState() ?                                                                                             \
    (Type*)GetDebugState()->Debug_Allocate( Arena, sizeof(Type), (umm)Number, #Type, __LINE__, __FILE__, Alignment, Protection ) : \
    (Type*)PushSize( Arena, sizeof(Type)*(umm)Number, Alignment, Protection)                                                       \
  )

#define AllocateAligned(Type, Arena, Number, Alignment)                                                                     \
  ( GetDebugState && GetDebugState() ?                                                                                      \
    (Type*)GetDebugState()->Debug_Allocate( Arena, sizeof(Type), (umm)Number, #Type, __LINE__, __FILE__, Alignment, True) : \
    (Type*)PushSize( Arena, sizeof(Type)*(umm)Number, Alignment, True)                                                      \
  )

#define Allocate(Type, Arena, Number)                                                                               \
  ( GetDebugState && GetDebugState() ?                                                                              \
    (Type*)GetDebugState()->Debug_Allocate( Arena, sizeof(Type), (umm)Number, #Type, __LINE__, __FILE__, 1, True) : \
    (Type*)PushSize( Arena, sizeof(Type)*(umm)Number, 1, True)                                                      \
  )

#define DEBUG_REGISTER_ARENA(Arena) do { if (GetDebugState) { GetDebugState()->RegisterArena(#Arena, Arena); } } while (false)

#define DEBUG_REGISTER_THREAD(ThreadIndex) do { if (GetDebugState) { GetDebugState()->RegisterThread(ThreadIndex); } } while (false)


bonsai_function u64 PlatformGetPageSize();
bonsai_function b32 PlatformSetProtection(u8 *Base, u64 Size, memory_protection_type Protection);

bonsai_function u8*  PlatformAllocateSize(umm AllocationSize);
bonsai_function b32 PlatformDeallocate(u8 *Base, umm Size);

b32
OnPageBoundary(memory_arena *Arena, umm PageSize)
{
  b32 Result = (umm)Arena->At % PageSize == 0;
  return Result;
}

inline u64
AlignTo(umm Mem, umm Alignment)
{
  Assert(Alignment);
  umm At = Mem;
  umm ToNextAlignment = Alignment - (At % Alignment);
  Assert( (At+ToNextAlignment) % Alignment == 0);

  if (ToNextAlignment != Alignment) // We're on a page boundary
  {
    At += ToNextAlignment;
    Assert(At % Alignment == 0);
  }

  return At;
}

inline u8*
AlignTo(u8 *Ptr, umm Alignment)
{
  Assert(Alignment);
  umm At = (umm)Ptr;
  umm ToNextAlignment = Alignment - (At % Alignment);
  Assert( (At+ToNextAlignment) % Alignment == 0);

  if (ToNextAlignment != Alignment) // We're on a page boundary
  {
    At += ToNextAlignment;
    Assert(At % Alignment == 0);
  }

  return (u8*)At;
}

inline void
AlignTo(memory_arena *Arena, umm Alignment)
{
  Arena->At = AlignTo(Arena->At, Alignment);
  Assert(Arena->At <= Arena->End);
  return;
}

inline void
SetToPageBoundary(memory_arena *Arena)
{
  umm Align = PlatformGetPageSize();
  AlignTo(Arena, Align);
  return;
}

template <typename T> umm
TotalSize(T *Sizable)
{
  Assert(Sizable->At <= Sizable->End);
  umm Result = (umm)(Sizable->End - Sizable->Start) * sizeof(Sizable->Start[0]);
  return Result;
}

template <typename T> umm
Remaining(T *Sizable, u32 Modifier = 0)
{
  umm Result = 0;
  if (Sizable->At+Modifier < Sizable->End)
  {
    Result = (umm)(Sizable->End - Sizable->At+Modifier);
  }
  return Result;
}

template <typename T> umm
TotalElements(T *Sizable)
{
  Assert(Sizable->At >= Sizable->Start);
  umm Result = (umm)(Sizable->End-Sizable->Start);
  return Result;
}

template <typename T> umm
AtElements(T *Sizable)
{
  Assert(Sizable->At >= Sizable->Start);
  umm Result = (umm)(Sizable->At-Sizable->Start);
  return Result;
}

bonsai_function b32
MemoryIsEqual(u8 *First, u8 *Second, umm Size)
{
  b32 Result = True;
  for (umm Index = 0;
      Index < Size;
      ++Index)
  {
    Result = Result && ( First[Index] == Second[Index]);
  }

  return Result;
}

struct push_metadata
{
  const char* Name;
  umm ArenaHash;
  umm HeadArenaHash;
  umm StructSize;
  umm StructCount;

  u32 PushCount;
};

inline void
MemSet(u8 *Src, u8 Value, umm Size)
{
  // TODO(Jesse, id: 96, tags: speed): Vectorize for speed boost!
  for( umm ByteIndex = 0;
       ByteIndex < Size;
       ++ByteIndex )
  {
     Src[ByteIndex] = Value;
  }
}

inline void
MemCopy(u8 *Src, u8 *Dest, umm Size)
{
  // TODO(Jesse, id: 97, tags: speed): Vectorize for speed boost!
  for( umm BytesCopied = 0;
       BytesCopied < Size;
       ++BytesCopied )
  {
     Dest[BytesCopied] = Src[BytesCopied];
  }
}

inline umm
HashArenaHead(memory_arena *Arena)
{
  umm Result = (umm)Arena;
  return Result;
}

inline umm
HashArena(memory_arena *Arena)
{
  umm Result = (umm)Arena->Start;
  return Result;
}

inline s32
SafeTruncateToS32(umm Size)
{
  Assert(Size <= s32_MAX);
  s32 Result = (s32)Size;
  return Result;
}

inline u32
SafeTruncateToU32(umm Size)
{
  Assert(Size <= u32_MAX);
  u32 Result = (u32)Size;
  return Result;
}

inline u16
SafeTruncateToU16(umm Size)
{
  Assert(Size <= u16_MAX);
  u16 Result = (u16)Size;
  return Result;
}

bonsai_function u8
SafeTruncateU8(s32 Size)
{
  Assert(Size < u8_MAX);
  u8 Result = (u8)Size;
  return Result;
}

bonsai_function b32
ProtectPage(u8* Mem)
{
  u64 PageSize = PlatformGetPageSize();
  Assert((umm)Mem % PageSize == 0);

  b32 Result = PlatformSetProtection(Mem, PageSize, MemoryProtection_Protected);
  return Result;
}

// @temp-string-builder-memory
// TODO(Jesse, id: 98, tags: robustness, api_improvement): Make allocating these on the stack work!
bonsai_function memory_arena*
AllocateArena(umm RequestedBytes = Megabytes(1), b32 MemProtect = True)
{
  // FIXME(Jesse): We shouldn't really be able to ask for < 1MB worth of space
  u64 PageSize = PlatformGetPageSize();
  u64 ToNextPage = PageSize - (RequestedBytes % PageSize);
  umm AllocationSize = RequestedBytes + ToNextPage;

  Assert(AllocationSize % PageSize == 0);

#if MEMPROTECT_OVERFLOW
  Assert(sizeof(memory_arena) < PageSize);
  u8 *ArenaBytes = PlatformAllocateSize(PageSize*2);
  ArenaBytes += (PageSize - sizeof(memory_arena));

#elif MEMPROTECT_UNDERFLOW
  NotImplemented;
#else

  u8 *ArenaBytes = PlatformAllocateSize(PageSize);
#endif

  memory_arena *Result = (memory_arena*)ArenaBytes;

  u8 *Bytes = PlatformAllocateSize(AllocationSize);
  Result->Start = Bytes;
  Result->At = Bytes;

  Result->End = Bytes + AllocationSize;
  Result->NextBlockSize = AllocationSize * 2;

#if MEMPROTECT_OVERFLOW
  if (MemProtect)
  {
    Assert(OnPageBoundary(Result, PageSize));
    ProtectPage(ArenaBytes + sizeof(memory_arena));
  }

  Assert((umm)Result->Start % PageSize == 0);
  Assert(Remaining(Result) >= RequestedBytes);
#else
  NotImplemented
#endif

  return Result;
}

bonsai_function b32
DeallocateArena(memory_arena *Arena)
{
  b32 Result = False;
  if (Arena->Start)
  {
    Result = PlatformDeallocate(Arena->Start, TotalSize(Arena));
  }

#if MEMPROTECT_OVERFLOW
  {
    umm PageSize = PlatformGetPageSize();
    u8 *ArenaBytes =  (u8*)Arena - ((umm)Arena % PageSize);
    Result &= PlatformDeallocate(ArenaBytes, PageSize*2);
  }
#else
  NotImplemented;
#endif

  return Result;
}

bonsai_function void
ReallocateArena(memory_arena *Arena, umm MinSize, b32 MemProtect)
{
  u64 AllocationSize = Arena->NextBlockSize;
  if (MinSize > AllocationSize)
    AllocationSize = MinSize;

  memory_arena *NewArena = AllocateArena(AllocationSize, MemProtect);

  memory_arena OldArena = *Arena;
  *Arena = *NewArena;
  *NewArena = OldArena;

  Arena->Prev = NewArena;

  Assert( (umm)(Arena->End - Arena->At) >= MinSize);
  Assert(Arena->At <= Arena->End);

  return;
}

inline void
VaporizeArena(memory_arena *Arena)
{
  if(Arena->Prev)
  {
    VaporizeArena(Arena->Prev);
    Arena->Prev = 0;
  }

  DeallocateArena(Arena);
}

bonsai_function void
UnprotectArena(memory_arena *Arena)
{
  /* TIMED_FUNCTION(); */
  umm Size = (umm)Arena->End - (umm)Arena->Start;
  b32 Err = PlatformSetProtection(Arena->Start, Size, MemoryProtection_RW);
  if (Err)
  {
    Error("Unprotecting arena failed");
    Assert(False);
  }

  return;
}

bonsai_function u8*
Reallocate(u8* Allocation, memory_arena* Arena, umm CurrentSize, umm RequestedSize)
{
  u8* Result = 0;
  if (Allocation + CurrentSize == Arena->At)
  {
    if (RequestedSize > CurrentSize)
    {
      umm Diff = RequestedSize - CurrentSize;
      if (Remaining(Arena) >= Diff)
      {
        Arena->At += Diff;
        Result = Allocation;
      }
      else
      {
        Error("Unable to reallocate : Arena didn't have enough space left to accommodate %lu bytes.", Diff);
      }
    }
    else
    {
      Error("Unable to reallocate : RequestedSize < CurrentSize");
    }
  }
  else
  {
    Error("Unable to reallocate : Allocation is not the final one on the arena, or the allocation was memprotected.");
  }

  return Result;
}

bonsai_function u8*
PushSize(memory_arena *Arena, umm SizeIn, umm Alignment, b32 MemProtect)
{
  umm ToAlignment = Alignment - (SizeIn % Alignment);
  umm AlignCorrectedSizeIn = SizeIn;

  if (ToAlignment != Alignment)
  {
    AlignCorrectedSizeIn += ToAlignment;
  }

  umm RequestedSize = AlignCorrectedSizeIn;
  Assert(Arena->At <= Arena->End);              // Sanity checks
  Assert(Remaining(Arena) <= TotalSize(Arena));

#if MEMPROTECT
  u64 PageSize = PlatformGetPageSize();
  if (MemProtect)
  {
    u32 Pages = (u32)((AlignCorrectedSizeIn/PageSize) + 1);
    RequestedSize = (Pages*PageSize) + PageSize;
    SetToPageBoundary(Arena);
    Assert( RequestedSize % PageSize == 0 );
    Assert( (umm)Arena->At % PageSize == 0);
  }
#endif
  Assert(Arena->At <= Arena->End);

  umm RemainingInArena = Remaining(Arena);
  b32 ArenaIsFull = RequestedSize > RemainingInArena;
  if (ArenaIsFull)
  {
    ReallocateArena(Arena, RequestedSize, MemProtect);
  }

  umm AtToAlignment = Alignment - ((umm)Arena->At % Alignment);
  if (AtToAlignment != Alignment)
  {
    Arena->At += AtToAlignment;
  }

  u8* Result = Arena->At;
  Assert(((umm)Arena->At % Alignment) == 0);

#if MEMPROTECT_OVERFLOW
  if (MemProtect)
  {
    umm EndOfStruct = (umm)Arena->At + AlignCorrectedSizeIn;
    umm EndToNextPage = PageSize - (EndOfStruct % PageSize);
    Assert( (EndOfStruct+EndToNextPage) % PageSize == 0);

    Result = Arena->At + EndToNextPage;
    u8* LastPage = Result + AlignCorrectedSizeIn;
    Assert( (u64)LastPage % PageSize == 0);

    ProtectPage(LastPage);
  }
#endif

#if MEMPROTECT_UNDERFLOW
  if (Arena->MemProtect)
  {
    umm At = (umm)Arena->At;
    umm NextPageOffset = PageSize - (At % PageSize);
    Assert( (At+NextPageOffset) % PageSize == 0);

    u8* NextPage = Arena->At + NextPageOffset;
    Assert( (umm)NextPage % PageSize == 0);
    ProtectPage(NextPage);

    Result = NextPage + PageSize;
  }
#endif

  Arena->At += RequestedSize;

#if MEMPROTECT

#if BONSAI_INTERNAL
  ++Arena->Pushes;
#endif

  if (MemProtect)
  {
    Assert( ((umm)Result+AlignCorrectedSizeIn) % PageSize == 0);
    Assert( (umm)Arena->At % PageSize == 0);
  }
#endif


  Assert(Arena->At <= Arena->End);
  Assert((umm)Result % Alignment == 0);

  Assert(Remaining(Arena) <= TotalSize(Arena)); // Sanity Check
  return Result;
}

bonsai_function void*
PushStruct(memory_arena *Memory, umm sizeofStruct, umm Alignment = 1, b32 MemProtect = True)
{
  void* Result = PushSize(Memory, sizeofStruct, Alignment, MemProtect);
  return Result;
}


