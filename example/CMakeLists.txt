set(REMK_PLATFORM_BUILD_EXAMPLES ON CACHE BOOL "Whether to build examples")
if(REMK_PLATFORM_BUILD_EXAMPLES)
  add_executable(http_client http_client.cpp)
  target_link_libraries(http_client remk_platform)
  add_test(http_client http_client)
endif()
