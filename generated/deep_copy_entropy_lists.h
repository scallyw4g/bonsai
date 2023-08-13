link_internal void
DeepCopy( entropy_lists *Src, entropy_lists *Dest)
{
  Assert(TotalElements(Src) <= TotalElements(Dest));
  IterateOver(Src, Element, ElementIndex)
  {
    DeepCopy(Element, Dest->Start+ElementIndex);
  }
}

