
# Getting Started

## Prebuilt Binaries

The fastest way to get started with Bonsai is to grab the prebuilt binaries from
the [Releases](https://github.com/scallyw4g/bonsai/releases) page. Windows and
Linux are supported; releases may be marked as pre-releases.

Download the archive for your platform and extract it, keeping the extracted
files and directories together, including the hidden `.root_marker` file.

## Running an Example

Bonsai ships with pre-built example projects under `bin/game_libs`. Open a
terminal in the extracted directory and run the loader:

Windows

```text
.\bin\game_loader.exe
```

Linux:

```sh
./bin/game_loader
```

Select an example from the project picker. Alternatively, pass the example
library as an argument to run it directly:

Windows

```text
.\bin\game_loader.exe bin\game_libs\terrain_gen_loadable.dll
```

Linux:

```sh
./bin/game_loader ./bin/game_libs/terrain_gen_loadable.so
```

The loader searches for `.root_marker` in the current directory and its parent
directories. Run it from the extracted directory (or a directory beneath it).
For navigation and editing, see [Controls](controls.md) and [Editor](editor.md).
To build from source, see [Build From Source](01_build_process.md).

