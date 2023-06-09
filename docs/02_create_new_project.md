# Creating a New Project with Bonsai

Creating a new project with Bonsai is pretty straight-forward.

## Setup the blank_project template

* Fork the repo
* Copy the contents of `examples/blank_project` to `examples/your_shiny_new_project_name`
* Profit

## Setup your build

* Create a `your_shiny_new_project_name.make.sh` script
* From that script, you can call `./make.sh BuildSingleExample examples/your_shiny_new_project_name`


## NOTES

It should be possible to put your game in any directory on your system and use
the bonsai directory as a library.  I haven't yet gotten to this level of build
hygiene, but this is a near/medium term goal for the build.
