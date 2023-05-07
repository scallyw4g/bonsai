struct voxel_synth_tile_linked_list_node
{
  voxel_synth_tile Element;
  voxel_synth_tile_linked_list_node *Next;
};

struct voxel_synth_tile_hashtable
{
  umm Size;
  voxel_synth_tile_linked_list_node **Elements;
};
link_internal voxel_synth_tile_linked_list_node *
Allocate_voxel_synth_tile_linked_list_node(memory_arena *Memory)
{
  voxel_synth_tile_linked_list_node *Result = Allocate( voxel_synth_tile_linked_list_node, Memory, 1);
  return Result;
}

link_internal voxel_synth_tile_hashtable
Allocate_voxel_synth_tile_hashtable(umm ElementCount, memory_arena *Memory)
{
  voxel_synth_tile_hashtable Result = {};
  Result.Elements = Allocate( voxel_synth_tile_linked_list_node*, Memory, ElementCount);
  Result.Size = ElementCount;
  return Result;
}

link_internal voxel_synth_tile_linked_list_node *
GetHashBucket(umm HashValue, voxel_synth_tile_hashtable *Table)
{
  Assert(Table->Size);
  voxel_synth_tile_linked_list_node *Result = Table->Elements[HashValue % Table->Size];
  return Result;
}

link_internal voxel_synth_tile *
GetFirstAtBucket(umm HashValue, voxel_synth_tile_hashtable *Table)
{
  voxel_synth_tile_linked_list_node *Bucket = GetHashBucket(HashValue, Table);
  voxel_synth_tile *Result = &Bucket->Element;
  return Result;
}

link_internal voxel_synth_tile *
Insert(voxel_synth_tile_linked_list_node *Node, voxel_synth_tile_hashtable *Table)
{
  Assert(Table->Size);
  umm HashValue = Hash(&Node->Element) % Table->Size;
  voxel_synth_tile_linked_list_node *Bucket = Table->Elements[HashValue];
  while (Bucket && Bucket->Next) Bucket = Bucket->Next;
  if (Bucket) { Assert(Bucket->Next == 0); Bucket->Next = Node; }
  else { Table->Elements[HashValue] = Node; }
  return &Node->Element;
}

link_internal voxel_synth_tile*
Insert(voxel_synth_tile Element, voxel_synth_tile_hashtable *Table, memory_arena *Memory)
{
  voxel_synth_tile_linked_list_node *Bucket = Allocate_voxel_synth_tile_linked_list_node(Memory);
  Bucket->Element = Element;
  Insert(Bucket, Table);
  return &Bucket->Element;
}
 
