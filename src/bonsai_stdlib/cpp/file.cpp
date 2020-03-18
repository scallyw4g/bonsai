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
    Error("Attempted to close %.*s, which was not open.", (s32)File->Path.Count, File->Path.Start);
  }

  return Result;
}

static const char* DefaultPermissions = "rw+b";

function b32
Rename(native_file CurrentFile, counted_string NewFilePath)
{
  const char* Old = GetNullTerminated(CurrentFile.Path);
  const char* New = GetNullTerminated(NewFilePath);
  b32 Result = (rename(Old, New) == 0) ? True : False;
  return Result;
}

function native_file
OpenFile(const char* FilePath, const char* Permissions = DefaultPermissions)
{
  native_file Result = {
    .Path = CS(FilePath)
  };

  FILE* Handle = fopen(FilePath, Permissions);
  if (Handle)
  {
    Result.Handle = Handle;
  }

  return Result;
}

function native_file
OpenFile(counted_string FilePath, const char* Permissions = DefaultPermissions)
{
  const char* NullTerminatedFilePath = GetNullTerminated(FilePath);
  native_file Result = OpenFile(NullTerminatedFilePath, Permissions);
  return Result;
}

function counted_string
GetRandomString(u32 Length, random_series* Entropy, memory_arena* Memory)
{
  counted_string Filename = {
    .Start = Allocate(char, Memory, Length),
    .Count = Length
  };

  for (u32 CharIndex = 0;
      CharIndex < Length;
      ++CharIndex)
  {
    s8 Try = (s8)RandomBetween(48, Entropy, 122);
    while (!IsAlphaNumeric(Try))
    {
      Try = (s8)RandomBetween(48, Entropy, 122);
    }
    ((char*)Filename.Start)[CharIndex] = Try;
  }

  return Filename;
}

function counted_string
GetRandomString(u32 Length, umm EntropySeed, memory_arena* Memory)
{
  random_series Entropy = { .Seed = EntropySeed };
  counted_string Result = GetRandomString(Length, &Entropy, Memory);
  return Result;
}

function counted_string
GetTmpFilename(random_series* Entropy, memory_arena* Memory)
{
  counted_string Filename = GetRandomString(32, Entropy, Memory);
  Filename = Concat(CS("tmp/"), Filename, Memory);
  return Filename;
}

function native_file
GetTempFile(random_series* Entropy, memory_arena* Memory)
{
  counted_string Filename = GetTmpFilename(Entropy, Memory);
  native_file Result = OpenFile(Filename, "wb");
  if (!Result.Handle)
    { Error("Opening File %.*s, errno: %d", (u32)Filename.Count, Filename.Start, errno); }
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
    Error("Writing to file %.*s", (s32)File->Path.Count, File->Path.Start);
  }
  return Result;
}

function inline b32
WriteToFile(native_file* File, ansi_stream Str)
{
  b32 Result = WriteToFile(File, CountedString(Str));
  return Result;
}

function b32
FileExists(const char* Path)
{
  b32 Result = False;

  native_file File = OpenFile(Path);
  if (File.Handle)
  {
    Result = True;
    if (!CloseFile(&File))
    {
      Error("Opened %s, but could not close it.", Path);
      Result = False;
    }
  }

  return Result;
}

function b32
FileExists(counted_string Path)
{
  const char* NullTerminatedFilePath = GetNullTerminated(Path);
  b32 Result = FileExists(NullTerminatedFilePath);
  return Result;
}

function void
LogToConsole(counted_string Output)
{
  if (!WriteToFile(&Stdout, Output))
  {
    Error("Writing to Stdout");
  }
}

