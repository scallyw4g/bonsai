
free_list_thing *
Unlink_TS(volatile free_list_thing** Freelist)
{
  FullBarrier;
  volatile free_list_thing* FirstFree = *Freelist;
  if (FirstFree)
  {
    volatile free_list_thing* NextMesh = FirstFree->Next;
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

  return Cast(free_list_thing*, FirstFree);
}

link_internal void
Link_TS(volatile free_list_thing** Freelist, free_list_thing* ToLink)
{
  FullBarrier;

  // NOTE(Jesse): We pass in random pointers to random structures, so this could be anything.
  /* Assert(!ToLink->Next); */

  volatile free_list_thing* FirstFree = *Freelist;
  ToLink->Next = FirstFree;
  while (!AtomicCompareExchange((volatile void**)Freelist, (void*)ToLink, (void*)FirstFree))
  {
    FirstFree = *Freelist;
    ToLink->Next = FirstFree;
  }
}

