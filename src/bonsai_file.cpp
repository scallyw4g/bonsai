struct native_file
{
  FILE* Handle;
  counted_string FilePath;
};

function b32
CloseFile(native_file* File)
{
  b32 Result = False;
  if (File->Handle)
  {
    Result = fclose(File->Handle) == 0 ? True : False;
  }
  else
  {
    Error("Attempted to close %.*s, which was not open.", (s32)File->FilePath.Count, File->FilePath.Start);
  }

  return Result;
}

static const char* DefaultPermissions = "rw+b";

function native_file
OpenFile(const char* FilePath, const char* Permissions = DefaultPermissions)
{
  native_file Result = {
    .FilePath = CS(FilePath)
  };

  FILE* Handle = fopen(FilePath, Permissions);
  if (Handle)
  {
    Result.Handle = Handle;
  }
  else
  {
    Error("Opening File %s", FilePath);
  }

  return Result;
}

function native_file
OpenFile(counted_string FilePath, const char* Permissions = DefaultPermissions)
{
  const char* NullTerminatedFilePath = Allocate(const char, TranArena, FilePath.Count+1);
  MemCopy((u8*)FilePath.Start, (u8*)NullTerminatedFilePath, FilePath.Count);
  native_file Result = OpenFile(NullTerminatedFilePath, Permissions);
  return Result;
}

function inline b32
WriteToFile(native_file* File, counted_string Str)
{
  b32 Result = False;
  umm BytesWriten = fwrite(Str.Start, 1, Str.Count, File->Handle);
  if (BytesWriten == Str.Count)
  {
    Result = True;
  }
  else
  {
    Error("Writing to file %.*s", (s32)File->FilePath.Count, File->FilePath.Start);
  }
  return Result;
}
