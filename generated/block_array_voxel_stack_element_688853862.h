struct voxel_stack_element_block_index
{
  u32 BlockIndex;
  u32 ElementIndex;
};

struct voxel_stack_element_block
{
  u32 At;
  voxel_stack_element *Elements;
  voxel_stack_element_block *Next;
};

link_internal voxel_stack_element_block*
Allocate_voxel_stack_element_block(memory_arena *Memory)
{
  voxel_stack_element_block *Result = Allocate(voxel_stack_element_block, Memory, 1);
  Result->Elements = Allocate(voxel_stack_element, Memory, 32);
  return Result;
}

struct voxel_stack_element_block_array
{
  voxel_stack_element_block First;
  voxel_stack_element_block *Current;

  memory_arena *Memory;
};

link_internal void
Push(voxel_stack_element_block_array *Array, voxel_stack_element *Element)
{
  if (Array->Memory == 0) { Array->Memory = AllocateArena(); }

  if (Array->Current == 0) { Array->First = *Allocate_voxel_stack_element_block(Array->Memory); Array->Current = &Array->First; }

  if (Array->Current->At == 32)
  {
    voxel_stack_element_block *Next = Allocate_voxel_stack_element_block(Array->Memory);
    Array->Current->Next = Next;
    Array->Current = Next;
    /* Array->At = 0; */
  }

  Array->Current->Elements[Array->Current->At++] = *Element;
}

link_internal voxel_stack_element *
GetPtr(voxel_stack_element_block *Block, umm Index)
{
  voxel_stack_element *Result = 0;
  if (Index < Block->At) { Result = Block->Elements + Index; }
  return Result;
}

link_internal u32
AtElements(voxel_stack_element_block *Block)
{
  return Block->At;
}


