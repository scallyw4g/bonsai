// external/bonsai_stdlib/src/xml.cpp:7:0

link_internal b32
AreEqual(xml_tag *Thing1, xml_tag *Thing2)
{
  b32 Result = MemoryIsEqual((u8*)Thing1, (u8*)Thing2, sizeof( xml_tag ) );

  return Result;
}

link_internal b32
AreEqual(xml_tag Thing1, xml_tag Thing2)
{
  b32 Result = MemoryIsEqual((u8*)&Thing1, (u8*)&Thing2, sizeof( xml_tag ) );

  return Result;
}

