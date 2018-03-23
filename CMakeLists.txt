cmake_minimum_required(VERSION 3.0.0)
project(remk_platform CXX)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(REMK_PLATFORM_ABI_MAJOR 2)
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
  include/remk/platform/aaa_base.h
  include/remk/platform/context.h
  include/remk/platform/errno.h
  include/remk/platform/netdb.h
  include/remk/platform/platform.h
  include/remk/platform/select.h
  include/remk/platform/socket.h
  include/remk/platform/sugar.h
  include/remk/platform/time.h
  include/remk/platform/uio.h
  src/context.cpp
  src/errno.cpp
  src/netdb.cpp
  src/select.cpp
  src/socket.cpp
  src/sugar.cpp
  src/time.cpp
  src/uio.cpp
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
  include/remk/platform
  DESTINATION
  include/remk
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
