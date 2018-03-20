#ifndef REMK_PLATFORM_SELECT_HPP
#define REMK_PLATFORM_SELECT_HPP

#include <remk/platform/aaa_base.h>

#ifdef __cplusplus
extern "C" {
#endif

REMK_PLATFORM_EXTERN int remk_platform_select(int maxfd, fd_set *readset,
    fd_set *writeset, fd_set *exceptset,
    struct timeval *timeout) REMK_PLATFORM_NOEXCEPT;

#ifdef __cplusplus
}
#endif
#endif
