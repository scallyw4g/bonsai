// external/bonsai_stdlib/src/to_string.cpp:59:0

link_internal cs
ToString(v2 *Element)
{
  cs Result = FSz("%S %S ", ToString(&Element->E[0]) , ToString(&Element->E[1]) );
  return Result;

}
link_internal cs
ToString(v2i *Element)
{
  cs Result = FSz("%S %S ", ToString(&Element->E[0]) , ToString(&Element->E[1]) );
  return Result;

}

