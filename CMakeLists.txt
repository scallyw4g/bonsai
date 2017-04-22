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
  add_definitions(-D_WIN32=1 )

  target_link_libraries(Bonsai Gdi32)
  target_link_libraries(Bonsai User32)
  target_link_libraries(Bonsai Shell32)

else()
  add_definitions( -DLINUX=1 -ggdb )
  add_definitions( -lGL )
endif()





# Platform specific

if (${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)  # g++
  message(STATUS "Compiling with g++")

  find_package(Threads)
  target_link_libraries(Bonsai ${CMAKE_THREAD_LIBS_INIT})
  target_link_libraries(Bonsai ${CMAKE_DL_LIBS})

  add_definitions( -std=c++11 -lpthread )

elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
  message(STATUS "Compiling with MSVC")

  add_definitions( /MDd /EHsc )
  add_definitions( -DMSVC=1 )

else()

  message(FATAL_ERROR "Unsupported Compiler")

endif()


