// callsite
// external/bonsai_stdlib/src/sort.h:108:0

// def (bubble_sort)
// external/bonsai_stdlib/src/sort.h:39:0
link_internal void
BubbleSort(sort_key *Keys, u32 Count)
{
  for (u32 Ignored = 0;
    Ignored < Count;
    ++Ignored)
  {
    b32 Sorted = True;

    for (u32 Inner = 0;
      Inner < (Count-1);
      ++Inner)
    {
      auto *KeyA = Keys+Inner;
      auto *KeyB = Keys+Inner+1;

      if (*KeyA < *KeyB)
      {
        auto Temp = *KeyA;
        *KeyA = *KeyB;
        *KeyB = Temp;
        Sorted = False;
      }
    }

    if (Sorted) break;
  }

  return;
}

link_internal void
BubbleSort_descending(sort_key *Keys, u32 Count)
{
  for (u32 Ignored = 0;
    Ignored < Count;
    ++Ignored)
  {
    b32 Sorted = True;

    for (u32 Inner = 0;
      Inner < (Count-1);
      ++Inner)
    {
      auto *KeyA = Keys+Inner;
      auto *KeyB = Keys+Inner+1;

      if (*KeyA > *KeyB)
      {
        auto Temp = *KeyA;
        *KeyA = *KeyB;
        *KeyB = Temp;
        Sorted = False;
      }
    }

    if (Sorted) break;
  }

  return;
}
link_internal void
BubbleSort(sort_key_f64 *Keys, u32 Count)
{
  for (u32 Ignored = 0;
    Ignored < Count;
    ++Ignored)
  {
    b32 Sorted = True;

    for (u32 Inner = 0;
      Inner < (Count-1);
      ++Inner)
    {
      auto *KeyA = Keys+Inner;
      auto *KeyB = Keys+Inner+1;

      if (*KeyA < *KeyB)
      {
        auto Temp = *KeyA;
        *KeyA = *KeyB;
        *KeyB = Temp;
        Sorted = False;
      }
    }

    if (Sorted) break;
  }

  return;
}

link_internal void
BubbleSort_descending(sort_key_f64 *Keys, u32 Count)
{
  for (u32 Ignored = 0;
    Ignored < Count;
    ++Ignored)
  {
    b32 Sorted = True;

    for (u32 Inner = 0;
      Inner < (Count-1);
      ++Inner)
    {
      auto *KeyA = Keys+Inner;
      auto *KeyB = Keys+Inner+1;

      if (*KeyA > *KeyB)
      {
        auto Temp = *KeyA;
        *KeyA = *KeyB;
        *KeyB = Temp;
        Sorted = False;
      }
    }

    if (Sorted) break;
  }

  return;
}
link_internal void
BubbleSort(sort_key_f32 *Keys, u32 Count)
{
  for (u32 Ignored = 0;
    Ignored < Count;
    ++Ignored)
  {
    b32 Sorted = True;

    for (u32 Inner = 0;
      Inner < (Count-1);
      ++Inner)
    {
      auto *KeyA = Keys+Inner;
      auto *KeyB = Keys+Inner+1;

      if (*KeyA < *KeyB)
      {
        auto Temp = *KeyA;
        *KeyA = *KeyB;
        *KeyB = Temp;
        Sorted = False;
      }
    }

    if (Sorted) break;
  }

  return;
}

link_internal void
BubbleSort_descending(sort_key_f32 *Keys, u32 Count)
{
  for (u32 Ignored = 0;
    Ignored < Count;
    ++Ignored)
  {
    b32 Sorted = True;

    for (u32 Inner = 0;
      Inner < (Count-1);
      ++Inner)
    {
      auto *KeyA = Keys+Inner;
      auto *KeyB = Keys+Inner+1;

      if (*KeyA > *KeyB)
      {
        auto Temp = *KeyA;
        *KeyA = *KeyB;
        *KeyB = Temp;
        Sorted = False;
      }
    }

    if (Sorted) break;
  }

  return;
}
link_internal void
BubbleSort(sort_key_string *Keys, u32 Count)
{
  for (u32 Ignored = 0;
    Ignored < Count;
    ++Ignored)
  {
    b32 Sorted = True;

    for (u32 Inner = 0;
      Inner < (Count-1);
      ++Inner)
    {
      auto *KeyA = Keys+Inner;
      auto *KeyB = Keys+Inner+1;

      if (*KeyA < *KeyB)
      {
        auto Temp = *KeyA;
        *KeyA = *KeyB;
        *KeyB = Temp;
        Sorted = False;
      }
    }

    if (Sorted) break;
  }

  return;
}

link_internal void
BubbleSort_descending(sort_key_string *Keys, u32 Count)
{
  for (u32 Ignored = 0;
    Ignored < Count;
    ++Ignored)
  {
    b32 Sorted = True;

    for (u32 Inner = 0;
      Inner < (Count-1);
      ++Inner)
    {
      auto *KeyA = Keys+Inner;
      auto *KeyB = Keys+Inner+1;

      if (*KeyA > *KeyB)
      {
        auto Temp = *KeyA;
        *KeyA = *KeyB;
        *KeyB = Temp;
        Sorted = False;
      }
    }

    if (Sorted) break;
  }

  return;
}


