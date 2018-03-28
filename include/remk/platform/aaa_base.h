#ifndef INCLUDE_REMK_PLATFORM_AAA_BASE_H
#define INCLUDE_REMK_PLATFORM_AAA_BASE_H

#ifdef _WIN32
#include <basetsd.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <errno.h>
#include <netdb.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#endif

#include <time.h>

#ifndef TIME_UTC
#define TIME_UTC 1
#endif

#ifdef _WIN32
struct iovec {
    void *iov_base;
    SIZE_T iov_len;
};
#define IOV_MAX 64
#endif

#endif
