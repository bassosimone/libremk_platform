#include <remk/platform/context.h>
#include <remk/platform/socket.h>

using namespace remk::platform;

int remk_platform_wsainit() noexcept {
    return Context::get_thread_local()->wsainit();
}

remk_platform_socket_t remk_platform_socket(
      int domain, int type, int protocol) noexcept {
    return Context::get_thread_local()->socket(domain, type, protocol);
}

int remk_platform_connect(remk_platform_socket_t handle, const sockaddr *saddr,
      remk_platform_socklen_t len) noexcept {
    return Context::get_thread_local()->connect(handle, saddr, len);
}

#ifdef _WIN32
int remk_platform_system_recv(
      SOCKET handle, char *buffer, int count, int flags) noexcept {
    return Context::get_thread_local()->system_recvfrom(
          handle, buffer, count, flags, nullptr, nullptr);
}
#else
ssize_t remk_platform_system_recv(
      int handle, void *buffer, size_t count, int flags) noexcept {
    return Context::get_thread_local()->system_recvfrom(
          handle, buffer, count, flags, nullptr, nullptr);
}
#endif

remk_platform_ssize_t remk_platform_recv(remk_platform_socket_t handle,
      void *buffer, remk_platform_size_t count, int flags) noexcept {
    return Context::get_thread_local()->recvfrom(
          handle, buffer, count, flags, nullptr, nullptr);
}

#ifdef _WIN32
int remk_platform_system_recvfrom(SOCKET handle, char *buffer, int count,
      int flags, struct sockaddr *sa, int *len) noexcept {
    return Context::get_thread_local()->system_recvfrom(
          handle, buffer, count, flags, sa, len);
}
#else
ssize_t remk_platform_system_recvfrom(int handle, void *buffer, size_t count,
      int flags, struct sockaddr *sa, socklen_t *len) noexcept {
    return Context::get_thread_local()->system_recvfrom(
          handle, buffer, count, flags, sa, len);
}
#endif

remk_platform_ssize_t remk_platform_recvfrom(remk_platform_socket_t handle,
      void *buffer, remk_platform_size_t count, int flags, sockaddr *sa,
      remk_platform_socklen_t *len) noexcept {
    return Context::get_thread_local()->recvfrom(
          handle, buffer, count, flags, sa, len);
}

#ifdef _WIN32
int remk_platform_system_send(
      SOCKET handle, const char *buffer, int count, int flags) noexcept {
    return Context::get_thread_local()->system_sendto(
          handle, buffer, count, flags, nullptr, 0);
}
#else
ssize_t remk_platform_system_send(
      int handle, const void *buffer, size_t count, int flags) noexcept {
    return Context::get_thread_local()->system_sendto(
          handle, buffer, count, flags, nullptr, 0);
}
#endif

remk_platform_ssize_t remk_platform_send(remk_platform_socket_t handle,
      const void *buffer, remk_platform_size_t count, int flags) noexcept {
    return Context::get_thread_local()->sendto(
          handle, buffer, count, flags, nullptr, 0);
}

#ifdef _WIN32
int remk_platform_system_sendto(SOCKET handle, const char *buffer, int count,
      int flags, const struct sockaddr *sa, int len) noexcept {
    return Context::get_thread_local()->system_sendto(
          handle, buffer, count, flags, sa, len);
}
#else
ssize_t remk_platform_system_sendto(int handle, const void *buffer,
      size_t count, int flags, const struct sockaddr *sa,
      socklen_t len) noexcept {
    return Context::get_thread_local()->system_sendto(
          handle, buffer, count, flags, sa, len);
}
#endif

remk_platform_ssize_t remk_platform_sendto(remk_platform_socket_t handle,
      const void *buffer, remk_platform_size_t count, int flags,
      const sockaddr *sa, remk_platform_socklen_t len) noexcept {
    return Context::get_thread_local()->sendto(
          handle, buffer, count, flags, sa, len);
}

int remk_platform_closesocket(remk_platform_socket_t handle) noexcept {
    return Context::get_thread_local()->closesocket(handle);
}
