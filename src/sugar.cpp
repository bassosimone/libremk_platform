#include <remk/platform/sugar.h>

#include <remk/platform/context.h>
#include <remk/platform/time.h>

#ifndef _WIN32
#include <fcntl.h>
#endif

double remk_platform_now() noexcept {
  timespec ts{};
  auto rv = remk_platform_timespec_get(&ts, TIME_UTC);
  if (rv != TIME_UTC) {
    return -1.0;
  }
  return (double)ts.tv_sec + (double)ts.tv_nsec / 1'000'000'000;
}

int remk_platform_setnonblocking(
      remk_platform_socket_t sock, int enable) noexcept {
  auto ctx = remk::platform::Context::get_thread_local();
#ifdef _WIN32
  unsigned long ul_enable = enable;
  if (ctx->system_ioctlsocket(sock, FIONBIO, &ul_enable) != 0) {
    return -1;
  }
#else
  auto flags = ctx->system_fcntl_int(sock, F_GETFL, 0);
  if (flags < 0) {
    return -1;
  }
  if (enable) {
    flags |= O_NONBLOCK;
  } else {
    flags &= ~O_NONBLOCK;
  }
  if (ctx->system_fcntl_int(sock, F_SETFL, flags) != 0) {
    return -1;
  }
#endif
  return 0;
}
