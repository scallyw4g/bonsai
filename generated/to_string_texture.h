// external/bonsai_stdlib/src/to_string.cpp:59:0

link_internal cs
ToString(texture *Element)
{
  cs Result = FSz("ID(%S) Dim(%S) Slices(%S) Channels(%S) IsDepthTexture(%S) DebugName(%S) Queued(%S) ", ToString(&Element->ID) , ToString(&Element->Dim) , ToString(&Element->Slices) , ToString(&Element->Channels) , ToString(&Element->IsDepthTexture) , ToString(&Element->DebugName) , ToString(&Element->Queued) );
  return Result;
}

