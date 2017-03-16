cmake_minimum_required (VERSION 3.0)

project(Bonsai)

set (Bonsai_VERSION_MAJOR 0)
set (Bonsai_VERSION_MINOR 1)

configure_file (
  ${PROJECT_SOURCE_DIR}/src/BonsaiConfig.h.in
  ${PROJECT_SOURCE_DIR}/src/BonsaiConfig.h
)

include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/external/glm-0.9.7.1)

add_subdirectory( glfw )

add_executable(Bonsai ${PROJECT_SOURCE_DIR}/src/game.cpp)

add_definitions( -DGLEW_BUILD=GLEW_STATIC )

# Platform specific

if (${CMAKE_CXX_COMPILER_ID} STREQUAL Clang)    # LLVM
  # Does Clang work?

elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)  # g++
  add_definitions( -DLINUX=1 )
  add_definitions( -lglfw -lGLEW -lGLU -lGL )


elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
  add_definitions( -D_WIN32=1 )

  add_definitions( /MDd /EHsc /NODEFAULTLIB:LIBCMTD.lib )

  target_link_libraries(Bonsai ${PROJECT_SOURCE_DIR}/lib/glfw3.lib)
  target_link_libraries(Bonsai Gdi32.lib)
  target_link_libraries(Bonsai User32.lib)
  target_link_libraries(Bonsai Shell32.lib)
  target_link_libraries(Bonsai Opengl32.lib)

  # target_link_libraries(Bonsai vcruntime.lib)

  # this is only valid/necessary on VS2015 because MS refactored the CRT, which
  # leaves GLFW broken for the moment
  # target_link_libraries(Bonsai legacy_stdio_definitions.lib)

endif()




