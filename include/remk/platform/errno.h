#ifndef REMK_PLATFORM_ERRNO_H
#define REMK_PLATFORM_ERRNO_H

#include <remk/platform/aaa_base.h>

#ifdef __cplusplus
extern "C" {
#endif

int remk_platform_get_last_error(void) REMK_PLATFORM_NOEXCEPT;

void remk_platform_set_last_error(int error_code) REMK_PLATFORM_NOEXCEPT;

#ifdef __cplusplus
}
#endif
#endif
