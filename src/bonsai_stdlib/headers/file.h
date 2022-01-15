struct random_series;
struct memory_arena;
struct ansi_stream;

static const char* DefaultPermissions = "a+b"; // Open file for read/write, read from start if exists


bonsai_function b32 CloseFile(native_file* File);

bonsai_function b32 Rename(native_file CurrentFile, counted_string NewFilePath);

bonsai_function b32 Remove(counted_string Filepath);

bonsai_function native_file OpenFile(const char* FilePath, const char* Permissions = DefaultPermissions);

bonsai_function native_file OpenFile(counted_string FilePath, const char* Permissions = DefaultPermissions);

bonsai_function counted_string GetRandomString(u32 Length, random_series* Entropy, memory_arena* Memory);

bonsai_function counted_string GetRandomString(u32 Length, umm EntropySeed, memory_arena* Memory);

bonsai_function counted_string GetTmpFilename(random_series* Entropy, memory_arena* Memory);

bonsai_function native_file GetTempFile(random_series* Entropy, memory_arena* Memory);

bonsai_function inline b32 WriteToFile(native_file* File, counted_string Str);

bonsai_function inline b32 WriteToFile(native_file* File, ansi_stream* Str);

bonsai_function b32 FileExists(const char* Path);

bonsai_function b32 FileExists(counted_string Path);

bonsai_function void ReadBytesIntoBuffer(FILE *Src, u64 BytesToRead, u8* Dest);
