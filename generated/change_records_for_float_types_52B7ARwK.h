// callsite
// external/bonsai_stdlib/src/primitives.h:356:0

// def (change_records_for_float_types)
// external/bonsai_stdlib/src/primitives.h:331:0
link_internal void
MaybePushChangeRecord( primitive_value_changed_record_block_array *ChangeRecords, r64 StartingValue, r64 *ValuePtr )
{
  r64 Tmp = Cast(r64, StartingValue);
  MaybePushChangeRecord(ChangeRecords, PrimitiveType_r64, ReinterpretCast(u64, Tmp), Cast(void*, ValuePtr));
}

link_internal void
MaybePushChangeRecord( primitive_value_changed_record_block_array *ChangeRecords, r64 *ValuePtr )
{
  MaybePushChangeRecord(ChangeRecords, PrimitiveType_r64, *ValuePtr, ValuePtr);
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
  MaybePushChangeRecord(ChangeRecords, PrimitiveType_r32, *ValuePtr, ValuePtr);
}


