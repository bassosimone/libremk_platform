#ifndef REMK_PLATFORM_SUGAR_H
#define REMK_PLATFORM_SUGAR_H

#include <remk/platform/aaa_base.h>

#ifdef __cplusplus
#include <string>
#endif

// Syntactic sugar APIs based on real platform APIs.

#ifdef __cplusplus
extern "C" {
#endif

double remk_platform_now(void) REMK_PLATFORM_NOEXCEPT;

int remk_platform_setnonblocking(
      remk_platform_socket_t sock, int enable) REMK_PLATFORM_NOEXCEPT;

#ifdef __cplusplus
int remk_platform_sockaddr_ntop(const sockaddr *sa, std::string *address,
      std::string *port) noexcept;
#endif

int remk_platform_sockaddr_pton(const char *address, const char *port,
      sockaddr_storage *sst) noexcept;

#ifdef __cplusplus
}
#endif
#endif
