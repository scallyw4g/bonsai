
global_variable bonsai_type_info_hashtable
Global_SerializeTypeTable;

global_variable memory_arena*
Global_SerializeTypeTableArena;


link_internal u8_stream
BeginDeserialization(cs Filename, memory_arena *TempMemory);

link_internal void
FinalizeDeserialization(u8_stream *Bytes);


link_internal u8_cursor_block_array
BeginSerialization();

link_internal b32
FinalizeSerialization(u8_cursor_block_array *Blocks, const char *Filename);
