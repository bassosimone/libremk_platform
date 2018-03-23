#include <remk/platform/sugar.h>

#ifndef _WIN32
#include <fcntl.h>
#endif

#include <assert.h>
#include <string.h>

#include <iomanip>
#include <sstream>

#include <remk/platform/context.h>
#include <remk/platform/errno.h>
#include <remk/platform/time.h>

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

int remk_platform_sockaddr_ntop(const sockaddr *sa, std::string *address,
      std::string *port) noexcept {
  if (sa == nullptr || address == nullptr || port == nullptr) {
    remk_platform_set_last_error(REMK_PLATFORM_ERROR_NAME(INVAL));
    return -1;
  }
  remk_platform_socklen_t len = 0;
  switch (sa->sa_family) {
  case AF_INET:
    len = sizeof (sockaddr_in);
    break;
  case AF_INET6:
    len = sizeof (sockaddr_in6);
    break;
  default:
    remk_platform_set_last_error(REMK_PLATFORM_ERROR_NAME(INVAL));
    return -1;
  }
  int flags = NI_NUMERICHOST | NI_NUMERICSERV;
  char a[NI_MAXHOST], p[NI_MAXSERV];
  if (::getnameinfo(sa, len, a, sizeof (a), p, sizeof (p), flags) != 0) {
    remk_platform_set_last_error(REMK_PLATFORM_ERROR_NAME(INVAL));
    return -1;
  }
  *address = a;
  *port = p;
  return 0;
}

int remk_platform_sockaddr_pton(const char *address, const char *port,
      sockaddr_storage *sst) noexcept {
  if (address == nullptr || port == nullptr || sst == nullptr) {
    remk_platform_set_last_error(REMK_PLATFORM_ERROR_NAME(INVAL));
    return -1;
  }
  memset(sst, 0, sizeof (*sst));
  addrinfo *rp = nullptr;
  addrinfo hints{};
  hints.ai_flags |= AI_NUMERICHOST | AI_NUMERICSERV;
  if (::getaddrinfo(address, port, &hints, &rp) != 0) {
    remk_platform_set_last_error(REMK_PLATFORM_ERROR_NAME(INVAL));
    return -1;
  }
  assert(rp != nullptr && rp->ai_addr && rp->ai_addrlen == sizeof (*sst));
  memcpy(sst, rp->ai_addr, rp->ai_addrlen);
  ::freeaddrinfo(rp);
  return 0;
}

std::string remk_platform_hexdump(const void *data, size_t count) noexcept {
  std::stringstream ss;
  if (data != nullptr) {
    const unsigned char *cbase = (const unsigned char *)data;
    for (size_t i = 0; i < count; ++i) {
      ss << std::hex << std::setfill('0') << std::setw(2)
         << (unsigned int)cbase[i];
      if (i < count - 1) { // safe b/c min(count) == 1 in this loop
        ss << " ";
      }
    }
  }
  return ss.str();
}
