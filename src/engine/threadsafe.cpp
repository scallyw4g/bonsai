
freelist_entry *
Unlink_TS(volatile freelist_entry **Freelist)
{
  FullBarrier;
  volatile freelist_entry *FirstFree = *Freelist;
  if (FirstFree)
  {
    volatile freelist_entry *NextMesh = FirstFree->Next;
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

  return Cast(freelist_entry*, FirstFree);
}

link_internal void
Link_TS(volatile freelist_entry **Freelist, freelist_entry* ToLink)
{
  FullBarrier;

  // NOTE(Jesse): We pass in random pointers to random structures, so this could be anything.
  /* Assert(!ToLink->Next); */

  volatile freelist_entry *FirstFree = *Freelist;
  ToLink->Next = FirstFree;
  while (!AtomicCompareExchange((volatile void**)Freelist, (void*)ToLink, (void*)FirstFree))
  {
    FirstFree = *Freelist;
    ToLink->Next = FirstFree;
  }
}

