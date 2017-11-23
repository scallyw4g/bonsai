
cmake_minimum_required (VERSION 3.0)

project(Bonsai)

set(Bonsai_VERSION_MAJOR 0)
set(Bonsai_VERSION_MINOR 1)

set(CMAKE_BUILD_TYPE Debug)

set( ROOT ${CMAKE_SOURCE_DIR}/.. )

set( EXAMPLES ${ROOT}/examples )
set( SRC ${ROOT}/src )
set( BIN ${ROOT}/bin )

set( LIBRARY_OUTPUT_PATH ${BIN} )
set( EXECUTABLE_OUTPUT_PATH ${BIN} )

add_executable(Bonsai ${SRC}/platform.cpp)

# Link with Opengl
find_package(OpenGL REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS} )
target_link_libraries(Bonsai ${OPENGL_LIBRARIES})

# Engine include dirs
include_directories(${SRC})
include_directories(${ROOT}/external/glm-0.9.7.1)

add_definitions( -DGL_GLEXT_PROTOTYPES=1 )


################################ USER INCLUDES ################################

#set( GAME_SRC_DIR ${EXAMPLES}/space_invaders )
set( GAME_SRC_DIR ${EXAMPLES}/ssao_test )

###############################################################################

include_directories(${GAME_SRC_DIR}/engine_constants)

add_library(Game SHARED ${GAME_SRC_DIR}/game.cpp)
target_link_libraries(Game ${OPENGL_LIBRARIES})
target_include_directories(Game PRIVATE ${GAME_SRC_DIR})

add_custom_target(run /usr/bin/optirun ${BIN}/Bonsai
                  DEPENDS Bonsai
                  COMMENT "Running Bonsai")


add_definitions( -DDEBUG=1 -DBONSAI_SLOW=1 )

#
# Platform specific
#

if (CYGWIN)
  message(FATAL_ERROR "Cygwin Unsupported")
endif()

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




if (UNIX)

  set(GAME_COMPILED_LIB ${BIN}/libGame.so)
  set(GAME_LOADABLE_LIB ${BIN}/libGameLoadable.so)

elseif(WIN32)

  set(GAME_COMPILED_LIB ${BIN}/Debug/Game.dll)
  set(GAME_LOADABLE_LIB ${BIN}/Debug/GameLoadable.dll)

else()

  message(FATAL_ERROR "Unsupported Platform")

endif()

add_custom_command( TARGET Game POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy
                    ${GAME_COMPILED_LIB} ${GAME_LOADABLE_LIB} )
#
# Compiler specific
#

### GCC

if (${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)
  message(STATUS "Compiling with g++")

  add_definitions( -Wall )

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
