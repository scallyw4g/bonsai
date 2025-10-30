// NOTE(Jesse): This includes implementations for performace profiling and debug tracing
#define BONSAI_DEBUG_SYSTEM_API 1

#include <bonsai_types.h>

#include <game_types.h>

BONSAI_API_MAIN_THREAD_INIT_CALLBACK()
{
  UNPACK_ENGINE_RESOURCES(Resources);
  Global_AssetPrefixPath = CSz("examples/ui_test/assets");

  world_position WorldCenter = {};
  canonical_position CameraTargetP = {};

  StandardCamera(Graphics->Camera, 30000.0f, 1000.0f);
  AllocateWorld(World, WorldCenter, WORLD_CHUNK_DIM, VisibleRegionSize_128);

  GameState = Allocate(game_state, Resources->GameMemory, 1);
  return GameState;
}

enum test_enum
{
  TestEnum_Foo,
  TestEnum_Bar,
  TestEnum_Baz,
};

poof(string_and_value_tables(test_enum))
#include <generated/string_and_value_tables_test_enum.h>
poof(do_editor_ui_for_enum(test_enum))
#include <generated/do_editor_ui_for_enum_test_enum.h>

enum radio_enum poof(@ui_display_radio)
{
  RadioEnum_Foo,
  RadioEnum_Bar,
  RadioEnum_Baz,
};

poof(string_and_value_tables(radio_enum))
#include <generated/string_and_value_tables_radio_enum.h>
poof(do_editor_ui_for_enum(radio_enum))
#include <generated/do_editor_ui_for_enum_radio_enum.h>


enum bitfield_enum poof(@bitfield)
{
  // Can put this in for testing the 0 path, or take it out to test the unset error path
  /* BitfieldEnum_Unset = (0), */

  BitfieldEnum_Foo = (1<<0),
  BitfieldEnum_Bar = (1<<1),
  BitfieldEnum_Baz = (1<<2),
};

poof(string_and_value_tables(bitfield_enum))
#include <generated/string_and_value_tables_bitfield_enum.h>
poof(do_editor_ui_for_enum(bitfield_enum))
#include <generated/do_editor_ui_for_enum_bitfield_enum.h>


struct nested_type
{
  s32 NestedFoo;
  f32 NestedBar;
  u32 Array[4];
};

poof(do_editor_ui_for_compound_type(nested_type))
#include <generated/do_editor_ui_for_compound_type_nested_type.h>

struct test_type
{

  u32 UnsignedNumber;
  f32 FloatNumber;
  s32 SignedNumber;
  b32 Boolean;

  nested_type Nested; poof(@ui_display_name({}))

  test_enum     BasicEnum;
  bitfield_enum BitfieldEnum;
  radio_enum    RadioEnum;
};

poof(do_editor_ui_for_compound_type(test_type))
#include <generated/do_editor_ui_for_compound_type_test_type.h>

BONSAI_API_MAIN_THREAD_CALLBACK()
{
  Assert(ThreadLocal_ThreadIndex == 0);

  TIMED_FUNCTION();
  UNPACK_ENGINE_RESOURCES(Resources);

  debug_global test_type Test = {};
  debug_global nested_type Nested = {};

  window_layout *Window = GetOrCreateWindow(Ui, "Test Window");

  PushWindowStart(Ui, Window);
    DoEditorUi(Ui, Window, &Test, CSz("Test Thing"), 0);

    PushNewRow(Ui);
    PushNewRow(Ui);
    PushNewRow(Ui);

    DoEditorUi(Ui, Window, &Nested, CSz("Test Nested"), 0);
  PushWindowEnd(Ui, Window);


}
