// TODO(Jesse): Should we have some sort of more systemic 'serialization_context' .. or something ..?

link_internal u8_cursor_block_array
BeginSerialization()
{
  Assert(Global_SerializeTypeTableArena == 0);

  Global_SerializeTypeTableArena = AllocateArena();
  Global_SerializeTypeTable = Allocate_bonsai_type_info_hashtable(64, Global_SerializeTypeTableArena);

  u8_cursor_block_array Blocks = U8CursorBlockArray(Global_SerializeTypeTableArena);
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
  u8_cursor_block_array TypeBufferOutputStream = U8CursorBlockArray(Global_SerializeTypeTableArena);
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





link_internal u8_stream
BeginDeserialization(cs Filename, memory_arena *TempMemory)
{
  u8_stream LevelBytes = U8_StreamFromFile(Filename, TempMemory);
  if (LevelBytes.Start)
  {
    u64 LevelHeaderVersion = {};
    Deserialize(&LevelBytes, &LevelHeaderVersion, 0);

    Global_SerializeTypeTableArena = AllocateArena();
    switch(LevelHeaderVersion)
    {
      // TODO(Jesse): Audit .level savefiles and remove this.. it's hella legacy
      case 0:
      {
        Assert(Global_SerializeTypeTable.Size == 0);
        Assert(Global_SerializeTypeTable.Elements == 0);
        Global_SerializeTypeTable = Allocate_bonsai_type_info_hashtable(64, Global_SerializeTypeTableArena);

        {
          entity *Dummy = 0;
          auto TI = TypeInfo(Dummy);
          TI.Version = 1;
          Insert(TI, &Global_SerializeTypeTable, Global_SerializeTypeTableArena);
        }

        {
          camera *Dummy = 0;
          auto TI = TypeInfo(Dummy);
          TI.Version = 0;
          Insert(TI, &Global_SerializeTypeTable, Global_SerializeTypeTableArena);
        }

        {
          level_header *Dummy = 0;
          auto TI = TypeInfo(Dummy);
          TI.Version = 0;
          Insert(TI, &Global_SerializeTypeTable, Global_SerializeTypeTableArena);
        }
      } break;

      case 1:
      {
        bonsai_type_info_buffer TypeInfoBuffer = {};
        Deserialize(&LevelBytes, &TypeInfoBuffer, Global_SerializeTypeTableArena);

        Assert(Global_SerializeTypeTable.Size == 0);
        Assert(Global_SerializeTypeTable.Elements == 0);
        Global_SerializeTypeTable = Allocate_bonsai_type_info_hashtable(NextPowerOfTwo(TypeInfoBuffer.Count), Global_SerializeTypeTableArena);

        IterateOver(&TypeInfoBuffer, TypeInfo, TypeInfoIndex)
        {
          Insert(*TypeInfo, &Global_SerializeTypeTable, Global_SerializeTypeTableArena);
        }
      } break;

      default: { SoftError("Could not load level file claiming version (%lu), bailing.", LevelHeaderVersion); LevelBytes = {}; }
    }
  }
  return LevelBytes;
}

link_internal void
FinalizeDeserialization(u8_stream *Bytes)
{
  *Bytes = {};

  Global_SerializeTypeTable = {};
  VaporizeArena(Global_SerializeTypeTableArena);
  Global_SerializeTypeTableArena = 0;
}
