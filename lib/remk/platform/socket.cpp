#include <remk/platform/socket.h>
#include <remk/platform/context.hpp>

using namespace remk::platform;

remk_platform_socket_t remk_platform_socket(int domain, int type,
    int protocol) noexcept {
  return Context::get_thread_local()->socket(domain, type, protocol);
}

remk_platform_ssize_t remk_platform_recv(remk_platform_socket_t handle,
    remk_platform_buffer_type_t *buffer, remk_platform_size_t count,
    int flags) noexcept {
  return Context::get_thread_local()->recv(handle, buffer, count, flags);
}

remk_platform_ssize_t remk_platform_send(remk_platform_socket_t handle,
    const remk_platform_buffer_type_t *buffer, remk_platform_size_t count,
    int flags) noexcept {
  return Context::get_thread_local()->send(handle, buffer, count, flags);
}

int remk_platform_closesocket(remk_platform_socket_t handle) noexcept {
  return Context::get_thread_local()->closesocket(handle);
}
