
      
        function void DebugPrint(cycle_range S, u32 Depth)
        {
          DebugPrint("cycle_range\n", Depth);
          
            DebugPrint("StartCycle = ", Depth);
            DebugPrint(S.StartCycle, Depth+1);
            DebugPrint("\n");

            DebugPrint("TotalCycles = ", Depth);
            DebugPrint(S.TotalCycles, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(cycle_range* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("cycle_range\n", Depth);
            
              DebugPrint("StartCycle = ", Depth);
              DebugPrint(S->StartCycle, Depth+1);
              DebugPrint("\n");

              DebugPrint("TotalCycles = ", Depth);
              DebugPrint(S->TotalCycles, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(memory_arena_stats S, u32 Depth)
        {
          DebugPrint("memory_arena_stats\n", Depth);
          
            DebugPrint("Allocations = ", Depth);
            DebugPrint(S.Allocations, Depth+1);
            DebugPrint("\n");

            DebugPrint("Pushes = ", Depth);
            DebugPrint(S.Pushes, Depth+1);
            DebugPrint("\n");

            DebugPrint("TotalAllocated = ", Depth);
            DebugPrint(S.TotalAllocated, Depth+1);
            DebugPrint("\n");

            DebugPrint("Remaining = ", Depth);
            DebugPrint(S.Remaining, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(memory_arena_stats* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("memory_arena_stats\n", Depth);
            
              DebugPrint("Allocations = ", Depth);
              DebugPrint(S->Allocations, Depth+1);
              DebugPrint("\n");

              DebugPrint("Pushes = ", Depth);
              DebugPrint(S->Pushes, Depth+1);
              DebugPrint("\n");

              DebugPrint("TotalAllocated = ", Depth);
              DebugPrint(S->TotalAllocated, Depth+1);
              DebugPrint("\n");

              DebugPrint("Remaining = ", Depth);
              DebugPrint(S->Remaining, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(min_max_avg_dt S, u32 Depth)
        {
          DebugPrint("min_max_avg_dt\n", Depth);
          
            DebugPrint("Min = ", Depth);
            DebugPrint(S.Min, Depth+1);
            DebugPrint("\n");

            DebugPrint("Max = ", Depth);
            DebugPrint(S.Max, Depth+1);
            DebugPrint("\n");

            DebugPrint("Avg = ", Depth);
            DebugPrint(S.Avg, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(min_max_avg_dt* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("min_max_avg_dt\n", Depth);
            
              DebugPrint("Min = ", Depth);
              DebugPrint(S->Min, Depth+1);
              DebugPrint("\n");

              DebugPrint("Max = ", Depth);
              DebugPrint(S->Max, Depth+1);
              DebugPrint("\n");

              DebugPrint("Avg = ", Depth);
              DebugPrint(S->Avg, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(debug_ui_render_group S, u32 Depth)
        {
          DebugPrint("debug_ui_render_group\n", Depth);
          
            DebugPrint("GameGeo = ", Depth);
            DebugPrint(S.GameGeo, Depth+1);
            DebugPrint("\n");

            DebugPrint("GameGeoShader = ", Depth);
            DebugPrint(S.GameGeoShader, Depth+1);
            DebugPrint("\n");

            DebugPrint("TextGroup = ", Depth);
            DebugPrint(S.TextGroup, Depth+1);
            DebugPrint("\n");

            DebugPrint("InteractionStackTop = ", Depth);
            DebugPrint(S.InteractionStackTop, Depth+1);
            DebugPrint("\n");

            DebugPrint("MouseP = ", Depth);
            DebugPrint(S.MouseP, Depth+1);
            DebugPrint("\n");

            DebugPrint("MouseDP = ", Depth);
            DebugPrint(S.MouseDP, Depth+1);
            DebugPrint("\n");

            DebugPrint("ScreenDim = ", Depth);
            DebugPrint(S.ScreenDim, Depth+1);
            DebugPrint("\n");

            DebugPrint("Input = ", Depth);
            DebugPrint(S.Input, Depth+1);
            DebugPrint("\n");

            DebugPrint("HighestWindow = ", Depth);
            DebugPrint(S.HighestWindow, Depth+1);
            DebugPrint("\n");

            DebugPrint("HoverInteractionId = ", Depth);
            DebugPrint(S.HoverInteractionId, Depth+1);
            DebugPrint("\n");

            DebugPrint("ClickedInteractionId = ", Depth);
            DebugPrint(S.ClickedInteractionId, Depth+1);
            DebugPrint("\n");

            DebugPrint("PressedInteractionId = ", Depth);
            DebugPrint(S.PressedInteractionId, Depth+1);
            DebugPrint("\n");

            DebugPrint("Geo = ", Depth);
            DebugPrint(S.Geo, Depth+1);
            DebugPrint("\n");

            DebugPrint("CommandBuffer = ", Depth);
            DebugPrint(S.CommandBuffer, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(debug_ui_render_group* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("debug_ui_render_group\n", Depth);
            
              DebugPrint("GameGeo = ", Depth);
              DebugPrint(S->GameGeo, Depth+1);
              DebugPrint("\n");

              DebugPrint("GameGeoShader = ", Depth);
              DebugPrint(S->GameGeoShader, Depth+1);
              DebugPrint("\n");

              DebugPrint("TextGroup = ", Depth);
              DebugPrint(S->TextGroup, Depth+1);
              DebugPrint("\n");

              DebugPrint("InteractionStackTop = ", Depth);
              DebugPrint(S->InteractionStackTop, Depth+1);
              DebugPrint("\n");

              DebugPrint("MouseP = ", Depth);
              DebugPrint(S->MouseP, Depth+1);
              DebugPrint("\n");

              DebugPrint("MouseDP = ", Depth);
              DebugPrint(S->MouseDP, Depth+1);
              DebugPrint("\n");

              DebugPrint("ScreenDim = ", Depth);
              DebugPrint(S->ScreenDim, Depth+1);
              DebugPrint("\n");

              DebugPrint("Input = ", Depth);
              DebugPrint(S->Input, Depth+1);
              DebugPrint("\n");

              DebugPrint("HighestWindow = ", Depth);
              DebugPrint(S->HighestWindow, Depth+1);
              DebugPrint("\n");

              DebugPrint("HoverInteractionId = ", Depth);
              DebugPrint(S->HoverInteractionId, Depth+1);
              DebugPrint("\n");

              DebugPrint("ClickedInteractionId = ", Depth);
              DebugPrint(S->ClickedInteractionId, Depth+1);
              DebugPrint("\n");

              DebugPrint("PressedInteractionId = ", Depth);
              DebugPrint(S->PressedInteractionId, Depth+1);
              DebugPrint("\n");

              DebugPrint("Geo = ", Depth);
              DebugPrint(S->Geo, Depth+1);
              DebugPrint("\n");

              DebugPrint("CommandBuffer = ", Depth);
              DebugPrint(S->CommandBuffer, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(debug_profile_scope S, u32 Depth)
        {
          DebugPrint("debug_profile_scope\n", Depth);
          
            DebugPrint("CycleCount = ", Depth);
            DebugPrint(S.CycleCount, Depth+1);
            DebugPrint("\n");

            DebugPrint("StartingCycle = ", Depth);
            DebugPrint(S.StartingCycle, Depth+1);
            DebugPrint("\n");

            DebugPrint("Name = ", Depth);
            DebugPrint(S.Name, Depth+1);
            DebugPrint("\n");

            DebugPrint("Expanded = ", Depth);
            DebugPrint(S.Expanded, Depth+1);
            DebugPrint("\n");

            DebugPrint("Sibling = ", Depth);
            DebugPrint(S.Sibling, Depth+1);
            DebugPrint("\n");

            DebugPrint("Child = ", Depth);
            DebugPrint(S.Child, Depth+1);
            DebugPrint("\n");

            DebugPrint("Parent = ", Depth);
            DebugPrint(S.Parent, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(debug_profile_scope* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("debug_profile_scope\n", Depth);
            
              DebugPrint("CycleCount = ", Depth);
              DebugPrint(S->CycleCount, Depth+1);
              DebugPrint("\n");

              DebugPrint("StartingCycle = ", Depth);
              DebugPrint(S->StartingCycle, Depth+1);
              DebugPrint("\n");

              DebugPrint("Name = ", Depth);
              DebugPrint(S->Name, Depth+1);
              DebugPrint("\n");

              DebugPrint("Expanded = ", Depth);
              DebugPrint(S->Expanded, Depth+1);
              DebugPrint("\n");

              DebugPrint("Sibling = ", Depth);
              DebugPrint(S->Sibling, Depth+1);
              DebugPrint("\n");

              DebugPrint("Child = ", Depth);
              DebugPrint(S->Child, Depth+1);
              DebugPrint("\n");

              DebugPrint("Parent = ", Depth);
              DebugPrint(S->Parent, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(unique_debug_profile_scope S, u32 Depth)
        {
          DebugPrint("unique_debug_profile_scope\n", Depth);
          
            DebugPrint("Name = ", Depth);
            DebugPrint(S.Name, Depth+1);
            DebugPrint("\n");

            DebugPrint("CallCount = ", Depth);
            DebugPrint(S.CallCount, Depth+1);
            DebugPrint("\n");

            DebugPrint("TotalCycles = ", Depth);
            DebugPrint(S.TotalCycles, Depth+1);
            DebugPrint("\n");

            DebugPrint("MinCycles = ", Depth);
            DebugPrint(S.MinCycles, Depth+1);
            DebugPrint("\n");

            DebugPrint("MaxCycles = ", Depth);
            DebugPrint(S.MaxCycles, Depth+1);
            DebugPrint("\n");

            DebugPrint("Scope = ", Depth);
            DebugPrint(S.Scope, Depth+1);
            DebugPrint("\n");

            DebugPrint("NextUnique = ", Depth);
            DebugPrint(S.NextUnique, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(unique_debug_profile_scope* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("unique_debug_profile_scope\n", Depth);
            
              DebugPrint("Name = ", Depth);
              DebugPrint(S->Name, Depth+1);
              DebugPrint("\n");

              DebugPrint("CallCount = ", Depth);
              DebugPrint(S->CallCount, Depth+1);
              DebugPrint("\n");

              DebugPrint("TotalCycles = ", Depth);
              DebugPrint(S->TotalCycles, Depth+1);
              DebugPrint("\n");

              DebugPrint("MinCycles = ", Depth);
              DebugPrint(S->MinCycles, Depth+1);
              DebugPrint("\n");

              DebugPrint("MaxCycles = ", Depth);
              DebugPrint(S->MaxCycles, Depth+1);
              DebugPrint("\n");

              DebugPrint("Scope = ", Depth);
              DebugPrint(S->Scope, Depth+1);
              DebugPrint("\n");

              DebugPrint("NextUnique = ", Depth);
              DebugPrint(S->NextUnique, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(debug_scope_tree S, u32 Depth)
        {
          DebugPrint("debug_scope_tree\n", Depth);
          
            DebugPrint("Root = ", Depth);
            DebugPrint(S.Root, Depth+1);
            DebugPrint("\n");

            DebugPrint("WriteScope = ", Depth);
            DebugPrint(S.WriteScope, Depth+1);
            DebugPrint("\n");

            DebugPrint("ParentOfNextScope = ", Depth);
            DebugPrint(S.ParentOfNextScope, Depth+1);
            DebugPrint("\n");

            DebugPrint("FrameRecorded = ", Depth);
            DebugPrint(S.FrameRecorded, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(debug_scope_tree* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("debug_scope_tree\n", Depth);
            
              DebugPrint("Root = ", Depth);
              DebugPrint(S->Root, Depth+1);
              DebugPrint("\n");

              DebugPrint("WriteScope = ", Depth);
              DebugPrint(S->WriteScope, Depth+1);
              DebugPrint("\n");

              DebugPrint("ParentOfNextScope = ", Depth);
              DebugPrint(S->ParentOfNextScope, Depth+1);
              DebugPrint("\n");

              DebugPrint("FrameRecorded = ", Depth);
              DebugPrint(S->FrameRecorded, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(debug_thread_state S, u32 Depth)
        {
          DebugPrint("debug_thread_state\n", Depth);
          
            DebugPrint("Memory = ", Depth);
            DebugPrint(S.Memory, Depth+1);
            DebugPrint("\n");

            DebugPrint("MetaTable = ", Depth);
            DebugPrint(S.MetaTable, Depth+1);
            DebugPrint("\n");

            DebugPrint("ScopeTrees = ", Depth);
            DebugPrint(S.ScopeTrees, Depth+1);
            DebugPrint("\n");

            DebugPrint("FirstFreeScope = ", Depth);
            DebugPrint(S.FirstFreeScope, Depth+1);
            DebugPrint("\n");

            DebugPrint("MutexOps = ", Depth);
            DebugPrint(S.MutexOps, Depth+1);
            DebugPrint("\n");

            DebugPrint("WriteIndex = ", Depth);
            DebugPrint(S.WriteIndex, Depth+1);
            DebugPrint("\n");

            DebugPrint("Pad = ", Depth);
            DebugPrint(S.Pad, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(debug_thread_state* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("debug_thread_state\n", Depth);
            
              DebugPrint("Memory = ", Depth);
              DebugPrint(S->Memory, Depth+1);
              DebugPrint("\n");

              DebugPrint("MetaTable = ", Depth);
              DebugPrint(S->MetaTable, Depth+1);
              DebugPrint("\n");

              DebugPrint("ScopeTrees = ", Depth);
              DebugPrint(S->ScopeTrees, Depth+1);
              DebugPrint("\n");

              DebugPrint("FirstFreeScope = ", Depth);
              DebugPrint(S->FirstFreeScope, Depth+1);
              DebugPrint("\n");

              DebugPrint("MutexOps = ", Depth);
              DebugPrint(S->MutexOps, Depth+1);
              DebugPrint("\n");

              DebugPrint("WriteIndex = ", Depth);
              DebugPrint(S->WriteIndex, Depth+1);
              DebugPrint("\n");

              DebugPrint("Pad = ", Depth);
              DebugPrint(S->Pad, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(registered_memory_arena S, u32 Depth)
        {
          DebugPrint("registered_memory_arena\n", Depth);
          
            DebugPrint("Arena = ", Depth);
            DebugPrint(S.Arena, Depth+1);
            DebugPrint("\n");

            DebugPrint("Name = ", Depth);
            DebugPrint(S.Name, Depth+1);
            DebugPrint("\n");

            DebugPrint("Expanded = ", Depth);
            DebugPrint(S.Expanded, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(registered_memory_arena* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("registered_memory_arena\n", Depth);
            
              DebugPrint("Arena = ", Depth);
              DebugPrint(S->Arena, Depth+1);
              DebugPrint("\n");

              DebugPrint("Name = ", Depth);
              DebugPrint(S->Name, Depth+1);
              DebugPrint("\n");

              DebugPrint("Expanded = ", Depth);
              DebugPrint(S->Expanded, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(selected_memory_arena S, u32 Depth)
        {
          DebugPrint("selected_memory_arena\n", Depth);
          
            DebugPrint("ArenaHash = ", Depth);
            DebugPrint(S.ArenaHash, Depth+1);
            DebugPrint("\n");

            DebugPrint("HeadArenaHash = ", Depth);
            DebugPrint(S.HeadArenaHash, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(selected_memory_arena* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("selected_memory_arena\n", Depth);
            
              DebugPrint("ArenaHash = ", Depth);
              DebugPrint(S->ArenaHash, Depth+1);
              DebugPrint("\n");

              DebugPrint("HeadArenaHash = ", Depth);
              DebugPrint(S->HeadArenaHash, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(selected_arenas S, u32 Depth)
        {
          DebugPrint("selected_arenas\n", Depth);
          
            DebugPrint("Count = ", Depth);
            DebugPrint(S.Count, Depth+1);
            DebugPrint("\n");

            DebugPrint("Arenas = ", Depth);
            DebugPrint(S.Arenas, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(selected_arenas* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("selected_arenas\n", Depth);
            
              DebugPrint("Count = ", Depth);
              DebugPrint(S->Count, Depth+1);
              DebugPrint("\n");

              DebugPrint("Arenas = ", Depth);
              DebugPrint(S->Arenas, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(frame_stats S, u32 Depth)
        {
          DebugPrint("frame_stats\n", Depth);
          
            DebugPrint("TotalCycles = ", Depth);
            DebugPrint(S.TotalCycles, Depth+1);
            DebugPrint("\n");

            DebugPrint("StartingCycle = ", Depth);
            DebugPrint(S.StartingCycle, Depth+1);
            DebugPrint("\n");

            DebugPrint("FrameMs = ", Depth);
            DebugPrint(S.FrameMs, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(frame_stats* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("frame_stats\n", Depth);
            
              DebugPrint("TotalCycles = ", Depth);
              DebugPrint(S->TotalCycles, Depth+1);
              DebugPrint("\n");

              DebugPrint("StartingCycle = ", Depth);
              DebugPrint(S->StartingCycle, Depth+1);
              DebugPrint("\n");

              DebugPrint("FrameMs = ", Depth);
              DebugPrint(S->FrameMs, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(called_function S, u32 Depth)
        {
          DebugPrint("called_function\n", Depth);
          
            DebugPrint("Name = ", Depth);
            DebugPrint(S.Name, Depth+1);
            DebugPrint("\n");

            DebugPrint("CallCount = ", Depth);
            DebugPrint(S.CallCount, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(called_function* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("called_function\n", Depth);
            
              DebugPrint("Name = ", Depth);
              DebugPrint(S->Name, Depth+1);
              DebugPrint("\n");

              DebugPrint("CallCount = ", Depth);
              DebugPrint(S->CallCount, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(debug_draw_call S, u32 Depth)
        {
          DebugPrint("debug_draw_call\n", Depth);
          
            DebugPrint("Caller = ", Depth);
            DebugPrint(S.Caller, Depth+1);
            DebugPrint("\n");

            DebugPrint("N = ", Depth);
            DebugPrint(S.N, Depth+1);
            DebugPrint("\n");

            DebugPrint("Calls = ", Depth);
            DebugPrint(S.Calls, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(debug_draw_call* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("debug_draw_call\n", Depth);
            
              DebugPrint("Caller = ", Depth);
              DebugPrint(S->Caller, Depth+1);
              DebugPrint("\n");

              DebugPrint("N = ", Depth);
              DebugPrint(S->N, Depth+1);
              DebugPrint("\n");

              DebugPrint("Calls = ", Depth);
              DebugPrint(S->Calls, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(debug_timed_function S, u32 Depth)
        {
          DebugPrint("debug_timed_function\n", Depth);
          
            DebugPrint("Scope = ", Depth);
            DebugPrint(S.Scope, Depth+1);
            DebugPrint("\n");

            DebugPrint("Tree = ", Depth);
            DebugPrint(S.Tree, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(debug_timed_function* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("debug_timed_function\n", Depth);
            
              DebugPrint("Scope = ", Depth);
              DebugPrint(S->Scope, Depth+1);
              DebugPrint("\n");

              DebugPrint("Tree = ", Depth);
              DebugPrint(S->Tree, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(window_layout S, u32 Depth)
        {
          DebugPrint("window_layout\n", Depth);
          
            DebugPrint("Title = ", Depth);
            DebugPrint(S.Title, Depth+1);
            DebugPrint("\n");

            DebugPrint("Basis = ", Depth);
            DebugPrint(S.Basis, Depth+1);
            DebugPrint("\n");

            DebugPrint("MaxClip = ", Depth);
            DebugPrint(S.MaxClip, Depth+1);
            DebugPrint("\n");

            DebugPrint("InteractionStackIndex = ", Depth);
            DebugPrint(S.InteractionStackIndex, Depth+1);
            DebugPrint("\n");

            DebugPrint("zBackground = ", Depth);
            DebugPrint(S.zBackground, Depth+1);
            DebugPrint("\n");

            DebugPrint("zText = ", Depth);
            DebugPrint(S.zText, Depth+1);
            DebugPrint("\n");

            DebugPrint("zBorder = ", Depth);
            DebugPrint(S.zBorder, Depth+1);
            DebugPrint("\n");

            DebugPrint("zTitleBar = ", Depth);
            DebugPrint(S.zTitleBar, Depth+1);
            DebugPrint("\n");

            DebugPrint("NextHotWindow = ", Depth);
            DebugPrint(S.NextHotWindow, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(window_layout* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("window_layout\n", Depth);
            
              DebugPrint("Title = ", Depth);
              DebugPrint(S->Title, Depth+1);
              DebugPrint("\n");

              DebugPrint("Basis = ", Depth);
              DebugPrint(S->Basis, Depth+1);
              DebugPrint("\n");

              DebugPrint("MaxClip = ", Depth);
              DebugPrint(S->MaxClip, Depth+1);
              DebugPrint("\n");

              DebugPrint("InteractionStackIndex = ", Depth);
              DebugPrint(S->InteractionStackIndex, Depth+1);
              DebugPrint("\n");

              DebugPrint("zBackground = ", Depth);
              DebugPrint(S->zBackground, Depth+1);
              DebugPrint("\n");

              DebugPrint("zText = ", Depth);
              DebugPrint(S->zText, Depth+1);
              DebugPrint("\n");

              DebugPrint("zBorder = ", Depth);
              DebugPrint(S->zBorder, Depth+1);
              DebugPrint("\n");

              DebugPrint("zTitleBar = ", Depth);
              DebugPrint(S->zTitleBar, Depth+1);
              DebugPrint("\n");

              DebugPrint("NextHotWindow = ", Depth);
              DebugPrint(S->NextHotWindow, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(font S, u32 Depth)
        {
          DebugPrint("font\n", Depth);
          
            DebugPrint("Size = ", Depth);
            DebugPrint(S.Size, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(font* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("font\n", Depth);
            
              DebugPrint("Size = ", Depth);
              DebugPrint(S->Size, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(ui_element_reference S, u32 Depth)
        {
          DebugPrint("ui_element_reference\n", Depth);
          
            DebugPrint("Index = ", Depth);
            DebugPrint(S.Index, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(ui_element_reference* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("ui_element_reference\n", Depth);
            
              DebugPrint("Index = ", Depth);
              DebugPrint(S->Index, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(ui_style S, u32 Depth)
        {
          DebugPrint("ui_style\n", Depth);
          
            DebugPrint("Color = ", Depth);
            DebugPrint(S.Color, Depth+1);
            DebugPrint("\n");

            DebugPrint("HoverColor = ", Depth);
            DebugPrint(S.HoverColor, Depth+1);
            DebugPrint("\n");

            DebugPrint("PressedColor = ", Depth);
            DebugPrint(S.PressedColor, Depth+1);
            DebugPrint("\n");

            DebugPrint("ClickedColor = ", Depth);
            DebugPrint(S.ClickedColor, Depth+1);
            DebugPrint("\n");

            DebugPrint("ActiveColor = ", Depth);
            DebugPrint(S.ActiveColor, Depth+1);
            DebugPrint("\n");

            DebugPrint("Font = ", Depth);
            DebugPrint(S.Font, Depth+1);
            DebugPrint("\n");

            DebugPrint("IsActive = ", Depth);
            DebugPrint(S.IsActive, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(ui_style* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("ui_style\n", Depth);
            
              DebugPrint("Color = ", Depth);
              DebugPrint(S->Color, Depth+1);
              DebugPrint("\n");

              DebugPrint("HoverColor = ", Depth);
              DebugPrint(S->HoverColor, Depth+1);
              DebugPrint("\n");

              DebugPrint("PressedColor = ", Depth);
              DebugPrint(S->PressedColor, Depth+1);
              DebugPrint("\n");

              DebugPrint("ClickedColor = ", Depth);
              DebugPrint(S->ClickedColor, Depth+1);
              DebugPrint("\n");

              DebugPrint("ActiveColor = ", Depth);
              DebugPrint(S->ActiveColor, Depth+1);
              DebugPrint("\n");

              DebugPrint("Font = ", Depth);
              DebugPrint(S->Font, Depth+1);
              DebugPrint("\n");

              DebugPrint("IsActive = ", Depth);
              DebugPrint(S->IsActive, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(layout S, u32 Depth)
        {
          DebugPrint("layout\n", Depth);
          
            DebugPrint("Basis = ", Depth);
            DebugPrint(S.Basis, Depth+1);
            DebugPrint("\n");

            DebugPrint("At = ", Depth);
            DebugPrint(S.At, Depth+1);
            DebugPrint("\n");

            DebugPrint("DrawBounds = ", Depth);
            DebugPrint(S.DrawBounds, Depth+1);
            DebugPrint("\n");

            DebugPrint("Padding = ", Depth);
            DebugPrint(S.Padding, Depth+1);
            DebugPrint("\n");

            DebugPrint("Prev = ", Depth);
            DebugPrint(S.Prev, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(layout* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("layout\n", Depth);
            
              DebugPrint("Basis = ", Depth);
              DebugPrint(S->Basis, Depth+1);
              DebugPrint("\n");

              DebugPrint("At = ", Depth);
              DebugPrint(S->At, Depth+1);
              DebugPrint("\n");

              DebugPrint("DrawBounds = ", Depth);
              DebugPrint(S->DrawBounds, Depth+1);
              DebugPrint("\n");

              DebugPrint("Padding = ", Depth);
              DebugPrint(S->Padding, Depth+1);
              DebugPrint("\n");

              DebugPrint("Prev = ", Depth);
              DebugPrint(S->Prev, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(ui_render_command_border S, u32 Depth)
        {
          DebugPrint("ui_render_command_border\n", Depth);
          
            DebugPrint("Window = ", Depth);
            DebugPrint(S.Window, Depth+1);
            DebugPrint("\n");

            DebugPrint("Bounds = ", Depth);
            DebugPrint(S.Bounds, Depth+1);
            DebugPrint("\n");

            DebugPrint("Color = ", Depth);
            DebugPrint(S.Color, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(ui_render_command_border* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("ui_render_command_border\n", Depth);
            
              DebugPrint("Window = ", Depth);
              DebugPrint(S->Window, Depth+1);
              DebugPrint("\n");

              DebugPrint("Bounds = ", Depth);
              DebugPrint(S->Bounds, Depth+1);
              DebugPrint("\n");

              DebugPrint("Color = ", Depth);
              DebugPrint(S->Color, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(ui_render_command_window_start S, u32 Depth)
        {
          DebugPrint("ui_render_command_window_start\n", Depth);
          
            DebugPrint("Layout = ", Depth);
            DebugPrint(S.Layout, Depth+1);
            DebugPrint("\n");

            DebugPrint("Window = ", Depth);
            DebugPrint(S.Window, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(ui_render_command_window_start* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("ui_render_command_window_start\n", Depth);
            
              DebugPrint("Layout = ", Depth);
              DebugPrint(S->Layout, Depth+1);
              DebugPrint("\n");

              DebugPrint("Window = ", Depth);
              DebugPrint(S->Window, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(ui_render_command_window_end S, u32 Depth)
        {
          DebugPrint("ui_render_command_window_end\n", Depth);
          
            DebugPrint("Window = ", Depth);
            DebugPrint(S.Window, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(ui_render_command_window_end* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("ui_render_command_window_end\n", Depth);
            
              DebugPrint("Window = ", Depth);
              DebugPrint(S->Window, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(ui_render_command_column_start S, u32 Depth)
        {
          DebugPrint("ui_render_command_column_start\n", Depth);
          
            DebugPrint("Layout = ", Depth);
            DebugPrint(S.Layout, Depth+1);
            DebugPrint("\n");

            DebugPrint("Style = ", Depth);
            DebugPrint(S.Style, Depth+1);
            DebugPrint("\n");

            DebugPrint("Width = ", Depth);
            DebugPrint(S.Width, Depth+1);
            DebugPrint("\n");

            DebugPrint("MaxWidth = ", Depth);
            DebugPrint(S.MaxWidth, Depth+1);
            DebugPrint("\n");

            DebugPrint("Params = ", Depth);
            DebugPrint(S.Params, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(ui_render_command_column_start* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("ui_render_command_column_start\n", Depth);
            
              DebugPrint("Layout = ", Depth);
              DebugPrint(S->Layout, Depth+1);
              DebugPrint("\n");

              DebugPrint("Style = ", Depth);
              DebugPrint(S->Style, Depth+1);
              DebugPrint("\n");

              DebugPrint("Width = ", Depth);
              DebugPrint(S->Width, Depth+1);
              DebugPrint("\n");

              DebugPrint("MaxWidth = ", Depth);
              DebugPrint(S->MaxWidth, Depth+1);
              DebugPrint("\n");

              DebugPrint("Params = ", Depth);
              DebugPrint(S->Params, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(ui_render_command_text S, u32 Depth)
        {
          DebugPrint("ui_render_command_text\n", Depth);
          
            DebugPrint("Layout = ", Depth);
            DebugPrint(S.Layout, Depth+1);
            DebugPrint("\n");

            DebugPrint("Style = ", Depth);
            DebugPrint(S.Style, Depth+1);
            DebugPrint("\n");

            DebugPrint("String = ", Depth);
            DebugPrint(S.String, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(ui_render_command_text* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("ui_render_command_text\n", Depth);
            
              DebugPrint("Layout = ", Depth);
              DebugPrint(S->Layout, Depth+1);
              DebugPrint("\n");

              DebugPrint("Style = ", Depth);
              DebugPrint(S->Style, Depth+1);
              DebugPrint("\n");

              DebugPrint("String = ", Depth);
              DebugPrint(S->String, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(ui_render_command_text_at S, u32 Depth)
        {
          DebugPrint("ui_render_command_text_at\n", Depth);
          
            DebugPrint("Text = ", Depth);
            DebugPrint(S.Text, Depth+1);
            DebugPrint("\n");

            DebugPrint("At = ", Depth);
            DebugPrint(S.At, Depth+1);
            DebugPrint("\n");

            DebugPrint("MaxClip = ", Depth);
            DebugPrint(S.MaxClip, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(ui_render_command_text_at* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("ui_render_command_text_at\n", Depth);
            
              DebugPrint("Text = ", Depth);
              DebugPrint(S->Text, Depth+1);
              DebugPrint("\n");

              DebugPrint("At = ", Depth);
              DebugPrint(S->At, Depth+1);
              DebugPrint("\n");

              DebugPrint("MaxClip = ", Depth);
              DebugPrint(S->MaxClip, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(ui_render_command_untextured_quad S, u32 Depth)
        {
          DebugPrint("ui_render_command_untextured_quad\n", Depth);
          
            DebugPrint("Layout = ", Depth);
            DebugPrint(S.Layout, Depth+1);
            DebugPrint("\n");

            DebugPrint("Style = ", Depth);
            DebugPrint(S.Style, Depth+1);
            DebugPrint("\n");

            DebugPrint("QuadDim = ", Depth);
            DebugPrint(S.QuadDim, Depth+1);
            DebugPrint("\n");

            DebugPrint("zDepth = ", Depth);
            DebugPrint(S.zDepth, Depth+1);
            DebugPrint("\n");

            DebugPrint("Params = ", Depth);
            DebugPrint(S.Params, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(ui_render_command_untextured_quad* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("ui_render_command_untextured_quad\n", Depth);
            
              DebugPrint("Layout = ", Depth);
              DebugPrint(S->Layout, Depth+1);
              DebugPrint("\n");

              DebugPrint("Style = ", Depth);
              DebugPrint(S->Style, Depth+1);
              DebugPrint("\n");

              DebugPrint("QuadDim = ", Depth);
              DebugPrint(S->QuadDim, Depth+1);
              DebugPrint("\n");

              DebugPrint("zDepth = ", Depth);
              DebugPrint(S->zDepth, Depth+1);
              DebugPrint("\n");

              DebugPrint("Params = ", Depth);
              DebugPrint(S->Params, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(ui_render_command_untextured_quad_at S, u32 Depth)
        {
          DebugPrint("ui_render_command_untextured_quad_at\n", Depth);
          
            DebugPrint("Layout = ", Depth);
            DebugPrint(S.Layout, Depth+1);
            DebugPrint("\n");

            DebugPrint("Style = ", Depth);
            DebugPrint(S.Style, Depth+1);
            DebugPrint("\n");

            DebugPrint("QuadDim = ", Depth);
            DebugPrint(S.QuadDim, Depth+1);
            DebugPrint("\n");

            DebugPrint("zDepth = ", Depth);
            DebugPrint(S.zDepth, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(ui_render_command_untextured_quad_at* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("ui_render_command_untextured_quad_at\n", Depth);
            
              DebugPrint("Layout = ", Depth);
              DebugPrint(S->Layout, Depth+1);
              DebugPrint("\n");

              DebugPrint("Style = ", Depth);
              DebugPrint(S->Style, Depth+1);
              DebugPrint("\n");

              DebugPrint("QuadDim = ", Depth);
              DebugPrint(S->QuadDim, Depth+1);
              DebugPrint("\n");

              DebugPrint("zDepth = ", Depth);
              DebugPrint(S->zDepth, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(ui_render_command_textured_quad S, u32 Depth)
        {
          DebugPrint("ui_render_command_textured_quad\n", Depth);
          
            DebugPrint("Layout = ", Depth);
            DebugPrint(S.Layout, Depth+1);
            DebugPrint("\n");

            DebugPrint("TextureSlice = ", Depth);
            DebugPrint(S.TextureSlice, Depth+1);
            DebugPrint("\n");

            DebugPrint("zDepth = ", Depth);
            DebugPrint(S.zDepth, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(ui_render_command_textured_quad* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("ui_render_command_textured_quad\n", Depth);
            
              DebugPrint("Layout = ", Depth);
              DebugPrint(S->Layout, Depth+1);
              DebugPrint("\n");

              DebugPrint("TextureSlice = ", Depth);
              DebugPrint(S->TextureSlice, Depth+1);
              DebugPrint("\n");

              DebugPrint("zDepth = ", Depth);
              DebugPrint(S->zDepth, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(ui_render_command_button_start S, u32 Depth)
        {
          DebugPrint("ui_render_command_button_start\n", Depth);
          
            DebugPrint("ID = ", Depth);
            DebugPrint(S.ID, Depth+1);
            DebugPrint("\n");

            DebugPrint("Style = ", Depth);
            DebugPrint(S.Style, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(ui_render_command_button_start* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("ui_render_command_button_start\n", Depth);
            
              DebugPrint("ID = ", Depth);
              DebugPrint(S->ID, Depth+1);
              DebugPrint("\n");

              DebugPrint("Style = ", Depth);
              DebugPrint(S->Style, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(ui_render_command_button_end S, u32 Depth)
        {
          DebugPrint("ui_render_command_button_end\n", Depth);
          
            DebugPrint("Params = ", Depth);
            DebugPrint(S.Params, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(ui_render_command_button_end* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("ui_render_command_button_end\n", Depth);
            
              DebugPrint("Params = ", Depth);
              DebugPrint(S->Params, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(ui_render_command_table_start S, u32 Depth)
        {
          DebugPrint("ui_render_command_table_start\n", Depth);
          
            DebugPrint("Layout = ", Depth);
            DebugPrint(S.Layout, Depth+1);
            DebugPrint("\n");

            DebugPrint("Style = ", Depth);
            DebugPrint(S.Style, Depth+1);
            DebugPrint("\n");

            DebugPrint("Position = ", Depth);
            DebugPrint(S.Position, Depth+1);
            DebugPrint("\n");

            DebugPrint("RelativeTo = ", Depth);
            DebugPrint(S.RelativeTo, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(ui_render_command_table_start* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("ui_render_command_table_start\n", Depth);
            
              DebugPrint("Layout = ", Depth);
              DebugPrint(S->Layout, Depth+1);
              DebugPrint("\n");

              DebugPrint("Style = ", Depth);
              DebugPrint(S->Style, Depth+1);
              DebugPrint("\n");

              DebugPrint("Position = ", Depth);
              DebugPrint(S->Position, Depth+1);
              DebugPrint("\n");

              DebugPrint("RelativeTo = ", Depth);
              DebugPrint(S->RelativeTo, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(ui_render_command_buffer S, u32 Depth)
        {
          DebugPrint("ui_render_command_buffer\n", Depth);
          
            DebugPrint("CommandCount = ", Depth);
            DebugPrint(S.CommandCount, Depth+1);
            DebugPrint("\n");

            DebugPrint("Commands = ", Depth);
            DebugPrint(S.Commands, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(ui_render_command_buffer* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("ui_render_command_buffer\n", Depth);
            
              DebugPrint("CommandCount = ", Depth);
              DebugPrint(S->CommandCount, Depth+1);
              DebugPrint("\n");

              DebugPrint("Commands = ", Depth);
              DebugPrint(S->Commands, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(clip_result S, u32 Depth)
        {
          DebugPrint("clip_result\n", Depth);
          
            DebugPrint("ClipStatus = ", Depth);
            DebugPrint(S.ClipStatus, Depth+1);
            DebugPrint("\n");

            DebugPrint("MaxClip = ", Depth);
            DebugPrint(S.MaxClip, Depth+1);
            DebugPrint("\n");

            DebugPrint("PartialClip = ", Depth);
            DebugPrint(S.PartialClip, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(clip_result* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("clip_result\n", Depth);
            
              DebugPrint("ClipStatus = ", Depth);
              DebugPrint(S->ClipStatus, Depth+1);
              DebugPrint("\n");

              DebugPrint("MaxClip = ", Depth);
              DebugPrint(S->MaxClip, Depth+1);
              DebugPrint("\n");

              DebugPrint("PartialClip = ", Depth);
              DebugPrint(S->PartialClip, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(sort_key S, u32 Depth)
        {
          DebugPrint("sort_key\n", Depth);
          
            DebugPrint("Value = ", Depth);
            DebugPrint(S.Value, Depth+1);
            DebugPrint("\n");

            DebugPrint("Index = ", Depth);
            DebugPrint(S.Index, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(sort_key* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("sort_key\n", Depth);
            
              DebugPrint("Value = ", Depth);
              DebugPrint(S->Value, Depth+1);
              DebugPrint("\n");

              DebugPrint("Index = ", Depth);
              DebugPrint(S->Index, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(window_sort_params S, u32 Depth)
        {
          DebugPrint("window_sort_params\n", Depth);
          
            DebugPrint("Count = ", Depth);
            DebugPrint(S.Count, Depth+1);
            DebugPrint("\n");

            DebugPrint("LowestInteractionStackIndex = ", Depth);
            DebugPrint(S.LowestInteractionStackIndex, Depth+1);
            DebugPrint("\n");

            DebugPrint("SortKeys = ", Depth);
            DebugPrint(S.SortKeys, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(window_sort_params* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("window_sort_params\n", Depth);
            
              DebugPrint("Count = ", Depth);
              DebugPrint(S->Count, Depth+1);
              DebugPrint("\n");

              DebugPrint("LowestInteractionStackIndex = ", Depth);
              DebugPrint(S->LowestInteractionStackIndex, Depth+1);
              DebugPrint("\n");

              DebugPrint("SortKeys = ", Depth);
              DebugPrint(S->SortKeys, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(find_button_start_result S, u32 Depth)
        {
          DebugPrint("find_button_start_result\n", Depth);
          
            DebugPrint("Index = ", Depth);
            DebugPrint(S.Index, Depth+1);
            DebugPrint("\n");

            DebugPrint("Command = ", Depth);
            DebugPrint(S.Command, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(find_button_start_result* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("find_button_start_result\n", Depth);
            
              DebugPrint("Index = ", Depth);
              DebugPrint(S->Index, Depth+1);
              DebugPrint("\n");

              DebugPrint("Command = ", Depth);
              DebugPrint(S->Command, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(find_command_result S, u32 Depth)
        {
          DebugPrint("find_command_result\n", Depth);
          
            DebugPrint("Command = ", Depth);
            DebugPrint(S.Command, Depth+1);
            DebugPrint("\n");

            DebugPrint("Index = ", Depth);
            DebugPrint(S.Index, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(find_command_result* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("find_command_result\n", Depth);
            
              DebugPrint("Command = ", Depth);
              DebugPrint(S->Command, Depth+1);
              DebugPrint("\n");

              DebugPrint("Index = ", Depth);
              DebugPrint(S->Index, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(interactable_handle S, u32 Depth)
        {
          DebugPrint("interactable_handle\n", Depth);
          
            DebugPrint("Id = ", Depth);
            DebugPrint(S.Id, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(interactable_handle* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("interactable_handle\n", Depth);
            
              DebugPrint("Id = ", Depth);
              DebugPrint(S->Id, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(interactable S, u32 Depth)
        {
          DebugPrint("interactable\n", Depth);
          
            DebugPrint("ID = ", Depth);
            DebugPrint(S.ID, Depth+1);
            DebugPrint("\n");

            DebugPrint("MinP = ", Depth);
            DebugPrint(S.MinP, Depth+1);
            DebugPrint("\n");

            DebugPrint("MaxP = ", Depth);
            DebugPrint(S.MaxP, Depth+1);
            DebugPrint("\n");

            DebugPrint("Window = ", Depth);
            DebugPrint(S.Window, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(interactable* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("interactable\n", Depth);
            
              DebugPrint("ID = ", Depth);
              DebugPrint(S->ID, Depth+1);
              DebugPrint("\n");

              DebugPrint("MinP = ", Depth);
              DebugPrint(S->MinP, Depth+1);
              DebugPrint("\n");

              DebugPrint("MaxP = ", Depth);
              DebugPrint(S->MaxP, Depth+1);
              DebugPrint("\n");

              DebugPrint("Window = ", Depth);
              DebugPrint(S->Window, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(button_interaction_result S, u32 Depth)
        {
          DebugPrint("button_interaction_result\n", Depth);
          
            DebugPrint("Pressed = ", Depth);
            DebugPrint(S.Pressed, Depth+1);
            DebugPrint("\n");

            DebugPrint("Clicked = ", Depth);
            DebugPrint(S.Clicked, Depth+1);
            DebugPrint("\n");

            DebugPrint("Hover = ", Depth);
            DebugPrint(S.Hover, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(button_interaction_result* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("button_interaction_result\n", Depth);
            
              DebugPrint("Pressed = ", Depth);
              DebugPrint(S->Pressed, Depth+1);
              DebugPrint("\n");

              DebugPrint("Clicked = ", Depth);
              DebugPrint(S->Clicked, Depth+1);
              DebugPrint("\n");

              DebugPrint("Hover = ", Depth);
              DebugPrint(S->Hover, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(render_state S, u32 Depth)
        {
          DebugPrint("render_state\n", Depth);
          
            DebugPrint("Window = ", Depth);
            DebugPrint(S.Window, Depth+1);
            DebugPrint("\n");

            DebugPrint("WindowStartCommandIndex = ", Depth);
            DebugPrint(S.WindowStartCommandIndex, Depth+1);
            DebugPrint("\n");

            DebugPrint("Layout = ", Depth);
            DebugPrint(S.Layout, Depth+1);
            DebugPrint("\n");

            DebugPrint("Hover = ", Depth);
            DebugPrint(S.Hover, Depth+1);
            DebugPrint("\n");

            DebugPrint("Pressed = ", Depth);
            DebugPrint(S.Pressed, Depth+1);
            DebugPrint("\n");

            DebugPrint("Clicked = ", Depth);
            DebugPrint(S.Clicked, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(render_state* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("render_state\n", Depth);
            
              DebugPrint("Window = ", Depth);
              DebugPrint(S->Window, Depth+1);
              DebugPrint("\n");

              DebugPrint("WindowStartCommandIndex = ", Depth);
              DebugPrint(S->WindowStartCommandIndex, Depth+1);
              DebugPrint("\n");

              DebugPrint("Layout = ", Depth);
              DebugPrint(S->Layout, Depth+1);
              DebugPrint("\n");

              DebugPrint("Hover = ", Depth);
              DebugPrint(S->Hover, Depth+1);
              DebugPrint("\n");

              DebugPrint("Pressed = ", Depth);
              DebugPrint(S->Pressed, Depth+1);
              DebugPrint("\n");

              DebugPrint("Clicked = ", Depth);
              DebugPrint(S->Clicked, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(debug_text_render_group S, u32 Depth)
        {
          DebugPrint("debug_text_render_group\n", Depth);
          
            DebugPrint("SolidUIVertexBuffer = ", Depth);
            DebugPrint(S.SolidUIVertexBuffer, Depth+1);
            DebugPrint("\n");

            DebugPrint("SolidUIColorBuffer = ", Depth);
            DebugPrint(S.SolidUIColorBuffer, Depth+1);
            DebugPrint("\n");

            DebugPrint("SolidUIUVBuffer = ", Depth);
            DebugPrint(S.SolidUIUVBuffer, Depth+1);
            DebugPrint("\n");

            DebugPrint("FontTexture = ", Depth);
            DebugPrint(S.FontTexture, Depth+1);
            DebugPrint("\n");

            DebugPrint("Text2DShader = ", Depth);
            DebugPrint(S.Text2DShader, Depth+1);
            DebugPrint("\n");

            DebugPrint("TextTextureUniform = ", Depth);
            DebugPrint(S.TextTextureUniform, Depth+1);
            DebugPrint("\n");

            DebugPrint("Geo = ", Depth);
            DebugPrint(S.Geo, Depth+1);
            DebugPrint("\n");

            DebugPrint("DebugFontTextureShader = ", Depth);
            DebugPrint(S.DebugFontTextureShader, Depth+1);
            DebugPrint("\n");

            DebugPrint("SolidUIShader = ", Depth);
            DebugPrint(S.SolidUIShader, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(debug_text_render_group* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("debug_text_render_group\n", Depth);
            
              DebugPrint("SolidUIVertexBuffer = ", Depth);
              DebugPrint(S->SolidUIVertexBuffer, Depth+1);
              DebugPrint("\n");

              DebugPrint("SolidUIColorBuffer = ", Depth);
              DebugPrint(S->SolidUIColorBuffer, Depth+1);
              DebugPrint("\n");

              DebugPrint("SolidUIUVBuffer = ", Depth);
              DebugPrint(S->SolidUIUVBuffer, Depth+1);
              DebugPrint("\n");

              DebugPrint("FontTexture = ", Depth);
              DebugPrint(S->FontTexture, Depth+1);
              DebugPrint("\n");

              DebugPrint("Text2DShader = ", Depth);
              DebugPrint(S->Text2DShader, Depth+1);
              DebugPrint("\n");

              DebugPrint("TextTextureUniform = ", Depth);
              DebugPrint(S->TextTextureUniform, Depth+1);
              DebugPrint("\n");

              DebugPrint("Geo = ", Depth);
              DebugPrint(S->Geo, Depth+1);
              DebugPrint("\n");

              DebugPrint("DebugFontTextureShader = ", Depth);
              DebugPrint(S->DebugFontTextureShader, Depth+1);
              DebugPrint("\n");

              DebugPrint("SolidUIShader = ", Depth);
              DebugPrint(S->SolidUIShader, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(c_decl_variable S, u32 Depth)
        {
          DebugPrint("c_decl_variable\n", Depth);
          
            DebugPrint("Type = ", Depth);
            DebugPrint(S.Type, Depth+1);
            DebugPrint("\n");

            DebugPrint("Name = ", Depth);
            DebugPrint(S.Name, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(c_decl_variable* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("c_decl_variable\n", Depth);
            
              DebugPrint("Type = ", Depth);
              DebugPrint(S->Type, Depth+1);
              DebugPrint("\n");

              DebugPrint("Name = ", Depth);
              DebugPrint(S->Name, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(c_decl_stream S, u32 Depth)
        {
          DebugPrint("c_decl_stream\n", Depth);
          
            DebugPrint("FirstChunk = ", Depth);
            DebugPrint(S.FirstChunk, Depth+1);
            DebugPrint("\n");

            DebugPrint("LastChunk = ", Depth);
            DebugPrint(S.LastChunk, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(c_decl_stream* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("c_decl_stream\n", Depth);
            
              DebugPrint("FirstChunk = ", Depth);
              DebugPrint(S->FirstChunk, Depth+1);
              DebugPrint("\n");

              DebugPrint("LastChunk = ", Depth);
              DebugPrint(S->LastChunk, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(struct_def S, u32 Depth)
        {
          DebugPrint("struct_def\n", Depth);
          
            DebugPrint("Name = ", Depth);
            DebugPrint(S.Name, Depth+1);
            DebugPrint("\n");

            DebugPrint("DefinedInFile = ", Depth);
            DebugPrint(S.DefinedInFile, Depth+1);
            DebugPrint("\n");

            DebugPrint("Fields = ", Depth);
            DebugPrint(S.Fields, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(struct_def* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("struct_def\n", Depth);
            
              DebugPrint("Name = ", Depth);
              DebugPrint(S->Name, Depth+1);
              DebugPrint("\n");

              DebugPrint("DefinedInFile = ", Depth);
              DebugPrint(S->DefinedInFile, Depth+1);
              DebugPrint("\n");

              DebugPrint("Fields = ", Depth);
              DebugPrint(S->Fields, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(c_decl_union S, u32 Depth)
        {
          DebugPrint("c_decl_union\n", Depth);
          
            DebugPrint("Body = ", Depth);
            DebugPrint(S.Body, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(c_decl_union* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("c_decl_union\n", Depth);
            
              DebugPrint("Body = ", Depth);
              DebugPrint(S->Body, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(enum_field S, u32 Depth)
        {
          DebugPrint("enum_field\n", Depth);
          
            DebugPrint("Name = ", Depth);
            DebugPrint(S.Name, Depth+1);
            DebugPrint("\n");

            DebugPrint("Value = ", Depth);
            DebugPrint(S.Value, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(enum_field* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("enum_field\n", Depth);
            
              DebugPrint("Name = ", Depth);
              DebugPrint(S->Name, Depth+1);
              DebugPrint("\n");

              DebugPrint("Value = ", Depth);
              DebugPrint(S->Value, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(d_union_member S, u32 Depth)
        {
          DebugPrint("d_union_member\n", Depth);
          
            DebugPrint("Type = ", Depth);
            DebugPrint(S.Type, Depth+1);
            DebugPrint("\n");

            DebugPrint("Name = ", Depth);
            DebugPrint(S.Name, Depth+1);
            DebugPrint("\n");

            DebugPrint("Flags = ", Depth);
            DebugPrint(S.Flags, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(d_union_member* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("d_union_member\n", Depth);
            
              DebugPrint("Type = ", Depth);
              DebugPrint(S->Type, Depth+1);
              DebugPrint("\n");

              DebugPrint("Name = ", Depth);
              DebugPrint(S->Name, Depth+1);
              DebugPrint("\n");

              DebugPrint("Flags = ", Depth);
              DebugPrint(S->Flags, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(c_decl_stream_chunk S, u32 Depth)
        {
          DebugPrint("c_decl_stream_chunk\n", Depth);
          
            DebugPrint("Element = ", Depth);
            DebugPrint(S.Element, Depth+1);
            DebugPrint("\n");

            DebugPrint("Next = ", Depth);
            DebugPrint(S.Next, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(c_decl_stream_chunk* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("c_decl_stream_chunk\n", Depth);
            
              DebugPrint("Element = ", Depth);
              DebugPrint(S->Element, Depth+1);
              DebugPrint("\n");

              DebugPrint("Next = ", Depth);
              DebugPrint(S->Next, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(enum_def S, u32 Depth)
        {
          DebugPrint("enum_def\n", Depth);
          
            DebugPrint("Name = ", Depth);
            DebugPrint(S.Name, Depth+1);
            DebugPrint("\n");

            DebugPrint("Fields = ", Depth);
            DebugPrint(S.Fields, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(enum_def* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("enum_def\n", Depth);
            
              DebugPrint("Name = ", Depth);
              DebugPrint(S->Name, Depth+1);
              DebugPrint("\n");

              DebugPrint("Fields = ", Depth);
              DebugPrint(S->Fields, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(datatype S, u32 Depth)
        {
          DebugPrint("datatype\n", Depth);
          
            DebugPrint("Type = ", Depth);
            DebugPrint(S.Type, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(datatype* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("datatype\n", Depth);
            
              DebugPrint("Type = ", Depth);
              DebugPrint(S->Type, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(c_token S, u32 Depth)
        {
          DebugPrint("c_token\n", Depth);
          
            DebugPrint("Type = ", Depth);
            DebugPrint(S.Type, Depth+1);
            DebugPrint("\n");

            DebugPrint("Value = ", Depth);
            DebugPrint(S.Value, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(c_token* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("c_token\n", Depth);
            
              DebugPrint("Type = ", Depth);
              DebugPrint(S->Type, Depth+1);
              DebugPrint("\n");

              DebugPrint("Value = ", Depth);
              DebugPrint(S->Value, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(d_union_decl S, u32 Depth)
        {
          DebugPrint("d_union_decl\n", Depth);
          
            DebugPrint("Name = ", Depth);
            DebugPrint(S.Name, Depth+1);
            DebugPrint("\n");

            DebugPrint("Members = ", Depth);
            DebugPrint(S.Members, Depth+1);
            DebugPrint("\n");

            DebugPrint("CommonMembers = ", Depth);
            DebugPrint(S.CommonMembers, Depth+1);
            DebugPrint("\n");

            DebugPrint("CustomEnumType = ", Depth);
            DebugPrint(S.CustomEnumType, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(d_union_decl* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("d_union_decl\n", Depth);
            
              DebugPrint("Name = ", Depth);
              DebugPrint(S->Name, Depth+1);
              DebugPrint("\n");

              DebugPrint("Members = ", Depth);
              DebugPrint(S->Members, Depth+1);
              DebugPrint("\n");

              DebugPrint("CommonMembers = ", Depth);
              DebugPrint(S->CommonMembers, Depth+1);
              DebugPrint("\n");

              DebugPrint("CustomEnumType = ", Depth);
              DebugPrint(S->CustomEnumType, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(c_parse_result S, u32 Depth)
        {
          DebugPrint("c_parse_result\n", Depth);
          
            DebugPrint("Valid = ", Depth);
            DebugPrint(S.Valid, Depth+1);
            DebugPrint("\n");

            DebugPrint("Tokens = ", Depth);
            DebugPrint(S.Tokens, Depth+1);
            DebugPrint("\n");

            DebugPrint("OutputTokens = ", Depth);
            DebugPrint(S.OutputTokens, Depth+1);
            DebugPrint("\n");

            DebugPrint("Filename = ", Depth);
            DebugPrint(S.Filename, Depth+1);
            DebugPrint("\n");

            DebugPrint("LineNumber = ", Depth);
            DebugPrint(S.LineNumber, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(c_parse_result* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("c_parse_result\n", Depth);
            
              DebugPrint("Valid = ", Depth);
              DebugPrint(S->Valid, Depth+1);
              DebugPrint("\n");

              DebugPrint("Tokens = ", Depth);
              DebugPrint(S->Tokens, Depth+1);
              DebugPrint("\n");

              DebugPrint("OutputTokens = ", Depth);
              DebugPrint(S->OutputTokens, Depth+1);
              DebugPrint("\n");

              DebugPrint("Filename = ", Depth);
              DebugPrint(S->Filename, Depth+1);
              DebugPrint("\n");

              DebugPrint("LineNumber = ", Depth);
              DebugPrint(S->LineNumber, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(meta_func S, u32 Depth)
        {
          DebugPrint("meta_func\n", Depth);
          
            DebugPrint("Type = ", Depth);
            DebugPrint(S.Type, Depth+1);
            DebugPrint("\n");

            DebugPrint("Name = ", Depth);
            DebugPrint(S.Name, Depth+1);
            DebugPrint("\n");

            DebugPrint("ArgType = ", Depth);
            DebugPrint(S.ArgType, Depth+1);
            DebugPrint("\n");

            DebugPrint("ArgName = ", Depth);
            DebugPrint(S.ArgName, Depth+1);
            DebugPrint("\n");

            DebugPrint("Body = ", Depth);
            DebugPrint(S.Body, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(meta_func* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("meta_func\n", Depth);
            
              DebugPrint("Type = ", Depth);
              DebugPrint(S->Type, Depth+1);
              DebugPrint("\n");

              DebugPrint("Name = ", Depth);
              DebugPrint(S->Name, Depth+1);
              DebugPrint("\n");

              DebugPrint("ArgType = ", Depth);
              DebugPrint(S->ArgType, Depth+1);
              DebugPrint("\n");

              DebugPrint("ArgName = ", Depth);
              DebugPrint(S->ArgName, Depth+1);
              DebugPrint("\n");

              DebugPrint("Body = ", Depth);
              DebugPrint(S->Body, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(todo S, u32 Depth)
        {
          DebugPrint("todo\n", Depth);
          
            DebugPrint("Id = ", Depth);
            DebugPrint(S.Id, Depth+1);
            DebugPrint("\n");

            DebugPrint("Value = ", Depth);
            DebugPrint(S.Value, Depth+1);
            DebugPrint("\n");

            DebugPrint("FoundInCodebase = ", Depth);
            DebugPrint(S.FoundInCodebase, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(todo* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("todo\n", Depth);
            
              DebugPrint("Id = ", Depth);
              DebugPrint(S->Id, Depth+1);
              DebugPrint("\n");

              DebugPrint("Value = ", Depth);
              DebugPrint(S->Value, Depth+1);
              DebugPrint("\n");

              DebugPrint("FoundInCodebase = ", Depth);
              DebugPrint(S->FoundInCodebase, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(tag S, u32 Depth)
        {
          DebugPrint("tag\n", Depth);
          
            DebugPrint("Tag = ", Depth);
            DebugPrint(S.Tag, Depth+1);
            DebugPrint("\n");

            DebugPrint("Todos = ", Depth);
            DebugPrint(S.Todos, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(tag* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("tag\n", Depth);
            
              DebugPrint("Tag = ", Depth);
              DebugPrint(S->Tag, Depth+1);
              DebugPrint("\n");

              DebugPrint("Todos = ", Depth);
              DebugPrint(S->Todos, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(person S, u32 Depth)
        {
          DebugPrint("person\n", Depth);
          
            DebugPrint("Name = ", Depth);
            DebugPrint(S.Name, Depth+1);
            DebugPrint("\n");

            DebugPrint("Tags = ", Depth);
            DebugPrint(S.Tags, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(person* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("person\n", Depth);
            
              DebugPrint("Name = ", Depth);
              DebugPrint(S->Name, Depth+1);
              DebugPrint("\n");

              DebugPrint("Tags = ", Depth);
              DebugPrint(S->Tags, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(arguments S, u32 Depth)
        {
          DebugPrint("arguments\n", Depth);
          
            DebugPrint("Outpath = ", Depth);
            DebugPrint(S.Outpath, Depth+1);
            DebugPrint("\n");

            DebugPrint("Files = ", Depth);
            DebugPrint(S.Files, Depth+1);
            DebugPrint("\n");

            DebugPrint("DoDebugWindow = ", Depth);
            DebugPrint(S.DoDebugWindow, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(arguments* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("arguments\n", Depth);
            
              DebugPrint("Outpath = ", Depth);
              DebugPrint(S->Outpath, Depth+1);
              DebugPrint("\n");

              DebugPrint("Files = ", Depth);
              DebugPrint(S->Files, Depth+1);
              DebugPrint("\n");

              DebugPrint("DoDebugWindow = ", Depth);
              DebugPrint(S->DoDebugWindow, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(program_datatypes S, u32 Depth)
        {
          DebugPrint("program_datatypes\n", Depth);
          
            DebugPrint("Structs = ", Depth);
            DebugPrint(S.Structs, Depth+1);
            DebugPrint("\n");

            DebugPrint("Enums = ", Depth);
            DebugPrint(S.Enums, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(program_datatypes* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("program_datatypes\n", Depth);
            
              DebugPrint("Structs = ", Depth);
              DebugPrint(S->Structs, Depth+1);
              DebugPrint("\n");

              DebugPrint("Enums = ", Depth);
              DebugPrint(S->Enums, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(c_decl_iterator S, u32 Depth)
        {
          DebugPrint("c_decl_iterator\n", Depth);
          
            DebugPrint("At = ", Depth);
            DebugPrint(S.At, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(c_decl_iterator* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("c_decl_iterator\n", Depth);
            
              DebugPrint("At = ", Depth);
              DebugPrint(S->At, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(for_enum_constraints S, u32 Depth)
        {
          DebugPrint("for_enum_constraints\n", Depth);
          
            DebugPrint("TypeName = ", Depth);
            DebugPrint(S.TypeName, Depth+1);
            DebugPrint("\n");

            DebugPrint("ValueName = ", Depth);
            DebugPrint(S.ValueName, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(for_enum_constraints* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("for_enum_constraints\n", Depth);
            
              DebugPrint("TypeName = ", Depth);
              DebugPrint(S->TypeName, Depth+1);
              DebugPrint("\n");

              DebugPrint("ValueName = ", Depth);
              DebugPrint(S->ValueName, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(body_text_constraints S, u32 Depth)
        {
          DebugPrint("body_text_constraints\n", Depth);
          
            DebugPrint("MemberContains = ", Depth);
            DebugPrint(S.MemberContains, Depth+1);
            DebugPrint("\n");

            DebugPrint("TypeTag = ", Depth);
            DebugPrint(S.TypeTag, Depth+1);
            DebugPrint("\n");

            DebugPrint("TypeName = ", Depth);
            DebugPrint(S.TypeName, Depth+1);
            DebugPrint("\n");

            DebugPrint("ValueName = ", Depth);
            DebugPrint(S.ValueName, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(body_text_constraints* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("body_text_constraints\n", Depth);
            
              DebugPrint("MemberContains = ", Depth);
              DebugPrint(S->MemberContains, Depth+1);
              DebugPrint("\n");

              DebugPrint("TypeTag = ", Depth);
              DebugPrint(S->TypeTag, Depth+1);
              DebugPrint("\n");

              DebugPrint("TypeName = ", Depth);
              DebugPrint(S->TypeName, Depth+1);
              DebugPrint("\n");

              DebugPrint("ValueName = ", Depth);
              DebugPrint(S->ValueName, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(replacement_pattern S, u32 Depth)
        {
          DebugPrint("replacement_pattern\n", Depth);
          
            DebugPrint("Match = ", Depth);
            DebugPrint(S.Match, Depth+1);
            DebugPrint("\n");

            DebugPrint("Replace = ", Depth);
            DebugPrint(S.Replace, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(replacement_pattern* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("replacement_pattern\n", Depth);
            
              DebugPrint("Match = ", Depth);
              DebugPrint(S->Match, Depth+1);
              DebugPrint("\n");

              DebugPrint("Replace = ", Depth);
              DebugPrint(S->Replace, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(tagged_counted_string_stream_stream_chunk S, u32 Depth)
        {
          DebugPrint("tagged_counted_string_stream_stream_chunk\n", Depth);
          
            DebugPrint("Element = ", Depth);
            DebugPrint(S.Element, Depth+1);
            DebugPrint("\n");

            DebugPrint("Next = ", Depth);
            DebugPrint(S.Next, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(tagged_counted_string_stream_stream_chunk* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("tagged_counted_string_stream_stream_chunk\n", Depth);
            
              DebugPrint("Element = ", Depth);
              DebugPrint(S->Element, Depth+1);
              DebugPrint("\n");

              DebugPrint("Next = ", Depth);
              DebugPrint(S->Next, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(tagged_counted_string_stream_stream S, u32 Depth)
        {
          DebugPrint("tagged_counted_string_stream_stream\n", Depth);
          
            DebugPrint("FirstChunk = ", Depth);
            DebugPrint(S.FirstChunk, Depth+1);
            DebugPrint("\n");

            DebugPrint("LastChunk = ", Depth);
            DebugPrint(S.LastChunk, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(tagged_counted_string_stream_stream* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("tagged_counted_string_stream_stream\n", Depth);
            
              DebugPrint("FirstChunk = ", Depth);
              DebugPrint(S->FirstChunk, Depth+1);
              DebugPrint("\n");

              DebugPrint("LastChunk = ", Depth);
              DebugPrint(S->LastChunk, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(tagged_counted_string_stream_iterator S, u32 Depth)
        {
          DebugPrint("tagged_counted_string_stream_iterator\n", Depth);
          
            DebugPrint("Stream = ", Depth);
            DebugPrint(S.Stream, Depth+1);
            DebugPrint("\n");

            DebugPrint("At = ", Depth);
            DebugPrint(S.At, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(tagged_counted_string_stream_iterator* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("tagged_counted_string_stream_iterator\n", Depth);
            
              DebugPrint("Stream = ", Depth);
              DebugPrint(S->Stream, Depth+1);
              DebugPrint("\n");

              DebugPrint("At = ", Depth);
              DebugPrint(S->At, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(c_token_cursor S, u32 Depth)
        {
          DebugPrint("c_token_cursor\n", Depth);
          
            DebugPrint("Start = ", Depth);
            DebugPrint(S.Start, Depth+1);
            DebugPrint("\n");

            DebugPrint("End = ", Depth);
            DebugPrint(S.End, Depth+1);
            DebugPrint("\n");

            DebugPrint("At = ", Depth);
            DebugPrint(S.At, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(c_token_cursor* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("c_token_cursor\n", Depth);
            
              DebugPrint("Start = ", Depth);
              DebugPrint(S->Start, Depth+1);
              DebugPrint("\n");

              DebugPrint("End = ", Depth);
              DebugPrint(S->End, Depth+1);
              DebugPrint("\n");

              DebugPrint("At = ", Depth);
              DebugPrint(S->At, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(enum_field_stream_chunk S, u32 Depth)
        {
          DebugPrint("enum_field_stream_chunk\n", Depth);
          
            DebugPrint("Element = ", Depth);
            DebugPrint(S.Element, Depth+1);
            DebugPrint("\n");

            DebugPrint("Next = ", Depth);
            DebugPrint(S.Next, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(enum_field_stream_chunk* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("enum_field_stream_chunk\n", Depth);
            
              DebugPrint("Element = ", Depth);
              DebugPrint(S->Element, Depth+1);
              DebugPrint("\n");

              DebugPrint("Next = ", Depth);
              DebugPrint(S->Next, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(enum_field_stream S, u32 Depth)
        {
          DebugPrint("enum_field_stream\n", Depth);
          
            DebugPrint("FirstChunk = ", Depth);
            DebugPrint(S.FirstChunk, Depth+1);
            DebugPrint("\n");

            DebugPrint("LastChunk = ", Depth);
            DebugPrint(S.LastChunk, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(enum_field_stream* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("enum_field_stream\n", Depth);
            
              DebugPrint("FirstChunk = ", Depth);
              DebugPrint(S->FirstChunk, Depth+1);
              DebugPrint("\n");

              DebugPrint("LastChunk = ", Depth);
              DebugPrint(S->LastChunk, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(enum_field_iterator S, u32 Depth)
        {
          DebugPrint("enum_field_iterator\n", Depth);
          
            DebugPrint("Stream = ", Depth);
            DebugPrint(S.Stream, Depth+1);
            DebugPrint("\n");

            DebugPrint("At = ", Depth);
            DebugPrint(S.At, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(enum_field_iterator* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("enum_field_iterator\n", Depth);
            
              DebugPrint("Stream = ", Depth);
              DebugPrint(S->Stream, Depth+1);
              DebugPrint("\n");

              DebugPrint("At = ", Depth);
              DebugPrint(S->At, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(enum_def_cursor S, u32 Depth)
        {
          DebugPrint("enum_def_cursor\n", Depth);
          
            DebugPrint("Start = ", Depth);
            DebugPrint(S.Start, Depth+1);
            DebugPrint("\n");

            DebugPrint("End = ", Depth);
            DebugPrint(S.End, Depth+1);
            DebugPrint("\n");

            DebugPrint("At = ", Depth);
            DebugPrint(S.At, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(enum_def_cursor* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("enum_def_cursor\n", Depth);
            
              DebugPrint("Start = ", Depth);
              DebugPrint(S->Start, Depth+1);
              DebugPrint("\n");

              DebugPrint("End = ", Depth);
              DebugPrint(S->End, Depth+1);
              DebugPrint("\n");

              DebugPrint("At = ", Depth);
              DebugPrint(S->At, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(enum_def_stream_chunk S, u32 Depth)
        {
          DebugPrint("enum_def_stream_chunk\n", Depth);
          
            DebugPrint("Element = ", Depth);
            DebugPrint(S.Element, Depth+1);
            DebugPrint("\n");

            DebugPrint("Next = ", Depth);
            DebugPrint(S.Next, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(enum_def_stream_chunk* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("enum_def_stream_chunk\n", Depth);
            
              DebugPrint("Element = ", Depth);
              DebugPrint(S->Element, Depth+1);
              DebugPrint("\n");

              DebugPrint("Next = ", Depth);
              DebugPrint(S->Next, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(enum_def_stream S, u32 Depth)
        {
          DebugPrint("enum_def_stream\n", Depth);
          
            DebugPrint("FirstChunk = ", Depth);
            DebugPrint(S.FirstChunk, Depth+1);
            DebugPrint("\n");

            DebugPrint("LastChunk = ", Depth);
            DebugPrint(S.LastChunk, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(enum_def_stream* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("enum_def_stream\n", Depth);
            
              DebugPrint("FirstChunk = ", Depth);
              DebugPrint(S->FirstChunk, Depth+1);
              DebugPrint("\n");

              DebugPrint("LastChunk = ", Depth);
              DebugPrint(S->LastChunk, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(enum_def_iterator S, u32 Depth)
        {
          DebugPrint("enum_def_iterator\n", Depth);
          
            DebugPrint("Stream = ", Depth);
            DebugPrint(S.Stream, Depth+1);
            DebugPrint("\n");

            DebugPrint("At = ", Depth);
            DebugPrint(S.At, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(enum_def_iterator* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("enum_def_iterator\n", Depth);
            
              DebugPrint("Stream = ", Depth);
              DebugPrint(S->Stream, Depth+1);
              DebugPrint("\n");

              DebugPrint("At = ", Depth);
              DebugPrint(S->At, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(v3_cursor S, u32 Depth)
        {
          DebugPrint("v3_cursor\n", Depth);
          
            DebugPrint("Start = ", Depth);
            DebugPrint(S.Start, Depth+1);
            DebugPrint("\n");

            DebugPrint("End = ", Depth);
            DebugPrint(S.End, Depth+1);
            DebugPrint("\n");

            DebugPrint("At = ", Depth);
            DebugPrint(S.At, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(v3_cursor* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("v3_cursor\n", Depth);
            
              DebugPrint("Start = ", Depth);
              DebugPrint(S->Start, Depth+1);
              DebugPrint("\n");

              DebugPrint("End = ", Depth);
              DebugPrint(S->End, Depth+1);
              DebugPrint("\n");

              DebugPrint("At = ", Depth);
              DebugPrint(S->At, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(c_decl S, u32 Depth)
        {
          DebugPrint("c_decl\n", Depth);
          
            DebugPrint("Type = ", Depth);
            DebugPrint(S.Type, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(c_decl* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("c_decl\n", Depth);
            
              DebugPrint("Type = ", Depth);
              DebugPrint(S->Type, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(d_union_member_stream_chunk S, u32 Depth)
        {
          DebugPrint("d_union_member_stream_chunk\n", Depth);
          
            DebugPrint("Element = ", Depth);
            DebugPrint(S.Element, Depth+1);
            DebugPrint("\n");

            DebugPrint("Next = ", Depth);
            DebugPrint(S.Next, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(d_union_member_stream_chunk* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("d_union_member_stream_chunk\n", Depth);
            
              DebugPrint("Element = ", Depth);
              DebugPrint(S->Element, Depth+1);
              DebugPrint("\n");

              DebugPrint("Next = ", Depth);
              DebugPrint(S->Next, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(d_union_member_stream S, u32 Depth)
        {
          DebugPrint("d_union_member_stream\n", Depth);
          
            DebugPrint("FirstChunk = ", Depth);
            DebugPrint(S.FirstChunk, Depth+1);
            DebugPrint("\n");

            DebugPrint("LastChunk = ", Depth);
            DebugPrint(S.LastChunk, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(d_union_member_stream* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("d_union_member_stream\n", Depth);
            
              DebugPrint("FirstChunk = ", Depth);
              DebugPrint(S->FirstChunk, Depth+1);
              DebugPrint("\n");

              DebugPrint("LastChunk = ", Depth);
              DebugPrint(S->LastChunk, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(d_union_member_iterator S, u32 Depth)
        {
          DebugPrint("d_union_member_iterator\n", Depth);
          
            DebugPrint("Stream = ", Depth);
            DebugPrint(S.Stream, Depth+1);
            DebugPrint("\n");

            DebugPrint("At = ", Depth);
            DebugPrint(S.At, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(d_union_member_iterator* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("d_union_member_iterator\n", Depth);
            
              DebugPrint("Stream = ", Depth);
              DebugPrint(S->Stream, Depth+1);
              DebugPrint("\n");

              DebugPrint("At = ", Depth);
              DebugPrint(S->At, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(ui_render_command S, u32 Depth)
        {
          DebugPrint("ui_render_command\n", Depth);
          
            DebugPrint("Type = ", Depth);
            DebugPrint(S.Type, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(ui_render_command* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("ui_render_command\n", Depth);
            
              DebugPrint("Type = ", Depth);
              DebugPrint(S->Type, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(struct_def_stream_chunk S, u32 Depth)
        {
          DebugPrint("struct_def_stream_chunk\n", Depth);
          
            DebugPrint("Element = ", Depth);
            DebugPrint(S.Element, Depth+1);
            DebugPrint("\n");

            DebugPrint("Next = ", Depth);
            DebugPrint(S.Next, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(struct_def_stream_chunk* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("struct_def_stream_chunk\n", Depth);
            
              DebugPrint("Element = ", Depth);
              DebugPrint(S->Element, Depth+1);
              DebugPrint("\n");

              DebugPrint("Next = ", Depth);
              DebugPrint(S->Next, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(struct_def_stream S, u32 Depth)
        {
          DebugPrint("struct_def_stream\n", Depth);
          
            DebugPrint("FirstChunk = ", Depth);
            DebugPrint(S.FirstChunk, Depth+1);
            DebugPrint("\n");

            DebugPrint("LastChunk = ", Depth);
            DebugPrint(S.LastChunk, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(struct_def_stream* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("struct_def_stream\n", Depth);
            
              DebugPrint("FirstChunk = ", Depth);
              DebugPrint(S->FirstChunk, Depth+1);
              DebugPrint("\n");

              DebugPrint("LastChunk = ", Depth);
              DebugPrint(S->LastChunk, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(struct_def_iterator S, u32 Depth)
        {
          DebugPrint("struct_def_iterator\n", Depth);
          
            DebugPrint("Stream = ", Depth);
            DebugPrint(S.Stream, Depth+1);
            DebugPrint("\n");

            DebugPrint("At = ", Depth);
            DebugPrint(S.At, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(struct_def_iterator* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("struct_def_iterator\n", Depth);
            
              DebugPrint("Stream = ", Depth);
              DebugPrint(S->Stream, Depth+1);
              DebugPrint("\n");

              DebugPrint("At = ", Depth);
              DebugPrint(S->At, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(counted_string_cursor S, u32 Depth)
        {
          DebugPrint("counted_string_cursor\n", Depth);
          
            DebugPrint("Start = ", Depth);
            DebugPrint(S.Start, Depth+1);
            DebugPrint("\n");

            DebugPrint("End = ", Depth);
            DebugPrint(S.End, Depth+1);
            DebugPrint("\n");

            DebugPrint("At = ", Depth);
            DebugPrint(S.At, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(counted_string_cursor* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("counted_string_cursor\n", Depth);
            
              DebugPrint("Start = ", Depth);
              DebugPrint(S->Start, Depth+1);
              DebugPrint("\n");

              DebugPrint("End = ", Depth);
              DebugPrint(S->End, Depth+1);
              DebugPrint("\n");

              DebugPrint("At = ", Depth);
              DebugPrint(S->At, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(tag_stream_chunk S, u32 Depth)
        {
          DebugPrint("tag_stream_chunk\n", Depth);
          
            DebugPrint("Element = ", Depth);
            DebugPrint(S.Element, Depth+1);
            DebugPrint("\n");

            DebugPrint("Next = ", Depth);
            DebugPrint(S.Next, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(tag_stream_chunk* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("tag_stream_chunk\n", Depth);
            
              DebugPrint("Element = ", Depth);
              DebugPrint(S->Element, Depth+1);
              DebugPrint("\n");

              DebugPrint("Next = ", Depth);
              DebugPrint(S->Next, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(tag_stream S, u32 Depth)
        {
          DebugPrint("tag_stream\n", Depth);
          
            DebugPrint("FirstChunk = ", Depth);
            DebugPrint(S.FirstChunk, Depth+1);
            DebugPrint("\n");

            DebugPrint("LastChunk = ", Depth);
            DebugPrint(S.LastChunk, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(tag_stream* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("tag_stream\n", Depth);
            
              DebugPrint("FirstChunk = ", Depth);
              DebugPrint(S->FirstChunk, Depth+1);
              DebugPrint("\n");

              DebugPrint("LastChunk = ", Depth);
              DebugPrint(S->LastChunk, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(tag_iterator S, u32 Depth)
        {
          DebugPrint("tag_iterator\n", Depth);
          
            DebugPrint("Stream = ", Depth);
            DebugPrint(S.Stream, Depth+1);
            DebugPrint("\n");

            DebugPrint("At = ", Depth);
            DebugPrint(S.At, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(tag_iterator* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("tag_iterator\n", Depth);
            
              DebugPrint("Stream = ", Depth);
              DebugPrint(S->Stream, Depth+1);
              DebugPrint("\n");

              DebugPrint("At = ", Depth);
              DebugPrint(S->At, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(c_parse_result_cursor S, u32 Depth)
        {
          DebugPrint("c_parse_result_cursor\n", Depth);
          
            DebugPrint("Start = ", Depth);
            DebugPrint(S.Start, Depth+1);
            DebugPrint("\n");

            DebugPrint("End = ", Depth);
            DebugPrint(S.End, Depth+1);
            DebugPrint("\n");

            DebugPrint("At = ", Depth);
            DebugPrint(S.At, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(c_parse_result_cursor* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("c_parse_result_cursor\n", Depth);
            
              DebugPrint("Start = ", Depth);
              DebugPrint(S->Start, Depth+1);
              DebugPrint("\n");

              DebugPrint("End = ", Depth);
              DebugPrint(S->End, Depth+1);
              DebugPrint("\n");

              DebugPrint("At = ", Depth);
              DebugPrint(S->At, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(c_decl_function S, u32 Depth)
        {
          DebugPrint("c_decl_function\n", Depth);
          
            DebugPrint("Type = ", Depth);
            DebugPrint(S.Type, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(c_decl_function* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("c_decl_function\n", Depth);
            
              DebugPrint("Type = ", Depth);
              DebugPrint(S->Type, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(c_decl_cursor S, u32 Depth)
        {
          DebugPrint("c_decl_cursor\n", Depth);
          
            DebugPrint("Start = ", Depth);
            DebugPrint(S.Start, Depth+1);
            DebugPrint("\n");

            DebugPrint("End = ", Depth);
            DebugPrint(S.End, Depth+1);
            DebugPrint("\n");

            DebugPrint("At = ", Depth);
            DebugPrint(S.At, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(c_decl_cursor* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("c_decl_cursor\n", Depth);
            
              DebugPrint("Start = ", Depth);
              DebugPrint(S->Start, Depth+1);
              DebugPrint("\n");

              DebugPrint("End = ", Depth);
              DebugPrint(S->End, Depth+1);
              DebugPrint("\n");

              DebugPrint("At = ", Depth);
              DebugPrint(S->At, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(test_meta_struct_stream_chunk S, u32 Depth)
        {
          DebugPrint("test_meta_struct_stream_chunk\n", Depth);
          
            DebugPrint("Element = ", Depth);
            DebugPrint(S.Element, Depth+1);
            DebugPrint("\n");

            DebugPrint("Next = ", Depth);
            DebugPrint(S.Next, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(test_meta_struct_stream_chunk* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("test_meta_struct_stream_chunk\n", Depth);
            
              DebugPrint("Element = ", Depth);
              DebugPrint(S->Element, Depth+1);
              DebugPrint("\n");

              DebugPrint("Next = ", Depth);
              DebugPrint(S->Next, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(test_meta_struct_stream S, u32 Depth)
        {
          DebugPrint("test_meta_struct_stream\n", Depth);
          
            DebugPrint("FirstChunk = ", Depth);
            DebugPrint(S.FirstChunk, Depth+1);
            DebugPrint("\n");

            DebugPrint("LastChunk = ", Depth);
            DebugPrint(S.LastChunk, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(test_meta_struct_stream* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("test_meta_struct_stream\n", Depth);
            
              DebugPrint("FirstChunk = ", Depth);
              DebugPrint(S->FirstChunk, Depth+1);
              DebugPrint("\n");

              DebugPrint("LastChunk = ", Depth);
              DebugPrint(S->LastChunk, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(test_meta_struct_iterator S, u32 Depth)
        {
          DebugPrint("test_meta_struct_iterator\n", Depth);
          
            DebugPrint("Stream = ", Depth);
            DebugPrint(S.Stream, Depth+1);
            DebugPrint("\n");

            DebugPrint("At = ", Depth);
            DebugPrint(S.At, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(test_meta_struct_iterator* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("test_meta_struct_iterator\n", Depth);
            
              DebugPrint("Stream = ", Depth);
              DebugPrint(S->Stream, Depth+1);
              DebugPrint("\n");

              DebugPrint("At = ", Depth);
              DebugPrint(S->At, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(struct_def_cursor S, u32 Depth)
        {
          DebugPrint("struct_def_cursor\n", Depth);
          
            DebugPrint("Start = ", Depth);
            DebugPrint(S.Start, Depth+1);
            DebugPrint("\n");

            DebugPrint("End = ", Depth);
            DebugPrint(S.End, Depth+1);
            DebugPrint("\n");

            DebugPrint("At = ", Depth);
            DebugPrint(S.At, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(struct_def_cursor* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("struct_def_cursor\n", Depth);
            
              DebugPrint("Start = ", Depth);
              DebugPrint(S->Start, Depth+1);
              DebugPrint("\n");

              DebugPrint("End = ", Depth);
              DebugPrint(S->End, Depth+1);
              DebugPrint("\n");

              DebugPrint("At = ", Depth);
              DebugPrint(S->At, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(person_stream_chunk S, u32 Depth)
        {
          DebugPrint("person_stream_chunk\n", Depth);
          
            DebugPrint("Element = ", Depth);
            DebugPrint(S.Element, Depth+1);
            DebugPrint("\n");

            DebugPrint("Next = ", Depth);
            DebugPrint(S.Next, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(person_stream_chunk* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("person_stream_chunk\n", Depth);
            
              DebugPrint("Element = ", Depth);
              DebugPrint(S->Element, Depth+1);
              DebugPrint("\n");

              DebugPrint("Next = ", Depth);
              DebugPrint(S->Next, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(person_stream S, u32 Depth)
        {
          DebugPrint("person_stream\n", Depth);
          
            DebugPrint("FirstChunk = ", Depth);
            DebugPrint(S.FirstChunk, Depth+1);
            DebugPrint("\n");

            DebugPrint("LastChunk = ", Depth);
            DebugPrint(S.LastChunk, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(person_stream* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("person_stream\n", Depth);
            
              DebugPrint("FirstChunk = ", Depth);
              DebugPrint(S->FirstChunk, Depth+1);
              DebugPrint("\n");

              DebugPrint("LastChunk = ", Depth);
              DebugPrint(S->LastChunk, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(person_iterator S, u32 Depth)
        {
          DebugPrint("person_iterator\n", Depth);
          
            DebugPrint("Stream = ", Depth);
            DebugPrint(S.Stream, Depth+1);
            DebugPrint("\n");

            DebugPrint("At = ", Depth);
            DebugPrint(S.At, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(person_iterator* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("person_iterator\n", Depth);
            
              DebugPrint("Stream = ", Depth);
              DebugPrint(S->Stream, Depth+1);
              DebugPrint("\n");

              DebugPrint("At = ", Depth);
              DebugPrint(S->At, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(counted_string_stream_chunk S, u32 Depth)
        {
          DebugPrint("counted_string_stream_chunk\n", Depth);
          
            DebugPrint("Element = ", Depth);
            DebugPrint(S.Element, Depth+1);
            DebugPrint("\n");

            DebugPrint("Next = ", Depth);
            DebugPrint(S.Next, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(counted_string_stream_chunk* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("counted_string_stream_chunk\n", Depth);
            
              DebugPrint("Element = ", Depth);
              DebugPrint(S->Element, Depth+1);
              DebugPrint("\n");

              DebugPrint("Next = ", Depth);
              DebugPrint(S->Next, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(counted_string_stream S, u32 Depth)
        {
          DebugPrint("counted_string_stream\n", Depth);
          
            DebugPrint("FirstChunk = ", Depth);
            DebugPrint(S.FirstChunk, Depth+1);
            DebugPrint("\n");

            DebugPrint("LastChunk = ", Depth);
            DebugPrint(S.LastChunk, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(counted_string_stream* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("counted_string_stream\n", Depth);
            
              DebugPrint("FirstChunk = ", Depth);
              DebugPrint(S->FirstChunk, Depth+1);
              DebugPrint("\n");

              DebugPrint("LastChunk = ", Depth);
              DebugPrint(S->LastChunk, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(counted_string_iterator S, u32 Depth)
        {
          DebugPrint("counted_string_iterator\n", Depth);
          
            DebugPrint("Stream = ", Depth);
            DebugPrint(S.Stream, Depth+1);
            DebugPrint("\n");

            DebugPrint("At = ", Depth);
            DebugPrint(S.At, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(counted_string_iterator* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("counted_string_iterator\n", Depth);
            
              DebugPrint("Stream = ", Depth);
              DebugPrint(S->Stream, Depth+1);
              DebugPrint("\n");

              DebugPrint("At = ", Depth);
              DebugPrint(S->At, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(todo_stream_chunk S, u32 Depth)
        {
          DebugPrint("todo_stream_chunk\n", Depth);
          
            DebugPrint("Element = ", Depth);
            DebugPrint(S.Element, Depth+1);
            DebugPrint("\n");

            DebugPrint("Next = ", Depth);
            DebugPrint(S.Next, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(todo_stream_chunk* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("todo_stream_chunk\n", Depth);
            
              DebugPrint("Element = ", Depth);
              DebugPrint(S->Element, Depth+1);
              DebugPrint("\n");

              DebugPrint("Next = ", Depth);
              DebugPrint(S->Next, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(todo_stream S, u32 Depth)
        {
          DebugPrint("todo_stream\n", Depth);
          
            DebugPrint("FirstChunk = ", Depth);
            DebugPrint(S.FirstChunk, Depth+1);
            DebugPrint("\n");

            DebugPrint("LastChunk = ", Depth);
            DebugPrint(S.LastChunk, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(todo_stream* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("todo_stream\n", Depth);
            
              DebugPrint("FirstChunk = ", Depth);
              DebugPrint(S->FirstChunk, Depth+1);
              DebugPrint("\n");

              DebugPrint("LastChunk = ", Depth);
              DebugPrint(S->LastChunk, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(todo_iterator S, u32 Depth)
        {
          DebugPrint("todo_iterator\n", Depth);
          
            DebugPrint("Stream = ", Depth);
            DebugPrint(S.Stream, Depth+1);
            DebugPrint("\n");

            DebugPrint("At = ", Depth);
            DebugPrint(S.At, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(todo_iterator* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("todo_iterator\n", Depth);
            
              DebugPrint("Stream = ", Depth);
              DebugPrint(S->Stream, Depth+1);
              DebugPrint("\n");

              DebugPrint("At = ", Depth);
              DebugPrint(S->At, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(work_queue_entry S, u32 Depth)
        {
          DebugPrint("work_queue_entry\n", Depth);
          
            DebugPrint("Type = ", Depth);
            DebugPrint(S.Type, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(work_queue_entry* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("work_queue_entry\n", Depth);
            
              DebugPrint("Type = ", Depth);
              DebugPrint(S->Type, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(meta_func_stream_chunk S, u32 Depth)
        {
          DebugPrint("meta_func_stream_chunk\n", Depth);
          
            DebugPrint("Element = ", Depth);
            DebugPrint(S.Element, Depth+1);
            DebugPrint("\n");

            DebugPrint("Next = ", Depth);
            DebugPrint(S.Next, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(meta_func_stream_chunk* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("meta_func_stream_chunk\n", Depth);
            
              DebugPrint("Element = ", Depth);
              DebugPrint(S->Element, Depth+1);
              DebugPrint("\n");

              DebugPrint("Next = ", Depth);
              DebugPrint(S->Next, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(meta_func_stream S, u32 Depth)
        {
          DebugPrint("meta_func_stream\n", Depth);
          
            DebugPrint("FirstChunk = ", Depth);
            DebugPrint(S.FirstChunk, Depth+1);
            DebugPrint("\n");

            DebugPrint("LastChunk = ", Depth);
            DebugPrint(S.LastChunk, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(meta_func_stream* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("meta_func_stream\n", Depth);
            
              DebugPrint("FirstChunk = ", Depth);
              DebugPrint(S->FirstChunk, Depth+1);
              DebugPrint("\n");

              DebugPrint("LastChunk = ", Depth);
              DebugPrint(S->LastChunk, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(meta_func_iterator S, u32 Depth)
        {
          DebugPrint("meta_func_iterator\n", Depth);
          
            DebugPrint("Stream = ", Depth);
            DebugPrint(S.Stream, Depth+1);
            DebugPrint("\n");

            DebugPrint("At = ", Depth);
            DebugPrint(S.At, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(meta_func_iterator* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("meta_func_iterator\n", Depth);
            
              DebugPrint("Stream = ", Depth);
              DebugPrint(S->Stream, Depth+1);
              DebugPrint("\n");

              DebugPrint("At = ", Depth);
              DebugPrint(S->At, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(client_state S, u32 Depth)
        {
          DebugPrint("client_state\n", Depth);
          
            DebugPrint("Id = ", Depth);
            DebugPrint(S.Id, Depth+1);
            DebugPrint("\n");

            DebugPrint("Counter = ", Depth);
            DebugPrint(S.Counter, Depth+1);
            DebugPrint("\n");

            DebugPrint("P = ", Depth);
            DebugPrint(S.P, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(client_state* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("client_state\n", Depth);
            
              DebugPrint("Id = ", Depth);
              DebugPrint(S->Id, Depth+1);
              DebugPrint("\n");

              DebugPrint("Counter = ", Depth);
              DebugPrint(S->Counter, Depth+1);
              DebugPrint("\n");

              DebugPrint("P = ", Depth);
              DebugPrint(S->P, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(server_state S, u32 Depth)
        {
          DebugPrint("server_state\n", Depth);
          
            DebugPrint("Clients = ", Depth);
            DebugPrint(S.Clients, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(server_state* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("server_state\n", Depth);
            
              DebugPrint("Clients = ", Depth);
              DebugPrint(S->Clients, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(client_to_server_message S, u32 Depth)
        {
          DebugPrint("client_to_server_message\n", Depth);
          
            DebugPrint("Client = ", Depth);
            DebugPrint(S.Client, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(client_to_server_message* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("client_to_server_message\n", Depth);
            
              DebugPrint("Client = ", Depth);
              DebugPrint(S->Client, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(handshake_message S, u32 Depth)
        {
          DebugPrint("handshake_message\n", Depth);
          
            DebugPrint("ClientId = ", Depth);
            DebugPrint(S.ClientId, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(handshake_message* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("handshake_message\n", Depth);
            
              DebugPrint("ClientId = ", Depth);
              DebugPrint(S->ClientId, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(socket_t S, u32 Depth)
        {
          DebugPrint("socket_t\n", Depth);
          
            DebugPrint("Id = ", Depth);
            DebugPrint(S.Id, Depth+1);
            DebugPrint("\n");

            DebugPrint("Type = ", Depth);
            DebugPrint(S.Type, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(socket_t* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("socket_t\n", Depth);
            
              DebugPrint("Id = ", Depth);
              DebugPrint(S->Id, Depth+1);
              DebugPrint("\n");

              DebugPrint("Type = ", Depth);
              DebugPrint(S->Type, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(plane S, u32 Depth)
        {
          DebugPrint("plane\n", Depth);
          
            DebugPrint("P = ", Depth);
            DebugPrint(S.P, Depth+1);
            DebugPrint("\n");

            DebugPrint("Normal = ", Depth);
            DebugPrint(S.Normal, Depth+1);
            DebugPrint("\n");

            DebugPrint("d = ", Depth);
            DebugPrint(S.d, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(plane* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("plane\n", Depth);
            
              DebugPrint("P = ", Depth);
              DebugPrint(S->P, Depth+1);
              DebugPrint("\n");

              DebugPrint("Normal = ", Depth);
              DebugPrint(S->Normal, Depth+1);
              DebugPrint("\n");

              DebugPrint("d = ", Depth);
              DebugPrint(S->d, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(frustum S, u32 Depth)
        {
          DebugPrint("frustum\n", Depth);
          
            DebugPrint("farClip = ", Depth);
            DebugPrint(S.farClip, Depth+1);
            DebugPrint("\n");

            DebugPrint("nearClip = ", Depth);
            DebugPrint(S.nearClip, Depth+1);
            DebugPrint("\n");

            DebugPrint("width = ", Depth);
            DebugPrint(S.width, Depth+1);
            DebugPrint("\n");

            DebugPrint("FOV = ", Depth);
            DebugPrint(S.FOV, Depth+1);
            DebugPrint("\n");

            DebugPrint("Top = ", Depth);
            DebugPrint(S.Top, Depth+1);
            DebugPrint("\n");

            DebugPrint("Bot = ", Depth);
            DebugPrint(S.Bot, Depth+1);
            DebugPrint("\n");

            DebugPrint("Left = ", Depth);
            DebugPrint(S.Left, Depth+1);
            DebugPrint("\n");

            DebugPrint("Right = ", Depth);
            DebugPrint(S.Right, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(frustum* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("frustum\n", Depth);
            
              DebugPrint("farClip = ", Depth);
              DebugPrint(S->farClip, Depth+1);
              DebugPrint("\n");

              DebugPrint("nearClip = ", Depth);
              DebugPrint(S->nearClip, Depth+1);
              DebugPrint("\n");

              DebugPrint("width = ", Depth);
              DebugPrint(S->width, Depth+1);
              DebugPrint("\n");

              DebugPrint("FOV = ", Depth);
              DebugPrint(S->FOV, Depth+1);
              DebugPrint("\n");

              DebugPrint("Top = ", Depth);
              DebugPrint(S->Top, Depth+1);
              DebugPrint("\n");

              DebugPrint("Bot = ", Depth);
              DebugPrint(S->Bot, Depth+1);
              DebugPrint("\n");

              DebugPrint("Left = ", Depth);
              DebugPrint(S->Left, Depth+1);
              DebugPrint("\n");

              DebugPrint("Right = ", Depth);
              DebugPrint(S->Right, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(camera S, u32 Depth)
        {
          DebugPrint("camera\n", Depth);
          
            DebugPrint("Frust = ", Depth);
            DebugPrint(S.Frust, Depth+1);
            DebugPrint("\n");

            DebugPrint("TargetP = ", Depth);
            DebugPrint(S.TargetP, Depth+1);
            DebugPrint("\n");

            DebugPrint("CurrentP = ", Depth);
            DebugPrint(S.CurrentP, Depth+1);
            DebugPrint("\n");

            DebugPrint("ViewingTarget = ", Depth);
            DebugPrint(S.ViewingTarget, Depth+1);
            DebugPrint("\n");

            DebugPrint("RenderSpacePosition = ", Depth);
            DebugPrint(S.RenderSpacePosition, Depth+1);
            DebugPrint("\n");

            DebugPrint("Pitch = ", Depth);
            DebugPrint(S.Pitch, Depth+1);
            DebugPrint("\n");

            DebugPrint("Roll = ", Depth);
            DebugPrint(S.Roll, Depth+1);
            DebugPrint("\n");

            DebugPrint("Yaw = ", Depth);
            DebugPrint(S.Yaw, Depth+1);
            DebugPrint("\n");

            DebugPrint("DistanceFromTarget = ", Depth);
            DebugPrint(S.DistanceFromTarget, Depth+1);
            DebugPrint("\n");

            DebugPrint("Front = ", Depth);
            DebugPrint(S.Front, Depth+1);
            DebugPrint("\n");

            DebugPrint("Right = ", Depth);
            DebugPrint(S.Right, Depth+1);
            DebugPrint("\n");

            DebugPrint("Up = ", Depth);
            DebugPrint(S.Up, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(camera* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("camera\n", Depth);
            
              DebugPrint("Frust = ", Depth);
              DebugPrint(S->Frust, Depth+1);
              DebugPrint("\n");

              DebugPrint("TargetP = ", Depth);
              DebugPrint(S->TargetP, Depth+1);
              DebugPrint("\n");

              DebugPrint("CurrentP = ", Depth);
              DebugPrint(S->CurrentP, Depth+1);
              DebugPrint("\n");

              DebugPrint("ViewingTarget = ", Depth);
              DebugPrint(S->ViewingTarget, Depth+1);
              DebugPrint("\n");

              DebugPrint("RenderSpacePosition = ", Depth);
              DebugPrint(S->RenderSpacePosition, Depth+1);
              DebugPrint("\n");

              DebugPrint("Pitch = ", Depth);
              DebugPrint(S->Pitch, Depth+1);
              DebugPrint("\n");

              DebugPrint("Roll = ", Depth);
              DebugPrint(S->Roll, Depth+1);
              DebugPrint("\n");

              DebugPrint("Yaw = ", Depth);
              DebugPrint(S->Yaw, Depth+1);
              DebugPrint("\n");

              DebugPrint("DistanceFromTarget = ", Depth);
              DebugPrint(S->DistanceFromTarget, Depth+1);
              DebugPrint("\n");

              DebugPrint("Front = ", Depth);
              DebugPrint(S->Front, Depth+1);
              DebugPrint("\n");

              DebugPrint("Right = ", Depth);
              DebugPrint(S->Right, Depth+1);
              DebugPrint("\n");

              DebugPrint("Up = ", Depth);
              DebugPrint(S->Up, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(work_queue_entry_copy_buffer S, u32 Depth)
        {
          DebugPrint("work_queue_entry_copy_buffer\n", Depth);
          
            DebugPrint("Src = ", Depth);
            DebugPrint(S.Src, Depth+1);
            DebugPrint("\n");

            DebugPrint("Dest = ", Depth);
            DebugPrint(S.Dest, Depth+1);
            DebugPrint("\n");

            DebugPrint("Basis = ", Depth);
            DebugPrint(S.Basis, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(work_queue_entry_copy_buffer* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("work_queue_entry_copy_buffer\n", Depth);
            
              DebugPrint("Src = ", Depth);
              DebugPrint(S->Src, Depth+1);
              DebugPrint("\n");

              DebugPrint("Dest = ", Depth);
              DebugPrint(S->Dest, Depth+1);
              DebugPrint("\n");

              DebugPrint("Basis = ", Depth);
              DebugPrint(S->Basis, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(work_queue_entry_init_world_chunk S, u32 Depth)
        {
          DebugPrint("work_queue_entry_init_world_chunk\n", Depth);
          
            DebugPrint("Input = ", Depth);
            DebugPrint(S.Input, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(work_queue_entry_init_world_chunk* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("work_queue_entry_init_world_chunk\n", Depth);
            
              DebugPrint("Input = ", Depth);
              DebugPrint(S->Input, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(edge S, u32 Depth)
        {
          DebugPrint("edge\n", Depth);
          
            DebugPrint("P0 = ", Depth);
            DebugPrint(S.P0, Depth+1);
            DebugPrint("\n");

            DebugPrint("P1 = ", Depth);
            DebugPrint(S.P1, Depth+1);
            DebugPrint("\n");

            DebugPrint("LengthSq = ", Depth);
            DebugPrint(S.LengthSq, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(edge* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("edge\n", Depth);
            
              DebugPrint("P0 = ", Depth);
              DebugPrint(S->P0, Depth+1);
              DebugPrint("\n");

              DebugPrint("P1 = ", Depth);
              DebugPrint(S->P1, Depth+1);
              DebugPrint("\n");

              DebugPrint("LengthSq = ", Depth);
              DebugPrint(S->LengthSq, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(triangle S, u32 Depth)
        {
          DebugPrint("triangle\n", Depth);
          
            DebugPrint("Points = ", Depth);
            DebugPrint(S.Points, Depth+1);
            DebugPrint("\n");

            DebugPrint("Index = ", Depth);
            DebugPrint(S.Index, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(triangle* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("triangle\n", Depth);
            
              DebugPrint("Points = ", Depth);
              DebugPrint(S->Points, Depth+1);
              DebugPrint("\n");

              DebugPrint("Index = ", Depth);
              DebugPrint(S->Index, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(boundary_voxels S, u32 Depth)
        {
          DebugPrint("boundary_voxels\n", Depth);
          
            DebugPrint("Points = ", Depth);
            DebugPrint(S.Points, Depth+1);
            DebugPrint("\n");

            DebugPrint("At = ", Depth);
            DebugPrint(S.At, Depth+1);
            DebugPrint("\n");

            DebugPrint("End = ", Depth);
            DebugPrint(S.End, Depth+1);
            DebugPrint("\n");

            DebugPrint("Min = ", Depth);
            DebugPrint(S.Min, Depth+1);
            DebugPrint("\n");

            DebugPrint("Max = ", Depth);
            DebugPrint(S.Max, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(boundary_voxels* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("boundary_voxels\n", Depth);
            
              DebugPrint("Points = ", Depth);
              DebugPrint(S->Points, Depth+1);
              DebugPrint("\n");

              DebugPrint("At = ", Depth);
              DebugPrint(S->At, Depth+1);
              DebugPrint("\n");

              DebugPrint("End = ", Depth);
              DebugPrint(S->End, Depth+1);
              DebugPrint("\n");

              DebugPrint("Min = ", Depth);
              DebugPrint(S->Min, Depth+1);
              DebugPrint("\n");

              DebugPrint("Max = ", Depth);
              DebugPrint(S->Max, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(current_triangles S, u32 Depth)
        {
          DebugPrint("current_triangles\n", Depth);
          
            DebugPrint("Tris = ", Depth);
            DebugPrint(S.Tris, Depth+1);
            DebugPrint("\n");

            DebugPrint("Max = ", Depth);
            DebugPrint(S.Max, Depth+1);
            DebugPrint("\n");

            DebugPrint("Count = ", Depth);
            DebugPrint(S.Count, Depth+1);
            DebugPrint("\n");

            DebugPrint("CurrentPointIndex = ", Depth);
            DebugPrint(S.CurrentPointIndex, Depth+1);
            DebugPrint("\n");

            DebugPrint("SurfacePoints = ", Depth);
            DebugPrint(S.SurfacePoints, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(current_triangles* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("current_triangles\n", Depth);
            
              DebugPrint("Tris = ", Depth);
              DebugPrint(S->Tris, Depth+1);
              DebugPrint("\n");

              DebugPrint("Max = ", Depth);
              DebugPrint(S->Max, Depth+1);
              DebugPrint("\n");

              DebugPrint("Count = ", Depth);
              DebugPrint(S->Count, Depth+1);
              DebugPrint("\n");

              DebugPrint("CurrentPointIndex = ", Depth);
              DebugPrint(S->CurrentPointIndex, Depth+1);
              DebugPrint("\n");

              DebugPrint("SurfacePoints = ", Depth);
              DebugPrint(S->SurfacePoints, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(canonical_position S, u32 Depth)
        {
          DebugPrint("canonical_position\n", Depth);
          
            DebugPrint("Offset = ", Depth);
            DebugPrint(S.Offset, Depth+1);
            DebugPrint("\n");

            DebugPrint("WorldP = ", Depth);
            DebugPrint(S.WorldP, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(canonical_position* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("canonical_position\n", Depth);
            
              DebugPrint("Offset = ", Depth);
              DebugPrint(S->Offset, Depth+1);
              DebugPrint("\n");

              DebugPrint("WorldP = ", Depth);
              DebugPrint(S->WorldP, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(loaded_collada_mesh S, u32 Depth)
        {
          DebugPrint("loaded_collada_mesh\n", Depth);
          
            DebugPrint("Mesh = ", Depth);
            DebugPrint(S.Mesh, Depth+1);
            DebugPrint("\n");

            DebugPrint("Dim = ", Depth);
            DebugPrint(S.Dim, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(loaded_collada_mesh* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("loaded_collada_mesh\n", Depth);
            
              DebugPrint("Mesh = ", Depth);
              DebugPrint(S->Mesh, Depth+1);
              DebugPrint("\n");

              DebugPrint("Dim = ", Depth);
              DebugPrint(S->Dim, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(free_mesh S, u32 Depth)
        {
          DebugPrint("free_mesh\n", Depth);
          
            DebugPrint("Mesh = ", Depth);
            DebugPrint(S.Mesh, Depth+1);
            DebugPrint("\n");

            DebugPrint("Next = ", Depth);
            DebugPrint(S.Next, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(free_mesh* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("free_mesh\n", Depth);
            
              DebugPrint("Mesh = ", Depth);
              DebugPrint(S->Mesh, Depth+1);
              DebugPrint("\n");

              DebugPrint("Next = ", Depth);
              DebugPrint(S->Next, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(mesh_freelist S, u32 Depth)
        {
          DebugPrint("mesh_freelist\n", Depth);
          
            DebugPrint("FirstFree = ", Depth);
            DebugPrint(S.FirstFree, Depth+1);
            DebugPrint("\n");

            DebugPrint("Containers = ", Depth);
            DebugPrint(S.Containers, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(mesh_freelist* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("mesh_freelist\n", Depth);
            
              DebugPrint("FirstFree = ", Depth);
              DebugPrint(S->FirstFree, Depth+1);
              DebugPrint("\n");

              DebugPrint("Containers = ", Depth);
              DebugPrint(S->Containers, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(xml_property S, u32 Depth)
        {
          DebugPrint("xml_property\n", Depth);
          
            DebugPrint("Name = ", Depth);
            DebugPrint(S.Name, Depth+1);
            DebugPrint("\n");

            DebugPrint("Value = ", Depth);
            DebugPrint(S.Value, Depth+1);
            DebugPrint("\n");

            DebugPrint("Next = ", Depth);
            DebugPrint(S.Next, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(xml_property* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("xml_property\n", Depth);
            
              DebugPrint("Name = ", Depth);
              DebugPrint(S->Name, Depth+1);
              DebugPrint("\n");

              DebugPrint("Value = ", Depth);
              DebugPrint(S->Value, Depth+1);
              DebugPrint("\n");

              DebugPrint("Next = ", Depth);
              DebugPrint(S->Next, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(xml_token S, u32 Depth)
        {
          DebugPrint("xml_token\n", Depth);
          
            DebugPrint("Type = ", Depth);
            DebugPrint(S.Type, Depth+1);
            DebugPrint("\n");

            DebugPrint("Property = ", Depth);
            DebugPrint(S.Property, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(xml_token* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("xml_token\n", Depth);
            
              DebugPrint("Type = ", Depth);
              DebugPrint(S->Type, Depth+1);
              DebugPrint("\n");

              DebugPrint("Property = ", Depth);
              DebugPrint(S->Property, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(xml_tag S, u32 Depth)
        {
          DebugPrint("xml_tag\n", Depth);
          
            DebugPrint("Open = ", Depth);
            DebugPrint(S.Open, Depth+1);
            DebugPrint("\n");

            DebugPrint("Parent = ", Depth);
            DebugPrint(S.Parent, Depth+1);
            DebugPrint("\n");

            DebugPrint("Sibling = ", Depth);
            DebugPrint(S.Sibling, Depth+1);
            DebugPrint("\n");

            DebugPrint("NextInHash = ", Depth);
            DebugPrint(S.NextInHash, Depth+1);
            DebugPrint("\n");

            DebugPrint("HashValue = ", Depth);
            DebugPrint(S.HashValue, Depth+1);
            DebugPrint("\n");

            DebugPrint("Value = ", Depth);
            DebugPrint(S.Value, Depth+1);
            DebugPrint("\n");

            DebugPrint("Properties = ", Depth);
            DebugPrint(S.Properties, Depth+1);
            DebugPrint("\n");

            DebugPrint("NextPropertySlot = ", Depth);
            DebugPrint(S.NextPropertySlot, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(xml_tag* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("xml_tag\n", Depth);
            
              DebugPrint("Open = ", Depth);
              DebugPrint(S->Open, Depth+1);
              DebugPrint("\n");

              DebugPrint("Parent = ", Depth);
              DebugPrint(S->Parent, Depth+1);
              DebugPrint("\n");

              DebugPrint("Sibling = ", Depth);
              DebugPrint(S->Sibling, Depth+1);
              DebugPrint("\n");

              DebugPrint("NextInHash = ", Depth);
              DebugPrint(S->NextInHash, Depth+1);
              DebugPrint("\n");

              DebugPrint("HashValue = ", Depth);
              DebugPrint(S->HashValue, Depth+1);
              DebugPrint("\n");

              DebugPrint("Value = ", Depth);
              DebugPrint(S->Value, Depth+1);
              DebugPrint("\n");

              DebugPrint("Properties = ", Depth);
              DebugPrint(S->Properties, Depth+1);
              DebugPrint("\n");

              DebugPrint("NextPropertySlot = ", Depth);
              DebugPrint(S->NextPropertySlot, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(xml_hashtable S, u32 Depth)
        {
          DebugPrint("xml_hashtable\n", Depth);
          
            DebugPrint("Size = ", Depth);
            DebugPrint(S.Size, Depth+1);
            DebugPrint("\n");

            DebugPrint("Table = ", Depth);
            DebugPrint(S.Table, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(xml_hashtable* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("xml_hashtable\n", Depth);
            
              DebugPrint("Size = ", Depth);
              DebugPrint(S->Size, Depth+1);
              DebugPrint("\n");

              DebugPrint("Table = ", Depth);
              DebugPrint(S->Table, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(xml_token_stream S, u32 Depth)
        {
          DebugPrint("xml_token_stream\n", Depth);
          
            DebugPrint("Start = ", Depth);
            DebugPrint(S.Start, Depth+1);
            DebugPrint("\n");

            DebugPrint("At = ", Depth);
            DebugPrint(S.At, Depth+1);
            DebugPrint("\n");

            DebugPrint("End = ", Depth);
            DebugPrint(S.End, Depth+1);
            DebugPrint("\n");

            DebugPrint("Hashes = ", Depth);
            DebugPrint(S.Hashes, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(xml_token_stream* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("xml_token_stream\n", Depth);
            
              DebugPrint("Start = ", Depth);
              DebugPrint(S->Start, Depth+1);
              DebugPrint("\n");

              DebugPrint("At = ", Depth);
              DebugPrint(S->At, Depth+1);
              DebugPrint("\n");

              DebugPrint("End = ", Depth);
              DebugPrint(S->End, Depth+1);
              DebugPrint("\n");

              DebugPrint("Hashes = ", Depth);
              DebugPrint(S->Hashes, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(xml_tag_stream S, u32 Depth)
        {
          DebugPrint("xml_tag_stream\n", Depth);
          
            DebugPrint("Start = ", Depth);
            DebugPrint(S.Start, Depth+1);
            DebugPrint("\n");

            DebugPrint("At = ", Depth);
            DebugPrint(S.At, Depth+1);
            DebugPrint("\n");

            DebugPrint("End = ", Depth);
            DebugPrint(S.End, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(xml_tag_stream* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("xml_tag_stream\n", Depth);
            
              DebugPrint("Start = ", Depth);
              DebugPrint(S->Start, Depth+1);
              DebugPrint("\n");

              DebugPrint("At = ", Depth);
              DebugPrint(S->At, Depth+1);
              DebugPrint("\n");

              DebugPrint("End = ", Depth);
              DebugPrint(S->End, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(xml_parsing_at_indicators S, u32 Depth)
        {
          DebugPrint("xml_parsing_at_indicators\n", Depth);
          
            DebugPrint("LastClosedTag = ", Depth);
            DebugPrint(S.LastClosedTag, Depth+1);
            DebugPrint("\n");

            DebugPrint("CurrentlyOpenTag = ", Depth);
            DebugPrint(S.CurrentlyOpenTag, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(xml_parsing_at_indicators* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("xml_parsing_at_indicators\n", Depth);
            
              DebugPrint("LastClosedTag = ", Depth);
              DebugPrint(S->LastClosedTag, Depth+1);
              DebugPrint("\n");

              DebugPrint("CurrentlyOpenTag = ", Depth);
              DebugPrint(S->CurrentlyOpenTag, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(post_processing_group S, u32 Depth)
        {
          DebugPrint("post_processing_group\n", Depth);
          
            DebugPrint("FBO = ", Depth);
            DebugPrint(S.FBO, Depth+1);
            DebugPrint("\n");

            DebugPrint("Shader = ", Depth);
            DebugPrint(S.Shader, Depth+1);
            DebugPrint("\n");

            DebugPrint("VertexBuffer = ", Depth);
            DebugPrint(S.VertexBuffer, Depth+1);
            DebugPrint("\n");

            DebugPrint("ColorBuffer = ", Depth);
            DebugPrint(S.ColorBuffer, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(post_processing_group* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("post_processing_group\n", Depth);
            
              DebugPrint("FBO = ", Depth);
              DebugPrint(S->FBO, Depth+1);
              DebugPrint("\n");

              DebugPrint("Shader = ", Depth);
              DebugPrint(S->Shader, Depth+1);
              DebugPrint("\n");

              DebugPrint("VertexBuffer = ", Depth);
              DebugPrint(S->VertexBuffer, Depth+1);
              DebugPrint("\n");

              DebugPrint("ColorBuffer = ", Depth);
              DebugPrint(S->ColorBuffer, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(graphics S, u32 Depth)
        {
          DebugPrint("graphics\n", Depth);
          
            DebugPrint("Camera = ", Depth);
            DebugPrint(S.Camera, Depth+1);
            DebugPrint("\n");

            DebugPrint("Lights = ", Depth);
            DebugPrint(S.Lights, Depth+1);
            DebugPrint("\n");

            DebugPrint("gBuffer = ", Depth);
            DebugPrint(S.gBuffer, Depth+1);
            DebugPrint("\n");

            DebugPrint("AoGroup = ", Depth);
            DebugPrint(S.AoGroup, Depth+1);
            DebugPrint("\n");

            DebugPrint("SG = ", Depth);
            DebugPrint(S.SG, Depth+1);
            DebugPrint("\n");

            DebugPrint("PostGroup = ", Depth);
            DebugPrint(S.PostGroup, Depth+1);
            DebugPrint("\n");

            DebugPrint("GpuBuffers = ", Depth);
            DebugPrint(S.GpuBuffers, Depth+1);
            DebugPrint("\n");

            DebugPrint("GpuBufferWriteIndex = ", Depth);
            DebugPrint(S.GpuBufferWriteIndex, Depth+1);
            DebugPrint("\n");

            DebugPrint("Memory = ", Depth);
            DebugPrint(S.Memory, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(graphics* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("graphics\n", Depth);
            
              DebugPrint("Camera = ", Depth);
              DebugPrint(S->Camera, Depth+1);
              DebugPrint("\n");

              DebugPrint("Lights = ", Depth);
              DebugPrint(S->Lights, Depth+1);
              DebugPrint("\n");

              DebugPrint("gBuffer = ", Depth);
              DebugPrint(S->gBuffer, Depth+1);
              DebugPrint("\n");

              DebugPrint("AoGroup = ", Depth);
              DebugPrint(S->AoGroup, Depth+1);
              DebugPrint("\n");

              DebugPrint("SG = ", Depth);
              DebugPrint(S->SG, Depth+1);
              DebugPrint("\n");

              DebugPrint("PostGroup = ", Depth);
              DebugPrint(S->PostGroup, Depth+1);
              DebugPrint("\n");

              DebugPrint("GpuBuffers = ", Depth);
              DebugPrint(S->GpuBuffers, Depth+1);
              DebugPrint("\n");

              DebugPrint("GpuBufferWriteIndex = ", Depth);
              DebugPrint(S->GpuBufferWriteIndex, Depth+1);
              DebugPrint("\n");

              DebugPrint("Memory = ", Depth);
              DebugPrint(S->Memory, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(shader_uniform S, u32 Depth)
        {
          DebugPrint("shader_uniform\n", Depth);
          
            DebugPrint("Type = ", Depth);
            DebugPrint(S.Type, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(shader_uniform* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("shader_uniform\n", Depth);
            
              DebugPrint("Type = ", Depth);
              DebugPrint(S->Type, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(shader S, u32 Depth)
        {
          DebugPrint("shader\n", Depth);
          
            DebugPrint("ID = ", Depth);
            DebugPrint(S.ID, Depth+1);
            DebugPrint("\n");

            DebugPrint("FirstUniform = ", Depth);
            DebugPrint(S.FirstUniform, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(shader* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("shader\n", Depth);
            
              DebugPrint("ID = ", Depth);
              DebugPrint(S->ID, Depth+1);
              DebugPrint("\n");

              DebugPrint("FirstUniform = ", Depth);
              DebugPrint(S->FirstUniform, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(texture S, u32 Depth)
        {
          DebugPrint("texture\n", Depth);
          
            DebugPrint("ID = ", Depth);
            DebugPrint(S.ID, Depth+1);
            DebugPrint("\n");

            DebugPrint("Dim = ", Depth);
            DebugPrint(S.Dim, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(texture* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("texture\n", Depth);
            
              DebugPrint("ID = ", Depth);
              DebugPrint(S->ID, Depth+1);
              DebugPrint("\n");

              DebugPrint("Dim = ", Depth);
              DebugPrint(S->Dim, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(light S, u32 Depth)
        {
          DebugPrint("light\n", Depth);
          
            DebugPrint("Position = ", Depth);
            DebugPrint(S.Position, Depth+1);
            DebugPrint("\n");

            DebugPrint("Color = ", Depth);
            DebugPrint(S.Color, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(light* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("light\n", Depth);
            
              DebugPrint("Position = ", Depth);
              DebugPrint(S->Position, Depth+1);
              DebugPrint("\n");

              DebugPrint("Color = ", Depth);
              DebugPrint(S->Color, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(game_lights S, u32 Depth)
        {
          DebugPrint("game_lights\n", Depth);
          
            DebugPrint("ColorTex = ", Depth);
            DebugPrint(S.ColorTex, Depth+1);
            DebugPrint("\n");

            DebugPrint("PositionTex = ", Depth);
            DebugPrint(S.PositionTex, Depth+1);
            DebugPrint("\n");

            DebugPrint("IndexToUV = ", Depth);
            DebugPrint(S.IndexToUV, Depth+1);
            DebugPrint("\n");

            DebugPrint("Count = ", Depth);
            DebugPrint(S.Count, Depth+1);
            DebugPrint("\n");

            DebugPrint("Lights = ", Depth);
            DebugPrint(S.Lights, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(game_lights* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("game_lights\n", Depth);
            
              DebugPrint("ColorTex = ", Depth);
              DebugPrint(S->ColorTex, Depth+1);
              DebugPrint("\n");

              DebugPrint("PositionTex = ", Depth);
              DebugPrint(S->PositionTex, Depth+1);
              DebugPrint("\n");

              DebugPrint("IndexToUV = ", Depth);
              DebugPrint(S->IndexToUV, Depth+1);
              DebugPrint("\n");

              DebugPrint("Count = ", Depth);
              DebugPrint(S->Count, Depth+1);
              DebugPrint("\n");

              DebugPrint("Lights = ", Depth);
              DebugPrint(S->Lights, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(RenderBasis S, u32 Depth)
        {
          DebugPrint("RenderBasis\n", Depth);
          
            DebugPrint("ModelMatrix = ", Depth);
            DebugPrint(S.ModelMatrix, Depth+1);
            DebugPrint("\n");

            DebugPrint("ViewMatrix = ", Depth);
            DebugPrint(S.ViewMatrix, Depth+1);
            DebugPrint("\n");

            DebugPrint("ProjectionMatrix = ", Depth);
            DebugPrint(S.ProjectionMatrix, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(RenderBasis* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("RenderBasis\n", Depth);
            
              DebugPrint("ModelMatrix = ", Depth);
              DebugPrint(S->ModelMatrix, Depth+1);
              DebugPrint("\n");

              DebugPrint("ViewMatrix = ", Depth);
              DebugPrint(S->ViewMatrix, Depth+1);
              DebugPrint("\n");

              DebugPrint("ProjectionMatrix = ", Depth);
              DebugPrint(S->ProjectionMatrix, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(framebuffer S, u32 Depth)
        {
          DebugPrint("framebuffer\n", Depth);
          
            DebugPrint("ID = ", Depth);
            DebugPrint(S.ID, Depth+1);
            DebugPrint("\n");

            DebugPrint("Attachments = ", Depth);
            DebugPrint(S.Attachments, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(framebuffer* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("framebuffer\n", Depth);
            
              DebugPrint("ID = ", Depth);
              DebugPrint(S->ID, Depth+1);
              DebugPrint("\n");

              DebugPrint("Attachments = ", Depth);
              DebugPrint(S->Attachments, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(ao_render_group S, u32 Depth)
        {
          DebugPrint("ao_render_group\n", Depth);
          
            DebugPrint("FBO = ", Depth);
            DebugPrint(S.FBO, Depth+1);
            DebugPrint("\n");

            DebugPrint("Shader = ", Depth);
            DebugPrint(S.Shader, Depth+1);
            DebugPrint("\n");

            DebugPrint("DebugSsaoShader = ", Depth);
            DebugPrint(S.DebugSsaoShader, Depth+1);
            DebugPrint("\n");

            DebugPrint("SsaoKernel = ", Depth);
            DebugPrint(S.SsaoKernel, Depth+1);
            DebugPrint("\n");

            DebugPrint("SsaoKernelUniform = ", Depth);
            DebugPrint(S.SsaoKernelUniform, Depth+1);
            DebugPrint("\n");

            DebugPrint("Texture = ", Depth);
            DebugPrint(S.Texture, Depth+1);
            DebugPrint("\n");

            DebugPrint("NoiseTile = ", Depth);
            DebugPrint(S.NoiseTile, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(ao_render_group* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("ao_render_group\n", Depth);
            
              DebugPrint("FBO = ", Depth);
              DebugPrint(S->FBO, Depth+1);
              DebugPrint("\n");

              DebugPrint("Shader = ", Depth);
              DebugPrint(S->Shader, Depth+1);
              DebugPrint("\n");

              DebugPrint("DebugSsaoShader = ", Depth);
              DebugPrint(S->DebugSsaoShader, Depth+1);
              DebugPrint("\n");

              DebugPrint("SsaoKernel = ", Depth);
              DebugPrint(S->SsaoKernel, Depth+1);
              DebugPrint("\n");

              DebugPrint("SsaoKernelUniform = ", Depth);
              DebugPrint(S->SsaoKernelUniform, Depth+1);
              DebugPrint("\n");

              DebugPrint("Texture = ", Depth);
              DebugPrint(S->Texture, Depth+1);
              DebugPrint("\n");

              DebugPrint("NoiseTile = ", Depth);
              DebugPrint(S->NoiseTile, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(g_buffer_textures S, u32 Depth)
        {
          DebugPrint("g_buffer_textures\n", Depth);
          
            DebugPrint("Color = ", Depth);
            DebugPrint(S.Color, Depth+1);
            DebugPrint("\n");

            DebugPrint("Normal = ", Depth);
            DebugPrint(S.Normal, Depth+1);
            DebugPrint("\n");

            DebugPrint("Position = ", Depth);
            DebugPrint(S.Position, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(g_buffer_textures* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("g_buffer_textures\n", Depth);
            
              DebugPrint("Color = ", Depth);
              DebugPrint(S->Color, Depth+1);
              DebugPrint("\n");

              DebugPrint("Normal = ", Depth);
              DebugPrint(S->Normal, Depth+1);
              DebugPrint("\n");

              DebugPrint("Position = ", Depth);
              DebugPrint(S->Position, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(g_buffer_render_group S, u32 Depth)
        {
          DebugPrint("g_buffer_render_group\n", Depth);
          
            DebugPrint("FBO = ", Depth);
            DebugPrint(S.FBO, Depth+1);
            DebugPrint("\n");

            DebugPrint("Textures = ", Depth);
            DebugPrint(S.Textures, Depth+1);
            DebugPrint("\n");

            DebugPrint("LightingShader = ", Depth);
            DebugPrint(S.LightingShader, Depth+1);
            DebugPrint("\n");

            DebugPrint("gBufferShader = ", Depth);
            DebugPrint(S.gBufferShader, Depth+1);
            DebugPrint("\n");

            DebugPrint("ShadowMVP = ", Depth);
            DebugPrint(S.ShadowMVP, Depth+1);
            DebugPrint("\n");

            DebugPrint("ViewProjection = ", Depth);
            DebugPrint(S.ViewProjection, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(g_buffer_render_group* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("g_buffer_render_group\n", Depth);
            
              DebugPrint("FBO = ", Depth);
              DebugPrint(S->FBO, Depth+1);
              DebugPrint("\n");

              DebugPrint("Textures = ", Depth);
              DebugPrint(S->Textures, Depth+1);
              DebugPrint("\n");

              DebugPrint("LightingShader = ", Depth);
              DebugPrint(S->LightingShader, Depth+1);
              DebugPrint("\n");

              DebugPrint("gBufferShader = ", Depth);
              DebugPrint(S->gBufferShader, Depth+1);
              DebugPrint("\n");

              DebugPrint("ShadowMVP = ", Depth);
              DebugPrint(S->ShadowMVP, Depth+1);
              DebugPrint("\n");

              DebugPrint("ViewProjection = ", Depth);
              DebugPrint(S->ViewProjection, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(shadow_render_group S, u32 Depth)
        {
          DebugPrint("shadow_render_group\n", Depth);
          
            DebugPrint("FramebufferName = ", Depth);
            DebugPrint(S.FramebufferName, Depth+1);
            DebugPrint("\n");

            DebugPrint("MVP_ID = ", Depth);
            DebugPrint(S.MVP_ID, Depth+1);
            DebugPrint("\n");

            DebugPrint("DebugTextureShader = ", Depth);
            DebugPrint(S.DebugTextureShader, Depth+1);
            DebugPrint("\n");

            DebugPrint("DepthShader = ", Depth);
            DebugPrint(S.DepthShader, Depth+1);
            DebugPrint("\n");

            DebugPrint("ShadowMap = ", Depth);
            DebugPrint(S.ShadowMap, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(shadow_render_group* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("shadow_render_group\n", Depth);
            
              DebugPrint("FramebufferName = ", Depth);
              DebugPrint(S->FramebufferName, Depth+1);
              DebugPrint("\n");

              DebugPrint("MVP_ID = ", Depth);
              DebugPrint(S->MVP_ID, Depth+1);
              DebugPrint("\n");

              DebugPrint("DebugTextureShader = ", Depth);
              DebugPrint(S->DebugTextureShader, Depth+1);
              DebugPrint("\n");

              DebugPrint("DepthShader = ", Depth);
              DebugPrint(S->DepthShader, Depth+1);
              DebugPrint("\n");

              DebugPrint("ShadowMap = ", Depth);
              DebugPrint(S->ShadowMap, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(untextured_3d_geometry_buffer S, u32 Depth)
        {
          DebugPrint("untextured_3d_geometry_buffer\n", Depth);
          
            DebugPrint("Verts = ", Depth);
            DebugPrint(S.Verts, Depth+1);
            DebugPrint("\n");

            DebugPrint("Colors = ", Depth);
            DebugPrint(S.Colors, Depth+1);
            DebugPrint("\n");

            DebugPrint("Normals = ", Depth);
            DebugPrint(S.Normals, Depth+1);
            DebugPrint("\n");

            DebugPrint("End = ", Depth);
            DebugPrint(S.End, Depth+1);
            DebugPrint("\n");

            DebugPrint("At = ", Depth);
            DebugPrint(S.At, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(untextured_3d_geometry_buffer* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("untextured_3d_geometry_buffer\n", Depth);
            
              DebugPrint("Verts = ", Depth);
              DebugPrint(S->Verts, Depth+1);
              DebugPrint("\n");

              DebugPrint("Colors = ", Depth);
              DebugPrint(S->Colors, Depth+1);
              DebugPrint("\n");

              DebugPrint("Normals = ", Depth);
              DebugPrint(S->Normals, Depth+1);
              DebugPrint("\n");

              DebugPrint("End = ", Depth);
              DebugPrint(S->End, Depth+1);
              DebugPrint("\n");

              DebugPrint("At = ", Depth);
              DebugPrint(S->At, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(gpu_mapped_element_buffer S, u32 Depth)
        {
          DebugPrint("gpu_mapped_element_buffer\n", Depth);
          
            DebugPrint("VertexHandle = ", Depth);
            DebugPrint(S.VertexHandle, Depth+1);
            DebugPrint("\n");

            DebugPrint("NormalHandle = ", Depth);
            DebugPrint(S.NormalHandle, Depth+1);
            DebugPrint("\n");

            DebugPrint("ColorHandle = ", Depth);
            DebugPrint(S.ColorHandle, Depth+1);
            DebugPrint("\n");

            DebugPrint("Buffer = ", Depth);
            DebugPrint(S.Buffer, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(gpu_mapped_element_buffer* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("gpu_mapped_element_buffer\n", Depth);
            
              DebugPrint("VertexHandle = ", Depth);
              DebugPrint(S->VertexHandle, Depth+1);
              DebugPrint("\n");

              DebugPrint("NormalHandle = ", Depth);
              DebugPrint(S->NormalHandle, Depth+1);
              DebugPrint("\n");

              DebugPrint("ColorHandle = ", Depth);
              DebugPrint(S->ColorHandle, Depth+1);
              DebugPrint("\n");

              DebugPrint("Buffer = ", Depth);
              DebugPrint(S->Buffer, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(textured_2d_geometry_buffer S, u32 Depth)
        {
          DebugPrint("textured_2d_geometry_buffer\n", Depth);
          
            DebugPrint("Verts = ", Depth);
            DebugPrint(S.Verts, Depth+1);
            DebugPrint("\n");

            DebugPrint("Colors = ", Depth);
            DebugPrint(S.Colors, Depth+1);
            DebugPrint("\n");

            DebugPrint("UVs = ", Depth);
            DebugPrint(S.UVs, Depth+1);
            DebugPrint("\n");

            DebugPrint("End = ", Depth);
            DebugPrint(S.End, Depth+1);
            DebugPrint("\n");

            DebugPrint("At = ", Depth);
            DebugPrint(S.At, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(textured_2d_geometry_buffer* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("textured_2d_geometry_buffer\n", Depth);
            
              DebugPrint("Verts = ", Depth);
              DebugPrint(S->Verts, Depth+1);
              DebugPrint("\n");

              DebugPrint("Colors = ", Depth);
              DebugPrint(S->Colors, Depth+1);
              DebugPrint("\n");

              DebugPrint("UVs = ", Depth);
              DebugPrint(S->UVs, Depth+1);
              DebugPrint("\n");

              DebugPrint("End = ", Depth);
              DebugPrint(S->End, Depth+1);
              DebugPrint("\n");

              DebugPrint("At = ", Depth);
              DebugPrint(S->At, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(untextured_2d_geometry_buffer S, u32 Depth)
        {
          DebugPrint("untextured_2d_geometry_buffer\n", Depth);
          
            DebugPrint("Verts = ", Depth);
            DebugPrint(S.Verts, Depth+1);
            DebugPrint("\n");

            DebugPrint("Colors = ", Depth);
            DebugPrint(S.Colors, Depth+1);
            DebugPrint("\n");

            DebugPrint("End = ", Depth);
            DebugPrint(S.End, Depth+1);
            DebugPrint("\n");

            DebugPrint("At = ", Depth);
            DebugPrint(S.At, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(untextured_2d_geometry_buffer* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("untextured_2d_geometry_buffer\n", Depth);
            
              DebugPrint("Verts = ", Depth);
              DebugPrint(S->Verts, Depth+1);
              DebugPrint("\n");

              DebugPrint("Colors = ", Depth);
              DebugPrint(S->Colors, Depth+1);
              DebugPrint("\n");

              DebugPrint("End = ", Depth);
              DebugPrint(S->End, Depth+1);
              DebugPrint("\n");

              DebugPrint("At = ", Depth);
              DebugPrint(S->At, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(keyframe S, u32 Depth)
        {
          DebugPrint("keyframe\n", Depth);
          
            DebugPrint("tEnd = ", Depth);
            DebugPrint(S.tEnd, Depth+1);
            DebugPrint("\n");

            DebugPrint("Value = ", Depth);
            DebugPrint(S.Value, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(keyframe* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("keyframe\n", Depth);
            
              DebugPrint("tEnd = ", Depth);
              DebugPrint(S->tEnd, Depth+1);
              DebugPrint("\n");

              DebugPrint("Value = ", Depth);
              DebugPrint(S->Value, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(animation S, u32 Depth)
        {
          DebugPrint("animation\n", Depth);
          
            DebugPrint("t = ", Depth);
            DebugPrint(S.t, Depth+1);
            DebugPrint("\n");

            DebugPrint("tEnd = ", Depth);
            DebugPrint(S.tEnd, Depth+1);
            DebugPrint("\n");

            DebugPrint("xKeyframeCount = ", Depth);
            DebugPrint(S.xKeyframeCount, Depth+1);
            DebugPrint("\n");

            DebugPrint("xKeyframes = ", Depth);
            DebugPrint(S.xKeyframes, Depth+1);
            DebugPrint("\n");

            DebugPrint("yKeyframeCount = ", Depth);
            DebugPrint(S.yKeyframeCount, Depth+1);
            DebugPrint("\n");

            DebugPrint("yKeyframes = ", Depth);
            DebugPrint(S.yKeyframes, Depth+1);
            DebugPrint("\n");

            DebugPrint("zKeyframeCount = ", Depth);
            DebugPrint(S.zKeyframeCount, Depth+1);
            DebugPrint("\n");

            DebugPrint("zKeyframes = ", Depth);
            DebugPrint(S.zKeyframes, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(animation* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("animation\n", Depth);
            
              DebugPrint("t = ", Depth);
              DebugPrint(S->t, Depth+1);
              DebugPrint("\n");

              DebugPrint("tEnd = ", Depth);
              DebugPrint(S->tEnd, Depth+1);
              DebugPrint("\n");

              DebugPrint("xKeyframeCount = ", Depth);
              DebugPrint(S->xKeyframeCount, Depth+1);
              DebugPrint("\n");

              DebugPrint("xKeyframes = ", Depth);
              DebugPrint(S->xKeyframes, Depth+1);
              DebugPrint("\n");

              DebugPrint("yKeyframeCount = ", Depth);
              DebugPrint(S->yKeyframeCount, Depth+1);
              DebugPrint("\n");

              DebugPrint("yKeyframes = ", Depth);
              DebugPrint(S->yKeyframes, Depth+1);
              DebugPrint("\n");

              DebugPrint("zKeyframeCount = ", Depth);
              DebugPrint(S->zKeyframeCount, Depth+1);
              DebugPrint("\n");

              DebugPrint("zKeyframes = ", Depth);
              DebugPrint(S->zKeyframes, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(game_mode S, u32 Depth)
        {
          DebugPrint("game_mode\n", Depth);
          
            DebugPrint("ActiveMode = ", Depth);
            DebugPrint(S.ActiveMode, Depth+1);
            DebugPrint("\n");

            DebugPrint("TimeRunning = ", Depth);
            DebugPrint(S.TimeRunning, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(game_mode* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("game_mode\n", Depth);
            
              DebugPrint("ActiveMode = ", Depth);
              DebugPrint(S->ActiveMode, Depth+1);
              DebugPrint("\n");

              DebugPrint("TimeRunning = ", Depth);
              DebugPrint(S->TimeRunning, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(voxel S, u32 Depth)
        {
          DebugPrint("voxel\n", Depth);
          
            DebugPrint("Flags = ", Depth);
            DebugPrint(S.Flags, Depth+1);
            DebugPrint("\n");

            DebugPrint("Color = ", Depth);
            DebugPrint(S.Color, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(voxel* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("voxel\n", Depth);
            
              DebugPrint("Flags = ", Depth);
              DebugPrint(S->Flags, Depth+1);
              DebugPrint("\n");

              DebugPrint("Color = ", Depth);
              DebugPrint(S->Color, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(boundary_voxel S, u32 Depth)
        {
          DebugPrint("boundary_voxel\n", Depth);
          
            DebugPrint("V = ", Depth);
            DebugPrint(S.V, Depth+1);
            DebugPrint("\n");

            DebugPrint("Offset = ", Depth);
            DebugPrint(S.Offset, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(boundary_voxel* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("boundary_voxel\n", Depth);
            
              DebugPrint("V = ", Depth);
              DebugPrint(S->V, Depth+1);
              DebugPrint("\n");

              DebugPrint("Offset = ", Depth);
              DebugPrint(S->Offset, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(chunk_data S, u32 Depth)
        {
          DebugPrint("chunk_data\n", Depth);
          
            DebugPrint("Flags = ", Depth);
            DebugPrint(S.Flags, Depth+1);
            DebugPrint("\n");

            DebugPrint("Voxels = ", Depth);
            DebugPrint(S.Voxels, Depth+1);
            DebugPrint("\n");

            DebugPrint("Pad = ", Depth);
            DebugPrint(S.Pad, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(chunk_data* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("chunk_data\n", Depth);
            
              DebugPrint("Flags = ", Depth);
              DebugPrint(S->Flags, Depth+1);
              DebugPrint("\n");

              DebugPrint("Voxels = ", Depth);
              DebugPrint(S->Voxels, Depth+1);
              DebugPrint("\n");

              DebugPrint("Pad = ", Depth);
              DebugPrint(S->Pad, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(model S, u32 Depth)
        {
          DebugPrint("model\n", Depth);
          
            DebugPrint("Mesh = ", Depth);
            DebugPrint(S.Mesh, Depth+1);
            DebugPrint("\n");

            DebugPrint("Dim = ", Depth);
            DebugPrint(S.Dim, Depth+1);
            DebugPrint("\n");

            DebugPrint("Animation = ", Depth);
            DebugPrint(S.Animation, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(model* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("model\n", Depth);
            
              DebugPrint("Mesh = ", Depth);
              DebugPrint(S->Mesh, Depth+1);
              DebugPrint("\n");

              DebugPrint("Dim = ", Depth);
              DebugPrint(S->Dim, Depth+1);
              DebugPrint("\n");

              DebugPrint("Animation = ", Depth);
              DebugPrint(S->Animation, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(physics S, u32 Depth)
        {
          DebugPrint("physics\n", Depth);
          
            DebugPrint("Velocity = ", Depth);
            DebugPrint(S.Velocity, Depth+1);
            DebugPrint("\n");

            DebugPrint("Force = ", Depth);
            DebugPrint(S.Force, Depth+1);
            DebugPrint("\n");

            DebugPrint("Delta = ", Depth);
            DebugPrint(S.Delta, Depth+1);
            DebugPrint("\n");

            DebugPrint("Drag = ", Depth);
            DebugPrint(S.Drag, Depth+1);
            DebugPrint("\n");

            DebugPrint("Mass = ", Depth);
            DebugPrint(S.Mass, Depth+1);
            DebugPrint("\n");

            DebugPrint("Speed = ", Depth);
            DebugPrint(S.Speed, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(physics* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("physics\n", Depth);
            
              DebugPrint("Velocity = ", Depth);
              DebugPrint(S->Velocity, Depth+1);
              DebugPrint("\n");

              DebugPrint("Force = ", Depth);
              DebugPrint(S->Force, Depth+1);
              DebugPrint("\n");

              DebugPrint("Delta = ", Depth);
              DebugPrint(S->Delta, Depth+1);
              DebugPrint("\n");

              DebugPrint("Drag = ", Depth);
              DebugPrint(S->Drag, Depth+1);
              DebugPrint("\n");

              DebugPrint("Mass = ", Depth);
              DebugPrint(S->Mass, Depth+1);
              DebugPrint("\n");

              DebugPrint("Speed = ", Depth);
              DebugPrint(S->Speed, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(particle S, u32 Depth)
        {
          DebugPrint("particle\n", Depth);
          
            DebugPrint("Offset = ", Depth);
            DebugPrint(S.Offset, Depth+1);
            DebugPrint("\n");

            DebugPrint("Physics = ", Depth);
            DebugPrint(S.Physics, Depth+1);
            DebugPrint("\n");

            DebugPrint("Color = ", Depth);
            DebugPrint(S.Color, Depth+1);
            DebugPrint("\n");

            DebugPrint("RemainingLifespan = ", Depth);
            DebugPrint(S.RemainingLifespan, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(particle* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("particle\n", Depth);
            
              DebugPrint("Offset = ", Depth);
              DebugPrint(S->Offset, Depth+1);
              DebugPrint("\n");

              DebugPrint("Physics = ", Depth);
              DebugPrint(S->Physics, Depth+1);
              DebugPrint("\n");

              DebugPrint("Color = ", Depth);
              DebugPrint(S->Color, Depth+1);
              DebugPrint("\n");

              DebugPrint("RemainingLifespan = ", Depth);
              DebugPrint(S->RemainingLifespan, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(particle_system_init_params S, u32 Depth)
        {
          DebugPrint("particle_system_init_params\n", Depth);
          
            DebugPrint("SpawnRegion = ", Depth);
            DebugPrint(S.SpawnRegion, Depth+1);
            DebugPrint("\n");

            DebugPrint("Physics = ", Depth);
            DebugPrint(S.Physics, Depth+1);
            DebugPrint("\n");

            DebugPrint("SystemMovementCoefficient = ", Depth);
            DebugPrint(S.SystemMovementCoefficient, Depth+1);
            DebugPrint("\n");

            DebugPrint("EmissionLifespan = ", Depth);
            DebugPrint(S.EmissionLifespan, Depth+1);
            DebugPrint("\n");

            DebugPrint("EmissionChance = ", Depth);
            DebugPrint(S.EmissionChance, Depth+1);
            DebugPrint("\n");

            DebugPrint("ParticleLifespan = ", Depth);
            DebugPrint(S.ParticleLifespan, Depth+1);
            DebugPrint("\n");

            DebugPrint("StartingDiameter = ", Depth);
            DebugPrint(S.StartingDiameter, Depth+1);
            DebugPrint("\n");

            DebugPrint("Entropy = ", Depth);
            DebugPrint(S.Entropy, Depth+1);
            DebugPrint("\n");

            DebugPrint("Colors = ", Depth);
            DebugPrint(S.Colors, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(particle_system_init_params* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("particle_system_init_params\n", Depth);
            
              DebugPrint("SpawnRegion = ", Depth);
              DebugPrint(S->SpawnRegion, Depth+1);
              DebugPrint("\n");

              DebugPrint("Physics = ", Depth);
              DebugPrint(S->Physics, Depth+1);
              DebugPrint("\n");

              DebugPrint("SystemMovementCoefficient = ", Depth);
              DebugPrint(S->SystemMovementCoefficient, Depth+1);
              DebugPrint("\n");

              DebugPrint("EmissionLifespan = ", Depth);
              DebugPrint(S->EmissionLifespan, Depth+1);
              DebugPrint("\n");

              DebugPrint("EmissionChance = ", Depth);
              DebugPrint(S->EmissionChance, Depth+1);
              DebugPrint("\n");

              DebugPrint("ParticleLifespan = ", Depth);
              DebugPrint(S->ParticleLifespan, Depth+1);
              DebugPrint("\n");

              DebugPrint("StartingDiameter = ", Depth);
              DebugPrint(S->StartingDiameter, Depth+1);
              DebugPrint("\n");

              DebugPrint("Entropy = ", Depth);
              DebugPrint(S->Entropy, Depth+1);
              DebugPrint("\n");

              DebugPrint("Colors = ", Depth);
              DebugPrint(S->Colors, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(particle_system S, u32 Depth)
        {
          DebugPrint("particle_system\n", Depth);
          
            DebugPrint("Entropy = ", Depth);
            DebugPrint(S.Entropy, Depth+1);
            DebugPrint("\n");

            DebugPrint("ActiveParticles = ", Depth);
            DebugPrint(S.ActiveParticles, Depth+1);
            DebugPrint("\n");

            DebugPrint("ParticleDuration = ", Depth);
            DebugPrint(S.ParticleDuration, Depth+1);
            DebugPrint("\n");

            DebugPrint("SpawnRegion = ", Depth);
            DebugPrint(S.SpawnRegion, Depth+1);
            DebugPrint("\n");

            DebugPrint("StartingDiameter = ", Depth);
            DebugPrint(S.StartingDiameter, Depth+1);
            DebugPrint("\n");

            DebugPrint("EmissionLifespan = ", Depth);
            DebugPrint(S.EmissionLifespan, Depth+1);
            DebugPrint("\n");

            DebugPrint("ParticleLifespan = ", Depth);
            DebugPrint(S.ParticleLifespan, Depth+1);
            DebugPrint("\n");

            DebugPrint("EmissionChance = ", Depth);
            DebugPrint(S.EmissionChance, Depth+1);
            DebugPrint("\n");

            DebugPrint("ParticlePhysics = ", Depth);
            DebugPrint(S.ParticlePhysics, Depth+1);
            DebugPrint("\n");

            DebugPrint("SystemMovementCoefficient = ", Depth);
            DebugPrint(S.SystemMovementCoefficient, Depth+1);
            DebugPrint("\n");

            DebugPrint("Colors = ", Depth);
            DebugPrint(S.Colors, Depth+1);
            DebugPrint("\n");

            DebugPrint("Particles = ", Depth);
            DebugPrint(S.Particles, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(particle_system* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("particle_system\n", Depth);
            
              DebugPrint("Entropy = ", Depth);
              DebugPrint(S->Entropy, Depth+1);
              DebugPrint("\n");

              DebugPrint("ActiveParticles = ", Depth);
              DebugPrint(S->ActiveParticles, Depth+1);
              DebugPrint("\n");

              DebugPrint("ParticleDuration = ", Depth);
              DebugPrint(S->ParticleDuration, Depth+1);
              DebugPrint("\n");

              DebugPrint("SpawnRegion = ", Depth);
              DebugPrint(S->SpawnRegion, Depth+1);
              DebugPrint("\n");

              DebugPrint("StartingDiameter = ", Depth);
              DebugPrint(S->StartingDiameter, Depth+1);
              DebugPrint("\n");

              DebugPrint("EmissionLifespan = ", Depth);
              DebugPrint(S->EmissionLifespan, Depth+1);
              DebugPrint("\n");

              DebugPrint("ParticleLifespan = ", Depth);
              DebugPrint(S->ParticleLifespan, Depth+1);
              DebugPrint("\n");

              DebugPrint("EmissionChance = ", Depth);
              DebugPrint(S->EmissionChance, Depth+1);
              DebugPrint("\n");

              DebugPrint("ParticlePhysics = ", Depth);
              DebugPrint(S->ParticlePhysics, Depth+1);
              DebugPrint("\n");

              DebugPrint("SystemMovementCoefficient = ", Depth);
              DebugPrint(S->SystemMovementCoefficient, Depth+1);
              DebugPrint("\n");

              DebugPrint("Colors = ", Depth);
              DebugPrint(S->Colors, Depth+1);
              DebugPrint("\n");

              DebugPrint("Particles = ", Depth);
              DebugPrint(S->Particles, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(entity S, u32 Depth)
        {
          DebugPrint("entity\n", Depth);
          
            DebugPrint("Model = ", Depth);
            DebugPrint(S.Model, Depth+1);
            DebugPrint("\n");

            DebugPrint("CollisionVolumeRadius = ", Depth);
            DebugPrint(S.CollisionVolumeRadius, Depth+1);
            DebugPrint("\n");

            DebugPrint("Emitter = ", Depth);
            DebugPrint(S.Emitter, Depth+1);
            DebugPrint("\n");

            DebugPrint("Physics = ", Depth);
            DebugPrint(S.Physics, Depth+1);
            DebugPrint("\n");

            DebugPrint("P = ", Depth);
            DebugPrint(S.P, Depth+1);
            DebugPrint("\n");

            DebugPrint("Rotation = ", Depth);
            DebugPrint(S.Rotation, Depth+1);
            DebugPrint("\n");

            DebugPrint("State = ", Depth);
            DebugPrint(S.State, Depth+1);
            DebugPrint("\n");

            DebugPrint("Type = ", Depth);
            DebugPrint(S.Type, Depth+1);
            DebugPrint("\n");

            DebugPrint("Scale = ", Depth);
            DebugPrint(S.Scale, Depth+1);
            DebugPrint("\n");

            DebugPrint("RateOfFire = ", Depth);
            DebugPrint(S.RateOfFire, Depth+1);
            DebugPrint("\n");

            DebugPrint("FireCooldown = ", Depth);
            DebugPrint(S.FireCooldown, Depth+1);
            DebugPrint("\n");

            DebugPrint("Health = ", Depth);
            DebugPrint(S.Health, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(entity* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("entity\n", Depth);
            
              DebugPrint("Model = ", Depth);
              DebugPrint(S->Model, Depth+1);
              DebugPrint("\n");

              DebugPrint("CollisionVolumeRadius = ", Depth);
              DebugPrint(S->CollisionVolumeRadius, Depth+1);
              DebugPrint("\n");

              DebugPrint("Emitter = ", Depth);
              DebugPrint(S->Emitter, Depth+1);
              DebugPrint("\n");

              DebugPrint("Physics = ", Depth);
              DebugPrint(S->Physics, Depth+1);
              DebugPrint("\n");

              DebugPrint("P = ", Depth);
              DebugPrint(S->P, Depth+1);
              DebugPrint("\n");

              DebugPrint("Rotation = ", Depth);
              DebugPrint(S->Rotation, Depth+1);
              DebugPrint("\n");

              DebugPrint("State = ", Depth);
              DebugPrint(S->State, Depth+1);
              DebugPrint("\n");

              DebugPrint("Type = ", Depth);
              DebugPrint(S->Type, Depth+1);
              DebugPrint("\n");

              DebugPrint("Scale = ", Depth);
              DebugPrint(S->Scale, Depth+1);
              DebugPrint("\n");

              DebugPrint("RateOfFire = ", Depth);
              DebugPrint(S->RateOfFire, Depth+1);
              DebugPrint("\n");

              DebugPrint("FireCooldown = ", Depth);
              DebugPrint(S->FireCooldown, Depth+1);
              DebugPrint("\n");

              DebugPrint("Health = ", Depth);
              DebugPrint(S->Health, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(frame_event S, u32 Depth)
        {
          DebugPrint("frame_event\n", Depth);
          
            DebugPrint("Type = ", Depth);
            DebugPrint(S.Type, Depth+1);
            DebugPrint("\n");

            DebugPrint("Entity = ", Depth);
            DebugPrint(S.Entity, Depth+1);
            DebugPrint("\n");

            DebugPrint("Next = ", Depth);
            DebugPrint(S.Next, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(frame_event* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("frame_event\n", Depth);
            
              DebugPrint("Type = ", Depth);
              DebugPrint(S->Type, Depth+1);
              DebugPrint("\n");

              DebugPrint("Entity = ", Depth);
              DebugPrint(S->Entity, Depth+1);
              DebugPrint("\n");

              DebugPrint("Next = ", Depth);
              DebugPrint(S->Next, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(event_queue S, u32 Depth)
        {
          DebugPrint("event_queue\n", Depth);
          
            DebugPrint("CurrentFrameIndex = ", Depth);
            DebugPrint(S.CurrentFrameIndex, Depth+1);
            DebugPrint("\n");

            DebugPrint("Queue = ", Depth);
            DebugPrint(S.Queue, Depth+1);
            DebugPrint("\n");

            DebugPrint("FirstFreeEvent = ", Depth);
            DebugPrint(S.FirstFreeEvent, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(event_queue* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("event_queue\n", Depth);
            
              DebugPrint("CurrentFrameIndex = ", Depth);
              DebugPrint(S->CurrentFrameIndex, Depth+1);
              DebugPrint("\n");

              DebugPrint("Queue = ", Depth);
              DebugPrint(S->Queue, Depth+1);
              DebugPrint("\n");

              DebugPrint("FirstFreeEvent = ", Depth);
              DebugPrint(S->FirstFreeEvent, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(entity_list S, u32 Depth)
        {
          DebugPrint("entity_list\n", Depth);
          
            DebugPrint("This = ", Depth);
            DebugPrint(S.This, Depth+1);
            DebugPrint("\n");

            DebugPrint("Next = ", Depth);
            DebugPrint(S.Next, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(entity_list* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("entity_list\n", Depth);
            
              DebugPrint("This = ", Depth);
              DebugPrint(S->This, Depth+1);
              DebugPrint("\n");

              DebugPrint("Next = ", Depth);
              DebugPrint(S->Next, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(point_buffer S, u32 Depth)
        {
          DebugPrint("point_buffer\n", Depth);
          
            DebugPrint("Count = ", Depth);
            DebugPrint(S.Count, Depth+1);
            DebugPrint("\n");

            DebugPrint("Points = ", Depth);
            DebugPrint(S.Points, Depth+1);
            DebugPrint("\n");

            DebugPrint("Min = ", Depth);
            DebugPrint(S.Min, Depth+1);
            DebugPrint("\n");

            DebugPrint("Max = ", Depth);
            DebugPrint(S.Max, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(point_buffer* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("point_buffer\n", Depth);
            
              DebugPrint("Count = ", Depth);
              DebugPrint(S->Count, Depth+1);
              DebugPrint("\n");

              DebugPrint("Points = ", Depth);
              DebugPrint(S->Points, Depth+1);
              DebugPrint("\n");

              DebugPrint("Min = ", Depth);
              DebugPrint(S->Min, Depth+1);
              DebugPrint("\n");

              DebugPrint("Max = ", Depth);
              DebugPrint(S->Max, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(world_chunk S, u32 Depth)
        {
          DebugPrint("world_chunk\n", Depth);
          
            DebugPrint("Data = ", Depth);
            DebugPrint(S.Data, Depth+1);
            DebugPrint("\n");

            DebugPrint("Mesh = ", Depth);
            DebugPrint(S.Mesh, Depth+1);
            DebugPrint("\n");

            DebugPrint("LodMesh = ", Depth);
            DebugPrint(S.LodMesh, Depth+1);
            DebugPrint("\n");

            DebugPrint("WorldP = ", Depth);
            DebugPrint(S.WorldP, Depth+1);
            DebugPrint("\n");

            DebugPrint("FilledCount = ", Depth);
            DebugPrint(S.FilledCount, Depth+1);
            DebugPrint("\n");

            DebugPrint("Picked = ", Depth);
            DebugPrint(S.Picked, Depth+1);
            DebugPrint("\n");

            DebugPrint("LodMesh_Complete = ", Depth);
            DebugPrint(S.LodMesh_Complete, Depth+1);
            DebugPrint("\n");

            DebugPrint("DrawBoundingVoxels = ", Depth);
            DebugPrint(S.DrawBoundingVoxels, Depth+1);
            DebugPrint("\n");

            DebugPrint("PointsToLeaveRemaining = ", Depth);
            DebugPrint(S.PointsToLeaveRemaining, Depth+1);
            DebugPrint("\n");

            DebugPrint("TriCount = ", Depth);
            DebugPrint(S.TriCount, Depth+1);
            DebugPrint("\n");

            DebugPrint("EdgeBoundaryVoxelCount = ", Depth);
            DebugPrint(S.EdgeBoundaryVoxelCount, Depth+1);
            DebugPrint("\n");

            DebugPrint("DimX = ", Depth);
            DebugPrint(S.DimX, Depth+1);
            DebugPrint("\n");

            DebugPrint("DimY = ", Depth);
            DebugPrint(S.DimY, Depth+1);
            DebugPrint("\n");

            DebugPrint("DimZ = ", Depth);
            DebugPrint(S.DimZ, Depth+1);
            DebugPrint("\n");

            DebugPrint("Pad = ", Depth);
            DebugPrint(S.Pad, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(world_chunk* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("world_chunk\n", Depth);
            
              DebugPrint("Data = ", Depth);
              DebugPrint(S->Data, Depth+1);
              DebugPrint("\n");

              DebugPrint("Mesh = ", Depth);
              DebugPrint(S->Mesh, Depth+1);
              DebugPrint("\n");

              DebugPrint("LodMesh = ", Depth);
              DebugPrint(S->LodMesh, Depth+1);
              DebugPrint("\n");

              DebugPrint("WorldP = ", Depth);
              DebugPrint(S->WorldP, Depth+1);
              DebugPrint("\n");

              DebugPrint("FilledCount = ", Depth);
              DebugPrint(S->FilledCount, Depth+1);
              DebugPrint("\n");

              DebugPrint("Picked = ", Depth);
              DebugPrint(S->Picked, Depth+1);
              DebugPrint("\n");

              DebugPrint("LodMesh_Complete = ", Depth);
              DebugPrint(S->LodMesh_Complete, Depth+1);
              DebugPrint("\n");

              DebugPrint("DrawBoundingVoxels = ", Depth);
              DebugPrint(S->DrawBoundingVoxels, Depth+1);
              DebugPrint("\n");

              DebugPrint("PointsToLeaveRemaining = ", Depth);
              DebugPrint(S->PointsToLeaveRemaining, Depth+1);
              DebugPrint("\n");

              DebugPrint("TriCount = ", Depth);
              DebugPrint(S->TriCount, Depth+1);
              DebugPrint("\n");

              DebugPrint("EdgeBoundaryVoxelCount = ", Depth);
              DebugPrint(S->EdgeBoundaryVoxelCount, Depth+1);
              DebugPrint("\n");

              DebugPrint("DimX = ", Depth);
              DebugPrint(S->DimX, Depth+1);
              DebugPrint("\n");

              DebugPrint("DimY = ", Depth);
              DebugPrint(S->DimY, Depth+1);
              DebugPrint("\n");

              DebugPrint("DimZ = ", Depth);
              DebugPrint(S->DimZ, Depth+1);
              DebugPrint("\n");

              DebugPrint("Pad = ", Depth);
              DebugPrint(S->Pad, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(collision_event S, u32 Depth)
        {
          DebugPrint("collision_event\n", Depth);
          
            DebugPrint("Chunk = ", Depth);
            DebugPrint(S.Chunk, Depth+1);
            DebugPrint("\n");

            DebugPrint("CP = ", Depth);
            DebugPrint(S.CP, Depth+1);
            DebugPrint("\n");

            DebugPrint("didCollide = ", Depth);
            DebugPrint(S.didCollide, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(collision_event* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("collision_event\n", Depth);
            
              DebugPrint("Chunk = ", Depth);
              DebugPrint(S->Chunk, Depth+1);
              DebugPrint("\n");

              DebugPrint("CP = ", Depth);
              DebugPrint(S->CP, Depth+1);
              DebugPrint("\n");

              DebugPrint("didCollide = ", Depth);
              DebugPrint(S->didCollide, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(free_world_chunk S, u32 Depth)
        {
          DebugPrint("free_world_chunk\n", Depth);
          
            DebugPrint("chunk_data = ", Depth);
            DebugPrint(S.chunk_data, Depth+1);
            DebugPrint("\n");

            DebugPrint("Next = ", Depth);
            DebugPrint(S.Next, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(free_world_chunk* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("free_world_chunk\n", Depth);
            
              DebugPrint("chunk_data = ", Depth);
              DebugPrint(S->chunk_data, Depth+1);
              DebugPrint("\n");

              DebugPrint("Next = ", Depth);
              DebugPrint(S->Next, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(world S, u32 Depth)
        {
          DebugPrint("world\n", Depth);
          
            DebugPrint("ChunkHash = ", Depth);
            DebugPrint(S.ChunkHash, Depth+1);
            DebugPrint("\n");

            DebugPrint("FreeChunks = ", Depth);
            DebugPrint(S.FreeChunks, Depth+1);
            DebugPrint("\n");

            DebugPrint("FreeChunkCount = ", Depth);
            DebugPrint(S.FreeChunkCount, Depth+1);
            DebugPrint("\n");

            DebugPrint("VisibleRegion = ", Depth);
            DebugPrint(S.VisibleRegion, Depth+1);
            DebugPrint("\n");

            DebugPrint("ChunkDim = ", Depth);
            DebugPrint(S.ChunkDim, Depth+1);
            DebugPrint("\n");

            DebugPrint("Center = ", Depth);
            DebugPrint(S.Center, Depth+1);
            DebugPrint("\n");

            DebugPrint("Memory = ", Depth);
            DebugPrint(S.Memory, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(world* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("world\n", Depth);
            
              DebugPrint("ChunkHash = ", Depth);
              DebugPrint(S->ChunkHash, Depth+1);
              DebugPrint("\n");

              DebugPrint("FreeChunks = ", Depth);
              DebugPrint(S->FreeChunks, Depth+1);
              DebugPrint("\n");

              DebugPrint("FreeChunkCount = ", Depth);
              DebugPrint(S->FreeChunkCount, Depth+1);
              DebugPrint("\n");

              DebugPrint("VisibleRegion = ", Depth);
              DebugPrint(S->VisibleRegion, Depth+1);
              DebugPrint("\n");

              DebugPrint("ChunkDim = ", Depth);
              DebugPrint(S->ChunkDim, Depth+1);
              DebugPrint("\n");

              DebugPrint("Center = ", Depth);
              DebugPrint(S->Center, Depth+1);
              DebugPrint("\n");

              DebugPrint("Memory = ", Depth);
              DebugPrint(S->Memory, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(thread S, u32 Depth)
        {
          DebugPrint("thread\n", Depth);
          
            DebugPrint("ThreadIndex = ", Depth);
            DebugPrint(S.ThreadIndex, Depth+1);
            DebugPrint("\n");

            DebugPrint("ID = ", Depth);
            DebugPrint(S.ID, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(thread* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("thread\n", Depth);
            
              DebugPrint("ThreadIndex = ", Depth);
              DebugPrint(S->ThreadIndex, Depth+1);
              DebugPrint("\n");

              DebugPrint("ID = ", Depth);
              DebugPrint(S->ID, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(thread_local_state S, u32 Depth)
        {
          DebugPrint("thread_local_state\n", Depth);
          
            DebugPrint("PermMemory = ", Depth);
            DebugPrint(S.PermMemory, Depth+1);
            DebugPrint("\n");

            DebugPrint("TempMemory = ", Depth);
            DebugPrint(S.TempMemory, Depth+1);
            DebugPrint("\n");

            DebugPrint("MeshFreelist = ", Depth);
            DebugPrint(S.MeshFreelist, Depth+1);
            DebugPrint("\n");

            DebugPrint("Noise = ", Depth);
            DebugPrint(S.Noise, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(thread_local_state* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("thread_local_state\n", Depth);
            
              DebugPrint("PermMemory = ", Depth);
              DebugPrint(S->PermMemory, Depth+1);
              DebugPrint("\n");

              DebugPrint("TempMemory = ", Depth);
              DebugPrint(S->TempMemory, Depth+1);
              DebugPrint("\n");

              DebugPrint("MeshFreelist = ", Depth);
              DebugPrint(S->MeshFreelist, Depth+1);
              DebugPrint("\n");

              DebugPrint("Noise = ", Depth);
              DebugPrint(S->Noise, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(u32_stream S, u32 Depth)
        {
          DebugPrint("u32_stream\n", Depth);
          
            DebugPrint("Start = ", Depth);
            DebugPrint(S.Start, Depth+1);
            DebugPrint("\n");

            DebugPrint("At = ", Depth);
            DebugPrint(S.At, Depth+1);
            DebugPrint("\n");

            DebugPrint("End = ", Depth);
            DebugPrint(S.End, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(u32_stream* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("u32_stream\n", Depth);
            
              DebugPrint("Start = ", Depth);
              DebugPrint(S->Start, Depth+1);
              DebugPrint("\n");

              DebugPrint("At = ", Depth);
              DebugPrint(S->At, Depth+1);
              DebugPrint("\n");

              DebugPrint("End = ", Depth);
              DebugPrint(S->End, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(u8_stream S, u32 Depth)
        {
          DebugPrint("u8_stream\n", Depth);
          
            DebugPrint("Start = ", Depth);
            DebugPrint(S.Start, Depth+1);
            DebugPrint("\n");

            DebugPrint("At = ", Depth);
            DebugPrint(S.At, Depth+1);
            DebugPrint("\n");

            DebugPrint("End = ", Depth);
            DebugPrint(S.End, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(u8_stream* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("u8_stream\n", Depth);
            
              DebugPrint("Start = ", Depth);
              DebugPrint(S->Start, Depth+1);
              DebugPrint("\n");

              DebugPrint("At = ", Depth);
              DebugPrint(S->At, Depth+1);
              DebugPrint("\n");

              DebugPrint("End = ", Depth);
              DebugPrint(S->End, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(ansi_stream S, u32 Depth)
        {
          DebugPrint("ansi_stream\n", Depth);
          
            DebugPrint("Start = ", Depth);
            DebugPrint(S.Start, Depth+1);
            DebugPrint("\n");

            DebugPrint("At = ", Depth);
            DebugPrint(S.At, Depth+1);
            DebugPrint("\n");

            DebugPrint("End = ", Depth);
            DebugPrint(S.End, Depth+1);
            DebugPrint("\n");

            DebugPrint("Filename = ", Depth);
            DebugPrint(S.Filename, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(ansi_stream* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("ansi_stream\n", Depth);
            
              DebugPrint("Start = ", Depth);
              DebugPrint(S->Start, Depth+1);
              DebugPrint("\n");

              DebugPrint("At = ", Depth);
              DebugPrint(S->At, Depth+1);
              DebugPrint("\n");

              DebugPrint("End = ", Depth);
              DebugPrint(S->End, Depth+1);
              DebugPrint("\n");

              DebugPrint("Filename = ", Depth);
              DebugPrint(S->Filename, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(r32_stream S, u32 Depth)
        {
          DebugPrint("r32_stream\n", Depth);
          
            DebugPrint("Start = ", Depth);
            DebugPrint(S.Start, Depth+1);
            DebugPrint("\n");

            DebugPrint("At = ", Depth);
            DebugPrint(S.At, Depth+1);
            DebugPrint("\n");

            DebugPrint("End = ", Depth);
            DebugPrint(S.End, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(r32_stream* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("r32_stream\n", Depth);
            
              DebugPrint("Start = ", Depth);
              DebugPrint(S->Start, Depth+1);
              DebugPrint("\n");

              DebugPrint("At = ", Depth);
              DebugPrint(S->At, Depth+1);
              DebugPrint("\n");

              DebugPrint("End = ", Depth);
              DebugPrint(S->End, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(mesh_metadata S, u32 Depth)
        {
          DebugPrint("mesh_metadata\n", Depth);
          
            DebugPrint("VertCount = ", Depth);
            DebugPrint(S.VertCount, Depth+1);
            DebugPrint("\n");

            DebugPrint("NormalCount = ", Depth);
            DebugPrint(S.NormalCount, Depth+1);
            DebugPrint("\n");

            DebugPrint("UVCount = ", Depth);
            DebugPrint(S.UVCount, Depth+1);
            DebugPrint("\n");

            DebugPrint("FaceCount = ", Depth);
            DebugPrint(S.FaceCount, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(mesh_metadata* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("mesh_metadata\n", Depth);
            
              DebugPrint("VertCount = ", Depth);
              DebugPrint(S->VertCount, Depth+1);
              DebugPrint("\n");

              DebugPrint("NormalCount = ", Depth);
              DebugPrint(S->NormalCount, Depth+1);
              DebugPrint("\n");

              DebugPrint("UVCount = ", Depth);
              DebugPrint(S->UVCount, Depth+1);
              DebugPrint("\n");

              DebugPrint("FaceCount = ", Depth);
              DebugPrint(S->FaceCount, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(rect2 S, u32 Depth)
        {
          DebugPrint("rect2\n", Depth);
          
            DebugPrint("Min = ", Depth);
            DebugPrint(S.Min, Depth+1);
            DebugPrint("\n");

            DebugPrint("Max = ", Depth);
            DebugPrint(S.Max, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(rect2* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("rect2\n", Depth);
            
              DebugPrint("Min = ", Depth);
              DebugPrint(S->Min, Depth+1);
              DebugPrint("\n");

              DebugPrint("Max = ", Depth);
              DebugPrint(S->Max, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(aabb S, u32 Depth)
        {
          DebugPrint("aabb\n", Depth);
          
            DebugPrint("Center = ", Depth);
            DebugPrint(S.Center, Depth+1);
            DebugPrint("\n");

            DebugPrint("Radius = ", Depth);
            DebugPrint(S.Radius, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(aabb* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("aabb\n", Depth);
            
              DebugPrint("Center = ", Depth);
              DebugPrint(S->Center, Depth+1);
              DebugPrint("\n");

              DebugPrint("Radius = ", Depth);
              DebugPrint(S->Radius, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(ray S, u32 Depth)
        {
          DebugPrint("ray\n", Depth);
          
            DebugPrint("Origin = ", Depth);
            DebugPrint(S.Origin, Depth+1);
            DebugPrint("\n");

            DebugPrint("Dir = ", Depth);
            DebugPrint(S.Dir, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(ray* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("ray\n", Depth);
            
              DebugPrint("Origin = ", Depth);
              DebugPrint(S->Origin, Depth+1);
              DebugPrint("\n");

              DebugPrint("Dir = ", Depth);
              DebugPrint(S->Dir, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(input_event S, u32 Depth)
        {
          DebugPrint("input_event\n", Depth);
          
            DebugPrint("Clicked = ", Depth);
            DebugPrint(S.Clicked, Depth+1);
            DebugPrint("\n");

            DebugPrint("Pressed = ", Depth);
            DebugPrint(S.Pressed, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(input_event* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("input_event\n", Depth);
            
              DebugPrint("Clicked = ", Depth);
              DebugPrint(S->Clicked, Depth+1);
              DebugPrint("\n");

              DebugPrint("Pressed = ", Depth);
              DebugPrint(S->Pressed, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(input S, u32 Depth)
        {
          DebugPrint("input\n", Depth);
          
            DebugPrint("Escape = ", Depth);
            DebugPrint(S.Escape, Depth+1);
            DebugPrint("\n");

            DebugPrint("Space = ", Depth);
            DebugPrint(S.Space, Depth+1);
            DebugPrint("\n");

            DebugPrint("Shift = ", Depth);
            DebugPrint(S.Shift, Depth+1);
            DebugPrint("\n");

            DebugPrint("Ctrl = ", Depth);
            DebugPrint(S.Ctrl, Depth+1);
            DebugPrint("\n");

            DebugPrint("Alt = ", Depth);
            DebugPrint(S.Alt, Depth+1);
            DebugPrint("\n");

            DebugPrint("F12 = ", Depth);
            DebugPrint(S.F12, Depth+1);
            DebugPrint("\n");

            DebugPrint("F11 = ", Depth);
            DebugPrint(S.F11, Depth+1);
            DebugPrint("\n");

            DebugPrint("F10 = ", Depth);
            DebugPrint(S.F10, Depth+1);
            DebugPrint("\n");

            DebugPrint("F9 = ", Depth);
            DebugPrint(S.F9, Depth+1);
            DebugPrint("\n");

            DebugPrint("F8 = ", Depth);
            DebugPrint(S.F8, Depth+1);
            DebugPrint("\n");

            DebugPrint("F7 = ", Depth);
            DebugPrint(S.F7, Depth+1);
            DebugPrint("\n");

            DebugPrint("F6 = ", Depth);
            DebugPrint(S.F6, Depth+1);
            DebugPrint("\n");

            DebugPrint("F5 = ", Depth);
            DebugPrint(S.F5, Depth+1);
            DebugPrint("\n");

            DebugPrint("F4 = ", Depth);
            DebugPrint(S.F4, Depth+1);
            DebugPrint("\n");

            DebugPrint("F3 = ", Depth);
            DebugPrint(S.F3, Depth+1);
            DebugPrint("\n");

            DebugPrint("F2 = ", Depth);
            DebugPrint(S.F2, Depth+1);
            DebugPrint("\n");

            DebugPrint("F1 = ", Depth);
            DebugPrint(S.F1, Depth+1);
            DebugPrint("\n");

            DebugPrint("RMB = ", Depth);
            DebugPrint(S.RMB, Depth+1);
            DebugPrint("\n");

            DebugPrint("LMB = ", Depth);
            DebugPrint(S.LMB, Depth+1);
            DebugPrint("\n");

            DebugPrint("MMB = ", Depth);
            DebugPrint(S.MMB, Depth+1);
            DebugPrint("\n");

            DebugPrint("W = ", Depth);
            DebugPrint(S.W, Depth+1);
            DebugPrint("\n");

            DebugPrint("A = ", Depth);
            DebugPrint(S.A, Depth+1);
            DebugPrint("\n");

            DebugPrint("S = ", Depth);
            DebugPrint(S.S, Depth+1);
            DebugPrint("\n");

            DebugPrint("D = ", Depth);
            DebugPrint(S.D, Depth+1);
            DebugPrint("\n");

            DebugPrint("Q = ", Depth);
            DebugPrint(S.Q, Depth+1);
            DebugPrint("\n");

            DebugPrint("E = ", Depth);
            DebugPrint(S.E, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(input* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("input\n", Depth);
            
              DebugPrint("Escape = ", Depth);
              DebugPrint(S->Escape, Depth+1);
              DebugPrint("\n");

              DebugPrint("Space = ", Depth);
              DebugPrint(S->Space, Depth+1);
              DebugPrint("\n");

              DebugPrint("Shift = ", Depth);
              DebugPrint(S->Shift, Depth+1);
              DebugPrint("\n");

              DebugPrint("Ctrl = ", Depth);
              DebugPrint(S->Ctrl, Depth+1);
              DebugPrint("\n");

              DebugPrint("Alt = ", Depth);
              DebugPrint(S->Alt, Depth+1);
              DebugPrint("\n");

              DebugPrint("F12 = ", Depth);
              DebugPrint(S->F12, Depth+1);
              DebugPrint("\n");

              DebugPrint("F11 = ", Depth);
              DebugPrint(S->F11, Depth+1);
              DebugPrint("\n");

              DebugPrint("F10 = ", Depth);
              DebugPrint(S->F10, Depth+1);
              DebugPrint("\n");

              DebugPrint("F9 = ", Depth);
              DebugPrint(S->F9, Depth+1);
              DebugPrint("\n");

              DebugPrint("F8 = ", Depth);
              DebugPrint(S->F8, Depth+1);
              DebugPrint("\n");

              DebugPrint("F7 = ", Depth);
              DebugPrint(S->F7, Depth+1);
              DebugPrint("\n");

              DebugPrint("F6 = ", Depth);
              DebugPrint(S->F6, Depth+1);
              DebugPrint("\n");

              DebugPrint("F5 = ", Depth);
              DebugPrint(S->F5, Depth+1);
              DebugPrint("\n");

              DebugPrint("F4 = ", Depth);
              DebugPrint(S->F4, Depth+1);
              DebugPrint("\n");

              DebugPrint("F3 = ", Depth);
              DebugPrint(S->F3, Depth+1);
              DebugPrint("\n");

              DebugPrint("F2 = ", Depth);
              DebugPrint(S->F2, Depth+1);
              DebugPrint("\n");

              DebugPrint("F1 = ", Depth);
              DebugPrint(S->F1, Depth+1);
              DebugPrint("\n");

              DebugPrint("RMB = ", Depth);
              DebugPrint(S->RMB, Depth+1);
              DebugPrint("\n");

              DebugPrint("LMB = ", Depth);
              DebugPrint(S->LMB, Depth+1);
              DebugPrint("\n");

              DebugPrint("MMB = ", Depth);
              DebugPrint(S->MMB, Depth+1);
              DebugPrint("\n");

              DebugPrint("W = ", Depth);
              DebugPrint(S->W, Depth+1);
              DebugPrint("\n");

              DebugPrint("A = ", Depth);
              DebugPrint(S->A, Depth+1);
              DebugPrint("\n");

              DebugPrint("S = ", Depth);
              DebugPrint(S->S, Depth+1);
              DebugPrint("\n");

              DebugPrint("D = ", Depth);
              DebugPrint(S->D, Depth+1);
              DebugPrint("\n");

              DebugPrint("Q = ", Depth);
              DebugPrint(S->Q, Depth+1);
              DebugPrint("\n");

              DebugPrint("E = ", Depth);
              DebugPrint(S->E, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(hotkeys S, u32 Depth)
        {
          DebugPrint("hotkeys\n", Depth);
          
            DebugPrint("Debug_RedrawEveryPush = ", Depth);
            DebugPrint(S.Debug_RedrawEveryPush, Depth+1);
            DebugPrint("\n");

            DebugPrint("Debug_ToggleLoopedGamePlayback = ", Depth);
            DebugPrint(S.Debug_ToggleLoopedGamePlayback, Depth+1);
            DebugPrint("\n");

            DebugPrint("Debug_ToggleTriggeredRuntimeBreak = ", Depth);
            DebugPrint(S.Debug_ToggleTriggeredRuntimeBreak, Depth+1);
            DebugPrint("\n");

            DebugPrint("Debug_Pause = ", Depth);
            DebugPrint(S.Debug_Pause, Depth+1);
            DebugPrint("\n");

            DebugPrint("Debug_ToggleProfile = ", Depth);
            DebugPrint(S.Debug_ToggleProfile, Depth+1);
            DebugPrint("\n");

            DebugPrint("Debug_ToggleMenu = ", Depth);
            DebugPrint(S.Debug_ToggleMenu, Depth+1);
            DebugPrint("\n");

            DebugPrint("Debug_TriangulateIncrement = ", Depth);
            DebugPrint(S.Debug_TriangulateIncrement, Depth+1);
            DebugPrint("\n");

            DebugPrint("Debug_TriangulateDecrement = ", Depth);
            DebugPrint(S.Debug_TriangulateDecrement, Depth+1);
            DebugPrint("\n");

            DebugPrint("Debug_MousePick = ", Depth);
            DebugPrint(S.Debug_MousePick, Depth+1);
            DebugPrint("\n");

            DebugPrint("Left = ", Depth);
            DebugPrint(S.Left, Depth+1);
            DebugPrint("\n");

            DebugPrint("Right = ", Depth);
            DebugPrint(S.Right, Depth+1);
            DebugPrint("\n");

            DebugPrint("Forward = ", Depth);
            DebugPrint(S.Forward, Depth+1);
            DebugPrint("\n");

            DebugPrint("Backward = ", Depth);
            DebugPrint(S.Backward, Depth+1);
            DebugPrint("\n");

            DebugPrint("Player_Fire = ", Depth);
            DebugPrint(S.Player_Fire, Depth+1);
            DebugPrint("\n");

            DebugPrint("Player_Proton = ", Depth);
            DebugPrint(S.Player_Proton, Depth+1);
            DebugPrint("\n");

            DebugPrint("Player_Spawn = ", Depth);
            DebugPrint(S.Player_Spawn, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(hotkeys* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("hotkeys\n", Depth);
            
              DebugPrint("Debug_RedrawEveryPush = ", Depth);
              DebugPrint(S->Debug_RedrawEveryPush, Depth+1);
              DebugPrint("\n");

              DebugPrint("Debug_ToggleLoopedGamePlayback = ", Depth);
              DebugPrint(S->Debug_ToggleLoopedGamePlayback, Depth+1);
              DebugPrint("\n");

              DebugPrint("Debug_ToggleTriggeredRuntimeBreak = ", Depth);
              DebugPrint(S->Debug_ToggleTriggeredRuntimeBreak, Depth+1);
              DebugPrint("\n");

              DebugPrint("Debug_Pause = ", Depth);
              DebugPrint(S->Debug_Pause, Depth+1);
              DebugPrint("\n");

              DebugPrint("Debug_ToggleProfile = ", Depth);
              DebugPrint(S->Debug_ToggleProfile, Depth+1);
              DebugPrint("\n");

              DebugPrint("Debug_ToggleMenu = ", Depth);
              DebugPrint(S->Debug_ToggleMenu, Depth+1);
              DebugPrint("\n");

              DebugPrint("Debug_TriangulateIncrement = ", Depth);
              DebugPrint(S->Debug_TriangulateIncrement, Depth+1);
              DebugPrint("\n");

              DebugPrint("Debug_TriangulateDecrement = ", Depth);
              DebugPrint(S->Debug_TriangulateDecrement, Depth+1);
              DebugPrint("\n");

              DebugPrint("Debug_MousePick = ", Depth);
              DebugPrint(S->Debug_MousePick, Depth+1);
              DebugPrint("\n");

              DebugPrint("Left = ", Depth);
              DebugPrint(S->Left, Depth+1);
              DebugPrint("\n");

              DebugPrint("Right = ", Depth);
              DebugPrint(S->Right, Depth+1);
              DebugPrint("\n");

              DebugPrint("Forward = ", Depth);
              DebugPrint(S->Forward, Depth+1);
              DebugPrint("\n");

              DebugPrint("Backward = ", Depth);
              DebugPrint(S->Backward, Depth+1);
              DebugPrint("\n");

              DebugPrint("Player_Fire = ", Depth);
              DebugPrint(S->Player_Fire, Depth+1);
              DebugPrint("\n");

              DebugPrint("Player_Proton = ", Depth);
              DebugPrint(S->Player_Proton, Depth+1);
              DebugPrint("\n");

              DebugPrint("Player_Spawn = ", Depth);
              DebugPrint(S->Player_Spawn, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(work_queue S, u32 Depth)
        {
          DebugPrint("work_queue\n", Depth);
          
            DebugPrint("EnqueueIndex = ", Depth);
            DebugPrint(S.EnqueueIndex, Depth+1);
            DebugPrint("\n");

            DebugPrint("DequeueIndex = ", Depth);
            DebugPrint(S.DequeueIndex, Depth+1);
            DebugPrint("\n");

            DebugPrint("Entries = ", Depth);
            DebugPrint(S.Entries, Depth+1);
            DebugPrint("\n");

            DebugPrint("GlobalQueueSemaphore = ", Depth);
            DebugPrint(S.GlobalQueueSemaphore, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(work_queue* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("work_queue\n", Depth);
            
              DebugPrint("EnqueueIndex = ", Depth);
              DebugPrint(S->EnqueueIndex, Depth+1);
              DebugPrint("\n");

              DebugPrint("DequeueIndex = ", Depth);
              DebugPrint(S->DequeueIndex, Depth+1);
              DebugPrint("\n");

              DebugPrint("Entries = ", Depth);
              DebugPrint(S->Entries, Depth+1);
              DebugPrint("\n");

              DebugPrint("GlobalQueueSemaphore = ", Depth);
              DebugPrint(S->GlobalQueueSemaphore, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(string_builder S, u32 Depth)
        {
          DebugPrint("string_builder\n", Depth);
          
            DebugPrint("Memory = ", Depth);
            DebugPrint(S.Memory, Depth+1);
            DebugPrint("\n");

            DebugPrint("Chunks = ", Depth);
            DebugPrint(S.Chunks, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(string_builder* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("string_builder\n", Depth);
            
              DebugPrint("Memory = ", Depth);
              DebugPrint(S->Memory, Depth+1);
              DebugPrint("\n");

              DebugPrint("Chunks = ", Depth);
              DebugPrint(S->Chunks, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(platform S, u32 Depth)
        {
          DebugPrint("platform\n", Depth);
          
            DebugPrint("LowPriority = ", Depth);
            DebugPrint(S.LowPriority, Depth+1);
            DebugPrint("\n");

            DebugPrint("HighPriority = ", Depth);
            DebugPrint(S.HighPriority, Depth+1);
            DebugPrint("\n");

            DebugPrint("QueueSemaphore = ", Depth);
            DebugPrint(S.QueueSemaphore, Depth+1);
            DebugPrint("\n");

            DebugPrint("Threads = ", Depth);
            DebugPrint(S.Threads, Depth+1);
            DebugPrint("\n");

            DebugPrint("MouseP = ", Depth);
            DebugPrint(S.MouseP, Depth+1);
            DebugPrint("\n");

            DebugPrint("MouseDP = ", Depth);
            DebugPrint(S.MouseDP, Depth+1);
            DebugPrint("\n");

            DebugPrint("Memory = ", Depth);
            DebugPrint(S.Memory, Depth+1);
            DebugPrint("\n");

            DebugPrint("dt = ", Depth);
            DebugPrint(S.dt, Depth+1);
            DebugPrint("\n");

            DebugPrint("WindowWidth = ", Depth);
            DebugPrint(S.WindowWidth, Depth+1);
            DebugPrint("\n");

            DebugPrint("WindowHeight = ", Depth);
            DebugPrint(S.WindowHeight, Depth+1);
            DebugPrint("\n");

            DebugPrint("Input = ", Depth);
            DebugPrint(S.Input, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(platform* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("platform\n", Depth);
            
              DebugPrint("LowPriority = ", Depth);
              DebugPrint(S->LowPriority, Depth+1);
              DebugPrint("\n");

              DebugPrint("HighPriority = ", Depth);
              DebugPrint(S->HighPriority, Depth+1);
              DebugPrint("\n");

              DebugPrint("QueueSemaphore = ", Depth);
              DebugPrint(S->QueueSemaphore, Depth+1);
              DebugPrint("\n");

              DebugPrint("Threads = ", Depth);
              DebugPrint(S->Threads, Depth+1);
              DebugPrint("\n");

              DebugPrint("MouseP = ", Depth);
              DebugPrint(S->MouseP, Depth+1);
              DebugPrint("\n");

              DebugPrint("MouseDP = ", Depth);
              DebugPrint(S->MouseDP, Depth+1);
              DebugPrint("\n");

              DebugPrint("Memory = ", Depth);
              DebugPrint(S->Memory, Depth+1);
              DebugPrint("\n");

              DebugPrint("dt = ", Depth);
              DebugPrint(S->dt, Depth+1);
              DebugPrint("\n");

              DebugPrint("WindowWidth = ", Depth);
              DebugPrint(S->WindowWidth, Depth+1);
              DebugPrint("\n");

              DebugPrint("WindowHeight = ", Depth);
              DebugPrint(S->WindowHeight, Depth+1);
              DebugPrint("\n");

              DebugPrint("Input = ", Depth);
              DebugPrint(S->Input, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(m4 S, u32 Depth)
        {
          DebugPrint("m4\n", Depth);
          
            DebugPrint("E = ", Depth);
            DebugPrint(S.E, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(m4* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("m4\n", Depth);
            
              DebugPrint("E = ", Depth);
              DebugPrint(S->E, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(m_nxn S, u32 Depth)
        {
          DebugPrint("m_nxn\n", Depth);
          
            DebugPrint("Elements = ", Depth);
            DebugPrint(S.Elements, Depth+1);
            DebugPrint("\n");

            DebugPrint("Rows = ", Depth);
            DebugPrint(S.Rows, Depth+1);
            DebugPrint("\n");

            DebugPrint("Columns = ", Depth);
            DebugPrint(S.Columns, Depth+1);
            DebugPrint("\n");

            DebugPrint("At = ", Depth);
            DebugPrint(S.At, Depth+1);
            DebugPrint("\n");

            DebugPrint("End = ", Depth);
            DebugPrint(S.End, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(m_nxn* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("m_nxn\n", Depth);
            
              DebugPrint("Elements = ", Depth);
              DebugPrint(S->Elements, Depth+1);
              DebugPrint("\n");

              DebugPrint("Rows = ", Depth);
              DebugPrint(S->Rows, Depth+1);
              DebugPrint("\n");

              DebugPrint("Columns = ", Depth);
              DebugPrint(S->Columns, Depth+1);
              DebugPrint("\n");

              DebugPrint("At = ", Depth);
              DebugPrint(S->At, Depth+1);
              DebugPrint("\n");

              DebugPrint("End = ", Depth);
              DebugPrint(S->End, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(random_series S, u32 Depth)
        {
          DebugPrint("random_series\n", Depth);
          
            DebugPrint("Seed = ", Depth);
            DebugPrint(S.Seed, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(random_series* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("random_series\n", Depth);
            
              DebugPrint("Seed = ", Depth);
              DebugPrint(S->Seed, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(memory_arena S, u32 Depth)
        {
          DebugPrint("memory_arena\n", Depth);
          
            DebugPrint("Start = ", Depth);
            DebugPrint(S.Start, Depth+1);
            DebugPrint("\n");

            DebugPrint("At = ", Depth);
            DebugPrint(S.At, Depth+1);
            DebugPrint("\n");

            DebugPrint("End = ", Depth);
            DebugPrint(S.End, Depth+1);
            DebugPrint("\n");

            DebugPrint("NextBlockSize = ", Depth);
            DebugPrint(S.NextBlockSize, Depth+1);
            DebugPrint("\n");

            DebugPrint("Prev = ", Depth);
            DebugPrint(S.Prev, Depth+1);
            DebugPrint("\n");

            DebugPrint("Pushes = ", Depth);
            DebugPrint(S.Pushes, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(memory_arena* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("memory_arena\n", Depth);
            
              DebugPrint("Start = ", Depth);
              DebugPrint(S->Start, Depth+1);
              DebugPrint("\n");

              DebugPrint("At = ", Depth);
              DebugPrint(S->At, Depth+1);
              DebugPrint("\n");

              DebugPrint("End = ", Depth);
              DebugPrint(S->End, Depth+1);
              DebugPrint("\n");

              DebugPrint("NextBlockSize = ", Depth);
              DebugPrint(S->NextBlockSize, Depth+1);
              DebugPrint("\n");

              DebugPrint("Prev = ", Depth);
              DebugPrint(S->Prev, Depth+1);
              DebugPrint("\n");

              DebugPrint("Pushes = ", Depth);
              DebugPrint(S->Pushes, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(push_metadata S, u32 Depth)
        {
          DebugPrint("push_metadata\n", Depth);
          
            DebugPrint("Name = ", Depth);
            DebugPrint(S.Name, Depth+1);
            DebugPrint("\n");

            DebugPrint("ArenaHash = ", Depth);
            DebugPrint(S.ArenaHash, Depth+1);
            DebugPrint("\n");

            DebugPrint("HeadArenaHash = ", Depth);
            DebugPrint(S.HeadArenaHash, Depth+1);
            DebugPrint("\n");

            DebugPrint("StructSize = ", Depth);
            DebugPrint(S.StructSize, Depth+1);
            DebugPrint("\n");

            DebugPrint("StructCount = ", Depth);
            DebugPrint(S.StructCount, Depth+1);
            DebugPrint("\n");

            DebugPrint("PushCount = ", Depth);
            DebugPrint(S.PushCount, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(push_metadata* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("push_metadata\n", Depth);
            
              DebugPrint("Name = ", Depth);
              DebugPrint(S->Name, Depth+1);
              DebugPrint("\n");

              DebugPrint("ArenaHash = ", Depth);
              DebugPrint(S->ArenaHash, Depth+1);
              DebugPrint("\n");

              DebugPrint("HeadArenaHash = ", Depth);
              DebugPrint(S->HeadArenaHash, Depth+1);
              DebugPrint("\n");

              DebugPrint("StructSize = ", Depth);
              DebugPrint(S->StructSize, Depth+1);
              DebugPrint("\n");

              DebugPrint("StructCount = ", Depth);
              DebugPrint(S->StructCount, Depth+1);
              DebugPrint("\n");

              DebugPrint("PushCount = ", Depth);
              DebugPrint(S->PushCount, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(v2i S, u32 Depth)
        {
          DebugPrint("v2i\n", Depth);
          
            DebugPrint("x = ", Depth);
            DebugPrint(S.x, Depth+1);
            DebugPrint("\n");

            DebugPrint("y = ", Depth);
            DebugPrint(S.y, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(v2i* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("v2i\n", Depth);
            
              DebugPrint("x = ", Depth);
              DebugPrint(S->x, Depth+1);
              DebugPrint("\n");

              DebugPrint("y = ", Depth);
              DebugPrint(S->y, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(v2 S, u32 Depth)
        {
          DebugPrint("v2\n", Depth);
          
            DebugPrint("x = ", Depth);
            DebugPrint(S.x, Depth+1);
            DebugPrint("\n");

            DebugPrint("y = ", Depth);
            DebugPrint(S.y, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(v2* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("v2\n", Depth);
            
              DebugPrint("x = ", Depth);
              DebugPrint(S->x, Depth+1);
              DebugPrint("\n");

              DebugPrint("y = ", Depth);
              DebugPrint(S->y, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(heap_allocation_block S, u32 Depth)
        {
          DebugPrint("heap_allocation_block\n", Depth);
          
            DebugPrint("Type = ", Depth);
            DebugPrint(S.Type, Depth+1);
            DebugPrint("\n");

            DebugPrint("Size = ", Depth);
            DebugPrint(S.Size, Depth+1);
            DebugPrint("\n");

            DebugPrint("PrevAllocationSize = ", Depth);
            DebugPrint(S.PrevAllocationSize, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(heap_allocation_block* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("heap_allocation_block\n", Depth);
            
              DebugPrint("Type = ", Depth);
              DebugPrint(S->Type, Depth+1);
              DebugPrint("\n");

              DebugPrint("Size = ", Depth);
              DebugPrint(S->Size, Depth+1);
              DebugPrint("\n");

              DebugPrint("PrevAllocationSize = ", Depth);
              DebugPrint(S->PrevAllocationSize, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(heap_allocator S, u32 Depth)
        {
          DebugPrint("heap_allocator\n", Depth);
          
            DebugPrint("Heap = ", Depth);
            DebugPrint(S.Heap, Depth+1);
            DebugPrint("\n");

            DebugPrint("Size = ", Depth);
            DebugPrint(S.Size, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(heap_allocator* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("heap_allocator\n", Depth);
            
              DebugPrint("Heap = ", Depth);
              DebugPrint(S->Heap, Depth+1);
              DebugPrint("\n");

              DebugPrint("Size = ", Depth);
              DebugPrint(S->Size, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(mutex S, u32 Depth)
        {
          DebugPrint("mutex\n", Depth);
          
            DebugPrint("M = ", Depth);
            DebugPrint(S.M, Depth+1);
            DebugPrint("\n");

            DebugPrint("DebugRecord = ", Depth);
            DebugPrint(S.DebugRecord, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(mutex* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("mutex\n", Depth);
            
              DebugPrint("M = ", Depth);
              DebugPrint(S->M, Depth+1);
              DebugPrint("\n");

              DebugPrint("DebugRecord = ", Depth);
              DebugPrint(S->DebugRecord, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(mutex_op_record S, u32 Depth)
        {
          DebugPrint("mutex_op_record\n", Depth);
          
            DebugPrint("Mutex = ", Depth);
            DebugPrint(S.Mutex, Depth+1);
            DebugPrint("\n");

            DebugPrint("Cycle = ", Depth);
            DebugPrint(S.Cycle, Depth+1);
            DebugPrint("\n");

            DebugPrint("Op = ", Depth);
            DebugPrint(S.Op, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(mutex_op_record* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("mutex_op_record\n", Depth);
            
              DebugPrint("Mutex = ", Depth);
              DebugPrint(S->Mutex, Depth+1);
              DebugPrint("\n");

              DebugPrint("Cycle = ", Depth);
              DebugPrint(S->Cycle, Depth+1);
              DebugPrint("\n");

              DebugPrint("Op = ", Depth);
              DebugPrint(S->Op, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(mutex_op_array S, u32 Depth)
        {
          DebugPrint("mutex_op_array\n", Depth);
          
            DebugPrint("NextRecord = ", Depth);
            DebugPrint(S.NextRecord, Depth+1);
            DebugPrint("\n");

            DebugPrint("Records = ", Depth);
            DebugPrint(S.Records, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(mutex_op_array* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("mutex_op_array\n", Depth);
            
              DebugPrint("NextRecord = ", Depth);
              DebugPrint(S->NextRecord, Depth+1);
              DebugPrint("\n");

              DebugPrint("Records = ", Depth);
              DebugPrint(S->Records, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(tagged_counted_string_stream S, u32 Depth)
        {
          DebugPrint("tagged_counted_string_stream\n", Depth);
          
            DebugPrint("Tag = ", Depth);
            DebugPrint(S.Tag, Depth+1);
            DebugPrint("\n");

            DebugPrint("Stream = ", Depth);
            DebugPrint(S.Stream, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(tagged_counted_string_stream* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("tagged_counted_string_stream\n", Depth);
            
              DebugPrint("Tag = ", Depth);
              DebugPrint(S->Tag, Depth+1);
              DebugPrint("\n");

              DebugPrint("Stream = ", Depth);
              DebugPrint(S->Stream, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(char_cursor S, u32 Depth)
        {
          DebugPrint("char_cursor\n", Depth);
          
            DebugPrint("Start = ", Depth);
            DebugPrint(S.Start, Depth+1);
            DebugPrint("\n");

            DebugPrint("End = ", Depth);
            DebugPrint(S.End, Depth+1);
            DebugPrint("\n");

            DebugPrint("At = ", Depth);
            DebugPrint(S.At, Depth+1);
            DebugPrint("\n");

            DebugPrint("Memory = ", Depth);
            DebugPrint(S.Memory, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(char_cursor* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("char_cursor\n", Depth);
            
              DebugPrint("Start = ", Depth);
              DebugPrint(S->Start, Depth+1);
              DebugPrint("\n");

              DebugPrint("End = ", Depth);
              DebugPrint(S->End, Depth+1);
              DebugPrint("\n");

              DebugPrint("At = ", Depth);
              DebugPrint(S->At, Depth+1);
              DebugPrint("\n");

              DebugPrint("Memory = ", Depth);
              DebugPrint(S->Memory, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(linei S, u32 Depth)
        {
          DebugPrint("linei\n", Depth);
          
            DebugPrint("MinP = ", Depth);
            DebugPrint(S.MinP, Depth+1);
            DebugPrint("\n");

            DebugPrint("MaxP = ", Depth);
            DebugPrint(S.MaxP, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(linei* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("linei\n", Depth);
            
              DebugPrint("MinP = ", Depth);
              DebugPrint(S->MinP, Depth+1);
              DebugPrint("\n");

              DebugPrint("MaxP = ", Depth);
              DebugPrint(S->MaxP, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(line S, u32 Depth)
        {
          DebugPrint("line\n", Depth);
          
            DebugPrint("MinP = ", Depth);
            DebugPrint(S.MinP, Depth+1);
            DebugPrint("\n");

            DebugPrint("MaxP = ", Depth);
            DebugPrint(S.MaxP, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(line* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("line\n", Depth);
            
              DebugPrint("MinP = ", Depth);
              DebugPrint(S->MinP, Depth+1);
              DebugPrint("\n");

              DebugPrint("MaxP = ", Depth);
              DebugPrint(S->MaxP, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(XVisualInfo S, u32 Depth)
        {
          DebugPrint("XVisualInfo\n", Depth);
          
            DebugPrint("visual = ", Depth);
            DebugPrint(S.visual, Depth+1);
            DebugPrint("\n");

            DebugPrint("visualid = ", Depth);
            DebugPrint(S.visualid, Depth+1);
            DebugPrint("\n");

            DebugPrint("screen = ", Depth);
            DebugPrint(S.screen, Depth+1);
            DebugPrint("\n");

            DebugPrint("depth = ", Depth);
            DebugPrint(S.depth, Depth+1);
            DebugPrint("\n");

            DebugPrint("c_class = ", Depth);
            DebugPrint(S.c_class, Depth+1);
            DebugPrint("\n");

            DebugPrint("red_mask = ", Depth);
            DebugPrint(S.red_mask, Depth+1);
            DebugPrint("\n");

            DebugPrint("green_mask = ", Depth);
            DebugPrint(S.green_mask, Depth+1);
            DebugPrint("\n");

            DebugPrint("blue_mask = ", Depth);
            DebugPrint(S.blue_mask, Depth+1);
            DebugPrint("\n");

            DebugPrint("colormap_size = ", Depth);
            DebugPrint(S.colormap_size, Depth+1);
            DebugPrint("\n");

            DebugPrint("bits_per_rgb = ", Depth);
            DebugPrint(S.bits_per_rgb, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(XVisualInfo* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("XVisualInfo\n", Depth);
            
              DebugPrint("visual = ", Depth);
              DebugPrint(S->visual, Depth+1);
              DebugPrint("\n");

              DebugPrint("visualid = ", Depth);
              DebugPrint(S->visualid, Depth+1);
              DebugPrint("\n");

              DebugPrint("screen = ", Depth);
              DebugPrint(S->screen, Depth+1);
              DebugPrint("\n");

              DebugPrint("depth = ", Depth);
              DebugPrint(S->depth, Depth+1);
              DebugPrint("\n");

              DebugPrint("c_class = ", Depth);
              DebugPrint(S->c_class, Depth+1);
              DebugPrint("\n");

              DebugPrint("red_mask = ", Depth);
              DebugPrint(S->red_mask, Depth+1);
              DebugPrint("\n");

              DebugPrint("green_mask = ", Depth);
              DebugPrint(S->green_mask, Depth+1);
              DebugPrint("\n");

              DebugPrint("blue_mask = ", Depth);
              DebugPrint(S->blue_mask, Depth+1);
              DebugPrint("\n");

              DebugPrint("colormap_size = ", Depth);
              DebugPrint(S->colormap_size, Depth+1);
              DebugPrint("\n");

              DebugPrint("bits_per_rgb = ", Depth);
              DebugPrint(S->bits_per_rgb, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(os S, u32 Depth)
        {
          DebugPrint("os\n", Depth);
          
            DebugPrint("Window = ", Depth);
            DebugPrint(S.Window, Depth+1);
            DebugPrint("\n");

            DebugPrint("Display = ", Depth);
            DebugPrint(S.Display, Depth+1);
            DebugPrint("\n");

            DebugPrint("GlContext = ", Depth);
            DebugPrint(S.GlContext, Depth+1);
            DebugPrint("\n");

            DebugPrint("ContinueRunning = ", Depth);
            DebugPrint(S.ContinueRunning, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(os* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("os\n", Depth);
            
              DebugPrint("Window = ", Depth);
              DebugPrint(S->Window, Depth+1);
              DebugPrint("\n");

              DebugPrint("Display = ", Depth);
              DebugPrint(S->Display, Depth+1);
              DebugPrint("\n");

              DebugPrint("GlContext = ", Depth);
              DebugPrint(S->GlContext, Depth+1);
              DebugPrint("\n");

              DebugPrint("ContinueRunning = ", Depth);
              DebugPrint(S->ContinueRunning, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(native_file S, u32 Depth)
        {
          DebugPrint("native_file\n", Depth);
          
            DebugPrint("Handle = ", Depth);
            DebugPrint(S.Handle, Depth+1);
            DebugPrint("\n");

            DebugPrint("Path = ", Depth);
            DebugPrint(S.Path, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(native_file* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("native_file\n", Depth);
            
              DebugPrint("Handle = ", Depth);
              DebugPrint(S->Handle, Depth+1);
              DebugPrint("\n");

              DebugPrint("Path = ", Depth);
              DebugPrint(S->Path, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(plane_computation S, u32 Depth)
        {
          DebugPrint("plane_computation\n", Depth);
          
            DebugPrint("Plane = ", Depth);
            DebugPrint(S.Plane, Depth+1);
            DebugPrint("\n");

            DebugPrint("Complete = ", Depth);
            DebugPrint(S.Complete, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(plane_computation* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("plane_computation\n", Depth);
            
              DebugPrint("Plane = ", Depth);
              DebugPrint(S->Plane, Depth+1);
              DebugPrint("\n");

              DebugPrint("Complete = ", Depth);
              DebugPrint(S->Complete, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(bitmap_image_header S, u32 Depth)
        {
          DebugPrint("bitmap_image_header\n", Depth);
          
            DebugPrint("SizeOfImageHeader = ", Depth);
            DebugPrint(S.SizeOfImageHeader, Depth+1);
            DebugPrint("\n");

            DebugPrint("WidthInPixels = ", Depth);
            DebugPrint(S.WidthInPixels, Depth+1);
            DebugPrint("\n");

            DebugPrint("HeightInPixels = ", Depth);
            DebugPrint(S.HeightInPixels, Depth+1);
            DebugPrint("\n");

            DebugPrint("ColorPlanes = ", Depth);
            DebugPrint(S.ColorPlanes, Depth+1);
            DebugPrint("\n");

            DebugPrint("BPP = ", Depth);
            DebugPrint(S.BPP, Depth+1);
            DebugPrint("\n");

            DebugPrint("CompressionType = ", Depth);
            DebugPrint(S.CompressionType, Depth+1);
            DebugPrint("\n");

            DebugPrint("SizeInBytes = ", Depth);
            DebugPrint(S.SizeInBytes, Depth+1);
            DebugPrint("\n");

            DebugPrint("xPixelsPerMeter = ", Depth);
            DebugPrint(S.xPixelsPerMeter, Depth+1);
            DebugPrint("\n");

            DebugPrint("yPixelsPerMeter = ", Depth);
            DebugPrint(S.yPixelsPerMeter, Depth+1);
            DebugPrint("\n");

            DebugPrint("ColorMapsUsed = ", Depth);
            DebugPrint(S.ColorMapsUsed, Depth+1);
            DebugPrint("\n");

            DebugPrint("NumSignificantColors = ", Depth);
            DebugPrint(S.NumSignificantColors, Depth+1);
            DebugPrint("\n");

            DebugPrint("RedMask = ", Depth);
            DebugPrint(S.RedMask, Depth+1);
            DebugPrint("\n");

            DebugPrint("GreenMask = ", Depth);
            DebugPrint(S.GreenMask, Depth+1);
            DebugPrint("\n");

            DebugPrint("BlueMask = ", Depth);
            DebugPrint(S.BlueMask, Depth+1);
            DebugPrint("\n");

            DebugPrint("AlphaMask = ", Depth);
            DebugPrint(S.AlphaMask, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(bitmap_image_header* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("bitmap_image_header\n", Depth);
            
              DebugPrint("SizeOfImageHeader = ", Depth);
              DebugPrint(S->SizeOfImageHeader, Depth+1);
              DebugPrint("\n");

              DebugPrint("WidthInPixels = ", Depth);
              DebugPrint(S->WidthInPixels, Depth+1);
              DebugPrint("\n");

              DebugPrint("HeightInPixels = ", Depth);
              DebugPrint(S->HeightInPixels, Depth+1);
              DebugPrint("\n");

              DebugPrint("ColorPlanes = ", Depth);
              DebugPrint(S->ColorPlanes, Depth+1);
              DebugPrint("\n");

              DebugPrint("BPP = ", Depth);
              DebugPrint(S->BPP, Depth+1);
              DebugPrint("\n");

              DebugPrint("CompressionType = ", Depth);
              DebugPrint(S->CompressionType, Depth+1);
              DebugPrint("\n");

              DebugPrint("SizeInBytes = ", Depth);
              DebugPrint(S->SizeInBytes, Depth+1);
              DebugPrint("\n");

              DebugPrint("xPixelsPerMeter = ", Depth);
              DebugPrint(S->xPixelsPerMeter, Depth+1);
              DebugPrint("\n");

              DebugPrint("yPixelsPerMeter = ", Depth);
              DebugPrint(S->yPixelsPerMeter, Depth+1);
              DebugPrint("\n");

              DebugPrint("ColorMapsUsed = ", Depth);
              DebugPrint(S->ColorMapsUsed, Depth+1);
              DebugPrint("\n");

              DebugPrint("NumSignificantColors = ", Depth);
              DebugPrint(S->NumSignificantColors, Depth+1);
              DebugPrint("\n");

              DebugPrint("RedMask = ", Depth);
              DebugPrint(S->RedMask, Depth+1);
              DebugPrint("\n");

              DebugPrint("GreenMask = ", Depth);
              DebugPrint(S->GreenMask, Depth+1);
              DebugPrint("\n");

              DebugPrint("BlueMask = ", Depth);
              DebugPrint(S->BlueMask, Depth+1);
              DebugPrint("\n");

              DebugPrint("AlphaMask = ", Depth);
              DebugPrint(S->AlphaMask, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(bitmap_header S, u32 Depth)
        {
          DebugPrint("bitmap_header\n", Depth);
          
            DebugPrint("Type = ", Depth);
            DebugPrint(S.Type, Depth+1);
            DebugPrint("\n");

            DebugPrint("FileSizeInBytes = ", Depth);
            DebugPrint(S.FileSizeInBytes, Depth+1);
            DebugPrint("\n");

            DebugPrint("Ignored = ", Depth);
            DebugPrint(S.Ignored, Depth+1);
            DebugPrint("\n");

            DebugPrint("OffsetToPixelData = ", Depth);
            DebugPrint(S.OffsetToPixelData, Depth+1);
            DebugPrint("\n");

            DebugPrint("Image = ", Depth);
            DebugPrint(S.Image, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(bitmap_header* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("bitmap_header\n", Depth);
            
              DebugPrint("Type = ", Depth);
              DebugPrint(S->Type, Depth+1);
              DebugPrint("\n");

              DebugPrint("FileSizeInBytes = ", Depth);
              DebugPrint(S->FileSizeInBytes, Depth+1);
              DebugPrint("\n");

              DebugPrint("Ignored = ", Depth);
              DebugPrint(S->Ignored, Depth+1);
              DebugPrint("\n");

              DebugPrint("OffsetToPixelData = ", Depth);
              DebugPrint(S->OffsetToPixelData, Depth+1);
              DebugPrint("\n");

              DebugPrint("Image = ", Depth);
              DebugPrint(S->Image, Depth+1);
              DebugPrint("\n");

          }
        }



      
        function void DebugPrint(bitmap S, u32 Depth)
        {
          DebugPrint("bitmap\n", Depth);
          
            DebugPrint("Dim = ", Depth);
            DebugPrint(S.Dim, Depth+1);
            DebugPrint("\n");

            DebugPrint("Pixels = ", Depth);
            DebugPrint(S.Pixels, Depth+1);
            DebugPrint("\n");

        }


      
        function void DebugPrint(bitmap* S, u32 Depth)
        {
          if (S)
          {
            DebugPrint("bitmap\n", Depth);
            
              DebugPrint("Dim = ", Depth);
              DebugPrint(S->Dim, Depth+1);
              DebugPrint("\n");

              DebugPrint("Pixels = ", Depth);
              DebugPrint(S->Pixels, Depth+1);
              DebugPrint("\n");

          }
        }



