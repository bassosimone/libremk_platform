#ifndef REMK_PLATFORM_SUGAR_H
#define REMK_PLATFORM_SUGAR_H

#include <remk/platform/aaa_base.h>

// Syntactic sugar APIs based on real platform APIs.

#ifdef __cplusplus
extern "C" {
#endif

double remk_platform_now(void) REMK_PLATFORM_NOEXCEPT;

#ifdef __cplusplus
}
#endif
#endif
