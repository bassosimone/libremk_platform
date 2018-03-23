#ifndef REMK_PLATFORM_UIO_H
#define REMK_PLATFORM_UIO_H

#include <remk/platform/aaa_base.h>

#ifdef _WIN32
struct iovec {
    void *iov_base;
    remk_platform_size_t iov_len;
};
#define IOV_MAX 64
#endif

#ifdef __cplusplus
extern "C" {
#endif

remk_platform_ssize_t remk_platform_writev(remk_platform_socket_t socket,
      const struct iovec *iov, int iovcnt) REMK_PLATFORM_NOEXCEPT;

#ifdef __cplusplus
}
#endif
#endif
