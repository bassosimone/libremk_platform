#include <remk/platform/context.h>
#include <remk/platform/socket.h>

using namespace remk::platform;

int remk_platform_wsainit() noexcept {
#ifdef _WIN32
    WORD version = MAKEWORD(2, 2);
    WSADATA wsadata;
    if (WSAStartup(version, &wsadata) != 0) {
        return -1;
    }
#endif
    return 0;
}

remk_platform_socket_t remk_platform_socket(
      int domain, int type, int protocol) noexcept {
    return Context::get_thread_local()->socket(domain, type, protocol);
}

int remk_platform_connect(remk_platform_socket_t handle, const sockaddr *saddr,
      remk_platform_socklen_t len) noexcept {
    return Context::get_thread_local()->connect(handle, saddr, len);
}

remk_platform_ssize_t remk_platform_recv(remk_platform_socket_t handle,
      void *buffer, remk_platform_size_t count, int flags) noexcept {
    return Context::get_thread_local()->recvfrom(
          handle, buffer, count, flags, nullptr, nullptr);
}

remk_platform_ssize_t remk_platform_recvfrom(remk_platform_socket_t handle,
      void *buffer, remk_platform_size_t count, int flags, sockaddr *sa,
      remk_platform_socklen_t *len) noexcept {
    return Context::get_thread_local()->recvfrom(
          handle, buffer, count, flags, sa, len);
}

remk_platform_ssize_t remk_platform_send(remk_platform_socket_t handle,
      const void *buffer, remk_platform_size_t count, int flags) noexcept {
    return Context::get_thread_local()->sendto(
          handle, buffer, count, flags, nullptr, 0);
}

remk_platform_ssize_t remk_platform_sendto(remk_platform_socket_t handle,
      const void *buffer, remk_platform_size_t count, int flags,
      const sockaddr *sa, remk_platform_socklen_t len) noexcept {
    return Context::get_thread_local()->sendto(
          handle, buffer, count, flags, sa, len);
}

int remk_platform_closesocket(remk_platform_socket_t handle) noexcept {
    return Context::get_thread_local()->closesocket(handle);
}
