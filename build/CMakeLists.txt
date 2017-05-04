cmake_minimum_required (VERSION 3.0)

project(Bonsai)

set (Bonsai_VERSION_MAJOR 0)
set (Bonsai_VERSION_MINOR 1)

set(CMAKE_BUILD_TYPE Debug)

set( ROOT ${CMAKE_SOURCE_DIR}/.. )

set( SRC ${ROOT}/src )
set( BIN ${ROOT}/bin )

set( LIBRARY_OUTPUT_PATH ${BIN} )
set( EXECUTABLE_OUTPUT_PATH ${BIN} )

add_executable(Bonsai ${SRC}/platform.cpp)
add_library(Game SHARED ${SRC}/game.cpp)

# Link with Opengl
find_package(OpenGL REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS} )
target_link_libraries(Game ${OPENGL_LIBRARIES})
target_link_libraries(Bonsai ${OPENGL_LIBRARIES})

include_directories(${SRC})
include_directories(${ROOT}/external/glm-0.9.7.1)

add_definitions( -DGL_GLEXT_PROTOTYPES=1 )


#
# Platform specific
#

if (CYGWIN)
  message(FATAL_ERROR "Refer to the README for instructions to run with Cygwin")
endif()

# Link against Win32 libs we need
if (WIN32)

  add_definitions( -DBONSAI_WIN32=1 )

  target_link_libraries(Bonsai Gdi32)
  target_link_libraries(Bonsai User32)
  target_link_libraries(Bonsai Shell32)
  target_link_libraries(Bonsai Opengl32)

endif()


# Generate GDB debug info
if (UNIX)
  add_definitions( -DLINUX=1 -ggdb )
endif()





#
# Compiler specific
#

### GCC

if (${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)
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
  add_definitions( -MDd -EHsc -DMSVC=1 )

else()
  message(FATAL_ERROR "Unsupported Compiler")
endif()

#
# Platform Specific
#

# if (UNIX)
#
#   set(COMPILED_LIB ${BIN}/libGame.so)
#   set(LOADABLE_LIB ${BIN}/libGameLoadable.so)
#
# elseif(WIN32)
#
#   set(COMPILED_LIB ${BIN}/Debug/Game.dll)
#   set(LOADABLE_LIB ${BIN}/Debug/GameLoadable.dll)
#
#   # set(COMPILED_LIB ${BIN}/Release/Game.dll)
#   # set(LOADABLE_LIB ${BIN}/Release/GameLoadable.dll)
#
# else()
#
#   message(FATAL_ERROR "Unsupported Platform")
#
# endif()

# Rename the game library on completion so the hot-loading code doesn't try to
# load it before the compiler is finished
# add_custom_command( TARGET Game POST_BUILD
#                     COMMAND ${CMAKE_COMMAND} -E rename ${COMPILED_LIB} ${LOADABLE_LIB} )

