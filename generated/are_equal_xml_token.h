// external/bonsai_stdlib/src/xml.cpp:1:0

link_internal b32
AreEqual(xml_token *Thing1, xml_token *Thing2)
{
  b32 Result = MemoryIsEqual((u8*)Thing1, (u8*)Thing2, sizeof( xml_token ) );

  return Result;
}

link_internal b32
AreEqual(xml_token Thing1, xml_token Thing2)
{
  b32 Result = MemoryIsEqual((u8*)&Thing1, (u8*)&Thing2, sizeof( xml_token ) );

  return Result;
}

