// src/engine/editor.cpp:155:0

link_internal void
DoEditorUi(renderer_2d *Ui, window_layout *Window, platform *Element, cs Name, ui_render_params *Params = &DefaultUiRenderParams_Generic)
{
  if (Element)
  {
    // NOTE(Jesse): This is wacky as fuck, but it's a pretty easy way to
    // support not drawing the toggl-y thing if we just want to dump the members.
    b32 DrawChildren = True;
    b32 DidToggle = False;
    if (Name.Count)
    {
      if (ToggleButton(Ui, FSz("v %S", Name), FSz("> %S", Name), UiId(Window, "toggle platform", Element), &DefaultUiRenderParams_Generic))
      {
        DidToggle = True;
        PushNewRow(Ui);
      }
      else
      {
        DrawChildren = False;
      }
    }

    if (DrawChildren)
    {
      PushTableStart(Ui);
      if (DidToggle) { OPEN_INDENT_FOR_TOGGLEABLE_REGION(); }
      DoEditorUi(Ui,
        Window,
&Element->LowPriority,
        CSz("LowPriority"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->HighPriority,
        CSz("HighPriority"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->HighPriorityWorkerCount,
        CSz("HighPriorityWorkerCount"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->HighPriorityModeFutex,
        CSz("HighPriorityModeFutex"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->WorkerThreadsSuspendFutex,
        CSz("WorkerThreadsSuspendFutex"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->WorkerThreadsExitFutex,
        CSz("WorkerThreadsExitFutex"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
        Element->Threads,
        CSz("Threads"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->MouseP,
        CSz("MouseP"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->MouseDP,
        CSz("MouseDP"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
        Element->Memory,
        CSz("Memory"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->GameTime,
        CSz("GameTime"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->dt,
        CSz("dt"),
        Params
        );





      PushNewRow(Ui);

      DoEditorUi(Ui,
        Window,
&Element->ScreenDim,
        CSz("ScreenDim"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
&Element->Input,
        CSz("Input"),
        Params
        );





      
      DoEditorUi(Ui,
        Window,
        Element->ServerState,
        CSz("ServerState"),
        Params
        );
      if (DidToggle) { CLOSE_INDENT_FOR_TOGGLEABLE_REGION(); }
      PushTableEnd(Ui);
    }
    else
    {
      PushNewRow(Ui);
    }

  }
  else
  {
    PushColumn(Ui, Name, Params);
    PushColumn(Ui, CSz("(null)"), Params);
    PushNewRow(Ui);
  }

}

