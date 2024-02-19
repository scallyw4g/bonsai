
struct member_info
{
  cs Type;
  cs Name;
  u64 Hash;

  /* u32 OffsetOfInBytes; */
};

poof(block_array(member_info, {8}))
#include <generated/block_array_member_info_688856411.h>

struct bonsai_type_info
{
  cs Name;
  u64 Version;

  u32 SizeOfInBytes;

  /* member_info_block_array Members; */
};

poof(are_equal(bonsai_type_info))
#include <generated/are_equal_bonsai_type_info.h>

link_internal umm
Hash(bonsai_type_info *Type)
{
  umm Result = Hash(Type->Name);
  return Result;
}

poof(buffer(bonsai_type_info))
#include <generated/buffer_bonsai_type_info.h>

poof(hashtable(bonsai_type_info))
#include <generated/hashtable_bonsai_type_info.h>

poof(hashtable_to_buffer(bonsai_type_info))
#include <generated/hashtable_to_buffer_bonsai_type_info.h>

poof(maybe(bonsai_type_info))
#include <generated/maybe_bonsai_type_info.h>

poof(hashtable_get(bonsai_type_info, {cs}, {Name}))
#include <generated/hashtable_get_bonsai_type_info_902_689333910.h>

global_variable bonsai_type_info_hashtable
Global_SerializeTypeTable;

global_variable memory_arena*
Global_SerializeTypeTableArena;

