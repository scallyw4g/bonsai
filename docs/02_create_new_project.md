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

Windows:

```text
bin\game_loader.exe bin\game_libs\shiny_new_project_loadable.dll
```

Linux:

```sh
./bin/game_loader ./bin/game_libs/shiny_new_project_loadable.so
```


## NOTES

It should be possible to put your game in any directory on your system and use
the bonsai directory as a library.  I haven't yet gotten to this level of build
hygiene, but this is a near/medium term goal for the build.


