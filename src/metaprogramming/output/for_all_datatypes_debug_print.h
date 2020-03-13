
      // Source File : src/debug_system/headers/debug.h
      function void DebugPrint(cycle_range* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  StartCycle = ");
            DebugPrint(S->StartCycle);
            DebugPrint("\n");

            DebugPrint("  TotalCycles = ");
            DebugPrint(S->TotalCycles);
            DebugPrint("\n");

        }
      }

      // Source File : src/debug_system/headers/debug.h
      function void DebugPrint(memory_arena_stats* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Allocations = ");
            DebugPrint(S->Allocations);
            DebugPrint("\n");

            DebugPrint("  Pushes = ");
            DebugPrint(S->Pushes);
            DebugPrint("\n");

            DebugPrint("  TotalAllocated = ");
            DebugPrint(S->TotalAllocated);
            DebugPrint("\n");

            DebugPrint("  Remaining = ");
            DebugPrint(S->Remaining);
            DebugPrint("\n");

        }
      }

      // Source File : src/debug_system/headers/debug.h
      function void DebugPrint(min_max_avg_dt* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Min = ");
            DebugPrint(S->Min);
            DebugPrint("\n");

            DebugPrint("  Max = ");
            DebugPrint(S->Max);
            DebugPrint("\n");

            DebugPrint("  Avg = ");
            DebugPrint(S->Avg);
            DebugPrint("\n");

        }
      }

      // Source File : src/debug_system/headers/debug.h
      function void DebugPrint(debug_ui_render_group* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  GameGeo = ");
            DebugPrint(S->GameGeo);
            DebugPrint("\n");

            DebugPrint("  GameGeoShader = ");
            DebugPrint(S->GameGeoShader);
            DebugPrint("\n");

            DebugPrint("  TextGroup = ");
            DebugPrint(S->TextGroup);
            DebugPrint("\n");

            DebugPrint("  InteractionStackTop = ");
            DebugPrint(S->InteractionStackTop);
            DebugPrint("\n");

            DebugPrint("  MouseP = ");
            DebugPrint(S->MouseP);
            DebugPrint("\n");

            DebugPrint("  MouseDP = ");
            DebugPrint(S->MouseDP);
            DebugPrint("\n");

            DebugPrint("  ScreenDim = ");
            DebugPrint(S->ScreenDim);
            DebugPrint("\n");

            DebugPrint("  Input = ");
            DebugPrint(S->Input);
            DebugPrint("\n");

            DebugPrint("  HighestWindow = ");
            DebugPrint(S->HighestWindow);
            DebugPrint("\n");

            DebugPrint("  HoverInteractionId = ");
            DebugPrint(S->HoverInteractionId);
            DebugPrint("\n");

            DebugPrint("  ClickedInteractionId = ");
            DebugPrint(S->ClickedInteractionId);
            DebugPrint("\n");

            DebugPrint("  PressedInteractionId = ");
            DebugPrint(S->PressedInteractionId);
            DebugPrint("\n");

            DebugPrint("  Geo = ");
            DebugPrint(S->Geo);
            DebugPrint("\n");

            DebugPrint("  CommandBuffer = ");
            DebugPrint(S->CommandBuffer);
            DebugPrint("\n");

        }
      }

      // Source File : src/debug_system/headers/debug.h
      function void DebugPrint(debug_profile_scope* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  CycleCount = ");
            DebugPrint(S->CycleCount);
            DebugPrint("\n");

            DebugPrint("  StartingCycle = ");
            DebugPrint(S->StartingCycle);
            DebugPrint("\n");

            DebugPrint("  Name = ");
            DebugPrint(S->Name);
            DebugPrint("\n");

            DebugPrint("  Expanded = ");
            DebugPrint(S->Expanded);
            DebugPrint("\n");

            DebugPrint("  Sibling = ");
            DebugPrint(S->Sibling);
            DebugPrint("\n");

            DebugPrint("  Child = ");
            DebugPrint(S->Child);
            DebugPrint("\n");

            DebugPrint("  Parent = ");
            DebugPrint(S->Parent);
            DebugPrint("\n");

        }
      }

      // Source File : src/debug_system/headers/debug.h
      function void DebugPrint(unique_debug_profile_scope* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Name = ");
            DebugPrint(S->Name);
            DebugPrint("\n");

            DebugPrint("  CallCount = ");
            DebugPrint(S->CallCount);
            DebugPrint("\n");

            DebugPrint("  TotalCycles = ");
            DebugPrint(S->TotalCycles);
            DebugPrint("\n");

            DebugPrint("  MinCycles = ");
            DebugPrint(S->MinCycles);
            DebugPrint("\n");

            DebugPrint("  MaxCycles = ");
            DebugPrint(S->MaxCycles);
            DebugPrint("\n");

            DebugPrint("  Scope = ");
            DebugPrint(S->Scope);
            DebugPrint("\n");

            DebugPrint("  NextUnique = ");
            DebugPrint(S->NextUnique);
            DebugPrint("\n");

        }
      }

      // Source File : src/debug_system/headers/debug.h
      function void DebugPrint(debug_scope_tree* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Root = ");
            DebugPrint(S->Root);
            DebugPrint("\n");

            DebugPrint("  WriteScope = ");
            DebugPrint(S->WriteScope);
            DebugPrint("\n");

            DebugPrint("  ParentOfNextScope = ");
            DebugPrint(S->ParentOfNextScope);
            DebugPrint("\n");

            DebugPrint("  FrameRecorded = ");
            DebugPrint(S->FrameRecorded);
            DebugPrint("\n");

        }
      }

      // Source File : src/debug_system/headers/debug.h
      function void DebugPrint(debug_thread_state* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Memory = ");
            DebugPrint(S->Memory);
            DebugPrint("\n");

            DebugPrint("  MetaTable = ");
            DebugPrint(S->MetaTable);
            DebugPrint("\n");

            DebugPrint("  ScopeTrees = ");
            DebugPrint(S->ScopeTrees);
            DebugPrint("\n");

            DebugPrint("  FirstFreeScope = ");
            DebugPrint(S->FirstFreeScope);
            DebugPrint("\n");

            DebugPrint("  MutexOps = ");
            DebugPrint(S->MutexOps);
            DebugPrint("\n");

            DebugPrint("  WriteIndex = ");
            DebugPrint(S->WriteIndex);
            DebugPrint("\n");

            DebugPrint("  Pad = ");
            DebugPrint(S->Pad);
            DebugPrint("\n");

        }
      }

      // Source File : src/debug_system/headers/debug.h
      function void DebugPrint(registered_memory_arena* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Arena = ");
            DebugPrint(S->Arena);
            DebugPrint("\n");

            DebugPrint("  Name = ");
            DebugPrint(S->Name);
            DebugPrint("\n");

            DebugPrint("  Expanded = ");
            DebugPrint(S->Expanded);
            DebugPrint("\n");

        }
      }

      // Source File : src/debug_system/headers/debug.h
      function void DebugPrint(selected_memory_arena* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  ArenaHash = ");
            DebugPrint(S->ArenaHash);
            DebugPrint("\n");

            DebugPrint("  HeadArenaHash = ");
            DebugPrint(S->HeadArenaHash);
            DebugPrint("\n");

        }
      }

      // Source File : src/debug_system/headers/debug.h
      function void DebugPrint(selected_arenas* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Count = ");
            DebugPrint(S->Count);
            DebugPrint("\n");

            DebugPrint("  Arenas = ");
            DebugPrint(S->Arenas);
            DebugPrint("\n");

        }
      }

      // Source File : src/debug_system/headers/debug.h
      function void DebugPrint(frame_stats* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  TotalCycles = ");
            DebugPrint(S->TotalCycles);
            DebugPrint("\n");

            DebugPrint("  StartingCycle = ");
            DebugPrint(S->StartingCycle);
            DebugPrint("\n");

            DebugPrint("  FrameMs = ");
            DebugPrint(S->FrameMs);
            DebugPrint("\n");

        }
      }

      // Source File : src/debug_system/headers/debug.h
      function void DebugPrint(called_function* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Name = ");
            DebugPrint(S->Name);
            DebugPrint("\n");

            DebugPrint("  CallCount = ");
            DebugPrint(S->CallCount);
            DebugPrint("\n");

        }
      }

      // Source File : src/debug_system/headers/debug.h
      function void DebugPrint(debug_draw_call* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Caller = ");
            DebugPrint(S->Caller);
            DebugPrint("\n");

            DebugPrint("  N = ");
            DebugPrint(S->N);
            DebugPrint("\n");

            DebugPrint("  Calls = ");
            DebugPrint(S->Calls);
            DebugPrint("\n");

        }
      }

      // Source File : src/debug_system/headers/debug.h
      function void DebugPrint(debug_timed_function* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Scope = ");
            DebugPrint(S->Scope);
            DebugPrint("\n");

            DebugPrint("  Tree = ");
            DebugPrint(S->Tree);
            DebugPrint("\n");

        }
      }

      // Source File : src/debug_system/headers/debug_ui.h
      function void DebugPrint(window_layout* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Title = ");
            DebugPrint(S->Title);
            DebugPrint("\n");

            DebugPrint("  Basis = ");
            DebugPrint(S->Basis);
            DebugPrint("\n");

            DebugPrint("  MaxClip = ");
            DebugPrint(S->MaxClip);
            DebugPrint("\n");

            DebugPrint("  InteractionStackIndex = ");
            DebugPrint(S->InteractionStackIndex);
            DebugPrint("\n");

            DebugPrint("  zBackground = ");
            DebugPrint(S->zBackground);
            DebugPrint("\n");

            DebugPrint("  zText = ");
            DebugPrint(S->zText);
            DebugPrint("\n");

            DebugPrint("  zBorder = ");
            DebugPrint(S->zBorder);
            DebugPrint("\n");

            DebugPrint("  zTitleBar = ");
            DebugPrint(S->zTitleBar);
            DebugPrint("\n");

            DebugPrint("  NextHotWindow = ");
            DebugPrint(S->NextHotWindow);
            DebugPrint("\n");

        }
      }

      // Source File : src/debug_system/headers/debug_ui.h
      function void DebugPrint(font* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Size = ");
            DebugPrint(S->Size);
            DebugPrint("\n");

        }
      }

      // Source File : src/debug_system/headers/debug_ui.h
      function void DebugPrint(ui_element_reference* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Index = ");
            DebugPrint(S->Index);
            DebugPrint("\n");

        }
      }

      // Source File : src/debug_system/headers/debug_ui.h
      function void DebugPrint(ui_style* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Color = ");
            DebugPrint(S->Color);
            DebugPrint("\n");

            DebugPrint("  HoverColor = ");
            DebugPrint(S->HoverColor);
            DebugPrint("\n");

            DebugPrint("  PressedColor = ");
            DebugPrint(S->PressedColor);
            DebugPrint("\n");

            DebugPrint("  ClickedColor = ");
            DebugPrint(S->ClickedColor);
            DebugPrint("\n");

            DebugPrint("  ActiveColor = ");
            DebugPrint(S->ActiveColor);
            DebugPrint("\n");

            DebugPrint("  Font = ");
            DebugPrint(S->Font);
            DebugPrint("\n");

            DebugPrint("  IsActive = ");
            DebugPrint(S->IsActive);
            DebugPrint("\n");

        }
      }

      // Source File : src/debug_system/headers/debug_ui.h
      function void DebugPrint(layout* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Basis = ");
            DebugPrint(S->Basis);
            DebugPrint("\n");

            DebugPrint("  At = ");
            DebugPrint(S->At);
            DebugPrint("\n");

            DebugPrint("  DrawBounds = ");
            DebugPrint(S->DrawBounds);
            DebugPrint("\n");

            DebugPrint("  Padding = ");
            DebugPrint(S->Padding);
            DebugPrint("\n");

            DebugPrint("  Prev = ");
            DebugPrint(S->Prev);
            DebugPrint("\n");

        }
      }

      // Source File : src/debug_system/headers/debug_ui.h
      function void DebugPrint(ui_render_command_border* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Window = ");
            DebugPrint(S->Window);
            DebugPrint("\n");

            DebugPrint("  Bounds = ");
            DebugPrint(S->Bounds);
            DebugPrint("\n");

            DebugPrint("  Color = ");
            DebugPrint(S->Color);
            DebugPrint("\n");

        }
      }

      // Source File : src/debug_system/headers/debug_ui.h
      function void DebugPrint(ui_render_command_window_start* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Layout = ");
            DebugPrint(S->Layout);
            DebugPrint("\n");

            DebugPrint("  Window = ");
            DebugPrint(S->Window);
            DebugPrint("\n");

        }
      }

      // Source File : src/debug_system/headers/debug_ui.h
      function void DebugPrint(ui_render_command_window_end* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Window = ");
            DebugPrint(S->Window);
            DebugPrint("\n");

        }
      }

      // Source File : src/debug_system/headers/debug_ui.h
      function void DebugPrint(ui_render_command_column_start* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Layout = ");
            DebugPrint(S->Layout);
            DebugPrint("\n");

            DebugPrint("  Style = ");
            DebugPrint(S->Style);
            DebugPrint("\n");

            DebugPrint("  Width = ");
            DebugPrint(S->Width);
            DebugPrint("\n");

            DebugPrint("  MaxWidth = ");
            DebugPrint(S->MaxWidth);
            DebugPrint("\n");

            DebugPrint("  Params = ");
            DebugPrint(S->Params);
            DebugPrint("\n");

        }
      }

      // Source File : src/debug_system/headers/debug_ui.h
      function void DebugPrint(ui_render_command_text* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Layout = ");
            DebugPrint(S->Layout);
            DebugPrint("\n");

            DebugPrint("  Style = ");
            DebugPrint(S->Style);
            DebugPrint("\n");

            DebugPrint("  String = ");
            DebugPrint(S->String);
            DebugPrint("\n");

        }
      }

      // Source File : src/debug_system/headers/debug_ui.h
      function void DebugPrint(ui_render_command_text_at* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Text = ");
            DebugPrint(S->Text);
            DebugPrint("\n");

            DebugPrint("  At = ");
            DebugPrint(S->At);
            DebugPrint("\n");

            DebugPrint("  MaxClip = ");
            DebugPrint(S->MaxClip);
            DebugPrint("\n");

        }
      }

      // Source File : src/debug_system/headers/debug_ui.h
      function void DebugPrint(ui_render_command_untextured_quad* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Layout = ");
            DebugPrint(S->Layout);
            DebugPrint("\n");

            DebugPrint("  Style = ");
            DebugPrint(S->Style);
            DebugPrint("\n");

            DebugPrint("  QuadDim = ");
            DebugPrint(S->QuadDim);
            DebugPrint("\n");

            DebugPrint("  zDepth = ");
            DebugPrint(S->zDepth);
            DebugPrint("\n");

            DebugPrint("  Params = ");
            DebugPrint(S->Params);
            DebugPrint("\n");

        }
      }

      // Source File : src/debug_system/headers/debug_ui.h
      function void DebugPrint(ui_render_command_untextured_quad_at* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Layout = ");
            DebugPrint(S->Layout);
            DebugPrint("\n");

            DebugPrint("  Style = ");
            DebugPrint(S->Style);
            DebugPrint("\n");

            DebugPrint("  QuadDim = ");
            DebugPrint(S->QuadDim);
            DebugPrint("\n");

            DebugPrint("  zDepth = ");
            DebugPrint(S->zDepth);
            DebugPrint("\n");

        }
      }

      // Source File : src/debug_system/headers/debug_ui.h
      function void DebugPrint(ui_render_command_textured_quad* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Layout = ");
            DebugPrint(S->Layout);
            DebugPrint("\n");

            DebugPrint("  TextureSlice = ");
            DebugPrint(S->TextureSlice);
            DebugPrint("\n");

            DebugPrint("  zDepth = ");
            DebugPrint(S->zDepth);
            DebugPrint("\n");

        }
      }

      // Source File : src/debug_system/headers/debug_ui.h
      function void DebugPrint(ui_render_command_button_start* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  ID = ");
            DebugPrint(S->ID);
            DebugPrint("\n");

            DebugPrint("  Style = ");
            DebugPrint(S->Style);
            DebugPrint("\n");

        }
      }

      // Source File : src/debug_system/headers/debug_ui.h
      function void DebugPrint(ui_render_command_button_end* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Params = ");
            DebugPrint(S->Params);
            DebugPrint("\n");

        }
      }

      // Source File : src/debug_system/headers/debug_ui.h
      function void DebugPrint(ui_render_command_table_start* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Layout = ");
            DebugPrint(S->Layout);
            DebugPrint("\n");

            DebugPrint("  Style = ");
            DebugPrint(S->Style);
            DebugPrint("\n");

            DebugPrint("  Position = ");
            DebugPrint(S->Position);
            DebugPrint("\n");

            DebugPrint("  RelativeTo = ");
            DebugPrint(S->RelativeTo);
            DebugPrint("\n");

        }
      }

      // Source File : src/debug_system/headers/debug_ui.h
      function void DebugPrint(ui_render_command_buffer* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  CommandCount = ");
            DebugPrint(S->CommandCount);
            DebugPrint("\n");

            DebugPrint("  Commands = ");
            DebugPrint(S->Commands);
            DebugPrint("\n");

        }
      }

      // Source File : src/debug_system/headers/debug_ui.h
      function void DebugPrint(clip_result* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  ClipStatus = ");
            DebugPrint(S->ClipStatus);
            DebugPrint("\n");

            DebugPrint("  MaxClip = ");
            DebugPrint(S->MaxClip);
            DebugPrint("\n");

            DebugPrint("  PartialClip = ");
            DebugPrint(S->PartialClip);
            DebugPrint("\n");

        }
      }

      // Source File : src/debug_system/headers/debug_ui.h
      function void DebugPrint(sort_key* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Value = ");
            DebugPrint(S->Value);
            DebugPrint("\n");

            DebugPrint("  Index = ");
            DebugPrint(S->Index);
            DebugPrint("\n");

        }
      }

      // Source File : src/debug_system/headers/debug_ui.h
      function void DebugPrint(window_sort_params* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Count = ");
            DebugPrint(S->Count);
            DebugPrint("\n");

            DebugPrint("  LowestInteractionStackIndex = ");
            DebugPrint(S->LowestInteractionStackIndex);
            DebugPrint("\n");

            DebugPrint("  SortKeys = ");
            DebugPrint(S->SortKeys);
            DebugPrint("\n");

        }
      }

      // Source File : src/debug_system/headers/debug_ui.h
      function void DebugPrint(find_button_start_result* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Index = ");
            DebugPrint(S->Index);
            DebugPrint("\n");

            DebugPrint("  Command = ");
            DebugPrint(S->Command);
            DebugPrint("\n");

        }
      }

      // Source File : src/debug_system/headers/debug_ui.h
      function void DebugPrint(find_command_result* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Command = ");
            DebugPrint(S->Command);
            DebugPrint("\n");

            DebugPrint("  Index = ");
            DebugPrint(S->Index);
            DebugPrint("\n");

        }
      }

      // Source File : src/debug_system/headers/interactable.h
      function void DebugPrint(interactable_handle* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Id = ");
            DebugPrint(S->Id);
            DebugPrint("\n");

        }
      }

      // Source File : src/debug_system/headers/interactable.h
      function void DebugPrint(interactable* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  ID = ");
            DebugPrint(S->ID);
            DebugPrint("\n");

            DebugPrint("  MinP = ");
            DebugPrint(S->MinP);
            DebugPrint("\n");

            DebugPrint("  MaxP = ");
            DebugPrint(S->MaxP);
            DebugPrint("\n");

            DebugPrint("  Window = ");
            DebugPrint(S->Window);
            DebugPrint("\n");

        }
      }

      // Source File : src/debug_system/headers/interactable.h
      function void DebugPrint(button_interaction_result* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Pressed = ");
            DebugPrint(S->Pressed);
            DebugPrint("\n");

            DebugPrint("  Clicked = ");
            DebugPrint(S->Clicked);
            DebugPrint("\n");

            DebugPrint("  Hover = ");
            DebugPrint(S->Hover);
            DebugPrint("\n");

        }
      }

      // Source File : src/debug_system/headers/debug_render.h
      function void DebugPrint(render_state* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Window = ");
            DebugPrint(S->Window);
            DebugPrint("\n");

            DebugPrint("  WindowStartCommandIndex = ");
            DebugPrint(S->WindowStartCommandIndex);
            DebugPrint("\n");

            DebugPrint("  Layout = ");
            DebugPrint(S->Layout);
            DebugPrint("\n");

            DebugPrint("  Hover = ");
            DebugPrint(S->Hover);
            DebugPrint("\n");

            DebugPrint("  Pressed = ");
            DebugPrint(S->Pressed);
            DebugPrint("\n");

            DebugPrint("  Clicked = ");
            DebugPrint(S->Clicked);
            DebugPrint("\n");

        }
      }

      // Source File : src/debug_system/headers/debug_render.h
      function void DebugPrint(debug_text_render_group* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  SolidUIVertexBuffer = ");
            DebugPrint(S->SolidUIVertexBuffer);
            DebugPrint("\n");

            DebugPrint("  SolidUIColorBuffer = ");
            DebugPrint(S->SolidUIColorBuffer);
            DebugPrint("\n");

            DebugPrint("  SolidUIUVBuffer = ");
            DebugPrint(S->SolidUIUVBuffer);
            DebugPrint("\n");

            DebugPrint("  FontTexture = ");
            DebugPrint(S->FontTexture);
            DebugPrint("\n");

            DebugPrint("  Text2DShader = ");
            DebugPrint(S->Text2DShader);
            DebugPrint("\n");

            DebugPrint("  TextTextureUniform = ");
            DebugPrint(S->TextTextureUniform);
            DebugPrint("\n");

            DebugPrint("  Geo = ");
            DebugPrint(S->Geo);
            DebugPrint("\n");

            DebugPrint("  DebugFontTextureShader = ");
            DebugPrint(S->DebugFontTextureShader);
            DebugPrint("\n");

            DebugPrint("  SolidUIShader = ");
            DebugPrint(S->SolidUIShader);
            DebugPrint("\n");

        }
      }

      // Source File : src/metaprogramming/preprocessor.h
      function void DebugPrint(c_decl_variable* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Type = ");
            DebugPrint(S->Type);
            DebugPrint("\n");

            DebugPrint("  Name = ");
            DebugPrint(S->Name);
            DebugPrint("\n");

        }
      }

      // Source File : src/metaprogramming/preprocessor.h
      function void DebugPrint(c_decl_stream* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  FirstChunk = ");
            DebugPrint(S->FirstChunk);
            DebugPrint("\n");

            DebugPrint("  LastChunk = ");
            DebugPrint(S->LastChunk);
            DebugPrint("\n");

        }
      }

      // Source File : src/metaprogramming/preprocessor.h
      function void DebugPrint(struct_def* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Name = ");
            DebugPrint(S->Name);
            DebugPrint("\n");

            DebugPrint("  DefinedInFile = ");
            DebugPrint(S->DefinedInFile);
            DebugPrint("\n");

            DebugPrint("  Fields = ");
            DebugPrint(S->Fields);
            DebugPrint("\n");

        }
      }

      // Source File : src/metaprogramming/preprocessor.h
      function void DebugPrint(c_decl_union* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Body = ");
            DebugPrint(S->Body);
            DebugPrint("\n");

        }
      }

      // Source File : src/metaprogramming/preprocessor.h
      function void DebugPrint(enum_field* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Name = ");
            DebugPrint(S->Name);
            DebugPrint("\n");

            DebugPrint("  Value = ");
            DebugPrint(S->Value);
            DebugPrint("\n");

        }
      }

      // Source File : src/metaprogramming/preprocessor.h
      function void DebugPrint(d_union_member* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Type = ");
            DebugPrint(S->Type);
            DebugPrint("\n");

            DebugPrint("  Name = ");
            DebugPrint(S->Name);
            DebugPrint("\n");

            DebugPrint("  Flags = ");
            DebugPrint(S->Flags);
            DebugPrint("\n");

        }
      }

      // Source File : src/metaprogramming/preprocessor.h
      function void DebugPrint(c_decl_stream_chunk* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Element = ");
            DebugPrint(S->Element);
            DebugPrint("\n");

            DebugPrint("  Next = ");
            DebugPrint(S->Next);
            DebugPrint("\n");

        }
      }

      // Source File : src/metaprogramming/preprocessor.h
      function void DebugPrint(enum_def* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Name = ");
            DebugPrint(S->Name);
            DebugPrint("\n");

            DebugPrint("  Fields = ");
            DebugPrint(S->Fields);
            DebugPrint("\n");

        }
      }

      // Source File : src/metaprogramming/preprocessor.h
      function void DebugPrint(c_token* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Type = ");
            DebugPrint(S->Type);
            DebugPrint("\n");

            DebugPrint("  Value = ");
            DebugPrint(S->Value);
            DebugPrint("\n");

        }
      }

      // Source File : src/metaprogramming/preprocessor.h
      function void DebugPrint(d_union_decl* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Name = ");
            DebugPrint(S->Name);
            DebugPrint("\n");

            DebugPrint("  Members = ");
            DebugPrint(S->Members);
            DebugPrint("\n");

            DebugPrint("  CommonMembers = ");
            DebugPrint(S->CommonMembers);
            DebugPrint("\n");

            DebugPrint("  CustomEnumType = ");
            DebugPrint(S->CustomEnumType);
            DebugPrint("\n");

        }
      }

      // Source File : src/metaprogramming/preprocessor.h
      function void DebugPrint(c_parse_result* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Valid = ");
            DebugPrint(S->Valid);
            DebugPrint("\n");

            DebugPrint("  Tokens = ");
            DebugPrint(S->Tokens);
            DebugPrint("\n");

            DebugPrint("  OutputTokens = ");
            DebugPrint(S->OutputTokens);
            DebugPrint("\n");

            DebugPrint("  Filename = ");
            DebugPrint(S->Filename);
            DebugPrint("\n");

            DebugPrint("  LineNumber = ");
            DebugPrint(S->LineNumber);
            DebugPrint("\n");

        }
      }

      // Source File : src/metaprogramming/preprocessor.h
      function void DebugPrint(todo* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Id = ");
            DebugPrint(S->Id);
            DebugPrint("\n");

            DebugPrint("  Value = ");
            DebugPrint(S->Value);
            DebugPrint("\n");

            DebugPrint("  FoundInCodebase = ");
            DebugPrint(S->FoundInCodebase);
            DebugPrint("\n");

        }
      }

      // Source File : src/metaprogramming/preprocessor.h
      function void DebugPrint(tag* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Tag = ");
            DebugPrint(S->Tag);
            DebugPrint("\n");

            DebugPrint("  Todos = ");
            DebugPrint(S->Todos);
            DebugPrint("\n");

        }
      }

      // Source File : src/metaprogramming/preprocessor.h
      function void DebugPrint(person* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Name = ");
            DebugPrint(S->Name);
            DebugPrint("\n");

            DebugPrint("  Tags = ");
            DebugPrint(S->Tags);
            DebugPrint("\n");

        }
      }

      // Source File : src/metaprogramming/preprocessor.h
      function void DebugPrint(arguments* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  OutPath = ");
            DebugPrint(S->OutPath);
            DebugPrint("\n");

            DebugPrint("  Files = ");
            DebugPrint(S->Files);
            DebugPrint("\n");

            DebugPrint("  DoDebugWindow = ");
            DebugPrint(S->DoDebugWindow);
            DebugPrint("\n");

        }
      }

      // Source File : src/metaprogramming/preprocessor.h
      function void DebugPrint(program_datatypes* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Structs = ");
            DebugPrint(S->Structs);
            DebugPrint("\n");

            DebugPrint("  Enums = ");
            DebugPrint(S->Enums);
            DebugPrint("\n");

        }
      }

      // Source File : src/metaprogramming/preprocessor.h
      function void DebugPrint(c_decl_iterator* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  At = ");
            DebugPrint(S->At);
            DebugPrint("\n");

        }
      }

      // Source File : src/metaprogramming/preprocessor.h
      function void DebugPrint(for_enum_constraints* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  TypeName = ");
            DebugPrint(S->TypeName);
            DebugPrint("\n");

            DebugPrint("  ValueName = ");
            DebugPrint(S->ValueName);
            DebugPrint("\n");

        }
      }

      // Source File : src/metaprogramming/preprocessor.h
      function void DebugPrint(body_text_constraints* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  MemberContains = ");
            DebugPrint(S->MemberContains);
            DebugPrint("\n");

            DebugPrint("  TypeTag = ");
            DebugPrint(S->TypeTag);
            DebugPrint("\n");

            DebugPrint("  TypeName = ");
            DebugPrint(S->TypeName);
            DebugPrint("\n");

            DebugPrint("  ValueName = ");
            DebugPrint(S->ValueName);
            DebugPrint("\n");

        }
      }

      // Source File : src/metaprogramming/preprocessor.h
      function void DebugPrint(replacement_pattern* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Match = ");
            DebugPrint(S->Match);
            DebugPrint("\n");

            DebugPrint("  Replace = ");
            DebugPrint(S->Replace);
            DebugPrint("\n");

        }
      }

      // Source File : src/metaprogramming/output/generate_cursor_c_token.h
      function void DebugPrint(c_token_cursor* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Start = ");
            DebugPrint(S->Start);
            DebugPrint("\n");

            DebugPrint("  End = ");
            DebugPrint(S->End);
            DebugPrint("\n");

            DebugPrint("  At = ");
            DebugPrint(S->At);
            DebugPrint("\n");

        }
      }

      // Source File : src/metaprogramming/output/generate_stream_enum_field.h
      function void DebugPrint(enum_field_stream_chunk* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Element = ");
            DebugPrint(S->Element);
            DebugPrint("\n");

            DebugPrint("  Next = ");
            DebugPrint(S->Next);
            DebugPrint("\n");

        }
      }

      // Source File : src/metaprogramming/output/generate_stream_enum_field.h
      function void DebugPrint(enum_field_stream* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  FirstChunk = ");
            DebugPrint(S->FirstChunk);
            DebugPrint("\n");

            DebugPrint("  LastChunk = ");
            DebugPrint(S->LastChunk);
            DebugPrint("\n");

        }
      }

      // Source File : src/metaprogramming/output/generate_stream_enum_field.h
      function void DebugPrint(enum_field_iterator* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Stream = ");
            DebugPrint(S->Stream);
            DebugPrint("\n");

            DebugPrint("  At = ");
            DebugPrint(S->At);
            DebugPrint("\n");

        }
      }

      // Source File : src/metaprogramming/output/generate_cursor_enum_def.h
      function void DebugPrint(enum_def_cursor* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Start = ");
            DebugPrint(S->Start);
            DebugPrint("\n");

            DebugPrint("  End = ");
            DebugPrint(S->End);
            DebugPrint("\n");

            DebugPrint("  At = ");
            DebugPrint(S->At);
            DebugPrint("\n");

        }
      }

      // Source File : src/metaprogramming/output/generate_stream_enum_def.h
      function void DebugPrint(enum_def_stream_chunk* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Element = ");
            DebugPrint(S->Element);
            DebugPrint("\n");

            DebugPrint("  Next = ");
            DebugPrint(S->Next);
            DebugPrint("\n");

        }
      }

      // Source File : src/metaprogramming/output/generate_stream_enum_def.h
      function void DebugPrint(enum_def_stream* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  FirstChunk = ");
            DebugPrint(S->FirstChunk);
            DebugPrint("\n");

            DebugPrint("  LastChunk = ");
            DebugPrint(S->LastChunk);
            DebugPrint("\n");

        }
      }

      // Source File : src/metaprogramming/output/generate_stream_enum_def.h
      function void DebugPrint(enum_def_iterator* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Stream = ");
            DebugPrint(S->Stream);
            DebugPrint("\n");

            DebugPrint("  At = ");
            DebugPrint(S->At);
            DebugPrint("\n");

        }
      }

      // Source File : src/metaprogramming/output/generate_cursor_v3.h
      function void DebugPrint(v3_cursor* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Start = ");
            DebugPrint(S->Start);
            DebugPrint("\n");

            DebugPrint("  End = ");
            DebugPrint(S->End);
            DebugPrint("\n");

            DebugPrint("  At = ");
            DebugPrint(S->At);
            DebugPrint("\n");

        }
      }

      // Source File : src/metaprogramming/output/d_union_c_decl.h
      function void DebugPrint(c_decl* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Type = ");
            DebugPrint(S->Type);
            DebugPrint("\n");

        }
      }

      // Source File : src/metaprogramming/output/generate_stream_d_union_member.h
      function void DebugPrint(d_union_member_stream_chunk* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Element = ");
            DebugPrint(S->Element);
            DebugPrint("\n");

            DebugPrint("  Next = ");
            DebugPrint(S->Next);
            DebugPrint("\n");

        }
      }

      // Source File : src/metaprogramming/output/generate_stream_d_union_member.h
      function void DebugPrint(d_union_member_stream* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  FirstChunk = ");
            DebugPrint(S->FirstChunk);
            DebugPrint("\n");

            DebugPrint("  LastChunk = ");
            DebugPrint(S->LastChunk);
            DebugPrint("\n");

        }
      }

      // Source File : src/metaprogramming/output/generate_stream_d_union_member.h
      function void DebugPrint(d_union_member_iterator* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Stream = ");
            DebugPrint(S->Stream);
            DebugPrint("\n");

            DebugPrint("  At = ");
            DebugPrint(S->At);
            DebugPrint("\n");

        }
      }

      // Source File : src/metaprogramming/output/d_union_ui_render_command.h
      function void DebugPrint(ui_render_command* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Type = ");
            DebugPrint(S->Type);
            DebugPrint("\n");

        }
      }

      // Source File : src/metaprogramming/output/generate_stream_struct_def.h
      function void DebugPrint(struct_def_stream_chunk* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Element = ");
            DebugPrint(S->Element);
            DebugPrint("\n");

            DebugPrint("  Next = ");
            DebugPrint(S->Next);
            DebugPrint("\n");

        }
      }

      // Source File : src/metaprogramming/output/generate_stream_struct_def.h
      function void DebugPrint(struct_def_stream* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  FirstChunk = ");
            DebugPrint(S->FirstChunk);
            DebugPrint("\n");

            DebugPrint("  LastChunk = ");
            DebugPrint(S->LastChunk);
            DebugPrint("\n");

        }
      }

      // Source File : src/metaprogramming/output/generate_stream_struct_def.h
      function void DebugPrint(struct_def_iterator* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Stream = ");
            DebugPrint(S->Stream);
            DebugPrint("\n");

            DebugPrint("  At = ");
            DebugPrint(S->At);
            DebugPrint("\n");

        }
      }

      // Source File : src/metaprogramming/output/generate_cursor_counted_string.h
      function void DebugPrint(counted_string_cursor* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Start = ");
            DebugPrint(S->Start);
            DebugPrint("\n");

            DebugPrint("  End = ");
            DebugPrint(S->End);
            DebugPrint("\n");

            DebugPrint("  At = ");
            DebugPrint(S->At);
            DebugPrint("\n");

        }
      }

      // Source File : src/metaprogramming/output/generate_stream_tag.h
      function void DebugPrint(tag_stream_chunk* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Element = ");
            DebugPrint(S->Element);
            DebugPrint("\n");

            DebugPrint("  Next = ");
            DebugPrint(S->Next);
            DebugPrint("\n");

        }
      }

      // Source File : src/metaprogramming/output/generate_stream_tag.h
      function void DebugPrint(tag_stream* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  FirstChunk = ");
            DebugPrint(S->FirstChunk);
            DebugPrint("\n");

            DebugPrint("  LastChunk = ");
            DebugPrint(S->LastChunk);
            DebugPrint("\n");

        }
      }

      // Source File : src/metaprogramming/output/generate_stream_tag.h
      function void DebugPrint(tag_iterator* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Stream = ");
            DebugPrint(S->Stream);
            DebugPrint("\n");

            DebugPrint("  At = ");
            DebugPrint(S->At);
            DebugPrint("\n");

        }
      }

      // Source File : src/metaprogramming/output/generate_cursor_c_parse_result.h
      function void DebugPrint(c_parse_result_cursor* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Start = ");
            DebugPrint(S->Start);
            DebugPrint("\n");

            DebugPrint("  End = ");
            DebugPrint(S->End);
            DebugPrint("\n");

            DebugPrint("  At = ");
            DebugPrint(S->At);
            DebugPrint("\n");

        }
      }

      // Source File : src/metaprogramming/output/d_union_c_decl_function.h
      function void DebugPrint(c_decl_function* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Type = ");
            DebugPrint(S->Type);
            DebugPrint("\n");

        }
      }

      // Source File : src/metaprogramming/output/generate_cursor_c_decl.h
      function void DebugPrint(c_decl_cursor* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Start = ");
            DebugPrint(S->Start);
            DebugPrint("\n");

            DebugPrint("  End = ");
            DebugPrint(S->End);
            DebugPrint("\n");

            DebugPrint("  At = ");
            DebugPrint(S->At);
            DebugPrint("\n");

        }
      }

      // Source File : src/metaprogramming/output/generate_cursor_struct_def.h
      function void DebugPrint(struct_def_cursor* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Start = ");
            DebugPrint(S->Start);
            DebugPrint("\n");

            DebugPrint("  End = ");
            DebugPrint(S->End);
            DebugPrint("\n");

            DebugPrint("  At = ");
            DebugPrint(S->At);
            DebugPrint("\n");

        }
      }

      // Source File : src/metaprogramming/output/generate_stream_person.h
      function void DebugPrint(person_stream_chunk* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Element = ");
            DebugPrint(S->Element);
            DebugPrint("\n");

            DebugPrint("  Next = ");
            DebugPrint(S->Next);
            DebugPrint("\n");

        }
      }

      // Source File : src/metaprogramming/output/generate_stream_person.h
      function void DebugPrint(person_stream* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  FirstChunk = ");
            DebugPrint(S->FirstChunk);
            DebugPrint("\n");

            DebugPrint("  LastChunk = ");
            DebugPrint(S->LastChunk);
            DebugPrint("\n");

        }
      }

      // Source File : src/metaprogramming/output/generate_stream_person.h
      function void DebugPrint(person_iterator* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Stream = ");
            DebugPrint(S->Stream);
            DebugPrint("\n");

            DebugPrint("  At = ");
            DebugPrint(S->At);
            DebugPrint("\n");

        }
      }

      // Source File : src/metaprogramming/output/generate_stream_counted_string.h
      function void DebugPrint(counted_string_stream_chunk* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Element = ");
            DebugPrint(S->Element);
            DebugPrint("\n");

            DebugPrint("  Next = ");
            DebugPrint(S->Next);
            DebugPrint("\n");

        }
      }

      // Source File : src/metaprogramming/output/generate_stream_counted_string.h
      function void DebugPrint(counted_string_stream* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  FirstChunk = ");
            DebugPrint(S->FirstChunk);
            DebugPrint("\n");

            DebugPrint("  LastChunk = ");
            DebugPrint(S->LastChunk);
            DebugPrint("\n");

        }
      }

      // Source File : src/metaprogramming/output/generate_stream_counted_string.h
      function void DebugPrint(counted_string_iterator* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Stream = ");
            DebugPrint(S->Stream);
            DebugPrint("\n");

            DebugPrint("  At = ");
            DebugPrint(S->At);
            DebugPrint("\n");

        }
      }

      // Source File : src/metaprogramming/output/generate_stream_todo.h
      function void DebugPrint(todo_stream_chunk* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Element = ");
            DebugPrint(S->Element);
            DebugPrint("\n");

            DebugPrint("  Next = ");
            DebugPrint(S->Next);
            DebugPrint("\n");

        }
      }

      // Source File : src/metaprogramming/output/generate_stream_todo.h
      function void DebugPrint(todo_stream* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  FirstChunk = ");
            DebugPrint(S->FirstChunk);
            DebugPrint("\n");

            DebugPrint("  LastChunk = ");
            DebugPrint(S->LastChunk);
            DebugPrint("\n");

        }
      }

      // Source File : src/metaprogramming/output/generate_stream_todo.h
      function void DebugPrint(todo_iterator* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Stream = ");
            DebugPrint(S->Stream);
            DebugPrint("\n");

            DebugPrint("  At = ");
            DebugPrint(S->At);
            DebugPrint("\n");

        }
      }

      // Source File : src/metaprogramming/output/d_union_work_queue_entry.h
      function void DebugPrint(work_queue_entry* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Type = ");
            DebugPrint(S->Type);
            DebugPrint("\n");

        }
      }

      // Source File : src/net/network.h
      function void DebugPrint(client_state* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Id = ");
            DebugPrint(S->Id);
            DebugPrint("\n");

            DebugPrint("  Counter = ");
            DebugPrint(S->Counter);
            DebugPrint("\n");

            DebugPrint("  P = ");
            DebugPrint(S->P);
            DebugPrint("\n");

        }
      }

      // Source File : src/net/network.h
      function void DebugPrint(server_state* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Clients = ");
            DebugPrint(S->Clients);
            DebugPrint("\n");

        }
      }

      // Source File : src/net/network.h
      function void DebugPrint(client_to_server_message* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Client = ");
            DebugPrint(S->Client);
            DebugPrint("\n");

        }
      }

      // Source File : src/net/network.h
      function void DebugPrint(handshake_message* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  ClientId = ");
            DebugPrint(S->ClientId);
            DebugPrint("\n");

        }
      }

      // Source File : src/net/network.h
      function void DebugPrint(socket_t* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Id = ");
            DebugPrint(S->Id);
            DebugPrint("\n");

            DebugPrint("  Type = ");
            DebugPrint(S->Type);
            DebugPrint("\n");

        }
      }

      // Source File : src/engine/headers/camera.h
      function void DebugPrint(plane* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  P = ");
            DebugPrint(S->P);
            DebugPrint("\n");

            DebugPrint("  Normal = ");
            DebugPrint(S->Normal);
            DebugPrint("\n");

            DebugPrint("  d = ");
            DebugPrint(S->d);
            DebugPrint("\n");

        }
      }

      // Source File : src/engine/headers/camera.h
      function void DebugPrint(frustum* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  farClip = ");
            DebugPrint(S->farClip);
            DebugPrint("\n");

            DebugPrint("  nearClip = ");
            DebugPrint(S->nearClip);
            DebugPrint("\n");

            DebugPrint("  width = ");
            DebugPrint(S->width);
            DebugPrint("\n");

            DebugPrint("  FOV = ");
            DebugPrint(S->FOV);
            DebugPrint("\n");

            DebugPrint("  Top = ");
            DebugPrint(S->Top);
            DebugPrint("\n");

            DebugPrint("  Bot = ");
            DebugPrint(S->Bot);
            DebugPrint("\n");

            DebugPrint("  Left = ");
            DebugPrint(S->Left);
            DebugPrint("\n");

            DebugPrint("  Right = ");
            DebugPrint(S->Right);
            DebugPrint("\n");

        }
      }

      // Source File : src/engine/headers/camera.h
      function void DebugPrint(camera* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Frust = ");
            DebugPrint(S->Frust);
            DebugPrint("\n");

            DebugPrint("  TargetP = ");
            DebugPrint(S->TargetP);
            DebugPrint("\n");

            DebugPrint("  CurrentP = ");
            DebugPrint(S->CurrentP);
            DebugPrint("\n");

            DebugPrint("  ViewingTarget = ");
            DebugPrint(S->ViewingTarget);
            DebugPrint("\n");

            DebugPrint("  RenderSpacePosition = ");
            DebugPrint(S->RenderSpacePosition);
            DebugPrint("\n");

            DebugPrint("  Pitch = ");
            DebugPrint(S->Pitch);
            DebugPrint("\n");

            DebugPrint("  Roll = ");
            DebugPrint(S->Roll);
            DebugPrint("\n");

            DebugPrint("  Yaw = ");
            DebugPrint(S->Yaw);
            DebugPrint("\n");

            DebugPrint("  DistanceFromTarget = ");
            DebugPrint(S->DistanceFromTarget);
            DebugPrint("\n");

            DebugPrint("  Front = ");
            DebugPrint(S->Front);
            DebugPrint("\n");

            DebugPrint("  Right = ");
            DebugPrint(S->Right);
            DebugPrint("\n");

            DebugPrint("  Up = ");
            DebugPrint(S->Up);
            DebugPrint("\n");

        }
      }

      // Source File : src/engine/headers/work_queue.h
      function void DebugPrint(work_queue_entry_copy_buffer* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Src = ");
            DebugPrint(S->Src);
            DebugPrint("\n");

            DebugPrint("  Dest = ");
            DebugPrint(S->Dest);
            DebugPrint("\n");

            DebugPrint("  Basis = ");
            DebugPrint(S->Basis);
            DebugPrint("\n");

        }
      }

      // Source File : src/engine/headers/work_queue.h
      function void DebugPrint(work_queue_entry_init_world_chunk* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Input = ");
            DebugPrint(S->Input);
            DebugPrint("\n");

        }
      }

      // Source File : src/engine/headers/triangle.h
      function void DebugPrint(edge* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  P0 = ");
            DebugPrint(S->P0);
            DebugPrint("\n");

            DebugPrint("  P1 = ");
            DebugPrint(S->P1);
            DebugPrint("\n");

            DebugPrint("  LengthSq = ");
            DebugPrint(S->LengthSq);
            DebugPrint("\n");

        }
      }

      // Source File : src/engine/headers/triangle.h
      function void DebugPrint(triangle* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Points = ");
            DebugPrint(S->Points);
            DebugPrint("\n");

            DebugPrint("  Index = ");
            DebugPrint(S->Index);
            DebugPrint("\n");

        }
      }

      // Source File : src/engine/headers/triangle.h
      function void DebugPrint(boundary_voxels* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Points = ");
            DebugPrint(S->Points);
            DebugPrint("\n");

            DebugPrint("  At = ");
            DebugPrint(S->At);
            DebugPrint("\n");

            DebugPrint("  End = ");
            DebugPrint(S->End);
            DebugPrint("\n");

            DebugPrint("  Min = ");
            DebugPrint(S->Min);
            DebugPrint("\n");

            DebugPrint("  Max = ");
            DebugPrint(S->Max);
            DebugPrint("\n");

        }
      }

      // Source File : src/engine/headers/triangle.h
      function void DebugPrint(current_triangles* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Tris = ");
            DebugPrint(S->Tris);
            DebugPrint("\n");

            DebugPrint("  Max = ");
            DebugPrint(S->Max);
            DebugPrint("\n");

            DebugPrint("  Count = ");
            DebugPrint(S->Count);
            DebugPrint("\n");

            DebugPrint("  CurrentPointIndex = ");
            DebugPrint(S->CurrentPointIndex);
            DebugPrint("\n");

            DebugPrint("  SurfacePoints = ");
            DebugPrint(S->SurfacePoints);
            DebugPrint("\n");

        }
      }

      // Source File : src/engine/headers/canonical_position.h
      function void DebugPrint(canonical_position* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Offset = ");
            DebugPrint(S->Offset);
            DebugPrint("\n");

            DebugPrint("  WorldP = ");
            DebugPrint(S->WorldP);
            DebugPrint("\n");

        }
      }

      // Source File : src/engine/headers/mesh.h
      function void DebugPrint(loaded_collada_mesh* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Mesh = ");
            DebugPrint(S->Mesh);
            DebugPrint("\n");

            DebugPrint("  Dim = ");
            DebugPrint(S->Dim);
            DebugPrint("\n");

        }
      }

      // Source File : src/engine/headers/mesh.h
      function void DebugPrint(free_mesh* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Mesh = ");
            DebugPrint(S->Mesh);
            DebugPrint("\n");

            DebugPrint("  Next = ");
            DebugPrint(S->Next);
            DebugPrint("\n");

        }
      }

      // Source File : src/engine/headers/mesh.h
      function void DebugPrint(mesh_freelist* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  FirstFree = ");
            DebugPrint(S->FirstFree);
            DebugPrint("\n");

            DebugPrint("  Containers = ");
            DebugPrint(S->Containers);
            DebugPrint("\n");

        }
      }

      // Source File : src/engine/headers/xml.h
      function void DebugPrint(xml_property* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Name = ");
            DebugPrint(S->Name);
            DebugPrint("\n");

            DebugPrint("  Value = ");
            DebugPrint(S->Value);
            DebugPrint("\n");

            DebugPrint("  Next = ");
            DebugPrint(S->Next);
            DebugPrint("\n");

        }
      }

      // Source File : src/engine/headers/xml.h
      function void DebugPrint(xml_token* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Type = ");
            DebugPrint(S->Type);
            DebugPrint("\n");

            DebugPrint("  Property = ");
            DebugPrint(S->Property);
            DebugPrint("\n");

        }
      }

      // Source File : src/engine/headers/xml.h
      function void DebugPrint(xml_tag* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Open = ");
            DebugPrint(S->Open);
            DebugPrint("\n");

            DebugPrint("  Parent = ");
            DebugPrint(S->Parent);
            DebugPrint("\n");

            DebugPrint("  Sibling = ");
            DebugPrint(S->Sibling);
            DebugPrint("\n");

            DebugPrint("  NextInHash = ");
            DebugPrint(S->NextInHash);
            DebugPrint("\n");

            DebugPrint("  HashValue = ");
            DebugPrint(S->HashValue);
            DebugPrint("\n");

            DebugPrint("  Value = ");
            DebugPrint(S->Value);
            DebugPrint("\n");

            DebugPrint("  Properties = ");
            DebugPrint(S->Properties);
            DebugPrint("\n");

            DebugPrint("  NextPropertySlot = ");
            DebugPrint(S->NextPropertySlot);
            DebugPrint("\n");

        }
      }

      // Source File : src/engine/headers/xml.h
      function void DebugPrint(xml_tag_stream* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Start = ");
            DebugPrint(S->Start);
            DebugPrint("\n");

            DebugPrint("  At = ");
            DebugPrint(S->At);
            DebugPrint("\n");

            DebugPrint("  End = ");
            DebugPrint(S->End);
            DebugPrint("\n");

        }
      }

      // Source File : src/engine/headers/xml.h
      function void DebugPrint(xml_parsing_at_indicators* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  LastClosedTag = ");
            DebugPrint(S->LastClosedTag);
            DebugPrint("\n");

            DebugPrint("  CurrentlyOpenTag = ");
            DebugPrint(S->CurrentlyOpenTag);
            DebugPrint("\n");

        }
      }

      // Source File : src/engine/headers/graphics.h
      function void DebugPrint(post_processing_group* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  FBO = ");
            DebugPrint(S->FBO);
            DebugPrint("\n");

            DebugPrint("  Shader = ");
            DebugPrint(S->Shader);
            DebugPrint("\n");

            DebugPrint("  VertexBuffer = ");
            DebugPrint(S->VertexBuffer);
            DebugPrint("\n");

            DebugPrint("  ColorBuffer = ");
            DebugPrint(S->ColorBuffer);
            DebugPrint("\n");

        }
      }

      // Source File : src/engine/headers/graphics.h
      function void DebugPrint(graphics* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Camera = ");
            DebugPrint(S->Camera);
            DebugPrint("\n");

            DebugPrint("  Lights = ");
            DebugPrint(S->Lights);
            DebugPrint("\n");

            DebugPrint("  gBuffer = ");
            DebugPrint(S->gBuffer);
            DebugPrint("\n");

            DebugPrint("  AoGroup = ");
            DebugPrint(S->AoGroup);
            DebugPrint("\n");

            DebugPrint("  SG = ");
            DebugPrint(S->SG);
            DebugPrint("\n");

            DebugPrint("  PostGroup = ");
            DebugPrint(S->PostGroup);
            DebugPrint("\n");

            DebugPrint("  GpuBuffers = ");
            DebugPrint(S->GpuBuffers);
            DebugPrint("\n");

            DebugPrint("  GpuBufferWriteIndex = ");
            DebugPrint(S->GpuBufferWriteIndex);
            DebugPrint("\n");

            DebugPrint("  Memory = ");
            DebugPrint(S->Memory);
            DebugPrint("\n");

        }
      }

      // Source File : src/engine/headers/render.h
      function void DebugPrint(shader_uniform* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Type = ");
            DebugPrint(S->Type);
            DebugPrint("\n");

        }
      }

      // Source File : src/engine/headers/render.h
      function void DebugPrint(shader* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  ID = ");
            DebugPrint(S->ID);
            DebugPrint("\n");

            DebugPrint("  FirstUniform = ");
            DebugPrint(S->FirstUniform);
            DebugPrint("\n");

        }
      }

      // Source File : src/engine/headers/render.h
      function void DebugPrint(texture* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  ID = ");
            DebugPrint(S->ID);
            DebugPrint("\n");

            DebugPrint("  Dim = ");
            DebugPrint(S->Dim);
            DebugPrint("\n");

        }
      }

      // Source File : src/engine/headers/render.h
      function void DebugPrint(light* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Position = ");
            DebugPrint(S->Position);
            DebugPrint("\n");

            DebugPrint("  Color = ");
            DebugPrint(S->Color);
            DebugPrint("\n");

        }
      }

      // Source File : src/engine/headers/render.h
      function void DebugPrint(game_lights* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  ColorTex = ");
            DebugPrint(S->ColorTex);
            DebugPrint("\n");

            DebugPrint("  PositionTex = ");
            DebugPrint(S->PositionTex);
            DebugPrint("\n");

            DebugPrint("  IndexToUV = ");
            DebugPrint(S->IndexToUV);
            DebugPrint("\n");

            DebugPrint("  Count = ");
            DebugPrint(S->Count);
            DebugPrint("\n");

            DebugPrint("  Lights = ");
            DebugPrint(S->Lights);
            DebugPrint("\n");

        }
      }

      // Source File : src/engine/headers/render.h
      function void DebugPrint(RenderBasis* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  ModelMatrix = ");
            DebugPrint(S->ModelMatrix);
            DebugPrint("\n");

            DebugPrint("  ViewMatrix = ");
            DebugPrint(S->ViewMatrix);
            DebugPrint("\n");

            DebugPrint("  ProjectionMatrix = ");
            DebugPrint(S->ProjectionMatrix);
            DebugPrint("\n");

        }
      }

      // Source File : src/engine/headers/render.h
      function void DebugPrint(framebuffer* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  ID = ");
            DebugPrint(S->ID);
            DebugPrint("\n");

            DebugPrint("  Attachments = ");
            DebugPrint(S->Attachments);
            DebugPrint("\n");

        }
      }

      // Source File : src/engine/headers/render.h
      function void DebugPrint(ao_render_group* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  FBO = ");
            DebugPrint(S->FBO);
            DebugPrint("\n");

            DebugPrint("  Shader = ");
            DebugPrint(S->Shader);
            DebugPrint("\n");

            DebugPrint("  DebugSsaoShader = ");
            DebugPrint(S->DebugSsaoShader);
            DebugPrint("\n");

            DebugPrint("  SsaoKernel = ");
            DebugPrint(S->SsaoKernel);
            DebugPrint("\n");

            DebugPrint("  SsaoKernelUniform = ");
            DebugPrint(S->SsaoKernelUniform);
            DebugPrint("\n");

            DebugPrint("  Texture = ");
            DebugPrint(S->Texture);
            DebugPrint("\n");

            DebugPrint("  NoiseTile = ");
            DebugPrint(S->NoiseTile);
            DebugPrint("\n");

        }
      }

      // Source File : src/engine/headers/render.h
      function void DebugPrint(g_buffer_textures* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Color = ");
            DebugPrint(S->Color);
            DebugPrint("\n");

            DebugPrint("  Normal = ");
            DebugPrint(S->Normal);
            DebugPrint("\n");

            DebugPrint("  Position = ");
            DebugPrint(S->Position);
            DebugPrint("\n");

        }
      }

      // Source File : src/engine/headers/render.h
      function void DebugPrint(g_buffer_render_group* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  FBO = ");
            DebugPrint(S->FBO);
            DebugPrint("\n");

            DebugPrint("  Textures = ");
            DebugPrint(S->Textures);
            DebugPrint("\n");

            DebugPrint("  LightingShader = ");
            DebugPrint(S->LightingShader);
            DebugPrint("\n");

            DebugPrint("  gBufferShader = ");
            DebugPrint(S->gBufferShader);
            DebugPrint("\n");

            DebugPrint("  ShadowMVP = ");
            DebugPrint(S->ShadowMVP);
            DebugPrint("\n");

            DebugPrint("  ViewProjection = ");
            DebugPrint(S->ViewProjection);
            DebugPrint("\n");

        }
      }

      // Source File : src/engine/headers/render.h
      function void DebugPrint(shadow_render_group* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  FramebufferName = ");
            DebugPrint(S->FramebufferName);
            DebugPrint("\n");

            DebugPrint("  MVP_ID = ");
            DebugPrint(S->MVP_ID);
            DebugPrint("\n");

            DebugPrint("  DebugTextureShader = ");
            DebugPrint(S->DebugTextureShader);
            DebugPrint("\n");

            DebugPrint("  DepthShader = ");
            DebugPrint(S->DepthShader);
            DebugPrint("\n");

            DebugPrint("  ShadowMap = ");
            DebugPrint(S->ShadowMap);
            DebugPrint("\n");

        }
      }

      // Source File : src/engine/headers/render.h
      function void DebugPrint(untextured_3d_geometry_buffer* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Verts = ");
            DebugPrint(S->Verts);
            DebugPrint("\n");

            DebugPrint("  Colors = ");
            DebugPrint(S->Colors);
            DebugPrint("\n");

            DebugPrint("  Normals = ");
            DebugPrint(S->Normals);
            DebugPrint("\n");

            DebugPrint("  End = ");
            DebugPrint(S->End);
            DebugPrint("\n");

            DebugPrint("  At = ");
            DebugPrint(S->At);
            DebugPrint("\n");

        }
      }

      // Source File : src/engine/headers/render.h
      function void DebugPrint(gpu_mapped_element_buffer* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  VertexHandle = ");
            DebugPrint(S->VertexHandle);
            DebugPrint("\n");

            DebugPrint("  NormalHandle = ");
            DebugPrint(S->NormalHandle);
            DebugPrint("\n");

            DebugPrint("  ColorHandle = ");
            DebugPrint(S->ColorHandle);
            DebugPrint("\n");

            DebugPrint("  Buffer = ");
            DebugPrint(S->Buffer);
            DebugPrint("\n");

        }
      }

      // Source File : src/engine/headers/render.h
      function void DebugPrint(textured_2d_geometry_buffer* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Verts = ");
            DebugPrint(S->Verts);
            DebugPrint("\n");

            DebugPrint("  Colors = ");
            DebugPrint(S->Colors);
            DebugPrint("\n");

            DebugPrint("  UVs = ");
            DebugPrint(S->UVs);
            DebugPrint("\n");

            DebugPrint("  End = ");
            DebugPrint(S->End);
            DebugPrint("\n");

            DebugPrint("  At = ");
            DebugPrint(S->At);
            DebugPrint("\n");

        }
      }

      // Source File : src/engine/headers/render.h
      function void DebugPrint(untextured_2d_geometry_buffer* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Verts = ");
            DebugPrint(S->Verts);
            DebugPrint("\n");

            DebugPrint("  Colors = ");
            DebugPrint(S->Colors);
            DebugPrint("\n");

            DebugPrint("  End = ");
            DebugPrint(S->End);
            DebugPrint("\n");

            DebugPrint("  At = ");
            DebugPrint(S->At);
            DebugPrint("\n");

        }
      }

      // Source File : src/engine/headers/animation.h
      function void DebugPrint(keyframe* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  tEnd = ");
            DebugPrint(S->tEnd);
            DebugPrint("\n");

            DebugPrint("  Value = ");
            DebugPrint(S->Value);
            DebugPrint("\n");

        }
      }

      // Source File : src/engine/headers/animation.h
      function void DebugPrint(animation* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  t = ");
            DebugPrint(S->t);
            DebugPrint("\n");

            DebugPrint("  tEnd = ");
            DebugPrint(S->tEnd);
            DebugPrint("\n");

            DebugPrint("  xKeyframeCount = ");
            DebugPrint(S->xKeyframeCount);
            DebugPrint("\n");

            DebugPrint("  xKeyframes = ");
            DebugPrint(S->xKeyframes);
            DebugPrint("\n");

            DebugPrint("  yKeyframeCount = ");
            DebugPrint(S->yKeyframeCount);
            DebugPrint("\n");

            DebugPrint("  yKeyframes = ");
            DebugPrint(S->yKeyframes);
            DebugPrint("\n");

            DebugPrint("  zKeyframeCount = ");
            DebugPrint(S->zKeyframeCount);
            DebugPrint("\n");

            DebugPrint("  zKeyframes = ");
            DebugPrint(S->zKeyframes);
            DebugPrint("\n");

        }
      }

      // Source File : src/engine/bonsai.h
      function void DebugPrint(game_mode* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  ActiveMode = ");
            DebugPrint(S->ActiveMode);
            DebugPrint("\n");

            DebugPrint("  TimeRunning = ");
            DebugPrint(S->TimeRunning);
            DebugPrint("\n");

        }
      }

      // Source File : src/engine/bonsai.h
      function void DebugPrint(voxel* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Flags = ");
            DebugPrint(S->Flags);
            DebugPrint("\n");

            DebugPrint("  Color = ");
            DebugPrint(S->Color);
            DebugPrint("\n");

        }
      }

      // Source File : src/engine/bonsai.h
      function void DebugPrint(boundary_voxel* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  V = ");
            DebugPrint(S->V);
            DebugPrint("\n");

            DebugPrint("  Offset = ");
            DebugPrint(S->Offset);
            DebugPrint("\n");

        }
      }

      // Source File : src/engine/bonsai.h
      function void DebugPrint(chunk_data* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Flags = ");
            DebugPrint(S->Flags);
            DebugPrint("\n");

            DebugPrint("  Voxels = ");
            DebugPrint(S->Voxels);
            DebugPrint("\n");

            DebugPrint("  Pad = ");
            DebugPrint(S->Pad);
            DebugPrint("\n");

        }
      }

      // Source File : src/engine/bonsai.h
      function void DebugPrint(model* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Mesh = ");
            DebugPrint(S->Mesh);
            DebugPrint("\n");

            DebugPrint("  Dim = ");
            DebugPrint(S->Dim);
            DebugPrint("\n");

            DebugPrint("  Animation = ");
            DebugPrint(S->Animation);
            DebugPrint("\n");

        }
      }

      // Source File : src/engine/bonsai.h
      function void DebugPrint(physics* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Velocity = ");
            DebugPrint(S->Velocity);
            DebugPrint("\n");

            DebugPrint("  Force = ");
            DebugPrint(S->Force);
            DebugPrint("\n");

            DebugPrint("  Delta = ");
            DebugPrint(S->Delta);
            DebugPrint("\n");

            DebugPrint("  Drag = ");
            DebugPrint(S->Drag);
            DebugPrint("\n");

            DebugPrint("  Mass = ");
            DebugPrint(S->Mass);
            DebugPrint("\n");

            DebugPrint("  Speed = ");
            DebugPrint(S->Speed);
            DebugPrint("\n");

        }
      }

      // Source File : src/engine/bonsai.h
      function void DebugPrint(particle* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Offset = ");
            DebugPrint(S->Offset);
            DebugPrint("\n");

            DebugPrint("  Physics = ");
            DebugPrint(S->Physics);
            DebugPrint("\n");

            DebugPrint("  Color = ");
            DebugPrint(S->Color);
            DebugPrint("\n");

            DebugPrint("  RemainingLifespan = ");
            DebugPrint(S->RemainingLifespan);
            DebugPrint("\n");

        }
      }

      // Source File : src/engine/bonsai.h
      function void DebugPrint(particle_system_init_params* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  SpawnRegion = ");
            DebugPrint(S->SpawnRegion);
            DebugPrint("\n");

            DebugPrint("  Physics = ");
            DebugPrint(S->Physics);
            DebugPrint("\n");

            DebugPrint("  SystemMovementCoefficient = ");
            DebugPrint(S->SystemMovementCoefficient);
            DebugPrint("\n");

            DebugPrint("  EmissionLifespan = ");
            DebugPrint(S->EmissionLifespan);
            DebugPrint("\n");

            DebugPrint("  EmissionChance = ");
            DebugPrint(S->EmissionChance);
            DebugPrint("\n");

            DebugPrint("  ParticleLifespan = ");
            DebugPrint(S->ParticleLifespan);
            DebugPrint("\n");

            DebugPrint("  StartingDiameter = ");
            DebugPrint(S->StartingDiameter);
            DebugPrint("\n");

            DebugPrint("  Entropy = ");
            DebugPrint(S->Entropy);
            DebugPrint("\n");

            DebugPrint("  Colors = ");
            DebugPrint(S->Colors);
            DebugPrint("\n");

        }
      }

      // Source File : src/engine/bonsai.h
      function void DebugPrint(particle_system* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Entropy = ");
            DebugPrint(S->Entropy);
            DebugPrint("\n");

            DebugPrint("  ActiveParticles = ");
            DebugPrint(S->ActiveParticles);
            DebugPrint("\n");

            DebugPrint("  ParticleDuration = ");
            DebugPrint(S->ParticleDuration);
            DebugPrint("\n");

            DebugPrint("  SpawnRegion = ");
            DebugPrint(S->SpawnRegion);
            DebugPrint("\n");

            DebugPrint("  StartingDiameter = ");
            DebugPrint(S->StartingDiameter);
            DebugPrint("\n");

            DebugPrint("  EmissionLifespan = ");
            DebugPrint(S->EmissionLifespan);
            DebugPrint("\n");

            DebugPrint("  ParticleLifespan = ");
            DebugPrint(S->ParticleLifespan);
            DebugPrint("\n");

            DebugPrint("  EmissionChance = ");
            DebugPrint(S->EmissionChance);
            DebugPrint("\n");

            DebugPrint("  ParticlePhysics = ");
            DebugPrint(S->ParticlePhysics);
            DebugPrint("\n");

            DebugPrint("  SystemMovementCoefficient = ");
            DebugPrint(S->SystemMovementCoefficient);
            DebugPrint("\n");

            DebugPrint("  Colors = ");
            DebugPrint(S->Colors);
            DebugPrint("\n");

        }
      }

      // Source File : src/engine/api.h
      function void DebugPrint(thread* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  ThreadIndex = ");
            DebugPrint(S->ThreadIndex);
            DebugPrint("\n");

            DebugPrint("  ID = ");
            DebugPrint(S->ID);
            DebugPrint("\n");

        }
      }

      // Source File : src/engine/api.h
      function void DebugPrint(thread_local_state* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  PermMemory = ");
            DebugPrint(S->PermMemory);
            DebugPrint("\n");

            DebugPrint("  TempMemory = ");
            DebugPrint(S->TempMemory);
            DebugPrint("\n");

            DebugPrint("  MeshFreelist = ");
            DebugPrint(S->MeshFreelist);
            DebugPrint("\n");

            DebugPrint("  Noise = ");
            DebugPrint(S->Noise);
            DebugPrint("\n");

        }
      }

      // Source File : src/bonsai_stdlib/headers/stream.h
      function void DebugPrint(u32_stream* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Start = ");
            DebugPrint(S->Start);
            DebugPrint("\n");

            DebugPrint("  At = ");
            DebugPrint(S->At);
            DebugPrint("\n");

            DebugPrint("  End = ");
            DebugPrint(S->End);
            DebugPrint("\n");

        }
      }

      // Source File : src/bonsai_stdlib/headers/stream.h
      function void DebugPrint(u8_stream* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Start = ");
            DebugPrint(S->Start);
            DebugPrint("\n");

            DebugPrint("  At = ");
            DebugPrint(S->At);
            DebugPrint("\n");

            DebugPrint("  End = ");
            DebugPrint(S->End);
            DebugPrint("\n");

        }
      }

      // Source File : src/bonsai_stdlib/headers/stream.h
      function void DebugPrint(ansi_stream* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Start = ");
            DebugPrint(S->Start);
            DebugPrint("\n");

            DebugPrint("  At = ");
            DebugPrint(S->At);
            DebugPrint("\n");

            DebugPrint("  End = ");
            DebugPrint(S->End);
            DebugPrint("\n");

            DebugPrint("  Filename = ");
            DebugPrint(S->Filename);
            DebugPrint("\n");

        }
      }

      // Source File : src/bonsai_stdlib/headers/stream.h
      function void DebugPrint(r32_stream* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Start = ");
            DebugPrint(S->Start);
            DebugPrint("\n");

            DebugPrint("  At = ");
            DebugPrint(S->At);
            DebugPrint("\n");

            DebugPrint("  End = ");
            DebugPrint(S->End);
            DebugPrint("\n");

        }
      }

      // Source File : src/bonsai_stdlib/headers/stream.h
      function void DebugPrint(mesh_metadata* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  VertCount = ");
            DebugPrint(S->VertCount);
            DebugPrint("\n");

            DebugPrint("  NormalCount = ");
            DebugPrint(S->NormalCount);
            DebugPrint("\n");

            DebugPrint("  UVCount = ");
            DebugPrint(S->UVCount);
            DebugPrint("\n");

            DebugPrint("  FaceCount = ");
            DebugPrint(S->FaceCount);
            DebugPrint("\n");

        }
      }

      // Source File : src/bonsai_stdlib/headers/rect.h
      function void DebugPrint(rect2* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Min = ");
            DebugPrint(S->Min);
            DebugPrint("\n");

            DebugPrint("  Max = ");
            DebugPrint(S->Max);
            DebugPrint("\n");

        }
      }

      // Source File : src/bonsai_stdlib/headers/rect.h
      function void DebugPrint(aabb* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Center = ");
            DebugPrint(S->Center);
            DebugPrint("\n");

            DebugPrint("  Radius = ");
            DebugPrint(S->Radius);
            DebugPrint("\n");

        }
      }

      // Source File : src/bonsai_stdlib/headers/rect.h
      function void DebugPrint(ray* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Origin = ");
            DebugPrint(S->Origin);
            DebugPrint("\n");

            DebugPrint("  Dir = ");
            DebugPrint(S->Dir);
            DebugPrint("\n");

        }
      }

      // Source File : src/bonsai_stdlib/headers/input.h
      function void DebugPrint(input_event* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Clicked = ");
            DebugPrint(S->Clicked);
            DebugPrint("\n");

            DebugPrint("  Pressed = ");
            DebugPrint(S->Pressed);
            DebugPrint("\n");

        }
      }

      // Source File : src/bonsai_stdlib/headers/input.h
      function void DebugPrint(input* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Escape = ");
            DebugPrint(S->Escape);
            DebugPrint("\n");

            DebugPrint("  Space = ");
            DebugPrint(S->Space);
            DebugPrint("\n");

            DebugPrint("  Shift = ");
            DebugPrint(S->Shift);
            DebugPrint("\n");

            DebugPrint("  Ctrl = ");
            DebugPrint(S->Ctrl);
            DebugPrint("\n");

            DebugPrint("  Alt = ");
            DebugPrint(S->Alt);
            DebugPrint("\n");

            DebugPrint("  F12 = ");
            DebugPrint(S->F12);
            DebugPrint("\n");

            DebugPrint("  F11 = ");
            DebugPrint(S->F11);
            DebugPrint("\n");

            DebugPrint("  F10 = ");
            DebugPrint(S->F10);
            DebugPrint("\n");

            DebugPrint("  F9 = ");
            DebugPrint(S->F9);
            DebugPrint("\n");

            DebugPrint("  F8 = ");
            DebugPrint(S->F8);
            DebugPrint("\n");

            DebugPrint("  F7 = ");
            DebugPrint(S->F7);
            DebugPrint("\n");

            DebugPrint("  F6 = ");
            DebugPrint(S->F6);
            DebugPrint("\n");

            DebugPrint("  F5 = ");
            DebugPrint(S->F5);
            DebugPrint("\n");

            DebugPrint("  F4 = ");
            DebugPrint(S->F4);
            DebugPrint("\n");

            DebugPrint("  F3 = ");
            DebugPrint(S->F3);
            DebugPrint("\n");

            DebugPrint("  F2 = ");
            DebugPrint(S->F2);
            DebugPrint("\n");

            DebugPrint("  F1 = ");
            DebugPrint(S->F1);
            DebugPrint("\n");

            DebugPrint("  RMB = ");
            DebugPrint(S->RMB);
            DebugPrint("\n");

            DebugPrint("  LMB = ");
            DebugPrint(S->LMB);
            DebugPrint("\n");

            DebugPrint("  MMB = ");
            DebugPrint(S->MMB);
            DebugPrint("\n");

            DebugPrint("  W = ");
            DebugPrint(S->W);
            DebugPrint("\n");

            DebugPrint("  A = ");
            DebugPrint(S->A);
            DebugPrint("\n");

            DebugPrint("  S = ");
            DebugPrint(S->S);
            DebugPrint("\n");

            DebugPrint("  D = ");
            DebugPrint(S->D);
            DebugPrint("\n");

            DebugPrint("  Q = ");
            DebugPrint(S->Q);
            DebugPrint("\n");

            DebugPrint("  E = ");
            DebugPrint(S->E);
            DebugPrint("\n");

        }
      }

      // Source File : src/bonsai_stdlib/headers/work_queue.h
      function void DebugPrint(work_queue* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  EnqueueIndex = ");
            DebugPrint(S->EnqueueIndex);
            DebugPrint("\n");

            DebugPrint("  DequeueIndex = ");
            DebugPrint(S->DequeueIndex);
            DebugPrint("\n");

            DebugPrint("  Entries = ");
            DebugPrint(S->Entries);
            DebugPrint("\n");

            DebugPrint("  GlobalQueueSemaphore = ");
            DebugPrint(S->GlobalQueueSemaphore);
            DebugPrint("\n");

        }
      }

      // Source File : src/bonsai_stdlib/headers/string_builder.h
      function void DebugPrint(string_builder* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Memory = ");
            DebugPrint(S->Memory);
            DebugPrint("\n");

            DebugPrint("  Chunks = ");
            DebugPrint(S->Chunks);
            DebugPrint("\n");

        }
      }

      // Source File : src/bonsai_stdlib/headers/platform_struct.h
      function void DebugPrint(platform* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  LowPriority = ");
            DebugPrint(S->LowPriority);
            DebugPrint("\n");

            DebugPrint("  HighPriority = ");
            DebugPrint(S->HighPriority);
            DebugPrint("\n");

            DebugPrint("  QueueSemaphore = ");
            DebugPrint(S->QueueSemaphore);
            DebugPrint("\n");

            DebugPrint("  Threads = ");
            DebugPrint(S->Threads);
            DebugPrint("\n");

            DebugPrint("  MouseP = ");
            DebugPrint(S->MouseP);
            DebugPrint("\n");

            DebugPrint("  MouseDP = ");
            DebugPrint(S->MouseDP);
            DebugPrint("\n");

            DebugPrint("  Memory = ");
            DebugPrint(S->Memory);
            DebugPrint("\n");

            DebugPrint("  dt = ");
            DebugPrint(S->dt);
            DebugPrint("\n");

            DebugPrint("  WindowWidth = ");
            DebugPrint(S->WindowWidth);
            DebugPrint("\n");

            DebugPrint("  WindowHeight = ");
            DebugPrint(S->WindowHeight);
            DebugPrint("\n");

            DebugPrint("  Input = ");
            DebugPrint(S->Input);
            DebugPrint("\n");

        }
      }

      // Source File : src/bonsai_stdlib/headers/matrix.h
      function void DebugPrint(m4* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  E = ");
            DebugPrint(S->E);
            DebugPrint("\n");

        }
      }

      // Source File : src/bonsai_stdlib/headers/matrix.h
      function void DebugPrint(m_nxn* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Elements = ");
            DebugPrint(S->Elements);
            DebugPrint("\n");

            DebugPrint("  Rows = ");
            DebugPrint(S->Rows);
            DebugPrint("\n");

            DebugPrint("  Columns = ");
            DebugPrint(S->Columns);
            DebugPrint("\n");

            DebugPrint("  At = ");
            DebugPrint(S->At);
            DebugPrint("\n");

            DebugPrint("  End = ");
            DebugPrint(S->End);
            DebugPrint("\n");

        }
      }

      // Source File : src/bonsai_stdlib/headers/random.h
      function void DebugPrint(random_series* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Seed = ");
            DebugPrint(S->Seed);
            DebugPrint("\n");

        }
      }

      // Source File : src/bonsai_stdlib/headers/memory_arena.h
      function void DebugPrint(memory_arena* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Start = ");
            DebugPrint(S->Start);
            DebugPrint("\n");

            DebugPrint("  At = ");
            DebugPrint(S->At);
            DebugPrint("\n");

            DebugPrint("  End = ");
            DebugPrint(S->End);
            DebugPrint("\n");

            DebugPrint("  NextBlockSize = ");
            DebugPrint(S->NextBlockSize);
            DebugPrint("\n");

            DebugPrint("  Prev = ");
            DebugPrint(S->Prev);
            DebugPrint("\n");

        }
      }

      // Source File : src/bonsai_stdlib/headers/vector.h
      function void DebugPrint(v2i* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  x = ");
            DebugPrint(S->x);
            DebugPrint("\n");

            DebugPrint("  y = ");
            DebugPrint(S->y);
            DebugPrint("\n");

        }
      }

      // Source File : src/bonsai_stdlib/headers/vector.h
      function void DebugPrint(v2* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  x = ");
            DebugPrint(S->x);
            DebugPrint("\n");

            DebugPrint("  y = ");
            DebugPrint(S->y);
            DebugPrint("\n");

        }
      }

      // Source File : src/bonsai_stdlib/headers/heap_allocator.h
      function void DebugPrint(heap_allocation_block* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Type = ");
            DebugPrint(S->Type);
            DebugPrint("\n");

            DebugPrint("  Size = ");
            DebugPrint(S->Size);
            DebugPrint("\n");

            DebugPrint("  PrevAllocationSize = ");
            DebugPrint(S->PrevAllocationSize);
            DebugPrint("\n");

        }
      }

      // Source File : src/bonsai_stdlib/headers/heap_allocator.h
      function void DebugPrint(heap_allocator* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Heap = ");
            DebugPrint(S->Heap);
            DebugPrint("\n");

            DebugPrint("  Size = ");
            DebugPrint(S->Size);
            DebugPrint("\n");

        }
      }

      // Source File : src/bonsai_stdlib/headers/counted_string.h
      function void DebugPrint(char_cursor* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Start = ");
            DebugPrint(S->Start);
            DebugPrint("\n");

            DebugPrint("  End = ");
            DebugPrint(S->End);
            DebugPrint("\n");

            DebugPrint("  At = ");
            DebugPrint(S->At);
            DebugPrint("\n");

            DebugPrint("  Memory = ");
            DebugPrint(S->Memory);
            DebugPrint("\n");

        }
      }

      // Source File : src/bonsai_stdlib/headers/line.h
      function void DebugPrint(linei* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  MinP = ");
            DebugPrint(S->MinP);
            DebugPrint("\n");

            DebugPrint("  MaxP = ");
            DebugPrint(S->MaxP);
            DebugPrint("\n");

        }
      }

      // Source File : src/bonsai_stdlib/headers/line.h
      function void DebugPrint(line* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  MinP = ");
            DebugPrint(S->MinP);
            DebugPrint("\n");

            DebugPrint("  MaxP = ");
            DebugPrint(S->MaxP);
            DebugPrint("\n");

        }
      }

      // Source File : src/unix_platform.h
      function void DebugPrint(XVisualInfo* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  visual = ");
            DebugPrint(S->visual);
            DebugPrint("\n");

            DebugPrint("  visualid = ");
            DebugPrint(S->visualid);
            DebugPrint("\n");

            DebugPrint("  screen = ");
            DebugPrint(S->screen);
            DebugPrint("\n");

            DebugPrint("  depth = ");
            DebugPrint(S->depth);
            DebugPrint("\n");

            DebugPrint("  c_class = ");
            DebugPrint(S->c_class);
            DebugPrint("\n");

            DebugPrint("  red_mask = ");
            DebugPrint(S->red_mask);
            DebugPrint("\n");

            DebugPrint("  green_mask = ");
            DebugPrint(S->green_mask);
            DebugPrint("\n");

            DebugPrint("  blue_mask = ");
            DebugPrint(S->blue_mask);
            DebugPrint("\n");

            DebugPrint("  colormap_size = ");
            DebugPrint(S->colormap_size);
            DebugPrint("\n");

            DebugPrint("  bits_per_rgb = ");
            DebugPrint(S->bits_per_rgb);
            DebugPrint("\n");

        }
      }

      // Source File : src/unix_platform.h
      function void DebugPrint(os* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Window = ");
            DebugPrint(S->Window);
            DebugPrint("\n");

            DebugPrint("  Display = ");
            DebugPrint(S->Display);
            DebugPrint("\n");

            DebugPrint("  GlContext = ");
            DebugPrint(S->GlContext);
            DebugPrint("\n");

            DebugPrint("  ContinueRunning = ");
            DebugPrint(S->ContinueRunning);
            DebugPrint("\n");

        }
      }

      // Source File : src/unix_platform.h
      function void DebugPrint(native_file* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Handle = ");
            DebugPrint(S->Handle);
            DebugPrint("\n");

            DebugPrint("  Path = ");
            DebugPrint(S->Path);
            DebugPrint("\n");

        }
      }

      // Source File : src/engine/cpp/world_chunk.cpp
      function void DebugPrint(plane_computation* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Plane = ");
            DebugPrint(S->Plane);
            DebugPrint("\n");

            DebugPrint("  Complete = ");
            DebugPrint(S->Complete);
            DebugPrint("\n");

        }
      }

      // Source File : src/bonsai_stdlib/cpp/bitmap.cpp
      function void DebugPrint(bitmap_image_header* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  SizeOfImageHeader = ");
            DebugPrint(S->SizeOfImageHeader);
            DebugPrint("\n");

            DebugPrint("  WidthInPixels = ");
            DebugPrint(S->WidthInPixels);
            DebugPrint("\n");

            DebugPrint("  HeightInPixels = ");
            DebugPrint(S->HeightInPixels);
            DebugPrint("\n");

            DebugPrint("  ColorPlanes = ");
            DebugPrint(S->ColorPlanes);
            DebugPrint("\n");

            DebugPrint("  BPP = ");
            DebugPrint(S->BPP);
            DebugPrint("\n");

            DebugPrint("  CompressionType = ");
            DebugPrint(S->CompressionType);
            DebugPrint("\n");

            DebugPrint("  SizeInBytes = ");
            DebugPrint(S->SizeInBytes);
            DebugPrint("\n");

            DebugPrint("  xPixelsPerMeter = ");
            DebugPrint(S->xPixelsPerMeter);
            DebugPrint("\n");

            DebugPrint("  yPixelsPerMeter = ");
            DebugPrint(S->yPixelsPerMeter);
            DebugPrint("\n");

            DebugPrint("  ColorMapsUsed = ");
            DebugPrint(S->ColorMapsUsed);
            DebugPrint("\n");

            DebugPrint("  NumSignificantColors = ");
            DebugPrint(S->NumSignificantColors);
            DebugPrint("\n");

            DebugPrint("  RedMask = ");
            DebugPrint(S->RedMask);
            DebugPrint("\n");

            DebugPrint("  GreenMask = ");
            DebugPrint(S->GreenMask);
            DebugPrint("\n");

            DebugPrint("  BlueMask = ");
            DebugPrint(S->BlueMask);
            DebugPrint("\n");

            DebugPrint("  AlphaMask = ");
            DebugPrint(S->AlphaMask);
            DebugPrint("\n");

        }
      }

      // Source File : src/bonsai_stdlib/cpp/bitmap.cpp
      function void DebugPrint(bitmap_header* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Type = ");
            DebugPrint(S->Type);
            DebugPrint("\n");

            DebugPrint("  FileSizeInBytes = ");
            DebugPrint(S->FileSizeInBytes);
            DebugPrint("\n");

            DebugPrint("  Ignored = ");
            DebugPrint(S->Ignored);
            DebugPrint("\n");

            DebugPrint("  OffsetToPixelData = ");
            DebugPrint(S->OffsetToPixelData);
            DebugPrint("\n");

            DebugPrint("  Image = ");
            DebugPrint(S->Image);
            DebugPrint("\n");

        }
      }

      // Source File : src/bonsai_stdlib/cpp/bitmap.cpp
      function void DebugPrint(bitmap* S)
      {
        if (S)
        {
          DebugPrint("struct StructName\n");

          
            DebugPrint("  Dim = ");
            DebugPrint(S->Dim);
            DebugPrint("\n");

            DebugPrint("  Pixels = ");
            DebugPrint(S->Pixels);
            DebugPrint("\n");

        }
      }

