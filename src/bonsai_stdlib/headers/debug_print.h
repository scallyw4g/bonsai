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

meta( for_all_datatypes( (StructName) {
      function void DebugPrint(StructName S);
  })
)
#include <metaprogramming/output/for_all_datatypes_debug_print_prototypes.h>

meta(
  for_all_datatypes(
    exclude
    counted_string,
    test_struct_1k
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

