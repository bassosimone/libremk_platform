/*
 * Public domain, 2018 Simone Basso.
 */
#ifndef SRC_STRCASECMP_H
#define SRC_STRCASECMP_H

#ifdef __cplusplus
extern "C" {
#endif

int
strcasecmp(const char *s1, const char *s2);

int
strncasecmp(const char *s1, const char *s2, size_t n);

#ifdef __cplusplus
}
#endif
#endif
