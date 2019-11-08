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

#define INVALID_THREAD_HANDLE U64_MAX

struct memory_arena
{
  u8* Start;

  u8* At;
  u8* End;
  umm NextBlockSize;

  memory_arena *Prev;

#if BONSAI_INTERNAL
  umm Pushes;
  u64 OwnedByThread = INVALID_THREAD_HANDLE;
#endif
};

#ifndef BONSAI_NO_DEBUG_MEMORY_ALLOCATOR

#define AllocateProtection(Type, Arena, Number, Protection) \
  (Type*)GetDebugState()->Debug_Allocate( Arena, sizeof(Type), (umm)Number, #Type, __LINE__, __FILE__, 1, Protection )

#define AllocateAlignedProtection(Type, Arena, Number, Alignment, Protection) \
  (Type*)GetDebugState()->Debug_Allocate( Arena, sizeof(Type), (umm)Number, #Type, __LINE__, __FILE__, Alignment, Protection )

#define AllocateAligned(Type, Arena, Number, Alignment) \
  (Type*)GetDebugState()->Debug_Allocate( Arena, sizeof(Type), (umm)Number, #Type, __LINE__, __FILE__, Alignment, True)

#define Allocate(Type, Arena, Number) \
  (Type*)GetDebugState()->Debug_Allocate( Arena, sizeof(Type), (umm)Number, #Type, __LINE__, __FILE__, 1, True)

#define DEBUG_REGISTER_ARENA(Arena) \
  GetDebugState()->RegisterArena(#Arena, Arena)

#define DEBUG_REGISTER_THREAD(ThreadIndex) \
  GetDebugState()->RegisterThread(ThreadIndex)

#else

#define AllocateProtection(Type, Arena, Number, Protection) \
  (Type*)PushSize( Arena, sizeof(Type)*(umm)Number, 1, False);

#define AllocateAlignedProtection(Type, Arena, Number, Alignment, Protection) \
  (Type*)PushSize( Arena, sizeof(Type)*(umm)Number, Alignment, Protection)

#define AllocateAligned(Type, Arena, Number, Alignment) \
  (Type*)PushSize( Arena, sizeof(Type)*(umm)Number, Alignment, True)

#define Allocate(Type, Arena, Number) \
  (Type*)PushSize( Arena, sizeof(Type)*(umm)Number, 1, False)

#define DEBUG_REGISTER_ARENA(...)

#define DEBUG_REGISTER_THREAD(...)

#endif


void            PlatformUnprotectArena(memory_arena *Arena);
void            PlatformDeallocateArena(memory_arena *Arena);
u8*             PlatformAllocateSize(umm AllocationSize);
memory_arena*   PlatformAllocateArena(umm Bytes = Megabytes(1), b32 MemProtect = True);
u8*             PlatformProtectPage(u8* Mem);
u64             PlatformGetPageSize();


void
ReallocateArena(memory_arena *Arena, umm MinSize, b32 MemProtect)
{
  u64 AllocationSize = Arena->NextBlockSize;
  if (MinSize > AllocationSize)
    AllocationSize = MinSize;

  memory_arena *NewArena =
    PlatformAllocateArena(AllocationSize, MemProtect);

  memory_arena OldArena = *Arena;
  *Arena = *NewArena;
  *NewArena = OldArena;

  Arena->Prev = NewArena;

  Assert( (umm)(Arena->End - Arena->At) >= MinSize);
  Assert(Arena->At <= Arena->End);

  return;
}

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

template <typename T> umm
TotalSize(T *Sizable)
{
  Assert(Sizable->At <= Sizable->End);
  umm Result = (umm)(Sizable->End - Sizable->Start) * sizeof(Sizable->Start[0]);
  return Result;
}

template <typename T> umm
Remaining(T *Sizable)
{
  Assert(Sizable->At <= Sizable->End);
  umm Result = (umm)(Sizable->End - Sizable->At);
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

template <typename T> b32
AreEqual(T First, T Second)
{
  b32 Result = True;
  umm TypeSize = sizeof(T);

  u8* FirstPtr = (u8*)&First;
  u8* SecondPtr = (u8*)&Second;

  for (umm Index = 0;
      Index < TypeSize;
      ++Index)
  {
    Result = Result && ( FirstPtr[Index] == SecondPtr[Index]);
  }

  return Result;
}

u8*
PushSize(memory_arena *Arena, umm SizeIn, umm Alignment, b32 MemProtect)
{
  umm ToAlignment = Alignment - (SizeIn % Alignment);
  umm AlignCorrectedSizeIn = SizeIn;

  if (ToAlignment != Alignment)
  {
    AlignCorrectedSizeIn += ToAlignment;
  }

  umm RequestedSize = AlignCorrectedSizeIn;
  Assert(Arena->At <= Arena->End);
  Assert(Remaining(Arena) <= TotalSize(Arena)); // Sanity Check

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
  if (ArenaIsFull) // Reallocate the arena
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
    Assert( (u64)LastPage % PageSize == 0)

    s32 ProtectSuccess = (mprotect(LastPage, PageSize, PROT_NONE) == 0);

    if (!ProtectSuccess)
    {
      Error("mprotect failed");
      PlatformDebugStacktrace();
      Assert(False);
    }
  }
#endif

#if MEMPROTECT_UNDERFLOW
  if (Arena->MemProtect)
  {
    umm At = (umm)Arena->At;
    umm NextPageOffset = PageSize - (At % PageSize);
    Assert( (At+NextPageOffset) % PageSize == 0)

    u8* NextPage = Arena->At + NextPageOffset;
    Assert( (umm)NextPage % PageSize == 0);
    mprotect(NextPage, PageSize, PROT_NONE);

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

void*
PushStruct(memory_arena *Memory, umm sizeofStruct, umm Alignment = 1, b32 MemProtect = True)
{
  void* Result = PushSize(Memory, sizeofStruct, Alignment, MemProtect);
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
  // TODO(Jesse): Vectorize for speed boost!
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
  // TODO(Jesse): Vectorize for speed boost!
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

inline void
VaporizeArena(memory_arena *Arena)
{
  if(Arena->Prev)
  {
    VaporizeArena(Arena->Prev);
    Arena->Prev = 0;
  }

  PlatformDeallocateArena(Arena);
}

inline u32
SafeTruncateToU32(umm Size)
{
  Assert(Size <= 0xFFFFFFFF);
  u32 Result = (u32)Size;
  return Result;
}

inline u16
SafeTruncateToU16(umm Size)
{
  Assert(Size <= 0xFFFF);
  u16 Result = (u16)Size;
  return Result;
}
