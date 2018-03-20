#ifndef REMK_PLATFORM_SOCKET_H
#define REMK_PLATFORM_SOCKET_H

#include <remk/platform/aaa_base.h>

#ifdef __cplusplus
extern "C" {
#endif

remk_platform_socket_t remk_platform_socket(int domain,
    int type, int protocol) REMK_PLATFORM_NOEXCEPT;

int remk_platform_connect(remk_platform_socket_t handle,
    const struct sockaddr *saddr,
    remk_platform_socklen_t len) REMK_PLATFORM_NOEXCEPT;

remk_platform_ssize_t remk_platform_recv(
    remk_platform_socket_t handle, remk_platform_buffer_type_t *buffer,
    remk_platform_size_t count, int flags) REMK_PLATFORM_NOEXCEPT;

remk_platform_ssize_t remk_platform_send(
    remk_platform_socket_t handle, const remk_platform_buffer_type_t *buffer,
    remk_platform_size_t count, int flags) REMK_PLATFORM_NOEXCEPT;

int remk_platform_closesocket(
    remk_platform_socket_t handle) REMK_PLATFORM_NOEXCEPT;

#ifdef __cplusplus
}
#endif
#endif
