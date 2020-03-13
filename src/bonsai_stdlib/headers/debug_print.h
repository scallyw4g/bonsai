function void
DebugPrint(voxel_position E)
{
  printf("%d", E.x);
  printf("%d", E.y);
  printf("%d", E.z);
}

function void
DebugPrint(v4 E)
{
  printf("%f", E.x);
  printf("%f", E.y);
  printf("%f", E.z);
  printf("%f", E.w);
}

function void
DebugPrint(v3i E)
{
  printf("%u", E.x);
  printf("%u", E.y);
  printf("%u", E.z);
}

function void
DebugPrint(v3 E)
{
  printf("%f", E.x);
  printf("%f", E.y);
  printf("%f", E.z);
}

function void
DebugPrint(r64 E)
{
  printf("%f", E);
}

function void
DebugPrint(s64 E)
{
  printf("%ld", E);
}

function void
DebugPrint(u64 E)
{
  printf("%lu", E);
}

function void
DebugPrint(r32 E)
{
  printf("%f", E);
}

function void
DebugPrint(s32 E)
{
  printf("%d", E);
}

function void
DebugPrint(u32 E)
{
  printf("%u", E);
}

function void
DebugPrint(volatile void* E)
{
  printf("%p", E);
}

function void
DebugPrint(void* E)
{
  printf("%p", E);
}

function void
DebugPrint(counted_string E)
{
  printf("%.*s", (u32)E.Count, E.Start);
}

function void
DebugPrint(const char* E)
{
  printf("%s", E);
}

meta(
  for_all_datatypes(
    exclude

    platform hotkeys xml_hashtable xml_token_stream

    thing1 thing2 thing3 thing4
    test_struct_8 test_struct_16 test_struct_32 test_struct_64 test_struct_128 test_struct_1k
    head_table ttf_vert ttf_contour simple_glyph font_table ttf offset_subtable

    counted_string
    mutex address network_connection
    debug_state
    perlin_noise platform thread_startup_params
    (StructName) {
      function void DebugPrint(StructName S);
    })
)
#include <metaprogramming/output/for_all_datatypes_debug_print_prototypes.h>

meta(
  for_all_datatypes(
    exclude

    platform hotkeys xml_hashtable xml_token_stream

    thing1 thing2 thing3 thing4
    test_struct_8 test_struct_16 test_struct_32 test_struct_64 test_struct_128 test_struct_1k
    head_table ttf_vert ttf_contour simple_glyph font_table ttf offset_subtable

    counted_string
    mutex address network_connection
    debug_state
    perlin_noise platform thread_startup_params
    (StructName)
    {
      function void DebugPrint(StructName S)
      {
        __(MemberType, MemberName)
        {
          DebugPrint("MemberName");
          DebugPrint(S.MemberName);
        }
      }
    }
  )
)
#include <metaprogramming/output/for_all_datatypes_debug_print.h>

