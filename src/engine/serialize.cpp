link_internal u8_cursor_block_array
BeginSerialization()
{
  Global_SerializeTypeTableArena = AllocateArena();
  Global_SerializeTypeTable = Allocate_bonsai_type_info_hashtable(64, Global_SerializeTypeTableArena);

  u8_cursor_block_array Blocks = {};
  {
    Blocks.BlockSize = Megabytes(32);
    u8_cursor First = U8Cursor(Blocks.BlockSize, Global_SerializeTypeTableArena);
    Ensure( Push(&Blocks, &First) );
  }

  return Blocks;
}

link_internal b32
FinalizeSerialization(u8_cursor_block_array *Blocks, const char *Filename)
{
  u8_cursor_block_array TypeBufferOutputStream = {};
  {
    TypeBufferOutputStream.BlockSize = Megabytes(32);

    u8_cursor First = U8Cursor(TypeBufferOutputStream.BlockSize, Global_SerializeTypeTableArena);
    Ensure( Push(&TypeBufferOutputStream, &First) );

    bonsai_type_info_buffer TypeBuffer = ToBuffer(&Global_SerializeTypeTable, Global_SerializeTypeTableArena);
    Serialize(&TypeBufferOutputStream, &TypeBuffer);
  }

  b32 Result = False;

  native_file LevelFile = OpenFile(Filename, FilePermission_Write);
  if (LevelFile.Handle)
  {
    u64 FileFormatVersion = LEVEL_FILE_FORMAT_VERSION_NUMBER;
    Result  = WriteToFile(&LevelFile, FileFormatVersion);
    Result &= WriteToFile(&LevelFile, &TypeBufferOutputStream);
    Result &= WriteToFile(&LevelFile, Blocks);
    Result &= CloseFile(&LevelFile);
  }

  Global_SerializeTypeTable = {};
  VaporizeArena(Global_SerializeTypeTableArena);
  Global_SerializeTypeTableArena = 0;

  return Result;
}
