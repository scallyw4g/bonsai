cmake_minimum_required (VERSION 3.0)

project(Bonsai)

set (Bonsai_VERSION_MAJOR 0)
set (Bonsai_VERSION_MINOR 1)

set(CMAKE_BUILD_TYPE Debug)

set( ROOT ${CMAKE_SOURCE_DIR}/.. )

set( EXAMPLES ${ROOT}/examples )
set( SRC ${ROOT}/src )
set( BIN ${ROOT}/bin )

set( LIBRARY_OUTPUT_PATH ${BIN} )
set( EXECUTABLE_OUTPUT_PATH ${BIN} )

set( SPACE_INVADERS_PATH ${EXAMPLES}/space_invaders )
set( SSAO_TEST_PATH ${EXAMPLES}/ssao_test )

add_executable(Bonsai ${SRC}/platform.cpp)
add_library(example_space_invaders SHARED ${SPACE_INVADERS_PATH}/game.cpp)
add_library(example_ssao_test SHARED ${SSAO_TEST_PATH}/game.cpp)

# Link with Opengl
find_package(OpenGL REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS} )
target_link_libraries(example_space_invaders ${OPENGL_LIBRARIES})
target_link_libraries(example_ssao_test ${OPENGL_LIBRARIES})
target_link_libraries(Bonsai ${OPENGL_LIBRARIES})

include_directories(${SRC})
include_directories(${SPACE_INVADERS_PATH})
include_directories(${SSAO_TEST_PATH})
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

if (UNIX)

  set(SPACE_INVADERS_COMPILED_LIB ${BIN}/libexample_space_invaders.so)
  set(SPACE_INVADERS_LOADABLE_LIB ${BIN}/libGameLoadable.so)

  set(SSAO_COMPILED_LIB ${BIN}/libexample_ssao_test.so)
  set(SSAO_LOADABLE_LIB ${BIN}/libGameLoadable.so)

elseif(WIN32)

  set(SPACE_INVADERS_COMPILED_LIB ${BIN}/Debug/Game.dll)
  set(SPACE_INVADERS_LOADABLE_LIB ${BIN}/Debug/GameLoadable.dll)

else()

  message(FATAL_ERROR "Unsupported Platform")

endif()

# Rename the game library on completion so the hot-loading code doesn't try to
# load it before the compiler is finished
# add_custom_command( TARGET example_space_invaders POST_BUILD
#  COMMAND ${CMAKE_COMMAND} -E copy ${SPACE_INVADERS_COMPILED_LIB} ${SPACE_INVADERS_LOADABLE_LIB} )

add_custom_command( TARGET example_ssao_test POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy ${SSAO_COMPILED_LIB} ${SSAO_LOADABLE_LIB} )

