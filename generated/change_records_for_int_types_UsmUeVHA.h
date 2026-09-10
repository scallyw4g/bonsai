// callsite
// external/bonsai_stdlib/src/primitives.h:354:0

// def (change_records_for_int_types)
// external/bonsai_stdlib/src/primitives.h:311:0
link_internal void
MaybePushChangeRecord( primitive_value_changed_record_block_array *ChangeRecords, s64 StartingValue, s64 *ValuePtr )
{
  MaybePushChangeRecord(ChangeRecords, PrimitiveType_s64, Cast(u64, StartingValue), Cast(void*, ValuePtr));
}

link_internal void
MaybePushChangeRecord( primitive_value_changed_record_block_array *ChangeRecords, s64 *ValuePtr )
{
  MaybePushChangeRecord(ChangeRecords, PrimitiveType_s64, *ValuePtr, ValuePtr);
}
link_internal void
MaybePushChangeRecord( primitive_value_changed_record_block_array *ChangeRecords, u64 StartingValue, u64 *ValuePtr )
{
  MaybePushChangeRecord(ChangeRecords, PrimitiveType_u64, Cast(u64, StartingValue), Cast(void*, ValuePtr));
}

link_internal void
MaybePushChangeRecord( primitive_value_changed_record_block_array *ChangeRecords, u64 *ValuePtr )
{
  MaybePushChangeRecord(ChangeRecords, PrimitiveType_u64, *ValuePtr, ValuePtr);
}
link_internal void
MaybePushChangeRecord( primitive_value_changed_record_block_array *ChangeRecords, s32 StartingValue, s32 *ValuePtr )
{
  MaybePushChangeRecord(ChangeRecords, PrimitiveType_s32, Cast(u64, StartingValue), Cast(void*, ValuePtr));
}

link_internal void
MaybePushChangeRecord( primitive_value_changed_record_block_array *ChangeRecords, s32 *ValuePtr )
{
  MaybePushChangeRecord(ChangeRecords, PrimitiveType_s32, *ValuePtr, ValuePtr);
}
link_internal void
MaybePushChangeRecord( primitive_value_changed_record_block_array *ChangeRecords, u32 StartingValue, u32 *ValuePtr )
{
  MaybePushChangeRecord(ChangeRecords, PrimitiveType_u32, Cast(u64, StartingValue), Cast(void*, ValuePtr));
}

link_internal void
MaybePushChangeRecord( primitive_value_changed_record_block_array *ChangeRecords, u32 *ValuePtr )
{
  MaybePushChangeRecord(ChangeRecords, PrimitiveType_u32, *ValuePtr, ValuePtr);
}
link_internal void
MaybePushChangeRecord( primitive_value_changed_record_block_array *ChangeRecords, s16 StartingValue, s16 *ValuePtr )
{
  MaybePushChangeRecord(ChangeRecords, PrimitiveType_s16, Cast(u64, StartingValue), Cast(void*, ValuePtr));
}

link_internal void
MaybePushChangeRecord( primitive_value_changed_record_block_array *ChangeRecords, s16 *ValuePtr )
{
  MaybePushChangeRecord(ChangeRecords, PrimitiveType_s16, *ValuePtr, ValuePtr);
}
link_internal void
MaybePushChangeRecord( primitive_value_changed_record_block_array *ChangeRecords, u16 StartingValue, u16 *ValuePtr )
{
  MaybePushChangeRecord(ChangeRecords, PrimitiveType_u16, Cast(u64, StartingValue), Cast(void*, ValuePtr));
}

link_internal void
MaybePushChangeRecord( primitive_value_changed_record_block_array *ChangeRecords, u16 *ValuePtr )
{
  MaybePushChangeRecord(ChangeRecords, PrimitiveType_u16, *ValuePtr, ValuePtr);
}
link_internal void
MaybePushChangeRecord( primitive_value_changed_record_block_array *ChangeRecords, s8 StartingValue, s8 *ValuePtr )
{
  MaybePushChangeRecord(ChangeRecords, PrimitiveType_s8, Cast(u64, StartingValue), Cast(void*, ValuePtr));
}

link_internal void
MaybePushChangeRecord( primitive_value_changed_record_block_array *ChangeRecords, s8 *ValuePtr )
{
  MaybePushChangeRecord(ChangeRecords, PrimitiveType_s8, *ValuePtr, ValuePtr);
}
link_internal void
MaybePushChangeRecord( primitive_value_changed_record_block_array *ChangeRecords, u8 StartingValue, u8 *ValuePtr )
{
  MaybePushChangeRecord(ChangeRecords, PrimitiveType_u8, Cast(u64, StartingValue), Cast(void*, ValuePtr));
}

link_internal void
MaybePushChangeRecord( primitive_value_changed_record_block_array *ChangeRecords, u8 *ValuePtr )
{
  MaybePushChangeRecord(ChangeRecords, PrimitiveType_u8, *ValuePtr, ValuePtr);
}


