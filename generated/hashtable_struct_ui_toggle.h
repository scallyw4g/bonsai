struct ui_toggle_linked_list_node
{
  ui_toggle Element;
  ui_toggle_linked_list_node *Next;
};

struct ui_toggle_hashtable
{
  umm Size;
  ui_toggle_linked_list_node **Elements;
  /* OWNED_BY_THREAD_MEMBER() */
};

