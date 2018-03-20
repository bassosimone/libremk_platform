cmake_minimum_required(VERSION 3.0.0)
project(remk_platform)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if(UNIX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")
endif()
add_library(
  remk_platform
  include/remk/platform/time.h
  include/remk/platform/context.hpp
  include/remk/platform/aaa_base.h
  include/remk/platform/netdb.h
  include/remk/platform/errno.h
  include/remk/platform/socket.h
  include/remk/platform/select.h
  lib/remk/platform/select.cpp
  lib/remk/platform/socket.cpp
  lib/remk/platform/context.cpp
  lib/remk/platform/errno.cpp
  lib/remk/platform/netdb.cpp
  lib/remk/platform/time.cpp
)
target_include_directories(
  remk_platform
  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)
install(
  DIRECTORY
  include/remk/platform
  DESTINATION
  include/remk/platform
)
install(
  TARGETS
  remk_platform
  DESTINATION
  lib
)
