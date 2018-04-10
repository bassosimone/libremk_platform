/*
 * Public domain, 2018 Simone Basso.
 */
#ifndef SRC_STRTONUM_H
#define SRC_STRTONUM_H

#ifdef __cplusplus
extern "C" {
#endif

long long
strtonum(const char *numstr, long long minval, long long maxval,
    const char **errstrp);

#ifdef __cplusplus
}
#endif
#endif
