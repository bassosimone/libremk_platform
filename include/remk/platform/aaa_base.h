#ifndef INCLUDE_REMK_PLATFORM_AAA_BASE_H
#define INCLUDE_REMK_PLATFORM_AAA_BASE_H

#ifdef _WIN32
#include <winsock2.h>
#include <time.h>
#else
#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <errno.h>
#include <netdb.h>
#include <unistd.h>
#endif

#if defined(_WIN32) && defined(REMK_PLATFORM_BUILD_DLL)
#define REMK_PLATFORM_EXTERN __declspec(dllexport)
#elif defined(_WIN32) && defined(REMK_PLATFORM_USE_DLL)
#define REMK_PLATFORM_EXTERN __declspec(dllimport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#define REMK_PLATFORM_EXTERN __attribute__((visibility("default")))
#else
#define REMK_PLATFORM_EXTERN /* Nothing. */
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

#ifdef _WIN32
struct timezone {
  int tz_minuteswest;
  int tz_dsttime;
};
#endif
#endif
