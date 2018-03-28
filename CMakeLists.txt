cmake_minimum_required(VERSION 3.0.0)
project(remk_platform CXX)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(REMK_PLATFORM_ABI_MAJOR 4)
set(REMK_PLATFORM_ABI_MINOR 0)
set(REMK_PLATFORM_ABI_PATCH 0)
set(REMK_PLATFORM_ABI ${REMK_PLATFORM_ABI_MAJOR}.${REMK_PLATFORM_ABI_MINOR}.${REMK_PLATFORM_ABI_PATCH})
if(UNIX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")
  if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wmissing-prototypes")
  endif()
endif()
add_library(
  remk_platform
  include/remk/platform.h
  include/remk/platform/context.h
  src/context.cpp
)
target_include_directories(
  remk_platform
  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)
set_target_properties(
  remk_platform
  PROPERTIES
  VERSION ${REMK_PLATFORM_ABI}
  SOVERSION ${REMK_PLATFORM_ABI_MAJOR}
)
install(
  DIRECTORY
  include/remk
  DESTINATION
  include
)
install(
  TARGETS
  remk_platform
  DESTINATION
  lib
)
if(WIN32)
  target_link_libraries(remk_platform ws2_32)
endif()
add_subdirectory(example)
