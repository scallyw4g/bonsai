
      // Source File : src/debug_system/headers/debug.h
      function void DebugPrint(cycle_range S)
      {
        
          DebugPrint("StartCycle");
          DebugPrint(S.StartCycle);

          DebugPrint("TotalCycles");
          DebugPrint(S.TotalCycles);

      }

      // Source File : src/debug_system/headers/debug.h
      function void DebugPrint(memory_arena_stats S)
      {
        
          DebugPrint("Allocations");
          DebugPrint(S.Allocations);

          DebugPrint("Pushes");
          DebugPrint(S.Pushes);

          DebugPrint("TotalAllocated");
          DebugPrint(S.TotalAllocated);

          DebugPrint("Remaining");
          DebugPrint(S.Remaining);

      }

      // Source File : src/debug_system/headers/debug.h
      function void DebugPrint(min_max_avg_dt S)
      {
        
          DebugPrint("Min");
          DebugPrint(S.Min);

          DebugPrint("Max");
          DebugPrint(S.Max);

          DebugPrint("Avg");
          DebugPrint(S.Avg);

      }

      // Source File : src/debug_system/headers/debug.h
      function void DebugPrint(debug_ui_render_group S)
      {
        
          DebugPrint("GameGeo");
          DebugPrint(S.GameGeo);

          DebugPrint("GameGeoShader");
          DebugPrint(S.GameGeoShader);

          DebugPrint("TextGroup");
          DebugPrint(S.TextGroup);

          DebugPrint("InteractionStackTop");
          DebugPrint(S.InteractionStackTop);

          DebugPrint("MouseP");
          DebugPrint(S.MouseP);

          DebugPrint("MouseDP");
          DebugPrint(S.MouseDP);

          DebugPrint("ScreenDim");
          DebugPrint(S.ScreenDim);

          DebugPrint("Input");
          DebugPrint(S.Input);

          DebugPrint("HighestWindow");
          DebugPrint(S.HighestWindow);

          DebugPrint("HoverInteractionId");
          DebugPrint(S.HoverInteractionId);

          DebugPrint("ClickedInteractionId");
          DebugPrint(S.ClickedInteractionId);

          DebugPrint("PressedInteractionId");
          DebugPrint(S.PressedInteractionId);

          DebugPrint("Geo");
          DebugPrint(S.Geo);

          DebugPrint("CommandBuffer");
          DebugPrint(S.CommandBuffer);

      }

      // Source File : src/debug_system/headers/debug.h
      function void DebugPrint(debug_profile_scope S)
      {
        
          DebugPrint("CycleCount");
          DebugPrint(S.CycleCount);

          DebugPrint("StartingCycle");
          DebugPrint(S.StartingCycle);

          DebugPrint("Name");
          DebugPrint(S.Name);

          DebugPrint("Expanded");
          DebugPrint(S.Expanded);

          DebugPrint("Sibling");
          DebugPrint(S.Sibling);

          DebugPrint("Child");
          DebugPrint(S.Child);

          DebugPrint("Parent");
          DebugPrint(S.Parent);

      }

      // Source File : src/debug_system/headers/debug.h
      function void DebugPrint(unique_debug_profile_scope S)
      {
        
          DebugPrint("Name");
          DebugPrint(S.Name);

          DebugPrint("CallCount");
          DebugPrint(S.CallCount);

          DebugPrint("TotalCycles");
          DebugPrint(S.TotalCycles);

          DebugPrint("MinCycles");
          DebugPrint(S.MinCycles);

          DebugPrint("MaxCycles");
          DebugPrint(S.MaxCycles);

          DebugPrint("Scope");
          DebugPrint(S.Scope);

          DebugPrint("NextUnique");
          DebugPrint(S.NextUnique);

      }

      // Source File : src/debug_system/headers/debug.h
      function void DebugPrint(debug_scope_tree S)
      {
        
          DebugPrint("Root");
          DebugPrint(S.Root);

          DebugPrint("WriteScope");
          DebugPrint(S.WriteScope);

          DebugPrint("ParentOfNextScope");
          DebugPrint(S.ParentOfNextScope);

          DebugPrint("FrameRecorded");
          DebugPrint(S.FrameRecorded);

      }

      // Source File : src/debug_system/headers/debug.h
      function void DebugPrint(debug_thread_state S)
      {
        
          DebugPrint("Memory");
          DebugPrint(S.Memory);

          DebugPrint("MetaTable");
          DebugPrint(S.MetaTable);

          DebugPrint("ScopeTrees");
          DebugPrint(S.ScopeTrees);

          DebugPrint("FirstFreeScope");
          DebugPrint(S.FirstFreeScope);

          DebugPrint("MutexOps");
          DebugPrint(S.MutexOps);

          DebugPrint("WriteIndex");
          DebugPrint(S.WriteIndex);

          DebugPrint("Pad");
          DebugPrint(S.Pad);

      }

      // Source File : src/debug_system/headers/debug.h
      function void DebugPrint(registered_memory_arena S)
      {
        
          DebugPrint("Arena");
          DebugPrint(S.Arena);

          DebugPrint("Name");
          DebugPrint(S.Name);

          DebugPrint("Expanded");
          DebugPrint(S.Expanded);

      }

      // Source File : src/debug_system/headers/debug.h
      function void DebugPrint(selected_memory_arena S)
      {
        
          DebugPrint("ArenaHash");
          DebugPrint(S.ArenaHash);

          DebugPrint("HeadArenaHash");
          DebugPrint(S.HeadArenaHash);

      }

      // Source File : src/debug_system/headers/debug.h
      function void DebugPrint(selected_arenas S)
      {
        
          DebugPrint("Count");
          DebugPrint(S.Count);

          DebugPrint("Arenas");
          DebugPrint(S.Arenas);

      }

      // Source File : src/debug_system/headers/debug.h
      function void DebugPrint(frame_stats S)
      {
        
          DebugPrint("TotalCycles");
          DebugPrint(S.TotalCycles);

          DebugPrint("StartingCycle");
          DebugPrint(S.StartingCycle);

          DebugPrint("FrameMs");
          DebugPrint(S.FrameMs);

      }

      // Source File : src/debug_system/headers/debug.h
      function void DebugPrint(called_function S)
      {
        
          DebugPrint("Name");
          DebugPrint(S.Name);

          DebugPrint("CallCount");
          DebugPrint(S.CallCount);

      }

      // Source File : src/debug_system/headers/debug.h
      function void DebugPrint(debug_draw_call S)
      {
        
          DebugPrint("Caller");
          DebugPrint(S.Caller);

          DebugPrint("N");
          DebugPrint(S.N);

          DebugPrint("Calls");
          DebugPrint(S.Calls);

      }

      // Source File : src/debug_system/headers/debug.h
      function void DebugPrint(debug_timed_function S)
      {
        
          DebugPrint("Scope");
          DebugPrint(S.Scope);

          DebugPrint("Tree");
          DebugPrint(S.Tree);

      }

      // Source File : src/debug_system/headers/debug_ui.h
      function void DebugPrint(window_layout S)
      {
        
          DebugPrint("Title");
          DebugPrint(S.Title);

          DebugPrint("Basis");
          DebugPrint(S.Basis);

          DebugPrint("MaxClip");
          DebugPrint(S.MaxClip);

          DebugPrint("InteractionStackIndex");
          DebugPrint(S.InteractionStackIndex);

          DebugPrint("zBackground");
          DebugPrint(S.zBackground);

          DebugPrint("zText");
          DebugPrint(S.zText);

          DebugPrint("zBorder");
          DebugPrint(S.zBorder);

          DebugPrint("zTitleBar");
          DebugPrint(S.zTitleBar);

          DebugPrint("NextHotWindow");
          DebugPrint(S.NextHotWindow);

      }

      // Source File : src/debug_system/headers/debug_ui.h
      function void DebugPrint(font S)
      {
        
          DebugPrint("Size");
          DebugPrint(S.Size);

      }

      // Source File : src/debug_system/headers/debug_ui.h
      function void DebugPrint(ui_element_reference S)
      {
        
          DebugPrint("Index");
          DebugPrint(S.Index);

      }

      // Source File : src/debug_system/headers/debug_ui.h
      function void DebugPrint(ui_style S)
      {
        
          DebugPrint("Color");
          DebugPrint(S.Color);

          DebugPrint("HoverColor");
          DebugPrint(S.HoverColor);

          DebugPrint("PressedColor");
          DebugPrint(S.PressedColor);

          DebugPrint("ClickedColor");
          DebugPrint(S.ClickedColor);

          DebugPrint("ActiveColor");
          DebugPrint(S.ActiveColor);

          DebugPrint("Font");
          DebugPrint(S.Font);

          DebugPrint("IsActive");
          DebugPrint(S.IsActive);

      }

      // Source File : src/debug_system/headers/debug_ui.h
      function void DebugPrint(layout S)
      {
        
          DebugPrint("Basis");
          DebugPrint(S.Basis);

          DebugPrint("At");
          DebugPrint(S.At);

          DebugPrint("DrawBounds");
          DebugPrint(S.DrawBounds);

          DebugPrint("Padding");
          DebugPrint(S.Padding);

          DebugPrint("Prev");
          DebugPrint(S.Prev);

      }

      // Source File : src/debug_system/headers/debug_ui.h
      function void DebugPrint(ui_render_command_border S)
      {
        
          DebugPrint("Window");
          DebugPrint(S.Window);

          DebugPrint("Bounds");
          DebugPrint(S.Bounds);

          DebugPrint("Color");
          DebugPrint(S.Color);

      }

      // Source File : src/debug_system/headers/debug_ui.h
      function void DebugPrint(ui_render_command_window_start S)
      {
        
          DebugPrint("Layout");
          DebugPrint(S.Layout);

          DebugPrint("Window");
          DebugPrint(S.Window);

      }

      // Source File : src/debug_system/headers/debug_ui.h
      function void DebugPrint(ui_render_command_window_end S)
      {
        
          DebugPrint("Window");
          DebugPrint(S.Window);

      }

      // Source File : src/debug_system/headers/debug_ui.h
      function void DebugPrint(ui_render_command_column_start S)
      {
        
          DebugPrint("Layout");
          DebugPrint(S.Layout);

          DebugPrint("Style");
          DebugPrint(S.Style);

          DebugPrint("Width");
          DebugPrint(S.Width);

          DebugPrint("MaxWidth");
          DebugPrint(S.MaxWidth);

          DebugPrint("Params");
          DebugPrint(S.Params);

      }

      // Source File : src/debug_system/headers/debug_ui.h
      function void DebugPrint(ui_render_command_text S)
      {
        
          DebugPrint("Layout");
          DebugPrint(S.Layout);

          DebugPrint("Style");
          DebugPrint(S.Style);

          DebugPrint("String");
          DebugPrint(S.String);

      }

      // Source File : src/debug_system/headers/debug_ui.h
      function void DebugPrint(ui_render_command_text_at S)
      {
        
          DebugPrint("Text");
          DebugPrint(S.Text);

          DebugPrint("At");
          DebugPrint(S.At);

          DebugPrint("MaxClip");
          DebugPrint(S.MaxClip);

      }

      // Source File : src/debug_system/headers/debug_ui.h
      function void DebugPrint(ui_render_command_untextured_quad S)
      {
        
          DebugPrint("Layout");
          DebugPrint(S.Layout);

          DebugPrint("Style");
          DebugPrint(S.Style);

          DebugPrint("QuadDim");
          DebugPrint(S.QuadDim);

          DebugPrint("zDepth");
          DebugPrint(S.zDepth);

          DebugPrint("Params");
          DebugPrint(S.Params);

      }

      // Source File : src/debug_system/headers/debug_ui.h
      function void DebugPrint(ui_render_command_untextured_quad_at S)
      {
        
          DebugPrint("Layout");
          DebugPrint(S.Layout);

          DebugPrint("Style");
          DebugPrint(S.Style);

          DebugPrint("QuadDim");
          DebugPrint(S.QuadDim);

          DebugPrint("zDepth");
          DebugPrint(S.zDepth);

      }

      // Source File : src/debug_system/headers/debug_ui.h
      function void DebugPrint(ui_render_command_textured_quad S)
      {
        
          DebugPrint("Layout");
          DebugPrint(S.Layout);

          DebugPrint("TextureSlice");
          DebugPrint(S.TextureSlice);

          DebugPrint("zDepth");
          DebugPrint(S.zDepth);

      }

      // Source File : src/debug_system/headers/debug_ui.h
      function void DebugPrint(ui_render_command_button_start S)
      {
        
          DebugPrint("ID");
          DebugPrint(S.ID);

          DebugPrint("Style");
          DebugPrint(S.Style);

      }

      // Source File : src/debug_system/headers/debug_ui.h
      function void DebugPrint(ui_render_command_button_end S)
      {
        
          DebugPrint("Params");
          DebugPrint(S.Params);

      }

      // Source File : src/debug_system/headers/debug_ui.h
      function void DebugPrint(ui_render_command_table_start S)
      {
        
          DebugPrint("Layout");
          DebugPrint(S.Layout);

          DebugPrint("Style");
          DebugPrint(S.Style);

          DebugPrint("Position");
          DebugPrint(S.Position);

          DebugPrint("RelativeTo");
          DebugPrint(S.RelativeTo);

      }

      // Source File : src/debug_system/headers/debug_ui.h
      function void DebugPrint(ui_render_command_buffer S)
      {
        
          DebugPrint("CommandCount");
          DebugPrint(S.CommandCount);

          DebugPrint("Commands");
          DebugPrint(S.Commands);

      }

      // Source File : src/debug_system/headers/debug_ui.h
      function void DebugPrint(clip_result S)
      {
        
          DebugPrint("ClipStatus");
          DebugPrint(S.ClipStatus);

          DebugPrint("MaxClip");
          DebugPrint(S.MaxClip);

          DebugPrint("PartialClip");
          DebugPrint(S.PartialClip);

      }

      // Source File : src/debug_system/headers/debug_ui.h
      function void DebugPrint(sort_key S)
      {
        
          DebugPrint("Value");
          DebugPrint(S.Value);

          DebugPrint("Index");
          DebugPrint(S.Index);

      }

      // Source File : src/debug_system/headers/debug_ui.h
      function void DebugPrint(window_sort_params S)
      {
        
          DebugPrint("Count");
          DebugPrint(S.Count);

          DebugPrint("LowestInteractionStackIndex");
          DebugPrint(S.LowestInteractionStackIndex);

          DebugPrint("SortKeys");
          DebugPrint(S.SortKeys);

      }

      // Source File : src/debug_system/headers/debug_ui.h
      function void DebugPrint(find_button_start_result S)
      {
        
          DebugPrint("Index");
          DebugPrint(S.Index);

          DebugPrint("Command");
          DebugPrint(S.Command);

      }

      // Source File : src/debug_system/headers/debug_ui.h
      function void DebugPrint(find_command_result S)
      {
        
          DebugPrint("Command");
          DebugPrint(S.Command);

          DebugPrint("Index");
          DebugPrint(S.Index);

      }

      // Source File : src/debug_system/headers/interactable.h
      function void DebugPrint(interactable_handle S)
      {
        
          DebugPrint("Id");
          DebugPrint(S.Id);

      }

      // Source File : src/debug_system/headers/interactable.h
      function void DebugPrint(interactable S)
      {
        
          DebugPrint("ID");
          DebugPrint(S.ID);

          DebugPrint("MinP");
          DebugPrint(S.MinP);

          DebugPrint("MaxP");
          DebugPrint(S.MaxP);

          DebugPrint("Window");
          DebugPrint(S.Window);

      }

      // Source File : src/debug_system/headers/interactable.h
      function void DebugPrint(button_interaction_result S)
      {
        
          DebugPrint("Pressed");
          DebugPrint(S.Pressed);

          DebugPrint("Clicked");
          DebugPrint(S.Clicked);

          DebugPrint("Hover");
          DebugPrint(S.Hover);

      }

      // Source File : src/debug_system/headers/debug_render.h
      function void DebugPrint(render_state S)
      {
        
          DebugPrint("Window");
          DebugPrint(S.Window);

          DebugPrint("WindowStartCommandIndex");
          DebugPrint(S.WindowStartCommandIndex);

          DebugPrint("Layout");
          DebugPrint(S.Layout);

          DebugPrint("Hover");
          DebugPrint(S.Hover);

          DebugPrint("Pressed");
          DebugPrint(S.Pressed);

          DebugPrint("Clicked");
          DebugPrint(S.Clicked);

      }

      // Source File : src/debug_system/headers/debug_render.h
      function void DebugPrint(debug_text_render_group S)
      {
        
          DebugPrint("SolidUIVertexBuffer");
          DebugPrint(S.SolidUIVertexBuffer);

          DebugPrint("SolidUIColorBuffer");
          DebugPrint(S.SolidUIColorBuffer);

          DebugPrint("SolidUIUVBuffer");
          DebugPrint(S.SolidUIUVBuffer);

          DebugPrint("FontTexture");
          DebugPrint(S.FontTexture);

          DebugPrint("Text2DShader");
          DebugPrint(S.Text2DShader);

          DebugPrint("TextTextureUniform");
          DebugPrint(S.TextTextureUniform);

          DebugPrint("Geo");
          DebugPrint(S.Geo);

          DebugPrint("DebugFontTextureShader");
          DebugPrint(S.DebugFontTextureShader);

          DebugPrint("SolidUIShader");
          DebugPrint(S.SolidUIShader);

      }

      // Source File : src/metaprogramming/preprocessor.h
      function void DebugPrint(c_decl_variable S)
      {
        
          DebugPrint("Type");
          DebugPrint(S.Type);

          DebugPrint("Name");
          DebugPrint(S.Name);

      }

      // Source File : src/metaprogramming/preprocessor.h
      function void DebugPrint(c_decl_stream S)
      {
        
          DebugPrint("FirstChunk");
          DebugPrint(S.FirstChunk);

          DebugPrint("LastChunk");
          DebugPrint(S.LastChunk);

      }

      // Source File : src/metaprogramming/preprocessor.h
      function void DebugPrint(struct_def S)
      {
        
          DebugPrint("Name");
          DebugPrint(S.Name);

          DebugPrint("DefinedInFile");
          DebugPrint(S.DefinedInFile);

          DebugPrint("Fields");
          DebugPrint(S.Fields);

      }

      // Source File : src/metaprogramming/preprocessor.h
      function void DebugPrint(c_decl_union S)
      {
        
          DebugPrint("Body");
          DebugPrint(S.Body);

      }

      // Source File : src/metaprogramming/preprocessor.h
      function void DebugPrint(enum_field S)
      {
        
          DebugPrint("Name");
          DebugPrint(S.Name);

          DebugPrint("Value");
          DebugPrint(S.Value);

      }

      // Source File : src/metaprogramming/preprocessor.h
      function void DebugPrint(d_union_member S)
      {
        
          DebugPrint("Type");
          DebugPrint(S.Type);

          DebugPrint("Name");
          DebugPrint(S.Name);

          DebugPrint("Flags");
          DebugPrint(S.Flags);

      }

      // Source File : src/metaprogramming/preprocessor.h
      function void DebugPrint(c_decl_stream_chunk S)
      {
        
          DebugPrint("Element");
          DebugPrint(S.Element);

          DebugPrint("Next");
          DebugPrint(S.Next);

      }

      // Source File : src/metaprogramming/preprocessor.h
      function void DebugPrint(enum_def S)
      {
        
          DebugPrint("Name");
          DebugPrint(S.Name);

          DebugPrint("Fields");
          DebugPrint(S.Fields);

      }

      // Source File : src/metaprogramming/preprocessor.h
      function void DebugPrint(c_token S)
      {
        
          DebugPrint("Type");
          DebugPrint(S.Type);

          DebugPrint("Value");
          DebugPrint(S.Value);

      }

      // Source File : src/metaprogramming/preprocessor.h
      function void DebugPrint(d_union_decl S)
      {
        
          DebugPrint("Name");
          DebugPrint(S.Name);

          DebugPrint("Members");
          DebugPrint(S.Members);

          DebugPrint("CommonMembers");
          DebugPrint(S.CommonMembers);

          DebugPrint("CustomEnumType");
          DebugPrint(S.CustomEnumType);

      }

      // Source File : src/metaprogramming/preprocessor.h
      function void DebugPrint(c_parse_result S)
      {
        
          DebugPrint("Valid");
          DebugPrint(S.Valid);

          DebugPrint("Tokens");
          DebugPrint(S.Tokens);

          DebugPrint("OutputTokens");
          DebugPrint(S.OutputTokens);

          DebugPrint("Filename");
          DebugPrint(S.Filename);

          DebugPrint("LineNumber");
          DebugPrint(S.LineNumber);

      }

      // Source File : src/metaprogramming/preprocessor.h
      function void DebugPrint(todo S)
      {
        
          DebugPrint("Id");
          DebugPrint(S.Id);

          DebugPrint("Value");
          DebugPrint(S.Value);

          DebugPrint("FoundInCodebase");
          DebugPrint(S.FoundInCodebase);

      }

      // Source File : src/metaprogramming/preprocessor.h
      function void DebugPrint(tag S)
      {
        
          DebugPrint("Tag");
          DebugPrint(S.Tag);

          DebugPrint("Todos");
          DebugPrint(S.Todos);

      }

      // Source File : src/metaprogramming/preprocessor.h
      function void DebugPrint(person S)
      {
        
          DebugPrint("Name");
          DebugPrint(S.Name);

          DebugPrint("Tags");
          DebugPrint(S.Tags);

      }

      // Source File : src/metaprogramming/preprocessor.h
      function void DebugPrint(arguments S)
      {
        
          DebugPrint("OutPath");
          DebugPrint(S.OutPath);

          DebugPrint("Files");
          DebugPrint(S.Files);

          DebugPrint("DoDebugWindow");
          DebugPrint(S.DoDebugWindow);

      }

      // Source File : src/metaprogramming/preprocessor.h
      function void DebugPrint(program_datatypes S)
      {
        
          DebugPrint("Structs");
          DebugPrint(S.Structs);

          DebugPrint("Enums");
          DebugPrint(S.Enums);

      }

      // Source File : src/metaprogramming/preprocessor.h
      function void DebugPrint(c_decl_iterator S)
      {
        
          DebugPrint("At");
          DebugPrint(S.At);

      }

      // Source File : src/metaprogramming/output/generate_cursor_c_token.h
      function void DebugPrint(c_token_cursor S)
      {
        
          DebugPrint("Start");
          DebugPrint(S.Start);

          DebugPrint("End");
          DebugPrint(S.End);

          DebugPrint("At");
          DebugPrint(S.At);

      }

      // Source File : src/metaprogramming/output/generate_stream_enum_field.h
      function void DebugPrint(enum_field_stream_chunk S)
      {
        
          DebugPrint("Element");
          DebugPrint(S.Element);

          DebugPrint("Next");
          DebugPrint(S.Next);

      }

      // Source File : src/metaprogramming/output/generate_stream_enum_field.h
      function void DebugPrint(enum_field_stream S)
      {
        
          DebugPrint("FirstChunk");
          DebugPrint(S.FirstChunk);

          DebugPrint("LastChunk");
          DebugPrint(S.LastChunk);

      }

      // Source File : src/metaprogramming/output/generate_stream_enum_field.h
      function void DebugPrint(enum_field_iterator S)
      {
        
          DebugPrint("Stream");
          DebugPrint(S.Stream);

          DebugPrint("At");
          DebugPrint(S.At);

      }

      // Source File : src/metaprogramming/output/generate_cursor_enum_def.h
      function void DebugPrint(enum_def_cursor S)
      {
        
          DebugPrint("Start");
          DebugPrint(S.Start);

          DebugPrint("End");
          DebugPrint(S.End);

          DebugPrint("At");
          DebugPrint(S.At);

      }

      // Source File : src/metaprogramming/output/generate_stream_enum_def.h
      function void DebugPrint(enum_def_stream_chunk S)
      {
        
          DebugPrint("Element");
          DebugPrint(S.Element);

          DebugPrint("Next");
          DebugPrint(S.Next);

      }

      // Source File : src/metaprogramming/output/generate_stream_enum_def.h
      function void DebugPrint(enum_def_stream S)
      {
        
          DebugPrint("FirstChunk");
          DebugPrint(S.FirstChunk);

          DebugPrint("LastChunk");
          DebugPrint(S.LastChunk);

      }

      // Source File : src/metaprogramming/output/generate_stream_enum_def.h
      function void DebugPrint(enum_def_iterator S)
      {
        
          DebugPrint("Stream");
          DebugPrint(S.Stream);

          DebugPrint("At");
          DebugPrint(S.At);

      }

      // Source File : src/metaprogramming/output/generate_cursor_v3.h
      function void DebugPrint(v3_cursor S)
      {
        
          DebugPrint("Start");
          DebugPrint(S.Start);

          DebugPrint("End");
          DebugPrint(S.End);

          DebugPrint("At");
          DebugPrint(S.At);

      }

      // Source File : src/metaprogramming/output/d_union_c_decl.h
      function void DebugPrint(c_decl S)
      {
        
          DebugPrint("Type");
          DebugPrint(S.Type);

      }

      // Source File : src/metaprogramming/output/generate_stream_d_union_member.h
      function void DebugPrint(d_union_member_stream_chunk S)
      {
        
          DebugPrint("Element");
          DebugPrint(S.Element);

          DebugPrint("Next");
          DebugPrint(S.Next);

      }

      // Source File : src/metaprogramming/output/generate_stream_d_union_member.h
      function void DebugPrint(d_union_member_stream S)
      {
        
          DebugPrint("FirstChunk");
          DebugPrint(S.FirstChunk);

          DebugPrint("LastChunk");
          DebugPrint(S.LastChunk);

      }

      // Source File : src/metaprogramming/output/generate_stream_d_union_member.h
      function void DebugPrint(d_union_member_iterator S)
      {
        
          DebugPrint("Stream");
          DebugPrint(S.Stream);

          DebugPrint("At");
          DebugPrint(S.At);

      }

      // Source File : src/metaprogramming/output/d_union_ui_render_command.h
      function void DebugPrint(ui_render_command S)
      {
        
          DebugPrint("Type");
          DebugPrint(S.Type);

      }

      // Source File : src/metaprogramming/output/generate_stream_struct_def.h
      function void DebugPrint(struct_def_stream_chunk S)
      {
        
          DebugPrint("Element");
          DebugPrint(S.Element);

          DebugPrint("Next");
          DebugPrint(S.Next);

      }

      // Source File : src/metaprogramming/output/generate_stream_struct_def.h
      function void DebugPrint(struct_def_stream S)
      {
        
          DebugPrint("FirstChunk");
          DebugPrint(S.FirstChunk);

          DebugPrint("LastChunk");
          DebugPrint(S.LastChunk);

      }

      // Source File : src/metaprogramming/output/generate_stream_struct_def.h
      function void DebugPrint(struct_def_iterator S)
      {
        
          DebugPrint("Stream");
          DebugPrint(S.Stream);

          DebugPrint("At");
          DebugPrint(S.At);

      }

      // Source File : src/metaprogramming/output/generate_cursor_counted_string.h
      function void DebugPrint(counted_string_cursor S)
      {
        
          DebugPrint("Start");
          DebugPrint(S.Start);

          DebugPrint("End");
          DebugPrint(S.End);

          DebugPrint("At");
          DebugPrint(S.At);

      }

      // Source File : src/metaprogramming/output/generate_stream_tag.h
      function void DebugPrint(tag_stream_chunk S)
      {
        
          DebugPrint("Element");
          DebugPrint(S.Element);

          DebugPrint("Next");
          DebugPrint(S.Next);

      }

      // Source File : src/metaprogramming/output/generate_stream_tag.h
      function void DebugPrint(tag_stream S)
      {
        
          DebugPrint("FirstChunk");
          DebugPrint(S.FirstChunk);

          DebugPrint("LastChunk");
          DebugPrint(S.LastChunk);

      }

      // Source File : src/metaprogramming/output/generate_stream_tag.h
      function void DebugPrint(tag_iterator S)
      {
        
          DebugPrint("Stream");
          DebugPrint(S.Stream);

          DebugPrint("At");
          DebugPrint(S.At);

      }

      // Source File : src/metaprogramming/output/generate_cursor_c_parse_result.h
      function void DebugPrint(c_parse_result_cursor S)
      {
        
          DebugPrint("Start");
          DebugPrint(S.Start);

          DebugPrint("End");
          DebugPrint(S.End);

          DebugPrint("At");
          DebugPrint(S.At);

      }

      // Source File : src/metaprogramming/output/d_union_c_decl_function.h
      function void DebugPrint(c_decl_function S)
      {
        
          DebugPrint("Type");
          DebugPrint(S.Type);

      }

      // Source File : src/metaprogramming/output/generate_cursor_c_decl.h
      function void DebugPrint(c_decl_cursor S)
      {
        
          DebugPrint("Start");
          DebugPrint(S.Start);

          DebugPrint("End");
          DebugPrint(S.End);

          DebugPrint("At");
          DebugPrint(S.At);

      }

      // Source File : src/metaprogramming/output/generate_cursor_struct_def.h
      function void DebugPrint(struct_def_cursor S)
      {
        
          DebugPrint("Start");
          DebugPrint(S.Start);

          DebugPrint("End");
          DebugPrint(S.End);

          DebugPrint("At");
          DebugPrint(S.At);

      }

      // Source File : src/metaprogramming/output/generate_stream_person.h
      function void DebugPrint(person_stream_chunk S)
      {
        
          DebugPrint("Element");
          DebugPrint(S.Element);

          DebugPrint("Next");
          DebugPrint(S.Next);

      }

      // Source File : src/metaprogramming/output/generate_stream_person.h
      function void DebugPrint(person_stream S)
      {
        
          DebugPrint("FirstChunk");
          DebugPrint(S.FirstChunk);

          DebugPrint("LastChunk");
          DebugPrint(S.LastChunk);

      }

      // Source File : src/metaprogramming/output/generate_stream_person.h
      function void DebugPrint(person_iterator S)
      {
        
          DebugPrint("Stream");
          DebugPrint(S.Stream);

          DebugPrint("At");
          DebugPrint(S.At);

      }

      // Source File : src/metaprogramming/output/generate_stream_counted_string.h
      function void DebugPrint(counted_string_stream_chunk S)
      {
        
          DebugPrint("Element");
          DebugPrint(S.Element);

          DebugPrint("Next");
          DebugPrint(S.Next);

      }

      // Source File : src/metaprogramming/output/generate_stream_counted_string.h
      function void DebugPrint(counted_string_stream S)
      {
        
          DebugPrint("FirstChunk");
          DebugPrint(S.FirstChunk);

          DebugPrint("LastChunk");
          DebugPrint(S.LastChunk);

      }

      // Source File : src/metaprogramming/output/generate_stream_counted_string.h
      function void DebugPrint(counted_string_iterator S)
      {
        
          DebugPrint("Stream");
          DebugPrint(S.Stream);

          DebugPrint("At");
          DebugPrint(S.At);

      }

      // Source File : src/metaprogramming/output/generate_stream_todo.h
      function void DebugPrint(todo_stream_chunk S)
      {
        
          DebugPrint("Element");
          DebugPrint(S.Element);

          DebugPrint("Next");
          DebugPrint(S.Next);

      }

      // Source File : src/metaprogramming/output/generate_stream_todo.h
      function void DebugPrint(todo_stream S)
      {
        
          DebugPrint("FirstChunk");
          DebugPrint(S.FirstChunk);

          DebugPrint("LastChunk");
          DebugPrint(S.LastChunk);

      }

      // Source File : src/metaprogramming/output/generate_stream_todo.h
      function void DebugPrint(todo_iterator S)
      {
        
          DebugPrint("Stream");
          DebugPrint(S.Stream);

          DebugPrint("At");
          DebugPrint(S.At);

      }

      // Source File : src/metaprogramming/output/d_union_work_queue_entry.h
      function void DebugPrint(work_queue_entry S)
      {
        
          DebugPrint("Type");
          DebugPrint(S.Type);

      }

      // Source File : src/net/network.h
      function void DebugPrint(client_state S)
      {
        
          DebugPrint("Id");
          DebugPrint(S.Id);

          DebugPrint("Counter");
          DebugPrint(S.Counter);

          DebugPrint("P");
          DebugPrint(S.P);

      }

      // Source File : src/net/network.h
      function void DebugPrint(server_state S)
      {
        
          DebugPrint("Clients");
          DebugPrint(S.Clients);

      }

      // Source File : src/net/network.h
      function void DebugPrint(client_to_server_message S)
      {
        
          DebugPrint("Client");
          DebugPrint(S.Client);

      }

      // Source File : src/net/network.h
      function void DebugPrint(handshake_message S)
      {
        
          DebugPrint("ClientId");
          DebugPrint(S.ClientId);

      }

      // Source File : src/net/network.h
      function void DebugPrint(socket_t S)
      {
        
          DebugPrint("Id");
          DebugPrint(S.Id);

          DebugPrint("Type");
          DebugPrint(S.Type);

      }

      // Source File : src/engine/headers/camera.h
      function void DebugPrint(plane S)
      {
        
          DebugPrint("P");
          DebugPrint(S.P);

          DebugPrint("Normal");
          DebugPrint(S.Normal);

          DebugPrint("d");
          DebugPrint(S.d);

      }

      // Source File : src/engine/headers/camera.h
      function void DebugPrint(frustum S)
      {
        
          DebugPrint("farClip");
          DebugPrint(S.farClip);

          DebugPrint("nearClip");
          DebugPrint(S.nearClip);

          DebugPrint("width");
          DebugPrint(S.width);

          DebugPrint("FOV");
          DebugPrint(S.FOV);

          DebugPrint("Top");
          DebugPrint(S.Top);

          DebugPrint("Bot");
          DebugPrint(S.Bot);

          DebugPrint("Left");
          DebugPrint(S.Left);

          DebugPrint("Right");
          DebugPrint(S.Right);

      }

      // Source File : src/engine/headers/camera.h
      function void DebugPrint(camera S)
      {
        
          DebugPrint("Frust");
          DebugPrint(S.Frust);

          DebugPrint("TargetP");
          DebugPrint(S.TargetP);

          DebugPrint("CurrentP");
          DebugPrint(S.CurrentP);

          DebugPrint("ViewingTarget");
          DebugPrint(S.ViewingTarget);

          DebugPrint("RenderSpacePosition");
          DebugPrint(S.RenderSpacePosition);

          DebugPrint("Pitch");
          DebugPrint(S.Pitch);

          DebugPrint("Roll");
          DebugPrint(S.Roll);

          DebugPrint("Yaw");
          DebugPrint(S.Yaw);

          DebugPrint("DistanceFromTarget");
          DebugPrint(S.DistanceFromTarget);

          DebugPrint("Front");
          DebugPrint(S.Front);

          DebugPrint("Right");
          DebugPrint(S.Right);

          DebugPrint("Up");
          DebugPrint(S.Up);

      }

      // Source File : src/engine/headers/work_queue.h
      function void DebugPrint(work_queue_entry_copy_buffer S)
      {
        
          DebugPrint("Src");
          DebugPrint(S.Src);

          DebugPrint("Dest");
          DebugPrint(S.Dest);

          DebugPrint("Basis");
          DebugPrint(S.Basis);

      }

      // Source File : src/engine/headers/work_queue.h
      function void DebugPrint(work_queue_entry_init_world_chunk S)
      {
        
          DebugPrint("Input");
          DebugPrint(S.Input);

      }

      // Source File : src/engine/headers/triangle.h
      function void DebugPrint(edge S)
      {
        
          DebugPrint("P0");
          DebugPrint(S.P0);

          DebugPrint("P1");
          DebugPrint(S.P1);

          DebugPrint("LengthSq");
          DebugPrint(S.LengthSq);

      }

      // Source File : src/engine/headers/triangle.h
      function void DebugPrint(triangle S)
      {
        
          DebugPrint("Points");
          DebugPrint(S.Points);

          DebugPrint("Index");
          DebugPrint(S.Index);

      }

      // Source File : src/engine/headers/triangle.h
      function void DebugPrint(boundary_voxels S)
      {
        
          DebugPrint("Points");
          DebugPrint(S.Points);

          DebugPrint("At");
          DebugPrint(S.At);

          DebugPrint("End");
          DebugPrint(S.End);

          DebugPrint("Min");
          DebugPrint(S.Min);

          DebugPrint("Max");
          DebugPrint(S.Max);

      }

      // Source File : src/engine/headers/triangle.h
      function void DebugPrint(current_triangles S)
      {
        
          DebugPrint("Tris");
          DebugPrint(S.Tris);

          DebugPrint("Max");
          DebugPrint(S.Max);

          DebugPrint("Count");
          DebugPrint(S.Count);

          DebugPrint("CurrentPointIndex");
          DebugPrint(S.CurrentPointIndex);

          DebugPrint("SurfacePoints");
          DebugPrint(S.SurfacePoints);

      }

      // Source File : src/engine/headers/canonical_position.h
      function void DebugPrint(canonical_position S)
      {
        
          DebugPrint("Offset");
          DebugPrint(S.Offset);

          DebugPrint("WorldP");
          DebugPrint(S.WorldP);

      }

      // Source File : src/engine/headers/mesh.h
      function void DebugPrint(loaded_collada_mesh S)
      {
        
          DebugPrint("Mesh");
          DebugPrint(S.Mesh);

          DebugPrint("Dim");
          DebugPrint(S.Dim);

      }

      // Source File : src/engine/headers/mesh.h
      function void DebugPrint(free_mesh S)
      {
        
          DebugPrint("Mesh");
          DebugPrint(S.Mesh);

          DebugPrint("Next");
          DebugPrint(S.Next);

      }

      // Source File : src/engine/headers/mesh.h
      function void DebugPrint(mesh_freelist S)
      {
        
          DebugPrint("FirstFree");
          DebugPrint(S.FirstFree);

          DebugPrint("Containers");
          DebugPrint(S.Containers);

      }

      // Source File : src/engine/headers/xml.h
      function void DebugPrint(xml_property S)
      {
        
          DebugPrint("Name");
          DebugPrint(S.Name);

          DebugPrint("Value");
          DebugPrint(S.Value);

          DebugPrint("Next");
          DebugPrint(S.Next);

      }

      // Source File : src/engine/headers/xml.h
      function void DebugPrint(xml_token S)
      {
        
          DebugPrint("Type");
          DebugPrint(S.Type);

          DebugPrint("Property");
          DebugPrint(S.Property);

      }

      // Source File : src/engine/headers/xml.h
      function void DebugPrint(xml_tag S)
      {
        
          DebugPrint("Open");
          DebugPrint(S.Open);

          DebugPrint("Parent");
          DebugPrint(S.Parent);

          DebugPrint("Sibling");
          DebugPrint(S.Sibling);

          DebugPrint("NextInHash");
          DebugPrint(S.NextInHash);

          DebugPrint("HashValue");
          DebugPrint(S.HashValue);

          DebugPrint("Value");
          DebugPrint(S.Value);

          DebugPrint("Properties");
          DebugPrint(S.Properties);

          DebugPrint("NextPropertySlot");
          DebugPrint(S.NextPropertySlot);

      }

      // Source File : src/engine/headers/xml.h
      function void DebugPrint(xml_tag_stream S)
      {
        
          DebugPrint("Start");
          DebugPrint(S.Start);

          DebugPrint("At");
          DebugPrint(S.At);

          DebugPrint("End");
          DebugPrint(S.End);

      }

      // Source File : src/engine/headers/xml.h
      function void DebugPrint(xml_parsing_at_indicators S)
      {
        
          DebugPrint("LastClosedTag");
          DebugPrint(S.LastClosedTag);

          DebugPrint("CurrentlyOpenTag");
          DebugPrint(S.CurrentlyOpenTag);

      }

      // Source File : src/engine/headers/graphics.h
      function void DebugPrint(post_processing_group S)
      {
        
          DebugPrint("FBO");
          DebugPrint(S.FBO);

          DebugPrint("Shader");
          DebugPrint(S.Shader);

          DebugPrint("VertexBuffer");
          DebugPrint(S.VertexBuffer);

          DebugPrint("ColorBuffer");
          DebugPrint(S.ColorBuffer);

      }

      // Source File : src/engine/headers/graphics.h
      function void DebugPrint(graphics S)
      {
        
          DebugPrint("Camera");
          DebugPrint(S.Camera);

          DebugPrint("Lights");
          DebugPrint(S.Lights);

          DebugPrint("gBuffer");
          DebugPrint(S.gBuffer);

          DebugPrint("AoGroup");
          DebugPrint(S.AoGroup);

          DebugPrint("SG");
          DebugPrint(S.SG);

          DebugPrint("PostGroup");
          DebugPrint(S.PostGroup);

          DebugPrint("GpuBuffers");
          DebugPrint(S.GpuBuffers);

          DebugPrint("GpuBufferWriteIndex");
          DebugPrint(S.GpuBufferWriteIndex);

          DebugPrint("Memory");
          DebugPrint(S.Memory);

      }

      // Source File : src/engine/headers/render.h
      function void DebugPrint(shader_uniform S)
      {
        
          DebugPrint("Type");
          DebugPrint(S.Type);

      }

      // Source File : src/engine/headers/render.h
      function void DebugPrint(shader S)
      {
        
          DebugPrint("ID");
          DebugPrint(S.ID);

          DebugPrint("FirstUniform");
          DebugPrint(S.FirstUniform);

      }

      // Source File : src/engine/headers/render.h
      function void DebugPrint(texture S)
      {
        
          DebugPrint("ID");
          DebugPrint(S.ID);

          DebugPrint("Dim");
          DebugPrint(S.Dim);

      }

      // Source File : src/engine/headers/render.h
      function void DebugPrint(light S)
      {
        
          DebugPrint("Position");
          DebugPrint(S.Position);

          DebugPrint("Color");
          DebugPrint(S.Color);

      }

      // Source File : src/engine/headers/render.h
      function void DebugPrint(game_lights S)
      {
        
          DebugPrint("ColorTex");
          DebugPrint(S.ColorTex);

          DebugPrint("PositionTex");
          DebugPrint(S.PositionTex);

          DebugPrint("IndexToUV");
          DebugPrint(S.IndexToUV);

          DebugPrint("Count");
          DebugPrint(S.Count);

          DebugPrint("Lights");
          DebugPrint(S.Lights);

      }

      // Source File : src/engine/headers/render.h
      function void DebugPrint(RenderBasis S)
      {
        
          DebugPrint("ModelMatrix");
          DebugPrint(S.ModelMatrix);

          DebugPrint("ViewMatrix");
          DebugPrint(S.ViewMatrix);

          DebugPrint("ProjectionMatrix");
          DebugPrint(S.ProjectionMatrix);

      }

      // Source File : src/engine/headers/render.h
      function void DebugPrint(framebuffer S)
      {
        
          DebugPrint("ID");
          DebugPrint(S.ID);

          DebugPrint("Attachments");
          DebugPrint(S.Attachments);

      }

      // Source File : src/engine/headers/render.h
      function void DebugPrint(ao_render_group S)
      {
        
          DebugPrint("FBO");
          DebugPrint(S.FBO);

          DebugPrint("Shader");
          DebugPrint(S.Shader);

          DebugPrint("DebugSsaoShader");
          DebugPrint(S.DebugSsaoShader);

          DebugPrint("SsaoKernel");
          DebugPrint(S.SsaoKernel);

          DebugPrint("SsaoKernelUniform");
          DebugPrint(S.SsaoKernelUniform);

          DebugPrint("Texture");
          DebugPrint(S.Texture);

          DebugPrint("NoiseTile");
          DebugPrint(S.NoiseTile);

      }

      // Source File : src/engine/headers/render.h
      function void DebugPrint(g_buffer_textures S)
      {
        
          DebugPrint("Color");
          DebugPrint(S.Color);

          DebugPrint("Normal");
          DebugPrint(S.Normal);

          DebugPrint("Position");
          DebugPrint(S.Position);

      }

      // Source File : src/engine/headers/render.h
      function void DebugPrint(g_buffer_render_group S)
      {
        
          DebugPrint("FBO");
          DebugPrint(S.FBO);

          DebugPrint("Textures");
          DebugPrint(S.Textures);

          DebugPrint("LightingShader");
          DebugPrint(S.LightingShader);

          DebugPrint("gBufferShader");
          DebugPrint(S.gBufferShader);

          DebugPrint("ShadowMVP");
          DebugPrint(S.ShadowMVP);

          DebugPrint("ViewProjection");
          DebugPrint(S.ViewProjection);

      }

      // Source File : src/engine/headers/render.h
      function void DebugPrint(shadow_render_group S)
      {
        
          DebugPrint("FramebufferName");
          DebugPrint(S.FramebufferName);

          DebugPrint("MVP_ID");
          DebugPrint(S.MVP_ID);

          DebugPrint("DebugTextureShader");
          DebugPrint(S.DebugTextureShader);

          DebugPrint("DepthShader");
          DebugPrint(S.DepthShader);

          DebugPrint("ShadowMap");
          DebugPrint(S.ShadowMap);

      }

      // Source File : src/engine/headers/render.h
      function void DebugPrint(untextured_3d_geometry_buffer S)
      {
        
          DebugPrint("Verts");
          DebugPrint(S.Verts);

          DebugPrint("Colors");
          DebugPrint(S.Colors);

          DebugPrint("Normals");
          DebugPrint(S.Normals);

          DebugPrint("End");
          DebugPrint(S.End);

          DebugPrint("At");
          DebugPrint(S.At);

      }

      // Source File : src/engine/headers/render.h
      function void DebugPrint(gpu_mapped_element_buffer S)
      {
        
          DebugPrint("VertexHandle");
          DebugPrint(S.VertexHandle);

          DebugPrint("NormalHandle");
          DebugPrint(S.NormalHandle);

          DebugPrint("ColorHandle");
          DebugPrint(S.ColorHandle);

          DebugPrint("Buffer");
          DebugPrint(S.Buffer);

      }

      // Source File : src/engine/headers/render.h
      function void DebugPrint(textured_2d_geometry_buffer S)
      {
        
          DebugPrint("Verts");
          DebugPrint(S.Verts);

          DebugPrint("Colors");
          DebugPrint(S.Colors);

          DebugPrint("UVs");
          DebugPrint(S.UVs);

          DebugPrint("End");
          DebugPrint(S.End);

          DebugPrint("At");
          DebugPrint(S.At);

      }

      // Source File : src/engine/headers/render.h
      function void DebugPrint(untextured_2d_geometry_buffer S)
      {
        
          DebugPrint("Verts");
          DebugPrint(S.Verts);

          DebugPrint("Colors");
          DebugPrint(S.Colors);

          DebugPrint("End");
          DebugPrint(S.End);

          DebugPrint("At");
          DebugPrint(S.At);

      }

      // Source File : src/engine/headers/animation.h
      function void DebugPrint(keyframe S)
      {
        
          DebugPrint("tEnd");
          DebugPrint(S.tEnd);

          DebugPrint("Value");
          DebugPrint(S.Value);

      }

      // Source File : src/engine/headers/animation.h
      function void DebugPrint(animation S)
      {
        
          DebugPrint("t");
          DebugPrint(S.t);

          DebugPrint("tEnd");
          DebugPrint(S.tEnd);

          DebugPrint("xKeyframeCount");
          DebugPrint(S.xKeyframeCount);

          DebugPrint("xKeyframes");
          DebugPrint(S.xKeyframes);

          DebugPrint("yKeyframeCount");
          DebugPrint(S.yKeyframeCount);

          DebugPrint("yKeyframes");
          DebugPrint(S.yKeyframes);

          DebugPrint("zKeyframeCount");
          DebugPrint(S.zKeyframeCount);

          DebugPrint("zKeyframes");
          DebugPrint(S.zKeyframes);

      }

      // Source File : src/engine/bonsai.h
      function void DebugPrint(game_mode S)
      {
        
          DebugPrint("ActiveMode");
          DebugPrint(S.ActiveMode);

          DebugPrint("TimeRunning");
          DebugPrint(S.TimeRunning);

      }

      // Source File : src/engine/bonsai.h
      function void DebugPrint(voxel S)
      {
        
          DebugPrint("Flags");
          DebugPrint(S.Flags);

          DebugPrint("Color");
          DebugPrint(S.Color);

      }

      // Source File : src/engine/bonsai.h
      function void DebugPrint(boundary_voxel S)
      {
        
          DebugPrint("V");
          DebugPrint(S.V);

          DebugPrint("Offset");
          DebugPrint(S.Offset);

      }

      // Source File : src/engine/bonsai.h
      function void DebugPrint(chunk_data S)
      {
        
          DebugPrint("Flags");
          DebugPrint(S.Flags);

          DebugPrint("Voxels");
          DebugPrint(S.Voxels);

          DebugPrint("Pad");
          DebugPrint(S.Pad);

      }

      // Source File : src/engine/bonsai.h
      function void DebugPrint(model S)
      {
        
          DebugPrint("Mesh");
          DebugPrint(S.Mesh);

          DebugPrint("Dim");
          DebugPrint(S.Dim);

          DebugPrint("Animation");
          DebugPrint(S.Animation);

      }

      // Source File : src/engine/bonsai.h
      function void DebugPrint(physics S)
      {
        
          DebugPrint("Velocity");
          DebugPrint(S.Velocity);

          DebugPrint("Force");
          DebugPrint(S.Force);

          DebugPrint("Delta");
          DebugPrint(S.Delta);

          DebugPrint("Drag");
          DebugPrint(S.Drag);

          DebugPrint("Mass");
          DebugPrint(S.Mass);

          DebugPrint("Speed");
          DebugPrint(S.Speed);

      }

      // Source File : src/engine/bonsai.h
      function void DebugPrint(particle S)
      {
        
          DebugPrint("Offset");
          DebugPrint(S.Offset);

          DebugPrint("Physics");
          DebugPrint(S.Physics);

          DebugPrint("Color");
          DebugPrint(S.Color);

          DebugPrint("RemainingLifespan");
          DebugPrint(S.RemainingLifespan);

      }

      // Source File : src/engine/bonsai.h
      function void DebugPrint(particle_system_init_params S)
      {
        
          DebugPrint("SpawnRegion");
          DebugPrint(S.SpawnRegion);

          DebugPrint("Physics");
          DebugPrint(S.Physics);

          DebugPrint("SystemMovementCoefficient");
          DebugPrint(S.SystemMovementCoefficient);

          DebugPrint("EmissionLifespan");
          DebugPrint(S.EmissionLifespan);

          DebugPrint("EmissionChance");
          DebugPrint(S.EmissionChance);

          DebugPrint("ParticleLifespan");
          DebugPrint(S.ParticleLifespan);

          DebugPrint("StartingDiameter");
          DebugPrint(S.StartingDiameter);

          DebugPrint("Entropy");
          DebugPrint(S.Entropy);

          DebugPrint("Colors");
          DebugPrint(S.Colors);

      }

      // Source File : src/engine/bonsai.h
      function void DebugPrint(particle_system S)
      {
        
          DebugPrint("Entropy");
          DebugPrint(S.Entropy);

          DebugPrint("ActiveParticles");
          DebugPrint(S.ActiveParticles);

          DebugPrint("ParticleDuration");
          DebugPrint(S.ParticleDuration);

          DebugPrint("SpawnRegion");
          DebugPrint(S.SpawnRegion);

          DebugPrint("StartingDiameter");
          DebugPrint(S.StartingDiameter);

          DebugPrint("EmissionLifespan");
          DebugPrint(S.EmissionLifespan);

          DebugPrint("ParticleLifespan");
          DebugPrint(S.ParticleLifespan);

          DebugPrint("EmissionChance");
          DebugPrint(S.EmissionChance);

          DebugPrint("ParticlePhysics");
          DebugPrint(S.ParticlePhysics);

          DebugPrint("SystemMovementCoefficient");
          DebugPrint(S.SystemMovementCoefficient);

          DebugPrint("Colors");
          DebugPrint(S.Colors);

      }

      // Source File : src/engine/api.h
      function void DebugPrint(thread S)
      {
        
          DebugPrint("ThreadIndex");
          DebugPrint(S.ThreadIndex);

          DebugPrint("ID");
          DebugPrint(S.ID);

      }

      // Source File : src/engine/api.h
      function void DebugPrint(thread_local_state S)
      {
        
          DebugPrint("PermMemory");
          DebugPrint(S.PermMemory);

          DebugPrint("TempMemory");
          DebugPrint(S.TempMemory);

          DebugPrint("MeshFreelist");
          DebugPrint(S.MeshFreelist);

          DebugPrint("Noise");
          DebugPrint(S.Noise);

      }

      // Source File : src/bonsai_stdlib/headers/stream.h
      function void DebugPrint(u32_stream S)
      {
        
          DebugPrint("Start");
          DebugPrint(S.Start);

          DebugPrint("At");
          DebugPrint(S.At);

          DebugPrint("End");
          DebugPrint(S.End);

      }

      // Source File : src/bonsai_stdlib/headers/stream.h
      function void DebugPrint(u8_stream S)
      {
        
          DebugPrint("Start");
          DebugPrint(S.Start);

          DebugPrint("At");
          DebugPrint(S.At);

          DebugPrint("End");
          DebugPrint(S.End);

      }

      // Source File : src/bonsai_stdlib/headers/stream.h
      function void DebugPrint(ansi_stream S)
      {
        
          DebugPrint("Start");
          DebugPrint(S.Start);

          DebugPrint("At");
          DebugPrint(S.At);

          DebugPrint("End");
          DebugPrint(S.End);

          DebugPrint("Filename");
          DebugPrint(S.Filename);

      }

      // Source File : src/bonsai_stdlib/headers/stream.h
      function void DebugPrint(r32_stream S)
      {
        
          DebugPrint("Start");
          DebugPrint(S.Start);

          DebugPrint("At");
          DebugPrint(S.At);

          DebugPrint("End");
          DebugPrint(S.End);

      }

      // Source File : src/bonsai_stdlib/headers/stream.h
      function void DebugPrint(mesh_metadata S)
      {
        
          DebugPrint("VertCount");
          DebugPrint(S.VertCount);

          DebugPrint("NormalCount");
          DebugPrint(S.NormalCount);

          DebugPrint("UVCount");
          DebugPrint(S.UVCount);

          DebugPrint("FaceCount");
          DebugPrint(S.FaceCount);

      }

      // Source File : src/bonsai_stdlib/headers/rect.h
      function void DebugPrint(rect2 S)
      {
        
          DebugPrint("Min");
          DebugPrint(S.Min);

          DebugPrint("Max");
          DebugPrint(S.Max);

      }

      // Source File : src/bonsai_stdlib/headers/rect.h
      function void DebugPrint(aabb S)
      {
        
          DebugPrint("Center");
          DebugPrint(S.Center);

          DebugPrint("Radius");
          DebugPrint(S.Radius);

      }

      // Source File : src/bonsai_stdlib/headers/rect.h
      function void DebugPrint(ray S)
      {
        
          DebugPrint("Origin");
          DebugPrint(S.Origin);

          DebugPrint("Dir");
          DebugPrint(S.Dir);

      }

      // Source File : src/bonsai_stdlib/headers/input.h
      function void DebugPrint(input_event S)
      {
        
          DebugPrint("Clicked");
          DebugPrint(S.Clicked);

          DebugPrint("Pressed");
          DebugPrint(S.Pressed);

      }

      // Source File : src/bonsai_stdlib/headers/input.h
      function void DebugPrint(input S)
      {
        
          DebugPrint("Escape");
          DebugPrint(S.Escape);

          DebugPrint("Space");
          DebugPrint(S.Space);

          DebugPrint("Shift");
          DebugPrint(S.Shift);

          DebugPrint("Ctrl");
          DebugPrint(S.Ctrl);

          DebugPrint("Alt");
          DebugPrint(S.Alt);

          DebugPrint("F12");
          DebugPrint(S.F12);

          DebugPrint("F11");
          DebugPrint(S.F11);

          DebugPrint("F10");
          DebugPrint(S.F10);

          DebugPrint("F9");
          DebugPrint(S.F9);

          DebugPrint("F8");
          DebugPrint(S.F8);

          DebugPrint("F7");
          DebugPrint(S.F7);

          DebugPrint("F6");
          DebugPrint(S.F6);

          DebugPrint("F5");
          DebugPrint(S.F5);

          DebugPrint("F4");
          DebugPrint(S.F4);

          DebugPrint("F3");
          DebugPrint(S.F3);

          DebugPrint("F2");
          DebugPrint(S.F2);

          DebugPrint("F1");
          DebugPrint(S.F1);

          DebugPrint("RMB");
          DebugPrint(S.RMB);

          DebugPrint("LMB");
          DebugPrint(S.LMB);

          DebugPrint("MMB");
          DebugPrint(S.MMB);

          DebugPrint("W");
          DebugPrint(S.W);

          DebugPrint("A");
          DebugPrint(S.A);

          DebugPrint("S");
          DebugPrint(S.S);

          DebugPrint("D");
          DebugPrint(S.D);

          DebugPrint("Q");
          DebugPrint(S.Q);

          DebugPrint("E");
          DebugPrint(S.E);

      }

      // Source File : src/bonsai_stdlib/headers/work_queue.h
      function void DebugPrint(work_queue S)
      {
        
          DebugPrint("EnqueueIndex");
          DebugPrint(S.EnqueueIndex);

          DebugPrint("DequeueIndex");
          DebugPrint(S.DequeueIndex);

          DebugPrint("Entries");
          DebugPrint(S.Entries);

          DebugPrint("GlobalQueueSemaphore");
          DebugPrint(S.GlobalQueueSemaphore);

      }

      // Source File : src/bonsai_stdlib/headers/string_builder.h
      function void DebugPrint(string_builder S)
      {
        
          DebugPrint("Memory");
          DebugPrint(S.Memory);

          DebugPrint("Chunks");
          DebugPrint(S.Chunks);

      }

      // Source File : src/bonsai_stdlib/headers/matrix.h
      function void DebugPrint(m4 S)
      {
        
          DebugPrint("E");
          DebugPrint(S.E);

      }

      // Source File : src/bonsai_stdlib/headers/matrix.h
      function void DebugPrint(m_nxn S)
      {
        
          DebugPrint("Elements");
          DebugPrint(S.Elements);

          DebugPrint("Rows");
          DebugPrint(S.Rows);

          DebugPrint("Columns");
          DebugPrint(S.Columns);

          DebugPrint("At");
          DebugPrint(S.At);

          DebugPrint("End");
          DebugPrint(S.End);

      }

      // Source File : src/bonsai_stdlib/headers/random.h
      function void DebugPrint(random_series S)
      {
        
          DebugPrint("Seed");
          DebugPrint(S.Seed);

      }

      // Source File : src/bonsai_stdlib/headers/memory_arena.h
      function void DebugPrint(memory_arena S)
      {
        
          DebugPrint("Start");
          DebugPrint(S.Start);

          DebugPrint("At");
          DebugPrint(S.At);

          DebugPrint("End");
          DebugPrint(S.End);

          DebugPrint("NextBlockSize");
          DebugPrint(S.NextBlockSize);

          DebugPrint("Prev");
          DebugPrint(S.Prev);

      }

      // Source File : src/bonsai_stdlib/headers/vector.h
      function void DebugPrint(v2i S)
      {
        
          DebugPrint("x");
          DebugPrint(S.x);

          DebugPrint("y");
          DebugPrint(S.y);

      }

      // Source File : src/bonsai_stdlib/headers/vector.h
      function void DebugPrint(v2 S)
      {
        
          DebugPrint("x");
          DebugPrint(S.x);

          DebugPrint("y");
          DebugPrint(S.y);

      }

      // Source File : src/bonsai_stdlib/headers/heap_allocator.h
      function void DebugPrint(heap_allocation_block S)
      {
        
          DebugPrint("Type");
          DebugPrint(S.Type);

          DebugPrint("Size");
          DebugPrint(S.Size);

          DebugPrint("PrevAllocationSize");
          DebugPrint(S.PrevAllocationSize);

      }

      // Source File : src/bonsai_stdlib/headers/heap_allocator.h
      function void DebugPrint(heap_allocator S)
      {
        
          DebugPrint("Heap");
          DebugPrint(S.Heap);

          DebugPrint("Size");
          DebugPrint(S.Size);

      }

      // Source File : src/bonsai_stdlib/headers/counted_string.h
      function void DebugPrint(char_cursor S)
      {
        
          DebugPrint("Start");
          DebugPrint(S.Start);

          DebugPrint("End");
          DebugPrint(S.End);

          DebugPrint("At");
          DebugPrint(S.At);

          DebugPrint("Memory");
          DebugPrint(S.Memory);

      }

      // Source File : src/bonsai_stdlib/headers/line.h
      function void DebugPrint(linei S)
      {
        
          DebugPrint("MinP");
          DebugPrint(S.MinP);

          DebugPrint("MaxP");
          DebugPrint(S.MaxP);

      }

      // Source File : src/bonsai_stdlib/headers/line.h
      function void DebugPrint(line S)
      {
        
          DebugPrint("MinP");
          DebugPrint(S.MinP);

          DebugPrint("MaxP");
          DebugPrint(S.MaxP);

      }

      // Source File : src/unix_platform.h
      function void DebugPrint(XVisualInfo S)
      {
        
          DebugPrint("visual");
          DebugPrint(S.visual);

          DebugPrint("visualid");
          DebugPrint(S.visualid);

          DebugPrint("screen");
          DebugPrint(S.screen);

          DebugPrint("depth");
          DebugPrint(S.depth);

          DebugPrint("c_class");
          DebugPrint(S.c_class);

          DebugPrint("red_mask");
          DebugPrint(S.red_mask);

          DebugPrint("green_mask");
          DebugPrint(S.green_mask);

          DebugPrint("blue_mask");
          DebugPrint(S.blue_mask);

          DebugPrint("colormap_size");
          DebugPrint(S.colormap_size);

          DebugPrint("bits_per_rgb");
          DebugPrint(S.bits_per_rgb);

      }

      // Source File : src/unix_platform.h
      function void DebugPrint(os S)
      {
        
          DebugPrint("Window");
          DebugPrint(S.Window);

          DebugPrint("Display");
          DebugPrint(S.Display);

          DebugPrint("GlContext");
          DebugPrint(S.GlContext);

          DebugPrint("ContinueRunning");
          DebugPrint(S.ContinueRunning);

      }

      // Source File : src/unix_platform.h
      function void DebugPrint(native_file S)
      {
        
          DebugPrint("Handle");
          DebugPrint(S.Handle);

          DebugPrint("Path");
          DebugPrint(S.Path);

      }

      // Source File : src/metaprogramming/preprocessor.cpp
      function void DebugPrint(for_enum_constraints S)
      {
        
          DebugPrint("TypeName");
          DebugPrint(S.TypeName);

          DebugPrint("ValueName");
          DebugPrint(S.ValueName);

      }

      // Source File : src/metaprogramming/preprocessor.cpp
      function void DebugPrint(body_text_constraints S)
      {
        
          DebugPrint("MemberContains");
          DebugPrint(S.MemberContains);

          DebugPrint("TypeTag");
          DebugPrint(S.TypeTag);

          DebugPrint("TypeName");
          DebugPrint(S.TypeName);

          DebugPrint("ValueName");
          DebugPrint(S.ValueName);

      }

      // Source File : src/metaprogramming/preprocessor.cpp
      function void DebugPrint(replacement_pattern S)
      {
        
          DebugPrint("Match");
          DebugPrint(S.Match);

          DebugPrint("Replace");
          DebugPrint(S.Replace);

      }

      // Source File : src/engine/cpp/world_chunk.cpp
      function void DebugPrint(plane_computation S)
      {
        
          DebugPrint("Plane");
          DebugPrint(S.Plane);

          DebugPrint("Complete");
          DebugPrint(S.Complete);

      }

      // Source File : src/bonsai_stdlib/cpp/bitmap.cpp
      function void DebugPrint(bitmap_image_header S)
      {
        
          DebugPrint("SizeOfImageHeader");
          DebugPrint(S.SizeOfImageHeader);

          DebugPrint("WidthInPixels");
          DebugPrint(S.WidthInPixels);

          DebugPrint("HeightInPixels");
          DebugPrint(S.HeightInPixels);

          DebugPrint("ColorPlanes");
          DebugPrint(S.ColorPlanes);

          DebugPrint("BPP");
          DebugPrint(S.BPP);

          DebugPrint("CompressionType");
          DebugPrint(S.CompressionType);

          DebugPrint("SizeInBytes");
          DebugPrint(S.SizeInBytes);

          DebugPrint("xPixelsPerMeter");
          DebugPrint(S.xPixelsPerMeter);

          DebugPrint("yPixelsPerMeter");
          DebugPrint(S.yPixelsPerMeter);

          DebugPrint("ColorMapsUsed");
          DebugPrint(S.ColorMapsUsed);

          DebugPrint("NumSignificantColors");
          DebugPrint(S.NumSignificantColors);

          DebugPrint("RedMask");
          DebugPrint(S.RedMask);

          DebugPrint("GreenMask");
          DebugPrint(S.GreenMask);

          DebugPrint("BlueMask");
          DebugPrint(S.BlueMask);

          DebugPrint("AlphaMask");
          DebugPrint(S.AlphaMask);

      }

      // Source File : src/bonsai_stdlib/cpp/bitmap.cpp
      function void DebugPrint(bitmap_header S)
      {
        
          DebugPrint("Type");
          DebugPrint(S.Type);

          DebugPrint("FileSizeInBytes");
          DebugPrint(S.FileSizeInBytes);

          DebugPrint("Ignored");
          DebugPrint(S.Ignored);

          DebugPrint("OffsetToPixelData");
          DebugPrint(S.OffsetToPixelData);

          DebugPrint("Image");
          DebugPrint(S.Image);

      }

      // Source File : src/bonsai_stdlib/cpp/bitmap.cpp
      function void DebugPrint(bitmap S)
      {
        
          DebugPrint("Dim");
          DebugPrint(S.Dim);

          DebugPrint("Pixels");
          DebugPrint(S.Pixels);

      }

