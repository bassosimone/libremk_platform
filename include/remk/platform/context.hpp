#ifndef REMK_PLATFORM_CONTEXT_HPP
#define REMK_PLATFORM_CONTEXT_HPP

#include <remk/platform/aaa_base.h>

namespace remk {
namespace platform {

class Context {
public:
  static void set_thread_local(Context *ctx) noexcept;

  static Context *get_thread_local() noexcept;

  virtual int timespec_get(timespec *ts, int base) noexcept;

  virtual int get_last_error() noexcept;

  virtual void set_last_error(int error_code) noexcept;

  virtual int getaddrinfo(const char *hostname, const char *servname,
                          const addrinfo *hints, addrinfo **res) noexcept;

  virtual void freeaddrinfo(addrinfo *aip) noexcept;

  virtual remk_platform_socket_t socket(int domain, int type,
                                        int protocol) noexcept;

  int connect(remk_platform_socket_t handle, const sockaddr *saddr,
              remk_platform_socklen_t len) noexcept;

  virtual remk_platform_ssize_t recv(remk_platform_socket_t handle,
                                     remk_platform_buffer_type_t *buffer,
                                     remk_platform_size_t count,
                                     int flags) noexcept;

  virtual remk_platform_ssize_t send(remk_platform_socket_t handle,
                                     const remk_platform_buffer_type_t *buffer,
                                     remk_platform_size_t count,
                                     int flags) noexcept;

  virtual int closesocket(remk_platform_socket_t handle) noexcept;

  virtual int select(int maxfd, fd_set *readset, fd_set *writeset,
                     fd_set *exceptset, timeval *timeout) noexcept;

  virtual ~Context() noexcept;
};

} // namespace platform
} // namespace remk
#endif
