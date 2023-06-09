# Building the Engine, Dependencies, and Examples

Building Bonsai is pretty straight-forward.  The main entry point for the build
is a shell script, `make.sh`.

NOTE: The officially supported compiler is clang-15.  Different versions may
work, but may also emit warnings, or errors.  If the following instructions do
not work for you, by all means open an issue and I will do what I can to assist.

## On Windows:
Bonsai requires `clang` and a unix-like shell to build on Windows.  Installing `git-bash` usually works for me.

## On Linux:
Ensure you have an OpenGL driver, GL headers, X11 headers, and clang

### Ubuntu
`sudo apt install freeglut3-dev libx11-dev clang-15`

### Archlinux
`sudo pacman -S freeglut libx11 clang` # NOTE(Jesse): Not sure how to specifically install clang 15 on Arch

### Common

```
git clone --recursive https://github.com/scallyw4g/bonsai bonsai && cd bonsai
./scripts/make.sh
```


## Build Options

Envoke the make script from the root directory by typing `./make.sh`

By default, the script is setup to build everything that should build with low
resistance in release mode.

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
