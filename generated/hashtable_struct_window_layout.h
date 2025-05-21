// external/bonsai_stdlib/src/ui/ui.h:146:0

struct window_layout_linked_list_node
{
  b32 Tombstoned;
  window_layout Element;
  window_layout_linked_list_node *Next;
};

struct window_layout_hashtable
{
  umm Size;
  window_layout_linked_list_node **Elements;
  /* OWNED_BY_THREAD_MEMBER() */
};

