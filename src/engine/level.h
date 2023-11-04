#define LEVEL_HEADER_MAGIC_NUMBER (0x6969696942042042)

struct level_header
{
  u64 MagicNumber = LEVEL_HEADER_MAGIC_NUMBER; 
  u32 ChunkCount;
};

CAssert(sizeof(level_header) == 16);

