
free_mesh *
Unlink_TS(volatile free_mesh** Freelist)
{
  FullBarrier;
  volatile free_mesh* FirstFree = *Freelist;
  if (FirstFree)
  {
    volatile free_mesh* NextMesh = FirstFree->Next;
    while (FirstFree && !AtomicCompareExchange((volatile void**)Freelist, (void*)NextMesh, (void*)FirstFree))
    {
      FirstFree = *Freelist;
      if (FirstFree)
      {
        NextMesh = FirstFree->Next;
      }
    }
  }

  if (FirstFree)
  {
    FirstFree->Next = 0;
  }

  return (free_mesh*)FirstFree;
}


free_mesh *
Link_TS(volatile free_mesh** Freelist, free_mesh* ToLink)
{
  FullBarrier;

  Assert(!ToLink->Next);

  volatile free_mesh* FirstFree = *Freelist;
  ToLink->Next = FirstFree;
  while (!AtomicCompareExchange((volatile void**)Freelist, (void*)ToLink, (void*)FirstFree))
  {
    FirstFree = *Freelist;
    ToLink->Next = FirstFree;
  }

  return (free_mesh*)FirstFree;
}



#if 0

void
Link_TS(mesh_freelist* Freelist, volatile free_mesh* ToPush)
{
  FullBarrier;
  Assert(ToPush);
  Assert(!ToPush->Mesh);
  Assert(!ToPush->Next);

  volatile free_mesh* FirstFree = Freelist->Containers;
  ToPush->Next = FirstFree;
  while (!AtomicCompareExchange((volatile void**)&Freelist->Containers, (void*)ToPush, (void*)FirstFree))
  {
    FirstFree = Freelist->Containers;
    ToPush->Next = FirstFree;
  }

  return;
}

void
Link_TS(mesh_freelist* Freelist, untextured_3d_geometry_buffer* Mesh)
{
  FullBarrier;
  volatile free_mesh* FirstFree = Freelist->FirstFree;

  // TODO(Jesse, robustness, metaprogramming): Need a way of dynamically allocating more of these on demand
  // if this unlink fails!
  // Can generate a free_mesh_stream!
  free_mesh* ToPush = Unlink_TS(&Freelist->Containers);
  Assert(ToPush);
  Assert(!ToPush->Mesh);
  Assert(!ToPush->Next);

  ToPush->Mesh = Mesh;
  ToPush->Next = FirstFree;

  while (!AtomicCompareExchange((volatile void**)&Freelist->FirstFree, (void*)ToPush, (void*)FirstFree))
  {
    FirstFree = Freelist->FirstFree;
    ToPush->Next = FirstFree;
  }

  return;
}

#endif
