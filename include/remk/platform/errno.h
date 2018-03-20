#ifndef REMK_PLATFORM_ERRNO_HPP
#define REMK_PLATFORM_ERRNO_HPP

#include <remk/platform/aaa_base.h>

#ifdef __cplusplus
extern "C" {
#endif

REMK_PLATFORM_EXTERN int remk_platform_get_last_error(
    void) REMK_PLATFORM_NOEXCEPT;

REMK_PLATFORM_EXTERN void remk_platform_set_last_error(
    int error_code) REMK_PLATFORM_NOEXCEPT;

#ifdef __cplusplus
}
#endif
#endif
