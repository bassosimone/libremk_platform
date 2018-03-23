#include <remk/platform/context.h>

#ifndef _WIN32
#include <fcntl.h>
#endif

#include <limits.h>

namespace remk {
namespace platform {

// TODO(bassosimone): make sure this works on iOS armv7s. For reference
// see https://github.com/measurement-kit/measurement-kit/issues/1319.
static thread_local Context *pointer_ = nullptr;

/*static*/ void Context::set_thread_local(Context *p) noexcept { pointer_ = p; }

/*static*/ Context *Context::get_thread_local() noexcept {
  static Context global_context_;
  return (pointer_ != nullptr) ? pointer_ : &global_context_;
}

// TODO(bassosimone): see whether we can use a monotonic clock here.
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

int Context::connect(remk_platform_socket_t handle, const sockaddr *saddr,
                     remk_platform_socklen_t salen) noexcept {
  return ::connect(handle, saddr, salen);
}

#ifdef _WIN32
int Context::system_recv(SOCKET handle, char *buffer, int count,
                         int flags) noexcept {
  return ::recv(handle, buffer, count, flags);
}
#else
ssize_t Context::system_recv(int handle, void *buffer, size_t count,
                             int flags) noexcept {
  return ::recv(handle, buffer, count, flags);
}
#endif

remk_platform_ssize_t Context::recv(remk_platform_socket_t handle,
                                    void *buffer,
                                    remk_platform_size_t count,
                                    int flags) noexcept {
#ifdef _WIN32
  if (count > INT_MAX) {
    WSASetLastError(WSAEINVAL);
    return -1;
  }
  return (remk_platform_ssize_t)system_recv(handle, (char *)buffer,
                                            (int)count, flags);
#else
  return system_recv(handle, buffer, count, flags);
#endif
}

#ifdef _WIN32
int Context::system_send(SOCKET handle, const char *buffer, int count,
                         int flags) noexcept {
  return ::send(handle, buffer, count, flags);
}
#else
ssize_t Context::system_send(int handle, const void *buffer, size_t count,
                             int flags) noexcept {
  return ::send(handle, buffer, count, flags);
}
#endif

remk_platform_ssize_t Context::send(remk_platform_socket_t handle,
                                    const void *buffer,
                                    remk_platform_size_t count,
                                    int flags) noexcept {
#ifdef _WIN32
  if (count > INT_MAX) {
    WSASetLastError(WSAEINVAL);
    return -1;
  }
  return (remk_platform_ssize_t)system_send(handle, (const char *)buffer,
                                            (int)count, flags);
#else
  return system_send(handle, buffer, count, flags);
#endif
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

#ifdef _WIN32
int Context::system_ioctlsocket(
      SOCKET s, long cmd, unsigned long *argp) noexcept {
  return ::ioctlsocket(s, cmd, argp);
}
#else
int Context::system_fcntl_int(int fd, int cmd, int arg) noexcept {
  return ::fcntl(fd, cmd, arg);
}
#endif

} // namespace platform
} // namespace remk
