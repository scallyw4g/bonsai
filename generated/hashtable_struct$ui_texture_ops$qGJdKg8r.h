// callsite
// external/bonsai_stdlib/src/ui/ui.h:467:0

// def (hashtable_struct)
// external/bonsai_stdlib/src/poof_functions.h:813:0
struct ui_texture_ops_linked_list_node
{
  b32 Tombstoned;
  ui_texture_ops Element;
  ui_texture_ops_linked_list_node *Next;
};

struct ui_texture_ops_hashtable
poof(
  @collection
  
  
)
{
  u32 Size;
  u32 Pad;
  ui_texture_ops_linked_list_node **Elements;
  /* OWNED_BY_THREAD_MEMBER() */
};

