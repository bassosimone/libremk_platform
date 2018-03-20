#ifndef INCLUDE_REMK_PLATFORM_AAA_BASE_H
#define INCLUDE_REMK_PLATFORM_AAA_BASE_H

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <errno.h>
#include <netdb.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#endif

#include <time.h>

#ifndef TIME_UTC
#define TIME_UTC 1
#endif

#if defined __cplusplus && __cplusplus >= 201103L
#define REMK_PLATFORM_NOEXCEPT noexcept
#elif defined __cplusplus
#define REMK_PLATFORM_NOEXCEPT throw()
#else
#define REMK_PLATFORM_NOEXCEPT /* Nothing */
#endif

#ifdef _WIN32
typedef SOCKET remk_platform_socket_t;
typedef char remk_platform_buffer_type_t;
typedef int remk_platform_size_t;
typedef int remk_platform_socklen_t;
typedef int remk_platform_ssize_t;
#else
typedef int remk_platform_socket_t;
typedef void remk_platform_buffer_type_t;
typedef size_t remk_platform_size_t;
typedef socklen_t remk_platform_socklen_t;
typedef ssize_t remk_platform_ssize_t;
#endif

#endif
