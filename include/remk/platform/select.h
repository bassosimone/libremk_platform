#ifndef REMK_PLATFORM_SELECT_H
#define REMK_PLATFORM_SELECT_H

#include <remk/platform/aaa_base.h>

#ifdef __cplusplus
extern "C" {
#endif

int remk_platform_select(int maxfd, fd_set *readset, fd_set *writeset,
      fd_set *exceptset, struct timeval *timeout) REMK_PLATFORM_NOEXCEPT;

#ifdef __cplusplus
}
#endif
#endif
