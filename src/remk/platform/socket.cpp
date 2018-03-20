#include <remk/platform/context.hpp>
#include <remk/platform/socket.h>

using namespace remk::platform;

remk_platform_socket_t remk_platform_socket(int domain, int type,
                                            int protocol) noexcept {
  return Context::get_thread_local()->socket(domain, type, protocol);
}

int remk_platform_connect(remk_platform_socket_t handle, const sockaddr *saddr,
                          remk_platform_socklen_t len) noexcept {
  return Context::get_thread_local()->connect(handle, saddr, len);
}

remk_platform_ssize_t remk_platform_recv(remk_platform_socket_t handle,
                                         remk_platform_buffer_type_t *buffer,
                                         remk_platform_size_t count,
                                         int flags) noexcept {
  return Context::get_thread_local()->recv(handle, buffer, count, flags);
}

remk_platform_ssize_t
remk_platform_send(remk_platform_socket_t handle,
                   const remk_platform_buffer_type_t *buffer,
                   remk_platform_size_t count, int flags) noexcept {
  return Context::get_thread_local()->send(handle, buffer, count, flags);
}

int remk_platform_closesocket(remk_platform_socket_t handle) noexcept {
  return Context::get_thread_local()->closesocket(handle);
}
