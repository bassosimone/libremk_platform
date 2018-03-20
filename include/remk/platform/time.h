#ifndef REMK_PLATFORM_TIME_H
#define REMK_PLATFORM_TIME_H

#include <remk/platform/aaa_base.h>

#ifdef __cplusplus
extern "C" {
#endif

REMK_PLATFORM_EXTERN int remk_platform_gettimeofday(
    struct timeval *tv, struct timezone *tz) REMK_PLATFORM_NOEXCEPT;

#ifdef __cplusplus
}
#endif
#endif
