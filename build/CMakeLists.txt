set(CMAKE_C_COMPILER "/usr/bin/clang")
set(CMAKE_CXX_COMPILER "/usr/bin/clang++")

cmake_minimum_required (VERSION 3.0)

project(Bonsai)

set(Bonsai_VERSION_MAJOR 0)
set(Bonsai_VERSION_MINOR 1)

set(CMAKE_BUILD_TYPE Debug)
# set(CMAKE_BUILD_TYPE Release)

set( ROOT ${CMAKE_SOURCE_DIR}/.. )

set( EXAMPLES ${ROOT}/examples )
set( TESTS ${ROOT}/tests )
set( SRC ${ROOT}/src )
set( BIN ${ROOT}/bin )

# Engine include dirs
include_directories(${SRC})
include_directories(${SRC}/datatypes)

set( LIBRARY_OUTPUT_PATH ${BIN} )
set( EXECUTABLE_OUTPUT_PATH ${BIN} )

add_executable(Bonsai          ${SRC}/platform.cpp)
add_executable(ttf             ${SRC}/font/ttf.cpp)
add_executable(Server          ${SRC}/net/server.cpp)
add_executable(test_m4         ${TESTS}/m4.cpp)
add_executable(test_collada    ${TESTS}/colladaloader.cpp)
add_executable(test_bitmap     ${TESTS}/bitmap.cpp)
add_executable(test_chunk      ${TESTS}/chunk.cpp)
add_executable(test_string     ${TESTS}/bonsai_string.cpp)
add_executable(test_objloader  ${TESTS}/objloader.cpp)
add_executable(test_allocation ${TESTS}/allocation.cpp)
add_executable(test_callgraph  ${TESTS}/callgraph.cpp)
add_executable(test_heap_allocation ${TESTS}/heap_allocation.cpp)

target_include_directories(test_m4         PRIVATE ${TESTS})
target_include_directories(test_collada    PRIVATE ${TESTS})
target_include_directories(test_bitmap     PRIVATE ${TESTS})
target_include_directories(test_chunk      PRIVATE ${TESTS})
target_include_directories(test_string     PRIVATE ${TESTS})
target_include_directories(test_objloader  PRIVATE ${TESTS})
target_include_directories(test_allocation PRIVATE ${TESTS})
target_include_directories(test_callgraph  PRIVATE ${TESTS})
target_include_directories(test_heap_allocation  PRIVATE ${TESTS})

add_definitions( -DGL_GLEXT_PROTOTYPES=1 )

add_library(DebugSystem SHARED ${SRC}/debug_system/debug.cpp)
target_include_directories(DebugSystem PRIVATE ${SRC}/debug_system)

# add_library(SsaoTest SHARED ${EXAMPLES}/ssao_test/game.cpp)
# target_include_directories(SsaoTest PRIVATE ${EXAMPLES}/ssao_test)
# set_target_properties(SsaoTest PROPERTIES OUTPUT_NAME "SsaoTest")
# set_target_properties(SsaoTest PROPERTIES PREFIX "")
# set_target_properties(SsaoTest PROPERTIES SUFFIX "")

# add_library(AnimationTest SHARED ${EXAMPLES}/animation_test/game.cpp)
# target_include_directories(AnimationTest PRIVATE ${EXAMPLES}/animation_test)
# set_target_properties(AnimationTest PROPERTIES OUTPUT_NAME "AnimationTest")
# set_target_properties(AnimationTest PROPERTIES PREFIX "")
# set_target_properties(AnimationTest PROPERTIES SUFFIX "")

add_library(WorldGen SHARED ${EXAMPLES}/world_gen/game.cpp)
target_include_directories(WorldGen PRIVATE ${EXAMPLES}/world_gen)
set_target_properties(WorldGen PROPERTIES OUTPUT_NAME "WorldGen")
set_target_properties(WorldGen PROPERTIES PREFIX "")
set_target_properties(WorldGen PROPERTIES SUFFIX "")

# Link with Opengl
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS} )
target_link_libraries(Bonsai ${OPENGL_LIBRARIES})
# target_link_libraries(SsaoTest ${OPENGL_LIBRARIES})
# target_link_libraries(AnimationTest ${OPENGL_LIBRARIES})
target_link_libraries(WorldGen ${OPENGL_LIBRARIES})
target_link_libraries(test_callgraph ${OPENGL_LIBRARIES})

