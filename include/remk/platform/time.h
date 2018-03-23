#ifndef REMK_PLATFORM_TIME_H
#define REMK_PLATFORM_TIME_H

#include <remk/platform/aaa_base.h>

#ifdef __cplusplus
extern "C" {
#endif

int remk_platform_timespec_get(
      struct timespec *ts, int base) REMK_PLATFORM_NOEXCEPT;

#ifdef __cplusplus
}
#endif
#endif
