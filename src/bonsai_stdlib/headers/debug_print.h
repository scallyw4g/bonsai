bonsai_function void
DebugPrint(r64 E, u32 Depth = 0)
{
  DebugLine("%*s%f", Depth, "", E);
}

bonsai_function void
DebugPrint(s64 E, u32 Depth = 0)
{
  DebugLine("%*s%ld", Depth, "", E);
}

bonsai_function void
DebugPrint(u64 E, u32 Depth = 0)
{
  DebugLine("%*s%lu", Depth, "", E);
}

bonsai_function void
DebugPrint(r32 E, u32 Depth = 0)
{
  DebugLine("%*s%.2f", Depth, "", E);
}

bonsai_function void
DebugPrint(s32 E, u32 Depth = 0)
{
  DebugLine("%*s%d", Depth, "", E);
}

bonsai_function void
DebugPrint(u32 E, u32 Depth = 0)
{
  DebugLine("%*s%u", Depth, "", E);
}

bonsai_function void
DebugPrint(volatile void* E, u32 Depth = 0)
{
  DebugLine("%*s%p", Depth, "", E);
}

bonsai_function void
DebugPrint(void* E, u32 Depth = 0)
{
  DebugLine("%*s%p", Depth, "", E);
}

bonsai_function void
DebugPrint(__m128 E, u32 Depth = 0)
{
  DebugLine("%*s%f %f %f %f", Depth, "", E[0], E[1], E[2], E[3]);
}

bonsai_function void
DebugPrint(counted_string E, u32 Depth = 0)
{
  DebugLine("%*s%S", Depth, "", E);
}

bonsai_function void
DebugPrint(const char* E, u32 Depth = 0)
{
  DebugLine("%*s%s", Depth, "", E);
}

#if 0
bonsai_function void
DebugPrint(native_mutex E, u32 Depth = 0)
{
  DebugLine("%*smutex(%u)", Depth, "", *(u32*)&E);
}

bonsai_function void
DebugPrint(semaphore E, u32 Depth = 0)
{
  DebugLine("%*ssemaphore(%u)", Depth, "", *(u32*)&E);
}
#endif

meta(
  named_list(external_datatypes)
  {
    thing1 thing2 thing3 thing4
    test_struct_8 test_struct_16 test_struct_32 test_struct_64 test_struct_128 test_struct_1k
    head_table ttf_vert ttf_contour simple_glyph font_table ttf offset_subtable ttf_flag
  }
)

// Note(Jesse): This is, for the purposes of DebugPrint at least, a primitive
// type and needs to be implemented by hand
meta( named_list(unprintable_datatypes) { counted_string })

// TODO(Jesse id: 185, tags: bug, high_priority): these should be printable!
meta( named_list(buggy_datatypes) { debug_timed_function thread_startup_params network_connection debug_state perlin_noise })

meta( named_list(d_unions) { ast_node } )

bonsai_function void DebugPrint( ast_node *UnionStruct, u32 Depth = 0);
bonsai_function void DebugPrint( ast_node UnionStruct, u32 Depth = 0);
meta(
  for_datatypes(all)
    .exclude(unprintable_datatypes buggy_datatypes external_datatypes d_unions)

    func (StructDef)
    {
      bonsai_function void DebugPrint( (StructDef.name)* S, u32 Depth = 0);
      bonsai_function void DebugPrint( (StructDef.name)  S, u32 Depth = 0);
    }

    func (EnumDef)
    {
      bonsai_function void DebugPrint((EnumDef.name) EnumValue, u32 Depth = 0);
    }
)
#include <metaprogramming/output/for_all_datatypes_debug_print_prototypes.h>

meta(
  for_datatypes(all).exclude(unprintable_datatypes buggy_datatypes external_datatypes d_unions)

  func (StructDef)
  {
    bonsai_function void DebugPrint( (StructDef.name) S, u32 Depth)
    {
      if (Depth == 0)
      {
        DebugPrint("(StructDef.name) {\n", Depth);
      }

      (
        StructDef.map_members (Member)
        {
          DebugPrint("(Member.type) (Member.name) {\n", Depth+2);
          DebugPrint(S.(Member.name), Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");
        }
      )
      DebugPrint("}\n", Depth);
    }

    bonsai_function void DebugPrint( (StructDef.name) *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }
  }

  func (EnumDef)
  {
    bonsai_function void DebugPrint( (EnumDef.name) EnumValue, u32 Depth)
    {
      switch (EnumValue)
      {
        (
          EnumDef.map_values (ValueDef)
          {
            case (ValueDef.name):
            {
              DebugPrint("(ValueDef.name) ", Depth);
            } break;
          }
        )
      }
    }
  }

)
#include <metaprogramming/output/for_all_datatypes_debug_print_functions.h>


/* meta(dunion_debug_print(ast_node)) */
#include <debug_print_ast_node.h>

