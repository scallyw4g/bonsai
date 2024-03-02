// external/bonsai_stdlib/src/ui/ui.h:109:0

link_internal b32
AreEqual(ui_toggle *Thing1, ui_toggle *Thing2)
{
  b32 Result = MemoryIsEqual((u8*)Thing1, (u8*)Thing2, sizeof( ui_toggle ) );

  return Result;
}

link_internal b32
AreEqual(ui_toggle Thing1, ui_toggle Thing2)
{
  b32 Result = MemoryIsEqual((u8*)&Thing1, (u8*)&Thing2, sizeof( ui_toggle ) );

  return Result;
}

