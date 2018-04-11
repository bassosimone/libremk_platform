/*
 * Public domain, 2018 Simone Basso.
 */
#ifndef SRC_THIRD_PARTY_UTF8_DECODE_H
#define SRC_THIRD_PARTY_UTF8_DECODE_H

#include <stdint.h>

#define UTF8_ACCEPT 0
#define UTF8_REJECT 1

#ifdef __cplusplus
extern "C" {
#endif

uint32_t remk_platform_utf8_decode(
      uint32_t *state, uint32_t *codep, uint32_t byte);

#ifdef __cplusplus
}
#endif
#endif
