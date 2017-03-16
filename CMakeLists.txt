cmake_minimum_required (VERSION 3.0)

project(Bonsai)

set (Bonsai_VERSION_MAJOR 0)
set (Bonsai_VERSION_MINOR 1)

configure_file (
  "${PROJECT_SOURCE_DIR}/src/BonsaiConfig.h.in"
  "${PROJECT_SOURCE_DIR}/src/BonsaiConfig.h"
)

include_directories( "${PROJECT_SOUERC_DIR}/src/" )

add_executable(build "${PROJECT_SOURCE_DIR}/src/game.cpp")
