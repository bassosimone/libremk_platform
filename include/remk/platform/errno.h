#ifndef REMK_PLATFORM_ERRNO_H
#define REMK_PLATFORM_ERRNO_H

#ifdef _WIN32
#define REMK_PLATFORM_ERROR_NAME(name_) WSAE##name_
#else
#define REMK_PLATFORM_ERROR_NAME(name_) E##name_
#endif

#endif
