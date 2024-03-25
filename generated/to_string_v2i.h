// external/bonsai_stdlib/src/to_string.cpp:47:0

link_internal cs
ToString(v2i *Element)
{
  cs Result = FSz("E(%S) (anonymous)(%S) ", ToString(Element->E) , ToString(Element->(anonymous)) );
  return Result;
}

