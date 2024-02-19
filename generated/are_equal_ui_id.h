link_internal b32
AreEqual(ui_id &Thing1, ui_id &Thing2)
{
  b32 Result = Thing1 == Thing2;
  return Result;
}

link_internal b32
AreEqual(ui_id *Thing1, ui_id *Thing2)
{
  b32 Result = *Thing1 == *Thing2;
  return Result;
}

