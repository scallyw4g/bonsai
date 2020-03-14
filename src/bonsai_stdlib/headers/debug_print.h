function void
DebugPrint(voxel_position E, u32 Depth = 0)
{
  printf("%*d", Depth, E.x);
  printf("%*d", Depth, E.y);
  printf("%*d", Depth, E.z);
}

function void
DebugPrint(Quaternion E, u32 Depth = 0)
{
  printf("%*f", Depth, E.E[0]);
  printf("%*f", Depth, E.E[1]);
  printf("%*f", Depth, E.E[2]);
  printf("%*f", Depth, E.E[3]);
}

function void
DebugPrint(v4 E, u32 Depth = 0)
{
  printf("%*f", Depth, E.x);
  printf("%*f", Depth, E.y);
  printf("%*f", Depth, E.z);
  printf("%*f", Depth, E.w);
}

function void
DebugPrint(v3i E, u32 Depth = 0)
{
  printf("%*u", Depth, E.x);
  printf("%*u", Depth, E.y);
  printf("%*u", Depth, E.z);
}

function void
DebugPrint(v3 E, u32 Depth = 0)
{
  printf("%*f", Depth, E.x);
  printf("%*f", Depth, E.y);
  printf("%*f", Depth, E.z);
}

function void
DebugPrint(r64 E, u32 Depth = 0)
{
  printf("%*f", Depth, E);
}

function void
DebugPrint(s64 E, u32 Depth = 0)
{
  printf("%*ld", Depth, E);
}

function void
DebugPrint(u64 E, u32 Depth = 0)
{
  printf("%*lu", Depth, E);
}

function void
DebugPrint(r32 E, u32 Depth = 0)
{
  printf("%*.2f", Depth, E);
}

function void
DebugPrint(s32 E, u32 Depth = 0)
{
  printf("%*d", Depth, E);
}

function void
DebugPrint(u32 E, u32 Depth = 0)
{
  printf("%*u", Depth, E);
}

function void
DebugPrint(volatile void* E, u32 Depth = 0)
{
  printf("%*p", Depth, E);
}

function void
DebugPrint(void* E, u32 Depth = 0)
{
  printf("%*p", Depth, E);
}

function void
DebugPrint(counted_string E, u32 Depth = 0)
{
  printf("%*.*s", Depth, (u32)E.Count, E.Start);
}

function void
DebugPrint(const char* E, u32 Depth = 0)
{
  u32 SLen = (u32)Length(E);
  u32 Pad = SLen + Depth;
  printf("%*s", Pad, E);
}

function void
DebugPrint(native_mutex E, u32 Depth = 0)
{
  printf("(mutex) : (%u) ? %u", *(u32*)&E, Depth);
}

function void
DebugPrint(semaphore E, u32 Depth = 0)
{
  printf("(semaphore) : (%u) ? %u", *(u32*)&E, Depth);
}

meta(
  named_list(external_datatypes)
  {
    thing1 thing2 thing3 thing4
    test_struct_8 test_struct_16 test_struct_32 test_struct_64 test_struct_128 test_struct_1k
    head_table ttf_vert ttf_contour simple_glyph font_table ttf offset_subtable
  }
)

// Note(Jesse): This is, for the purposes of DebugPrint at least, a primitive
// type and needs to be implemented by hand
meta( named_list(unprintable_datatypes) { counted_string })

// TODO(Jesse id: 185, tags: bug, high_priority): these should be printable!
meta( named_list(buggy_datatypes) { thread_startup_params network_connection debug_state perlin_noise })

meta(
  for_all_datatypes(
    exclude(unprintable_datatypes buggy_datatypes external_datatypes),
    {
      __(StructName)
      {  function void DebugPrint(StructName* S, u32 Depth = 0);
        function void DebugPrint(StructName S, u32 Depth = 0); }
    }
  )
)
#include <metaprogramming/output/for_all_datatypes_debug_print_prototypes.h>

meta(
  for_all_datatypes(
    exclude(unprintable_datatypes buggy_datatypes external_datatypes),
    {
      __(StructName)
      {
        function void DebugPrint(StructName S, u32 Depth)
        {
          DebugPrint("StructName\n", Depth);
          __(MemberType, MemberName)
          {
            DebugPrint("MemberName = ", Depth);
            DebugPrint(S.MemberName, Depth+1);
            DebugPrint("\n");
          }
        }
      }

      __(StructName)
      {
        function void DebugPrint(StructName* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("StructName\n", Depth);
            __(MemberType, MemberName)
            {
              DebugPrint("MemberName = ", Depth);
              DebugPrint(S->MemberName, Depth+1);
              DebugPrint("\n");
            }
          }
        }
      }

    }

  )
)
#include <metaprogramming/output/for_all_datatypes_debug_print_functions.h>

