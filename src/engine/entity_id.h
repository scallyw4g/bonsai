struct entity_id
{
  u32 Index;
  u32 Generation;
};

link_internal b32
operator==(entity_id &LHS, entity_id &RHS)
{
  b32 Result = LHS.Index == RHS.Index &&
               LHS.Generation == RHS.Generation;
  return Result;
}


link_internal b32
operator!=(entity_id &LHS, entity_id &RHS)
{
  b32 Result = !(LHS == RHS);
  return Result;
}
