// callsite
// external/bonsai_stdlib/src/to_string.cpp:65:0

// def (to_string)
// external/bonsai_stdlib/src/to_string.cpp:36:0
link_internal cs
ToString(texture *Element)
{
  cs Result = FSz("ID(%S) Dim(%S) Slices(%S) Channels(%S) IsDepthTexture(%S) Format(%S) DebugName(%S) Queued(%S) ",     ToString(  & Element->ID)
,    ToString(  & Element->Dim)
,    ToString(  & Element->Slices)
,    ToString(  & Element->Channels)
,    ToString(  & Element->IsDepthTexture)
,    ToString(   Element->Format)
,    ToString(  & Element->DebugName)
,    ToString(  & Element->Queued)
);
  return Result;
}

