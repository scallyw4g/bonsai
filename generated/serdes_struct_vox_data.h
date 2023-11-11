link_internal b32
Serialize(native_file *File, vox_data *Element)
{
  chunk_data *ChunkDataPrevValue = Element->ChunkData;
  if (Element->ChunkData) { Element->ChunkData = Cast(chunk_data*, 0x1); }

  v3 *PalettePrevValue = Element->Palette;
  if (Element->Palette) { Element->Palette = Cast(v3*, 0x1); }


  b32 Result = WriteToFile(File, Cast(u8*, Element), sizeof(vox_data));

  Element->ChunkData = ChunkDataPrevValue;

  Element->Palette = PalettePrevValue;


  if (Element->ChunkData) { Result &= Serialize(File, Element->ChunkData); }

  if (Element->Palette) { Result &= Serialize(File, Element->Palette); }


  return Result;
}

link_internal vox_data *
Deserialize_vox_data(u8_stream *Bytes)
{
  vox_data *Result = Cast(vox_data*, Bytes->At);
  Bytes->At += sizeof(vox_data);
  Assert(Bytes->At <= Bytes->End);

  if (Result->ChunkData)
  {
    Result->ChunkData = Deserialize_chunk_data(Bytes);
  }

  if (Result->Palette)
  {
    Result->Palette = Deserialize_v3(Bytes);
  }

  return Result;
}


