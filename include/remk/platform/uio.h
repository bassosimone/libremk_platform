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

#endif
