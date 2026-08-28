
enum wav_chunk_tag
{
  WavChunkTag_RIFF = 'FFIR',
  WavChunkTag_WAVE = 'EVAW',
  WavChunkTag_fmt  = ' tmf',
  WavChunkTag_data = 'atad',
};

struct wav_sound
{
  u8_stream Data;

  u16 FormatTag;
  u16 ChannelCount;
  u32 SamplesPerSecond;
  u32 BytesPerSecond;
  u16 BlockAlign;
  u16 BitsPerSample;
  u16 FormatExtraByteCount;

  u32 DataByteCount;
  u32 SampleFrameCount;
};

struct wav_riff_header
{
  u32 RiffTag;
  u32 RiffChunkSize;
  u32 WaveTag;
};

struct wav_chunk_header
{
  u32 Tag;
  u32 Size;
};

link_internal b32
LoadWavFromDisk(const char *Filename, wav_sound *Result, memory_arena *Memory)
{
  u8_stream FileBytes = U8_StreamFromFile(Filename, Memory);
  if (FileBytes.Start == 0 || FileBytes.End == 0)
  {
    SoftError("Opening wav file (%s)", Filename);
    return False;
  }

  if ((umm)(FileBytes.End - FileBytes.Start) < sizeof(wav_riff_header))
  {
    SoftError("Invalid wav file (too small): %s", Filename);
    return False;
  }

  wav_riff_header RiffHeader = {};
  CopyMemory(FileBytes.At, &RiffHeader, sizeof(RiffHeader));
  FileBytes.At += sizeof(RiffHeader);

  if (RiffHeader.RiffTag != WavChunkTag_RIFF || RiffHeader.WaveTag != WavChunkTag_WAVE)
  {
    SoftError("Invalid wav RIFF/WAVE header in file: %s", Filename);
    return False;
  }

  b32 FoundFmt = False;
  b32 FoundData = False;

  while (FileBytes.At + sizeof(wav_chunk_header) <= FileBytes.End)
  {
    wav_chunk_header ChunkHeader = {};
    CopyMemory(FileBytes.At, &ChunkHeader, sizeof(ChunkHeader));
    FileBytes.At += sizeof(ChunkHeader);

    if (FileBytes.At + ChunkHeader.Size > FileBytes.End)
    {
      SoftError("Wav chunk out of bounds in file: %s", Filename);
      return False;
    }

    if (ChunkHeader.Tag == WavChunkTag_fmt)
    {
      if (ChunkHeader.Size < 16)
      {
        SoftError("Invalid wav fmt chunk in file: %s", Filename);
        return False;
      }

      u8 *Fmt = FileBytes.At;
      CopyMemory(Fmt + 0,  &Result->FormatTag, sizeof(Result->FormatTag));
      CopyMemory(Fmt + 2,  &Result->ChannelCount, sizeof(Result->ChannelCount));
      CopyMemory(Fmt + 4,  &Result->SamplesPerSecond, sizeof(Result->SamplesPerSecond));
      CopyMemory(Fmt + 8,  &Result->BytesPerSecond, sizeof(Result->BytesPerSecond));
      CopyMemory(Fmt + 12, &Result->BlockAlign, sizeof(Result->BlockAlign));
      CopyMemory(Fmt + 14, &Result->BitsPerSample, sizeof(Result->BitsPerSample));

      if (ChunkHeader.Size >= 18)
      {
        CopyMemory(Fmt + 16, &Result->FormatExtraByteCount, sizeof(Result->FormatExtraByteCount));
      }

      FoundFmt = True;
    }
    else if (ChunkHeader.Tag == WavChunkTag_data)
    {
      Result->Data = {
        .Start = FileBytes.At,
        .At    = FileBytes.At,
        .End   = FileBytes.At + ChunkHeader.Size,
      };

      Result->DataByteCount = ChunkHeader.Size;
      FoundData = True;
    }

    umm AdvanceCount = ChunkHeader.Size;
    if (AdvanceCount & 1) { ++AdvanceCount; }
    FileBytes.At += AdvanceCount;
  }

  if (FoundFmt == False)
  {
    SoftError("Missing fmt chunk in wav file: %s", Filename);
    return False;
  }

  if (FoundData == False)
  {
    SoftError("Missing data chunk in wav file: %s", Filename);
    return False;
  }

  if (Result->BlockAlign)
  {
    Result->SampleFrameCount = Result->DataByteCount / Result->BlockAlign;
  }

  return True;
}

link_internal b32
LoadWavFromDisk(counted_string Filename, wav_sound *Result, memory_arena *Memory)
{
  const char *NullTerminated = GetNullTerminated(Filename, Memory);
  b32 Loaded = LoadWavFromDisk(NullTerminated, Result, Memory);
  return Loaded;
}

link_internal b32
PlaySound(wav_sound *Sound)
{
  if (Sound == 0)
  {
    SoftError("PlaySound called with null wav_sound");
    return False;
  }

  if (Sound->Data.Start == 0 || Sound->DataByteCount == 0)
  {
    SoftError("PlaySound called with empty wav_sound data");
    return False;
  }

#if BONSAI_WIN32
  platform *Plat = &GetStdlib()->Plat;
  b32 Played = PlatformPlaySoundBuffer(Plat,
                                       Sound->Data.Start,
                                       Sound->DataByteCount,
                                       Sound->FormatTag,
                                       Sound->ChannelCount,
                                       Sound->SamplesPerSecond,
                                       Sound->BytesPerSecond,
                                       Sound->BlockAlign,
                                       Sound->BitsPerSample);
  return Played;
#else
  Warn("PlaySound is not implemented on this platform");
  return False;
#endif
}
