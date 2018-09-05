
cmake_minimum_required (VERSION 3.0)

project(Bonsai)

set(Bonsai_VERSION_MAJOR 0)
set(Bonsai_VERSION_MINOR 1)

set(CMAKE_BUILD_TYPE Debug)

set( ROOT ${CMAKE_SOURCE_DIR}/.. )

set( EXAMPLES ${ROOT}/examples )
set( TESTS ${ROOT}/tests )
set( SRC ${ROOT}/src )
set( BIN ${ROOT}/bin )

# Engine include dirs
include_directories(${SRC})
include_directories(${SRC}/datatypes)
include_directories(${TESTS})

set( LIBRARY_OUTPUT_PATH ${BIN} )
set( EXECUTABLE_OUTPUT_PATH ${BIN} )

add_executable(Bonsai ${SRC}/platform.cpp)
add_executable(Server ${SRC}/net/server.cpp)
add_executable(test_m4 ${TESTS}/m4.cpp)
add_executable(test_chunk ${TESTS}/chunk.cpp)
add_executable(test_string ${TESTS}/string.cpp)
add_executable(test_objloader ${TESTS}/objloader.cpp)
add_executable(test_allocation ${TESTS}/allocation.cpp)
add_executable(test_callgraph ${TESTS}/callgraph.cpp)

add_definitions( -DGL_GLEXT_PROTOTYPES=1 )

################################ USER INCLUDES ################################

#set( GAME_SRC_DIR ${EXAMPLES}/space_invaders )
set( GAME_SRC_DIR ${EXAMPLES}/ssao_test )

###############################################################################

include_directories(${GAME_SRC_DIR})

add_library(Game SHARED ${GAME_SRC_DIR}/game.cpp)
target_include_directories(Game PRIVATE ${GAME_SRC_DIR})

# Link with Opengl
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS} )
target_link_libraries(Bonsai ${OPENGL_LIBRARIES})
target_link_libraries(Game ${OPENGL_LIBRARIES})
target_link_libraries(Server ${OPENGL_LIBRARIES})
target_link_libraries(test_m4 ${OPENGL_LIBRARIES})
target_link_libraries(test_chunk ${OPENGL_LIBRARIES})
target_link_libraries(test_string ${OPENGL_LIBRARIES})
target_link_libraries(test_objloader ${OPENGL_LIBRARIES})
target_link_libraries(test_allocation ${OPENGL_LIBRARIES})
target_link_libraries(test_callgraph ${OPENGL_LIBRARIES})

add_custom_target(run /usr/bin/optirun ${BIN}/Bonsai
                  DEPENDS Bonsai
                  COMMENT "Running Bonsai")

add_custom_target(tests ${BIN}/test_m4
                        ${BIN}/test_chunk
                        ${BIN}/test_string
                        ${BIN}/test_objloader
                        ${BIN}/test_allocation
                        ${BIN}/test_callgraph
                        COMMENT "Running M4 Tests")

target_compile_definitions(Bonsai  PUBLIC BONSAI_INTERNAL=1)
target_compile_definitions(Server  PUBLIC BONSAI_INTERNAL=1)
target_compile_definitions(Game    PUBLIC BONSAI_INTERNAL=1)
target_compile_definitions(test_m4 PUBLIC BONSAI_INTERNAL=1)
target_compile_definitions(test_chunk PUBLIC BONSAI_INTERNAL=1)
target_compile_definitions(test_string PUBLIC BONSAI_INTERNAL=1)
target_compile_definitions(test_objloader PUBLIC BONSAI_INTERNAL=1)
target_compile_definitions(test_allocation PUBLIC BONSAI_INTERNAL=1)
target_compile_definitions(test_callgraph PUBLIC BONSAI_INTERNAL=1)



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

add_definitions( -std=c++11 )

if (UNIX)

  set(GAME_COMPILED_LIB ${BIN}/libGame.so)
  set(GAME_LOADABLE_LIB ${BIN}/libGameLoadable.so)


  find_package(Threads REQUIRED)
  find_package(X11 REQUIRED)

  target_link_libraries(Bonsai X11)                       # Xlib
  target_link_libraries(Bonsai ${CMAKE_THREAD_LIBS_INIT}) # -lpthread
  target_link_libraries(Bonsai ${CMAKE_DL_LIBS})          # .so loader

  target_link_libraries(test_chunk X11)                       # Xlib
  target_link_libraries(test_chunk ${CMAKE_THREAD_LIBS_INIT}) # -lpthread
  target_link_libraries(test_chunk ${CMAKE_DL_LIBS})          # .so loader

  target_link_libraries(test_m4 X11)                       # Xlib
  target_link_libraries(test_m4 ${CMAKE_THREAD_LIBS_INIT}) # -lpthread
  target_link_libraries(test_m4 ${CMAKE_DL_LIBS})          # .so loader

  target_link_libraries(test_string X11)                       # Xlib
  target_link_libraries(test_string ${CMAKE_THREAD_LIBS_INIT}) # -lpthread
  target_link_libraries(test_string ${CMAKE_DL_LIBS})          # .so loader

  target_link_libraries(test_objloader X11)                       # Xlib
  target_link_libraries(test_objloader ${CMAKE_THREAD_LIBS_INIT}) # -lpthread
  target_link_libraries(test_objloader ${CMAKE_DL_LIBS})          # .so loader

  target_link_libraries(test_callgraph X11)                       # Xlib
  target_link_libraries(test_callgraph ${CMAKE_THREAD_LIBS_INIT}) # -lpthread
  target_link_libraries(test_callgraph ${CMAKE_DL_LIBS})          # .so loader


elseif(WIN32)

  set(GAME_COMPILED_LIB ${BIN}/Debug/Game.dll)
  set(GAME_LOADABLE_LIB ${BIN}/Debug/GameLoadable.dll)

else()

  message(FATAL_ERROR "Unsupported Platform")

endif()

#
# Compiler specific
#

### GCC


add_definitions( -Wpedantic )
add_definitions( -Wno-undef )
add_definitions( -Wno-old-style-cast )
add_definitions( -Wno-float-equal )
add_definitions( -Wno-padded )
add_definitions( -Wno-double-promotion )
add_definitions( -Wno-switch-enum )

# TODO(Jesse): Do an alignment pass on allocator.
add_definitions( -Wno-cast-align )

# TODO(Jesse): Should this be turned on again?
add_definitions( -Wno-sign-conversion )


if (${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)
  message(STATUS "Compiling with g++")
elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
  message(STATUS "Compiling with MSVC")
  add_definitions( -MDd -EHsc -DMSVC=1 )
elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL Clang)
  message(STATUS "Compiling with Clang")
  add_definitions( -Weverything )
  add_definitions( -Wno-c++98-compat-pedantic )
  add_definitions( -Wno-gnu-anonymous-struct )
  add_definitions( -Wno-missing-prototypes )
  # TODO(Jesse): Should this be turned on again?
  add_definitions( -Wno-global-constructors )
else()
  message(FATAL_ERROR "Unsupported Compiler")
endif()

# Rename the Game lib once it's finished so the platform can re-load it
add_custom_command( TARGET Game POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy
                    ${GAME_COMPILED_LIB} ${GAME_LOADABLE_LIB} )
