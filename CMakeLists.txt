cmake_minimum_required (VERSION 3.0)

project(Bonsai)

set (Bonsai_VERSION_MAJOR 0)
set (Bonsai_VERSION_MINOR 1)

set(CMAKE_BUILD_TYPE Debug)

# Build executable to /build
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build)


add_executable(Bonsai ${PROJECT_SOURCE_DIR}/src/platform.cpp)
add_library(Game SHARED ${PROJECT_SOURCE_DIR}/src/game.cpp)

# Link with Opengl
find_package(OpenGL REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS} )
target_link_libraries(Game ${OPENGL_LIBRARIES})
target_link_libraries(Bonsai ${OPENGL_LIBRARIES})

include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/external/glm-0.9.7.1)

add_definitions( -DGL_GLEXT_PROTOTYPES=1 )

if (CYGWIN OR WIN32)

  set(WIN32 1)
  add_definitions(-D_WIN32=1 -D_CRT_SECURE_NO_WARNGINS=1)

  target_link_libraries(Bonsai Gdi32)
  target_link_libraries(Bonsai User32)
  target_link_libraries(Bonsai Shell32)

else()
  add_definitions( -DLINUX=1 -ggdb )
endif()





# Platform specific

if (${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)  # gcc
  message(STATUS "Compiling with g++")

  add_definitions( -std=c++11 -Wall )

  find_package(Threads REQUIRED)
  find_package(X11 REQUIRED)

  # Xlib
  target_link_libraries(Bonsai X11)

  # -lpthread
  target_link_libraries(Bonsai ${CMAKE_THREAD_LIBS_INIT})

  # SO loader
  target_link_libraries(Bonsai ${CMAKE_DL_LIBS})

elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
  message(STATUS "Compiling with MSVC")
  add_definitions( /MDd /EHsc -DMSVC=1 )

else()
  message(FATAL_ERROR "Unsupported Compiler")
endif()


