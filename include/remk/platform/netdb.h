#ifndef REMK_PLATFORM_NETDB_HPP
#define REMK_PLATFORM_NETDB_HPP

#include <remk/platform/aaa_base.h>

#ifdef __cplusplus
extern "C" {
#endif

int remk_platform_getaddrinfo(const char *hostname, const char *servname,
                              const struct addrinfo *hints,
                              struct addrinfo **res) REMK_PLATFORM_NOEXCEPT;

void remk_platform_freeaddrinfo(struct addrinfo *aip) REMK_PLATFORM_NOEXCEPT;

#ifdef __cplusplus
}
#endif
#endif
