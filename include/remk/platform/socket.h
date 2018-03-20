#ifndef REMK_PLATFORM_SOCKET_H
#define REMK_PLATFORM_SOCKET_H

#include <remk/platform/aaa_base.h>

#ifdef __cplusplus
extern "C" {
#endif

REMK_PLATFORM_EXTERN remk_platform_socket_t remk_platform_socket(int domain,
    int type, int protocol) REMK_PLATFORM_NOEXCEPT;

REMK_PLATFORM_EXTERN remk_platform_ssize_t remk_platform_recv(
    remk_platform_socket_t handle, remk_platform_buffer_type_t *buffer,
    remk_platform_size_t count, int flags) REMK_PLATFORM_NOEXCEPT;

REMK_PLATFORM_EXTERN remk_platform_ssize_t remk_platform_send(
    remk_platform_socket_t handle, const remk_platform_buffer_type_t *buffer,
    remk_platform_size_t count, int flags) REMK_PLATFORM_NOEXCEPT;

REMK_PLATFORM_EXTERN int remk_platform_closesocket(
    remk_platform_socket_t handle) REMK_PLATFORM_NOEXCEPT;

#ifdef __cplusplus
}
#endif
#endif
