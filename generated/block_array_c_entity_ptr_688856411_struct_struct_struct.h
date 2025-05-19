// src/engine/world_chunk.cpp:1:0





link_internal entity_ptr_block *
Allocate_entity_ptr_block(memory_arena *Memory)
{
  entity_ptr_block *Result = Allocate( entity_ptr_block, Memory, 1);
  Result->Elements = Allocate( entity_ptr, Memory, 8);
  return Result;
}

link_internal cs
CS( entity_ptr_block_array_index Index )
{
  return FSz("(%u)(%u)", Index.BlockIndex, Index.ElementIndex);
}

link_internal entity_ptr 
Set( entity_ptr_block_array *Arr,
  entity_ptr Element,
  entity_ptr_block_array_index Index )
{
  entity_ptr Result = {};
  if (Index.Block)
  {
    entity_ptr *Slot = &Index.Block->Elements[Index.ElementIndex];
    *Slot = Element;

    Result = *Slot;
  }

  return Result;
}

link_internal void
RemoveUnordered( entity_ptr_block_array *Array, entity_ptr_block_array_index Index)
{
  entity_ptr_block_array_index LastI = LastIndex(Array);

  entity_ptr Element = GetPtr(Array, Index);
  entity_ptr LastElement = GetPtr(Array, LastI);

  Set(Array, LastElement, Index);

  Assert(Array->Current->At);
  Array->Current->At -= 1;

  if (Array->Current->At == 0)
  {
    // TODO(Jesse): There's obviously a way better way to do this ..
    auto AtE = AtElements(Array);
    s32 Count = s32(GetIndex(&AtE));

    if (Count == 0)
    {
      // Nothing to be done, we've popping the last thing off the array
      Assert(Index.Block == Array->First);
      Assert(Index.Block == Array->Current);
      Assert(Index.BlockIndex == 0);
      Assert(Index.ElementIndex == 0);
    }
    else
    {
      // Walk the chain till we get to the second-last one
      entity_ptr_block *Current = Array->First;
      entity_ptr_block *LastB = LastI.Block;

      while (Current->Next && Current->Next != LastB)
      {
        Current = Current->Next;
      }

      Assert(Current->Next == LastB || Current->Next == 0);
      Array->Current = Current;
    }
  }
}

link_internal void
RemoveOrdered( entity_ptr_block_array *Array, entity_ptr_block_array_index Index)
{
  auto End = AtElements(Array);
  auto   AtI = Index;
  auto NextI = Index;
  ++NextI;

  while (NextI < End)
  {
    auto At    =  GetPtr(Array, AtI);
    auto NextV = *GetPtr(Array, NextI);

    *At = NextV;

    ++AtI;
    ++NextI;
  }

  RemoveUnordered(Array, NextI);
}

link_internal void
RemoveOrdered( entity_ptr_block_array *Array, entity_ptr Element )
{
  IterateOver(Array, E, I)
  {
    if (E == Element)
    {
      RemoveOrdered(Array, I);
      break;
    }
  }
}

link_internal entity_ptr_block_array_index
Find( entity_ptr_block_array *Array, entity_ptr Query)
{
  entity_ptr_block_array_index Result = INVALID_BLOCK_ARRAY_INDEX;
  IterateOver(Array, E, Index)
  {
    if ( E == Query)
    {
      Result = Index;
      break;
    }
  }
  return Result;
}

link_internal b32
IsValid(entity_ptr_block_array_index *Index)
{
  entity_ptr_block_array_index Test = INVALID_BLOCK_ARRAY_INDEX;
  b32 Result = (AreEqual(Index, &Test) == False);
  return Result;
}

link_internal entity_ptr *
Push( entity_ptr_block_array *Array, entity_ptr *Element)
{
  Assert(Array->Memory);

  if (Array->First == 0) { Array->First = Allocate_entity_ptr_block(Array->Memory); Array->Current = Array->First; }

  if (Array->Current->At == 8)
  {
    if (Array->Current->Next)
    {
      Array->Current = Array->Current->Next;
      Assert(Array->Current->At == 0);
    }
    else
    {
      entity_ptr_block *Next = Allocate_entity_ptr_block(Array->Memory);
      Next->Index = Array->Current->Index + 1;

      Array->Current->Next = Next;
      Array->Current = Next;
    }
  }

  entity_ptr *Result = Array->Current->Elements + Array->Current->At;

  Array->Current->Elements[Array->Current->At++] = *Element;

  return Result;
}

link_internal entity_ptr *
Push( entity_ptr_block_array *Array )
{
  entity_ptr Element = {};
  auto Result = Push(Array, &Element);
  return Result;
}

