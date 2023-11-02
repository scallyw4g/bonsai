
struct level_header
{
  u64 MagicNumber = 0x6969696942042042;
  u32 ChunkCount;
};

CAssert(sizeof(level_header) == 16);

