cmake_minimum_required (VERSION 3.0)

project(Bonsai)

set (Bonsai_VERSION_MAJOR 0)
set (Bonsai_VERSION_MINOR 1)

# Build executable to /build
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)

add_executable(Bonsai ${PROJECT_SOURCE_DIR}/src/game.cpp)

configure_file (
  ${PROJECT_SOURCE_DIR}/src/BonsaiConfig.h.in
  ${PROJECT_SOURCE_DIR}/src/BonsaiConfig.h
)

include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/external/glm-0.9.7.1)




find_package(OpenGL REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS} )
target_link_libraries(Bonsai ${OPENGL_LIBRARIES} )



add_subdirectory( external/glfw )
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)


add_definitions( -DGLEW_BUILD=GLEW_STATIC )

# Platform specific

if (${CMAKE_CXX_COMPILER_ID} STREQUAL Clang)    # LLVM
  # Does Clang work?

elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)  # g++
  add_definitions( -DLINUX=1 )
  add_definitions( -lGLEW -lGLU -lGL )
  target_link_libraries( Bonsai glfw )

elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
  add_definitions( -D_WIN32=1 )

  add_definitions( /MDd /EHsc /NODEFAULTLIB:LIBCMTD.lib )

  target_link_libraries(Bonsai Gdi32.lib)
  target_link_libraries(Bonsai User32.lib)
  target_link_libraries(Bonsai Shell32.lib)
  target_link_libraries(Bonsai Opengl32.lib)

  target_link_libraries(Bonsai ${PROJECT_SOURCE_DIR}/build/external/glfw/src/Debug/glfw3.lib)

  # this is only valid/necessary on VS2015 because MS refactored the CRT, which
  # leaves GLFW broken for the moment
  # target_link_libraries(Bonsai legacy_stdio_definitions.lib)

endif()


