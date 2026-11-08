// callsite
// external/bonsai_stdlib/src/primitive_containers.h:95:0

// def (change_records_for_float_types)
// external/bonsai_stdlib/src/primitive_containers.h:66:0
link_internal void
MaybePushChangeRecord( base_ptr_relative_edit_block_array *ChangeRecords, r64 StartingValue, r64 *ValuePtr, ui_id Id)
{
  r64 Tmp = Cast(r64, StartingValue);
  MaybePushChangeRecord(ChangeRecords, PrimitiveType_r64, ReinterpretCast(u64, Tmp), Cast(u64, ValuePtr), Id);
}

link_internal void
MaybePushChangeRecord( base_ptr_relative_edit_block_array *ChangeRecords, r64 *ValuePtr, ui_id Id)
{
  MaybePushChangeRecord(ChangeRecords, *ValuePtr, ValuePtr, Id);
}
link_internal void
MaybePushChangeRecord( base_ptr_relative_edit_block_array *ChangeRecords, r32 StartingValue, r32 *ValuePtr, ui_id Id)
{
  r64 Tmp = Cast(r64, StartingValue);
  MaybePushChangeRecord(ChangeRecords, PrimitiveType_r32, ReinterpretCast(u64, Tmp), Cast(u64, ValuePtr), Id);
}

link_internal void
MaybePushChangeRecord( base_ptr_relative_edit_block_array *ChangeRecords, r32 *ValuePtr, ui_id Id)
{
  MaybePushChangeRecord(ChangeRecords, *ValuePtr, ValuePtr, Id);
}


