struct xml_tag_linked_list_node
{
  xml_tag Element;
  xml_tag_linked_list_node *Next;
};

struct xml_tag_hashtable
{
  umm Size;
  xml_tag_linked_list_node **Elements;
  /* OWNED_BY_THREAD_MEMBER() */
};

