
link_internal texture_block*
Allocate_texture_block(memory_arena *Memory)
{
  texture_block *Result = Allocate(texture_block, Memory, 1);
  Result->Elements = Allocate(texture, Memory, 8);
  return Result;
}

link_internal texture *
Push(texture_block_array *Array, texture *Element)
{
  if (Array->Memory == 0) { Array->Memory = AllocateArena(); }

  if (Array->Current == 0) { Array->First = *Allocate_texture_block(Array->Memory); Array->Current = &Array->First; }

  if (Array->Current->At == 8)
  {
    texture_block *Next = Allocate_texture_block(Array->Memory);
    Next->Index = Array->Current->Index + 1;

    Array->Current->Next = Next;
    Array->Current = Next;
    /* Array->At = 0; */
  }

  texture *Result = Array->Current->Elements + Array->Current->At;

  Array->Current->Elements[Array->Current->At++] = *Element;

  return Result;
}

