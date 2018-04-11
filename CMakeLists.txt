cmake_minimum_required(VERSION 3.1.0)
project(remk_platform LANGUAGES C CXX)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(REMK_PLATFORM_ABI_MAJOR 4)
set(REMK_PLATFORM_ABI_MINOR 0)
set(REMK_PLATFORM_ABI_PATCH 0)
set(REMK_PLATFORM_ABI ${REMK_PLATFORM_ABI_MAJOR}.${REMK_PLATFORM_ABI_MINOR}.${REMK_PLATFORM_ABI_PATCH})

include(CheckIncludeFiles)
include(CheckFunctionExists)
include(CheckLibraryExists)

# Assumption: if there is no strcasecmp() there is no strncasecmp().
check_function_exists(strcasecmp HAVE_STRCASECMP)
if(NOT (${HAVE_STRCASECMP}))
  list(APPEND REMK_PORTABLE_SOURCES "src/strcasecmp.c")
endif()

check_function_exists(strtonum HAVE_STRTONUM)
if(NOT (${HAVE_STRTONUM}))
  list(APPEND REMK_PORTABLE_SOURCES "src/strtonum.c")
endif()

if(${UNIX})
  set(REMK_FLAGS "-Wall -Wextra -Werror -pedantic")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${REMK_FLAGS} -Wmissing-prototypes")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${REMK_FLAGS}")
  if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    # for GCC, -Wmissing-prototypes only works for C/ObjC.
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wmissing-prototypes")
  endif()
endif()

add_library(
  remk_platform
  include/remk/platform.hpp
  src/platform.cpp
  src/strcasecmp.h
  src/strtonum.h
  src/utf8_decode.c
  src/utf8_decode.h
  ${REMK_PORTABLE_SOURCES}
)
target_include_directories(
  remk_platform
  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}
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

enable_testing()
add_subdirectory(example)
