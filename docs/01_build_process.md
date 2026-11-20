# Building the Engine, Dependencies, and Examples

Building Bonsai is pretty straight-forward.  The main entry point for the build
is a shell script, `make.sh`.

The build script invokes `clang++`. CI does not pin a specific compiler
version. If the following instructions do not work for you, open an issue and
include your compiler version and platform.

## Dependencies

Install a Bash-compatible shell, clang++, and the platform dependencies
described in the [bonsai_stdlib dependency guide](https://github.com/scallyw4g/bonsai_stdlib/blob/master/docs/dependencies.md).
That guide may suggest a specific Clang version, but the Bonsai build script
and CI do not currently enforce one.

## Quickstart

```
git clone --recursive https://github.com/scallyw4g/bonsai bonsai && cd bonsai
./make.sh
```

## Build Options

Invoke the make script from the root directory by typing `./make.sh`.

By default, the script is configured to build everything that should build with
low resistance with `-O2`. This includes the loader, bundled examples, and
tests; Linux also builds debug-only tests.

The following options can be appended to the make script to control which targets are built.

* BuildExecutables

Builds `game_loader`, the entry point when running a game.

* BuildAll

Builds the loader, bundled examples, and tests. On Linux, also builds
debug-only tests.

* BuildBundledExamples

Builds all the examples bundled with the engine.

* BuildSingleExample

Builds a single example.  Can target a bundled example, or a custom game.  More
information on [02_create_new_project.md](02_create_new_project.md).  You may
pass this option multiple times.

* BuildTests

Builds the test executables listed in `make.sh`.

* BuildDebugOnlyTests

Builds tests that are only valid in debug mode, due to breaking in -O2 mode.

NOTE(Jesse): This is a long-standing kludge that should be removed, but it
requires writing an instruction decoder, which I was not smart enough to do
when I started the project.  I could do it now, but haven't gotten around to
it.

* RunTests

Runs the test executables already present in `bin/tests`. Build them first
with `BuildTests` or the default build.

* RunPoof

Runs the metaprogramming compiler, `poof`, if it is installed and on `PATH`.
See [poof](https://github.com/scallyw4g/poof) for details.

* BundleRelease

Packages the loader, selected examples, and assets into a platform-specific
release archive. Build the loader and bundled examples first.

* BuildWithEMCC

Builds the engine using emcc, targeting WASM.

NOTE(Jesse): This is currently broken, though it should be pretty easy to resurrect.

* -Od
* -O0
* -O1
* -O2
* -O3

Controls the optimization level passed to the compiler.
