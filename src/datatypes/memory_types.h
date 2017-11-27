struct memory_arena
{
  u8* FirstFreeByte;
  umm Remaining;
  umm TotalSize;
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
Allocate(umm Bytes)
{
  u8* Result = (u8*)calloc(1, (size_t)Bytes);
  if (!Result)
  {
    Assert(False);
    Error("Unable to allocate memory!");
  }
  return Result;
}

void*
PushSize(memory_arena *Arena, umm Size)
{
  void* Result = 0;

  if (Size <= Arena->Remaining)
  {
    Result = (void*)Arena->FirstFreeByte;
    Arena->FirstFreeByte += Size;
    Arena->Remaining -= Size;
  }

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

inline void
SubArena( memory_arena *Src, memory_arena *Dest, umm Size)
{
  Dest->FirstFreeByte = (u8*)PushSize(Src, Size);
  Dest->Remaining = Size;
  Dest->TotalSize = Size;

  Assert(Dest->FirstFreeByte);

  return;
}

memory_arena *
SubArena( memory_arena *Src, umm Size)
{
  memory_arena *Result = PUSH_STRUCT_CHECKED(memory_arena, Src, 1);
  SubArena(Src, Result, Size);
  return Result;
}

inline void
AllocateAndInitializeArena(memory_arena *Arena, umm Size)
{
  Arena->Remaining = Size;
  Arena->TotalSize = Size;

  Arena->FirstFreeByte = Allocate(Arena->Remaining);

  Assert(Arena->FirstFreeByte);

  return;
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


