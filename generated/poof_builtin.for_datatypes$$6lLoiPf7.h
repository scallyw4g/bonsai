// callsite
// src/engine/serdes.cpp:473:0

// def (poof_builtin.for_datatypes)
// src/engine/serdes.cpp:473:0





















































































link_internal void
Marshal( layer_settings_3 *Stored, layer_settings *Live)
{
          Live->Type = Stored->Type;



        Live->Noise = Stored->Noise;



        Live->Shape = Stored->Shape;



        Live->Brush = Stored->Brush;



        Live->Offset = Stored->Offset;



        Live->Rotation = Stored->Rotation;



        Live->Flags = world_edit_op_flags(Live->Flags | world_edit_op_flags(Stored->Invert * WorldEditOpFlag_Invert));



        Live->Flags = world_edit_op_flags(Live->Flags | world_edit_op_flags(Stored->Normalized * WorldEditOpFlag_Normalized));



        



        



        Live->ValueBias = Stored->ValueBias;



        Live->Power = Stored->Power;



        Live->ValueFunc = Stored->ValueFunc;



        Live->BlendMode = Stored->BlendMode;



        Live->Smoothing = Stored->Smoothing;



        Live->BasisOffset = Stored->BasisOffset;



        Live->ColorMode = Stored->ColorMode;



        Live->ColorTextureFilePath = Stored->ColorTextureFilePath;



        Live->HSVColor = Stored->HSVColor;



        Live->Flags = world_edit_op_flags(Live->Flags | world_edit_op_flags(Stored->Disabled * WorldEditOpFlag_Disabled));





}










































































link_internal void
Marshal( layer_settings_2 *Stored, layer_settings *Live)
{
          Live->Type = Stored->Type;



        Live->Noise = Stored->Noise;



        Live->Shape = Stored->Shape;



        Live->Brush = Stored->Brush;



        Live->Offset = Stored->Offset;



        Live->Rotation = Stored->Rotation;



        Live->Flags = world_edit_op_flags(Live->Flags | world_edit_op_flags(Stored->Invert * WorldEditOpFlag_Invert));



        Live->Flags = world_edit_op_flags(Live->Flags | world_edit_op_flags(Stored->Normalized * WorldEditOpFlag_Normalized));



        



        



        Live->ValueBias = Stored->ValueBias;



        Live->Power = Stored->Power;



        Live->ValueFunc = Stored->ValueFunc;



        Live->BlendMode = Stored->BlendMode;



        Live->Smoothing = Stored->Smoothing;



        Live->BasisOffset = Stored->BasisOffset;



        Live->ColorMode = Stored->ColorMode;



        Live->HSVColor = Stored->HSVColor;



        Live->Flags = world_edit_op_flags(Live->Flags | world_edit_op_flags(Stored->Disabled * WorldEditOpFlag_Disabled));





}

























































































































































































































































link_internal void
Marshal( world_edit_0 *Stored, world_edit *Live)
{
          Live->Region = Stored->Region;



        Live->Brush = Stored->Brush;



        Live->Rotation = Stored->Rotation;



        Live->Flags = Stored->Flags;



        SetSelected(Live, Stored->Dirty);



        SetSelected(Live, Stored->Selected);



        Live->Ordinal = Stored->Ordinal;





}









link_internal void
Marshal( layer_settings_1 *Stored, layer_settings *Live)
{
          Live->Type = Stored->Type;



        Live->Noise = Stored->Noise;



        Live->Shape = Stored->Shape;



        Live->Brush = Stored->Brush;



        Live->Flags = world_edit_op_flags(Live->Flags | world_edit_op_flags(Stored->Invert * WorldEditOpFlag_Invert));



        Live->Flags = world_edit_op_flags(Live->Flags | world_edit_op_flags(Stored->Normalized * WorldEditOpFlag_Normalized));



        



        



        Live->ValueBias = Stored->ValueBias;



        Live->Power = Stored->Power;



        Live->ValueFunc = Stored->ValueFunc;



        Live->BlendMode = Stored->BlendMode;



        Live->Smoothing = Stored->Smoothing;



        Live->ColorMode = Stored->ColorMode;



        Live->BasisOffset = Stored->BasisOffset;



        Live->HSVColor = Stored->HSVColor;



        Live->Flags = world_edit_op_flags(Live->Flags | world_edit_op_flags(Stored->Disabled * WorldEditOpFlag_Disabled));





}









































































link_internal void
Marshal( world_edit_layer_0 *Stored, world_edit_layer *Live)
{
        RangeIterator(Index, (256) + 1)
  {
        Live->NameBuf[Index] = Stored->NameBuf[Index];

  }


        Live->EditIndices = Stored->EditIndices;





}



























































link_internal void
Marshal( layer_settings_4 *Stored, layer_settings *Live)
{
          Live->Type = Stored->Type;



        Live->Noise = Stored->Noise;



        Live->Shape = Stored->Shape;



        Live->Brush = Stored->Brush;



        Live->Offset = Stored->Offset;



        Live->Rotation = Stored->Rotation;



        Live->Flags = Stored->Flags;



        Live->ValueBias = Stored->ValueBias;



        Live->Power = Stored->Power;



        Live->ValueFunc = Stored->ValueFunc;



        Live->BlendMode = Stored->BlendMode;



        Live->Smoothing = Stored->Smoothing;



        Live->BasisOffset = Stored->BasisOffset;



        Live->ColorMode = Stored->ColorMode;



        Live->ColorTextureFilePath = Stored->ColorTextureFilePath;



        Live->TextureScale = Stored->TextureScale;



        Live->HSVColor = Stored->HSVColor;





}













































































































link_internal void
Marshal( layer_settings_0 *Stored, layer_settings *Live)
{
          Live->Type = Stored->Type;



        Live->Noise = Stored->Noise;



        Live->Shape = Stored->Shape;



        Live->Brush = Stored->Brush;



        Live->Flags = world_edit_op_flags(Live->Flags | world_edit_op_flags(Stored->Invert * WorldEditOpFlag_Invert));



        Live->Flags = world_edit_op_flags(Live->Flags | world_edit_op_flags(Stored->Normalized * WorldEditOpFlag_Normalized));



        



        



        Live->ValueBias = Stored->ValueBias;



        Live->ValueFunc = Stored->ValueFunc;



        Live->BlendMode = Stored->BlendMode;



        Live->Smoothing = Stored->Smoothing;



        Live->ColorMode = Stored->ColorMode;



        Live->BasisOffset = Stored->BasisOffset;



        Live->HSVColor = Stored->HSVColor;



        Live->Flags = world_edit_op_flags(Live->Flags | world_edit_op_flags(Stored->Disabled * WorldEditOpFlag_Disabled));





}






















































