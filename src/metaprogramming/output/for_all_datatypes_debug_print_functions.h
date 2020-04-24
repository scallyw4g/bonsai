
    function void DebugPrint( cycle_range S, u32 Depth)
    {
      DebugPrint("cycle_range {\n", Depth);
      
          DebugPrint("u64 StartCycle {\n", Depth+2);
          DebugPrint(S.StartCycle, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("u64 TotalCycles {\n", Depth+2);
          DebugPrint(S.TotalCycles, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( cycle_range *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( memory_arena_stats S, u32 Depth)
    {
      DebugPrint("memory_arena_stats {\n", Depth);
      
          DebugPrint("u64 Allocations {\n", Depth+2);
          DebugPrint(S.Allocations, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("u64 Pushes {\n", Depth+2);
          DebugPrint(S.Pushes, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("u64 TotalAllocated {\n", Depth+2);
          DebugPrint(S.TotalAllocated, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("u64 Remaining {\n", Depth+2);
          DebugPrint(S.Remaining, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( memory_arena_stats *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( min_max_avg_dt S, u32 Depth)
    {
      DebugPrint("min_max_avg_dt {\n", Depth);
      
          DebugPrint("r64 Min {\n", Depth+2);
          DebugPrint(S.Min, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("r64 Max {\n", Depth+2);
          DebugPrint(S.Max, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("r64 Avg {\n", Depth+2);
          DebugPrint(S.Avg, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( min_max_avg_dt *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( debug_ui_render_group S, u32 Depth)
    {
      DebugPrint("debug_ui_render_group {\n", Depth);
      
          DebugPrint("gpu_mapped_element_buffer GameGeo {\n", Depth+2);
          DebugPrint(S.GameGeo, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("shader GameGeoShader {\n", Depth+2);
          DebugPrint(S.GameGeoShader, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("debug_text_render_group TextGroup {\n", Depth+2);
          DebugPrint(S.TextGroup, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("u64 InteractionStackTop {\n", Depth+2);
          DebugPrint(S.InteractionStackTop, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("v2 MouseP {\n", Depth+2);
          DebugPrint(S.MouseP, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("v2 MouseDP {\n", Depth+2);
          DebugPrint(S.MouseDP, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("v2 ScreenDim {\n", Depth+2);
          DebugPrint(S.ScreenDim, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("input Input {\n", Depth+2);
          DebugPrint(S.Input, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("window_layout HighestWindow {\n", Depth+2);
          DebugPrint(S.HighestWindow, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("umm HoverInteractionId {\n", Depth+2);
          DebugPrint(S.HoverInteractionId, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("umm ClickedInteractionId {\n", Depth+2);
          DebugPrint(S.ClickedInteractionId, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("umm PressedInteractionId {\n", Depth+2);
          DebugPrint(S.PressedInteractionId, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("untextured_2d_geometry_buffer Geo {\n", Depth+2);
          DebugPrint(S.Geo, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("ui_render_command_buffer CommandBuffer {\n", Depth+2);
          DebugPrint(S.CommandBuffer, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( debug_ui_render_group *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( debug_profile_scope S, u32 Depth)
    {
      DebugPrint("debug_profile_scope {\n", Depth);
      
          DebugPrint("u64 CycleCount {\n", Depth+2);
          DebugPrint(S.CycleCount, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("u64 StartingCycle {\n", Depth+2);
          DebugPrint(S.StartingCycle, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("char Name {\n", Depth+2);
          DebugPrint(S.Name, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("b32 Expanded {\n", Depth+2);
          DebugPrint(S.Expanded, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("debug_profile_scope Sibling {\n", Depth+2);
          DebugPrint(S.Sibling, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("debug_profile_scope Child {\n", Depth+2);
          DebugPrint(S.Child, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("debug_profile_scope Parent {\n", Depth+2);
          DebugPrint(S.Parent, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( debug_profile_scope *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( unique_debug_profile_scope S, u32 Depth)
    {
      DebugPrint("unique_debug_profile_scope {\n", Depth);
      
          DebugPrint("char Name {\n", Depth+2);
          DebugPrint(S.Name, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("u32 CallCount {\n", Depth+2);
          DebugPrint(S.CallCount, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("u64 TotalCycles {\n", Depth+2);
          DebugPrint(S.TotalCycles, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("u64 MinCycles {\n", Depth+2);
          DebugPrint(S.MinCycles, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("u64 MaxCycles {\n", Depth+2);
          DebugPrint(S.MaxCycles, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("debug_profile_scope Scope {\n", Depth+2);
          DebugPrint(S.Scope, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("unique_debug_profile_scope NextUnique {\n", Depth+2);
          DebugPrint(S.NextUnique, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( unique_debug_profile_scope *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( debug_scope_tree S, u32 Depth)
    {
      DebugPrint("debug_scope_tree {\n", Depth);
      
          DebugPrint("debug_profile_scope Root {\n", Depth+2);
          DebugPrint(S.Root, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("debug_profile_scope WriteScope {\n", Depth+2);
          DebugPrint(S.WriteScope, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("debug_profile_scope ParentOfNextScope {\n", Depth+2);
          DebugPrint(S.ParentOfNextScope, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("u64 FrameRecorded {\n", Depth+2);
          DebugPrint(S.FrameRecorded, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( debug_scope_tree *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( debug_thread_state S, u32 Depth)
    {
      DebugPrint("debug_thread_state {\n", Depth);
      
          DebugPrint("memory_arena Memory {\n", Depth+2);
          DebugPrint(S.Memory, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("push_metadata MetaTable {\n", Depth+2);
          DebugPrint(S.MetaTable, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("debug_scope_tree ScopeTrees {\n", Depth+2);
          DebugPrint(S.ScopeTrees, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("debug_profile_scope FirstFreeScope {\n", Depth+2);
          DebugPrint(S.FirstFreeScope, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("mutex_op_array MutexOps {\n", Depth+2);
          DebugPrint(S.MutexOps, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("u32 WriteIndex {\n", Depth+2);
          DebugPrint(S.WriteIndex, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("u8 Pad {\n", Depth+2);
          DebugPrint(S.Pad, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( debug_thread_state *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( registered_memory_arena S, u32 Depth)
    {
      DebugPrint("registered_memory_arena {\n", Depth);
      
          DebugPrint("memory_arena Arena {\n", Depth+2);
          DebugPrint(S.Arena, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("char Name {\n", Depth+2);
          DebugPrint(S.Name, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("b32 Expanded {\n", Depth+2);
          DebugPrint(S.Expanded, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( registered_memory_arena *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( selected_memory_arena S, u32 Depth)
    {
      DebugPrint("selected_memory_arena {\n", Depth);
      
          DebugPrint("umm ArenaHash {\n", Depth+2);
          DebugPrint(S.ArenaHash, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("umm HeadArenaHash {\n", Depth+2);
          DebugPrint(S.HeadArenaHash, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( selected_memory_arena *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( selected_arenas S, u32 Depth)
    {
      DebugPrint("selected_arenas {\n", Depth);
      
          DebugPrint("u32 Count {\n", Depth+2);
          DebugPrint(S.Count, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("selected_memory_arena Arenas {\n", Depth+2);
          DebugPrint(S.Arenas, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( selected_arenas *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( frame_stats S, u32 Depth)
    {
      DebugPrint("frame_stats {\n", Depth);
      
          DebugPrint("u64 TotalCycles {\n", Depth+2);
          DebugPrint(S.TotalCycles, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("u64 StartingCycle {\n", Depth+2);
          DebugPrint(S.StartingCycle, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("r64 FrameMs {\n", Depth+2);
          DebugPrint(S.FrameMs, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( frame_stats *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( called_function S, u32 Depth)
    {
      DebugPrint("called_function {\n", Depth);
      
          DebugPrint("char Name {\n", Depth+2);
          DebugPrint(S.Name, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("u32 CallCount {\n", Depth+2);
          DebugPrint(S.CallCount, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( called_function *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( debug_draw_call S, u32 Depth)
    {
      DebugPrint("debug_draw_call {\n", Depth);
      
          DebugPrint("char Caller {\n", Depth+2);
          DebugPrint(S.Caller, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("u32 N {\n", Depth+2);
          DebugPrint(S.N, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("u32 Calls {\n", Depth+2);
          DebugPrint(S.Calls, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( debug_draw_call *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( window_layout S, u32 Depth)
    {
      DebugPrint("window_layout {\n", Depth);
      
          DebugPrint("counted_string Title {\n", Depth+2);
          DebugPrint(S.Title, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("v2 Basis {\n", Depth+2);
          DebugPrint(S.Basis, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("v2 MaxClip {\n", Depth+2);
          DebugPrint(S.MaxClip, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("u64 InteractionStackIndex {\n", Depth+2);
          DebugPrint(S.InteractionStackIndex, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("r32 zBackground {\n", Depth+2);
          DebugPrint(S.zBackground, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("r32 zText {\n", Depth+2);
          DebugPrint(S.zText, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("r32 zBorder {\n", Depth+2);
          DebugPrint(S.zBorder, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("r32 zTitleBar {\n", Depth+2);
          DebugPrint(S.zTitleBar, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("window_layout NextHotWindow {\n", Depth+2);
          DebugPrint(S.NextHotWindow, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( window_layout *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( font S, u32 Depth)
    {
      DebugPrint("font {\n", Depth);
      
          DebugPrint("v2 Size {\n", Depth+2);
          DebugPrint(S.Size, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( font *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( ui_element_reference S, u32 Depth)
    {
      DebugPrint("ui_element_reference {\n", Depth);
      
          DebugPrint("u32 Index {\n", Depth+2);
          DebugPrint(S.Index, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( ui_element_reference *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( ui_style S, u32 Depth)
    {
      DebugPrint("ui_style {\n", Depth);
      
          DebugPrint("v3 Color {\n", Depth+2);
          DebugPrint(S.Color, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("v3 HoverColor {\n", Depth+2);
          DebugPrint(S.HoverColor, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("v3 PressedColor {\n", Depth+2);
          DebugPrint(S.PressedColor, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("v3 ClickedColor {\n", Depth+2);
          DebugPrint(S.ClickedColor, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("v3 ActiveColor {\n", Depth+2);
          DebugPrint(S.ActiveColor, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("font Font {\n", Depth+2);
          DebugPrint(S.Font, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("b32 IsActive {\n", Depth+2);
          DebugPrint(S.IsActive, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( ui_style *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( layout S, u32 Depth)
    {
      DebugPrint("layout {\n", Depth);
      
          DebugPrint("v2 Basis {\n", Depth+2);
          DebugPrint(S.Basis, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("v2 At {\n", Depth+2);
          DebugPrint(S.At, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("rect2 DrawBounds {\n", Depth+2);
          DebugPrint(S.DrawBounds, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("v4 Padding {\n", Depth+2);
          DebugPrint(S.Padding, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("layout Prev {\n", Depth+2);
          DebugPrint(S.Prev, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( layout *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( ui_render_command_border S, u32 Depth)
    {
      DebugPrint("ui_render_command_border {\n", Depth);
      
          DebugPrint("window_layout Window {\n", Depth+2);
          DebugPrint(S.Window, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("rect2 Bounds {\n", Depth+2);
          DebugPrint(S.Bounds, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("v3 Color {\n", Depth+2);
          DebugPrint(S.Color, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( ui_render_command_border *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( ui_render_command_window_start S, u32 Depth)
    {
      DebugPrint("ui_render_command_window_start {\n", Depth);
      
          DebugPrint("layout Layout {\n", Depth+2);
          DebugPrint(S.Layout, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("window_layout Window {\n", Depth+2);
          DebugPrint(S.Window, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( ui_render_command_window_start *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( ui_render_command_window_end S, u32 Depth)
    {
      DebugPrint("ui_render_command_window_end {\n", Depth);
      
          DebugPrint("window_layout Window {\n", Depth+2);
          DebugPrint(S.Window, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( ui_render_command_window_end *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( ui_render_command_column_start S, u32 Depth)
    {
      DebugPrint("ui_render_command_column_start {\n", Depth);
      
          DebugPrint("layout Layout {\n", Depth+2);
          DebugPrint(S.Layout, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("ui_style Style {\n", Depth+2);
          DebugPrint(S.Style, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("r32 Width {\n", Depth+2);
          DebugPrint(S.Width, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("r32 MaxWidth {\n", Depth+2);
          DebugPrint(S.MaxWidth, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("column_render_params Params {\n", Depth+2);
          DebugPrint(S.Params, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( ui_render_command_column_start *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( ui_render_command_text S, u32 Depth)
    {
      DebugPrint("ui_render_command_text {\n", Depth);
      
          DebugPrint("layout Layout {\n", Depth+2);
          DebugPrint(S.Layout, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("ui_style Style {\n", Depth+2);
          DebugPrint(S.Style, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("counted_string String {\n", Depth+2);
          DebugPrint(S.String, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( ui_render_command_text *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( ui_render_command_text_at S, u32 Depth)
    {
      DebugPrint("ui_render_command_text_at {\n", Depth);
      
          DebugPrint("counted_string Text {\n", Depth+2);
          DebugPrint(S.Text, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("v2 At {\n", Depth+2);
          DebugPrint(S.At, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("v2 MaxClip {\n", Depth+2);
          DebugPrint(S.MaxClip, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( ui_render_command_text_at *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( ui_render_command_untextured_quad S, u32 Depth)
    {
      DebugPrint("ui_render_command_untextured_quad {\n", Depth);
      
          DebugPrint("layout Layout {\n", Depth+2);
          DebugPrint(S.Layout, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("ui_style Style {\n", Depth+2);
          DebugPrint(S.Style, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("v2 QuadDim {\n", Depth+2);
          DebugPrint(S.QuadDim, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("z_depth zDepth {\n", Depth+2);
          DebugPrint(S.zDepth, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("quad_render_params Params {\n", Depth+2);
          DebugPrint(S.Params, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( ui_render_command_untextured_quad *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( ui_render_command_untextured_quad_at S, u32 Depth)
    {
      DebugPrint("ui_render_command_untextured_quad_at {\n", Depth);
      
          DebugPrint("layout Layout {\n", Depth+2);
          DebugPrint(S.Layout, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("ui_style Style {\n", Depth+2);
          DebugPrint(S.Style, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("v2 QuadDim {\n", Depth+2);
          DebugPrint(S.QuadDim, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("z_depth zDepth {\n", Depth+2);
          DebugPrint(S.zDepth, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( ui_render_command_untextured_quad_at *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( ui_render_command_textured_quad S, u32 Depth)
    {
      DebugPrint("ui_render_command_textured_quad {\n", Depth);
      
          DebugPrint("layout Layout {\n", Depth+2);
          DebugPrint(S.Layout, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("debug_texture_array_slice TextureSlice {\n", Depth+2);
          DebugPrint(S.TextureSlice, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("z_depth zDepth {\n", Depth+2);
          DebugPrint(S.zDepth, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( ui_render_command_textured_quad *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( ui_render_command_button_start S, u32 Depth)
    {
      DebugPrint("ui_render_command_button_start {\n", Depth);
      
          DebugPrint("umm ID {\n", Depth+2);
          DebugPrint(S.ID, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("ui_style Style {\n", Depth+2);
          DebugPrint(S.Style, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( ui_render_command_button_start *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( ui_render_command_button_end S, u32 Depth)
    {
      DebugPrint("ui_render_command_button_end {\n", Depth);
      
          DebugPrint("button_end_params Params {\n", Depth+2);
          DebugPrint(S.Params, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( ui_render_command_button_end *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( ui_render_command_table_start S, u32 Depth)
    {
      DebugPrint("ui_render_command_table_start {\n", Depth);
      
          DebugPrint("layout Layout {\n", Depth+2);
          DebugPrint(S.Layout, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("ui_style Style {\n", Depth+2);
          DebugPrint(S.Style, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("relative_position Position {\n", Depth+2);
          DebugPrint(S.Position, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("ui_element_reference RelativeTo {\n", Depth+2);
          DebugPrint(S.RelativeTo, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( ui_render_command_table_start *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( ui_render_command_buffer S, u32 Depth)
    {
      DebugPrint("ui_render_command_buffer {\n", Depth);
      
          DebugPrint("u32 CommandCount {\n", Depth+2);
          DebugPrint(S.CommandCount, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("ui_render_command Commands {\n", Depth+2);
          DebugPrint(S.Commands, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( ui_render_command_buffer *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( clip_result S, u32 Depth)
    {
      DebugPrint("clip_result {\n", Depth);
      
          DebugPrint("clip_status ClipStatus {\n", Depth+2);
          DebugPrint(S.ClipStatus, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("v2 MaxClip {\n", Depth+2);
          DebugPrint(S.MaxClip, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("rect2 PartialClip {\n", Depth+2);
          DebugPrint(S.PartialClip, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( clip_result *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( sort_key S, u32 Depth)
    {
      DebugPrint("sort_key {\n", Depth);
      
          DebugPrint("u64 Value {\n", Depth+2);
          DebugPrint(S.Value, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("u64 Index {\n", Depth+2);
          DebugPrint(S.Index, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( sort_key *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( window_sort_params S, u32 Depth)
    {
      DebugPrint("window_sort_params {\n", Depth);
      
          DebugPrint("u32 Count {\n", Depth+2);
          DebugPrint(S.Count, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("u64 LowestInteractionStackIndex {\n", Depth+2);
          DebugPrint(S.LowestInteractionStackIndex, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("sort_key SortKeys {\n", Depth+2);
          DebugPrint(S.SortKeys, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( window_sort_params *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( find_button_start_result S, u32 Depth)
    {
      DebugPrint("find_button_start_result {\n", Depth);
      
          DebugPrint("u32 Index {\n", Depth+2);
          DebugPrint(S.Index, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("ui_render_command_button_start Command {\n", Depth+2);
          DebugPrint(S.Command, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( find_button_start_result *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( find_command_result S, u32 Depth)
    {
      DebugPrint("find_command_result {\n", Depth);
      
          DebugPrint("ui_render_command Command {\n", Depth+2);
          DebugPrint(S.Command, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("u32 Index {\n", Depth+2);
          DebugPrint(S.Index, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( find_command_result *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( interactable_handle S, u32 Depth)
    {
      DebugPrint("interactable_handle {\n", Depth);
      
          DebugPrint("umm Id {\n", Depth+2);
          DebugPrint(S.Id, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( interactable_handle *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( interactable S, u32 Depth)
    {
      DebugPrint("interactable {\n", Depth);
      
          DebugPrint("umm ID {\n", Depth+2);
          DebugPrint(S.ID, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("v2 MinP {\n", Depth+2);
          DebugPrint(S.MinP, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("v2 MaxP {\n", Depth+2);
          DebugPrint(S.MaxP, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("window_layout Window {\n", Depth+2);
          DebugPrint(S.Window, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( interactable *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( button_interaction_result S, u32 Depth)
    {
      DebugPrint("button_interaction_result {\n", Depth);
      
          DebugPrint("b32 Pressed {\n", Depth+2);
          DebugPrint(S.Pressed, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("b32 Clicked {\n", Depth+2);
          DebugPrint(S.Clicked, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("b32 Hover {\n", Depth+2);
          DebugPrint(S.Hover, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( button_interaction_result *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( render_state S, u32 Depth)
    {
      DebugPrint("render_state {\n", Depth);
      
          DebugPrint("window_layout Window {\n", Depth+2);
          DebugPrint(S.Window, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("u32 WindowStartCommandIndex {\n", Depth+2);
          DebugPrint(S.WindowStartCommandIndex, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("layout Layout {\n", Depth+2);
          DebugPrint(S.Layout, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("b32 Hover {\n", Depth+2);
          DebugPrint(S.Hover, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("b32 Pressed {\n", Depth+2);
          DebugPrint(S.Pressed, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("b32 Clicked {\n", Depth+2);
          DebugPrint(S.Clicked, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( render_state *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( debug_text_render_group S, u32 Depth)
    {
      DebugPrint("debug_text_render_group {\n", Depth);
      
          DebugPrint("u32 SolidUIVertexBuffer {\n", Depth+2);
          DebugPrint(S.SolidUIVertexBuffer, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("u32 SolidUIColorBuffer {\n", Depth+2);
          DebugPrint(S.SolidUIColorBuffer, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("u32 SolidUIUVBuffer {\n", Depth+2);
          DebugPrint(S.SolidUIUVBuffer, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("texture FontTexture {\n", Depth+2);
          DebugPrint(S.FontTexture, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("shader Text2DShader {\n", Depth+2);
          DebugPrint(S.Text2DShader, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("s32 TextTextureUniform {\n", Depth+2);
          DebugPrint(S.TextTextureUniform, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("textured_2d_geometry_buffer Geo {\n", Depth+2);
          DebugPrint(S.Geo, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("shader DebugFontTextureShader {\n", Depth+2);
          DebugPrint(S.DebugFontTextureShader, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("shader SolidUIShader {\n", Depth+2);
          DebugPrint(S.SolidUIShader, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( debug_text_render_group *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( struct_member_stream S, u32 Depth)
    {
      DebugPrint("struct_member_stream {\n", Depth);
      
          DebugPrint("struct_member_stream_chunk FirstChunk {\n", Depth+2);
          DebugPrint(S.FirstChunk, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("struct_member_stream_chunk LastChunk {\n", Depth+2);
          DebugPrint(S.LastChunk, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( struct_member_stream *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( struct_def S, u32 Depth)
    {
      DebugPrint("struct_def {\n", Depth);
      
          DebugPrint("counted_string Type {\n", Depth+2);
          DebugPrint(S.Type, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("counted_string DefinedInFile {\n", Depth+2);
          DebugPrint(S.DefinedInFile, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("struct_member_stream Members {\n", Depth+2);
          DebugPrint(S.Members, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("b32 IsUnion {\n", Depth+2);
          DebugPrint(S.IsUnion, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( struct_def *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( struct_member_anonymous S, u32 Depth)
    {
      DebugPrint("struct_member_anonymous {\n", Depth);
      
          DebugPrint("struct_def Body {\n", Depth+2);
          DebugPrint(S.Body, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( struct_member_anonymous *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( type_spec S, u32 Depth)
    {
      DebugPrint("type_spec {\n", Depth);
      
          DebugPrint("counted_string Name {\n", Depth+2);
          DebugPrint(S.Name, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("u32 IndirectionLevel {\n", Depth+2);
          DebugPrint(S.IndirectionLevel, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("u32 ReferenceLevel {\n", Depth+2);
          DebugPrint(S.ReferenceLevel, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("b32 Unsigned {\n", Depth+2);
          DebugPrint(S.Unsigned, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("b32 Volatile {\n", Depth+2);
          DebugPrint(S.Volatile, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("b32 Const {\n", Depth+2);
          DebugPrint(S.Const, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("b32 ThreadLocal {\n", Depth+2);
          DebugPrint(S.ThreadLocal, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("b32 IsMetaTemplateVar {\n", Depth+2);
          DebugPrint(S.IsMetaTemplateVar, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("b32 IsFunction {\n", Depth+2);
          DebugPrint(S.IsFunction, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("b32 IsOperator {\n", Depth+2);
          DebugPrint(S.IsOperator, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("b32 IsConstructor {\n", Depth+2);
          DebugPrint(S.IsConstructor, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("b32 Inline {\n", Depth+2);
          DebugPrint(S.Inline, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("b32 IsStatic {\n", Depth+2);
          DebugPrint(S.IsStatic, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("b32 IsExported {\n", Depth+2);
          DebugPrint(S.IsExported, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("b32 IsTemplateFunction {\n", Depth+2);
          DebugPrint(S.IsTemplateFunction, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("counted_string TemplateSource {\n", Depth+2);
          DebugPrint(S.TemplateSource, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("counted_string Namespace {\n", Depth+2);
          DebugPrint(S.Namespace, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("counted_string SourceText {\n", Depth+2);
          DebugPrint(S.SourceText, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( type_spec *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( variable S, u32 Depth)
    {
      DebugPrint("variable {\n", Depth);
      
          DebugPrint("type_spec Type {\n", Depth+2);
          DebugPrint(S.Type, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("counted_string Name {\n", Depth+2);
          DebugPrint(S.Name, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("b32 IsVariadic {\n", Depth+2);
          DebugPrint(S.IsVariadic, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("counted_string Namespace {\n", Depth+2);
          DebugPrint(S.Namespace, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("counted_string FnPointerDef {\n", Depth+2);
          DebugPrint(S.FnPointerDef, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("counted_string StaticBufferSize {\n", Depth+2);
          DebugPrint(S.StaticBufferSize, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("counted_string SourceText {\n", Depth+2);
          DebugPrint(S.SourceText, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( variable *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( enum_member S, u32 Depth)
    {
      DebugPrint("enum_member {\n", Depth);
      
          DebugPrint("counted_string Name {\n", Depth+2);
          DebugPrint(S.Name, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("counted_string Value {\n", Depth+2);
          DebugPrint(S.Value, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( enum_member *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( d_union_member S, u32 Depth)
    {
      DebugPrint("d_union_member {\n", Depth);
      
          DebugPrint("counted_string Type {\n", Depth+2);
          DebugPrint(S.Type, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("counted_string Name {\n", Depth+2);
          DebugPrint(S.Name, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("d_union_flags Flags {\n", Depth+2);
          DebugPrint(S.Flags, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( d_union_member *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( enum_def S, u32 Depth)
    {
      DebugPrint("enum_def {\n", Depth);
      
          DebugPrint("counted_string Name {\n", Depth+2);
          DebugPrint(S.Name, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("enum_member_stream Members {\n", Depth+2);
          DebugPrint(S.Members, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( enum_def *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( datatype S, u32 Depth)
    {
      DebugPrint("datatype {\n", Depth);
      
          DebugPrint("datatype_type Type {\n", Depth+2);
          DebugPrint(S.Type, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( datatype *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( meta_func_arg S, u32 Depth)
    {
      DebugPrint("meta_func_arg {\n", Depth);
      
          DebugPrint("counted_string Match {\n", Depth+2);
          DebugPrint(S.Match, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("datatype Data {\n", Depth+2);
          DebugPrint(S.Data, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( meta_func_arg *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( c_token S, u32 Depth)
    {
      DebugPrint("c_token {\n", Depth);
      
          DebugPrint("c_token_type Type {\n", Depth+2);
          DebugPrint(S.Type, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("counted_string Value {\n", Depth+2);
          DebugPrint(S.Value, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( c_token *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( d_union_decl S, u32 Depth)
    {
      DebugPrint("d_union_decl {\n", Depth);
      
          DebugPrint("counted_string Name {\n", Depth+2);
          DebugPrint(S.Name, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("d_union_member_stream Members {\n", Depth+2);
          DebugPrint(S.Members, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("struct_member_stream CommonMembers {\n", Depth+2);
          DebugPrint(S.CommonMembers, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("counted_string CustomEnumType {\n", Depth+2);
          DebugPrint(S.CustomEnumType, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( d_union_decl *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( c_parse_result S, u32 Depth)
    {
      DebugPrint("c_parse_result {\n", Depth);
      
          DebugPrint("b32 Valid {\n", Depth+2);
          DebugPrint(S.Valid, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("c_token_cursor Tokens {\n", Depth+2);
          DebugPrint(S.Tokens, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("c_token_cursor OutputTokens {\n", Depth+2);
          DebugPrint(S.OutputTokens, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("counted_string Filename {\n", Depth+2);
          DebugPrint(S.Filename, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("u32 LineNumber {\n", Depth+2);
          DebugPrint(S.LineNumber, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( c_parse_result *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( meta_func S, u32 Depth)
    {
      DebugPrint("meta_func {\n", Depth);
      
          DebugPrint("counted_string Name {\n", Depth+2);
          DebugPrint(S.Name, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("counted_string ArgName {\n", Depth+2);
          DebugPrint(S.ArgName, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("c_parse_result Body {\n", Depth+2);
          DebugPrint(S.Body, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( meta_func *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( todo S, u32 Depth)
    {
      DebugPrint("todo {\n", Depth);
      
          DebugPrint("counted_string Id {\n", Depth+2);
          DebugPrint(S.Id, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("counted_string Value {\n", Depth+2);
          DebugPrint(S.Value, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("b32 FoundInCodebase {\n", Depth+2);
          DebugPrint(S.FoundInCodebase, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( todo *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( tag S, u32 Depth)
    {
      DebugPrint("tag {\n", Depth);
      
          DebugPrint("counted_string Tag {\n", Depth+2);
          DebugPrint(S.Tag, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("todo_stream Todos {\n", Depth+2);
          DebugPrint(S.Todos, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( tag *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( person S, u32 Depth)
    {
      DebugPrint("person {\n", Depth);
      
          DebugPrint("counted_string Name {\n", Depth+2);
          DebugPrint(S.Name, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("tag_stream Tags {\n", Depth+2);
          DebugPrint(S.Tags, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( person *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( ast_node_expression S, u32 Depth)
    {
      DebugPrint("ast_node_expression {\n", Depth);
      
          DebugPrint("ast_node Value {\n", Depth+2);
          DebugPrint(S.Value, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("ast_node Next {\n", Depth+2);
          DebugPrint(S.Next, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( ast_node_expression *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( ast_node_statement S, u32 Depth)
    {
      DebugPrint("ast_node_statement {\n", Depth);
      
          DebugPrint("ast_node_expression LHS {\n", Depth+2);
          DebugPrint(S.LHS, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("ast_node_expression RHS {\n", Depth+2);
          DebugPrint(S.RHS, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("ast_node_statement Next {\n", Depth+2);
          DebugPrint(S.Next, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( ast_node_statement *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( ast_node_function_call S, u32 Depth)
    {
      DebugPrint("ast_node_function_call {\n", Depth);
      
          DebugPrint("function_def Prototype {\n", Depth+2);
          DebugPrint(S.Prototype, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("ast_node_expression Args {\n", Depth+2);
          DebugPrint(S.Args, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( ast_node_function_call *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( ast_node_variable_def S, u32 Depth)
    {
      DebugPrint("ast_node_variable_def {\n", Depth);
      
          DebugPrint("datatype Type {\n", Depth+2);
          DebugPrint(S.Type, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("variable Decl {\n", Depth+2);
          DebugPrint(S.Decl, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("ast_node Value {\n", Depth+2);
          DebugPrint(S.Value, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( ast_node_variable_def *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( ast_node_scope S, u32 Depth)
    {
      DebugPrint("ast_node_scope {\n", Depth);
      
          DebugPrint("ast_node_statement FirstStatement {\n", Depth+2);
          DebugPrint(S.FirstStatement, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( ast_node_scope *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( ast_node_parenthesized S, u32 Depth)
    {
      DebugPrint("ast_node_parenthesized {\n", Depth);
      
          DebugPrint("ast_node_expression Expr {\n", Depth+2);
          DebugPrint(S.Expr, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( ast_node_parenthesized *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( ast_node_initializer_list S, u32 Depth)
    {
      DebugPrint("ast_node_initializer_list {\n", Depth);
      
          DebugPrint("u32 Ignored {\n", Depth+2);
          DebugPrint(S.Ignored, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( ast_node_initializer_list *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( ast_node_operator S, u32 Depth)
    {
      DebugPrint("ast_node_operator {\n", Depth);
      
          DebugPrint("ast_node_expression Operand {\n", Depth+2);
          DebugPrint(S.Operand, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("c_token Token {\n", Depth+2);
          DebugPrint(S.Token, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( ast_node_operator *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( ast_node_literal S, u32 Depth)
    {
      DebugPrint("ast_node_literal {\n", Depth);
      
          DebugPrint("c_token Token {\n", Depth+2);
          DebugPrint(S.Token, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( ast_node_literal *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( ast_node_symbol S, u32 Depth)
    {
      DebugPrint("ast_node_symbol {\n", Depth);
      
          DebugPrint("c_token Token {\n", Depth+2);
          DebugPrint(S.Token, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("ast_node Access {\n", Depth+2);
          DebugPrint(S.Access, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( ast_node_symbol *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( ast_node_ignored S, u32 Depth)
    {
      DebugPrint("ast_node_ignored {\n", Depth);
      
          DebugPrint("c_token Token {\n", Depth+2);
          DebugPrint(S.Token, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( ast_node_ignored *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( ast_node_return S, u32 Depth)
    {
      DebugPrint("ast_node_return {\n", Depth);
      
          DebugPrint("ast_node_expression Value {\n", Depth+2);
          DebugPrint(S.Value, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( ast_node_return *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( ast_node_predicated S, u32 Depth)
    {
      DebugPrint("ast_node_predicated {\n", Depth);
      
          DebugPrint("c_token Token {\n", Depth+2);
          DebugPrint(S.Token, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("ast_node_expression Predicate {\n", Depth+2);
          DebugPrint(S.Predicate, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( ast_node_predicated *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( arguments S, u32 Depth)
    {
      DebugPrint("arguments {\n", Depth);
      
          DebugPrint("counted_string Outpath {\n", Depth+2);
          DebugPrint(S.Outpath, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("counted_string_cursor Files {\n", Depth+2);
          DebugPrint(S.Files, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("b32 DoDebugWindow {\n", Depth+2);
          DebugPrint(S.DoDebugWindow, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( arguments *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( scope S, u32 Depth)
    {
      DebugPrint("scope {\n", Depth);
      
          DebugPrint("variable_stream Decls {\n", Depth+2);
          DebugPrint(S.Decls, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("scope Parent {\n", Depth+2);
          DebugPrint(S.Parent, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( scope *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( function_def S, u32 Depth)
    {
      DebugPrint("function_def {\n", Depth);
      
          DebugPrint("variable Prototype {\n", Depth+2);
          DebugPrint(S.Prototype, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("variable_stream Args {\n", Depth+2);
          DebugPrint(S.Args, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("ast_node_variable_def_stream Locals {\n", Depth+2);
          DebugPrint(S.Locals, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("c_parse_result Body {\n", Depth+2);
          DebugPrint(S.Body, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("ast_node_statement Ast {\n", Depth+2);
          DebugPrint(S.Ast, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( function_def *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( program_datatypes S, u32 Depth)
    {
      DebugPrint("program_datatypes {\n", Depth);
      
          DebugPrint("struct_def_stream Structs {\n", Depth+2);
          DebugPrint(S.Structs, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("enum_def_stream Enums {\n", Depth+2);
          DebugPrint(S.Enums, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("function_def_stream Functions {\n", Depth+2);
          DebugPrint(S.Functions, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( program_datatypes *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( for_enum_constraints S, u32 Depth)
    {
      DebugPrint("for_enum_constraints {\n", Depth);
      
          DebugPrint("counted_string TypeName {\n", Depth+2);
          DebugPrint(S.TypeName, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("counted_string ValueName {\n", Depth+2);
          DebugPrint(S.ValueName, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( for_enum_constraints *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( body_text_constraints S, u32 Depth)
    {
      DebugPrint("body_text_constraints {\n", Depth);
      
          DebugPrint("counted_string MemberContains {\n", Depth+2);
          DebugPrint(S.MemberContains, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("counted_string TypeTag {\n", Depth+2);
          DebugPrint(S.TypeTag, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("counted_string TypeName {\n", Depth+2);
          DebugPrint(S.TypeName, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("counted_string ValueName {\n", Depth+2);
          DebugPrint(S.ValueName, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( body_text_constraints *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( metaprogramming_info S, u32 Depth)
    {
      DebugPrint("metaprogramming_info {\n", Depth);
      
          DebugPrint("meta_func_stream FunctionDefs {\n", Depth+2);
          DebugPrint(S.FunctionDefs, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("program_datatypes Datatypes {\n", Depth+2);
          DebugPrint(S.Datatypes, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( metaprogramming_info *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( todo_list_info S, u32 Depth)
    {
      DebugPrint("todo_list_info {\n", Depth);
      
          DebugPrint("person_stream People {\n", Depth+2);
          DebugPrint(S.People, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("tagged_counted_string_stream_stream NameLists {\n", Depth+2);
          DebugPrint(S.NameLists, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( todo_list_info *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( string_from_parser S, u32 Depth)
    {
      DebugPrint("string_from_parser {\n", Depth);
      
          DebugPrint("char Start {\n", Depth+2);
          DebugPrint(S.Start, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( string_from_parser *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( client_state S, u32 Depth)
    {
      DebugPrint("client_state {\n", Depth);
      
          DebugPrint("s32 Id {\n", Depth+2);
          DebugPrint(S.Id, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("u32 Counter {\n", Depth+2);
          DebugPrint(S.Counter, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("canonical_position P {\n", Depth+2);
          DebugPrint(S.P, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( client_state *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( server_state S, u32 Depth)
    {
      DebugPrint("server_state {\n", Depth);
      
          DebugPrint("client_state Clients {\n", Depth+2);
          DebugPrint(S.Clients, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( server_state *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( client_to_server_message S, u32 Depth)
    {
      DebugPrint("client_to_server_message {\n", Depth);
      
          DebugPrint("client_state Client {\n", Depth+2);
          DebugPrint(S.Client, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( client_to_server_message *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( handshake_message S, u32 Depth)
    {
      DebugPrint("handshake_message {\n", Depth);
      
          DebugPrint("s32 ClientId {\n", Depth+2);
          DebugPrint(S.ClientId, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( handshake_message *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( socket_t S, u32 Depth)
    {
      DebugPrint("socket_t {\n", Depth);
      
          DebugPrint("s32 Id {\n", Depth+2);
          DebugPrint(S.Id, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("socket_type Type {\n", Depth+2);
          DebugPrint(S.Type, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( socket_t *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( plane S, u32 Depth)
    {
      DebugPrint("plane {\n", Depth);
      
          DebugPrint("v3 P {\n", Depth+2);
          DebugPrint(S.P, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("v3 Normal {\n", Depth+2);
          DebugPrint(S.Normal, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("r32 d {\n", Depth+2);
          DebugPrint(S.d, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( plane *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( frustum S, u32 Depth)
    {
      DebugPrint("frustum {\n", Depth);
      
          DebugPrint("float farClip {\n", Depth+2);
          DebugPrint(S.farClip, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("float nearClip {\n", Depth+2);
          DebugPrint(S.nearClip, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("float width {\n", Depth+2);
          DebugPrint(S.width, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("float FOV {\n", Depth+2);
          DebugPrint(S.FOV, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("plane Top {\n", Depth+2);
          DebugPrint(S.Top, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("plane Bot {\n", Depth+2);
          DebugPrint(S.Bot, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("plane Left {\n", Depth+2);
          DebugPrint(S.Left, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("plane Right {\n", Depth+2);
          DebugPrint(S.Right, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( frustum *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( camera S, u32 Depth)
    {
      DebugPrint("camera {\n", Depth);
      
          DebugPrint("frustum Frust {\n", Depth+2);
          DebugPrint(S.Frust, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("canonical_position TargetP {\n", Depth+2);
          DebugPrint(S.TargetP, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("canonical_position CurrentP {\n", Depth+2);
          DebugPrint(S.CurrentP, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("canonical_position ViewingTarget {\n", Depth+2);
          DebugPrint(S.ViewingTarget, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("v3 RenderSpacePosition {\n", Depth+2);
          DebugPrint(S.RenderSpacePosition, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("r32 Pitch {\n", Depth+2);
          DebugPrint(S.Pitch, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("r32 Roll {\n", Depth+2);
          DebugPrint(S.Roll, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("r32 Yaw {\n", Depth+2);
          DebugPrint(S.Yaw, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("r32 DistanceFromTarget {\n", Depth+2);
          DebugPrint(S.DistanceFromTarget, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("v3 Front {\n", Depth+2);
          DebugPrint(S.Front, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("v3 Right {\n", Depth+2);
          DebugPrint(S.Right, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("v3 Up {\n", Depth+2);
          DebugPrint(S.Up, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( camera *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( work_queue_entry_copy_buffer S, u32 Depth)
    {
      DebugPrint("work_queue_entry_copy_buffer {\n", Depth);
      
          DebugPrint("untextured_3d_geometry_buffer Src {\n", Depth+2);
          DebugPrint(S.Src, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("untextured_3d_geometry_buffer Dest {\n", Depth+2);
          DebugPrint(S.Dest, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("v3 Basis {\n", Depth+2);
          DebugPrint(S.Basis, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( work_queue_entry_copy_buffer *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( work_queue_entry_init_world_chunk S, u32 Depth)
    {
      DebugPrint("work_queue_entry_init_world_chunk {\n", Depth);
      
          DebugPrint("void Input {\n", Depth+2);
          DebugPrint(S.Input, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( work_queue_entry_init_world_chunk *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( edge S, u32 Depth)
    {
      DebugPrint("edge {\n", Depth);
      
          DebugPrint("voxel_position P0 {\n", Depth+2);
          DebugPrint(S.P0, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("voxel_position P1 {\n", Depth+2);
          DebugPrint(S.P1, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("r32 LengthSq {\n", Depth+2);
          DebugPrint(S.LengthSq, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( edge *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( triangle S, u32 Depth)
    {
      DebugPrint("triangle {\n", Depth);
      
          DebugPrint("voxel_position Points {\n", Depth+2);
          DebugPrint(S.Points, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("u32 Index {\n", Depth+2);
          DebugPrint(S.Index, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( triangle *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( boundary_voxels S, u32 Depth)
    {
      DebugPrint("boundary_voxels {\n", Depth);
      
          DebugPrint("voxel_position Points {\n", Depth+2);
          DebugPrint(S.Points, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("u32 At {\n", Depth+2);
          DebugPrint(S.At, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("u32 End {\n", Depth+2);
          DebugPrint(S.End, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("voxel_position Min {\n", Depth+2);
          DebugPrint(S.Min, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("voxel_position Max {\n", Depth+2);
          DebugPrint(S.Max, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( boundary_voxels *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( current_triangles S, u32 Depth)
    {
      DebugPrint("current_triangles {\n", Depth);
      
          DebugPrint("triangle Tris {\n", Depth+2);
          DebugPrint(S.Tris, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("u32 Max {\n", Depth+2);
          DebugPrint(S.Max, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("u32 Count {\n", Depth+2);
          DebugPrint(S.Count, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("u32 CurrentPointIndex {\n", Depth+2);
          DebugPrint(S.CurrentPointIndex, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("boundary_voxels SurfacePoints {\n", Depth+2);
          DebugPrint(S.SurfacePoints, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( current_triangles *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( canonical_position S, u32 Depth)
    {
      DebugPrint("canonical_position {\n", Depth);
      
          DebugPrint("v3 Offset {\n", Depth+2);
          DebugPrint(S.Offset, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("world_position WorldP {\n", Depth+2);
          DebugPrint(S.WorldP, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( canonical_position *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( loaded_collada_mesh S, u32 Depth)
    {
      DebugPrint("loaded_collada_mesh {\n", Depth);
      
          DebugPrint("untextured_3d_geometry_buffer Mesh {\n", Depth+2);
          DebugPrint(S.Mesh, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("v3 Dim {\n", Depth+2);
          DebugPrint(S.Dim, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( loaded_collada_mesh *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( free_mesh S, u32 Depth)
    {
      DebugPrint("free_mesh {\n", Depth);
      
          DebugPrint("untextured_3d_geometry_buffer Mesh {\n", Depth+2);
          DebugPrint(S.Mesh, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("free_mesh Next {\n", Depth+2);
          DebugPrint(S.Next, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( free_mesh *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( mesh_freelist S, u32 Depth)
    {
      DebugPrint("mesh_freelist {\n", Depth);
      
          DebugPrint("free_mesh FirstFree {\n", Depth+2);
          DebugPrint(S.FirstFree, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("free_mesh Containers {\n", Depth+2);
          DebugPrint(S.Containers, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( mesh_freelist *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( xml_property S, u32 Depth)
    {
      DebugPrint("xml_property {\n", Depth);
      
          DebugPrint("counted_string Name {\n", Depth+2);
          DebugPrint(S.Name, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("counted_string Value {\n", Depth+2);
          DebugPrint(S.Value, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("xml_property Next {\n", Depth+2);
          DebugPrint(S.Next, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( xml_property *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( xml_token S, u32 Depth)
    {
      DebugPrint("xml_token {\n", Depth);
      
          DebugPrint("xml_token_type Type {\n", Depth+2);
          DebugPrint(S.Type, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("xml_property Property {\n", Depth+2);
          DebugPrint(S.Property, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( xml_token *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( xml_tag S, u32 Depth)
    {
      DebugPrint("xml_tag {\n", Depth);
      
          DebugPrint("xml_token Open {\n", Depth+2);
          DebugPrint(S.Open, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("xml_tag Parent {\n", Depth+2);
          DebugPrint(S.Parent, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("xml_tag Sibling {\n", Depth+2);
          DebugPrint(S.Sibling, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("xml_tag NextInHash {\n", Depth+2);
          DebugPrint(S.NextInHash, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("umm HashValue {\n", Depth+2);
          DebugPrint(S.HashValue, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("counted_string Value {\n", Depth+2);
          DebugPrint(S.Value, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("xml_property Properties {\n", Depth+2);
          DebugPrint(S.Properties, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("xml_property NextPropertySlot {\n", Depth+2);
          DebugPrint(S.NextPropertySlot, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( xml_tag *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( xml_hashtable S, u32 Depth)
    {
      DebugPrint("xml_hashtable {\n", Depth);
      
          DebugPrint("umm Size {\n", Depth+2);
          DebugPrint(S.Size, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("xml_tag Table {\n", Depth+2);
          DebugPrint(S.Table, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( xml_hashtable *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( xml_token_stream S, u32 Depth)
    {
      DebugPrint("xml_token_stream {\n", Depth);
      
          DebugPrint("xml_token Start {\n", Depth+2);
          DebugPrint(S.Start, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("xml_token At {\n", Depth+2);
          DebugPrint(S.At, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("xml_token End {\n", Depth+2);
          DebugPrint(S.End, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("xml_hashtable Hashes {\n", Depth+2);
          DebugPrint(S.Hashes, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( xml_token_stream *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( xml_tag_stream S, u32 Depth)
    {
      DebugPrint("xml_tag_stream {\n", Depth);
      
          DebugPrint("xml_tag Start {\n", Depth+2);
          DebugPrint(S.Start, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("xml_tag At {\n", Depth+2);
          DebugPrint(S.At, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("xml_tag End {\n", Depth+2);
          DebugPrint(S.End, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( xml_tag_stream *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( xml_parsing_at_indicators S, u32 Depth)
    {
      DebugPrint("xml_parsing_at_indicators {\n", Depth);
      
          DebugPrint("xml_tag LastClosedTag {\n", Depth+2);
          DebugPrint(S.LastClosedTag, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("xml_tag CurrentlyOpenTag {\n", Depth+2);
          DebugPrint(S.CurrentlyOpenTag, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( xml_parsing_at_indicators *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( post_processing_group S, u32 Depth)
    {
      DebugPrint("post_processing_group {\n", Depth);
      
          DebugPrint("framebuffer FBO {\n", Depth+2);
          DebugPrint(S.FBO, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("shader Shader {\n", Depth+2);
          DebugPrint(S.Shader, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("u32 VertexBuffer {\n", Depth+2);
          DebugPrint(S.VertexBuffer, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("u32 ColorBuffer {\n", Depth+2);
          DebugPrint(S.ColorBuffer, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( post_processing_group *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( graphics S, u32 Depth)
    {
      DebugPrint("graphics {\n", Depth);
      
          DebugPrint("camera Camera {\n", Depth+2);
          DebugPrint(S.Camera, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("game_lights Lights {\n", Depth+2);
          DebugPrint(S.Lights, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("g_buffer_render_group gBuffer {\n", Depth+2);
          DebugPrint(S.gBuffer, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("ao_render_group AoGroup {\n", Depth+2);
          DebugPrint(S.AoGroup, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("shadow_render_group SG {\n", Depth+2);
          DebugPrint(S.SG, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("post_processing_group PostGroup {\n", Depth+2);
          DebugPrint(S.PostGroup, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("gpu_mapped_element_buffer GpuBuffers {\n", Depth+2);
          DebugPrint(S.GpuBuffers, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("u32 GpuBufferWriteIndex {\n", Depth+2);
          DebugPrint(S.GpuBufferWriteIndex, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("memory_arena Memory {\n", Depth+2);
          DebugPrint(S.Memory, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( graphics *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( shader_uniform S, u32 Depth)
    {
      DebugPrint("shader_uniform {\n", Depth);
      
          DebugPrint("shader_uniform_type Type {\n", Depth+2);
          DebugPrint(S.Type, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("s32 ID {\n", Depth+2);
          DebugPrint(S.ID, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("char Name {\n", Depth+2);
          DebugPrint(S.Name, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("shader_uniform Next {\n", Depth+2);
          DebugPrint(S.Next, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( shader_uniform *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( shader S, u32 Depth)
    {
      DebugPrint("shader {\n", Depth);
      
          DebugPrint("u32 ID {\n", Depth+2);
          DebugPrint(S.ID, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("shader_uniform FirstUniform {\n", Depth+2);
          DebugPrint(S.FirstUniform, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( shader *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( texture S, u32 Depth)
    {
      DebugPrint("texture {\n", Depth);
      
          DebugPrint("u32 ID {\n", Depth+2);
          DebugPrint(S.ID, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("v2i Dim {\n", Depth+2);
          DebugPrint(S.Dim, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( texture *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( light S, u32 Depth)
    {
      DebugPrint("light {\n", Depth);
      
          DebugPrint("v3 Position {\n", Depth+2);
          DebugPrint(S.Position, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("v3 Color {\n", Depth+2);
          DebugPrint(S.Color, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( light *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( game_lights S, u32 Depth)
    {
      DebugPrint("game_lights {\n", Depth);
      
          DebugPrint("texture ColorTex {\n", Depth+2);
          DebugPrint(S.ColorTex, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("texture PositionTex {\n", Depth+2);
          DebugPrint(S.PositionTex, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("r32 IndexToUV {\n", Depth+2);
          DebugPrint(S.IndexToUV, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("s32 Count {\n", Depth+2);
          DebugPrint(S.Count, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("light Lights {\n", Depth+2);
          DebugPrint(S.Lights, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( game_lights *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( RenderBasis S, u32 Depth)
    {
      DebugPrint("RenderBasis {\n", Depth);
      
          DebugPrint("m4 ModelMatrix {\n", Depth+2);
          DebugPrint(S.ModelMatrix, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("m4 ViewMatrix {\n", Depth+2);
          DebugPrint(S.ViewMatrix, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("m4 ProjectionMatrix {\n", Depth+2);
          DebugPrint(S.ProjectionMatrix, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( RenderBasis *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( framebuffer S, u32 Depth)
    {
      DebugPrint("framebuffer {\n", Depth);
      
          DebugPrint("u32 ID {\n", Depth+2);
          DebugPrint(S.ID, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("u32 Attachments {\n", Depth+2);
          DebugPrint(S.Attachments, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( framebuffer *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( ao_render_group S, u32 Depth)
    {
      DebugPrint("ao_render_group {\n", Depth);
      
          DebugPrint("framebuffer FBO {\n", Depth+2);
          DebugPrint(S.FBO, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("shader Shader {\n", Depth+2);
          DebugPrint(S.Shader, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("shader DebugSsaoShader {\n", Depth+2);
          DebugPrint(S.DebugSsaoShader, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("v3 SsaoKernel {\n", Depth+2);
          DebugPrint(S.SsaoKernel, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("s32 SsaoKernelUniform {\n", Depth+2);
          DebugPrint(S.SsaoKernelUniform, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("texture Texture {\n", Depth+2);
          DebugPrint(S.Texture, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("v3 NoiseTile {\n", Depth+2);
          DebugPrint(S.NoiseTile, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( ao_render_group *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( g_buffer_textures S, u32 Depth)
    {
      DebugPrint("g_buffer_textures {\n", Depth);
      
          DebugPrint("texture Color {\n", Depth+2);
          DebugPrint(S.Color, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("texture Normal {\n", Depth+2);
          DebugPrint(S.Normal, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("texture Position {\n", Depth+2);
          DebugPrint(S.Position, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( g_buffer_textures *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( g_buffer_render_group S, u32 Depth)
    {
      DebugPrint("g_buffer_render_group {\n", Depth);
      
          DebugPrint("framebuffer FBO {\n", Depth+2);
          DebugPrint(S.FBO, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("g_buffer_textures Textures {\n", Depth+2);
          DebugPrint(S.Textures, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("shader LightingShader {\n", Depth+2);
          DebugPrint(S.LightingShader, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("shader gBufferShader {\n", Depth+2);
          DebugPrint(S.gBufferShader, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("m4 ShadowMVP {\n", Depth+2);
          DebugPrint(S.ShadowMVP, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("m4 ViewProjection {\n", Depth+2);
          DebugPrint(S.ViewProjection, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( g_buffer_render_group *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( shadow_render_group S, u32 Depth)
    {
      DebugPrint("shadow_render_group {\n", Depth);
      
          DebugPrint("u32 FramebufferName {\n", Depth+2);
          DebugPrint(S.FramebufferName, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("u32 MVP_ID {\n", Depth+2);
          DebugPrint(S.MVP_ID, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("shader DebugTextureShader {\n", Depth+2);
          DebugPrint(S.DebugTextureShader, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("shader DepthShader {\n", Depth+2);
          DebugPrint(S.DepthShader, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("texture ShadowMap {\n", Depth+2);
          DebugPrint(S.ShadowMap, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( shadow_render_group *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( untextured_3d_geometry_buffer S, u32 Depth)
    {
      DebugPrint("untextured_3d_geometry_buffer {\n", Depth);
      
          DebugPrint("v3 Verts {\n", Depth+2);
          DebugPrint(S.Verts, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("v4 Colors {\n", Depth+2);
          DebugPrint(S.Colors, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("v3 Normals {\n", Depth+2);
          DebugPrint(S.Normals, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("u32 End {\n", Depth+2);
          DebugPrint(S.End, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("u32 At {\n", Depth+2);
          DebugPrint(S.At, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( untextured_3d_geometry_buffer *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( gpu_mapped_element_buffer S, u32 Depth)
    {
      DebugPrint("gpu_mapped_element_buffer {\n", Depth);
      
          DebugPrint("u32 VertexHandle {\n", Depth+2);
          DebugPrint(S.VertexHandle, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("u32 NormalHandle {\n", Depth+2);
          DebugPrint(S.NormalHandle, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("u32 ColorHandle {\n", Depth+2);
          DebugPrint(S.ColorHandle, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("untextured_3d_geometry_buffer Buffer {\n", Depth+2);
          DebugPrint(S.Buffer, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( gpu_mapped_element_buffer *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( textured_2d_geometry_buffer S, u32 Depth)
    {
      DebugPrint("textured_2d_geometry_buffer {\n", Depth);
      
          DebugPrint("v3 Verts {\n", Depth+2);
          DebugPrint(S.Verts, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("v3 Colors {\n", Depth+2);
          DebugPrint(S.Colors, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("v3 UVs {\n", Depth+2);
          DebugPrint(S.UVs, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("u32 End {\n", Depth+2);
          DebugPrint(S.End, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("u32 At {\n", Depth+2);
          DebugPrint(S.At, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( textured_2d_geometry_buffer *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( untextured_2d_geometry_buffer S, u32 Depth)
    {
      DebugPrint("untextured_2d_geometry_buffer {\n", Depth);
      
          DebugPrint("v3 Verts {\n", Depth+2);
          DebugPrint(S.Verts, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("v3 Colors {\n", Depth+2);
          DebugPrint(S.Colors, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("u32 End {\n", Depth+2);
          DebugPrint(S.End, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("u32 At {\n", Depth+2);
          DebugPrint(S.At, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( untextured_2d_geometry_buffer *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( keyframe S, u32 Depth)
    {
      DebugPrint("keyframe {\n", Depth);
      
          DebugPrint("r32 tEnd {\n", Depth+2);
          DebugPrint(S.tEnd, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("r32 Value {\n", Depth+2);
          DebugPrint(S.Value, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( keyframe *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( animation S, u32 Depth)
    {
      DebugPrint("animation {\n", Depth);
      
          DebugPrint("r32 t {\n", Depth+2);
          DebugPrint(S.t, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("r32 tEnd {\n", Depth+2);
          DebugPrint(S.tEnd, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("u32 xKeyframeCount {\n", Depth+2);
          DebugPrint(S.xKeyframeCount, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("keyframe xKeyframes {\n", Depth+2);
          DebugPrint(S.xKeyframes, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("u32 yKeyframeCount {\n", Depth+2);
          DebugPrint(S.yKeyframeCount, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("keyframe yKeyframes {\n", Depth+2);
          DebugPrint(S.yKeyframes, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("u32 zKeyframeCount {\n", Depth+2);
          DebugPrint(S.zKeyframeCount, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("keyframe zKeyframes {\n", Depth+2);
          DebugPrint(S.zKeyframes, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( animation *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( game_mode S, u32 Depth)
    {
      DebugPrint("game_mode {\n", Depth);
      
          DebugPrint("game_mode_type ActiveMode {\n", Depth+2);
          DebugPrint(S.ActiveMode, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("r64 TimeRunning {\n", Depth+2);
          DebugPrint(S.TimeRunning, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( game_mode *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( voxel S, u32 Depth)
    {
      DebugPrint("voxel {\n", Depth);
      
          DebugPrint("voxel_flag Flags {\n", Depth+2);
          DebugPrint(S.Flags, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("u8 Color {\n", Depth+2);
          DebugPrint(S.Color, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( voxel *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( boundary_voxel S, u32 Depth)
    {
      DebugPrint("boundary_voxel {\n", Depth);
      
          DebugPrint("voxel V {\n", Depth+2);
          DebugPrint(S.V, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("voxel_position Offset {\n", Depth+2);
          DebugPrint(S.Offset, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( boundary_voxel *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( chunk_data S, u32 Depth)
    {
      DebugPrint("chunk_data {\n", Depth);
      
          DebugPrint("chunk_flag Flags {\n", Depth+2);
          DebugPrint(S.Flags, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("voxel Voxels {\n", Depth+2);
          DebugPrint(S.Voxels, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("u8 Pad {\n", Depth+2);
          DebugPrint(S.Pad, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( chunk_data *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( model S, u32 Depth)
    {
      DebugPrint("model {\n", Depth);
      
          DebugPrint("untextured_3d_geometry_buffer Mesh {\n", Depth+2);
          DebugPrint(S.Mesh, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("chunk_dimension Dim {\n", Depth+2);
          DebugPrint(S.Dim, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("animation Animation {\n", Depth+2);
          DebugPrint(S.Animation, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( model *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( physics S, u32 Depth)
    {
      DebugPrint("physics {\n", Depth);
      
          DebugPrint("v3 Velocity {\n", Depth+2);
          DebugPrint(S.Velocity, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("v3 Force {\n", Depth+2);
          DebugPrint(S.Force, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("v3 Delta {\n", Depth+2);
          DebugPrint(S.Delta, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("r32 Drag {\n", Depth+2);
          DebugPrint(S.Drag, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("r32 Mass {\n", Depth+2);
          DebugPrint(S.Mass, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("r32 Speed {\n", Depth+2);
          DebugPrint(S.Speed, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( physics *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( particle S, u32 Depth)
    {
      DebugPrint("particle {\n", Depth);
      
          DebugPrint("v3 Offset {\n", Depth+2);
          DebugPrint(S.Offset, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("physics Physics {\n", Depth+2);
          DebugPrint(S.Physics, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("u8 Color {\n", Depth+2);
          DebugPrint(S.Color, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("r32 RemainingLifespan {\n", Depth+2);
          DebugPrint(S.RemainingLifespan, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( particle *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( particle_system_init_params S, u32 Depth)
    {
      DebugPrint("particle_system_init_params {\n", Depth);
      
          DebugPrint("aabb SpawnRegion {\n", Depth+2);
          DebugPrint(S.SpawnRegion, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("physics Physics {\n", Depth+2);
          DebugPrint(S.Physics, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("r32 SystemMovementCoefficient {\n", Depth+2);
          DebugPrint(S.SystemMovementCoefficient, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("r32 EmissionLifespan {\n", Depth+2);
          DebugPrint(S.EmissionLifespan, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("r32 EmissionChance {\n", Depth+2);
          DebugPrint(S.EmissionChance, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("r32 ParticleLifespan {\n", Depth+2);
          DebugPrint(S.ParticleLifespan, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("v3 StartingDiameter {\n", Depth+2);
          DebugPrint(S.StartingDiameter, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("random_series Entropy {\n", Depth+2);
          DebugPrint(S.Entropy, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("u8 Colors {\n", Depth+2);
          DebugPrint(S.Colors, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( particle_system_init_params *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( particle_system S, u32 Depth)
    {
      DebugPrint("particle_system {\n", Depth);
      
          DebugPrint("random_series Entropy {\n", Depth+2);
          DebugPrint(S.Entropy, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("s32 ActiveParticles {\n", Depth+2);
          DebugPrint(S.ActiveParticles, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("r32 ParticleDuration {\n", Depth+2);
          DebugPrint(S.ParticleDuration, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("aabb SpawnRegion {\n", Depth+2);
          DebugPrint(S.SpawnRegion, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("v3 StartingDiameter {\n", Depth+2);
          DebugPrint(S.StartingDiameter, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("r32 EmissionLifespan {\n", Depth+2);
          DebugPrint(S.EmissionLifespan, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("r32 ParticleLifespan {\n", Depth+2);
          DebugPrint(S.ParticleLifespan, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("r32 EmissionChance {\n", Depth+2);
          DebugPrint(S.EmissionChance, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("physics ParticlePhysics {\n", Depth+2);
          DebugPrint(S.ParticlePhysics, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("r32 SystemMovementCoefficient {\n", Depth+2);
          DebugPrint(S.SystemMovementCoefficient, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("u8 Colors {\n", Depth+2);
          DebugPrint(S.Colors, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("particle Particles {\n", Depth+2);
          DebugPrint(S.Particles, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( particle_system *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( entity S, u32 Depth)
    {
      DebugPrint("entity {\n", Depth);
      
          DebugPrint("model Model {\n", Depth+2);
          DebugPrint(S.Model, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("v3 CollisionVolumeRadius {\n", Depth+2);
          DebugPrint(S.CollisionVolumeRadius, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("particle_system Emitter {\n", Depth+2);
          DebugPrint(S.Emitter, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("physics Physics {\n", Depth+2);
          DebugPrint(S.Physics, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("canonical_position P {\n", Depth+2);
          DebugPrint(S.P, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("Quaternion Rotation {\n", Depth+2);
          DebugPrint(S.Rotation, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("entity_state State {\n", Depth+2);
          DebugPrint(S.State, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("entity_type Type {\n", Depth+2);
          DebugPrint(S.Type, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("r32 Scale {\n", Depth+2);
          DebugPrint(S.Scale, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("r32 RateOfFire {\n", Depth+2);
          DebugPrint(S.RateOfFire, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("r32 FireCooldown {\n", Depth+2);
          DebugPrint(S.FireCooldown, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("s32 Health {\n", Depth+2);
          DebugPrint(S.Health, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( entity *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( frame_event S, u32 Depth)
    {
      DebugPrint("frame_event {\n", Depth);
      
          DebugPrint("frame_event_type Type {\n", Depth+2);
          DebugPrint(S.Type, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("entity Entity {\n", Depth+2);
          DebugPrint(S.Entity, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("frame_event Next {\n", Depth+2);
          DebugPrint(S.Next, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( frame_event *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( event_queue S, u32 Depth)
    {
      DebugPrint("event_queue {\n", Depth);
      
          DebugPrint("u64 CurrentFrameIndex {\n", Depth+2);
          DebugPrint(S.CurrentFrameIndex, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("frame_event Queue {\n", Depth+2);
          DebugPrint(S.Queue, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("frame_event FirstFreeEvent {\n", Depth+2);
          DebugPrint(S.FirstFreeEvent, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( event_queue *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( entity_list S, u32 Depth)
    {
      DebugPrint("entity_list {\n", Depth);
      
          DebugPrint("entity This {\n", Depth+2);
          DebugPrint(S.This, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("entity Next {\n", Depth+2);
          DebugPrint(S.Next, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( entity_list *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( point_buffer S, u32 Depth)
    {
      DebugPrint("point_buffer {\n", Depth);
      
          DebugPrint("s32 Count {\n", Depth+2);
          DebugPrint(S.Count, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("voxel_position Points {\n", Depth+2);
          DebugPrint(S.Points, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("voxel_position Min {\n", Depth+2);
          DebugPrint(S.Min, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("voxel_position Max {\n", Depth+2);
          DebugPrint(S.Max, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( point_buffer *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( world_chunk S, u32 Depth)
    {
      DebugPrint("world_chunk {\n", Depth);
      
          DebugPrint("chunk_data Data {\n", Depth+2);
          DebugPrint(S.Data, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("untextured_3d_geometry_buffer Mesh {\n", Depth+2);
          DebugPrint(S.Mesh, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("untextured_3d_geometry_buffer LodMesh {\n", Depth+2);
          DebugPrint(S.LodMesh, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("world_position WorldP {\n", Depth+2);
          DebugPrint(S.WorldP, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("u32 FilledCount {\n", Depth+2);
          DebugPrint(S.FilledCount, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("b32 Picked {\n", Depth+2);
          DebugPrint(S.Picked, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("b32 LodMesh_Complete {\n", Depth+2);
          DebugPrint(S.LodMesh_Complete, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("b32 DrawBoundingVoxels {\n", Depth+2);
          DebugPrint(S.DrawBoundingVoxels, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("s32 PointsToLeaveRemaining {\n", Depth+2);
          DebugPrint(S.PointsToLeaveRemaining, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("u32 TriCount {\n", Depth+2);
          DebugPrint(S.TriCount, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("s32 EdgeBoundaryVoxelCount {\n", Depth+2);
          DebugPrint(S.EdgeBoundaryVoxelCount, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("u8 DimX {\n", Depth+2);
          DebugPrint(S.DimX, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("u8 DimY {\n", Depth+2);
          DebugPrint(S.DimY, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("u8 DimZ {\n", Depth+2);
          DebugPrint(S.DimZ, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("u8 Pad {\n", Depth+2);
          DebugPrint(S.Pad, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( world_chunk *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( collision_event S, u32 Depth)
    {
      DebugPrint("collision_event {\n", Depth);
      
          DebugPrint("world_chunk Chunk {\n", Depth+2);
          DebugPrint(S.Chunk, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("canonical_position CP {\n", Depth+2);
          DebugPrint(S.CP, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("bool didCollide {\n", Depth+2);
          DebugPrint(S.didCollide, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( collision_event *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( free_world_chunk S, u32 Depth)
    {
      DebugPrint("free_world_chunk {\n", Depth);
      
          DebugPrint("world_chunk chunk_data {\n", Depth+2);
          DebugPrint(S.chunk_data, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("world_chunk Next {\n", Depth+2);
          DebugPrint(S.Next, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( free_world_chunk *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( world S, u32 Depth)
    {
      DebugPrint("world {\n", Depth);
      
          DebugPrint("world_chunk ChunkHash {\n", Depth+2);
          DebugPrint(S.ChunkHash, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("world_chunk FreeChunks {\n", Depth+2);
          DebugPrint(S.FreeChunks, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("int FreeChunkCount {\n", Depth+2);
          DebugPrint(S.FreeChunkCount, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("chunk_dimension VisibleRegion {\n", Depth+2);
          DebugPrint(S.VisibleRegion, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("chunk_dimension ChunkDim {\n", Depth+2);
          DebugPrint(S.ChunkDim, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("world_position Center {\n", Depth+2);
          DebugPrint(S.Center, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("memory_arena Memory {\n", Depth+2);
          DebugPrint(S.Memory, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( world *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( thread S, u32 Depth)
    {
      DebugPrint("thread {\n", Depth);
      
          DebugPrint("u32 ThreadIndex {\n", Depth+2);
          DebugPrint(S.ThreadIndex, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("thread_id ID {\n", Depth+2);
          DebugPrint(S.ID, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( thread *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( thread_local_state S, u32 Depth)
    {
      DebugPrint("thread_local_state {\n", Depth);
      
          DebugPrint("memory_arena PermMemory {\n", Depth+2);
          DebugPrint(S.PermMemory, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("memory_arena TempMemory {\n", Depth+2);
          DebugPrint(S.TempMemory, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("mesh_freelist MeshFreelist {\n", Depth+2);
          DebugPrint(S.MeshFreelist, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("perlin_noise Noise {\n", Depth+2);
          DebugPrint(S.Noise, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( thread_local_state *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( u32_stream S, u32 Depth)
    {
      DebugPrint("u32_stream {\n", Depth);
      
          DebugPrint("u32 Start {\n", Depth+2);
          DebugPrint(S.Start, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("u32 At {\n", Depth+2);
          DebugPrint(S.At, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("u32 End {\n", Depth+2);
          DebugPrint(S.End, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( u32_stream *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( u8_stream S, u32 Depth)
    {
      DebugPrint("u8_stream {\n", Depth);
      
          DebugPrint("u8 Start {\n", Depth+2);
          DebugPrint(S.Start, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("u8 At {\n", Depth+2);
          DebugPrint(S.At, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("u8 End {\n", Depth+2);
          DebugPrint(S.End, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( u8_stream *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( ansi_stream S, u32 Depth)
    {
      DebugPrint("ansi_stream {\n", Depth);
      
          DebugPrint("char Start {\n", Depth+2);
          DebugPrint(S.Start, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("char At {\n", Depth+2);
          DebugPrint(S.At, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("char End {\n", Depth+2);
          DebugPrint(S.End, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("counted_string Filename {\n", Depth+2);
          DebugPrint(S.Filename, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( ansi_stream *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( r32_stream S, u32 Depth)
    {
      DebugPrint("r32_stream {\n", Depth);
      
          DebugPrint("r32 Start {\n", Depth+2);
          DebugPrint(S.Start, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("r32 At {\n", Depth+2);
          DebugPrint(S.At, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("r32 End {\n", Depth+2);
          DebugPrint(S.End, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( r32_stream *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( mesh_metadata S, u32 Depth)
    {
      DebugPrint("mesh_metadata {\n", Depth);
      
          DebugPrint("u32 VertCount {\n", Depth+2);
          DebugPrint(S.VertCount, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("u32 NormalCount {\n", Depth+2);
          DebugPrint(S.NormalCount, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("u32 UVCount {\n", Depth+2);
          DebugPrint(S.UVCount, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("u32 FaceCount {\n", Depth+2);
          DebugPrint(S.FaceCount, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( mesh_metadata *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( rect2 S, u32 Depth)
    {
      DebugPrint("rect2 {\n", Depth);
      
          DebugPrint("v2 Min {\n", Depth+2);
          DebugPrint(S.Min, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("v2 Max {\n", Depth+2);
          DebugPrint(S.Max, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( rect2 *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( aabb S, u32 Depth)
    {
      DebugPrint("aabb {\n", Depth);
      
          DebugPrint("v3 Center {\n", Depth+2);
          DebugPrint(S.Center, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("v3 Radius {\n", Depth+2);
          DebugPrint(S.Radius, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( aabb *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( ray S, u32 Depth)
    {
      DebugPrint("ray {\n", Depth);
      
          DebugPrint("v3 Origin {\n", Depth+2);
          DebugPrint(S.Origin, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("v3 Dir {\n", Depth+2);
          DebugPrint(S.Dir, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( ray *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( input_event S, u32 Depth)
    {
      DebugPrint("input_event {\n", Depth);
      
          DebugPrint("b32 Clicked {\n", Depth+2);
          DebugPrint(S.Clicked, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("b32 Pressed {\n", Depth+2);
          DebugPrint(S.Pressed, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( input_event *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( input S, u32 Depth)
    {
      DebugPrint("input {\n", Depth);
      
          DebugPrint("input_event Escape {\n", Depth+2);
          DebugPrint(S.Escape, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("input_event Space {\n", Depth+2);
          DebugPrint(S.Space, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("input_event Shift {\n", Depth+2);
          DebugPrint(S.Shift, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("input_event Ctrl {\n", Depth+2);
          DebugPrint(S.Ctrl, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("input_event Alt {\n", Depth+2);
          DebugPrint(S.Alt, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("input_event F12 {\n", Depth+2);
          DebugPrint(S.F12, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("input_event F11 {\n", Depth+2);
          DebugPrint(S.F11, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("input_event F10 {\n", Depth+2);
          DebugPrint(S.F10, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("input_event F9 {\n", Depth+2);
          DebugPrint(S.F9, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("input_event F8 {\n", Depth+2);
          DebugPrint(S.F8, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("input_event F7 {\n", Depth+2);
          DebugPrint(S.F7, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("input_event F6 {\n", Depth+2);
          DebugPrint(S.F6, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("input_event F5 {\n", Depth+2);
          DebugPrint(S.F5, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("input_event F4 {\n", Depth+2);
          DebugPrint(S.F4, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("input_event F3 {\n", Depth+2);
          DebugPrint(S.F3, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("input_event F2 {\n", Depth+2);
          DebugPrint(S.F2, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("input_event F1 {\n", Depth+2);
          DebugPrint(S.F1, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("input_event RMB {\n", Depth+2);
          DebugPrint(S.RMB, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("input_event LMB {\n", Depth+2);
          DebugPrint(S.LMB, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("input_event MMB {\n", Depth+2);
          DebugPrint(S.MMB, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("input_event W {\n", Depth+2);
          DebugPrint(S.W, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("input_event A {\n", Depth+2);
          DebugPrint(S.A, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("input_event S {\n", Depth+2);
          DebugPrint(S.S, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("input_event D {\n", Depth+2);
          DebugPrint(S.D, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("input_event Q {\n", Depth+2);
          DebugPrint(S.Q, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("input_event E {\n", Depth+2);
          DebugPrint(S.E, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( input *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( hotkeys S, u32 Depth)
    {
      DebugPrint("hotkeys {\n", Depth);
      
          DebugPrint("b32 Debug_RedrawEveryPush {\n", Depth+2);
          DebugPrint(S.Debug_RedrawEveryPush, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("b32 Debug_ToggleLoopedGamePlayback {\n", Depth+2);
          DebugPrint(S.Debug_ToggleLoopedGamePlayback, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("b32 Debug_ToggleTriggeredRuntimeBreak {\n", Depth+2);
          DebugPrint(S.Debug_ToggleTriggeredRuntimeBreak, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("b32 Debug_Pause {\n", Depth+2);
          DebugPrint(S.Debug_Pause, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("b32 Debug_ToggleProfile {\n", Depth+2);
          DebugPrint(S.Debug_ToggleProfile, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("b32 Debug_ToggleMenu {\n", Depth+2);
          DebugPrint(S.Debug_ToggleMenu, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("b32 Debug_TriangulateIncrement {\n", Depth+2);
          DebugPrint(S.Debug_TriangulateIncrement, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("b32 Debug_TriangulateDecrement {\n", Depth+2);
          DebugPrint(S.Debug_TriangulateDecrement, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("b32 Debug_MousePick {\n", Depth+2);
          DebugPrint(S.Debug_MousePick, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("b32 Left {\n", Depth+2);
          DebugPrint(S.Left, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("b32 Right {\n", Depth+2);
          DebugPrint(S.Right, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("b32 Forward {\n", Depth+2);
          DebugPrint(S.Forward, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("b32 Backward {\n", Depth+2);
          DebugPrint(S.Backward, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("b32 Player_Fire {\n", Depth+2);
          DebugPrint(S.Player_Fire, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("b32 Player_Proton {\n", Depth+2);
          DebugPrint(S.Player_Proton, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("b32 Player_Spawn {\n", Depth+2);
          DebugPrint(S.Player_Spawn, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( hotkeys *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( work_queue S, u32 Depth)
    {
      DebugPrint("work_queue {\n", Depth);
      
          DebugPrint("u32 EnqueueIndex {\n", Depth+2);
          DebugPrint(S.EnqueueIndex, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("u32 DequeueIndex {\n", Depth+2);
          DebugPrint(S.DequeueIndex, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("work_queue_entry Entries {\n", Depth+2);
          DebugPrint(S.Entries, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("semaphore GlobalQueueSemaphore {\n", Depth+2);
          DebugPrint(S.GlobalQueueSemaphore, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( work_queue *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( string_builder S, u32 Depth)
    {
      DebugPrint("string_builder {\n", Depth);
      
          DebugPrint("memory_arena Memory {\n", Depth+2);
          DebugPrint(S.Memory, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("counted_string_stream Chunks {\n", Depth+2);
          DebugPrint(S.Chunks, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( string_builder *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( platform S, u32 Depth)
    {
      DebugPrint("platform {\n", Depth);
      
          DebugPrint("work_queue LowPriority {\n", Depth+2);
          DebugPrint(S.LowPriority, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("work_queue HighPriority {\n", Depth+2);
          DebugPrint(S.HighPriority, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("semaphore QueueSemaphore {\n", Depth+2);
          DebugPrint(S.QueueSemaphore, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("thread_startup_params Threads {\n", Depth+2);
          DebugPrint(S.Threads, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("v2 MouseP {\n", Depth+2);
          DebugPrint(S.MouseP, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("v2 MouseDP {\n", Depth+2);
          DebugPrint(S.MouseDP, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("memory_arena Memory {\n", Depth+2);
          DebugPrint(S.Memory, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("r32 dt {\n", Depth+2);
          DebugPrint(S.dt, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("s32 WindowWidth {\n", Depth+2);
          DebugPrint(S.WindowWidth, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("s32 WindowHeight {\n", Depth+2);
          DebugPrint(S.WindowHeight, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("input Input {\n", Depth+2);
          DebugPrint(S.Input, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( platform *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( m4 S, u32 Depth)
    {
      DebugPrint("m4 {\n", Depth);
      
          DebugPrint("v4 E {\n", Depth+2);
          DebugPrint(S.E, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( m4 *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( m_nxn S, u32 Depth)
    {
      DebugPrint("m_nxn {\n", Depth);
      
          DebugPrint("r32 Elements {\n", Depth+2);
          DebugPrint(S.Elements, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("u32 Rows {\n", Depth+2);
          DebugPrint(S.Rows, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("u32 Columns {\n", Depth+2);
          DebugPrint(S.Columns, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("u32 At {\n", Depth+2);
          DebugPrint(S.At, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("u32 End {\n", Depth+2);
          DebugPrint(S.End, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( m_nxn *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( random_series S, u32 Depth)
    {
      DebugPrint("random_series {\n", Depth);
      
          DebugPrint("u64 Seed {\n", Depth+2);
          DebugPrint(S.Seed, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( random_series *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( memory_arena S, u32 Depth)
    {
      DebugPrint("memory_arena {\n", Depth);
      
          DebugPrint("u8 Start {\n", Depth+2);
          DebugPrint(S.Start, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("u8 At {\n", Depth+2);
          DebugPrint(S.At, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("u8 End {\n", Depth+2);
          DebugPrint(S.End, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("umm NextBlockSize {\n", Depth+2);
          DebugPrint(S.NextBlockSize, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("memory_arena Prev {\n", Depth+2);
          DebugPrint(S.Prev, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("umm Pushes {\n", Depth+2);
          DebugPrint(S.Pushes, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( memory_arena *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( push_metadata S, u32 Depth)
    {
      DebugPrint("push_metadata {\n", Depth);
      
          DebugPrint("char Name {\n", Depth+2);
          DebugPrint(S.Name, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("umm ArenaHash {\n", Depth+2);
          DebugPrint(S.ArenaHash, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("umm HeadArenaHash {\n", Depth+2);
          DebugPrint(S.HeadArenaHash, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("umm StructSize {\n", Depth+2);
          DebugPrint(S.StructSize, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("umm StructCount {\n", Depth+2);
          DebugPrint(S.StructCount, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("u32 PushCount {\n", Depth+2);
          DebugPrint(S.PushCount, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( push_metadata *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( v2i S, u32 Depth)
    {
      DebugPrint("v2i {\n", Depth);
      
          DebugPrint("s32 x {\n", Depth+2);
          DebugPrint(S.x, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("s32 y {\n", Depth+2);
          DebugPrint(S.y, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( v2i *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( v2 S, u32 Depth)
    {
      DebugPrint("v2 {\n", Depth);
      
          DebugPrint("r32 x {\n", Depth+2);
          DebugPrint(S.x, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("r32 y {\n", Depth+2);
          DebugPrint(S.y, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( v2 *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( v3i S, u32 Depth)
    {
      DebugPrint("v3i {\n", Depth);
      
          DebugPrint("s32 E {\n", Depth+2);
          DebugPrint(S.E, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( v3i *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( v3 S, u32 Depth)
    {
      DebugPrint("v3 {\n", Depth);
      
          DebugPrint("r32 E {\n", Depth+2);
          DebugPrint(S.E, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( v3 *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( v4 S, u32 Depth)
    {
      DebugPrint("v4 {\n", Depth);
      
          DebugPrint("r32 E {\n", Depth+2);
          DebugPrint(S.E, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( v4 *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( voxel_position S, u32 Depth)
    {
      DebugPrint("voxel_position {\n", Depth);
      
          DebugPrint("s32 E {\n", Depth+2);
          DebugPrint(S.E, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( voxel_position *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( f32_reg S, u32 Depth)
    {
      DebugPrint("f32_reg {\n", Depth);
      
          DebugPrint("r32 F {\n", Depth+2);
          DebugPrint(S.F, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("__m128 Sse {\n", Depth+2);
          DebugPrint(S.Sse, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( f32_reg *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( heap_allocation_block S, u32 Depth)
    {
      DebugPrint("heap_allocation_block {\n", Depth);
      
          DebugPrint("heap_allocation_type Type {\n", Depth+2);
          DebugPrint(S.Type, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("umm Size {\n", Depth+2);
          DebugPrint(S.Size, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("umm PrevAllocationSize {\n", Depth+2);
          DebugPrint(S.PrevAllocationSize, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( heap_allocation_block *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( heap_allocator S, u32 Depth)
    {
      DebugPrint("heap_allocator {\n", Depth);
      
          DebugPrint("heap_allocation_block Heap {\n", Depth+2);
          DebugPrint(S.Heap, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("umm Size {\n", Depth+2);
          DebugPrint(S.Size, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( heap_allocator *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( Quaternion S, u32 Depth)
    {
      DebugPrint("Quaternion {\n", Depth);
      
          DebugPrint("float E {\n", Depth+2);
          DebugPrint(S.E, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( Quaternion *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( mutex S, u32 Depth)
    {
      DebugPrint("mutex {\n", Depth);
      
          DebugPrint("native_mutex M {\n", Depth+2);
          DebugPrint(S.M, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("mutex_op_record DebugRecord {\n", Depth+2);
          DebugPrint(S.DebugRecord, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( mutex *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( mutex_op_record S, u32 Depth)
    {
      DebugPrint("mutex_op_record {\n", Depth);
      
          DebugPrint("mutex Mutex {\n", Depth+2);
          DebugPrint(S.Mutex, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("u64 Cycle {\n", Depth+2);
          DebugPrint(S.Cycle, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("mutex_op Op {\n", Depth+2);
          DebugPrint(S.Op, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( mutex_op_record *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( mutex_op_array S, u32 Depth)
    {
      DebugPrint("mutex_op_array {\n", Depth);
      
          DebugPrint("u32 NextRecord {\n", Depth+2);
          DebugPrint(S.NextRecord, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("mutex_op_record Records {\n", Depth+2);
          DebugPrint(S.Records, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( mutex_op_array *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( tagged_counted_string_stream S, u32 Depth)
    {
      DebugPrint("tagged_counted_string_stream {\n", Depth);
      
          DebugPrint("counted_string Tag {\n", Depth+2);
          DebugPrint(S.Tag, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("counted_string_stream Stream {\n", Depth+2);
          DebugPrint(S.Stream, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( tagged_counted_string_stream *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( char_cursor S, u32 Depth)
    {
      DebugPrint("char_cursor {\n", Depth);
      
          DebugPrint("char Start {\n", Depth+2);
          DebugPrint(S.Start, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("char End {\n", Depth+2);
          DebugPrint(S.End, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("char At {\n", Depth+2);
          DebugPrint(S.At, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("memory_arena Memory {\n", Depth+2);
          DebugPrint(S.Memory, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( char_cursor *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( linei S, u32 Depth)
    {
      DebugPrint("linei {\n", Depth);
      
          DebugPrint("v3i MinP {\n", Depth+2);
          DebugPrint(S.MinP, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("v3i MaxP {\n", Depth+2);
          DebugPrint(S.MaxP, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( linei *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( line S, u32 Depth)
    {
      DebugPrint("line {\n", Depth);
      
          DebugPrint("v3 MinP {\n", Depth+2);
          DebugPrint(S.MinP, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("v3 MaxP {\n", Depth+2);
          DebugPrint(S.MaxP, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( line *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( XVisualInfo S, u32 Depth)
    {
      DebugPrint("XVisualInfo {\n", Depth);
      
          DebugPrint("Visual visual {\n", Depth+2);
          DebugPrint(S.visual, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("VisualID visualid {\n", Depth+2);
          DebugPrint(S.visualid, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("s32 screen {\n", Depth+2);
          DebugPrint(S.screen, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("s32 depth {\n", Depth+2);
          DebugPrint(S.depth, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("s32 c_class {\n", Depth+2);
          DebugPrint(S.c_class, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("long red_mask {\n", Depth+2);
          DebugPrint(S.red_mask, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("long green_mask {\n", Depth+2);
          DebugPrint(S.green_mask, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("long blue_mask {\n", Depth+2);
          DebugPrint(S.blue_mask, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("s32 colormap_size {\n", Depth+2);
          DebugPrint(S.colormap_size, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("s32 bits_per_rgb {\n", Depth+2);
          DebugPrint(S.bits_per_rgb, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( XVisualInfo *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( os S, u32 Depth)
    {
      DebugPrint("os {\n", Depth);
      
          DebugPrint("window Window {\n", Depth+2);
          DebugPrint(S.Window, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("display Display {\n", Depth+2);
          DebugPrint(S.Display, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("gl_context GlContext {\n", Depth+2);
          DebugPrint(S.GlContext, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("b32 ContinueRunning {\n", Depth+2);
          DebugPrint(S.ContinueRunning, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( os *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( native_file S, u32 Depth)
    {
      DebugPrint("native_file {\n", Depth);
      
          DebugPrint("FILE Handle {\n", Depth+2);
          DebugPrint(S.Handle, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("counted_string Path {\n", Depth+2);
          DebugPrint(S.Path, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( native_file *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( plane_computation S, u32 Depth)
    {
      DebugPrint("plane_computation {\n", Depth);
      
          DebugPrint("plane Plane {\n", Depth+2);
          DebugPrint(S.Plane, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("b32 Complete {\n", Depth+2);
          DebugPrint(S.Complete, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( plane_computation *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( bitmap_image_header S, u32 Depth)
    {
      DebugPrint("bitmap_image_header {\n", Depth);
      
          DebugPrint("u32 SizeOfImageHeader {\n", Depth+2);
          DebugPrint(S.SizeOfImageHeader, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("s32 WidthInPixels {\n", Depth+2);
          DebugPrint(S.WidthInPixels, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("s32 HeightInPixels {\n", Depth+2);
          DebugPrint(S.HeightInPixels, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("u16 ColorPlanes {\n", Depth+2);
          DebugPrint(S.ColorPlanes, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("u16 BPP {\n", Depth+2);
          DebugPrint(S.BPP, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("u32 CompressionType {\n", Depth+2);
          DebugPrint(S.CompressionType, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("u32 SizeInBytes {\n", Depth+2);
          DebugPrint(S.SizeInBytes, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("u32 xPixelsPerMeter {\n", Depth+2);
          DebugPrint(S.xPixelsPerMeter, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("u32 yPixelsPerMeter {\n", Depth+2);
          DebugPrint(S.yPixelsPerMeter, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("u32 ColorMapsUsed {\n", Depth+2);
          DebugPrint(S.ColorMapsUsed, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("u32 NumSignificantColors {\n", Depth+2);
          DebugPrint(S.NumSignificantColors, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("u32 RedMask {\n", Depth+2);
          DebugPrint(S.RedMask, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("u32 GreenMask {\n", Depth+2);
          DebugPrint(S.GreenMask, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("u32 BlueMask {\n", Depth+2);
          DebugPrint(S.BlueMask, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("u32 AlphaMask {\n", Depth+2);
          DebugPrint(S.AlphaMask, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( bitmap_image_header *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( bitmap_header S, u32 Depth)
    {
      DebugPrint("bitmap_header {\n", Depth);
      
          DebugPrint("u16 Type {\n", Depth+2);
          DebugPrint(S.Type, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("u32 FileSizeInBytes {\n", Depth+2);
          DebugPrint(S.FileSizeInBytes, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("u32 Ignored {\n", Depth+2);
          DebugPrint(S.Ignored, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("u32 OffsetToPixelData {\n", Depth+2);
          DebugPrint(S.OffsetToPixelData, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("bitmap_image_header Image {\n", Depth+2);
          DebugPrint(S.Image, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( bitmap_header *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( bitmap S, u32 Depth)
    {
      DebugPrint("bitmap {\n", Depth);
      
          DebugPrint("v2i Dim {\n", Depth+2);
          DebugPrint(S.Dim, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("u32_stream Pixels {\n", Depth+2);
          DebugPrint(S.Pixels, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( bitmap *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( ui_render_command S, u32 Depth)
    {
      DebugPrint("ui_render_command {\n", Depth);
      
          DebugPrint("ui_render_command_type Type {\n", Depth+2);
          DebugPrint(S.Type, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( ui_render_command *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( struct_member_function S, u32 Depth)
    {
      DebugPrint("struct_member_function {\n", Depth);
      
          DebugPrint("struct_member_function_type Type {\n", Depth+2);
          DebugPrint(S.Type, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( struct_member_function *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( struct_def_cursor S, u32 Depth)
    {
      DebugPrint("struct_def_cursor {\n", Depth);
      
          DebugPrint("struct_def Start {\n", Depth+2);
          DebugPrint(S.Start, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("struct_def End {\n", Depth+2);
          DebugPrint(S.End, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("struct_def At {\n", Depth+2);
          DebugPrint(S.At, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( struct_def_cursor *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( struct_def_stream_chunk S, u32 Depth)
    {
      DebugPrint("struct_def_stream_chunk {\n", Depth);
      
          DebugPrint("struct_def Element {\n", Depth+2);
          DebugPrint(S.Element, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("struct_def_stream_chunk Next {\n", Depth+2);
          DebugPrint(S.Next, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( struct_def_stream_chunk *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( struct_def_stream S, u32 Depth)
    {
      DebugPrint("struct_def_stream {\n", Depth);
      
          DebugPrint("struct_def_stream_chunk FirstChunk {\n", Depth+2);
          DebugPrint(S.FirstChunk, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("struct_def_stream_chunk LastChunk {\n", Depth+2);
          DebugPrint(S.LastChunk, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( struct_def_stream *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( struct_def_iterator S, u32 Depth)
    {
      DebugPrint("struct_def_iterator {\n", Depth);
      
          DebugPrint("struct_def_stream Stream {\n", Depth+2);
          DebugPrint(S.Stream, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("struct_def_stream_chunk At {\n", Depth+2);
          DebugPrint(S.At, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( struct_def_iterator *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( variable_stream_chunk S, u32 Depth)
    {
      DebugPrint("variable_stream_chunk {\n", Depth);
      
          DebugPrint("variable Element {\n", Depth+2);
          DebugPrint(S.Element, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("variable_stream_chunk Next {\n", Depth+2);
          DebugPrint(S.Next, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( variable_stream_chunk *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( variable_stream S, u32 Depth)
    {
      DebugPrint("variable_stream {\n", Depth);
      
          DebugPrint("variable_stream_chunk FirstChunk {\n", Depth+2);
          DebugPrint(S.FirstChunk, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("variable_stream_chunk LastChunk {\n", Depth+2);
          DebugPrint(S.LastChunk, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( variable_stream *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( variable_iterator S, u32 Depth)
    {
      DebugPrint("variable_iterator {\n", Depth);
      
          DebugPrint("variable_stream Stream {\n", Depth+2);
          DebugPrint(S.Stream, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("variable_stream_chunk At {\n", Depth+2);
          DebugPrint(S.At, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( variable_iterator *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( struct_member S, u32 Depth)
    {
      DebugPrint("struct_member {\n", Depth);
      
          DebugPrint("struct_member_type Type {\n", Depth+2);
          DebugPrint(S.Type, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( struct_member *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( struct_member_cursor S, u32 Depth)
    {
      DebugPrint("struct_member_cursor {\n", Depth);
      
          DebugPrint("struct_member Start {\n", Depth+2);
          DebugPrint(S.Start, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("struct_member End {\n", Depth+2);
          DebugPrint(S.End, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("struct_member At {\n", Depth+2);
          DebugPrint(S.At, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( struct_member_cursor *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( struct_member_stream_chunk S, u32 Depth)
    {
      DebugPrint("struct_member_stream_chunk {\n", Depth);
      
          DebugPrint("struct_member Element {\n", Depth+2);
          DebugPrint(S.Element, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("struct_member_stream_chunk Next {\n", Depth+2);
          DebugPrint(S.Next, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( struct_member_stream_chunk *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( enum_member_stream_chunk S, u32 Depth)
    {
      DebugPrint("enum_member_stream_chunk {\n", Depth);
      
          DebugPrint("enum_member Element {\n", Depth+2);
          DebugPrint(S.Element, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("enum_member_stream_chunk Next {\n", Depth+2);
          DebugPrint(S.Next, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( enum_member_stream_chunk *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( enum_member_stream S, u32 Depth)
    {
      DebugPrint("enum_member_stream {\n", Depth);
      
          DebugPrint("enum_member_stream_chunk FirstChunk {\n", Depth+2);
          DebugPrint(S.FirstChunk, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("enum_member_stream_chunk LastChunk {\n", Depth+2);
          DebugPrint(S.LastChunk, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( enum_member_stream *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( enum_member_iterator S, u32 Depth)
    {
      DebugPrint("enum_member_iterator {\n", Depth);
      
          DebugPrint("enum_member_stream Stream {\n", Depth+2);
          DebugPrint(S.Stream, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("enum_member_stream_chunk At {\n", Depth+2);
          DebugPrint(S.At, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( enum_member_iterator *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( d_union_member_stream_chunk S, u32 Depth)
    {
      DebugPrint("d_union_member_stream_chunk {\n", Depth);
      
          DebugPrint("d_union_member Element {\n", Depth+2);
          DebugPrint(S.Element, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("d_union_member_stream_chunk Next {\n", Depth+2);
          DebugPrint(S.Next, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( d_union_member_stream_chunk *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( d_union_member_stream S, u32 Depth)
    {
      DebugPrint("d_union_member_stream {\n", Depth);
      
          DebugPrint("d_union_member_stream_chunk FirstChunk {\n", Depth+2);
          DebugPrint(S.FirstChunk, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("d_union_member_stream_chunk LastChunk {\n", Depth+2);
          DebugPrint(S.LastChunk, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( d_union_member_stream *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( d_union_member_iterator S, u32 Depth)
    {
      DebugPrint("d_union_member_iterator {\n", Depth);
      
          DebugPrint("d_union_member_stream Stream {\n", Depth+2);
          DebugPrint(S.Stream, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("d_union_member_stream_chunk At {\n", Depth+2);
          DebugPrint(S.At, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( d_union_member_iterator *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( enum_def_cursor S, u32 Depth)
    {
      DebugPrint("enum_def_cursor {\n", Depth);
      
          DebugPrint("enum_def Start {\n", Depth+2);
          DebugPrint(S.Start, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("enum_def End {\n", Depth+2);
          DebugPrint(S.End, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("enum_def At {\n", Depth+2);
          DebugPrint(S.At, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( enum_def_cursor *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( enum_def_stream_chunk S, u32 Depth)
    {
      DebugPrint("enum_def_stream_chunk {\n", Depth);
      
          DebugPrint("enum_def Element {\n", Depth+2);
          DebugPrint(S.Element, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("enum_def_stream_chunk Next {\n", Depth+2);
          DebugPrint(S.Next, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( enum_def_stream_chunk *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( enum_def_stream S, u32 Depth)
    {
      DebugPrint("enum_def_stream {\n", Depth);
      
          DebugPrint("enum_def_stream_chunk FirstChunk {\n", Depth+2);
          DebugPrint(S.FirstChunk, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("enum_def_stream_chunk LastChunk {\n", Depth+2);
          DebugPrint(S.LastChunk, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( enum_def_stream *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( enum_def_iterator S, u32 Depth)
    {
      DebugPrint("enum_def_iterator {\n", Depth);
      
          DebugPrint("enum_def_stream Stream {\n", Depth+2);
          DebugPrint(S.Stream, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("enum_def_stream_chunk At {\n", Depth+2);
          DebugPrint(S.At, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( enum_def_iterator *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( meta_func_arg_stream_chunk S, u32 Depth)
    {
      DebugPrint("meta_func_arg_stream_chunk {\n", Depth);
      
          DebugPrint("meta_func_arg Element {\n", Depth+2);
          DebugPrint(S.Element, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("meta_func_arg_stream_chunk Next {\n", Depth+2);
          DebugPrint(S.Next, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( meta_func_arg_stream_chunk *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( meta_func_arg_stream S, u32 Depth)
    {
      DebugPrint("meta_func_arg_stream {\n", Depth);
      
          DebugPrint("meta_func_arg_stream_chunk FirstChunk {\n", Depth+2);
          DebugPrint(S.FirstChunk, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("meta_func_arg_stream_chunk LastChunk {\n", Depth+2);
          DebugPrint(S.LastChunk, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( meta_func_arg_stream *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( meta_func_arg_iterator S, u32 Depth)
    {
      DebugPrint("meta_func_arg_iterator {\n", Depth);
      
          DebugPrint("meta_func_arg_stream Stream {\n", Depth+2);
          DebugPrint(S.Stream, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("meta_func_arg_stream_chunk At {\n", Depth+2);
          DebugPrint(S.At, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( meta_func_arg_iterator *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( c_token_cursor S, u32 Depth)
    {
      DebugPrint("c_token_cursor {\n", Depth);
      
          DebugPrint("c_token Start {\n", Depth+2);
          DebugPrint(S.Start, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("c_token End {\n", Depth+2);
          DebugPrint(S.End, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("c_token At {\n", Depth+2);
          DebugPrint(S.At, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( c_token_cursor *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( c_parse_result_cursor S, u32 Depth)
    {
      DebugPrint("c_parse_result_cursor {\n", Depth);
      
          DebugPrint("c_parse_result Start {\n", Depth+2);
          DebugPrint(S.Start, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("c_parse_result End {\n", Depth+2);
          DebugPrint(S.End, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("c_parse_result At {\n", Depth+2);
          DebugPrint(S.At, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( c_parse_result_cursor *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( meta_func_stream_chunk S, u32 Depth)
    {
      DebugPrint("meta_func_stream_chunk {\n", Depth);
      
          DebugPrint("meta_func Element {\n", Depth+2);
          DebugPrint(S.Element, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("meta_func_stream_chunk Next {\n", Depth+2);
          DebugPrint(S.Next, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( meta_func_stream_chunk *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( meta_func_stream S, u32 Depth)
    {
      DebugPrint("meta_func_stream {\n", Depth);
      
          DebugPrint("meta_func_stream_chunk FirstChunk {\n", Depth+2);
          DebugPrint(S.FirstChunk, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("meta_func_stream_chunk LastChunk {\n", Depth+2);
          DebugPrint(S.LastChunk, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( meta_func_stream *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( meta_func_iterator S, u32 Depth)
    {
      DebugPrint("meta_func_iterator {\n", Depth);
      
          DebugPrint("meta_func_stream Stream {\n", Depth+2);
          DebugPrint(S.Stream, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("meta_func_stream_chunk At {\n", Depth+2);
          DebugPrint(S.At, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( meta_func_iterator *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( todo_stream_chunk S, u32 Depth)
    {
      DebugPrint("todo_stream_chunk {\n", Depth);
      
          DebugPrint("todo Element {\n", Depth+2);
          DebugPrint(S.Element, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("todo_stream_chunk Next {\n", Depth+2);
          DebugPrint(S.Next, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( todo_stream_chunk *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( todo_stream S, u32 Depth)
    {
      DebugPrint("todo_stream {\n", Depth);
      
          DebugPrint("todo_stream_chunk FirstChunk {\n", Depth+2);
          DebugPrint(S.FirstChunk, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("todo_stream_chunk LastChunk {\n", Depth+2);
          DebugPrint(S.LastChunk, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( todo_stream *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( todo_iterator S, u32 Depth)
    {
      DebugPrint("todo_iterator {\n", Depth);
      
          DebugPrint("todo_stream Stream {\n", Depth+2);
          DebugPrint(S.Stream, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("todo_stream_chunk At {\n", Depth+2);
          DebugPrint(S.At, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( todo_iterator *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( tag_stream_chunk S, u32 Depth)
    {
      DebugPrint("tag_stream_chunk {\n", Depth);
      
          DebugPrint("tag Element {\n", Depth+2);
          DebugPrint(S.Element, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("tag_stream_chunk Next {\n", Depth+2);
          DebugPrint(S.Next, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( tag_stream_chunk *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( tag_stream S, u32 Depth)
    {
      DebugPrint("tag_stream {\n", Depth);
      
          DebugPrint("tag_stream_chunk FirstChunk {\n", Depth+2);
          DebugPrint(S.FirstChunk, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("tag_stream_chunk LastChunk {\n", Depth+2);
          DebugPrint(S.LastChunk, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( tag_stream *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( tag_iterator S, u32 Depth)
    {
      DebugPrint("tag_iterator {\n", Depth);
      
          DebugPrint("tag_stream Stream {\n", Depth+2);
          DebugPrint(S.Stream, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("tag_stream_chunk At {\n", Depth+2);
          DebugPrint(S.At, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( tag_iterator *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( person_stream_chunk S, u32 Depth)
    {
      DebugPrint("person_stream_chunk {\n", Depth);
      
          DebugPrint("person Element {\n", Depth+2);
          DebugPrint(S.Element, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("person_stream_chunk Next {\n", Depth+2);
          DebugPrint(S.Next, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( person_stream_chunk *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( person_stream S, u32 Depth)
    {
      DebugPrint("person_stream {\n", Depth);
      
          DebugPrint("person_stream_chunk FirstChunk {\n", Depth+2);
          DebugPrint(S.FirstChunk, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("person_stream_chunk LastChunk {\n", Depth+2);
          DebugPrint(S.LastChunk, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( person_stream *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( person_iterator S, u32 Depth)
    {
      DebugPrint("person_iterator {\n", Depth);
      
          DebugPrint("person_stream Stream {\n", Depth+2);
          DebugPrint(S.Stream, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("person_stream_chunk At {\n", Depth+2);
          DebugPrint(S.At, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( person_iterator *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( ast_node_variable_def_stream_chunk S, u32 Depth)
    {
      DebugPrint("ast_node_variable_def_stream_chunk {\n", Depth);
      
          DebugPrint("ast_node_variable_def Element {\n", Depth+2);
          DebugPrint(S.Element, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("ast_node_variable_def_stream_chunk Next {\n", Depth+2);
          DebugPrint(S.Next, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( ast_node_variable_def_stream_chunk *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( ast_node_variable_def_stream S, u32 Depth)
    {
      DebugPrint("ast_node_variable_def_stream {\n", Depth);
      
          DebugPrint("ast_node_variable_def_stream_chunk FirstChunk {\n", Depth+2);
          DebugPrint(S.FirstChunk, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("ast_node_variable_def_stream_chunk LastChunk {\n", Depth+2);
          DebugPrint(S.LastChunk, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( ast_node_variable_def_stream *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( ast_node_variable_def_iterator S, u32 Depth)
    {
      DebugPrint("ast_node_variable_def_iterator {\n", Depth);
      
          DebugPrint("ast_node_variable_def_stream Stream {\n", Depth+2);
          DebugPrint(S.Stream, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("ast_node_variable_def_stream_chunk At {\n", Depth+2);
          DebugPrint(S.At, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( ast_node_variable_def_iterator *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( ast_node S, u32 Depth)
    {
      DebugPrint("ast_node {\n", Depth);
      
          DebugPrint("ast_node_type Type {\n", Depth+2);
          DebugPrint(S.Type, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( ast_node *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( ast_node_stream_chunk S, u32 Depth)
    {
      DebugPrint("ast_node_stream_chunk {\n", Depth);
      
          DebugPrint("ast_node Element {\n", Depth+2);
          DebugPrint(S.Element, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("ast_node_stream_chunk Next {\n", Depth+2);
          DebugPrint(S.Next, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( ast_node_stream_chunk *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( ast_node_stream S, u32 Depth)
    {
      DebugPrint("ast_node_stream {\n", Depth);
      
          DebugPrint("ast_node_stream_chunk FirstChunk {\n", Depth+2);
          DebugPrint(S.FirstChunk, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("ast_node_stream_chunk LastChunk {\n", Depth+2);
          DebugPrint(S.LastChunk, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( ast_node_stream *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( ast_node_iterator S, u32 Depth)
    {
      DebugPrint("ast_node_iterator {\n", Depth);
      
          DebugPrint("ast_node_stream Stream {\n", Depth+2);
          DebugPrint(S.Stream, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("ast_node_stream_chunk At {\n", Depth+2);
          DebugPrint(S.At, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( ast_node_iterator *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( function_def_stream_chunk S, u32 Depth)
    {
      DebugPrint("function_def_stream_chunk {\n", Depth);
      
          DebugPrint("function_def Element {\n", Depth+2);
          DebugPrint(S.Element, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("function_def_stream_chunk Next {\n", Depth+2);
          DebugPrint(S.Next, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( function_def_stream_chunk *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( function_def_stream S, u32 Depth)
    {
      DebugPrint("function_def_stream {\n", Depth);
      
          DebugPrint("function_def_stream_chunk FirstChunk {\n", Depth+2);
          DebugPrint(S.FirstChunk, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("function_def_stream_chunk LastChunk {\n", Depth+2);
          DebugPrint(S.LastChunk, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( function_def_stream *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( function_def_iterator S, u32 Depth)
    {
      DebugPrint("function_def_iterator {\n", Depth);
      
          DebugPrint("function_def_stream Stream {\n", Depth+2);
          DebugPrint(S.Stream, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("function_def_stream_chunk At {\n", Depth+2);
          DebugPrint(S.At, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( function_def_iterator *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( struct_member_iterator S, u32 Depth)
    {
      DebugPrint("struct_member_iterator {\n", Depth);
      
          DebugPrint("struct_member_stream Stream {\n", Depth+2);
          DebugPrint(S.Stream, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("struct_member_stream_chunk At {\n", Depth+2);
          DebugPrint(S.At, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( struct_member_iterator *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( work_queue_entry S, u32 Depth)
    {
      DebugPrint("work_queue_entry {\n", Depth);
      
          DebugPrint("work_queue_entry_type Type {\n", Depth+2);
          DebugPrint(S.Type, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( work_queue_entry *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( v3_cursor S, u32 Depth)
    {
      DebugPrint("v3_cursor {\n", Depth);
      
          DebugPrint("v3 Start {\n", Depth+2);
          DebugPrint(S.Start, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("v3 End {\n", Depth+2);
          DebugPrint(S.End, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("v3 At {\n", Depth+2);
          DebugPrint(S.At, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( v3_cursor *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( counted_string_cursor S, u32 Depth)
    {
      DebugPrint("counted_string_cursor {\n", Depth);
      
          DebugPrint("counted_string Start {\n", Depth+2);
          DebugPrint(S.Start, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("counted_string End {\n", Depth+2);
          DebugPrint(S.End, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("counted_string At {\n", Depth+2);
          DebugPrint(S.At, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( counted_string_cursor *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( counted_string_stream_chunk S, u32 Depth)
    {
      DebugPrint("counted_string_stream_chunk {\n", Depth);
      
          DebugPrint("counted_string Element {\n", Depth+2);
          DebugPrint(S.Element, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("counted_string_stream_chunk Next {\n", Depth+2);
          DebugPrint(S.Next, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( counted_string_stream_chunk *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( counted_string_stream S, u32 Depth)
    {
      DebugPrint("counted_string_stream {\n", Depth);
      
          DebugPrint("counted_string_stream_chunk FirstChunk {\n", Depth+2);
          DebugPrint(S.FirstChunk, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("counted_string_stream_chunk LastChunk {\n", Depth+2);
          DebugPrint(S.LastChunk, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( counted_string_stream *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( counted_string_iterator S, u32 Depth)
    {
      DebugPrint("counted_string_iterator {\n", Depth);
      
          DebugPrint("counted_string_stream Stream {\n", Depth+2);
          DebugPrint(S.Stream, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("counted_string_stream_chunk At {\n", Depth+2);
          DebugPrint(S.At, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( counted_string_iterator *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( tagged_counted_string_stream_stream_chunk S, u32 Depth)
    {
      DebugPrint("tagged_counted_string_stream_stream_chunk {\n", Depth);
      
          DebugPrint("tagged_counted_string_stream Element {\n", Depth+2);
          DebugPrint(S.Element, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("tagged_counted_string_stream_stream_chunk Next {\n", Depth+2);
          DebugPrint(S.Next, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( tagged_counted_string_stream_stream_chunk *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( tagged_counted_string_stream_stream S, u32 Depth)
    {
      DebugPrint("tagged_counted_string_stream_stream {\n", Depth);
      
          DebugPrint("tagged_counted_string_stream_stream_chunk FirstChunk {\n", Depth+2);
          DebugPrint(S.FirstChunk, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("tagged_counted_string_stream_stream_chunk LastChunk {\n", Depth+2);
          DebugPrint(S.LastChunk, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( tagged_counted_string_stream_stream *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( tagged_counted_string_stream_iterator S, u32 Depth)
    {
      DebugPrint("tagged_counted_string_stream_iterator {\n", Depth);
      
          DebugPrint("tagged_counted_string_stream_stream Stream {\n", Depth+2);
          DebugPrint(S.Stream, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

          DebugPrint("tagged_counted_string_stream_stream_chunk At {\n", Depth+2);
          DebugPrint(S.At, Depth+4);
          DebugPrint("\n");
          DebugPrint("}", Depth+2);
          DebugPrint("\n");

      DebugPrint("}\n", Depth);
    }

    function void DebugPrint( tagged_counted_string_stream_iterator *S, u32 Depth)
    {
      if (S) { DebugPrint(*S, Depth); }
      else { DebugPrint("ptr(0)", Depth); }
    }

    function void DebugPrint( debug_ui_type EnumValue, u32 Depth)
    {
      switch (EnumValue)
      {
        
            case DebugUIType_None:
            {
              DebugPrint("DebugUIType_None ", Depth);
              DebugPrint("0");
            } break;

            case DebugUIType_PickedChunks:
            {
              DebugPrint("DebugUIType_PickedChunks ", Depth);
              DebugPrint("(1 << 0)");
            } break;

            case DebugUIType_CallGraph:
            {
              DebugPrint("DebugUIType_CallGraph ", Depth);
              DebugPrint("(1 << 1)");
            } break;

            case DebugUIType_CollatedFunctionCalls:
            {
              DebugPrint("DebugUIType_CollatedFunctionCalls ", Depth);
              DebugPrint("(1 << 2)");
            } break;

            case DebugUIType_Memory:
            {
              DebugPrint("DebugUIType_Memory ", Depth);
              DebugPrint("(1 << 3)");
            } break;

            case DebugUIType_Graphics:
            {
              DebugPrint("DebugUIType_Graphics ", Depth);
              DebugPrint("(1 << 4)");
            } break;

            case DebugUIType_Network:
            {
              DebugPrint("DebugUIType_Network ", Depth);
              DebugPrint("(1 << 5)");
            } break;

            case DebugUIType_DrawCalls:
            {
              DebugPrint("DebugUIType_DrawCalls ", Depth);
              DebugPrint("(1 << 6)");
            } break;

      }
    }

    function void DebugPrint( debug_texture_array_slice EnumValue, u32 Depth)
    {
      switch (EnumValue)
      {
        
            case DebugTextureArraySlice_Font:
            {
              DebugPrint("DebugTextureArraySlice_Font ", Depth);
              DebugPrint("");
            } break;

            case DebugTextureArraySlice_Viewport:
            {
              DebugPrint("DebugTextureArraySlice_Viewport ", Depth);
              DebugPrint("");
            } break;

            case DebugTextureArraySlice_Count:
            {
              DebugPrint("DebugTextureArraySlice_Count ", Depth);
              DebugPrint("");
            } break;

      }
    }

    function void DebugPrint( column_render_params EnumValue, u32 Depth)
    {
      switch (EnumValue)
      {
        
            case ColumnRenderParam_LeftAlign:
            {
              DebugPrint("ColumnRenderParam_LeftAlign ", Depth);
              DebugPrint("0");
            } break;

            case ColumnRenderParam_RightAlign:
            {
              DebugPrint("ColumnRenderParam_RightAlign ", Depth);
              DebugPrint("(1 << 0)");
            } break;

      }
    }

    function void DebugPrint( quad_render_params EnumValue, u32 Depth)
    {
      switch (EnumValue)
      {
        
            case QuadRenderParam_NoAdvance:
            {
              DebugPrint("QuadRenderParam_NoAdvance ", Depth);
              DebugPrint("0");
            } break;

            case QuadRenderParam_AdvanceLayout:
            {
              DebugPrint("QuadRenderParam_AdvanceLayout ", Depth);
              DebugPrint("(1 << 0)");
            } break;

            case QuadRenderParam_AdvanceClip:
            {
              DebugPrint("QuadRenderParam_AdvanceClip ", Depth);
              DebugPrint("(1 << 1)");
            } break;

            case QuadRenderParam_Default:
            {
              DebugPrint("QuadRenderParam_Default ", Depth);
              DebugPrint("(QuadRenderParam_AdvanceLayout|QuadRenderParam_AdvanceClip)");
            } break;

      }
    }

    function void DebugPrint( button_end_params EnumValue, u32 Depth)
    {
      switch (EnumValue)
      {
        
            case ButtonEndParam_NoOp:
            {
              DebugPrint("ButtonEndParam_NoOp ", Depth);
              DebugPrint("0");
            } break;

            case ButtonEndParam_DiscardButtonDrawBounds:
            {
              DebugPrint("ButtonEndParam_DiscardButtonDrawBounds ", Depth);
              DebugPrint("(1 << 0)");
            } break;

      }
    }

    function void DebugPrint( relative_position EnumValue, u32 Depth)
    {
      switch (EnumValue)
      {
        
            case Position_None:
            {
              DebugPrint("Position_None ", Depth);
              DebugPrint("");
            } break;

            case Position_LeftOf:
            {
              DebugPrint("Position_LeftOf ", Depth);
              DebugPrint("");
            } break;

            case Position_RightOf:
            {
              DebugPrint("Position_RightOf ", Depth);
              DebugPrint("");
            } break;

            case Position_Above:
            {
              DebugPrint("Position_Above ", Depth);
              DebugPrint("");
            } break;

            case Position_Below:
            {
              DebugPrint("Position_Below ", Depth);
              DebugPrint("");
            } break;

      }
    }

    function void DebugPrint( z_depth EnumValue, u32 Depth)
    {
      switch (EnumValue)
      {
        
            case zDepth_Background:
            {
              DebugPrint("zDepth_Background ", Depth);
              DebugPrint("");
            } break;

            case zDepth_Text:
            {
              DebugPrint("zDepth_Text ", Depth);
              DebugPrint("");
            } break;

            case zDepth_TitleBar:
            {
              DebugPrint("zDepth_TitleBar ", Depth);
              DebugPrint("");
            } break;

            case zDepth_Border:
            {
              DebugPrint("zDepth_Border ", Depth);
              DebugPrint("");
            } break;

      }
    }

    function void DebugPrint( clip_status EnumValue, u32 Depth)
    {
      switch (EnumValue)
      {
        
            case ClipStatus_NoClipping:
            {
              DebugPrint("ClipStatus_NoClipping ", Depth);
              DebugPrint("");
            } break;

            case ClipStatus_PartialClipping:
            {
              DebugPrint("ClipStatus_PartialClipping ", Depth);
              DebugPrint("");
            } break;

            case ClipStatus_FullyClipped:
            {
              DebugPrint("ClipStatus_FullyClipped ", Depth);
              DebugPrint("");
            } break;

      }
    }

    function void DebugPrint( d_union_flags EnumValue, u32 Depth)
    {
      switch (EnumValue)
      {
        
            case d_union_flag_none:
            {
              DebugPrint("d_union_flag_none ", Depth);
              DebugPrint("");
            } break;

            case d_union_flag_enum_only:
            {
              DebugPrint("d_union_flag_enum_only ", Depth);
              DebugPrint("");
            } break;

      }
    }

    function void DebugPrint( metaprogramming_directive EnumValue, u32 Depth)
    {
      switch (EnumValue)
      {
        
            case meta_directive_noop:
            {
              DebugPrint("meta_directive_noop ", Depth);
              DebugPrint("");
            } break;

            case enum_only:
            {
              DebugPrint("enum_only ", Depth);
              DebugPrint("");
            } break;

            case d_union:
            {
              DebugPrint("d_union ", Depth);
              DebugPrint("");
            } break;

            case named_list:
            {
              DebugPrint("named_list ", Depth);
              DebugPrint("");
            } break;

            case for_datatypes:
            {
              DebugPrint("for_datatypes ", Depth);
              DebugPrint("");
            } break;

            case func:
            {
              DebugPrint("func ", Depth);
              DebugPrint("");
            } break;

            case polymorphic_func:
            {
              DebugPrint("polymorphic_func ", Depth);
              DebugPrint("");
            } break;

      }
    }

    function void DebugPrint( meta_arg_operator EnumValue, u32 Depth)
    {
      switch (EnumValue)
      {
        
            case meta_arg_operator_noop:
            {
              DebugPrint("meta_arg_operator_noop ", Depth);
              DebugPrint("");
            } break;

            case name:
            {
              DebugPrint("name ", Depth);
              DebugPrint("");
            } break;

            case type:
            {
              DebugPrint("type ", Depth);
              DebugPrint("");
            } break;

            case value:
            {
              DebugPrint("value ", Depth);
              DebugPrint("");
            } break;

            case map_values:
            {
              DebugPrint("map_values ", Depth);
              DebugPrint("");
            } break;

            case map_members:
            {
              DebugPrint("map_members ", Depth);
              DebugPrint("");
            } break;

            case is_enum:
            {
              DebugPrint("is_enum ", Depth);
              DebugPrint("");
            } break;

            case is_struct:
            {
              DebugPrint("is_struct ", Depth);
              DebugPrint("");
            } break;

      }
    }

    function void DebugPrint( meta_transform_op EnumValue, u32 Depth)
    {
      switch (EnumValue)
      {
        
            case meta_transform_op_noop:
            {
              DebugPrint("meta_transform_op_noop ", Depth);
              DebugPrint("");
            } break;

            case to_capital_case:
            {
              DebugPrint("to_capital_case ", Depth);
              DebugPrint("(1 << 0)");
            } break;

      }
    }

    function void DebugPrint( c_token_type EnumValue, u32 Depth)
    {
      switch (EnumValue)
      {
        
            case CTokenType_Unknown:
            {
              DebugPrint("CTokenType_Unknown ", Depth);
              DebugPrint("0");
            } break;

            case CTokenType_OpenBracket:
            {
              DebugPrint("CTokenType_OpenBracket ", Depth);
              DebugPrint("'['");
            } break;

            case CTokenType_CloseBracket:
            {
              DebugPrint("CTokenType_CloseBracket ", Depth);
              DebugPrint("']'");
            } break;

            case CTokenType_OpenBrace:
            {
              DebugPrint("CTokenType_OpenBrace ", Depth);
              DebugPrint("'{'");
            } break;

            case CTokenType_CloseBrace:
            {
              DebugPrint("CTokenType_CloseBrace ", Depth);
              DebugPrint("'}'");
            } break;

            case CTokenType_OpenParen:
            {
              DebugPrint("CTokenType_OpenParen ", Depth);
              DebugPrint("'('");
            } break;

            case CTokenType_CloseParen:
            {
              DebugPrint("CTokenType_CloseParen ", Depth);
              DebugPrint("')'");
            } break;

            case CTokenType_Dot:
            {
              DebugPrint("CTokenType_Dot ", Depth);
              DebugPrint("'.'");
            } break;

            case CTokenType_Comma:
            {
              DebugPrint("CTokenType_Comma ", Depth);
              DebugPrint("','");
            } break;

            case CTokenType_Semicolon:
            {
              DebugPrint("CTokenType_Semicolon ", Depth);
              DebugPrint("';'");
            } break;

            case CTokenType_Colon:
            {
              DebugPrint("CTokenType_Colon ", Depth);
              DebugPrint("':'");
            } break;

            case CTokenType_Hash:
            {
              DebugPrint("CTokenType_Hash ", Depth);
              DebugPrint("'#'");
            } break;

            case CTokenType_At:
            {
              DebugPrint("CTokenType_At ", Depth);
              DebugPrint("'@'");
            } break;

            case CTokenType_Dollar:
            {
              DebugPrint("CTokenType_Dollar ", Depth);
              DebugPrint("'$'");
            } break;

            case CTokenType_Space:
            {
              DebugPrint("CTokenType_Space ", Depth);
              DebugPrint("' '");
            } break;

            case CTokenType_Star:
            {
              DebugPrint("CTokenType_Star ", Depth);
              DebugPrint("'*'");
            } break;

            case CTokenType_Ampersand:
            {
              DebugPrint("CTokenType_Ampersand ", Depth);
              DebugPrint("'&'");
            } break;

            case CTokenType_SingleQuote:
            {
              DebugPrint("CTokenType_SingleQuote ", Depth);
              DebugPrint("'\''");
            } break;

            case CTokenType_DoubleQuote:
            {
              DebugPrint("CTokenType_DoubleQuote ", Depth);
              DebugPrint("'\"'");
            } break;

            case CTokenType_Equals:
            {
              DebugPrint("CTokenType_Equals ", Depth);
              DebugPrint("'='");
            } break;

            case CTokenType_LT:
            {
              DebugPrint("CTokenType_LT ", Depth);
              DebugPrint("'<'");
            } break;

            case CTokenType_GT:
            {
              DebugPrint("CTokenType_GT ", Depth);
              DebugPrint("'>'");
            } break;

            case CTokenType_Plus:
            {
              DebugPrint("CTokenType_Plus ", Depth);
              DebugPrint("'+'");
            } break;

            case CTokenType_Minus:
            {
              DebugPrint("CTokenType_Minus ", Depth);
              DebugPrint("'-'");
            } break;

            case CTokenType_Percent:
            {
              DebugPrint("CTokenType_Percent ", Depth);
              DebugPrint("'%'");
            } break;

            case CTokenType_Bang:
            {
              DebugPrint("CTokenType_Bang ", Depth);
              DebugPrint("'!'");
            } break;

            case CTokenType_Hat:
            {
              DebugPrint("CTokenType_Hat ", Depth);
              DebugPrint("'^'");
            } break;

            case CTokenType_Question:
            {
              DebugPrint("CTokenType_Question ", Depth);
              DebugPrint("'?'");
            } break;

            case CTokenType_FSlash:
            {
              DebugPrint("CTokenType_FSlash ", Depth);
              DebugPrint("'/'");
            } break;

            case CTokenType_BSlash:
            {
              DebugPrint("CTokenType_BSlash ", Depth);
              DebugPrint("'\\'");
            } break;

            case CTokenType_Tilde:
            {
              DebugPrint("CTokenType_Tilde ", Depth);
              DebugPrint("'~'");
            } break;

            case CTokenType_Backtick:
            {
              DebugPrint("CTokenType_Backtick ", Depth);
              DebugPrint("'`'");
            } break;

            case CTokenType_Pipe:
            {
              DebugPrint("CTokenType_Pipe ", Depth);
              DebugPrint("'|'");
            } break;

            case CTokenType_Newline:
            {
              DebugPrint("CTokenType_Newline ", Depth);
              DebugPrint("'\n'");
            } break;

            case CTokenType_CarrigeReturn:
            {
              DebugPrint("CTokenType_CarrigeReturn ", Depth);
              DebugPrint("'\r'");
            } break;

            case CTokenType_EOF:
            {
              DebugPrint("CTokenType_EOF ", Depth);
              DebugPrint("EOF");
            } break;

            case CTokenType_CommentSingleLine:
            {
              DebugPrint("CTokenType_CommentSingleLine ", Depth);
              DebugPrint("256");
            } break;

            case CTokenType_CommentMultiLineStart:
            {
              DebugPrint("CTokenType_CommentMultiLineStart ", Depth);
              DebugPrint("");
            } break;

            case CTokenType_CommentMultiLineEnd:
            {
              DebugPrint("CTokenType_CommentMultiLineEnd ", Depth);
              DebugPrint("");
            } break;

            case CTokenType_Identifier:
            {
              DebugPrint("CTokenType_Identifier ", Depth);
              DebugPrint("");
            } break;

            case CTokenType_String:
            {
              DebugPrint("CTokenType_String ", Depth);
              DebugPrint("");
            } break;

            case CTokenType_Char:
            {
              DebugPrint("CTokenType_Char ", Depth);
              DebugPrint("");
            } break;

            case CTokenType_EscapedNewline:
            {
              DebugPrint("CTokenType_EscapedNewline ", Depth);
              DebugPrint("");
            } break;

            case CTokenType_If:
            {
              DebugPrint("CTokenType_If ", Depth);
              DebugPrint("");
            } break;

            case CTokenType_Else:
            {
              DebugPrint("CTokenType_Else ", Depth);
              DebugPrint("");
            } break;

            case CTokenType_Break:
            {
              DebugPrint("CTokenType_Break ", Depth);
              DebugPrint("");
            } break;

            case CTokenType_Switch:
            {
              DebugPrint("CTokenType_Switch ", Depth);
              DebugPrint("");
            } break;

            case CTokenType_Case:
            {
              DebugPrint("CTokenType_Case ", Depth);
              DebugPrint("");
            } break;

            case CTokenType_Default:
            {
              DebugPrint("CTokenType_Default ", Depth);
              DebugPrint("");
            } break;

            case CTokenType_For:
            {
              DebugPrint("CTokenType_For ", Depth);
              DebugPrint("");
            } break;

            case CTokenType_While:
            {
              DebugPrint("CTokenType_While ", Depth);
              DebugPrint("");
            } break;

            case CTokenType_Continue:
            {
              DebugPrint("CTokenType_Continue ", Depth);
              DebugPrint("");
            } break;

            case CTokenType_Return:
            {
              DebugPrint("CTokenType_Return ", Depth);
              DebugPrint("");
            } break;

            case CTokenType_LeftShift:
            {
              DebugPrint("CTokenType_LeftShift ", Depth);
              DebugPrint("");
            } break;

            case CTokenType_RightShift:
            {
              DebugPrint("CTokenType_RightShift ", Depth);
              DebugPrint("");
            } break;

            case CTokenType_LessEqual:
            {
              DebugPrint("CTokenType_LessEqual ", Depth);
              DebugPrint("");
            } break;

            case CTokenType_GreaterEqual:
            {
              DebugPrint("CTokenType_GreaterEqual ", Depth);
              DebugPrint("");
            } break;

            case CTokenType_AreEqual:
            {
              DebugPrint("CTokenType_AreEqual ", Depth);
              DebugPrint("");
            } break;

            case CTokenType_NotEqual:
            {
              DebugPrint("CTokenType_NotEqual ", Depth);
              DebugPrint("");
            } break;

            case CTokenType_PlusEquals:
            {
              DebugPrint("CTokenType_PlusEquals ", Depth);
              DebugPrint("");
            } break;

            case CTokenType_MinusEquals:
            {
              DebugPrint("CTokenType_MinusEquals ", Depth);
              DebugPrint("");
            } break;

            case CTokenType_TimesEquals:
            {
              DebugPrint("CTokenType_TimesEquals ", Depth);
              DebugPrint("");
            } break;

            case CTokenType_DivEquals:
            {
              DebugPrint("CTokenType_DivEquals ", Depth);
              DebugPrint("");
            } break;

            case CTokenType_ModEquals:
            {
              DebugPrint("CTokenType_ModEquals ", Depth);
              DebugPrint("");
            } break;

            case CTokenType_AndEquals:
            {
              DebugPrint("CTokenType_AndEquals ", Depth);
              DebugPrint("");
            } break;

            case CTokenType_OrEquals:
            {
              DebugPrint("CTokenType_OrEquals ", Depth);
              DebugPrint("");
            } break;

            case CTokenType_XorEquals:
            {
              DebugPrint("CTokenType_XorEquals ", Depth);
              DebugPrint("");
            } break;

            case CTokenType_Increment:
            {
              DebugPrint("CTokenType_Increment ", Depth);
              DebugPrint("");
            } break;

            case CTokenType_Decrement:
            {
              DebugPrint("CTokenType_Decrement ", Depth);
              DebugPrint("");
            } break;

            case CTokenType_LogicalAnd:
            {
              DebugPrint("CTokenType_LogicalAnd ", Depth);
              DebugPrint("");
            } break;

            case CTokenType_LogicalOr:
            {
              DebugPrint("CTokenType_LogicalOr ", Depth);
              DebugPrint("");
            } break;

            case CTokenType_Arrow:
            {
              DebugPrint("CTokenType_Arrow ", Depth);
              DebugPrint("");
            } break;

      }
    }

    function void DebugPrint( datatype_type EnumValue, u32 Depth)
    {
      switch (EnumValue)
      {
        
            case type_datatype_noop:
            {
              DebugPrint("type_datatype_noop ", Depth);
              DebugPrint("");
            } break;

            case type_struct_def:
            {
              DebugPrint("type_struct_def ", Depth);
              DebugPrint("");
            } break;

            case type_struct_member:
            {
              DebugPrint("type_struct_member ", Depth);
              DebugPrint("");
            } break;

            case type_enum_def:
            {
              DebugPrint("type_enum_def ", Depth);
              DebugPrint("");
            } break;

            case type_enum_member:
            {
              DebugPrint("type_enum_member ", Depth);
              DebugPrint("");
            } break;

      }
    }

    function void DebugPrint( output_mode EnumValue, u32 Depth)
    {
      switch (EnumValue)
      {
        
            case Output_NoOverwrite:
            {
              DebugPrint("Output_NoOverwrite ", Depth);
              DebugPrint("");
            } break;

            case Output_Unsafe:
            {
              DebugPrint("Output_Unsafe ", Depth);
              DebugPrint("");
            } break;

      }
    }

    function void DebugPrint( socket_type EnumValue, u32 Depth)
    {
      switch (EnumValue)
      {
        
            case Socket_Blocking:
            {
              DebugPrint("Socket_Blocking ", Depth);
              DebugPrint("");
            } break;

            case Socket_NonBlocking:
            {
              DebugPrint("Socket_NonBlocking ", Depth);
              DebugPrint("");
            } break;

      }
    }

    function void DebugPrint( connection_state EnumValue, u32 Depth)
    {
      switch (EnumValue)
      {
        
            case ConnectionState_Disconnected:
            {
              DebugPrint("ConnectionState_Disconnected ", Depth);
              DebugPrint("");
            } break;

            case ConnectionState_AwaitingHandshake:
            {
              DebugPrint("ConnectionState_AwaitingHandshake ", Depth);
              DebugPrint("");
            } break;

            case ConnectionState_Connected:
            {
              DebugPrint("ConnectionState_Connected ", Depth);
              DebugPrint("");
            } break;

      }
    }

    function void DebugPrint( socket_op_result EnumValue, u32 Depth)
    {
      switch (EnumValue)
      {
        
            case SocketOpResult_Noop:
            {
              DebugPrint("SocketOpResult_Noop ", Depth);
              DebugPrint("");
            } break;

            case SocketOpResult_CompletedRW:
            {
              DebugPrint("SocketOpResult_CompletedRW ", Depth);
              DebugPrint("");
            } break;

            case SocketOpResult_Fail:
            {
              DebugPrint("SocketOpResult_Fail ", Depth);
              DebugPrint("");
            } break;

      }
    }

    function void DebugPrint( socket_op EnumValue, u32 Depth)
    {
      switch (EnumValue)
      {
        
            case SocketOp_Null:
            {
              DebugPrint("SocketOp_Null ", Depth);
              DebugPrint("");
            } break;

            case SocketOp_Read:
            {
              DebugPrint("SocketOp_Read ", Depth);
              DebugPrint("");
            } break;

            case SocketOp_Write:
            {
              DebugPrint("SocketOp_Write ", Depth);
              DebugPrint("");
            } break;

            case SocketOp_Count:
            {
              DebugPrint("SocketOp_Count ", Depth);
              DebugPrint("");
            } break;

      }
    }

    function void DebugPrint( xml_token_type EnumValue, u32 Depth)
    {
      switch (EnumValue)
      {
        
            case XmlTokenType_Unknown:
            {
              DebugPrint("XmlTokenType_Unknown ", Depth);
              DebugPrint("");
            } break;

            case XmlTokenType_Boolean:
            {
              DebugPrint("XmlTokenType_Boolean ", Depth);
              DebugPrint("");
            } break;

            case XmlTokenType_Float:
            {
              DebugPrint("XmlTokenType_Float ", Depth);
              DebugPrint("");
            } break;

            case XmlTokenType_Int:
            {
              DebugPrint("XmlTokenType_Int ", Depth);
              DebugPrint("");
            } break;

            case XmlTokenType_Property:
            {
              DebugPrint("XmlTokenType_Property ", Depth);
              DebugPrint("");
            } break;

            case XmlTokenType_Open:
            {
              DebugPrint("XmlTokenType_Open ", Depth);
              DebugPrint("");
            } break;

            case XmlTokenType_Close:
            {
              DebugPrint("XmlTokenType_Close ", Depth);
              DebugPrint("");
            } break;

            case XmlTokenType_Count:
            {
              DebugPrint("XmlTokenType_Count ", Depth);
              DebugPrint("");
            } break;

      }
    }

    function void DebugPrint( shader_uniform_type EnumValue, u32 Depth)
    {
      switch (EnumValue)
      {
        
            case ShaderUniform_Undefined:
            {
              DebugPrint("ShaderUniform_Undefined ", Depth);
              DebugPrint("");
            } break;

            case ShaderUniform_M4:
            {
              DebugPrint("ShaderUniform_M4 ", Depth);
              DebugPrint("");
            } break;

            case ShaderUniform_V3:
            {
              DebugPrint("ShaderUniform_V3 ", Depth);
              DebugPrint("");
            } break;

            case ShaderUniform_S32:
            {
              DebugPrint("ShaderUniform_S32 ", Depth);
              DebugPrint("");
            } break;

            case ShaderUniform_U32:
            {
              DebugPrint("ShaderUniform_U32 ", Depth);
              DebugPrint("");
            } break;

            case ShaderUniform_R32:
            {
              DebugPrint("ShaderUniform_R32 ", Depth);
              DebugPrint("");
            } break;

            case ShaderUniform_Texture:
            {
              DebugPrint("ShaderUniform_Texture ", Depth);
              DebugPrint("");
            } break;

            case ShaderUniform_Light:
            {
              DebugPrint("ShaderUniform_Light ", Depth);
              DebugPrint("");
            } break;

            case ShaderUniform_Camera:
            {
              DebugPrint("ShaderUniform_Camera ", Depth);
              DebugPrint("");
            } break;

      }
    }

    function void DebugPrint( frame_event_type EnumValue, u32 Depth)
    {
      switch (EnumValue)
      {
        
            case FrameEvent_Undefined:
            {
              DebugPrint("FrameEvent_Undefined ", Depth);
              DebugPrint("");
            } break;

            case FrameEvent_Explosion:
            {
              DebugPrint("FrameEvent_Explosion ", Depth);
              DebugPrint("");
            } break;

            case FrameEvent_GameModeLoss:
            {
              DebugPrint("FrameEvent_GameModeLoss ", Depth);
              DebugPrint("");
            } break;

            case FrameEvent_GameModePlaying:
            {
              DebugPrint("FrameEvent_GameModePlaying ", Depth);
              DebugPrint("");
            } break;

            case FrameEvent_GameModeWon:
            {
              DebugPrint("FrameEvent_GameModeWon ", Depth);
              DebugPrint("");
            } break;

      }
    }

    function void DebugPrint( game_mode_type EnumValue, u32 Depth)
    {
      switch (EnumValue)
      {
        
            case GameMode_Title:
            {
              DebugPrint("GameMode_Title ", Depth);
              DebugPrint("");
            } break;

            case GameMode_Playing:
            {
              DebugPrint("GameMode_Playing ", Depth);
              DebugPrint("");
            } break;

            case GameMode_Won:
            {
              DebugPrint("GameMode_Won ", Depth);
              DebugPrint("");
            } break;

            case GameMode_Loss:
            {
              DebugPrint("GameMode_Loss ", Depth);
              DebugPrint("");
            } break;

      }
    }

    function void DebugPrint( chunk_flag EnumValue, u32 Depth)
    {
      switch (EnumValue)
      {
        
            case Chunk_Uninitialized:
            {
              DebugPrint("Chunk_Uninitialized ", Depth);
              DebugPrint("0 << 0");
            } break;

            case Chunk_Initialized:
            {
              DebugPrint("Chunk_Initialized ", Depth);
              DebugPrint("1 << 1");
            } break;

            case Chunk_Queued:
            {
              DebugPrint("Chunk_Queued ", Depth);
              DebugPrint("1 << 2");
            } break;

            case Chunk_MeshComplete:
            {
              DebugPrint("Chunk_MeshComplete ", Depth);
              DebugPrint("1 << 3");
            } break;

            case Chunk_GpuMeshComplete:
            {
              DebugPrint("Chunk_GpuMeshComplete ", Depth);
              DebugPrint("1 << 4");
            } break;

            case Chunk_Garbage:
            {
              DebugPrint("Chunk_Garbage ", Depth);
              DebugPrint("1 << 5");
            } break;

            case Chunk_Collected:
            {
              DebugPrint("Chunk_Collected ", Depth);
              DebugPrint("1 << 6");
            } break;

      }
    }

    function void DebugPrint( voxel_flag EnumValue, u32 Depth)
    {
      switch (EnumValue)
      {
        
            case Voxel_Empty:
            {
              DebugPrint("Voxel_Empty ", Depth);
              DebugPrint("0");
            } break;

            case Voxel_Filled:
            {
              DebugPrint("Voxel_Filled ", Depth);
              DebugPrint("1 << 0");
            } break;

            case Voxel_LeftFace:
            {
              DebugPrint("Voxel_LeftFace ", Depth);
              DebugPrint("1 << 1");
            } break;

            case Voxel_RightFace:
            {
              DebugPrint("Voxel_RightFace ", Depth);
              DebugPrint("1 << 2");
            } break;

            case Voxel_TopFace:
            {
              DebugPrint("Voxel_TopFace ", Depth);
              DebugPrint("1 << 3");
            } break;

            case Voxel_BottomFace:
            {
              DebugPrint("Voxel_BottomFace ", Depth);
              DebugPrint("1 << 4");
            } break;

            case Voxel_FrontFace:
            {
              DebugPrint("Voxel_FrontFace ", Depth);
              DebugPrint("1 << 5");
            } break;

            case Voxel_BackFace:
            {
              DebugPrint("Voxel_BackFace ", Depth);
              DebugPrint("1 << 6");
            } break;

      }
    }

    function void DebugPrint( entity_state EnumValue, u32 Depth)
    {
      switch (EnumValue)
      {
        
            case EntityState_Uninitialized:
            {
              DebugPrint("EntityState_Uninitialized ", Depth);
              DebugPrint("0");
            } break;

            case EntityState_Initialized:
            {
              DebugPrint("EntityState_Initialized ", Depth);
              DebugPrint("1 << 0");
            } break;

            case EntityState_Spawned:
            {
              DebugPrint("EntityState_Spawned ", Depth);
              DebugPrint("1 << 1");
            } break;

            case EntityState_Destroyed:
            {
              DebugPrint("EntityState_Destroyed ", Depth);
              DebugPrint("1 << 2");
            } break;

            case EntityState_Reserved:
            {
              DebugPrint("EntityState_Reserved ", Depth);
              DebugPrint("1 << 3");
            } break;

      }
    }

    function void DebugPrint( entity_type EnumValue, u32 Depth)
    {
      switch (EnumValue)
      {
        
            case EntityType_None:
            {
              DebugPrint("EntityType_None ", Depth);
              DebugPrint("0");
            } break;

            case EntityType_Player:
            {
              DebugPrint("EntityType_Player ", Depth);
              DebugPrint("1 << 0");
            } break;

            case EntityType_Enemy:
            {
              DebugPrint("EntityType_Enemy ", Depth);
              DebugPrint("1 << 1");
            } break;

            case EntityType_EnemyProjectile:
            {
              DebugPrint("EntityType_EnemyProjectile ", Depth);
              DebugPrint("1 << 2");
            } break;

            case EntityType_PlayerProjectile:
            {
              DebugPrint("EntityType_PlayerProjectile ", Depth);
              DebugPrint("1 << 3");
            } break;

            case EntityType_Loot:
            {
              DebugPrint("EntityType_Loot ", Depth);
              DebugPrint("1 << 4");
            } break;

            case EntityType_PlayerProton:
            {
              DebugPrint("EntityType_PlayerProton ", Depth);
              DebugPrint("1 << 5");
            } break;

            case EntityType_ParticleSystem:
            {
              DebugPrint("EntityType_ParticleSystem ", Depth);
              DebugPrint("1 << 6");
            } break;

      }
    }

    function void DebugPrint( collision_type EnumValue, u32 Depth)
    {
      switch (EnumValue)
      {
        
            case Collision_Player_Enemy:
            {
              DebugPrint("Collision_Player_Enemy ", Depth);
              DebugPrint("EntityType_Player | EntityType_Enemy");
            } break;

            case Collision_Player_EnemyProjectile:
            {
              DebugPrint("Collision_Player_EnemyProjectile ", Depth);
              DebugPrint("EntityType_Player | EntityType_EnemyProjectile");
            } break;

            case Collision_Player_PlayerProjectile:
            {
              DebugPrint("Collision_Player_PlayerProjectile ", Depth);
              DebugPrint("EntityType_Player | EntityType_PlayerProjectile");
            } break;

            case Collision_Player_Loot:
            {
              DebugPrint("Collision_Player_Loot ", Depth);
              DebugPrint("EntityType_Player | EntityType_Loot");
            } break;

            case Collision_Enemy_PlayerProjectile:
            {
              DebugPrint("Collision_Enemy_PlayerProjectile ", Depth);
              DebugPrint("EntityType_Enemy  | EntityType_PlayerProjectile");
            } break;

            case Collision_Enemy_PlayerProton:
            {
              DebugPrint("Collision_Enemy_PlayerProton ", Depth);
              DebugPrint("EntityType_Enemy  | EntityType_PlayerProton");
            } break;

            case Collision_Enemy_EnemyProjectile:
            {
              DebugPrint("Collision_Enemy_EnemyProjectile ", Depth);
              DebugPrint("EntityType_Enemy  | EntityType_EnemyProjectile");
            } break;

            case Collision_Enemy_Enemy:
            {
              DebugPrint("Collision_Enemy_Enemy ", Depth);
              DebugPrint("EntityType_Enemy");
            } break;

      }
    }

    function void DebugPrint( model_index EnumValue, u32 Depth)
    {
      switch (EnumValue)
      {
        
            case ModelIndex_None:
            {
              DebugPrint("ModelIndex_None ", Depth);
              DebugPrint("");
            } break;

            case ModelIndex_Enemy:
            {
              DebugPrint("ModelIndex_Enemy ", Depth);
              DebugPrint("EntityType_Enemy");
            } break;

            case ModelIndex_Player:
            {
              DebugPrint("ModelIndex_Player ", Depth);
              DebugPrint("EntityType_Player");
            } break;

            case ModelIndex_Loot:
            {
              DebugPrint("ModelIndex_Loot ", Depth);
              DebugPrint("EntityType_Loot");
            } break;

            case ModelIndex_Projectile:
            {
              DebugPrint("ModelIndex_Projectile ", Depth);
              DebugPrint("");
            } break;

            case ModelIndex_Proton:
            {
              DebugPrint("ModelIndex_Proton ", Depth);
              DebugPrint("");
            } break;

            case ModelIndex_Count:
            {
              DebugPrint("ModelIndex_Count ", Depth);
              DebugPrint("");
            } break;

      }
    }

    function void DebugPrint( heap_allocation_type EnumValue, u32 Depth)
    {
      switch (EnumValue)
      {
        
            case AllocationType_Free:
            {
              DebugPrint("AllocationType_Free ", Depth);
              DebugPrint("");
            } break;

            case AllocationType_Reserved:
            {
              DebugPrint("AllocationType_Reserved ", Depth);
              DebugPrint("");
            } break;

      }
    }

    function void DebugPrint( mutex_op EnumValue, u32 Depth)
    {
      switch (EnumValue)
      {
        
            case MutexOp_Waiting:
            {
              DebugPrint("MutexOp_Waiting ", Depth);
              DebugPrint("");
            } break;

            case MutexOp_Aquired:
            {
              DebugPrint("MutexOp_Aquired ", Depth);
              DebugPrint("");
            } break;

            case MutexOp_Released:
            {
              DebugPrint("MutexOp_Released ", Depth);
              DebugPrint("");
            } break;

      }
    }

    function void DebugPrint( Sign EnumValue, u32 Depth)
    {
      switch (EnumValue)
      {
        
            case Negative:
            {
              DebugPrint("Negative ", Depth);
              DebugPrint("-1");
            } break;

            case Zero:
            {
              DebugPrint("Zero ", Depth);
              DebugPrint("0");
            } break;

            case Positive:
            {
              DebugPrint("Positive ", Depth);
              DebugPrint("1");
            } break;

      }
    }

    function void DebugPrint( Chunk_ID EnumValue, u32 Depth)
    {
      switch (EnumValue)
      {
        
            case ID_NONE:
            {
              DebugPrint("ID_NONE ", Depth);
              DebugPrint("0");
            } break;

            case ID_VOX:
            {
              DebugPrint("ID_VOX ", Depth);
              DebugPrint("' XOV'");
            } break;

            case ID_MAIN:
            {
              DebugPrint("ID_MAIN ", Depth);
              DebugPrint("'NIAM'");
            } break;

            case ID_PACK:
            {
              DebugPrint("ID_PACK ", Depth);
              DebugPrint("'KCAP'");
            } break;

            case ID_SIZE:
            {
              DebugPrint("ID_SIZE ", Depth);
              DebugPrint("'EZIS'");
            } break;

            case ID_XYZI:
            {
              DebugPrint("ID_XYZI ", Depth);
              DebugPrint("'IZYX'");
            } break;

            case ID_RGBA:
            {
              DebugPrint("ID_RGBA ", Depth);
              DebugPrint("'ABGR'");
            } break;

      }
    }

    function void DebugPrint( ui_render_command_type EnumValue, u32 Depth)
    {
      switch (EnumValue)
      {
        
            case type_ui_render_command_noop:
            {
              DebugPrint("type_ui_render_command_noop ", Depth);
              DebugPrint("");
            } break;

            case type_ui_render_command_window_start:
            {
              DebugPrint("type_ui_render_command_window_start ", Depth);
              DebugPrint("");
            } break;

            case type_ui_render_command_window_end:
            {
              DebugPrint("type_ui_render_command_window_end ", Depth);
              DebugPrint("");
            } break;

            case type_ui_render_command_button_start:
            {
              DebugPrint("type_ui_render_command_button_start ", Depth);
              DebugPrint("");
            } break;

            case type_ui_render_command_button_end:
            {
              DebugPrint("type_ui_render_command_button_end ", Depth);
              DebugPrint("");
            } break;

            case type_ui_render_command_table_start:
            {
              DebugPrint("type_ui_render_command_table_start ", Depth);
              DebugPrint("");
            } break;

            case type_ui_render_command_column_start:
            {
              DebugPrint("type_ui_render_command_column_start ", Depth);
              DebugPrint("");
            } break;

            case type_ui_render_command_column_end:
            {
              DebugPrint("type_ui_render_command_column_end ", Depth);
              DebugPrint("");
            } break;

            case type_ui_render_command_text:
            {
              DebugPrint("type_ui_render_command_text ", Depth);
              DebugPrint("");
            } break;

            case type_ui_render_command_text_at:
            {
              DebugPrint("type_ui_render_command_text_at ", Depth);
              DebugPrint("");
            } break;

            case type_ui_render_command_textured_quad:
            {
              DebugPrint("type_ui_render_command_textured_quad ", Depth);
              DebugPrint("");
            } break;

            case type_ui_render_command_untextured_quad:
            {
              DebugPrint("type_ui_render_command_untextured_quad ", Depth);
              DebugPrint("");
            } break;

            case type_ui_render_command_untextured_quad_at:
            {
              DebugPrint("type_ui_render_command_untextured_quad_at ", Depth);
              DebugPrint("");
            } break;

            case type_ui_render_command_border:
            {
              DebugPrint("type_ui_render_command_border ", Depth);
              DebugPrint("");
            } break;

            case type_ui_render_command_new_row:
            {
              DebugPrint("type_ui_render_command_new_row ", Depth);
              DebugPrint("");
            } break;

            case type_ui_render_command_table_end:
            {
              DebugPrint("type_ui_render_command_table_end ", Depth);
              DebugPrint("");
            } break;

      }
    }

    function void DebugPrint( struct_member_function_type EnumValue, u32 Depth)
    {
      switch (EnumValue)
      {
        
            case type_struct_member_function_noop:
            {
              DebugPrint("type_struct_member_function_noop ", Depth);
              DebugPrint("");
            } break;

            case type_struct_member_function_normal:
            {
              DebugPrint("type_struct_member_function_normal ", Depth);
              DebugPrint("");
            } break;

            case type_struct_member_function_operator:
            {
              DebugPrint("type_struct_member_function_operator ", Depth);
              DebugPrint("");
            } break;

            case type_struct_member_function_constructor:
            {
              DebugPrint("type_struct_member_function_constructor ", Depth);
              DebugPrint("");
            } break;

            case type_struct_member_function_destructor:
            {
              DebugPrint("type_struct_member_function_destructor ", Depth);
              DebugPrint("");
            } break;

      }
    }

    function void DebugPrint( struct_member_type EnumValue, u32 Depth)
    {
      switch (EnumValue)
      {
        
            case type_struct_member_noop:
            {
              DebugPrint("type_struct_member_noop ", Depth);
              DebugPrint("");
            } break;

            case type_variable:
            {
              DebugPrint("type_variable ", Depth);
              DebugPrint("");
            } break;

            case type_struct_member_function:
            {
              DebugPrint("type_struct_member_function ", Depth);
              DebugPrint("");
            } break;

            case type_struct_member_anonymous:
            {
              DebugPrint("type_struct_member_anonymous ", Depth);
              DebugPrint("");
            } break;

      }
    }

    function void DebugPrint( ast_node_type EnumValue, u32 Depth)
    {
      switch (EnumValue)
      {
        
            case type_ast_node_noop:
            {
              DebugPrint("type_ast_node_noop ", Depth);
              DebugPrint("");
            } break;

            case type_ast_node_literal:
            {
              DebugPrint("type_ast_node_literal ", Depth);
              DebugPrint("");
            } break;

            case type_ast_node_expression:
            {
              DebugPrint("type_ast_node_expression ", Depth);
              DebugPrint("");
            } break;

            case type_ast_node_parenthesized:
            {
              DebugPrint("type_ast_node_parenthesized ", Depth);
              DebugPrint("");
            } break;

            case type_ast_node_operator:
            {
              DebugPrint("type_ast_node_operator ", Depth);
              DebugPrint("");
            } break;

            case type_ast_node_function_call:
            {
              DebugPrint("type_ast_node_function_call ", Depth);
              DebugPrint("");
            } break;

            case type_ast_node_return:
            {
              DebugPrint("type_ast_node_return ", Depth);
              DebugPrint("");
            } break;

            case type_ast_node_scope:
            {
              DebugPrint("type_ast_node_scope ", Depth);
              DebugPrint("");
            } break;

            case type_ast_node_initializer_list:
            {
              DebugPrint("type_ast_node_initializer_list ", Depth);
              DebugPrint("");
            } break;

            case type_ast_node_ignored:
            {
              DebugPrint("type_ast_node_ignored ", Depth);
              DebugPrint("");
            } break;

            case type_ast_node_symbol:
            {
              DebugPrint("type_ast_node_symbol ", Depth);
              DebugPrint("");
            } break;

            case type_ast_node_variable_def:
            {
              DebugPrint("type_ast_node_variable_def ", Depth);
              DebugPrint("");
            } break;

      }
    }

    function void DebugPrint( work_queue_entry_type EnumValue, u32 Depth)
    {
      switch (EnumValue)
      {
        
            case type_work_queue_entry_noop:
            {
              DebugPrint("type_work_queue_entry_noop ", Depth);
              DebugPrint("");
            } break;

            case type_work_queue_entry_init_world_chunk:
            {
              DebugPrint("type_work_queue_entry_init_world_chunk ", Depth);
              DebugPrint("");
            } break;

            case type_work_queue_entry_copy_buffer:
            {
              DebugPrint("type_work_queue_entry_copy_buffer ", Depth);
              DebugPrint("");
            } break;

      }
    }

