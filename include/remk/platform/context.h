#ifndef REMK_PLATFORM_CONTEXT_H
#define REMK_PLATFORM_CONTEXT_H
#ifdef __cplusplus

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

  virtual int connect(remk_platform_socket_t handle, const sockaddr *saddr,
                      remk_platform_socklen_t len) noexcept;

#ifdef _WIN32
  virtual int system_recvfrom(SOCKET handle, char *buffer, int count,
                          int flags, sockaddr *addr, int *len) noexcept;
#else
  virtual ssize_t system_recvfrom(int handle, void *buffer, size_t count,
                              int flags, sockaddr *addr, socklen_t *len)
                                  noexcept;
#endif

  virtual remk_platform_ssize_t recvfrom(remk_platform_socket_t handle,
                                     void *buffer,
                                     remk_platform_size_t count,
                                     int flags,
                                     sockaddr *addr,
                                     remk_platform_socklen_t *len) noexcept;

#ifdef _WIN32
  virtual int system_sendto(SOCKET handle, const char *buffer, int count,
                         int flags, const sockaddr *addr, int len) noexcept;
#else
  virtual ssize_t system_sendto(int handle, const void *buffer, size_t count,
                             int flags, const sockaddr *addr, socklen_t len)
                                noexcept;
#endif

  virtual remk_platform_ssize_t sendto(remk_platform_socket_t handle,
                                    const void *buffer,
                                    remk_platform_size_t count,
                                    int flags,
                                    const sockaddr *addr,
                                    remk_platform_socklen_t len) noexcept;

  virtual int closesocket(remk_platform_socket_t handle) noexcept;

  virtual int select(int maxfd, fd_set *readset, fd_set *writeset,
                     fd_set *exceptset, timeval *timeout) noexcept;

#ifdef _WIN32
  virtual int system_ioctlsocket(
      SOCKET s, long cmd, unsigned long *argp) noexcept;
#else
  virtual int system_fcntl_int(int fd, int cmd, int arg) noexcept;
#endif

  virtual ~Context() noexcept;
};

} // namespace platform
} // namespace remk
#endif
#endif
