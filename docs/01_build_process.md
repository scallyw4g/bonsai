# Building the Engine, Dependencies, and Examples

Building Bonsai is pretty straight-forward.  The main entry point for the build
is a shell script, `make.sh`.

The build uses `clang++` with C++17 and x86 SIMD extensions. The macOS port has
been exercised with Apple clang 21. Different compiler versions may emit
additional warnings or errors; include the compiler version when reporting an issue.

## Dependencies

Follow the instructions for fetching dependencies for bonsai_stdlib [https://github.com/scallyw4g/bonsai_stdlib/blob/master/docs/dependencies.md](https://github.com/scallyw4g/bonsai_stdlib/blob/master/docs/dependencies.md)

## Quickstart

```
git clone --recursive https://github.com/scallyw4g/bonsai bonsai && cd bonsai
./make.sh
```

## macOS

Install Xcode or its Command Line Tools. The current SIMD implementation requires
an x86_64 target with AVX2. On Apple Silicon, use Rosetta 2 with AVX2 support;
this is not a native arm64 build.

From the repository root:

```sh
bash make.sh BuildAll GenerateCompileCommands -O2
bash make.sh RunTests
bin/game_loader bin/game_libs/terrain_gen_loadable.dylib
```

The build selects Objective-C++, Cocoa, and the system OpenGL framework. No
third-party windowing library is required.

macOS uses OpenGL 4.1 and GLSL 410, including after shader hot reload. Transform
and terrain-edit data use integer texture buffers; draw lists use direct draws
with an explicit transform index. Other desktop platforms retain the existing
GLSL 460, shader-storage-buffer, and multi-draw-indirect path. The macOS path
therefore has more CPU draw-call overhead.

The existing alpha limitations, including shadow mapping, asset loading, and
incomplete material features, still apply. Some bundled brush files report
deserialization errors at startup; terrain rendering still runs. Shader changes
finish queued terrain work before rebuilding the world, so switching can pause
while GPU readback and mesh jobs complete. Audio and Windows-specific profiler
counters are not implemented on macOS.

## C++ language server

`lsp.json` enables `clangd` for this repository. `GenerateCompileCommands` records
the actual compiler flags and unity-build entrypoints in `compile_commands.json`;
the database is local and ignored by Git. Regenerate it after changing build
flags or targets. Used alone, the option builds all targets.

Included `.cpp` files are not standalone translation units. Open an owning
entrypoint, such as `src/game_loader.cpp`, before navigating their symbols.


## Build Options

Envoke the make script from the root directory by typing `./make.sh`

By default, the script is configured to build everything that should build with
low resistance in release mode.

The following options can be appended to the make script to control which targets are built.

* BuildExecutables

Builds various standalone tools that the engine relies on, including the
game_loader, which is the entry point when running a game.

* BuildBundledExamples

Builds all the examples bundled with the engine.

* BuildSingleExample

Builds a single example.  Can target a bundled example, or a custom game.  More
information on [02_create_new_project.md](02_create_new_project.md).  You may
pass this option multiple times.

* BuildTests

Builds the test suites for the bonsai stdlib.

* BuildDebugOnlyTests

Builds tests that are only valid in debug mode, due to breaking in -O2 mode.

NOTE(Jesse): This is a long-standing kludge that should be removed, but it
requires writing an instruction decoder, which I was not smart enough to do
when I started the project.  I could do it now, but haven't gotten around to
it.

* RunTests

Runs the test suites

* BuildDebugSystem

Builds the debug system, which the engine uses to do performance and memory
allocation profiling.

* MakeDebugLibRelease

Builds and bundles the assets suitable for creating a debug system release,
which makes it easy to integrate into external projects that do not depend
on Bonsai or bonsai_stdlib.

* RunPoof

Runs the metaprogramming compiler, `poof`.  See [poof](https://github.com/scallyw4g/poof) for details.

* BuildWithEMCC

Builds the engine using emcc, targeting WASM.

NOTE(Jesse): This is currently broken, though it should be pretty easy to resurrect.

* -Od
* -O0
* -O1
* -O2

Controls the optimization level passed to the compiler.
