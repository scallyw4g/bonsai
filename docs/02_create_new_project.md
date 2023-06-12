# Creating a New Project with Bonsai

Creating a new project with Bonsai is pretty straight-forward.

## Setup the blank_project template

* Fork the repo
* Copy the contents of `examples/blank_project` to `examples/shiny_new_project`

## Setup your build

* Create a script called `shiny_new_project.make.sh` and add the following commands:

```
#! /usr/bin/env bash

./make.sh BuildExecutables BuildDebugSystem BuildSingleExample examples/shiny_new_project
```

Take a look at [01_build_process.md](01_build_process.md) for details on all the
switches the `make.sh` script accepts


## Run your project

The build should have created `bin/game_loader` (.exe on Windows) and
`bin/shiny_new_project` (.dll on Windows).  At a CLI, you can run your game
with `bin/game_loader(.exe) bin/shiny_new_project(.dll)`


## NOTES

It should be possible to put your game in any directory on your system and use
the bonsai directory as a library.  I haven't yet gotten to this level of build
hygiene, but this is a near/medium term goal for the build.
