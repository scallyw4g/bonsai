
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

