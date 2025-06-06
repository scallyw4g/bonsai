// external/bonsai_stdlib/src/primitive_containers.h:34:0

struct u32_linked_list_node
{
  b32 Tombstoned;
  u32 Element;
  u32_linked_list_node *Next;
};

struct u32_hashtable
{
  umm Size;
  u32_linked_list_node **Elements;
  /* OWNED_BY_THREAD_MEMBER() */
};

