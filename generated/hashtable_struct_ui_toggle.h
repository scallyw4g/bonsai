// callsite
// external/bonsai_stdlib/src/ui/ui.h:331:0

// def (hashtable_struct)
// external/bonsai_stdlib/src/poof_functions.h:793:0
struct ui_toggle_linked_list_node
{
  b32 Tombstoned;
  ui_toggle Element;
  ui_toggle_linked_list_node *Next;
};

struct ui_toggle_hashtable
poof(
  @collection
  
   @do_editor_ui 
)
{
  umm Size;
  ui_toggle_linked_list_node **Elements;
  /* OWNED_BY_THREAD_MEMBER() */
};