add_custom_target(run ${BIN}/Bonsai
                  DEPENDS Bonsai
                  COMMENT "Running Bonsai")

add_custom_target(tests ${BIN}/test_m4
                        ${BIN}/test_collada
                        ${BIN}/test_bitmap
                        ${BIN}/test_chunk
                        ${BIN}/test_string
                        ${BIN}/test_objloader
                        ${BIN}/test_allocation
                        ${BIN}/test_callgraph
                        ${BIN}/test_heap_allocation
                        COMMENT "Running M4 Tests")

target_compile_definitions(Bonsai               PRIVATE BONSAI_INTERNAL=1)
target_compile_definitions(Server               PRIVATE BONSAI_INTERNAL=1)
target_compile_definitions(DebugSystem          PRIVATE BONSAI_INTERNAL=1)

# target_compile_definitions(AnimationTest        PRIVATE BONSAI_INTERNAL=1)
# target_compile_definitions(SsaoTest             PRIVATE BONSAI_INTERNAL=1)
target_compile_definitions(WorldGen             PRIVATE BONSAI_INTERNAL=1)

target_compile_definitions(ttf                  PRIVATE BONSAI_INTERNAL=1)

target_compile_definitions(test_m4              PRIVATE BONSAI_INTERNAL=1)
target_compile_definitions(test_collada         PRIVATE BONSAI_INTERNAL=1)
target_compile_definitions(test_bitmap          PRIVATE BONSAI_INTERNAL=1)
target_compile_definitions(test_chunk           PRIVATE BONSAI_INTERNAL=1)
target_compile_definitions(test_string          PRIVATE BONSAI_INTERNAL=1)
target_compile_definitions(test_objloader       PRIVATE BONSAI_INTERNAL=1)
target_compile_definitions(test_allocation      PRIVATE BONSAI_INTERNAL=1)
target_compile_definitions(test_callgraph       PRIVATE BONSAI_INTERNAL=1)
target_compile_definitions(test_heap_allocation PRIVATE BONSAI_INTERNAL=1)



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

  find_package(Threads REQUIRED)
  find_package(X11 REQUIRED)

  target_link_libraries(Bonsai X11)                       # Xlib
  target_link_libraries(Bonsai ${CMAKE_THREAD_LIBS_INIT}) # -lpthread
  target_link_libraries(Bonsai ${CMAKE_DL_LIBS})          # .so loader

  target_link_libraries(test_callgraph X11)               # Xlib
  target_link_libraries(test_callgraph ${CMAKE_DL_LIBS})  # .so loader

elseif(WIN32)

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
add_definitions( -Wno-multichar )

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
  add_definitions( -Wno-zero-as-null-pointer-constant)
  add_definitions( -Wno-format-nonliteral )
  add_definitions( -Wno-cast-qual )
  add_definitions( -Wno-unused-function )
  add_definitions( -Wno-four-char-constants )


  add_definitions( -Wno-unused-variable )
  add_definitions( -Wno-unused-parameter )


  # TODO(Jesse): Should this be turned on again?
  add_definitions( -Wno-global-constructors )

else()
  message(FATAL_ERROR "Unsupported Compiler")
endif()

# Rename the Game lib once it's finished so the platform can re-load it

# get_property(ssaoTestBinary TARGET SsaoTest PROPERTY OUTPUT_NAME)
# add_custom_command( TARGET SsaoTest POST_BUILD
#                     COMMAND ${CMAKE_COMMAND} -E copy
#                     ../bin/${ssaoTestBinary} ../bin/${ssaoTestBinary}Loadable.so )

get_property(worldGenBinary TARGET WorldGen PROPERTY OUTPUT_NAME)
add_custom_command( TARGET WorldGen POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy
                    ../bin/${worldGenBinary} ../bin/${worldGenBinary}Loadable.so )

# get_property(animationTestBinary TARGET AnimationTest PROPERTY OUTPUT_NAME)
# add_custom_command( TARGET AnimationTest POST_BUILD
#                     COMMAND ${CMAKE_COMMAND} -E copy
#                     ../bin/${animationTestBinary} ../bin/${animationTestBinary}Loadable.so )


