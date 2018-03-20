#include <remk/platform/context.hpp>

namespace remk {
namespace platform {

static thread_local Context *pointer_ = nullptr;

/*static*/ void Context::set_thread_local(Context *p) noexcept {
  pointer_ = p;
}

/*static*/ Context *Context::get_thread_local() noexcept {
  static Context global_context_;
  return (pointer_ != nullptr) ? pointer_ : &global_context_;
}

int Context::timespec_get(timespec *ts, int base) noexcept {
#ifdef _WIN32
  return ::timespec_get(ts, base);
#else
  if (ts == nullptr || base != TIME_UTC) {
    errno = EINVAL;
    return 0;
  }
  timeval tv{};
  if (::gettimeofday(&tv, nullptr) != 0) {
    return 0;
  }
  ts->tv_sec = tv.tv_sec;
  ts->tv_nsec = tv.tv_usec * 1'000;
  return TIME_UTC;
#endif
}

int Context::get_last_error() noexcept {
#ifdef _WIN32
  return GetLastError();
#else
  return errno;
#endif
}

void Context::set_last_error(int error_code) noexcept {
#ifdef _WIN32
  SetLastError(error_code);
#else
  errno = error_code;
#endif
}

int Context::getaddrinfo(const char *hostname, const char *servname,
      const addrinfo *hints, addrinfo **res) noexcept {
  return ::getaddrinfo(hostname, servname, hints, res);
}

void Context::freeaddrinfo(addrinfo *aip) noexcept {
  return ::freeaddrinfo(aip);
}

remk_platform_socket_t Context::socket(int domain, int type,
      int protocol) noexcept {
  return ::socket(domain, type, protocol);
}

remk_platform_ssize_t Context::recv(remk_platform_socket_t handle,
      remk_platform_buffer_type_t *buffer, remk_platform_size_t count,
      int flags) noexcept {
  return ::recv(handle, buffer, count, flags);
}

remk_platform_ssize_t Context::send(remk_platform_socket_t handle,
      const remk_platform_buffer_type_t *buffer, remk_platform_size_t count,
      int flags) noexcept {
  return ::send(handle, buffer, count, flags);
}

int Context::closesocket(remk_platform_socket_t handle) noexcept {
#ifdef _WIN32
  return ::closesocket(handle);
#else
  return ::close(handle);
#endif
}

int Context::select(int maxfd, fd_set *readset, fd_set *writeset,
      fd_set *exceptset, timeval *timeout) noexcept {
  return ::select(maxfd, readset, writeset, exceptset, timeout);
}

Context::~Context() noexcept {}

} // namespace platform
} // namespace remk
