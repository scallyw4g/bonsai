// callsite
// external/bonsai_stdlib/src/primitive_containers.h:92:0

// def (change_records_for_float_types)
// external/bonsai_stdlib/src/primitive_containers.h:66:0
link_internal void
MaybePushChangeRecord( primitive_value_changed_record_block_array *ChangeRecords, r64 StartingValue, r64 *ValuePtr )
{
  r64 Tmp = Cast(r64, StartingValue);
  MaybePushChangeRecord(ChangeRecords, PrimitiveType_r64, ReinterpretCast(u64, Tmp), Cast(void*, ValuePtr));
}

link_internal void
MaybePushChangeRecord( primitive_value_changed_record_block_array *ChangeRecords, r64 *ValuePtr )
{
  MaybePushChangeRecord(ChangeRecords, *ValuePtr, ValuePtr);
}
link_internal void
MaybePushChangeRecord( primitive_value_changed_record_block_array *ChangeRecords, r32 StartingValue, r32 *ValuePtr )
{
  r64 Tmp = Cast(r64, StartingValue);
  MaybePushChangeRecord(ChangeRecords, PrimitiveType_r32, ReinterpretCast(u64, Tmp), Cast(void*, ValuePtr));
}

link_internal void
MaybePushChangeRecord( primitive_value_changed_record_block_array *ChangeRecords, r32 *ValuePtr )
{
  MaybePushChangeRecord(ChangeRecords, *ValuePtr, ValuePtr);
}


