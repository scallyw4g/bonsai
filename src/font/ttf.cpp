#define BONSAI_NO_MUTEX_TRACKING
#define BONSAI_NO_TIMED_FUNCTIONS
#define BONSAI_NO_PUSH_METADATA
#define BONSAI_NO_DEBUG_MEMORY_ALLOCATOR

#include <bonsai_types.h>
#include <unix_platform.cpp>

struct ttf
{
  u32 head; // Font Header

  u32 cmap; // Character Glyph mapping
  u32 glyf; // Glyph data
  u32 hhea; // Horizontal Header
  u32 htmx; // Horizontal Metrics
  u32 loca; // Index to Location
  u32 maxp; // Maximum Profile
  u32 name; // Naming
  u32 post; // PostScript
};

u32
InitTtf(const char* SourceFile, memory_arena *Arena)
{
  binary_stream TtfSource = BinaryStreamFromFile(SourceFile, Arena);
}

int
main()
{
  return 0;
}
