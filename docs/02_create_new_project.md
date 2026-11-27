# Creating a New Project with Bonsai

Creating a new project with Bonsai is pretty straight-forward.

## Setup the blank_project template

* Fork the repo
* Copy the contents of `examples/blank_project` to `examples/shiny_new_project`

## Setup your build

* Create a script called `shiny_new_project.make.sh` and add the following commands:

```
#! /usr/bin/env bash

./make.sh BuildExecutables BuildSingleExample examples/shiny_new_project
```

Take a look at [01_build_process.md](01_build_process.md) for details on all the
switches the `make.sh` script accepts. Run the new script from the repository
root, for example with `bash shiny_new_project.make.sh`.


## Run your project

The build creates the loader and a loadable project library under
`bin/game_libs`. From the repository root, run:

Windows

```text
./bin/game_loader.exe ./bin/game_libs/shiny_new_project_loadable.dll
```

Linux:

```sh
./bin/game_loader ./bin/game_libs/shiny_new_project_loadable.so
```


### NOTES on using MSYS2

When launching MSYS2, make sure to launch the `MSYS2 CLANG64` variant.  It has
`/clang64/bin` added to your path.  If you launch another variant, you'll want
to add `PATH=$PATH:/clang64/bin` to your `~/.bashrc` file.  I do this in any case.

The C drive is mounted at `/c`.  You can access your users files at `/c/users/<username>`.


## NOTES on using WSL2

Using WSL2 does nominally work, though it is both too slow to be usable and
presents with visual artifacting.  Since I got it working at one point, I'll
leave notes here in case they're useful in the future.

As far as I can tell, you must force WSL to target OpenGL 4.6 by adding the
following environment variables to your .bashrc

```sh
#export MESA_GL_VERSION_OVERRIDE=4.6
#export MESA_GLSL_VERSION_OVERRIDE=460
```

I also had to give the MESA driver a hint to use my NVIDIA GPU, instead of the
integrated Intel graphics adapter.

```sh
export MESA_D3D12_DEFAULT_ADAPTER_NAME=NVIDIA
```


