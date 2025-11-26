// callsite
// external/bonsai_stdlib/src/ui/ui.h:151:0

// def (hashtable_struct)
// external/bonsai_stdlib/src/poof_functions.h:793:0
struct window_layout_linked_list_node
{
  b32 Tombstoned;
  window_layout Element;
  window_layout_linked_list_node *Next;
};

struct window_layout_hashtable
poof(
  @collection
  
   @do_editor_ui 
)
{
  umm Size;
  window_layout_linked_list_node **Elements;
  /* OWNED_BY_THREAD_MEMBER() */
};

