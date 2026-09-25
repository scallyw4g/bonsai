
struct member_info
{
  cs Type;
  cs Name;
  u64 Hash;

  /* u32 OffsetOfInBytes; */
};

poof(block_array(member_info, {8}))
#include <generated/block_array$member_info.688856411$9DyxjdLY.h>

struct bonsai_type_info
{
  cs Name;
  u64 Version;

  u32 SizeOfInBytes;

  /* member_info_block_array Members; */
};

poof(are_equal(bonsai_type_info))
#include <generated/are_equal$bonsai_type_info$ZdI2Gk3U.h>

link_internal u32
Hash(bonsai_type_info *Type)
{
  u32 Result = Hash(Type->Name);
  return Result;
}

poof(buffer(bonsai_type_info))
#include <generated/buffer$bonsai_type_info$60QKFvNL.h>

poof(hashtable(bonsai_type_info))
#include <generated/hashtable$bonsai_type_info$GIAHTOZF.h>

poof(hashtable_to_buffer(bonsai_type_info))
#include <generated/hashtable_to_buffer$bonsai_type_info$fjHHAE1h.h>

poof(maybe(bonsai_type_info))
#include <generated/maybe$bonsai_type_info$JfyNCP70.h>

poof(hashtable_get(bonsai_type_info, {cs}, {Name}))
#include <generated/hashtable_get$bonsai_type_info.902.689333910$6LXDT9YV.h>

